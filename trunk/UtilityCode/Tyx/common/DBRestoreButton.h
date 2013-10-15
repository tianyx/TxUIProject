// DBRestoreButton.h: interface for the DBRestoreButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBRESTOREBUTTON_H__F1CDA0E3_04A4_4ADE_9564_C3CC34806741__INCLUDED_)
#define AFX_DBRESTOREBUTTON_H__F1CDA0E3_04A4_4ADE_9564_C3CC34806741__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBButton.h"

class DBRestoreButton : public DBButton  
{
public:
	BOOL m_bMinimized;

	void Create(CRect &rect, CWnd *pWndParent,
		UINT nID, UINT nIDRestore,
		LPCTSTR lpszBitmaps, LPCTSTR lpszBmpsRestores);

	BOOL m_bMaximized;
	void Exchange();
	DBRestoreButton();
	virtual ~DBRestoreButton();

protected:
	void Init();
	UINT m_nIDTemp;
	CBitmap m_bmpsRestore[4];
};

#endif // !defined(AFX_DBRESTOREBUTTON_H__F1CDA0E3_04A4_4ADE_9564_C3CC34806741__INCLUDED_)
