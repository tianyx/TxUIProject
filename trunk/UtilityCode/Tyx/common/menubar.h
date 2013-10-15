#if !defined(AFX_MENUBAR_H__6216EE5A_FE4E_4548_8233_AC3B0FFACCF0__INCLUDED_)
#define AFX_MENUBAR_H__6216EE5A_FE4E_4548_8233_AC3B0FFACCF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuBar window

class CMenuBar : public CToolBar
{
// Construction
public:
	CMenuBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
	virtual ~CMenuBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMenuBar)
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

private:
	CSize m_szButton;
	enum {MENU_ITEM_COUNT = 6};
	CBitmap m_bmpsBtn[MENU_ITEM_COUNT][3];
	int m_iHeight;
	CBrush m_bruBkgnd;
	int m_itemDrop;
	HMENU m_hMenu;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUBAR_H__6216EE5A_FE4E_4548_8233_AC3B0FFACCF0__INCLUDED_)
