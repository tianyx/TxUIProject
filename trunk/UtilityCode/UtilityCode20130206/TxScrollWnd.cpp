/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxScrollWnd.cpp
	file base:	TxScrollWnd
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// TxScrollWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClipImportMonitor.h"
#include "TxScrollWnd.h"


// CTxScrollWnd

IMPLEMENT_DYNAMIC(CTxScrollWnd, CWnd)

CTxScrollWnd::CTxScrollWnd()
{
	m_nBarStyle = SB_VERT;
	m_bMouseOver = FALSE;
	m_bSetLeaveTrack = FALSE;
	m_bPressed = FALSE;
	m_bUseBarBmp = TRUE;
	m_szScrollView = CSize(0,0);
	m_scBar[SB_VERT].SetBarStyle(SB_VERT);
	m_scBar[SB_HORZ].SetBarStyle(SB_HORZ);
	m_scBar[SB_HORZ].SetOtherBar(&m_scBar[SB_VERT]);
	m_scBar[SB_VERT].SetOtherBar(&m_scBar[SB_HORZ]);
	m_scBar[SB_HORZ].SetParent(this);
	m_scBar[SB_VERT].SetParent(this);
}

CTxScrollWnd::~CTxScrollWnd()
{
}


BEGIN_MESSAGE_MAP(CTxScrollWnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CTxScrollWnd 消息处理程序



LRESULT CTxScrollWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	MSG msg;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	m_scBar[SB_HORZ].RelayMsg(&msg);
	m_scBar[SB_VERT].RelayMsg(&msg);

	HRESULT hr = CWnd::WindowProc(message, wParam, lParam);
// 	if (m_nBarStyle == SB_BOTH || m_nBarStyle == SB_HORZ)
// 	{
// 		m_scBar[SB_HORZ].PostRelayMsg( message,  wParam,  lParam);
// 	}
// 
// 	if (m_nBarStyle == SB_BOTH || m_nBarStyle == SB_VERT)
// 	{
// 		m_scBar[SB_VERT].PostRelayMsg( message,  wParam,  lParam);
// 	}
// 

	return hr;
}

void CTxScrollWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bPressed = TRUE;
	CWnd::OnLButtonDown(nFlags, point);
}

void CTxScrollWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonUp(nFlags, point);
}


void CTxScrollWnd::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bPressed = FALSE;
	m_bMouseOver = FALSE;
	CWnd::OnMouseLeave();
}


void CTxScrollWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bMouseOver == FALSE)
	{   
		m_bMouseOver = TRUE;
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

BOOL CTxScrollWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CTxScrollWnd::GetScrollInfo( int nBar, LPSCROLLINFO lpScrollInfo )
{
	ASSERT(nBar < 2);
	m_scBar[nBar].GetScrollInfo(lpScrollInfo);
	return TRUE;
}

void CTxScrollWnd::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_scBar[SB_VERT].LoadBarBmp(TEXT("skins\\ScrollbarBmpVert.bmp"), 0,20,40,60);
	m_scBar[SB_HORZ].LoadBarBmp(TEXT("skins\\ScrollbarBmpHorz.bmp"), 0,20,40,60);
	CWnd::PreSubclassWindow();
}

BOOL CTxScrollWnd::EnableTxScrollBar( int nBar )
{
	ASSERT(nBar < 2);
	m_scBar[nBar].EnableBar();
	m_nBarStyle |= nBar;
	return TRUE;
}

BOOL CTxScrollWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		m_scBar[SB_VERT].RelayMsg(pMsg);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CTxScrollWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	BOOL bRet = CWnd::OnEraseBkgnd(pDC);
	m_scBar[SB_HORZ].DrawBar(pDC);
	m_scBar[SB_VERT].DrawBar(pDC);

	return bRet;

}

void CTxScrollWnd::SetScrollViewSize( CSize szView )
{
	if (szView != m_szScrollView)
	{
		m_szScrollView = szView;
		m_scBar[SB_HORZ].SetViewSize(szView);
		m_scBar[SB_VERT].SetViewSize(szView);
	}
}

void CTxScrollWnd::SetScrollPos( int nBar, int nPos, BOOL bRedraw /*= TRUE*/)
{
	if (nBar == SB_HORZ)
	{
		m_scBar[SB_HORZ].SetScrollPos(nPos, bRedraw);
	}
	else if(nBar == SB_VERT)
	{
		m_scBar[SB_VERT].SetScrollPos(nPos, bRedraw);
	}
}

int CTxScrollWnd::GetScrollPos( int nBar )
{
	if (nBar == SB_HORZ)
	{
		return m_scBar[SB_HORZ].GetScrollPos();
	}
	else if(nBar == SB_VERT)
	{
		return m_scBar[SB_VERT].GetScrollPos();
	}
	ASSERT(FALSE);
	return 0;
}

void CTxScrollWnd::TxGetClientRect( CRect& rcClient )
{
	CWnd::GetClientRect(rcClient);
}


