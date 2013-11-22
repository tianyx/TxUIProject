/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	DlgMovedBase.h
	file base:	DlgMovedBase
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/

#pragma once
#include "afxwin.h"


class CDlgMovedBase : public CDialog
{
	DECLARE_DYNAMIC(CDlgMovedBase)

public:
	CDlgMovedBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);
public:
	~CDlgMovedBase(void);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	BOOL m_bDraging;

	CPoint m_ptPre;
	CPoint m_ptBeforeDrag;
	CRect m_rcPre;
	CRect m_rcOrg;
	CRect m_rcNext;

	HCURSOR m_hCursor;

	CString m_strLanKeyName;
	
	HRGN m_hRgn;

protected:
	BOOL m_bDragWithWnd;
protected:
	CRect m_rcDragRect;

private:
	//nMode = 0 first, 1 normal, 2 end.
	void DrawDragRect(CRect* pRcBefore, CRect* pRcAfter, int nMode = 1);


public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
