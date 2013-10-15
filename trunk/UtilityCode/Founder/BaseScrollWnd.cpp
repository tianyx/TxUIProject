// BaseScrollWnd.cpp : implementation file
//

#include "..\stdafx.h"
#include "BaseScrollWnd.h"


// CBaseScrollWnd

IMPLEMENT_DYNAMIC(CBaseScrollWnd, CWnd)

CBaseScrollWnd::CBaseScrollWnd()
{

}

CBaseScrollWnd::~CBaseScrollWnd()
{
}


BEGIN_MESSAGE_MAP(CBaseScrollWnd, CWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CBaseScrollWnd message handlers


BOOL CBaseScrollWnd::Create( LPCTSTR lpszWindowName, CWnd *pParent, CRect rc, UINT nID, DWORD dwStyle)
{
	CString sWindowClassName = ::AfxRegisterWndClass(CS_DBLCLKS | CS_SAVEBITS); 

	//Try to create it with default styles
	if( !CWnd::Create( sWindowClassName, lpszWindowName, dwStyle, rc, pParent, nID) )
	{
		return FALSE;
	}
	
	//Set the scrolling info
	SCROLLINFO scrlinfo;
	scrlinfo.cbSize = sizeof(scrlinfo);

	scrlinfo.fMask = SIF_PAGE | SIF_RANGE;
	scrlinfo.nMax = 0;
	scrlinfo.nMin = 0;
	scrlinfo.nPage = 0;
	scrlinfo.nPos = 0;
	SetScrollInfo(SB_VERT,&scrlinfo);
	SetScrollInfo(SB_HORZ,&scrlinfo);

	return TRUE;

}

void CBaseScrollWnd::OnDraw( CDC *pDC, CRect rectClipBox )
{

}

void CBaseScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

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
				HWND hWndScroll;
				if ( pScrollBar == NULL )
					hWndScroll = m_hWnd;
				else
					hWndScroll = pScrollBar->m_hWnd;

				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				::GetScrollInfo(hWndScroll, SB_HORZ, &info);

				nPos = info.nTrackPos;

				int nScr = nScrollPos - nPos;

				SetScrollPos(SB_HORZ,nPos, FALSE);
			}
			break;
		case SB_THUMBTRACK:
			{
				HWND hWndScroll;
				if ( pScrollBar == NULL )
					hWndScroll = m_hWnd;
				else
					hWndScroll = pScrollBar->m_hWnd;

				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				::GetScrollInfo(hWndScroll, SB_HORZ, &info);

				nPos = info.nTrackPos;

				int nScr = nScrollPos - nPos;

				SetScrollPos(SB_HORZ,nPos,FALSE);
			}
			break;
	}	

	CClientDC dc(this);
	OnDraw( &dc );



	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBaseScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	int nScrollPos = GetScrollPos(SB_VERT);
	int nLimit = GetScrollLimit(SB_VERT);

	int nScroll = nLimit;

	int SCROLL_AMT_Y = 150;

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
				HWND hWndScroll;
				if ( pScrollBar == NULL )
					hWndScroll = m_hWnd;
				else
					hWndScroll = pScrollBar->m_hWnd;

				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				::GetScrollInfo(hWndScroll, SB_VERT, &info);

				nPos = info.nTrackPos;

				int nScr = nScrollPos - nPos;

				SetScrollPos(SB_VERT,nPos);
			}
			break;
		case SB_THUMBTRACK:
			{
				HWND hWndScroll;
				if ( pScrollBar == NULL )
					hWndScroll = m_hWnd;
				else
					hWndScroll = pScrollBar->m_hWnd;

				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_TRACKPOS;
				::GetScrollInfo(hWndScroll, SB_VERT, &info);

				nPos = info.nTrackPos;

				int nScr = nScrollPos - nPos;

				SetScrollPos(SB_VERT,nPos,FALSE);
			}
			break;
	}	

	CClientDC dc(this);
	OnDraw( &dc );

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBaseScrollWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default

	int nVertScroll = GetScrollPos(SB_VERT);

	int maxpos = GetScrollLimit(SB_VERT);

	if(zDelta < 0)
	{
		int nNewPos = min(nVertScroll + 240 , maxpos);

		SetScrollPos(SB_VERT,nNewPos);		
	}
	else
	{
		int nNewPos = max((nVertScroll - 240) , 0) ;

		SetScrollPos(SB_VERT, nNewPos);		
	}
	
	CClientDC dc(this);
	OnDraw( &dc );

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
