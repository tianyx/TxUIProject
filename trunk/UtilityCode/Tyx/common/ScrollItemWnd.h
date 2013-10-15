/********************************************************************
	created:	2011/08/10
	created:	10:8:2011   15:25
	filename: 	e:\Source\EZIM\Src\EZMessenger\common\ScrollItemWnd.h
	file path:	e:\Source\EZIM\Src\EZMessenger\common
	file base:	ScrollItemWnd
	file ext:	h
	author:		tian_yuanxin
	
	purpose:	
*********************************************************************/
#pragma once
#include "afxwin.h"
#include "DBMenu.h"
#include "ImDropTarget.h"

#define  MSG_SCROLL_ITEM_FOCUS_OR_LAYOUT_CHANGED	WM_USER + 1201
#define  MSG_CHILD_SCROLL_ITEM_DESTORYED			WM_USER + 1202
#define  MSG_SCROLL_ITEM_DESTORYED					WM_USER + 1203
#define  MSG_SCROLL_ITEM_UNEXPAND					WM_USER + 1204
#define  MSG_SCROLL_ITEM_UPDATEINFO					WM_USER + 1205
#define  MSG_SCROLL_ITEM_CALCEXPAND_WIDTH			WM_USER + 1206



enum DRAG_DROP_STATE
{
	DD_NONE,
	DD_DOWN,
	DD_MOVE
};


class CScrollItemWnd :
	public CWnd
{
public:
	CScrollItemWnd(void);
public:
	~CScrollItemWnd(void);


	BOOL m_bInFocus;
	BOOL m_bSetLeaveTrack;
	BOOL m_bMouseOver;

	BOOL m_bExpanded;

	DBMenu*	m_pMenuHelper; //for pop menu must creat in subclass

	static HWND ms_hCurrFocusItem;
	BOOL m_bDragEnabled;
	BOOL m_bDropEnabled;
	
	int m_nRowOffSet;
	virtual int GetRowOffSet();
	virtual int GetCurrMaxOffSet();
	
	virtual void RelayChildMsg(UINT message);
	void EnableDragDrop(BOOL bDragEnable = TRUE, BOOL bDropEnable = TRUE);
protected:
	CIMDropTarget m_DropTarget;
	COleDataSource m_DataDragSrc;
	DRAG_DROP_STATE m_bDraging;

	virtual void QueryDragData(IM_DRAG_DATA& dataIn);
	virtual BOOL CanDoDrag();
	virtual BOOL CanDoDrop();
	virtual BOOL UpdateUserInfo(int nTypeIn);
	virtual void DrawDragLine(CRect* pRcOld, CRect* pRcNew, int nMode);
private:
	BOOL DragFunc();
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnNcDestroy();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnCaptureChanged(CWnd *pWnd);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	virtual BOOL CalcChatItemRect(CRect& rcOut) = 0;
	virtual void InvalidateNonBtnRect(BOOL bErase = FALSE) = 0;
	//if want pop menu override this func
	virtual BOOL OnPopMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	LRESULT OnMouseLeave (WPARAM wParam, LPARAM lParam);


public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
