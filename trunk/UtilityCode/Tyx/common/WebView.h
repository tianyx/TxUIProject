#if !defined(AFX_WEBVIEW_H__DD9D5B89_D781_4924_A2AA_EDDB6DAA2E70__INCLUDED_)
#define AFX_WEBVIEW_H__DD9D5B89_D781_4924_A2AA_EDDB6DAA2E70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWebView html view


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

//--------------------------------
//Web命令类型定义
//--------------------------------
#define	 WEB_DATATYPE_DATA		0X01  //Web数据
#define  WEB_DATATYPE_SCROLL	0X02  //滚动视图
//--------------------------------

#define MSG_WEB_USER_NEW_ADDR WM_USER + 206
#define MSG_WEB_NAVIGATE_NEW_ADDR WM_USER + 207
#define MSG_WEB_NAVIGATE_FINISH_NAVIGATE WM_USER + 208
class CWebView : public CHtmlView
{
protected:
	DECLARE_DYNCREATE(CWebView)

// html Data
public:
	CWebView();           // protected constructor used by dynamic creation
	virtual ~CWebView();
	//{{AFX_DATA(CWebView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	struct STRUCT_WEB_VIEWDATA{
		BYTE	bCommand;
		DWORD	dwFlags;
		WORD    wTotalLen;
		WORD    wUrlLen;
		WORD    wTargetFrameNameLen;
		WORD    wHeaderLen;
		WORD    wPostDataLen;
		//data
	};
// Attributes
public:
	struct WEB_BROWSE_CONTENT{
		
		int			nFlags;
		CString		strUrl;
		CString		strTargetFrameName;
		CString		strHeader;
		CByteArray	arrByts;
		
	}m_BrowseContent;

	//----------------
	//滚动视图
	//----------------
	struct WEB_VIEWSCROLL{
		BYTE	bCommand;	//=0X02 命令
		POINT	ptScroll;	//滚动位置
		SIZE	szTotal;	//滚动尺寸
	};

	BOOL		m_bClick;
	static BOOL		m_bWantNewUrl;
	
	CString		m_strMouseUrl;
	CString     m_strOldUrl;
	DWORD       m_dwViewID;

	CHAR        m_szUrl[ MAX_PATH ];
	CHAR        m_szTargetFrameName[MAX_PATH];
	CHAR        m_szHeader[ MAX_PATH ];

	BOOL        m_bEnableGoBack;
	BOOL        m_bEnableGoForward;

// Operations
public:
	void ProcessData(PBYTE pbData, DWORD dwLen);
	void Reset();
	DWORD GetViewID();
	void  SetViewID( DWORD dwViewID );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebView)
	public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCommandStateChange(long nCommand, BOOL bEnable);
	virtual void OnStatusTextChange(LPCTSTR lpszText);
	virtual void OnTitleChange(LPCTSTR lpszText);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	OnScrollCallBack();
	void	HandleScrollView(PBYTE pData, DWORD dwLen);
	BOOL  HandleData( PBYTE pbData,UINT unDataLen );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWebView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CPoint m_ptScrollPos;
	const UINT m_nIDTimer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBVIEW_H__DD9D5B89_D781_4924_A2AA_EDDB6DAA2E70__INCLUDED_)
