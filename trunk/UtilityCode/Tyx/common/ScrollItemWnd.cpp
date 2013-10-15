#include "StdAfx.h"
#include "ScrollItemWnd.h"
#include "DlgIMContainer.h"
#include "Mutibitmapbuttonex.h"

#define ID_TIMER_DODRAGDROP 1301

HWND CScrollItemWnd::ms_hCurrFocusItem = NULL;
#define ID_TIMER_DRAG_SHOWER 1302

CScrollItemWnd::CScrollItemWnd(void)
{
	m_bInFocus = FALSE;
	m_bMouseOver = FALSE;
	m_bSetLeaveTrack = FALSE;

	m_bExpanded = FALSE;
	m_nRowOffSet = 0;


	m_pMenuHelper = NULL;
	m_bDragEnabled = FALSE;
	m_bDropEnabled = FALSE;
	m_bDraging = DD_NONE;

}

CScrollItemWnd::~CScrollItemWnd(void)
{
}
BEGIN_MESSAGE_MAP(CScrollItemWnd, CWnd)
	ON_WM_NCDESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE (WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_CONTEXTMENU()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CScrollItemWnd::OnNcDestroy()
{
	GetParent()->SendMessage(MSG_SCROLL_ITEM_DESTORYED, (WPARAM)this->GetSafeHwnd(), 0);
	if (ms_hCurrFocusItem == GetSafeHwnd())
	{
		ms_hCurrFocusItem = NULL;
	}

	CWnd::OnNcDestroy();
	delete this;
	// TODO: 在此处添加消息处理程序代码
}

void CScrollItemWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bMouseOver)
	{   
		m_bMouseOver = TRUE;
		InvalidateNonBtnRect(FALSE);
	}

	if (m_bDraging == DD_DOWN)
	{
		m_bDraging = DD_MOVE;
		PostMessage(MSG_SCROLL_ITEM_UNEXPAND, 0,0);
		SetTimer(ID_TIMER_DODRAGDROP, 50, NULL);
		return;
	}

	if (!m_bSetLeaveTrack)
	{
		TRACKMOUSEEVENT msevent;
		msevent.cbSize = sizeof(msevent);
		msevent.dwFlags =TME_LEAVE;
		msevent.dwHoverTime = HOVER_DEFAULT;
		msevent.hwndTrack = this->GetSafeHwnd();
		BOOL bSuc =TrackMouseEvent(&msevent);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CScrollItemWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDraging = DD_DOWN;

	CWnd::OnLButtonDown(nFlags, point);

}

void CScrollItemWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDraging = DD_NONE;
	CWnd::OnLButtonUp(nFlags, point);
	SetFocus();

}

void CScrollItemWnd::OnCaptureChanged(CWnd *pWnd)
{
	// TODO: 在此处添加消息处理程序代码
	CPoint pt;
	GetCursorPos(&pt);
	CRect rcThis;
	GetWindowRect(rcThis);

	if (pWnd != this && !rcThis.PtInRect(pt))
	{
		m_bMouseOver = FALSE;
		InvalidateNonBtnRect(FALSE);
	}

	CWnd::OnCaptureChanged(pWnd);
}

void CScrollItemWnd::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	InvalidateNonBtnRect(FALSE);

	// TODO: 在此处添加消息处理程序代码
}

void CScrollItemWnd::OnSetFocus(CWnd* pOldWnd)
{
	
	CWnd::OnSetFocus(pOldWnd);
	if (ms_hCurrFocusItem != GetSafeHwnd())
	{
		HWND hOldItem = ms_hCurrFocusItem;
		ms_hCurrFocusItem = GetSafeHwnd();
		if (hOldItem)
		{
			CWnd* pWnd = CWnd::FromHandle(hOldItem);
			if (pWnd)
			{
				pWnd->Invalidate();
			}
			//::UpdateWindow(hOldItem);
		}

		TRACE1("\nfocusitem = %0xx", ms_hCurrFocusItem);

		GetParent()->PostMessage(MSG_SCROLL_ITEM_FOCUS_OR_LAYOUT_CHANGED, 0,0);
	}

	if (pOldWnd != this)
	{
		InvalidateNonBtnRect(FALSE);
	}

	// TODO: 在此处添加消息处理程序代码
}

void CScrollItemWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);
	if (!bShow && nStatus == 0)
	{
		m_bMouseOver = FALSE;
	}

	if (!bShow && GetCapture() == this && nStatus == 0)
		ReleaseCapture();
	// TODO: 在此处添加消息处理程序代码
}

LRESULT CScrollItemWnd::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	CRect rect;
	GetWindowRect(&rect);
	CPoint pt;
	GetCursorPos(&pt);
	if (!rect.PtInRect(pt))
	{
		m_bMouseOver = FALSE;
		InvalidateNonBtnRect(FALSE);
	}
	m_bSetLeaveTrack = FALSE;
	return 1;
}

BOOL CScrollItemWnd::OnPopMenu( CWnd* /*pWnd*/, CPoint /*point*/ )
{
	return TRUE;
}

void CScrollItemWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	SetFocus();
	OnPopMenu(pWnd, point);
}

void CScrollItemWnd::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pMenuHelper && m_pMenuHelper->MeasureItem(lpMeasureItemStruct))
	{
		return;
	}
	CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CScrollItemWnd::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pMenuHelper && m_pMenuHelper->DrawItem(lpDrawItemStruct))
	{
		return;
	}
	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CScrollItemWnd::EnableDragDrop( BOOL bDragEnable /*= TRUE*/ , BOOL bDropEnable /*= TRUE*/)
{
	m_bDragEnabled = bDragEnable;
	m_bDropEnabled = bDropEnable;
	if (!m_bDropEnabled)
	{
		m_DropTarget.Revoke();
	}


}


BOOL CScrollItemWnd::DragFunc()
{
	if (!m_bDragEnabled)
	{
		return FALSE;
	}
	IM_DRAG_DATA dataIn;
	QueryDragData(dataIn);
	if (dataIn.hwndSrc == NULL)
	{
		return FALSE;
	}
	CRect rcBegin;
	GetWindowRect(rcBegin);
	ScreenToClient(rcBegin);

	const int nMemLen = sizeof(dataIn);
	HGLOBAL hdata = ::GlobalAlloc( GHND, nMemLen);
	if (hdata)
	{
		//lock the mem forbiden move
		void* pData = ::GlobalLock(hdata);
		ZeroMemory(pData, nMemLen);
		memcpy(pData, &dataIn, nMemLen);
		GlobalUnlock(hdata);
		//use CacheGlobalData
		m_DataDragSrc.CacheGlobalData(g_ImItemFmt,hdata);

		//or CacheData
		// 		STGMEDIUM mediumData;
		// 		ZeroMemory(&mediumData, sizeof(mediumData));
		// 		mediumData.tymed = TYMED_HGLOBAL;
		// 		mediumData.hGlobal = hdata;
		// 		m_DataDragSrc.CacheData(CF_TEXT, &mediumData);
		CRect rcDrag;
		GetWindowRect(rcDrag);
		CPoint ptAnchor;
		GetCursorPos(&ptAnchor);
		g_pDlgMainContainer->m_pDlgDragShower->BeginDrag(GetWindowDC(), rcDrag, ptAnchor);
		SetTimer(ID_TIMER_DRAG_SHOWER, 10, NULL);
		DROPEFFECT effectRet = m_DataDragSrc.DoDragDrop(DROPEFFECT_MOVE, &rcBegin);
		KillTimer(ID_TIMER_DRAG_SHOWER);
		g_pDlgMainContainer->m_pDlgDragShower->EndDrag();
		//after drop release the hdata;
		m_DataDragSrc.Empty();
		TRACE("\nfree drag data");
		::GlobalFree(hdata);
		hdata = NULL;
		m_DropTarget.CleanDragRect();

	}
	return TRUE;
}

void CScrollItemWnd::QueryDragData( IM_DRAG_DATA& dataIn )
{
	SendMessage(MSG_QUERY_DRAG_DATA, (WPARAM)&dataIn, 0);
}

int CScrollItemWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_bDropEnabled)
	{
		//DragAcceptFiles(m_bDragEnabled);
		m_DropTarget.Register(this);

	}
	return 0;
}

void CScrollItemWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_TIMER_DODRAGDROP)
	{
		KillTimer(nIDEvent);
		DragFunc();
	}
	else if (nIDEvent == ID_TIMER_DRAG_SHOWER)
	{
		//TRACE("\nshower timer active");
		CPoint ptAnchor;
		GetCursorPos(&ptAnchor);
		g_pDlgMainContainer->m_pDlgDragShower->DragMove(ptAnchor);

	}
	CWnd::OnTimer(nIDEvent);
}

BOOL CScrollItemWnd::CanDoDrag()
{
	return TRUE;
}

BOOL CScrollItemWnd::CanDoDrop()
{
	return 0;
}

LRESULT CScrollItemWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_SCROLL_ITEM_CANDROP)
	{
		int* pnType = (int*)wParam;
		*pnType = CanDoDrop();
		return TRUE;
	}
	else if (message == MSG_SCROLL_ITEM_UPDATEINFO)
	{
		UpdateUserInfo((int)wParam);
		return TRUE;
	}
	else if (message == MSG_SCROLL_ITEM_DRAW_DRAGRECT)
	{
		IM_DRAG_RECT_DRAW* pData = (IM_DRAG_RECT_DRAW*)wParam;
		DrawDragLine(pData->pRcOld, pData->pRcNew, pData->nMode);
	}
	else if (message == MSG_CHILD_RELAY_EVENT)
	{
		RelayChildMsg(wParam);
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

int CScrollItemWnd::GetRowOffSet()
{
	return 0;
}

BOOL CScrollItemWnd::UpdateUserInfo( int nTypeIn )
{
	return TRUE;
}


void CScrollItemWnd::DrawDragLine( CRect* pRcOld, CRect* pRcNew, int nMode)
{
	if (pRcOld)
	{
		CRect rcOld = *pRcOld;
		ScreenToClient(&rcOld);
		rcOld.InflateRect(1,1,1,1);
		InvalidateRect(rcOld, FALSE);
		if (nMode == 2)
		{
			return;
		}
	}
	
	

	if (pRcNew)
	{
		CRect rcNew = *pRcNew;
		ScreenToClient(&rcNew);
		CWindowDC dc(this);
		dc.FillSolidRect(&rcNew, RGB(0,0, 255));
	}
}

int CScrollItemWnd::GetCurrMaxOffSet()
{
	return m_nRowOffSet;
}

void CScrollItemWnd::RelayChildMsg( UINT message )
{
	if (message == WM_MOUSEMOVE
		||message == WM_LBUTTONUP)
	{
		CRect rect;
		GetWindowRect(&rect);
		CPoint pt;
		GetCursorPos(&pt);
		BOOL bMouseOver = TRUE;
		if (!rect.PtInRect(pt))
		{
			bMouseOver = FALSE;
		}
		
		if (m_bMouseOver != bMouseOver)
		{
			m_bMouseOver = bMouseOver;
			InvalidateNonBtnRect(FALSE);
		}
// 
// 		if (m_bMouseOver)
// 		{
// 			if (!m_bSetLeaveTrack)
// 			{
// 				TRACKMOUSEEVENT msevent;
// 				msevent.cbSize = sizeof(msevent);
// 				msevent.dwFlags =TME_LEAVE;
// 				msevent.dwHoverTime = HOVER_DEFAULT;
// 				msevent.hwndTrack = this->GetSafeHwnd();
// 				BOOL bSuc =TrackMouseEvent(&msevent);
// 			}
// 		}
	}
}

