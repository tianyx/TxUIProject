#if !defined(AFX_COMMONBUTTON_H__9E734E72_31DB_419E_8F63_83C2CD2C0E87__INCLUDED_)
#define AFX_COMMONBUTTON_H__9E734E72_31DB_419E_8F63_83C2CD2C0E87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonButton.h : header file
//

#include "Mutibitmapbuttonex.h"
/////////////////////////////////////////////////////////////////////////////
// CCommonButton window

class CCommonButton : public CMultiBitmapButtonEx
{
// Construction
public:
	CCommonButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommonButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommonButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONBUTTON_H__9E734E72_31DB_419E_8F63_83C2CD2C0E87__INCLUDED_)
