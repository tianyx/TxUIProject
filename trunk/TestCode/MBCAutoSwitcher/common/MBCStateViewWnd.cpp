// ..\common\MBCStateViewWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "..\common\MBCStateViewWnd.h"


// CMBCStateViewWnd

IMPLEMENT_DYNAMIC(CMBCStateViewWnd, CWnd)

CMBCStateViewWnd::CMBCStateViewWnd()
{

}

CMBCStateViewWnd::~CMBCStateViewWnd()
{
}


BEGIN_MESSAGE_MAP(CMBCStateViewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CMBCStateViewWnd 消息处理程序



void CMBCStateViewWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CRect rcThis;
	GetClientRect(rcThis);
	CPoint ptOrg(0,0);
	if (GetStyle() & WS_HSCROLL)
	{
		SCROLLINFO info;
		if(GetScrollInfo(SB_HORZ, &info, SIF_POS))
		{
			ptOrg.x = -info.nPos;
			ASSERT(ptOrg.x < rcThis.Width());

		}
	}

	if (GetStyle() & WS_VSCROLL)
	{
		SCROLLINFO info;
		if(GetScrollInfo(SB_VERT, &info, SIF_POS))
		{
			ptOrg.y = -info.nPos;
			ASSERT(ptOrg.y < rcThis.Height());

		}
	}

	//TRACE("\ndraw rect pt org = %d, %d", ptOrg.x, ptOrg.y);
	CAutoLock lock(&m_lockgs);
	m_ghDrawer.DrawGraph(dc.GetSafeHdc(),  ptOrg, rcThis.Size());
}

LRESULT CMBCStateViewWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_UPDATE_GRAPHSTATE)
	{	
		CAutoLock lock(&m_lockgs);
		m_ghDrawer.ResetDrawData(m_gsState);
		
		ReCalcViewSize(m_ghDrawer.m_szGraphSize, m_ghDrawer.m_dZoom);
		Invalidate(TRUE);
		return TRUE;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CMBCStateViewWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CMBCStateViewWnd::OnDestroy()
{
	CWnd::OnDestroy();
	m_ghDrawer.ClearDrawer();
	// TODO: 在此处添加消息处理程序代码
}

void CMBCStateViewWnd::ReCalcViewSize( CSize szViewIn , double dZoom)
{
	if (dZoom < 0.001)
	{
		return;
	}
	CSize szReal( szViewIn.cx, szViewIn.cy );
	CRect rcClient;
	GetClientRect(rcClient);
	UINT nStyle = 0;
	SCROLLINFO info;
	ZeroMemory(&info, sizeof(info));
	info.cbSize = sizeof(info);
	if(szReal.cx >rcClient.Width()  )
	{
// 		if((GetStyle()&WS_HSCROLL) == 0)
// 		{
// 			ModifyStyle(0,WS_HSCROLL, 0);
// 		}

		//getscroll info
		if(GetScrollInfo(SB_HORZ, &info, SIF_ALL))
		{
			info.nMax = szReal.cx - rcClient.Width();
			info.nPos = info.nPos > info.nMax? info.nMax:info.nPos;

		}
		else 
		{
			info.nMax = szReal.cx - rcClient.Width();
			info.nPos = 0;
		}
		info.nMin = 0;
		info.fMask = SIF_POS|SIF_RANGE;
		SetScrollInfo(SB_HORZ, &info);
	}
	else
	{
		info.fMask = SIF_POS|SIF_RANGE;
		info.nMax = 0;
		info.nMin = 0;
		info.nPos = 0;
		SetScrollInfo(SB_HORZ, &info);
// 		if((GetStyle()&WS_HSCROLL) != 0)
// 		{
// 			ModifyStyle(WS_HSCROLL, 0, 0);
// 		}
	}

	if(szReal.cy > rcClient.Height()  )
	{
// 		if((GetStyle()&WS_VSCROLL) == 0)
// 		{
// 			ModifyStyle(0,WS_VSCROLL, 0);
// 		}
		//getscroll info
		if(GetScrollInfo(SB_VERT, &info, SIF_ALL))
		{
			info.nMax = szReal.cy - rcClient.Height();
			info.nPos = info.nPos > info.nMax? info.nMax:info.nPos;
		}
		else
		{
			info.nMax = szReal.cy - rcClient.Height();
			info.nPos = 0;
		}

		info.nMin = 0;
		info.fMask = SIF_POS|SIF_RANGE;
		SetScrollInfo(SB_VERT, &info);
	}
	else
	{
		info.fMask = SIF_POS|SIF_RANGE;
		info.nMax = 0;
		info.nPos = 0;
		SetScrollInfo(SB_VERT, &info);
// 		if((GetStyle()&WS_VSCROLL) != 0)
// 		{
// 			ModifyStyle(WS_VSCROLL, 0, 0);
// 		}
	}

	Invalidate(TRUE);
	
}

void CMBCStateViewWnd::OnSize(UINT nType, int cx, int cy)
{

	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ReCalcViewSize(m_ghDrawer.m_szGraphSize, m_ghDrawer.m_dZoom);

}

void CMBCStateViewWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nScrollPos = GetScrollPos(SB_HORZ);
	int nLimit = GetScrollLimit(SB_HORZ);

	int nScroll = nLimit;

	int SCROLL_AMT_Y = 150;

	switch(nSBCode) {
		case SB_LINELEFT:      // Scroll left.      	
		case SB_PAGELEFT :

			if(nScrollPos <= 0)
			{
				return;
			}
			nScroll = min(nScrollPos,SCROLL_AMT_Y);
			SetScrollPos(SB_HORZ,nScrollPos - nScroll);
			break;   
		case SB_LINERIGHT:   // Scroll right
		case SB_PAGERIGHT :

			if(nScrollPos >= nLimit)
			{
				return;
			}
			nScroll = min(nScroll-nScrollPos,SCROLL_AMT_Y);
			SetScrollPos(SB_HORZ,nScrollPos + nScroll);
			break;
		case SB_THUMBPOSITION:
			{
				
				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				GetScrollInfo(SB_HORZ, &info);

				nPos = info.nTrackPos;

				int nScr = nScrollPos - nPos;

				SetScrollPos(SB_HORZ,nPos, FALSE);
			}
			break;
		case SB_THUMBTRACK:
			{
				

				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				GetScrollInfo( SB_HORZ, &info);

				nPos = info.nTrackPos;
				nPos = info.nMax < nPos? info.nMax:nPos;
				nPos = info.nMin> nPos? 0:nPos;
				SetScrollPos(SB_HORZ,nPos,FALSE);
			}
			break;
	}	
	if (nPos != nScrollPos)
	{
		Invalidate(TRUE);

	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMBCStateViewWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nScrollPos = GetScrollPos(SB_VERT);
	int nLimit = GetScrollLimit(SB_VERT);

	int nScroll = nLimit;
	int SCROLL_AMT_Y = 150;
	if (nSBCode == SB_THUMBPOSITION)
	{

	}
	TRACE("\nbarpos = %d",nPos);

	switch(nSBCode) {
		case SB_LINEUP:      // Scroll up.      
		case SB_PAGEUP:
			if(nScrollPos <= 0)
			{
				return;
			}
			nScroll = min(nScrollPos,SCROLL_AMT_Y);
			SetScrollPos(SB_VERT,nScrollPos - nScroll);
			break;   
		case SB_LINEDOWN:   // Scroll down.
		case SB_PAGEDOWN:
			if(nScrollPos >= nLimit)
			{
				return;
			}
			nScroll = min(nScroll-nScrollPos,SCROLL_AMT_Y);
			SetScrollPos(SB_VERT,nScrollPos + nScroll);
			break;
		case SB_THUMBPOSITION:
			{
				
				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				GetScrollInfo(SB_VERT, &info);

				nPos = info.nTrackPos;

				int nScr = nScrollPos - nPos;
				//SetScrollPos(SB_VERT,nPos);
				TRACE("\nset scroll pos = %d", nPos);
			}
			break;
		case SB_THUMBTRACK:
			{
				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				GetScrollInfo(SB_VERT, &info);

				nPos = info.nTrackPos;
				nPos = info.nMax < nPos? info.nMax:nPos;
				nPos = info.nMin> nPos? 0:nPos;

				int nScr = nScrollPos - nPos;
				TRACE("\n vmax = %d,track = %d, pos = %d",info.nMax,info.nTrackPos,nPos);

				//SetScrollPos(SB_VERT,nPos,FALSE);
			}
			break;
	}
	if (nPos != nScrollPos)
	{
		Invalidate(TRUE);

	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);


}

BOOL CMBCStateViewWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nVertScroll = GetScrollPos(SB_VERT);

	TRACE("\nmouse wheel");
	int maxpos = GetScrollLimit(SB_VERT);
	int nNewPos = 0;
	if(nFlags==MK_CONTROL)
	{
		//zoom the view
		double dDeta = zDelta> 0? 0.1:-0.1;
		dDeta +=m_ghDrawer.m_dZoom;
		dDeta = dDeta > 1.0? 1.0:dDeta;
		dDeta = dDeta < 0.1? 0.1:dDeta;
		double dDfff =m_ghDrawer.m_dZoom - dDeta;
		if ( dDfff < 0.00001  && dDfff > -0.00001)
		{
			//same
		}
		else
		{
			m_ghDrawer.m_dZoom = dDeta;
			ReCalcViewSize(m_ghDrawer.m_szGraphSize, m_ghDrawer.m_dZoom);
			if (m_ghDrawer.m_dZoom  < 1.0001 && m_ghDrawer.m_dZoom > 0.9999)
			{
				m_ghDrawer.m_dZoom = 1.0;
			}
			m_ghDrawer.ReLayoutGraph(m_ghDrawer.m_dZoom);
		}
		return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	}

	if(zDelta < 0)
	{
		nNewPos = min(nVertScroll + 50 , maxpos);

		SetScrollPos(SB_VERT,nNewPos);	
		
	}
	else
	{
		nNewPos = max((nVertScroll - 50) , 0) ;

		SetScrollPos(SB_VERT, nNewPos);		
	}
	if (nNewPos != nVertScroll)
	{
		Invalidate(TRUE);
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

int CMBCStateViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0,GetStyle()|WS_HSCROLL|WS_VSCROLL, 0);
	// TODO:  在此添加您专用的创建代码
	SCROLLINFO scrlinfo;
	scrlinfo.cbSize = sizeof(scrlinfo);

	scrlinfo.fMask = SIF_PAGE | SIF_RANGE;
	scrlinfo.nMax = 0;
	scrlinfo.nMin = 0;
	scrlinfo.nPage = 0;
	scrlinfo.nPos = 0;
	SetScrollInfo(SB_VERT,&scrlinfo);
	SetScrollInfo(SB_HORZ,&scrlinfo);

	return 0;
}

void CMBCStateViewWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnLButtonUp(nFlags, point);
}

void CMBCStateViewWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetFocus();
	CWnd::OnLButtonDown(nFlags, point);
}
