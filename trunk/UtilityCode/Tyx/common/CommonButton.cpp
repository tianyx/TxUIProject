// CommonButton.cpp : implementation file
//

#include "stdafx.h"
#include "CommonButton.h"
#include "SkinFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonButton

CCommonButton::CCommonButton()
{
	CMultiBitmapButtonEx::LoadBitmaps(Common_Button);
}

CCommonButton::~CCommonButton()
{
}


BEGIN_MESSAGE_MAP(CCommonButton, CMultiBitmapButtonEx)
	//{{AFX_MSG_MAP(CCommonButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonButton message handlers
