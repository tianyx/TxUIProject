

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBStretchImage.h"

// BitmapButtonEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapButtonExwindow

class CBitmapButtonEx: public CButton
{
// Construction
public:
	CBitmapButtonEx();

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	COLORREF SetTextColor(COLORREF clrText);
	void LoadBitmaps(LPCTSTR lpszBitmaps, BOOL bAutoStretch = TRUE);
	
	void LoadBitmaps(LPCTSTR lpszBmpNormal, LPCTSTR lpszBmpSel,
		LPCTSTR lpszBmpHover, LPCTSTR lpszBmpDisable, 
		BOOL bAutoStretch = TRUE);
	
	virtual ~CBitmapButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapButtonEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:

	DBStretchImage m_imageNormal;	//正常状态
	DBStretchImage m_imageSel;		//按下状态
	DBStretchImage m_imageHover;	//Hover状态
	DBStretchImage m_imageDisable;	//Disable状态
	
	BOOL m_bMouseOver;	//鼠标是否位于按钮上方

protected:
	BOOL m_bHasToolTip;
	void DrawCheckButton(CDC *pDC, UINT style);
	BOOL m_bCheckBtn;
	BOOL m_bCheck;
	BOOL m_bAutoStretch;
	CFont m_font;
	COLORREF m_clrText;
	BOOL m_bPushed;
	virtual void DrawButton(CDC *pDC,UINT style);
public:
	//added by tianyx 2009-5-25--17:54 for
	//for drop btns
	CBitmapButtonEx* m_pNaborBtn;
	CBitmapButtonEx* m_pNaborBtn2; //for drop btn;
	int m_nNaborSetState; 
	BOOL m_bNaborCalledReDraw;
	void DrawWithNabourState(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//ended by tianyx

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
