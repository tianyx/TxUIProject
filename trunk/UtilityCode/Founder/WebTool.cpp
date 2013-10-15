#include "..\stdafx.h"
#include "WebTool.h"
#include "DCTool.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWndStyleTool::CWndStyleTool()
{

}

CWndStyleTool::~CWndStyleTool()
{

}

HRESULT CWndStyleTool::SetPinToDesktopStyle(HWND hWnd)
{
	SET_FUNC_NAME( _T("CWndStyleTool::SetPinToDesktop(HWND hWnd)") );
	HRESULT hr = E_FAIL;

	try	
	{	
		HWND hOldParent = NULL;
		VERIFY_SUCCEEDED( SetPinToDesktopStyle( hWnd, hOldParent) );
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT CWndStyleTool::SetPinToDesktopStyle(HWND hWnd, HWND &hOldParent)
{
	SET_FUNC_NAME( _T("CWndStyleTool::SetPinToDesktopStyle(HWND hWnd, HWND &hOldParent)") );
	
	HRESULT hr			= E_FAIL;
	DWORD dwStyle		= NULL;
	HWND hDesktop		= NULL;
	CWnd* pWndDesktop	= NULL;
		
	try	
	{
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		dwStyle = GetWindowLong( hWnd, GWL_STYLE);
		VERIFY_TRUE( (dwStyle & WS_CHILD) == 0);		
		hDesktop = ::FindWindow( _T("Progman"), NULL);	
		VERIFY_TRUE( hDesktop != NULL);	
		VERIFY_TRUE( (hOldParent = ::SetParent( hWnd, hDesktop)) != NULL );		
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CWndStyleTool::RemoveToolWindowStyle(HWND hWnd)
{
	SET_FUNC_NAME( _T("CWndStyleTool::RemoveToolWindowStyle(HWND hWnd)") );
	
	HRESULT hr			= E_FAIL;	
	try	
	{	
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		VERIFY_SUCCEEDED( _RemoveStyle( hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW ) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;	
}

HRESULT CWndStyleTool::SetToolWindowStyle(HWND hWnd)
{
	SET_FUNC_NAME( _T("CWndStyleTool::SetToolWindowStyle()") );
	
	HRESULT hr			= E_FAIL;	
	try	
	{	
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		VERIFY_SUCCEEDED( _AddStyle( hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW ) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;
}


HRESULT CWndStyleTool::SetLayerWindowStyle(HWND hWnd)
{
	SET_FUNC_NAME( _T("CWndStyleTool::SetLayerWindowStyle(HWND hWnd)") );
	
	HRESULT hr			= E_FAIL;	
	try	
	{	
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		VERIFY_SUCCEEDED( _AddStyle( hWnd, GWL_EXSTYLE, WS_EX_LAYERED ) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;	
}

HRESULT CWndStyleTool::RemoveLayerWindowStyle(HWND hWnd)
{
	SET_FUNC_NAME( _T("CWndStyleTool::RemoveLayerWindowStyle(HWND hWnd)") );
	
	HRESULT hr			= E_FAIL;	
	try	
	{	
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		VERIFY_SUCCEEDED( _RemoveStyle( hWnd, GWL_EXSTYLE, WS_EX_LAYERED ) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;
	
}

HRESULT CWndStyleTool::_AddStyle(HWND hWnd, DWORD dwFlag, DWORD dwStyle)
{
	SET_FUNC_NAME( _T("CWndStyleTool::_AddStyle(HWND hWnd, DWORD dwFlag, DWORD dwStyle)") );
	
	HRESULT hr			= E_FAIL;	
	try	
	{	
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		VERIFY_TRUE( SetWindowLong( hWnd, dwFlag, GetWindowLong( hWnd, dwFlag) | dwStyle ) != 0 );
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CWndStyleTool::_RemoveStyle(HWND hWnd, DWORD dwFlag, DWORD dwStyle)
{
	SET_FUNC_NAME( _T("_RemoveStyle(HWND hWnd, DWORD dwFlag, DWORD dwStyle)") );
	
	HRESULT hr			= E_FAIL;	
	try	
	{	
		VERIFY_TRUE( hWnd != NULL && IsWindow(hWnd) );
		VERIFY_TRUE( SetWindowLong( hWnd, dwFlag, GetWindowLong( hWnd, dwFlag) & ~(dwStyle) ) != 0 );
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}


CAnimateWndTool* GetAnimateWndTool()
{
	static CAnimateWndTool theTool;	
	return &theTool;
}

/////////////////////////////////////////////////////////////////////////////
// CAnimateWndTool

CAnimateWndTool::CAnimateWndTool()
{
	m_agent.Create( NULL, _T("agent"), WS_OVERLAPPED, CRect( -1, -1, 0, 0), CWnd::FromHandle(GetDesktopWindow()), 100, NULL );
}

CAnimateWndTool::~CAnimateWndTool()
{
	if ( IsWindow( m_agent.GetSafeHwnd() ) )
	{
		m_agent.DestroyWindow();
	}
}

HRESULT	CAnimateWndTool::AnimateWindow( HWND hWnd, DWORD dwTime, DWORD dwFlags, const CRect& rectOrg, const CRect& rectDest, UINT uMsg)
{
	REGISTER_FUNC( CAnimateWndTool::AnimateWindow( HWND hWnd, DWORD dwTime, DWORD dwFlags) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( hWnd && ::IsWindow(hWnd) );
		VERIFY_TRUE( ::IsWindow(m_agent.GetSafeHwnd()) );
		_tagCustomdata task;
		
		task.bTaskStart		= FALSE;
		task.dwFlag			= dwFlags;
		task.dwTaskDuration = dwTime; // milliseconds
		task.hWnd			= hWnd;
		task.rectDest		= rectDest;
		task.rectOrg		= rectOrg;
		task.timeTaskBegin	= ::GetTickCount();
		task.uMsg			= uMsg;

		if ( (dwFlags & AW_BLEND) != 0 )
		{
			if ( (dwFlags & AW_HIDE) != 0 )
			{
				task.nFadeStart = 255;
				task.nFadeEnd = 0;
			}
			else
			{
				task.nFadeStart = 0;
				task.nFadeEnd = 255;
			}
		}	

		m_agent.SendMessage( WM_ANIMATEWNDTOOL_ADD_TASK, (WPARAM)(&task), NULL );

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CAnimateWndTool::SetTimerInterval(int nTimerInterval)
{
	HRESULT hr = E_FAIL;
	BOOL bSucceeded = FALSE;

	if ( IsWindow(m_agent.GetSafeHwnd()) )
	{
		m_agent.SendMessage( WM_ANIMATEWNDTOOL_SETTIMERINTERVAL, (WPARAM)nTimerInterval, (LPARAM)(&bSucceeded) );
		hr = bSucceeded ? S_OK : E_FAIL;
	}	
	
	return hr;
}
/////////////////////////////////////////////////////////////////////////////
// CAnimateWndTool message handlers

CAnimateWndTool::CAnimateWndToolAgent::CAnimateWndToolAgent()
{
	m_CustomDataArray.SetSize(32);
	m_nCustomDataIndex = 0;
	m_nTimerInterval = 10;
}

CAnimateWndTool::CAnimateWndToolAgent::~CAnimateWndToolAgent()
{
}

LRESULT CAnimateWndTool::CAnimateWndToolAgent::OnAddtask( WPARAM wParam, LPARAM lParam)
{
	_tagCustomdata* pCustomdata = (_tagCustomdata*)wParam;
	BOOL bAutoDelete = (BOOL)lParam;
	
	if ( pCustomdata == NULL )
	{
		return 0L;
	}

	m_CustomDataArray.SetAtGrow( m_nCustomDataIndex++, *pCustomdata );

	if ( bAutoDelete )
	{
		delete pCustomdata;
	}

	if ( m_nCustomDataIndex == 1 ) 
	{
		SetTimer( 1, m_nTimerInterval, NULL );
	}

	return 0L;
}

LRESULT CAnimateWndTool::CAnimateWndToolAgent::SetTimerInterval( WPARAM wParam, LPARAM lParam)
{
	int nTimerInterval = (int)wParam;
	if ( nTimerInterval > 0 )
	{
		m_nTimerInterval = nTimerInterval;
		if ( m_nCustomDataIndex > 0 )
		{
			SetTimer( 1, m_nTimerInterval, NULL );
		}
	}

	BOOL *pbSucceed = (BOOL*)lParam;
	if ( pbSucceed )
	{
		*pbSucceed = ( nTimerInterval > 0 );
	}

	return 0;
}

LRESULT CAnimateWndTool::CAnimateWndToolAgent::OnTimer(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nIDEvent = (UINT)wParam;
	if ( nIDEvent != 1 )
	{
		return 0L;
	}
	//
	DWORD dwTickCountCur = ::GetTickCount();
	double dPercent = 0.0;
	
	BOOL bTaskFinished = FALSE;

	for  ( int i = m_nCustomDataIndex - 1; i >= 0; i-- )
	{
		bTaskFinished = FALSE;

		_tagCustomdata& customData = m_CustomDataArray[i];

		if ( !::IsWindow( customData.hWnd ) || customData.dwTaskDuration <= 0)
		{
			m_CustomDataArray.RemoveAt( i );
			m_nCustomDataIndex--;
			continue;
		}
		
		if ( !customData.bTaskStart )
		{
			customData.dwStyleEx = GetWindowLong( customData.hWnd, GWL_EXSTYLE);
		}

		
		if ( dwTickCountCur >= customData.timeTaskBegin )
		{
			dPercent = ( dwTickCountCur - customData.timeTaskBegin ) * 1.0 / customData.dwTaskDuration;
			if ( dwTickCountCur - customData.timeTaskBegin >= customData.dwTaskDuration )
			{				
				bTaskFinished = TRUE;
			}			
		}
		else
		{
			dPercent = ( (DWORD)0xFFFFFFFF - customData.timeTaskBegin + dwTickCountCur ) * 1.0 / customData.dwTaskDuration;
			if ( (DWORD)0xFFFFFFFF - customData.timeTaskBegin + dwTickCountCur >= customData.dwTaskDuration )
			{
				bTaskFinished = TRUE;
			}
		}
		
		if ( bTaskFinished)
		{
			if ( (customData.dwFlag & AW_BLEND) && (customData.dwFlag & AW_HIDE) )
			{
				::ShowWindow( customData.hWnd, SW_HIDE );				
			}

			SetWindowLong( customData.hWnd, GWL_EXSTYLE, customData.dwStyleEx);
			::MoveWindow( customData.hWnd, customData.rectDest.left, customData.rectDest.top, customData.rectDest.Width(), customData.rectDest.Height(), TRUE );
			if ( customData.uMsg )
			{
				::PostMessage( customData.hWnd, customData.uMsg, NULL, NULL);
			}
			m_CustomDataArray.RemoveAt( i );
			m_nCustomDataIndex--;
			continue;
		}
		
		if ( ( customData.dwFlag & AW_BLEND))
		{
			if ( !customData.bTaskStart )
			{
				SetWindowLong( customData.hWnd, GWL_EXSTYLE, GetWindowLong( customData.hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED );
			}
			::SetLayeredWindowAttributes( customData.hWnd, RGB(0,0,0), customData.nFadeStart + (int)((customData.nFadeEnd - customData.nFadeStart) * dPercent ), LWA_ALPHA);
			
			// 特殊处理
			if ( ( customData.dwFlag & AW_SLIDE ) == 0 ) 
			{
				CRect rect;
				::GetWindowRect( customData.hWnd, &rect );
				::MoveWindow( customData.hWnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE );		
			}
		}

		if (( customData.dwFlag & AW_SLIDE ) ) 
		{
				// 滑动
				CRect rect( customData.rectOrg );
				int nDeltaRight = 0;
				int nDeltaLeft  = 0;
				int nDeltaTop  = 0;
				int nDeltaBottom  = 0;
#pragma warning(disable:4244)
				nDeltaLeft = ( customData.rectDest.left - customData.rectOrg.left ) * dPercent;
				nDeltaRight = ( customData.rectDest.right - customData.rectOrg.right ) * dPercent;
				nDeltaTop = ( customData.rectDest.top - customData.rectOrg.top ) * dPercent;
				nDeltaBottom = ( customData.rectDest.bottom - customData.rectOrg.bottom ) * dPercent;
#pragma warning(default:4244)
				
				rect.left += nDeltaLeft;
				rect.right += nDeltaRight;
				rect.top += nDeltaTop;
				rect.bottom += nDeltaBottom;
				
				::MoveWindow( customData.hWnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE );		
		}	

		customData.bTaskStart = TRUE;
		
	}
	
	
	if ( m_nCustomDataIndex <= 0 )
	{
		KillTimer( 1 );
	}
	//
	return 0L;
}

BEGIN_MESSAGE_MAP(CAnimateWndTool::CAnimateWndToolAgent, CWnd)
//{{AFX_MSG_MAP(CAnimateWndToolAgent)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
ON_MESSAGE( WM_ANIMATEWNDTOOL_ADD_TASK, CAnimateWndTool::CAnimateWndToolAgent::OnAddtask)
ON_MESSAGE( WM_TIMER, CAnimateWndTool::CAnimateWndToolAgent::OnTimer)
ON_MESSAGE( WM_ANIMATEWNDTOOL_SETTIMERINTERVAL, CAnimateWndTool::CAnimateWndToolAgent::SetTimerInterval)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebToolAgent

CWebTool::CWebToolAgent::CWebToolAgent(  CWebTool* pWebTool )
{
	m_pWebTool = pWebTool;
}

CWebTool::CWebToolAgent::~CWebToolAgent()
{
}

void CWebTool::CWebToolAgent::DocumentComplete(LPDISPATCH pDisp, VARIANT* URL)
{
	UNUSED_ALWAYS(pDisp);
	ASSERT(V_VT(URL) == VT_BSTR);

	CString str(V_BSTR(URL));
	OnDocumentComplete(str);

}

void CWebTool::CWebToolAgent::OnDocumentComplete(LPCTSTR lpszURL)
{
	REGISTER_FUNC(CWebTool::CWebToolAgent::OnDocumentComplete(LPCTSTR lpszURL) );
	try
	{
		VERIFY_TRUE( m_pWebTool != NULL );
		m_pWebTool->m_bHtmlLoad = TRUE;
	}
	my_catch;	
}

void CWebTool::CWebToolAgent::BeforeNavigate2(LPDISPATCH /* pDisp */, VARIANT* URL,
											  VARIANT* Flags, VARIANT* TargetFrameName,
											  VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	ASSERT(V_VT(URL) == VT_BSTR);
	ASSERT(V_VT(TargetFrameName) == VT_BSTR);
	ASSERT(V_VT(PostData) == (VT_VARIANT | VT_BYREF));
	ASSERT(V_VT(Headers) == VT_BSTR);
	ASSERT(Cancel != NULL);
	
	USES_CONVERSION;
	
	VARIANT* vtPostedData = V_VARIANTREF(PostData);
	CByteArray array;
	if (V_VT(vtPostedData) & VT_ARRAY)
	{
		// must be a vector of bytes
		ASSERT(vtPostedData->parray->cDims == 1 && vtPostedData->parray->cbElements == 1);
		
		vtPostedData->vt |= VT_UI1;
		COleSafeArray safe(vtPostedData);
		
		DWORD dwSize = safe.GetOneDimSize();
		LPVOID pVoid;
		safe.AccessData(&pVoid);
		
		array.SetSize(dwSize);
		LPBYTE lpByte = array.GetData();
		
		memcpy(lpByte, pVoid, dwSize);
		safe.UnaccessData();
	}
	// make real parameters out of the notification
	
	CString strTargetFrameName(V_BSTR(TargetFrameName));
	CString strURL(V_BSTR(URL));
	CString strHeaders ( V_BSTR(Headers));
	DWORD nFlags = V_I4(Flags);
	
	// notify the user's class
	m_pWebTool->OnBeforeNavigate2(strURL, nFlags, strTargetFrameName,
		array, strHeaders, Cancel);
}

BEGIN_MESSAGE_MAP( CWebTool::CWebToolAgent, CWnd)
	//{{AFX_MSG_MAP(CWebToolAgent)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP( CWebTool::CWebToolAgent, CWnd)
	ON_EVENT(CWebTool::CWebToolAgent, AFX_IDW_PANE_FIRST, 259 /* DocumentComplete */, CWebTool::CWebToolAgent::DocumentComplete, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CWebTool::CWebToolAgent, AFX_IDW_PANE_FIRST, 250 /* BeforeNavigate2 */, CWebTool::CWebToolAgent::BeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebToolAgent message handlers


CWebTool::CWebTool(BOOL bUIMode):
m_wndBrowser(this)
{
	REGISTER_FUNC( constructor : CWebTool::CWebTool(BOOL bUIMode) );

	m_bUIMode		= bUIMode;
	
	m_spHTMLDocument = NULL;

	m_bHtmlLoad = FALSE;
	
	if ( !m_bUIMode )
	{
		// without UI mode
		WARNING_WHEN( FAILED(_Init()) );
	}
}

CWebTool::~CWebTool(void)
{
	if ( ::IsWindow(m_wndBrowser.GetSafeHwnd()) )
	{
		m_wndBrowser.DestroyWindow();
	}
}

HRESULT	CWebTool::_Init()
{
	REGISTER_FUNC( protected : CWebTool::_Init() );
	HRESULT hr = E_FAIL;

	try
	{		
		//
		VERIFY_TRUE( !m_bUIMode );		
		VERIFY_SUCCEEDED( Create( _T(""), WS_CHILD, CRect(-1,-1,0,0), CWnd::FromHandle(GetDesktopWindow())/*, AFX_IDW_PANE_FIRST*/));
		hr = S_OK;
	}
	my_catch;

	return hr;

}

CWnd* CWebTool::GetWnd()
{
	return &m_wndBrowser;
}

HRESULT	CWebTool::Create( const CString& strWindowName, DWORD dwStyle, CRect rect, CWnd* pParent /*, UINT nID*/)
{
	REGISTER_FUNC( CWebTool::Create( const CString& strWindowName, DWORD dwStyle, CRect rect, CWnd* pParent, UINT nID) );
	HRESULT hr = E_FAIL;

	if ( IsWindow(m_wndBrowser.GetSafeHwnd()) )
	{
		return S_FALSE;
	}

	try
	{			
		
		VERIFY_TRUE( m_wndBrowser.CreateControl(CLSID_WebBrowser, strWindowName, dwStyle, rect, pParent, AFX_IDW_PANE_FIRST)) ;

		LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
		VERIFY_TRUE( NULL != lpUnk );
		VERIFY_SUCCEEDED( lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_spBrowserApp) );
		VERIFY_TRUE( m_spBrowserApp != NULL);
		VERIFY_SUCCEEDED( Navigate2(_T("about:blank")));
	
		hr = S_OK;
	}
	my_catch;

	if ( FAILED(hr) )
	{

		m_wndBrowser.DestroyWindow();		
	}
	
	return hr;
}

HRESULT	CWebTool::GetBodyStyleInterface( CComPtr<IHTMLStyle>& spStyle )
{
	REGISTER_FUNC( HRESULT	CWebTool::GetBodyStyleInterface( CComPtr<IHTMLStyle>& spStyle ) );
	HRESULT hr = E_FAIL;

	try
	{
		CComQIPtr<IHTMLDocument2> spDoc;
		VERIFY_SUCCEEDED( GetHtmlDocumentPtr( spDoc) );
		CComQIPtr<IHTMLElement> spBody;
		VERIFY_SUCCEEDED( spDoc->get_body(&spBody) );
		VERIFY_TRUE( spBody != NULL );		
		VERIFY_SUCCEEDED( spBody->get_style(&spStyle) );   
		VERIFY_TRUE( spStyle != NULL );
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT CWebTool::SetBodyStyle( LPCTSTR lpszAttributeName, LPCTSTR lpszAttributeValue)
{
	REGISTER_FUNC( HRESULT CWebTool::SetBodyStyle( LPCTSTR lpszAttributeName, LPCTSTR lpszAttributeValue) );
	HRESULT hr = E_FAIL;

	try
	{
		CComQIPtr<IHTMLDocument2> spDoc;
		VERIFY_SUCCEEDED( GetHtmlDocumentPtr( spDoc) );
		CComQIPtr<IHTMLElement> spBody;
		VERIFY_SUCCEEDED( spDoc->get_body(&spBody) );
		VERIFY_TRUE( spBody != NULL );
		CComQIPtr<IHTMLElement> spElement;
		VERIFY_SUCCEEDED( spBody->QueryInterface(IID_IHTMLElement,(void**)&spElement) );
		VERIFY_TRUE( spElement != NULL );

		CComBSTR v( lpszAttributeName );
		CComVariant vv= lpszAttributeValue;
		VERIFY_SUCCEEDED( spElement->setAttribute( v, vv, 0 ) );

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT CWebTool::Close()
{
	REGISTER_FUNC( CWebTool::Close() );
	HRESULT hr = E_FAIL;

	try
	{
		m_bHtmlLoad = FALSE;
		m_bInit = FALSE;
		m_spHTMLDocument = NULL;
		m_spBrowserApp.Release();
		if ( IsWindow( m_wndBrowser.GetSafeHwnd()) )
		{
			m_wndBrowser.DestroyWindow();
		}
		hr = S_OK;
	}
	my_catch;

	return hr;
	
}

HRESULT	CWebTool::GetWebBrowserPtr( CComPtr<IWebBrowser2>& spBrowserApp)
{
	REGISTER_FUNC(CWebTool::GetWebBrowserPtr( IWebBrowser2Ptr& spBrowserApp));
	HRESULT hr = E_FAIL;

	spBrowserApp = NULL;

	try
	{
		VERIFY_TRUE( m_spBrowserApp != NULL );
		spBrowserApp = m_spBrowserApp;
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CWebTool::GetHtmlDocumentPtr( CComPtr<IHTMLDocument2>& spHTMLDocument )
{
	REGISTER_FUNC( CWebTool::GetHtmlDocumentPtr( CComPtr<IHTMLDocument2>& spHTMLDocument ) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( m_spBrowserApp != NULL );
		LPDISPATCH lpDispatch = NULL;
		VERIFY_SUCCEEDED( m_spBrowserApp->get_Document( &lpDispatch));
		VERIFY_TRUE( lpDispatch != NULL );
		if ( m_spHTMLDocument != NULL )
		{
			m_spHTMLDocument.Release();
		}
		hr = lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&m_spHTMLDocument);
		lpDispatch->Release();

		VERIFY_SUCCEEDED( hr );		
		VERIFY_TRUE( m_spHTMLDocument != NULL );
		spHTMLDocument = m_spHTMLDocument;

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CWebTool::ExecScript( const CString& strScript, const CString& strLanguage)
{
	REGISTER_FUNC(CWebTool::ExecScript( const CString& strJScript));
	HRESULT hr = E_FAIL;
	CComPtr<IHTMLDocument2> spDoc;
	CComQIPtr<IHTMLWindow2> spHtmlWindow;

	try
	{
		VERIFY_TRUE( m_spBrowserApp != NULL );
		VERIFY_SUCCEEDED( GetHtmlDocumentPtr(spDoc) );
		VERIFY_SUCCEEDED( spDoc->get_parentWindow( &spHtmlWindow) );
		VERIFY_TRUE( spHtmlWindow != NULL );
		CComBSTR bstrJScript = strScript;
		CComBSTR bstrLanguage = strLanguage;
		VARIANT vReturn;
		VariantInit( &vReturn );
		VERIFY_SUCCEEDED(spHtmlWindow->execScript( bstrJScript, bstrLanguage, &vReturn ));
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CWebTool::GetJScript(CComPtr<IDispatch>& spDisp)
{
	REGISTER_FUNC( CWebTool::GetJScript(CComPtr<IDispatch>& spDisp) );
	HRESULT hr = E_FAIL;

	CComPtr<IHTMLDocument2> spDoc;
	
	try
	{
		VERIFY_SUCCEEDED( GetHtmlDocumentPtr( spDoc) );		
		VERIFY_SUCCEEDED(spDoc->get_Script( &spDisp ));
		VERIFY_TRUE( spDisp != NULL );

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CWebTool::CallJScript(const CString& strFunc, const CStringArray &paramArray, CComVariant *pVarResult)
{
	REGISTER_FUNC( CWebTool::CallJScript(const CString strFunc, const CStringArray &paramArray, CComVariant *pVarResult) );
	HRESULT hr = E_FAIL;

	int i = 0;
	CComPtr<IDispatch> spScript;
	CComBSTR bstrMember(strFunc);
	DISPID dispid = NULL;
	const int arraySize = paramArray.GetSize();
	DISPPARAMS dispparams;
	EXCEPINFO excepInfo;
	CComVariant vaResult;
	UINT nArgErr = (UINT)-1;  // initialize to invalid arg

	memset(&excepInfo, 0, sizeof excepInfo);
	memset( &dispparams, 0, sizeof(dispparams) );

	try
	{
		VERIFY_TRUE( arraySize >= 0 );
		VERIFY_SUCCEEDED( GetJScript(spScript) );		
		VERIFY_SUCCEEDED( spScript->GetIDsOfNames( IID_NULL, &bstrMember, 1, LOCALE_SYSTEM_DEFAULT, &dispid) );		
		dispparams.cArgs = arraySize;	// Number of arguments
		dispparams.rgvarg = ( arraySize > 0 ) ? new VARIANT[dispparams.cArgs] : NULL;
		for( i = 0; i < arraySize; i++ )
		{
			CComBSTR bstr = paramArray.GetAt(arraySize - 1 - i); // back reading
			bstr.CopyTo(&dispparams.rgvarg[i].bstrVal);	//Array of arguments
			dispparams.rgvarg[i].vt = VT_BSTR;
		}
		dispparams.cNamedArgs = 0;	// Number of named arguments	

		VERIFY_SUCCEEDED(spScript->Invoke( dispid, IID_NULL, 0, DISPATCH_METHOD, &dispparams, &vaResult, &excepInfo, &nArgErr) );	

		if(pVarResult)
		{
			*pVarResult = vaResult;
		}

		hr = S_OK;
	}
	my_catch;

	for( i = 0; i < arraySize; i++) 
	{ 
		// If false, the bstr argument is attached to the new object without making a copy by calling SysAllocString
		// 
		_bstr_t _bstrVal(dispparams.rgvarg[i].bstrVal, FALSE ); 
	} 


	delete []dispparams.rgvarg;

	return hr;
}

HRESULT	CWebTool::CallJScript(const CString& strFunc,CComVariant* pVarResult)
{
	REGISTER_FUNC( CWebTool::CallJScript(const CString strFunc,CComVariant* pVarResult) );
	HRESULT hr = E_FAIL;

	CStringArray paramArray;

	try
	{
		
		VERIFY_SUCCEEDED(CallJScript(strFunc,paramArray,pVarResult));
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CWebTool::CallJScript(const CString& strFunc,const CString& strArg1, CComVariant* pVarResult)
{
	REGISTER_FUNC( CWebTool::CallJScript(const CString& strFunc,const CString& strArg1, CComVariant* pVarResult) );
	HRESULT hr = E_FAIL;
	CStringArray paramArray;

	try
	{
		paramArray.Add( strArg1 );
		VERIFY_SUCCEEDED( CallJScript( strFunc, paramArray, pVarResult));
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CWebTool::Navigate2(const CString& strURL, DWORD dwFlags, const CString& strTargetFrameName, const CString& strHeaders,
						  LPVOID lpvPostData , DWORD dwPostDataLen , BOOL bAsync)
{
	REGISTER_FUNC( CWebTool::Navigate2(const CString& strURL, DWORD dwFlags, const CString& strTargetFrameName, const CString& strHeaders,
		LPVOID lpvPostData , DWORD dwPostDataLen ));

	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( m_spBrowserApp != NULL);

		COleSafeArray vPostData;
		if ( lpvPostData != NULL )
		{
			if ( dwPostDataLen == 0 )
				dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

			vPostData.CreateOneDim( VT_UI1, dwPostDataLen, lpvPostData );
		}

		COleVariant vURL( strURL, VT_BSTR);
		COleVariant vHeaders( strHeaders, VT_BSTR);
		COleVariant vTargetFrameName( strTargetFrameName, VT_BSTR);
		COleVariant vFlags((long) dwFlags, VT_I4);

		m_bHtmlLoad = FALSE;

		VERIFY_SUCCEEDED(m_spBrowserApp->Navigate2( vURL, vFlags, vTargetFrameName, vPostData, vHeaders));

		MSG msg;

		while( !bAsync && !m_bHtmlLoad )
		{
			if (!PeekMessage(&msg,NULL,0,0,PM_REMOVE)) continue;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		hr = S_OK;
	}
	my_catch;
	
	return hr;
}

HRESULT	CWebTool::Navigate2ResHtml( UINT nID, BOOL bAsync )
{
	REGISTER_FUNC( HRESULT	CWebTool::Navigate2ResHtml( UINT nID, BOOL bAsync ) );
	HRESULT hr = E_FAIL;

	CComPtr<IHTMLDocument2> spDoc;
	CComQIPtr<IHTMLElement> spElement;
	CComQIPtr<IPersistStreamInit> spPersistStreamInit;

	IStream *pStream = NULL;
	CDCTool dcTool;

	try
	{
		VERIFY_SUCCEEDED( Navigate2( _T("about:blank") ) );
		VERIFY_SUCCEEDED( GetHtmlDocumentPtr(spDoc));
		VERIFY_SUCCEEDED( spDoc->QueryInterface( IID_IPersistStreamInit,  (void**)&spPersistStreamInit ) );

		VERIFY_SUCCEEDED( spPersistStreamInit->InitNew() );		
		VERIFY_SUCCEEDED( dcTool.Resource2IStream( AfxGetResourceHandle(), nID, RT_HTML, &pStream ) );
		VERIFY_SUCCEEDED(spPersistStreamInit->Load( pStream ));

		hr = S_OK;
	}
	my_catch;

	if ( pStream != NULL )
	{
		pStream->Release();
	}

	return hr;

}

HRESULT	CWebTool::Navigate2MemHtml( CString strHtmlSource, BOOL bJustBody, BOOL bAsync )
{
	REGISTER_FUNC( CWebTool::Navigate2MemHtml( CString strHtmlSource, BOOL bAsync ) );
	HRESULT hr = E_FAIL;
	BSTR bstr = NULL;

	try
	{
		CComPtr<IHTMLDocument2> spDoc;
		CComQIPtr<IHTMLElement> spElement;
		CComPtr<IStream> spStream;

		VERIFY_SUCCEEDED( Navigate2( _T("about:blank") ) );
		VERIFY_SUCCEEDED( GetHtmlDocumentPtr(spDoc));
		VERIFY_SUCCEEDED(spDoc->get_body( &spElement ) );
		VERIFY_TRUE( spElement != NULL );
		/*
		
		*/
		if ( bJustBody )
		{
			bstr = strHtmlSource.AllocSysString();
			VERIFY_SUCCEEDED(spElement->put_innerHTML( bstr ) );
		}
		else
		{
			int nLength = strHtmlSource.GetLength();
			HGLOBAL hMem = GlobalAlloc( GMEM_FIXED, ( nLength + 1) * sizeof(TCHAR) ); 
			BYTE* pmem = (BYTE*)GlobalLock( hMem ); 
			memcpy( pmem,  strHtmlSource.GetBuffer( nLength ), ( nLength + 1) * sizeof(TCHAR) ); 
			strHtmlSource.ReleaseBuffer();

			CreateStreamOnHGlobal( hMem, FALSE, &spStream ); 				
			GlobalUnlock( hMem ); 

			CComQIPtr<IPersistStreamInit> spPersistStreamInit;
			VERIFY_SUCCEEDED( spDoc->QueryInterface( IID_IPersistStreamInit,  (void**)&spPersistStreamInit ) );

			VERIFY_SUCCEEDED( spPersistStreamInit->InitNew() );		
			VERIFY_SUCCEEDED(spPersistStreamInit->Load( spStream ));

		}
		


		hr = S_OK;
	}
	my_catch;

	if ( bstr )
	{
		::SysFreeString( bstr );
	}

	return hr;

}

void CWebTool::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	// default to continuing
	*pbCancel = FALSE;
	
	// user will override to handle this notification
	UNUSED_ALWAYS(lpszURL);
	UNUSED_ALWAYS(nFlags);
	UNUSED_ALWAYS(lpszTargetFrameName);
	UNUSED_ALWAYS(baPostedData);
	UNUSED_ALWAYS(lpszHeaders);
	
}


/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd

CHtmlWnd::CHtmlWnd()
{
}

CHtmlWnd::~CHtmlWnd()
{
	m_webTool.Close();
	DestroyWindow();	
}

BOOL CHtmlWnd::Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExstyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
	int cx = GetSystemMetrics( SM_CXVSCROLL ) + 1;
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	   
	wndcls.style = 0;    
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = NULL;
	wndcls.hCursor = NULL;
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = lpszClassName;
	   
	if ( !AfxRegisterClass( &wndcls ) )
	{
		return FALSE;
	}
  
	if ( CWnd::CreateEx( dwExstyle, wndcls.lpszClassName,
		lpszWindowName, dwStyle, rect, NULL, NULL, NULL) == 0 )
	{
		return FALSE;
	}
	   
	if ( FAILED(m_webTool.Create( lpszWindowName, WS_CHILD | WS_VISIBLE, CRect( 0 , 0,
		rect.right - rect.left + cx, rect.bottom - rect.top), this ) ) )
	{
		return FALSE;
	}
	   
	return TRUE;
}

void CHtmlWnd::OnDestroy() 
{
	m_webTool.Close();
	CWnd::OnDestroy();	
	// TODO: Add your message handler code here
	
}

BEGIN_MESSAGE_MAP(CHtmlWnd, CWnd)
//{{AFX_MSG_MAP(CHtmlWnd)
// NOTE - the ClassWizard will add and remove mapping macros here.
ON_WM_DESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd message handlers


/////////////////////////////////////////////////////////////////////////////
// CNotifyMsg

CNotifyMsg::CNotifyMsg()
{	
	m_nAutoCloseAfterSecond = 0;
}

CNotifyMsg::~CNotifyMsg()
{
	DestroyWindow();
}

BOOL CNotifyMsg::Create( LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	DWORD dwStyle, dwExstyle;	
	dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS ;	
	dwExstyle = WS_EX_TRANSPARENT | WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOPMOST | WS_EX_TOOLWINDOW;
	
   	return CHtmlWnd::Create( _T("NotifyMsg"), lpszWindowName, dwStyle, dwExstyle, rect, pParentWnd, nID, pContext );

}

HRESULT CNotifyMsg::UpdateContent(const CString &strContent, BOOL bURL)
{
	REGISTER_FUNC( CNotifyMsg::UpdateContent(const CString &strContent) );
	HRESULT hr = E_FAIL;

	try
	{
		if ( bURL )
		{
			VERIFY_SUCCEEDED( m_webTool.Navigate2( strContent ) );
		}
		else
		{
			VERIFY_SUCCEEDED( m_webTool.Navigate2MemHtml(strContent) );
		}

		if ( m_nAutoCloseAfterSecond > 0 )
		{
			SetTimer( 1, m_nAutoCloseAfterSecond, NULL );
		}
		
		hr = S_OK;
	}
	my_catch;

	return hr;
	
}

HRESULT	CNotifyMsg::SetAutoClose(int nMilliSecond )
{
	if ( nMilliSecond < 0 )
	{
		return E_FAIL;
	}
	m_nAutoCloseAfterSecond = nMilliSecond;
	
	if ( m_nAutoCloseAfterSecond > 0 )
	{
		SetTimer( 1, m_nAutoCloseAfterSecond, NULL );
	}
	else
	{
		KillTimer(1);
	}
	
	return S_OK;
}

void CNotifyMsg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	if ( IsWindow(GetSafeHwnd()))
	{
		PostMessage( WM_CLOSE, NULL, NULL );
	}
	
	CHtmlWnd::OnTimer(nIDEvent);
}

BEGIN_MESSAGE_MAP(CNotifyMsg, CHtmlWnd)
//{{AFX_MSG_MAP(CNotifyMsg)
// NOTE - the ClassWizard will add and remove mapping macros here.
ON_WM_TIMER()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNotifyMsg message handlers


/////////////////////////////////////////////////////////////////////////////
// CTrayIconAgentWnd

CTrayTool::CTrayIconAgentWnd::CTrayIconAgentWnd()
{
	m_pAgent = NULL;
	m_nCounter = 0;
}

CTrayTool::CTrayIconAgentWnd::~CTrayIconAgentWnd()
{
}


BEGIN_MESSAGE_MAP(CTrayTool::CTrayIconAgentWnd, CWnd)
	//{{AFX_MSG_MAP(CTrayTool::CTrayIconAgentWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_MESSAGE( WM_MY_TRAYICON_NOTIFY , CTrayTool::CTrayIconAgentWnd::OnTrayIconMsg)
		ON_MESSAGE( WM_TIMER , CTrayTool::CTrayIconAgentWnd::OnTimer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTrayIconAgentWnd message handlers

LRESULT CTrayTool::CTrayIconAgentWnd::OnTrayIconMsg(WPARAM wParam, LPARAM lParam)
{
	SET_FUNC_NAME( _T("CTrayTool::CTrayIconAgentWnd::OnTrayIconMsg(WPARAM wParam, LPARAM lParam)") );

	UINT uID = (UINT)wParam;//发出该消息的图标的ID
	UINT uMouseMsg = (UINT)lParam;//鼠标动作	

	try
	{
		VERIFY_TRUE( m_pAgent != NULL );
		VERIFY_TRUE( m_pAgent->m_iconNotifyData.uID == uID);

		// 鼠标右键
		if( uMouseMsg == WM_RBUTTONDOWN )
		{
			if ( m_pAgent->m_RBCallBack.IsInit() )
			{
				WARNING_IF_FAILED(m_pAgent->m_RBCallBack.InvokeCallback( wParam, lParam ));
			}
		}

		// 鼠标右键双击
		if( uMouseMsg == WM_RBUTTONDBLCLK )
		{
			if ( m_pAgent->m_RBDClickCallBack.IsInit() )
			{
				WARNING_IF_FAILED(m_pAgent->m_RBDClickCallBack.InvokeCallback( wParam, lParam ));
			}
		}

		// 鼠标左键
		if( uMouseMsg == WM_LBUTTONDOWN )
		{
			if ( m_pAgent->m_LBCallBack.IsInit() )
			{
				WARNING_IF_FAILED(m_pAgent->m_LBCallBack.InvokeCallback( wParam, lParam ));
			}
		}

		// 鼠标左键双击
		if( uMouseMsg == WM_LBUTTONDBLCLK )
		{
			if ( m_pAgent->m_LBDClickCallBack.IsInit() )
			{
				WARNING_IF_FAILED(m_pAgent->m_LBDClickCallBack.InvokeCallback( wParam, lParam ));
			}
		}
	}
	my_catch;

	return 0;
}

LRESULT CTrayTool::CTrayIconAgentWnd::OnTimer(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	
	UINT nIDEvent = static_cast<UINT>(wParam); 

	try
	{
		VERIFY_TRUE( m_pAgent != NULL );
		VERIFY_TRUE( m_pAgent->m_bIconFlash );
		VERIFY_TRUE( m_pAgent->m_hIconArray.GetSize() != 0 );
		m_nCounter = ( m_nCounter + 1 ) % static_cast<int>(m_pAgent->m_hIconArray.GetSize());
		VERIFY_SUCCEEDED( m_pAgent->ModifyIcon( m_pAgent->m_hIconArray[ m_nCounter ] ) );
	}
	my_catch;

	CWnd::OnTimer(nIDEvent);
	
	return 0L;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayTool::CTrayTool() : m_hNormalIcon(NULL)
{		
//	Init();
	m_nMsgboxcx = 0;
	m_nMsgboxcy = 0;

}

CTrayTool::~CTrayTool() 
{
	Remove();
	m_wndFake.DestroyWindow();	
}

HRESULT CTrayTool::Init()
{
	SET_FUNC_NAME( _T("CTrayTool::Init()") );
	HRESULT hr = E_FAIL;
	USES_CONVERSION;
	
	try
	{
		if ( !IsInit() )
		{
			srand( GetTickCount());

			VERIFY_TRUE( m_wndFake.Create( NULL, NULL, NULL, CRect( 0, 0, 0, 0), CWnd::FromHandle(GetDesktopWindow()), 1) );
			
			VERIFY_SUCCEEDED( CreateNotifyMsgBox() );
			
			m_hNormalIcon = NULL;
			m_bIconFlash = FALSE;

			m_wndFake.m_pAgent = this;
			m_iconNotifyData.cbSize = sizeof( NOTIFYICONDATA );
			m_iconNotifyData.hWnd = m_wndFake.m_hWnd;
			m_iconNotifyData.uID = rand() % 1000 + 1000;
			m_iconNotifyData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP ;
			m_iconNotifyData.uCallbackMessage = WM_MY_TRAYICON_NOTIFY;
			
			m_iconNotifyData.hIcon = m_hNormalIcon ? m_hNormalIcon : AfxGetApp()->LoadStandardIcon(IDI_ASTERISK );
#if ( _MSC_VER < MS_VC_VERSION_2003 )
			_tcscpy( m_iconNotifyData.szTip, _T("") );
#else
			_tcscpy_s( m_iconNotifyData.szTip, _T("") );
#endif
			Add();//向任务栏添加图标			
		}
		else
		{
			const BOOL HAS_INIT(TRUE);
			WARNING_IF_TRUE(HAS_INIT);
		}
		
		hr = S_OK;
	}
	my_catch;
	
	return hr;
}

BOOL CTrayTool::IsInit()
{
	return ( IsWindow( m_wndFake.GetSafeHwnd()) );
}



HRESULT CTrayTool::Add()
{
	REGISTER_FUNC( CTrayTool::Add() );

	HRESULT hr = E_FAIL;
	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_TRUE( Shell_NotifyIcon( NIM_ADD, &m_iconNotifyData) );
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CTrayTool::CreateNotifyMsgBox()
{
	REGISTER_FUNC( CTrayTool::CreateNotifyMsgBox() );
	HRESULT hr = E_FAIL;

	try
	{
		// 创建消息窗口
		int nScreenCx = GetSystemMetrics( SM_CXSCREEN );
		int nScreenCy = GetSystemMetrics( SM_CYSCREEN );
		
		int left = ( m_nMsgboxcx == 0 ) ? ( nScreenCx * 80 / 100 ) : ( nScreenCx - m_nMsgboxcx ); 
		int top = nScreenCy;
		int right = nScreenCx;
		int bottom = ( m_nMsgboxcy == 0 ) ? ( nScreenCy + nScreenCy * 20 / 100 ) : ( nScreenCy + m_nMsgboxcy );
		
		VERIFY_TRUE(m_wndNotifyMsg.Create( _T("123"), CRect(  left, top, right, bottom), &m_wndFake, 1 , NULL));
		
		CWndStyleTool styleTool;
		styleTool.SetLayerWindowStyle( m_wndNotifyMsg.GetSafeHwnd() );
		SetLayeredWindowAttributes( m_wndNotifyMsg.GetSafeHwnd(), 0, (255), LWA_ALPHA);

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT CTrayTool::Remove()
{
	REGISTER_FUNC( CTrayTool::Remove() );
	
	HRESULT hr = E_FAIL;
	try
	{
		if ( IsInit() )
		{
			VERIFY_TRUE( Shell_NotifyIcon( NIM_DELETE, &m_iconNotifyData) );
			if ( IsWindow( m_wndFake.GetSafeHwnd()))
			{
				m_wndFake.DestroyWindow();
			}
		}
		
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CTrayTool::SetTips(const CString &strTips)
{
	REGISTER_FUNC( CTrayTool::ModifyTips(const CString &strTips) );
	
	HRESULT hr = E_FAIL;
	try
	{
		VERIFY_TRUE( IsInit() );
#if ( _MSC_VER < MS_VC_VERSION_2003 )
		_tcscpy( m_iconNotifyData.szTip, strTips );
#else
		_tcscpy_s( m_iconNotifyData.szTip, strTips );
#endif
		VERIFY_TRUE( Shell_NotifyIcon( NIM_MODIFY, &m_iconNotifyData) );
		hr = S_OK;
	}
	my_catch;
	
	return hr;
	
}


HRESULT CTrayTool::ModifyIcon(HICON hIcon)
{
	REGISTER_FUNC( CTrayTool::ModifyIcon(HICON hIcon) );
	
	HRESULT hr = E_FAIL;
	try
	{
		VERIFY_TRUE( hIcon != NULL );
		VERIFY_TRUE( IsInit() );
		m_iconNotifyData.hIcon = hIcon;
		m_iconNotifyData.uFlags &= ~NIF_INFO;
		VERIFY_TRUE( Shell_NotifyIcon( NIM_MODIFY, &m_iconNotifyData) );		
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CTrayTool::SetIconArray( CArray<HICON,HICON>& hIconArray )
{
	REGISTER_FUNC( CTrayTool::SetIconArray( CArray<HICON,HICON>& hIconArray ) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( hIconArray.GetSize() > 0 ); 
		m_hIconArray.RemoveAll();
		m_hIconArray.Append(hIconArray);
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT CTrayTool::StartIconFlash(int nMilliSecond)
{
	REGISTER_FUNC( CTrayTool::StartIconFlash(int nMilliSecond) );
	HRESULT hr = E_FAIL;
	
	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_TRUE( m_hIconArray.GetSize() > 0 );
		VERIFY_TRUE( nMilliSecond > 0 ); 
		m_bIconFlash = TRUE;
		m_wndFake.SetTimer( 1, nMilliSecond, NULL );
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CTrayTool::StopIconFlash()
{
	REGISTER_FUNC( CTrayTool::StopIconFlash() );
	HRESULT hr = E_FAIL;
	
	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_TRUE( m_hIconArray.GetSize() > 0 || m_hNormalIcon != NULL ); 
		m_wndFake.KillTimer( 1 );
		m_wndFake.m_nCounter = 0;
		m_bIconFlash = FALSE;

		if ( m_hNormalIcon != NULL )
		{
			VERIFY_SUCCEEDED( ModifyIcon(m_hNormalIcon) );
		}
		else
		{
			if ( m_hIconArray.GetSize() > 0  )
			{
				VERIFY_SUCCEEDED( ModifyIcon(m_hIconArray[0]) );
			}
		}

		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CTrayTool::SetIcon(HICON hIcon)
{
	REGISTER_FUNC( CTrayTool::SetIcon(HICON hIcon) );
	HRESULT hr = E_FAIL;
	
	try
	{
		VERIFY_SUCCEEDED( ModifyIcon(hIcon) );	
		m_hNormalIcon = hIcon;
		hr = S_OK;
	}
	my_catch;
	
	return hr;

}

HRESULT CTrayTool::PopupMessage(const CString& strMsg, const CString& strTitle, int nMilliSecond, DWORD dwFlag )
{
	REGISTER_FUNC( CTrayTool::PopupMessage(const CString& strMsg, const CString& strTitle, int nMilliSecond, DWORD dwFlag ) );
	HRESULT hr = E_FAIL;
	
	try
	{
		switch( dwFlag )
		{
		case DEFAULT_STYLE:
		
			{
				m_iconNotifyData.uFlags |= NIF_INFO;
				m_iconNotifyData.dwInfoFlags = NIIF_WARNING;   
				m_iconNotifyData.uTimeout = nMilliSecond;

#if ( _MSC_VER < MS_VC_VERSION_2003 )
				_tcscpy( m_iconNotifyData.szInfo, strMsg );		
				_tcscpy( m_iconNotifyData.szInfoTitle, strTitle );	
#else
				_tcscpy_s( m_iconNotifyData.szInfo, strMsg );		
				_tcscpy_s( m_iconNotifyData.szInfoTitle, strTitle );	
#endif
				VERIFY_TRUE( Shell_NotifyIcon( NIM_MODIFY, &m_iconNotifyData) );
				break;
			}

		default:
			{
				int nCx = GetSystemMetrics( SM_CXSCREEN );
				int nCy = GetSystemMetrics( SM_CYSCREEN );
				
				if ( !::IsWindow(m_wndNotifyMsg.GetSafeHwnd() ) )
				{
					CreateNotifyMsgBox();
				}
				if ( ::IsWindow(m_wndNotifyMsg.GetSafeHwnd() ) )
				{
					CRect rect;
					CRect rectOrg;
					CRect rectDest;
					m_wndNotifyMsg.GetWindowRect( rect );
					rectOrg = CRect( nCx - rect.Width(), nCy, nCx, nCy + rect.Height());
					rectDest = rectOrg;
					rectDest.OffsetRect( CSize(0, -rect.Height()) );	
					rectOrg = rectDest;
					m_wndNotifyMsg.SetAutoClose(nMilliSecond);
					m_wndNotifyMsg.UpdateContent( strMsg, TRUE );
					GetAnimateWndTool()->AnimateWindow( m_wndNotifyMsg.m_hWnd, 500, dwFlag, rectOrg, rectDest );
				}
			}
			break;
		}
		hr = S_OK;
	}
	my_catch;

	return hr;

}


HRESULT CTrayTool::SetMsgboxSize(int cx, int cy)
{
	if ( cx <= 0 || cy <= 0 )
	{
		return E_FAIL;
	}

	m_nMsgboxcx = cx;
	m_nMsgboxcy = cy;

	if ( IsInit() )
	{
		CRect rect;
		m_wndNotifyMsg.GetWindowRect( &rect );
		m_wndNotifyMsg.MoveWindow( rect.left, rect.top, cx, cy, TRUE );
	}

	return S_OK;
}
