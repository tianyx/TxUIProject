#if !defined(AFX_DIALOGMULTILAN_H__47C5412C_3287_47DA_A029_F45E95D7DB58__INCLUDED_)
#define AFX_DIALOGMULTILAN_H__47C5412C_3287_47DA_A029_F45E95D7DB58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMultiLan.h : header file
//
#include "DlgHDBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogMultiLan dialog

class CDialogMultiLan : public CDialog
{
	DECLARE_DYNAMIC(CDialogMultiLan)

// Construction
public:
	CDialogMultiLan( UINT nResourceID,LPCTSTR szName,CWnd* pParent = NULL);   // standard constructor

	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );

// Dialog Data
	//{{AFX_DATA(CDialogMultiLan)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	virtual void LoadLanguage(BOOL bRead);

	//add by tyx for child reload language;
	virtual void ChildWndReloadLan(){}; 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMultiLan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMultiLan)
	//}}AFX_MSG
	afx_msg LRESULT OnLanguageChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

protected:
	const CString   m_strDlgName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMULTILAN_H__47C5412C_3287_47DA_A029_F45E95D7DB58__INCLUDED_)
