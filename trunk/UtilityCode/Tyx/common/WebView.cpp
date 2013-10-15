// WebView.cpp : implementation file
//

#include "stdafx.h"
#include "WebView.h"
//#include "WebFrame.h"
//#include "MainFrm.h"
#include "FunctionObject.h"
#include "mshtml.h"
#include <comdef.h>
#include <atlbase.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   SAFERELEASE(x)	if ((x)) {(x)->Release(); (x) = NULL;}

/////////////////////////////////////////////////////////////////////////////
// CWebView

IMPLEMENT_DYNCREATE(CWebView, CHtmlView)

BOOL CWebView::m_bWantNewUrl = FALSE;
CWebView::CWebView()
	:m_nIDTimer(107),
	m_ptScrollPos(0, 0)
{
	m_bClick = FALSE;
	m_bEnableGoBack = FALSE;
	m_bEnableGoForward = FALSE;
	m_szUrl[0]=0;
	m_szTargetFrameName[0] = 0;
	m_szHeader[0] = 0;
	//{{AFX_DATA_INIT(CWebView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	CRegKey reg;
	LPCTSTR lp="Software\\Microsoft\\Internet Explorer\\Main";
	LPCTSTR lpKey="Disable Script Debugger";
	LPCTSTR lpKey2="Error Dlg Displayed On Every Error";
	if(reg.Open(HKEY_CURRENT_USER,lp) == ERROR_SUCCESS)
	{
		if(reg.SetValue("yes",lpKey) != ERROR_SUCCESS)
		{
			TRACE("failed ! \n");
		}
		if(reg.SetValue("no",lpKey2) != ERROR_SUCCESS)
		{
			TRACE("failed ! \n");
		}
		reg.Close();
	}


}

CWebView::~CWebView()
{
}

void CWebView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebView, CHtmlView)
	//{{AFX_MSG_MAP(CWebView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebView diagnostics

#ifdef _DEBUG
void CWebView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebView message handlers

void CWebView::SetViewID(DWORD dwViewID)
{
	m_dwViewID = dwViewID;
}

DWORD CWebView::GetViewID()
{
	return m_dwViewID;
}

void CWebView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	CString strUrl = GetLocationURL();
	TRACE1(TEXT("\nurl= %s"), strUrl);
	if (m_bClick || m_bWantNewUrl)
	{
		::SendMessage(GetParent()->GetSafeHwnd(), MSG_WEB_NAVIGATE_NEW_ADDR, (WPARAM)lpszURL, 0);
		m_bWantNewUrl = FALSE;
		m_bClick = FALSE;
	}

	// TODO: Add your specialized code here and/or call the base class
	if( baPostedData.GetSize() > 0 ){
		m_BrowseContent.strUrl = lpszURL;
		m_BrowseContent.nFlags = nFlags;
		if( lpszTargetFrameName )
			m_BrowseContent.strTargetFrameName = lpszTargetFrameName;
		else
			m_BrowseContent.strTargetFrameName = _TEXT("");
		if( lpszHeaders )
			m_BrowseContent.strHeader = lpszHeaders;
		else
			m_BrowseContent.strHeader = _TEXT("");
		m_BrowseContent.arrByts.Copy( baPostedData );
	}
	
	SetSilent(TRUE);
	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags,	lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CWebView::OnNavigateComplete2(LPCTSTR strURL) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString strUrl = GetLocationURL();

//	TRACE1(TEXT("\nurl= %s"), strUrl);
	//{add by tian_yx 2009-5-21 for 
// 	if (g_pMainFrm->m_pFrmHtml->m_wndTabCtrl.GetActiveViewID() == this->m_dwViewID)
// 	{
// 		g_pMainFrm->m_pFrmHtml->m_wndEditAddr.SetWindowText(strUrl);
// 	}
	//}end by tian_yx 2009-5-21
	CString strBlank = TEXT("about:blank");
	if( (m_strOldUrl != strBlank && !m_strMouseUrl.IsEmpty()) || (m_strMouseUrl != strUrl) ){

		m_strOldUrl = strUrl;

		if( m_strOldUrl == m_BrowseContent.strUrl ){

			UINT unTotalLen = m_BrowseContent.strUrl.GetLength()+\
				m_BrowseContent.strHeader.GetLength()+\
				m_BrowseContent.strTargetFrameName.GetLength()+\
				m_BrowseContent.arrByts.GetSize();
			UINT unSize = sizeof(STRUCT_WEB_VIEWDATA)+unTotalLen;

			PBYTE pbBuffer = new BYTE[unSize];
			if( pbBuffer ){
				STRUCT_WEB_VIEWDATA *pData = (STRUCT_WEB_VIEWDATA*)pbBuffer;
				pData->bCommand = WEB_DATATYPE_DATA;
				pData->wTotalLen = (WORD)unTotalLen;
				pData->dwFlags = m_BrowseContent.nFlags;
				pData->wHeaderLen = m_BrowseContent.strHeader.GetLength();
				pData->wUrlLen = m_BrowseContent.strUrl.GetLength();
				pData->wTargetFrameNameLen = m_BrowseContent.strTargetFrameName.GetLength();
				pData->wPostDataLen = m_BrowseContent.arrByts.GetSize();
				
				PBYTE pbTemp = pbBuffer+sizeof(STRUCT_WEB_VIEWDATA);
				if( pData->wUrlLen > 0 ){
					CopyMemory( pbTemp,m_BrowseContent.strUrl,pData->wUrlLen );
					pbTemp += pData->wUrlLen;
				}
				if( pData->wTargetFrameNameLen > 0 ){
					CopyMemory( pbTemp,m_BrowseContent.strTargetFrameName,pData->wTargetFrameNameLen );
					pbTemp += pData->wTargetFrameNameLen;
				}
				if( pData->wHeaderLen > 0 ){
					CopyMemory( pbTemp,m_BrowseContent.strHeader,pData->wHeaderLen );
					pbTemp += pData->wHeaderLen;
				}
				if( pData->wPostDataLen > 0 ){
					CopyMemory( pbTemp,m_BrowseContent.arrByts.GetData(),pData->wPostDataLen );
				}
// 				CWebFrame *pFrame = g_pMainFrm->m_pFrmHtml;
// 				if( pFrame && pFrame->IsKindOf( RUNTIME_CLASS(CWebFrame))){
// 					pFrame->SendViewData( m_dwViewID,pbBuffer,unSize );	
// 				}				
				
				delete[] pbBuffer;
			}
		}else{

			UINT unSize = sizeof(STRUCT_WEB_VIEWDATA)+m_strOldUrl.GetLength();
			PBYTE pbBuffer = new BYTE[ unSize ];
			if( pbBuffer ){
				STRUCT_WEB_VIEWDATA *pData = (STRUCT_WEB_VIEWDATA*)pbBuffer;
				pData->bCommand = WEB_DATATYPE_DATA;
				pData->wTotalLen = (WORD)m_strOldUrl.GetLength();
				pData->dwFlags = 0;
				pData->wHeaderLen = 0;
				pData->wUrlLen = (WORD)m_strOldUrl.GetLength();
				pData->wTargetFrameNameLen = 0;
				pData->wPostDataLen = 0;

				CopyMemory( pbBuffer+sizeof(STRUCT_WEB_VIEWDATA),m_strOldUrl,m_strOldUrl.GetLength() );

// 				CWebFrame *pFrame = g_pMainFrm->m_pFrmHtml;
// 				if( pFrame && pFrame->IsKindOf( RUNTIME_CLASS(CWebFrame))){
// 					pFrame->SendViewData( m_dwViewID,pbBuffer,unSize );	
// 				}		

				delete[] pbBuffer;
			}
		}
	}	

	//禁止脚本错误
	LPDISPATCH		pDispDoc	= NULL;
	IHTMLDocument2	*pHTMLDoc	= NULL;
	IHTMLWindow2	*pWnd		= NULL;
	IHTMLWindow2	*pWndParent = NULL;
	try{
		pDispDoc = GetHtmlDocument();
		if (pDispDoc)
		{
			if (SUCCEEDED(pDispDoc->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc))
				&& pHTMLDoc != NULL
				&& SUCCEEDED(pHTMLDoc->get_parentWindow(&pWnd))
				&& pWnd != NULL
				&& SUCCEEDED(pWnd->get_parent(&pWndParent))
				&& pWndParent != NULL)
			{	
				_variant_t varFuc = CFunctionObject<CWebView>::CreateEventFunctionObject(
					NULL,
					NULL, 0);
				pWnd->put_onerror(varFuc);
				pWndParent->put_onerror(varFuc);
			}
		}
	}
	catch(...)
	{
		ASSERT(0);
	}
	try{
		SAFERELEASE(pWndParent);
		SAFERELEASE(pWnd);
		SAFERELEASE(pHTMLDoc);
		SAFERELEASE(pDispDoc);
	}
	catch(...)
	{
		ASSERT(0);
	}

	CHtmlView::OnNavigateComplete2(strURL);
	if (GetParent())
	{
		m_bWantNewUrl = TRUE;
		::SendMessage(GetParent()->GetSafeHwnd(), MSG_WEB_NAVIGATE_FINISH_NAVIGATE, (WPARAM)this, 0);
	}

	ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
		0, SWP_FRAMECHANGED);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER
		| SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void CWebView::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	if(/ * !g_pMainFrm->m_pFrmHtml->EnableOperation() || * /!m_bClick ){
		*Cancel = TRUE;
		//CHtmlView::OnNewWindow2( ppDisp,Cancel );	//cz modify
	}else{
	/ *
		CWebView *pView = g_pMainFrm->m_pFrmHtml->OnAddView();
			if( pView && pView->IsKindOf(RUNTIME_CLASS(CWebView))){
				*ppDisp = pView->GetApplication();* /
	
/ *
			*Cancel = FALSE;
		}else
* /
			*Cancel = TRUE;	
	}*/
	if (m_bClick)
	{
		if (GetParent())
		{
			m_bWantNewUrl = TRUE;
			::SendMessage(GetParent()->GetSafeHwnd(), MSG_WEB_USER_NEW_ADDR, (WPARAM)ppDisp, 0);
		}
		*Cancel = FALSE;
	}
	else
	{
		*Cancel = TRUE;
	}
	m_bClick = FALSE;	
	//CHtmlView::OnNewWindow2(ppDisp, Cancel); //cz modify
}

BOOL CWebView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	if( pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_RBUTTONUP ||
		pMsg->message == WM_RBUTTONDBLCLK )
		return TRUE;
		
	if( / *!g_pMainFrm->m_pFrmHtml->EnableOperation() &&* /
		pMsg->hwnd == GetSafeHwnd() ){
		if(pMsg->message ==  WM_LBUTTONDOWN ||
			pMsg->message == WM_LBUTTONUP ||
			pMsg->message == WM_LBUTTONDBLCLK ||
			pMsg->message == WM_KEYDOWN )
			return TRUE;
	}
	*/

	if( pMsg->message==WM_LBUTTONDOWN ){
		m_bClick = TRUE;		
	}	
	
	return CHtmlView::PreTranslateMessage(pMsg);
}

void CWebView::OnCommandStateChange(long nCommand, BOOL bEnable) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch( nCommand ) {
	case 1:
		m_bEnableGoForward = bEnable;
		break;
	case 2:
		m_bEnableGoBack = bEnable;
		break;
	}
	
	CHtmlView::OnCommandStateChange(nCommand, bEnable);
}

void CWebView::OnStatusTextChange(LPCTSTR lpszText) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_strMouseUrl = lpszText;
	CHtmlView::OnStatusTextChange(lpszText);
}

void CWebView::OnTitleChange(LPCTSTR lpszText) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	CWebFrame *pFrame = g_pMainFrm->m_pFrmHtml;
	if( pFrame && pFrame->IsKindOf( RUNTIME_CLASS(CWebFrame))){
		pFrame->OnViewTitleChanged( m_dwViewID,lpszText );
	}*/

	
	CHtmlView::OnTitleChange(lpszText);
}

BOOL CWebView::HandleData(PBYTE pbData, UINT unDataLen)
{
	STRUCT_WEB_VIEWDATA *pData = (STRUCT_WEB_VIEWDATA*)pbData;
	if( unDataLen == pData->wTotalLen+sizeof(STRUCT_WEB_VIEWDATA) ){
		PBYTE pbTemp = pbData+sizeof(STRUCT_WEB_VIEWDATA);
		if( pData->wUrlLen >= MAX_PATH ||
			pData->wTargetFrameNameLen >= MAX_PATH ||
			pData->wHeaderLen >= MAX_PATH )
			return FALSE;

		m_szUrl[0] = 0;
		if( pData->wUrlLen >0 ){
			CopyMemory( m_szUrl,pbTemp,pData->wUrlLen );
			m_szUrl[pData->wUrlLen] = 0;
			pbTemp+=pData->wUrlLen;
		}

		m_szTargetFrameName[0]=0;
		if( pData->wTargetFrameNameLen > 0 ){
			CopyMemory(m_szTargetFrameName,pbTemp,pData->wTargetFrameNameLen );
			m_szTargetFrameName[pData->wTargetFrameNameLen]=0;
			pbTemp+= pData->wTargetFrameNameLen;
		}

		m_szHeader[0]=0;
		if( pData->wHeaderLen >0 ){
			CopyMemory( m_szHeader,pbTemp,pData->wHeaderLen );
			m_szHeader[pData->wHeaderLen]=0;
			pbTemp+=pData->wHeaderLen;
		}

		if( pData->wPostDataLen > 0 )
			Navigate2( m_szUrl,pData->dwFlags,m_szTargetFrameName,m_szHeader,pbTemp,pData->wPostDataLen );
		else
			Navigate2( m_szUrl,pData->dwFlags,m_szTargetFrameName,m_szHeader );
		
		return TRUE;
	}

	return FALSE;
}



void CWebView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWnd::PostNcDestroy();
}

void CWebView::Reset()
{
	Stop();
	Navigate( "about:blank" );
	m_bClick = FALSE;
	m_bEnableGoBack = FALSE;
	m_bEnableGoForward = FALSE;	
	m_szUrl[0]=0;
	m_szTargetFrameName[0] = 0;
	m_szHeader[0] = 0;
	ShowWindow( SW_HIDE );
}

void CWebView::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nIDTimer)
		OnScrollCallBack();
	
	CHtmlView::OnTimer(nIDEvent);
}

int CWebView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHtmlView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetTimer(m_nIDTimer, 100, NULL);
	
	return 0;
}

void CWebView::OnDestroy() 
{
	if( ::IsWindow( GetSafeHwnd()))
		KillTimer( m_nIDTimer );
	CHtmlView::OnDestroy();
	
}

void CWebView::ProcessData(PBYTE pbData, DWORD dwLen)
{
	BYTE bCommand = *pbData;
	switch (bCommand)
	{
	case WEB_DATATYPE_DATA:
		HandleData(pbData, dwLen);
		break;
	case WEB_DATATYPE_SCROLL:
		HandleScrollView(pbData, dwLen);
		break;
	default:
//		ASSERT(0);
		break;
	}

}

void CWebView::OnScrollCallBack()
{
	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc2 = NULL;
	IHTMLElement *pElement = NULL;
	IHTMLElement2 *pElement2 = NULL;
	
	try{
		pDisp = GetHtmlDocument();
		if (pDisp)
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc2))
				&& pDisp != NULL)
			{		
				if (SUCCEEDED(pHTMLDoc2->get_body(&pElement))
					&& pElement != NULL)
				{				
					//获得IHTMLElement2接口指针，用以访问滚动条
					if (SUCCEEDED(pElement->QueryInterface(IID_IHTMLElement2, (void**)&pElement2))
						&& pElement2 != NULL)
					{										
						// 获得滚动条高度
						long scroll_height; 
						pElement2->get_scrollHeight(&scroll_height);
						
						// 获得滚动条宽度
						long scroll_width;
						pElement2->get_scrollWidth(&scroll_width);
						
						// 获得滚动条位置
						long scroll_top, scroll_left;
						pElement2->get_scrollTop(&scroll_top);
						pElement2->get_scrollLeft(&scroll_left);

						//发送数据
						WEB_VIEWSCROLL scrollInfo;
						scrollInfo.bCommand = WEB_DATATYPE_SCROLL;
						scrollInfo.ptScroll.x = scroll_left;
						scrollInfo.ptScroll.y = scroll_top;
						scrollInfo.szTotal.cx = scroll_width;
						scrollInfo.szTotal.cy = scroll_height;
						
						if (m_ptScrollPos != scrollInfo.ptScroll)
						{
							m_ptScrollPos = scrollInfo.ptScroll;
/*
							CWebFrame *pFrame = (CWebFrame*)GetParentFrame();
							if (pFrame && pFrame->IsKindOf(RUNTIME_CLASS(CWebFrame)))
							{
								pFrame->SendViewData(m_dwViewID, (PBYTE)&scrollInfo, sizeof (WEB_VIEWSCROLL));	
							}
*/
						}
						
					}
				}
			}
		}
	}
	catch(...)
	{
		ASSERT(0);
	}
	
	try{
		if (pElement2 != NULL)
			pElement2->Release();
		if (pElement != NULL)
			pElement->Release();
		if (pHTMLDoc2!=NULL)
			pHTMLDoc2->Release();
		if (pDisp!=NULL)
			pDisp->Release();
	}
	catch(...)
	{
		ASSERT(0);
	}
}

void CWebView::HandleScrollView(PBYTE pData, DWORD dwLen)
{
	if (dwLen != sizeof (WEB_VIEWSCROLL))
	{
		ASSERT(0);
		return;
	}
	WEB_VIEWSCROLL *pScroll = (WEB_VIEWSCROLL *)pData;

	LPDISPATCH pDisp = NULL;
	IHTMLDocument2 *pHTMLDoc2 = NULL;
	IHTMLElement *pElement = NULL;
	IHTMLElement2 *pElement2 = NULL;
	
	try{
		pDisp = GetHtmlDocument();
		if (pDisp)
		{
			if (SUCCEEDED(pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc2))
				&& pDisp != NULL)
			{		
				if (SUCCEEDED(pHTMLDoc2->get_body(&pElement))
					&& pElement != NULL)
				{				
					//获得IHTMLElement2接口指针，用以访问滚动条
					if (SUCCEEDED(pElement->QueryInterface(IID_IHTMLElement2, (void**)&pElement2))
						&& pElement2 != NULL)
					{		
						pElement2->put_scrollLeft(pScroll->ptScroll.x);

						pElement2->put_scrollTop(pScroll->ptScroll.y);
					}
				}
			}
		}
	}
	catch(...)
	{
		ASSERT(0);
	}
	
	try{
		if (pElement2 != NULL)
			pElement2->Release();
		if (pElement != NULL)
			pElement->Release();
		if (pHTMLDoc2!=NULL)
			pHTMLDoc2->Release();
		if (pDisp!=NULL)
			pDisp->Release();
	}
	catch(...)
	{
		ASSERT(0);
	}

}