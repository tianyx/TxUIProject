// LienseRightDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "FCVSServer.h"
#include "LienseRightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLienseRightDlg dialog


CLienseRightDlg::CLienseRightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLienseRightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLienseRightDlg)
	m_strSerialNum = _T("");
	//}}AFX_DATA_INIT
}


void CLienseRightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLienseRightDlg)
	DDX_Text(pDX, IDC_LIENSERIGHT_EDIT, m_strSerialNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLienseRightDlg, CDialog)
	//{{AFX_MSG_MAP(CLienseRightDlg)
	ON_BN_CLICKED(IDOK, OnCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLienseRightDlg message handlers

void CLienseRightDlg::OnCopy() 
{
	// TODO: Add your control notification handler code here

	if ( !OpenClipboard() )
	{
		AfxMessageBox( "Cannot open the Clipboard" );
		return;
	}
	// Remove the current Clipboard contents
	if( !EmptyClipboard() )
	{
		AfxMessageBox( "Cannot empty the Clipboard" );
		return;
	}
	// ...
	// Get the currently selected data
	// ...
	// For the appropriate data formats...

	LPTSTR  lptstrCopy; 
    HGLOBAL hglbCopy; 
    
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, 
		(m_strSerialNum.GetLength()) * sizeof(TCHAR)+1); 
	if (hglbCopy == NULL) 
	{ 
		CloseClipboard(); 
		return ; 
	} 

	lptstrCopy =(char*)GlobalLock(hglbCopy); 
	memcpy(lptstrCopy, m_strSerialNum.GetBuffer(m_strSerialNum.GetLength()), 
		m_strSerialNum.GetLength()); 
	lptstrCopy[m_strSerialNum.GetLength()+1] = (TCHAR)0;    // null character 
	GlobalUnlock(hglbCopy); 

    m_strSerialNum.ReleaseBuffer();

	if ( ::SetClipboardData( CF_TEXT,hglbCopy) == NULL )
	{
		AfxMessageBox( "Unable to set Clipboard data" );
		GlobalFree(hglbCopy);
		CloseClipboard();
		return;
	}
	// ...
    GlobalFree(hglbCopy);
	CloseClipboard();

}

void CLienseRightDlg::SetSerialNum(CString strSerialNum)
{
   m_strSerialNum = strSerialNum;
//   UpdateData();
}
