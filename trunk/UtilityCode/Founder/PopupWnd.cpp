// PopupWnd.cpp : implementation file
//

#include "..\stdafx.h"
#include "..\resource.h"
#include "PopupWnd.h"


// CPopupWnd

IMPLEMENT_DYNAMIC(CPopupWnd, CWnd)

CPopupWnd::CPopupWnd()
{
	m_pDialog = NULL;
	m_bSetFocus = TRUE;
	m_nMode = MODE_TOP_LEFT;
	m_hTargetWnd = NULL;
}

CPopupWnd::~CPopupWnd()
{
}


BEGIN_MESSAGE_MAP(CPopupWnd, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


CPopupWnd* CPopupWnd::GetPopupWnd()
{
	CPopupWnd *pPopup = new CPopupWnd();
	return pPopup;
}

// CPopupWnd message handlers

CSize CPopupWnd::GetContentDlgSize( CDialog* pDlg, UINT nID )
{
	CSize sizeDlg;
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

	//
	LPCTSTR lpszName = MAKEINTRESOURCE( nID );
	HINSTANCE hInstance = AfxFindResourceHandle( lpszName, RT_DIALOG ); 
	HRSRC hResource = ::FindResource( hInstance, lpszName, RT_DIALOG );
	HGLOBAL hTemplate = LoadResource( hInstance, hResource);

	DLGTEMPLATE* pTemplate;
	WORD *pDlgTemplateTemp = NULL;
	int nPageWidth = 0;
	int nPageHeight = 0;

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
	::MapDialogRect( pDlg->GetSafeHwnd(), &rectTemp );

	sizeDlg.cx = rectTemp.Width();
	sizeDlg.cy = rectTemp.Height();

	//
	return sizeDlg;

}

void CPopupWnd::OnSetFocus(CWnd* pOldWnd)
{
	if ( !m_bSetFocus && pOldWnd && pOldWnd->GetSafeHwnd() )
	{
		pOldWnd->SetFocus();
		return;
	}
	CWnd::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

void CPopupWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rect;
	CBitmap bmp;
	CDCTool dcTool;

	bmp.LoadBitmap( IDB_BITMAP_MSG_BK );	
	GetClientRect( &rect );
	
	dcTool.DrawBkPicture( dc, bmp, rect );
}

void CPopupWnd::OnDestroy()
{
	if ( m_pDialog )
	{
		if ( m_pDialog->GetSafeHwnd() )
		{
			m_pDialog->DestroyWindow();
		}

		delete m_pDialog;
		m_pDialog = NULL;

	}

	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CPopupWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if ( nIDEvent == 1 )
	{
		KillTimer( nIDEvent );	
		CWnd::OnTimer(nIDEvent);
		DestroyWindow();
	}
	else
	{
		CWnd::OnTimer(nIDEvent);
		BOOL bDestroy = TRUE;
		if ( m_hTargetWnd && ::IsWindow( m_hTargetWnd) )
		{
			CRect rect;
			::GetWindowRect( m_hTargetWnd, &rect );
			if ( rect.EqualRect( m_rectTarget ) )
			{
				bDestroy = FALSE;
			}
		}

		if ( bDestroy )
		{
			KillTimer( nIDEvent );	
			DestroyWindow();
		}
	}
	
}

void CPopupWnd::OnNcDestroy()
{
	CWnd::OnNcDestroy();
	delete this;
	// TODO: Add your message handler code here
}
