#if !defined(AFX_BCMENUBAR_H__6216EE5A_FE4E_4548_8233_AC3B0FFACCF0__INCLUDED_)
#define AFX_BCMENUBAR_H__6216EE5A_FE4E_4548_8233_AC3B0FFACCF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuBar.h : header file
//
#include "DBMenu.h"
#include "dbstretchimage.h"

#define MSG_BCMENUBAR_NEED_UPDATE WM_USER + 600    //wparam = hmenu
/////////////////////////////////////////////////////////////////////////////
// CBCMenuBar window

class CBCMenuBar : public CToolBar
{
// Construction
public:
	CBCMenuBar();

// Attributes
public:
	
// Operations
public:
	void InitMenuID(int nMenuID);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCMenuBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
	virtual ~CBCMenuBar();

	CMenu* GetMenuBarMenu();
	// Generated message map functions
protected:
	//{{AFX_MSG(CBCMenuBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnTBDropDown(NMHDR * pNotifyStruct, LRESULT *result);
	afx_msg void OnDrawTBItem(NMHDR * pNotifyStruct, LRESULT *result);
	afx_msg LRESULT OnLanguageChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

protected:
	virtual void LoadLanguage(BOOL bRead);

protected:
	void UpdateMenu(HMENU hmenu);
	void SendMenuCmd(UINT menuID);
private:
	CSize m_szButton;
	enum {MENU_ITEM_COUNT = 6};
	//CBitmap m_bmpsBtn[MENU_ITEM_COUNT][3];
	DBStretchImage m_bmpBtn[3];

	int m_iHeight;
	CBrush m_bruBkgnd;
	int m_itemDrop;
	HMENU m_hMenu;
	int m_nMenuID;
	CMenu m_Menu;
private:
	DBMenu*	m_pMenuHelper;
	afx_msg void OnDestroy();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCMENUBAR_H__6216EE5A_FE4E_4548_8233_AC3B0FFACCF0__INCLUDED_)
