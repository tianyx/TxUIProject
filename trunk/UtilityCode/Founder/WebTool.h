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
	
	//					�����ڶ���������
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
		BOOL			bTaskStart;		// ���ڶ����Ƿ�������
		DWORD			dwStyleEx;		// ����ԭ����չ���
		CRect			rectDest;		// ���������󣬴��ڵ�Ŀ��λ��
		CRect			rectOrg;		// ������ʼǰ�����ڵ�ԭʼλ��		
		int				nFadeStart;		// ������ʼǰ�����ڵ�͸����
		int				nFadeEnd;		// ���������󣬴��ڵ�͸����
		
		DWORD			timeTaskBegin;	// ����ʼʱ��, GetTickCount
		DWORD			dwTaskDuration;	// �������ʱ��
		
		DWORD			dwFlag;			// ��־λ������ָʾ���ڶ����ķ���ƶ������뵭����������
		HWND			hWnd;			// �����Ķ��󴰿�

		UINT			uMsg;			// ������ɺ��� hWnd ��ָ��Ĵ��ڷ��͵���Ϣ
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

//			ʹ��CWebTool��֮ǰ�����ȵ��� CLibraryTool::InitOleLibrary�������г�ʼ��
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

	// �麯��
	public:
		virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);

public:
	// bUI ��ʾ�Ƿ񴴽�UI
	CWebTool(BOOL bUIMode = TRUE);
public:
	virtual ~CWebTool(void);

	// operation
public:

	//				��ʾ��Դ�е�HTML, bAsync ��ʾ�Ƿ�Ϊ�첽����
	HRESULT			Navigate2ResHtml( UINT nID, BOOL bAsync = FALSE );
	//				��ʾ�ڴ��е�HTML, bAsync ��ʾ�Ƿ�Ϊ�첽����
	HRESULT			Navigate2MemHtml( CString strHtmlSource, BOOL bJustBody = TRUE, BOOL bAsync = FALSE);

	//				bAsync ��ʾ�Ƿ�Ϊ�첽����
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
	int				m_nAutoCloseAfterSecond;			// ָ��ʱ������Զ��رմ���
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
		// ������ʾ��Ϣʱ����Ϣ���ڵķ��
		enum{ DEFAULT_STYLE = 0 /*ϵͳĬ�Ϸ��*/, FADE_STYLE/*���뽥��*/, SLIDE_STYLE /*����*/};
		
	public:
		CTrayTool();
		virtual ~CTrayTool();
		
	public:
		// ����Ҽ�
		template<class T>
			HRESULT				SetRBtnCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_RBCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		// ����Ҽ�˫��
		template<class T>
			HRESULT				SetRBtnDClickCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_RBDClickCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		
		// ������
		template<class T>
			HRESULT				SetLBtnCallBack( LRESULT (T::*pFunc)(WPARAM, LPARAM), T* pThis  )
		{ HRESULT hr = E_FAIL; WARNING_IF_FAILED( hr = m_LBCallBack.SetCallback( pFunc, pThis) );	return hr; } // yes, = not ==
		
		// ������˫��
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

		CNotifyMsg			m_wndNotifyMsg;				// ��Ϣ����
		int					m_nMsgboxcx;				// ��Ϣ���ڵĿ��
		int					m_nMsgboxcy;				// ��Ϣ���ڵĸ߶�

		CTrayIconAgentWnd	m_wndFake;
		
		NOTIFYICONDATA		m_iconNotifyData;
		CArray<HICON,HICON>	m_hIconArray;
		HICON				m_hNormalIcon;				// ��ͨ״̬�µ�Icon
		BOOL				m_bIconFlash;				// Icon �Ƿ�����˸״̬
		
		CDelegateTool		m_RBCallBack;				// ����Ҽ�
		CDelegateTool		m_RBDClickCallBack;			// ����Ҽ�˫��
		CDelegateTool		m_LBCallBack;				// ������
		CDelegateTool		m_LBDClickCallBack;			// ������˫��
};