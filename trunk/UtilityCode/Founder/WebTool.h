#pragma once
#include <atlbase.h>
#include <Mshtml.h>
#include <comdef.h> 


class CWndStyleTool  
{
public:
	HRESULT				RemoveLayerWindowStyle(HWND hWnd);
	HRESULT				SetLayerWindowStyle(HWND hWnd);
	HRESULT				RemoveToolWindowStyle(HWND hWnd);
	
	//					ToolWindow style
	HRESULT				SetToolWindowStyle(HWND hWnd);
	
	//					将窗口钉在桌面上
	HRESULT				SetPinToDesktopStyle(HWND hWnd ,HWND& hOldParent);	
	HRESULT				SetPinToDesktopStyle( HWND hWnd );
	
	CWndStyleTool();
	virtual ~CWndStyleTool();
	
protected:
	HRESULT				_AddStyle( HWND hWnd, DWORD dwFlag, DWORD dwStyle);
	HRESULT				_RemoveStyle(HWND hWnd, DWORD dwFlag, DWORD dwStyle);
};


class CAnimateWndTool 
{
	
	
protected:
	enum { WM_ANIMATEWNDTOOL_ADD_TASK = ( WM_USER + 200 ) };
	enum { WM_ANIMATEWNDTOOL_SETTIMERINTERVAL = ( WM_USER + 201 ) };
	
	typedef struct _tagCustomdata
	{		
		_tagCustomdata() 
		{ 
			hWnd			= NULL;						
			dwFlag			= 0;
			dwTaskDuration	= 0;
			rectDest	= CRect( 0, 0, 0, 0);
			rectOrg		= CRect( 0, 0, 0, 0);
			nFadeStart	= 100;
			nFadeEnd	= 100;
			bTaskStart	= FALSE;
			uMsg		= 0;
		}
		BOOL			bTaskStart;		// 窗口动画是否已启动
		DWORD			dwStyleEx;		// 窗口原有扩展风格
		CRect			rectDest;		// 动画结束后，窗口的目标位置
		CRect			rectOrg;		// 动画开始前，窗口的原始位置		
		int				nFadeStart;		// 动画开始前，窗口的透明度
		int				nFadeEnd;		// 动画结束后，窗口的透明度
		
		DWORD			timeTaskBegin;	// 任务开始时间, GetTickCount
		DWORD			dwTaskDuration;	// 任务持续时间
		
		DWORD			dwFlag;			// 标志位，用于指示窗口动画的风格，移动，淡入淡出还是其他
		HWND			hWnd;			// 操作的对象窗口

		UINT			uMsg;			// 动画完成后，向 hWnd 所指向的窗口发送的消息
	};
	
	class CAnimateWndToolAgent : public CWnd
	{
	protected:					
		CArray<_tagCustomdata, _tagCustomdata&> m_CustomDataArray;
		// Construction
	public:
		CAnimateWndToolAgent();
		
		// Attributes
	public:
		
		// Operations
	public:
		
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CAnimateWndToolAgent)
		//}}AFX_VIRTUAL
		
		// Implementation
	public:
		virtual ~CAnimateWndToolAgent();
		
	protected:
		int					m_nTimerInterval;
		int					m_nCustomDataIndex;
		// Generated message map functions
	protected:
		//{{AFX_MSG(CAnimateWndToolAgent)
		// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG
		afx_msg LRESULT OnAddtask( WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT SetTimerInterval( WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnTimer(WPARAM wParam, LPARAM lParam) ;
		DECLARE_MESSAGE_MAP()
	};
	
	/////////////////////////////////////////////////////////////////////////////
		
	friend class				CAnimateWndTool::CAnimateWndToolAgent;
	friend CAnimateWndTool*		GetAnimateWndTool();
	
	public:
		HRESULT					AnimateWindow( HWND hWnd, DWORD dwTime, DWORD dwFlags, const CRect& rectOrg, const CRect& rectDest, UINT uMsg = 0);
		HRESULT					SetTimerInterval(int nTimerInterval);
		
		// Construction
	public:
		CAnimateWndTool();
		virtual ~CAnimateWndTool();
		
	protected:
		CAnimateWndToolAgent	m_agent;
		
};


extern CAnimateWndTool*			GetAnimateWndTool();

//			使用CWebTool类之前，请先调用 CLibraryTool::InitOleLibrary函数进行初始化
class CWebTool
{

protected:
	class CWebToolAgent : public CWnd
	{
		// Construction
	public:
		CWebToolAgent( CWebTool* pWebTool);

		// Attributes
	public:

		// Operations
	protected:
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CWebToolAgent)
		virtual void OnDocumentComplete(LPCTSTR lpszURL); 
		virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
		virtual void BeforeNavigate2(	LPDISPATCH /* pDisp */, VARIANT* URL,
			VARIANT* Flags, VARIANT* TargetFrameName,
										VARIANT* PostData, VARIANT* Headers, BOOL* Cancel	);
		//}}AFX_VIRTUAL

		// Implementation
	public:
		virtual ~CWebToolAgent();

	protected:
		CWebTool*		m_pWebTool;
		// Generated message map functions
	protected:
		//{{AFX_MSG(CWebToolAgent)
		// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
		DECLARE_EVENTSINK_MAP()
	};
	
	friend class CWebToolAgent;

	// 虚函数
	public:
		virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

public:
	// bUI 表示是否创建UI
	CWebTool(BOOL bUIMode = TRUE);
public:
	virtual ~CWebTool(void);

	// operation
public:

	//				显示资源中的HTML, bAsync 表示是否为异步操作
	HRESULT			Navigate2ResHtml( UINT nID, BOOL bAsync = FALSE );
	//				显示内存中的HTML, bAsync 表示是否为异步操作
	HRESULT			Navigate2MemHtml( CString strHtmlSource, BOOL bJustBody = TRUE, BOOL bAsync = FALSE);

	//				bAsync 表示是否为异步操作
	HRESULT			Navigate2(const CString& strURL, DWORD dwFlags = 0, const CString& strTargetFrameName = _T("") ,const CString& strHeaders = _T("") ,
								LPVOID lpvPostData = NULL , DWORD dwPostDataLen = 0 , BOOL bAsync = FALSE);
public:
	HRESULT			SetBodyStyle( LPCTSTR lpszAttributeName, LPCTSTR lpszAttributeValue);
	HRESULT			GetBodyStyleInterface( CComPtr<IHTMLStyle>& spStyle );
public:
	HRESULT			GetWebBrowserPtr( CComPtr<IWebBrowser2>& spBrowserApp);
	HRESULT			GetHtmlDocumentPtr( CComPtr<IHTMLDocument2>& spHTMLDocument );

	HRESULT			ExecScript( const CString& strScript, const CString& strLanguage = _T("JavaScript"));
	HRESULT			GetJScript(CComPtr<IDispatch>& spDisp);
	HRESULT			CallJScript(const CString& strFunc, const CStringArray &paramArray, CComVariant *pVarResult);
	HRESULT			CallJScript(const CString& strFunc,CComVariant* pVarResult);
	HRESULT			CallJScript(const CString& strFunc,const CString& strArg1, CComVariant* pVarResult);
public:
	HRESULT			Close();
	HRESULT			Create( const CString& strWindowName, DWORD dwStyle, CRect rect, CWnd* pParent/*, UINT nID*/);

public:
	CWnd*			GetWnd();
	//operation
protected:
	HRESULT			_Init();
	// attribute
protected:
	BOOL			m_bInit;
	BOOL			m_bUIMode;
	BOOL			m_bHtmlLoad;
	CWebToolAgent	m_wndBrowser;
	CComQIPtr<IWebBrowser2>	m_spBrowserApp;
	CComPtr<IHTMLDocument2> m_spHTMLDocument;
};


/////////////////////////////////////////////////////////////////////////////
// CHtmlWnd window
class CHtmlWnd : public CWnd
{
	// Construction
public:
	CHtmlWnd();
	
	// Attributes
public:
	
	// Operations
public:
	BOOL			Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, DWORD dwExstyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
		
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlWnd)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CHtmlWnd();
	
protected:
	CWebTool		m_webTool;

	// Generated message map functions
protected:
	//{{AFX_MSG(CHtmlWnd)
	// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CNotifyMsg window

class CNotifyMsg : public CHtmlWnd
{
	// Construction
public:
	CNotifyMsg();
	virtual ~CNotifyMsg();
	
	// Attributes
public:
	
	// Operations
public:
	BOOL			Create( LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) ;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotifyMsg)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	HRESULT			UpdateContent(const CString& strContent, BOOL bURL = FALSE);
	HRESULT			SetAutoClose(int nMilliSecond );
	
	
protected:	
	int				m_nAutoCloseAfterSecond;			// 指定时间过后，自动关闭窗口
	// Generated message map functions
protected:
	//{{AFX_MSG(CNotifyMsg)
	// NOTE - the ClassWizard will add and remove member functions here.	
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#define		WM_MY_TRAYICON_NOTIFY		( WM_USER + 201)

class CTrayTool  
{
	class CTrayIconAgentWnd : public CWnd
	{
		// Construction
	public:
		CTrayIconAgentWnd();
	public:
		CTrayTool*		m_pAgent;
		int				m_nCounter;
	protected:
		afx_msg LRESULT OnTrayIconMsg(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT	OnTimer(WPARAM wParam, LPARAM lParam);
	public:
		virtual ~CTrayIconAgentWnd();	
		DECLARE_MESSAGE_MAP()
	};
	
	friend class CTrayTool::CTrayIconAgentWnd;
	
	public:
		// 弹出提示消息时，消息窗口的风格
		enum{ DEFAULT_STYLE = 0 /*系统默认风格*/, FADE_STYLE/*渐入渐出*/, SLIDE_STYLE /*滑行*/};
		
	public:
		CTrayTool();
		virtual ~CTrayTool();
		
	public:
		// 鼠标右键
		template<class T>
			HRESULT				SetRBtnCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_RBCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		// 鼠标右键双击
		template<class T>
			HRESULT				SetRBtnDClickCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_RBDClickCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		
		// 鼠标左键
		template<class T>
			HRESULT				SetLBtnCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_LBCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		// 鼠标左键双击
		template<class T>
			HRESULT				SetLBtnDClickCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_LBDClickCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		
	public:
		HRESULT				SetMsgboxSize( int cx, int cy);
		HRESULT				PopupMessage(const CString& strMsg, const CString& strTitle = _T(""), int nMilliSecond = 2000, DWORD dwFlag = DEFAULT_STYLE);
		
		HRESULT				SetIcon( HICON hIcon );
		HRESULT				SetIconArray( CArray<HICON,HICON>& hIconArray );
		
		HRESULT				StopIconFlash();
		HRESULT				StartIconFlash( int nMilliSecond );		
		
		HRESULT				SetTips( const CString& strTips);
		
		HRESULT				Remove();
		
		HRESULT				Init();
		BOOL				IsInit();
		
	protected:
		HRESULT				CreateNotifyMsgBox();
		HRESULT				Add();
		HRESULT				ModifyIcon( HICON hIcon);	
		
		
	protected:

		CNotifyMsg			m_wndNotifyMsg;				// 消息窗口
		int					m_nMsgboxcx;				// 消息窗口的宽度
		int					m_nMsgboxcy;				// 消息窗口的高度

		CTrayIconAgentWnd	m_wndFake;
		
		NOTIFYICONDATA		m_iconNotifyData;
		CArray<HICON,HICON>	m_hIconArray;
		HICON				m_hNormalIcon;				// 普通状态下的Icon
		BOOL				m_bIconFlash;				// Icon 是否处于闪烁状态
		
		CDelegateTool		m_RBCallBack;				// 鼠标右键
		CDelegateTool		m_RBDClickCallBack;			// 鼠标右键双击
		CDelegateTool		m_LBCallBack;				// 鼠标左键
		CDelegateTool		m_LBDClickCallBack;			// 鼠标左键双击
};