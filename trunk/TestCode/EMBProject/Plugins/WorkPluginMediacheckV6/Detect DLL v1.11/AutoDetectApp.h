// AutoDetectApp.h : main header file for the AutoDetect DLL
//

#if !defined(AFX_AUTODETECTAPP_H__435D102F_CD93_4EAA_BAB6_E3EE97430CAF__INCLUDED_)
#define AFX_AUTODETECTAPP_H__435D102F_CD93_4EAA_BAB6_E3EE97430CAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoDetectApp
// See AutoDetectApp.cpp for the implementation of this class
//

class CAutoDetectApp : public CWinApp
{
public:
	CAutoDetectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoDetectApp)
	virtual BOOL InitInstance();
	virtual int ExitInstance(); // return app exit code

	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAutoDetectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTODETECTAPP_H__435D102F_CD93_4EAA_BAB6_E3EE97430CAF__INCLUDED_)
