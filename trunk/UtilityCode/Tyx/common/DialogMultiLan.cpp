// DialogMultiLan.cpp : implementation file
//

#include "stdafx.h"
#include "DialogMultiLan.h"
#include "Global.h"
#include "LanManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMultiLan dialog
IMPLEMENT_DYNAMIC(CDialogMultiLan, CDialog)

CDialogMultiLan::CDialogMultiLan(UINT nResourceID,LPCTSTR szName,CWnd* pParent /*=NULL*/)
	: CDialog(nResourceID, pParent),
	m_strDlgName(szName)
{
	//{{AFX_DATA_INIT(CDialogMultiLan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogMultiLan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMultiLan)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMultiLan, CDialog)
	//{{AFX_MSG_MAP(CDialogMultiLan)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_LANGUAGE_CHANDGED, OnLanguageChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMultiLan message handlers
void CDialogMultiLan::LoadLanguage(BOOL bRead)
{
	if(bRead)
	{
		g_lanManager.WndReadLan(m_strDlgName, this);
		ChildWndReloadLan();
	}
	else
	{
		g_lanManager.WndWriteLan(m_strDlgName, this);
	}
}

LRESULT CDialogMultiLan::OnLanguageChanged(WPARAM, LPARAM)
{
	LoadLanguage(TRUE);

	return 0;
}


LRESULT CDialogMultiLan::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CDialog::WindowProc(message, wParam, lParam);
	if (message == WM_INITDIALOG)
	{
		if (!g_lanManager.m_bRead)
		{
			if (g_lanManager.GetDefLanIndex() == g_lanManager.GetCurLanIndex())
				LoadLanguage(FALSE);
		}
		else
			LoadLanguage(TRUE);
	}

	return lResult;
}