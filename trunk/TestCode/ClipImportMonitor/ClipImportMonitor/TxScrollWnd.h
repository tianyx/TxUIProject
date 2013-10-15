/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxScrollWnd.h
	file base:	TxScrollWnd
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include "TxScrollBar.h"
// CTxScrollWnd
using namespace Gdiplus;

class CTxListHeader;
class CTxScrollWnd : public CWnd
{
	DECLARE_DYNAMIC(CTxScrollWnd)

	friend class CTxListHeader;
public:
	CTxScrollWnd();
	virtual ~CTxScrollWnd();

public:
	virtual void TxGetClientRect(CRect& rcClient);

protected:
	DECLARE_MESSAGE_MAP()

	CTxScrollBar m_scBar[2];

	UINT m_nBarStyle; //sb_vert, sb_both, sb_horz
	BOOL m_bMouseOver;
	BOOL m_bSetLeaveTrack;
	BOOL m_bPressed;
	BOOL m_bUseBarBmp;

	CSize m_szScrollView;


	BOOL EnableTxScrollBar(int nBar);

	BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo);

	virtual void SetScrollViewSize(CSize szView);
	int GetScrollPos(int nBar);
	void SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
	CSize GetScrollViewSize(){return m_szScrollView;}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
protected:
	virtual void PreSubclassWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


