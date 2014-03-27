#if !defined(AFX_LIENSERIGHTDLG_H__A05243FB_33EB_4F3B_A0E6_8EC6ABA995C9__INCLUDED_)
#define AFX_LIENSERIGHTDLG_H__A05243FB_33EB_4F3B_A0E6_8EC6ABA995C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LienseRightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLienseRightDlg dialog

class CLienseRightDlg : public CDialog
{
// Construction
public:
	void SetSerialNum(CString strSerialNum);
	CLienseRightDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLienseRightDlg)
	enum { IDD = IDD_LIENSERIGHT_DIALOG };
	CString	m_strSerialNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLienseRightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	CString m_strSerialNum;

	// Generated message map functions
	//{{AFX_MSG(CLienseRightDlg)
	afx_msg void OnCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIENSERIGHTDLG_H__A05243FB_33EB_4F3B_A0E6_8EC6ABA995C9__INCLUDED_)
