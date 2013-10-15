// ResizablePropertySheet.cpp : implementation file
//

#include "../stdafx.h"
#include "ResizablePropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct {
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
	// 后面的属性不能直接用，要根据具体的情况分析,是变长结构
	// ...
} DLGTEMPLATEEX;

/////////////////////////////////////////////////////////////////////////////
// CResizablePropertySheet

IMPLEMENT_DYNAMIC(CResizablePropertySheet, CMFCPropertySheet)

CResizablePropertySheet::CResizablePropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CResizablePropertySheet::CResizablePropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CResizablePropertySheet::~CResizablePropertySheet()
{
}


BEGIN_MESSAGE_MAP(CResizablePropertySheet, CMFCPropertySheet)
	//{{AFX_MSG_MAP(CResizablePropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizablePropertySheet message handlers

BOOL CResizablePropertySheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	NMHDR *pNMHDR = (NMHDR*)lParam;

	BOOL bResult = CMFCPropertySheet::OnNotify(wParam, lParam, pResult);

	if ( pNMHDR->code == TCN_SELCHANGE && pNMHDR->idFrom == 0x3020 )
	{
		Recalculatelayout();		
	}

	return bResult;
}

HRESULT CResizablePropertySheet::RecalculatelayoutCustomControl()
{	
	return S_OK;
}

HRESULT CResizablePropertySheet::Recalculatelayout()
{
	CTabCtrl *pTabCtrl = GetTabControl();

	CRect rectActivePage;
	CRect rectSheet;
	CRect rectTabCtrl;

	int nPageWidth = 0;
	int nPageHeight = 0;

	int nIndex = pTabCtrl->GetCurSel();
	CPropertyPage *pActivePage = GetPage(nIndex);
	BOOL bActivePageCreated = FALSE;

	PROPSHEETPAGE *p = (PROPSHEETPAGE*)&pActivePage->m_psp;
	bActivePageCreated = pActivePage && pActivePage->GetSafeHwnd();

	DLGTEMPLATE* pTemplate;
	WORD *pDlgTemplateTemp = NULL;
	
	LPCTSTR lpszName = MAKEINTRESOURCE( p->pszTemplate );
	HINSTANCE hInstance = AfxFindResourceHandle( lpszName, RT_DIALOG ); 
	HRSRC hResource = ::FindResource( hInstance, lpszName, RT_DIALOG );
	HGLOBAL hTemplate = LoadResource( hInstance, hResource);

	pTemplate = (DLGTEMPLATE*)(LPCDLGTEMPLATE)LockResource(hTemplate);
	// 判断是哪一种 dlg 模板
	pDlgTemplateTemp = (WORD*)pTemplate;
	if ( pDlgTemplateTemp[1] == 0xFFFF )
	{
		// 扩展模板
		DLGTEMPLATEEX *pTemplateEx = (DLGTEMPLATEEX*)pTemplate;
		nPageWidth = pTemplateEx->cx;
		nPageHeight = pTemplateEx->cy;

	}
	else
	{
		nPageWidth = pTemplate->cx;
		nPageHeight = pTemplate->cy;
	}

	//
	
	UnlockResource(hTemplate);

	// dialog模板使用的是 dialog unit, 需要转换成 pixel unit 
	CRect rectTemp( 0, 0, nPageWidth, nPageHeight);
	::MapDialogRect( GetSafeHwnd(), &rectTemp );

	nPageWidth = rectTemp.Width();
	nPageHeight = rectTemp.Height();
	//
	rectActivePage = CRect( m_SheetData.page.nLeftMargin, m_SheetData.page.nTopMargin, m_SheetData.page.nLeftMargin + nPageWidth, m_SheetData.page.nTopMargin + nPageHeight);
	
	GetWindowRect(&rectSheet);


	// 计算sheet的大小
	rectSheet.right = rectSheet.left + m_SheetData.page.nLeftMargin + nPageWidth + m_SheetData.page.nRightMargin ;
	rectSheet.bottom = rectSheet.top + m_SheetData.page.nTopMargin + nPageHeight + m_SheetData.page.nBottomMargin;
	// move sheet	
	MoveWindow( rectSheet );


	// 计算Tab 的大小
	rectTabCtrl.left = rectSheet.left + m_SheetData.tab.nLeftMargin;
	rectTabCtrl.top = rectSheet.top + m_SheetData.tab.nTopMargin;
	rectTabCtrl.right = rectSheet.right - m_SheetData.tab.nRightMargin;
	rectTabCtrl.bottom = rectSheet.bottom - m_SheetData.tab.nBottomMargin ;
	// move tab control
	ScreenToClient( &rectTabCtrl );
	pTabCtrl->MoveWindow( rectTabCtrl);	

	// 计算 page 的大小
	rectActivePage.left = rectSheet.left + m_SheetData.page.nLeftMargin;
	rectActivePage.top = rectSheet.top + m_SheetData.page.nTopMargin;
	rectActivePage.right = rectSheet.right - m_SheetData.page.nRightMargin;
	rectActivePage.bottom = rectSheet.bottom - m_SheetData.page.nBottomMargin;
	// move page
	if ( bActivePageCreated )
	{
		ScreenToClient( &rectActivePage );
		pActivePage->MoveWindow( rectActivePage );
	}	

	// move buttons
	UINT nIDs[] = { IDOK, IDCANCEL, IDHELP, ID_APPLY_NOW};
	tagPositionData *pPosData[] = { &m_SheetData.btnOK, &m_SheetData.btnCancel, &m_SheetData.btnHelp, &m_SheetData.btnApplyNow };
	
	for ( int i = 0; i < sizeof(nIDs) / sizeof(UINT); i++ )
	{
		CWnd *pBtn = GetDlgItem( nIDs[i]);
		if ( pBtn && pBtn->GetSafeHwnd()  )
		{
			CRect rectBtn;			
			pBtn->GetWindowRect( &rectBtn );
			int cx = rectBtn.Width();
			int cy = rectBtn.Height();	
			
			rectBtn.right = rectSheet.right - pPosData[i]->nRightMargin;
			rectBtn.left = rectBtn.right - cx;
			rectBtn.bottom = rectSheet.bottom - pPosData[i]->nBottomMargin;
			rectBtn.top = rectBtn.bottom - cy;

			ScreenToClient( &rectBtn );
			pBtn->MoveWindow( &rectBtn );
		}
	}

	RecalculatelayoutCustomControl();

	return S_OK;
	//
}



BOOL CResizablePropertySheet::OnInitDialog() 
{
	REGISTER_FUNC(  CResizablePropertySheet::OnInitDialog() );

	BOOL bResult = CMFCPropertySheet::OnInitDialog();
	
	if ( (m_psh.dwFlags & PSH_WIZARD) != 0 )
	{
		FOOT_MARK_STRING( _T(" CResizablePropertySheet is not resizable in wizard mode") );
	}
	else
	{
		WARNING_IF_FALSE( GetTabControl()->ModifyStyle( TCS_MULTILINE, NULL) );

		WARNING_IF_FAILED( InitControl() );
		
		WARNING_IF_FAILED ( InitControlPosData() );
		
		WARNING_IF_FAILED ( Recalculatelayout() );
		
	}
	
	// TODO: Add your specialized code here	
	return bResult;
}

HRESULT CResizablePropertySheet::InitCustomControlPosData()
{
	return S_OK;
}

HRESULT CResizablePropertySheet::InitControlPosData()
{
	CRect rectSheet;
	GetWindowRect( &rectSheet );

	// tab control
	CTabCtrl *pTabCtrl = GetTabControl();
	CRect rectTab;
	pTabCtrl->GetWindowRect( &rectTab );
	m_SheetData.tab.RecalPosData( rectSheet, rectTab );

	// page
	CPropertyPage *pActivePage = GetActivePage();
	CRect rectPage;
	if ( pActivePage && pActivePage->GetSafeHwnd() )
	{
		pActivePage->GetWindowRect( &rectPage );
		m_SheetData.page.RecalPosData( rectSheet, rectPage );		
	}

	UINT nIDs[] = { IDOK, IDCANCEL, IDHELP, ID_APPLY_NOW};
	tagPositionData *pPosData[] = { &m_SheetData.btnOK, &m_SheetData.btnCancel, &m_SheetData.btnHelp, &m_SheetData.btnApplyNow };

	for ( int i = 0; i < sizeof(nIDs) / sizeof(UINT); i++ )
	{
		CWnd *pBtn = GetDlgItem( nIDs[i]);
		if ( pBtn && pBtn->GetSafeHwnd() )
		{
			CRect rectBtn;
			pBtn->GetWindowRect( &rectBtn );
			pPosData[i]->RecalPosData( rectSheet, rectBtn);
		}
	}
	// 初始化自定义控件的位置信息
	InitCustomControlPosData();
	//
	return S_OK;
}

HRESULT CResizablePropertySheet::InitControl()
{
	return S_OK;
}


HRESULT CResizablePropertySheet::SetPageTitle(CPropertyPage *pPage, const CString &strPageTitle)
{
	HRESULT hr = S_OK;
	
	if ( !GetTabControl() || !GetTabControl()->GetSafeHwnd() || pPage == NULL )
	{
		return E_FAIL;
	}

	int nIndex = GetPageIndex( pPage );

	if ( nIndex >= 0 )
	{
		TCITEM tcItem;
		tcItem.mask  = TCIF_TEXT;
		tcItem.pszText = (LPTSTR)(LPCTSTR)strPageTitle;
		GetTabControl()->SetItem( nIndex, &tcItem );
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

HRESULT CResizablePropertySheet::SetPageTitle(int nPageIndex, const CString &strPageTitle)
{
	return SetPageTitle( GetPage(nPageIndex), strPageTitle );
}



/////////////////////////////////////////////////////////////////////////////
// CResizablePropertySheetEx

IMPLEMENT_DYNAMIC(CResizablePropertySheetEx, CResizablePropertySheet)

CResizablePropertySheetEx::CResizablePropertySheetEx(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
:CResizablePropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CResizablePropertySheetEx::CResizablePropertySheetEx(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:CResizablePropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CResizablePropertySheetEx::~CResizablePropertySheetEx()
{
}


BEGIN_MESSAGE_MAP(CResizablePropertySheetEx, CResizablePropertySheet)
//{{AFX_MSG_MAP(CResizablePropertySheetEx)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizablePropertySheetEx message handlers

HRESULT CResizablePropertySheetEx::InitControl()
{
	return S_OK;
}

HRESULT CResizablePropertySheetEx::CreateButton()
{
	REGISTER_FUNC( CResizablePropertySheetEx::CreateButton() );
	HRESULT hr = E_FAIL;
	BOOL bModeless = FALSE;

	try
	{
		bModeless = ( m_psh.dwFlags & PSH_MODELESS ) != 0;
		if ( bModeless )
		{
			//
			int cyBorder = ::GetSystemMetrics( SM_CYBORDER );
			// 非模式对话框
			CRect rect;
			CRect rectTabCtrl;
			CRect rectBtnOK;
			GetWindowRect( &rect );

			CTabCtrl *pTabCtrl = GetTabControl();
			VERIFY_TRUE( pTabCtrl != NULL && ::IsWindow( pTabCtrl->GetSafeHwnd()));
			
			pTabCtrl->GetWindowRect( &rectTabCtrl );
			ScreenToClient( &rectTabCtrl );

			CWnd *pBtnOK = GetDlgItem( IDOK );
			VERIFY_TRUE( pBtnOK != NULL && IsWindow( pBtnOK->GetSafeHwnd()));
			pBtnOK->GetWindowRect( &rectBtnOK);
			ScreenToClient( &rectBtnOK );

			rect.bottom += rectBtnOK.Height() + rectBtnOK.top - rectTabCtrl.bottom + cyBorder;
			MoveWindow( rect );

			

			UINT nIDs[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };	
			BOOL bEnable[] = { TRUE, TRUE, FALSE, TRUE};
			for ( int i = 0; i < sizeof(nIDs) / sizeof(UINT); i++ )
			{
				CWnd * pBtn = GetDlgItem( nIDs[i] );
				VERIFY_TRUE( pBtn != NULL && IsWindow( pBtn->GetSafeHwnd()));
				pBtn->EnableWindow( bEnable[i] );
				pBtn->ShowWindow( TRUE );
			}

		}

		hr = S_OK;
	}
	my_catch;

	return hr;

}

BOOL CResizablePropertySheetEx::OnInitDialog()
{
	REGISTER_FUNC(CResizablePropertySheetEx::OnInitDialog());

	BOOL b = CMFCPropertySheet::OnInitDialog();

	if ( (m_psh.dwFlags & PSH_WIZARD) != 0 )
	{
		FOOT_MARK_STRING( _T(" CResizablePropertySheet is not resizable in wizard mode") );
	}
	else
	{
		WARNING_IF_FALSE( GetTabControl()->ModifyStyle( TCS_MULTILINE, NULL) );

		WARNING_IF_FAILED( CreateButton() );
		
		WARNING_IF_FAILED( InitControl() );
		
		WARNING_IF_FAILED( InitControlPosData() );
		
		WARNING_IF_FAILED( Recalculatelayout() );
	}

	return b;
}

BOOL CResizablePropertySheetEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL bModeless = ( m_psh.dwFlags & PSH_MODELESS ) != 0;
	
	if( bModeless && HIWORD(wParam) == BN_CLICKED )             
	{             
		switch( LOWORD(wParam) ) 
		{             
		case   ID_APPLY_NOW:           //           Apply             
			PressButton(PSBTN_APPLYNOW);             
			return   TRUE;             
		case   IDCANCEL:             
			PressButton(PSBTN_CANCEL); 
			DestroyWindow();             
			return   TRUE; 
		case   ID_WIZFINISH:             
			PressButton(PSBTN_FINISH); 
			DestroyWindow();             
			return   TRUE; 
		case   IDHELP:             
			PressButton   (PSBTN_HELP);             
			return   TRUE; 

		case IDOK:
			PressButton(PSBTN_APPLYNOW); 
			DestroyWindow();
			return TRUE;
		}
	}             

	return CResizablePropertySheet::OnCommand( wParam, lParam );
}
