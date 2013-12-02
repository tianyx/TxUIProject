#pragma once
#include "GraphDrawer.h"
#include "MBCCommonDef.h"
#include "AutoCritSec.h"
// CMBCStateViewWnd
#define MSG_UPDATE_GRAPHSTATE WM_USER + 3150

class CMBCStateViewWnd : public CWnd
{
	DECLARE_DYNAMIC(CMBCStateViewWnd)

public:
	CMBCStateViewWnd();
	virtual ~CMBCStateViewWnd();

protected:
	DECLARE_MESSAGE_MAP()
	
	CGraphDrawer m_ghDrawer;
	CAutoCritSec m_lockgs;
	ST_GRAPHSTATE m_gsState;

	void ReCalcViewSize(CSize szViewIn , double dZoom);
	
	BOOL HitTestObject(ST_OBJSTATEINFO& hitObjInfo);
	
public:
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


