#if !defined(AFX_DBSLIDERCTRL_H__941BEC3F_B56B_41E2_A0E9_801BDA958C46__INCLUDED_)
#define AFX_DBSLIDERCTRL_H__941BEC3F_B56B_41E2_A0E9_801BDA958C46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBSliderCtrl.h : header file
//

#include "DBStretchImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDBSliderCtrl window

class CDBSliderCtrl : public CWnd
{
// Construction
public:
	CDBSliderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBSliderCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(LPCTSTR lpszBmpChannelLeft,
				LPCTSTR lpszBmpChannelRight,
				LPCTSTR lpszBmpThumbs,
				const RECT& rect,
				CWnd* pParentWnd,
				UINT nID,
				BOOL bHorz = TRUE );

	void VerifyPos();
	void GetChannelRect( LPRECT lprc ) const;
	void GetThumbRect( LPRECT lprc ) const;
	int GetLineSize() const;
	void SetLineSize(int nSize);
	int GetPageSize() const;
	void SetPageSize(int nPage);
	int GetPos() const;
	void SetPos(int nPos);
	void SetRange( int nMin, int nMax, BOOL bRedraw = FALSE);
	void GetRange(int &nMin, int &nMax) const;
	virtual ~CDBSliderCtrl();
// 	void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);
// 	void Invalidate(BOOL bErase = TRUE);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDBSliderCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void Initialize();

private:

	COLORREF m_clrBkgnd;	//背景颜色
	BOOL m_bPushed;			//鼠标是否按下
	CSize m_szThumb;	

	CPoint m_ptDragPos;	//开始拖动时光标相对于Thumb中心的位置
	
	BOOL m_bHorz;
	DBStretchImage m_imageChannelRight;
	DBStretchImage m_imageChannelLeft;
	DBStretchImage m_bmpsThumb[4];
	
	int m_iPos;
	int m_iMin;
	int m_iMax;
	int m_iPageSize;
	int m_iLineSize;

	enum 
	{	
		thumbStateNormal,
		thumbStateSel,
		thumbStateHover,
		thumbStateDisable
	} m_thumbState;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBSLIDERCTRL_H__941BEC3F_B56B_41E2_A0E9_801BDA958C46__INCLUDED_)
