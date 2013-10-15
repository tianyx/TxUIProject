#if !defined(AFX_MESSAGEBOX_H__19B4605F_8217_4C07_93F3_276BFD5763BE__INCLUDED_)
#define AFX_MESSAGEBOX_H__19B4605F_8217_4C07_93F3_276BFD5763BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageBox dialog
#include "resource.h"
#include "DBDialog.h"
#include "CommonButton.h"
#include "DlgMovedBase.h"
#include "HDStatic.h"
#include "DlgCommonContainer.h"
class CDlgLoginMsg;
class CMessageBox : public CDlgCommonContainer 
{

	DECLARE_DYNAMIC(CMessageBox)
// Construction
public:
	CMessageBox(LPCTSTR szMsg,UINT unType,DWORD dwWaitTime,CWnd* pParent = NULL);   // standard constructor
	~CMessageBox(void);

	DECLARE_MESSAGE_MAP()

// Dialog Data
	//{{AFX_DATA(CMessageBox)
	enum { IDD = IDD_DIALOG_MSG };

public:
	CStatic	m_staticIcon;
	CHDStatic	m_static;
	CString	m_strPrompt;
	//}}AFX_DATA
	UINT    m_unType;
	HICON   m_hIcon;
	DWORD   m_dwWaitTime;
	BOOL	m_bCenterScreen;
	COLORREF m_crfTitleClr;
	BOOL m_bTopMost;

private:
	static HWND m_hWndLast;
public:
	static void WaitForLastBoxClose(DWORD dwTimeOut = 5000);
	static BOOL IsShowing();
public:
	void CenterScreen();
	void ReLayout();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CMultiBitmapButtonEx	m_btnOK;
	CMultiBitmapButtonEx	m_btnCancel;
	DBStretchImage			m_imgQAIcon;
	DBStretchImage			m_imgErrIcon;
	DBStretchImage			m_imgInfoIcon;
	DBStretchImage*			m_pImgCurrIcn;

	UINT m_nIcnType;
// Implementation
protected:
	void SetIcon();
	// Generated message map functions
	//{{AFX_MSG(CMessageBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void PostNcDestroy();

public:
	void SetSpecialHeader(CString& strSpecialIn);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEBOX_H__19B4605F_8217_4C07_93F3_276BFD5763BE__INCLUDED_)
