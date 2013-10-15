/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxScrollBar.cpp
	file base:	TxScrollBar
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxScrollBar.h"
#include "GDIDrawFunc.h"
#include "TxScrollWnd.h"
CTxScrollBar::CTxScrollBar()
{
	ZeroMemory(&m_slInfo, sizeof(m_slInfo));
	m_nBarStyle = SB_VERT;
	m_pOtherbar = NULL;
	m_nBarShowState = 0;
	m_nBarHotPart = TXBARPART_NONE;
	m_nBarState = TXBARSTATE_NORMAL;
	m_ptDragPre.x = 0;
	m_ptDragPre.y = 0;
	m_pParent = NULL;
	m_bCaptured = FALSE;
	m_bBarEnabled = FALSE;
	m_szView = CSize(0,0);
	m_nPixsPerScrollBarMove = 1;
	m_nPixPerScrollBkClick = 1;
	m_nMinPixScrollPerBtnClick = g_globalInfo.scrollbarParam.nMinViewPixPerScrollPix;
	m_nScrollPixPerWhell = g_globalInfo.viewSetting.nItemHeight/2;
	for (int i = 0; i < 5; i++)
	{
		m_vrcPart[i].SetRectEmpty();
	}
}

CTxScrollBar::~CTxScrollBar(void)
{
}

BOOL CTxScrollBar::GetScrollInfo(LPSCROLLINFO lpScrollInfo )
{
	*lpScrollInfo = m_slInfo;
	return TRUE;
}

BOOL CTxScrollBar::SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw /*= TRUE*/)
{
	BOOL bChanged = m_slInfo.nPos != lpScrollInfo->nPos || m_slInfo.nMax != lpScrollInfo->nMax
		|| m_slInfo.nMin != lpScrollInfo->nMin;
	m_slInfo = *lpScrollInfo;
	ReCalSize();
	return TRUE;
}

HRESULT CTxScrollBar::RelayMsg( MSG* pMsg )
{
	HRESULT hr = S_OK;
	if ( pMsg->message == WM_SIZE && m_bBarEnabled)
	{
		ReCalSize();
		return hr;
	}

	if (!TxUTHasState(m_nBarShowState, TXBARSHOW_READY) || !m_bBarEnabled)
	{
		return hr;
	}

	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			OnMouseMove(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_MOUSELEAVE:
		{
			OnMouseLeave(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_LBUTTONDOWN:
		{
			OnLButtonDown(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_LBUTTONUP:
		{
			OnLButtonUp(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_KILLFOCUS:
		{
			OnKillFocus(pMsg->wParam, pMsg->lParam);
		}break;
	case WM_MOUSEWHEEL:
		{
			if (m_nBarStyle == SB_VERT)
			{
				OnMouseWHeel(pMsg->wParam, pMsg->lParam);
			}
		}break;
	case WM_CREATE:
		{
			//
		}break;

	case WM_DESTROY:
	case WM_NCDESTROY:
		{
			m_nBarShowState = 0;
		}break;

	}

	return hr;
}

void CTxScrollBar::ReCalSize()
{
	if(!m_pParent || !m_bBarEnabled)
	{
		return;
	}

	BOOL bShowing = m_nBarShowState == TXBARSHOW_SHOWING;


	CRect rcParent;
	m_pParent->TxGetClientRect(rcParent);
	if (rcParent.IsRectEmpty())
	{
		m_rcBarRect.SetRectEmpty();
		TxUTModifyState(m_nBarShowState, TXBARSHOW_READY, 0);
		if (bShowing)
		{
			NotifyRefresh();
		}
		return;
	}
	int nOldMax = m_slInfo.nMax;

	m_slInfo.nMax  = (m_nBarStyle == SB_VERT)? m_szView.cy - rcParent.Height():m_szView.cx- rcParent.Width();
	
	if (m_slInfo.nMax <= 0)
	{
		m_slInfo.nPos = 0;
		m_slInfo.nMax = 0;
		TxUTModifyState(m_nBarShowState, TXBARSHOW_READY, 0);
		if (nOldMax > 0)
		{
			NotifyRefresh();
		}
		return;
	}

	int nOldPos = m_slInfo.nPos;
	m_slInfo.nPos = m_slInfo.nPos > m_slInfo.nMax? m_slInfo.nMax:m_slInfo.nPos;

	if (m_nBarStyle == SB_VERT)
	{
		m_rcBarRect = CRect(rcParent.right - m_BarBmpSec.nBarWidth, rcParent.top, rcParent.right, rcParent.bottom);
	}
	else
	{
		m_rcBarRect = CRect(rcParent.left, rcParent.bottom - m_BarBmpSec.nBarWidth, rcParent.right, rcParent.bottom);
	}


	int nMinViewPixsPerScrollPix = g_globalInfo.scrollbarParam.nMinViewPixPerScrollPix;
	int nBarWidth = m_nBarStyle== SB_VERT? m_rcBarRect.Height():m_rcBarRect.Width();
	if (nBarWidth < m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_MID]
		|| m_slInfo.nMax < nMinViewPixsPerScrollPix)
	{
		TxUTModifyState(m_nBarShowState, TXBARSHOW_READY, 0);
		if (bShowing)
		{
			//redraw parent
			NotifyRefresh();
		}
		return;
	}

	TxUTModifyState(m_nBarShowState, 0, TXBARSHOW_READY);
	//recalc bar part rect
	int nBtnLen = m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_BTNMAX];
	int nBarSlideLen = nBarWidth - 2*nBtnLen;

	//calc mid btn len
	int nMidBtnLen = g_globalInfo.scrollbarParam.nMinMidLen;
	if (nBarSlideLen - nMidBtnLen <= 0)
	{
		//not draw thumb
		nMidBtnLen = 0;
	}
	else
	{
		int nMaxBkNeeded = m_slInfo.nMax / nMinViewPixsPerScrollPix;
		if (nMaxBkNeeded + nMidBtnLen <= nBarSlideLen)
		{
			nMidBtnLen = nBarSlideLen - nMaxBkNeeded;
		}
	}

	//calc upbk len
	int nBkUpLen = (int) (((double)(nBarSlideLen - nMidBtnLen))/m_slInfo.nMax *m_slInfo.nPos);
	int nBkDownLen = nBarSlideLen - nMidBtnLen - nBkUpLen;
	int nBkLen = nBkUpLen + nBkDownLen;
	if (nBkLen <= 0)
	{
		nBkLen = 1;
	}
	m_nPixsPerScrollBarMove = m_slInfo.nMax / nBkLen;
	if (m_nPixsPerScrollBarMove == 0)
	{
		m_nPixsPerScrollBarMove = 1;
	}
	if (m_nBarStyle == SB_VERT)
	{
		m_vrcPart[TXBARPART_MIN] = CRect(m_rcBarRect.left, m_rcBarRect.top, m_rcBarRect.right, m_rcBarRect.top + nBtnLen);
		m_vrcPart[TXBARPART_MAX] = CRect(m_rcBarRect.left, m_rcBarRect.bottom - nBtnLen, m_rcBarRect.right, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_MINBK] = CRect(m_rcBarRect.left, m_vrcPart[TXBARPART_MIN].bottom, m_rcBarRect.right, m_vrcPart[TXBARPART_MIN].bottom + nBkUpLen);
		m_vrcPart[TXBARPART_MID] = CRect(m_rcBarRect.left, m_vrcPart[TXBARPART_MINBK].bottom, m_rcBarRect.right, m_vrcPart[TXBARPART_MINBK].bottom +nMidBtnLen);
		m_vrcPart[TXBARPART_BKMAX] = CRect(m_rcBarRect.left, m_vrcPart[TXBARPART_MID].bottom, m_rcBarRect.right, m_vrcPart[TXBARPART_MAX].top);
	}
	else
	{
		m_vrcPart[TXBARPART_MIN] = CRect(m_rcBarRect.left, m_rcBarRect.top, m_rcBarRect.left + nBtnLen, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_MAX] = CRect(m_rcBarRect.right - nBtnLen, m_rcBarRect.top, m_rcBarRect.right, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_MINBK] = CRect(m_vrcPart[TXBARPART_MIN].right, m_rcBarRect.top, m_vrcPart[TXBARPART_MIN].right + nBkUpLen, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_MID] = CRect(m_vrcPart[TXBARPART_MINBK].right, m_rcBarRect.top, m_vrcPart[TXBARPART_MINBK].right + nMidBtnLen, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_BKMAX] = CRect(m_vrcPart[TXBARPART_MID].right, m_rcBarRect.top, m_vrcPart[TXBARPART_MAX].left, m_rcBarRect.bottom);
	}

	if (nOldPos != m_slInfo.nPos || nOldMax != m_slInfo.nMax || TxUTHasState(m_nBarShowState, TXBARSHOW_HOVER))
	{
		NotifyRefresh();
	}
	
}

void CTxScrollBar::CheckHotPart()
{
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);

	UINT nOldShowState = m_nBarShowState;

	BOOL bShowing = nOldShowState == TXBARSHOW_SHOWING;
	if (!m_rcBarRect.PtInRect(pt))
	{
		TxUTModifyState(m_nBarShowState, TXBARSHOW_HOVER, 0);
		m_nBarHotPart = TXBARPART_NONE;
		m_nBarState = TXBARSTATE_NORMAL;
		if (bShowing)
		{
			//redraw parent
			NotifyRefresh();
		}
		return;
	}

	UINT nOldState =m_nBarState;
	UINT nOldHotPart = m_nBarHotPart;
	
	TxUTModifyState(m_nBarState, 0, TXBARSTATE_HOVER);

	for (UINT i = TXBARPART_MIN; i <= TXBARPART_MAX; i++)
	{
		if (m_vrcPart[i].PtInRect(pt))
		{
			m_nBarHotPart = i;
			break;
		}
	}

	if ((nOldHotPart != m_nBarHotPart  || nOldState != m_nBarState) && bShowing)
	{
		//redraw parent
		NotifyRefresh();
	}

}

void CTxScrollBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);

	UINT nOldShowState = m_nBarShowState;
	UINT nOldHotPart = m_nBarHotPart;
	UINT nOldBarState = m_nBarState;
	BOOL bShowing = nOldShowState == TXBARSHOW_SHOWING;

	if (!m_bCaptured)
	{
		TxUTModifyState(m_nBarShowState, TXBARSHOW_HOVER, 0);
	}
	
	m_nBarHotPart = TXBARPART_NONE;
	m_nBarState = TXBARSTATE_NORMAL;

	if (m_nBarHotPart != nOldHotPart || m_nBarShowState != nOldShowState
		|| m_nBarState != nOldBarState)
	{
		NotifyRefresh();

	}

}

void CTxScrollBar::OnLButtonDown( WPARAM wparam, LPARAM lparam )
{
	if (m_pOtherbar &&  m_pOtherbar->m_nBarShowState == TXBARSHOW_SHOWING)
	{
		return;
	}

	int nHotPart = GetHotPart();
	if (nHotPart == TXBARPART_NONE)
	{
		return;
	}

	UINT nOldShowState = m_nBarShowState;
	UINT nOldHotPart = m_nBarHotPart;
	UINT nOldBarState = m_nBarState;

	//if not capture mid btn, capture it
	if (nHotPart == TXBARPART_MID)
	{
		TxUTModifyState(m_nBarState, 0, TXBARSTATE_LDOWN);

		 if (m_bCaptured)
		 {
			 //may error
			// ASSERT(FALSE);
		 }
		 m_pParent->SetCapture();
		 m_bCaptured = TRUE;

		 CPoint pt;
		 GetCursorPos(&pt);
		 m_pParent->ScreenToClient(&pt);
		 m_ptDragPre = pt;
		 return;
	}

	//if min btn, scroll 1 pixel
	int nOldPos = m_slInfo.nPos;
	if (nHotPart == TXBARPART_MIN)
	{
		
		if (m_slInfo.nPos > (m_slInfo.nMin + m_nMinPixScrollPerBtnClick))
		{
			m_slInfo.nPos-= m_nMinPixScrollPerBtnClick;
		}
		else
		{
			m_slInfo.nPos = 0;
		}

	}
	// if max btn, scroll 1 pixel
	else if (nHotPart == TXBARPART_MAX)
	{
		if (m_slInfo.nPos < m_slInfo.nMax -m_nMinPixScrollPerBtnClick)
		{
			m_slInfo.nPos+=m_nMinPixScrollPerBtnClick;
		}
		else
		{
			m_slInfo.nPos = m_slInfo.nMax;
		}
	}
	//if min-bk, scroll 1 client screen
	else if (nHotPart == TXBARPART_MINBK
		|| nHotPart == TXBARPART_BKMAX)
	{
		
		int nScrollPixs = GetBarPixsPerClientArea();
		if (nHotPart == TXBARPART_MINBK)
		{
			if (m_slInfo.nPos - nScrollPixs <=0)
			{
				m_slInfo.nPos = 0;
			}
			else
			{
				m_slInfo.nPos -= nScrollPixs;
			}
		}
		else
		{
			if (m_slInfo.nPos + nScrollPixs > m_slInfo.nMax)
			{
				m_slInfo.nPos = m_slInfo.nMax;
			}
			else
			{
				m_slInfo.nPos += nScrollPixs;
			}
		}
	}
	
	if (nOldPos != m_slInfo.nPos)
	{
		ReCalScrollPartSize();
	}
	if (m_nBarHotPart != nOldHotPart || m_nBarShowState != nOldShowState
		|| m_nBarState != nOldBarState ||nOldPos != m_slInfo.nPos)
	{
		NotifyRefresh();

	}
}

void CTxScrollBar::OnLButtonUp( WPARAM wparam, LPARAM lparam )
{
	if (m_bCaptured)
	{
		ReleaseCapture();
		m_bCaptured = FALSE;
	}
	
	UINT nOldBarState = m_nBarState;
	TxUTModifyState(m_nBarState, TXBARSTATE_LDOWN, 0);
	if (m_nBarShowState == TXBARSHOW_SHOWING && m_nBarState != nOldBarState)
	{
		NotifyRefresh();
	}
}

int CTxScrollBar::GetHotPart()
{
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);

	int nRet = TXBARPART_NONE;
	if (m_rcBarRect.PtInRect(pt))
	{
		for (UINT i = TXBARPART_MIN; i <= TXBARPART_MAX; i++)
		{
			if (m_vrcPart[i].PtInRect(pt))
			{
				nRet = i;
				break;
			}
		}
	}
	
	return nRet;
}

int CTxScrollBar::GetBarPixsPerClientArea()
{
	if (m_slInfo.nMax <= 0)
	{
		ASSERT(FALSE);
		return 0;
	}
	double dBkLen = 0; 
	double dPixClient = 0;
	if (m_nBarStyle == SB_VERT)
	{
		dBkLen =m_vrcPart[TXBARPART_MINBK].Height() + m_vrcPart[TXBARPART_BKMAX].Height();
		dPixClient = m_rcBarRect.Height();
	}
	else
	{
		dBkLen = m_vrcPart[TXBARPART_MINBK].Width() + m_vrcPart[TXBARPART_BKMAX].Width();
		dPixClient = m_rcBarRect.Width();
	}
	return (int)(dBkLen/m_slInfo.nMax* (dPixClient));

}

void CTxScrollBar::OnMouseMove( WPARAM wparam, LPARAM lparam )
{
	if (m_pOtherbar &&  m_pOtherbar->m_nBarShowState == TXBARSHOW_SHOWING)
	{
		return;
	}
	CPoint pt;
	GetCursorPos(&pt);
	m_pParent->ScreenToClient(&pt);

	if (m_bCaptured)
	{
		//drag mode
		int nPrePos = m_nBarStyle == SB_VERT? m_ptDragPre.y:m_ptDragPre.x;
		int nCurPos =m_nBarStyle == SB_VERT? pt.y:pt.x;
		if (nPrePos != nCurPos)
		{
			int nNewBarPos =  (nCurPos - nPrePos)*m_nPixsPerScrollBarMove + m_slInfo.nPos;
			nNewBarPos = nNewBarPos < 0? 0:(nNewBarPos > m_slInfo.nMax? m_slInfo.nMax:nNewBarPos);
			if (m_slInfo.nPos != nNewBarPos)
			{
				TRACE("\nvbar = %d, %d", m_slInfo.nPos, nNewBarPos);
				m_slInfo.nPos = nNewBarPos;
				ReCalScrollPartSize();
				NotifyRefresh();
			}
		}
		m_ptDragPre = pt;
		return;
	}

	int nOldHotPart = m_nBarHotPart;
	int nOldShowState = m_nBarShowState;
	int nOldBarState = m_nBarState;
	m_nBarHotPart = GetHotPart();
	if (m_nBarHotPart == TXBARPART_NONE)
	{
		TxUTModifyState(m_nBarShowState, TXBARSHOW_HOVER, 0);
		TxUTModifyState(m_nBarState, TXBARSTATE_HOVER, 0);
	}
	else
	{
		if (m_pOtherbar && m_pOtherbar->m_nBarShowState == TXBARSHOW_SHOWING)
		{
			//one bar is showing, not show it
		}
		else
		{
			TxUTModifyState(m_nBarState, 0, TXBARSTATE_HOVER);
			TxUTModifyState(m_nBarShowState, 0, TXBARSHOW_HOVER);
		}
	}

	if (nOldShowState != m_nBarShowState || nOldHotPart != m_nBarHotPart
		|| nOldBarState != m_nBarState)
	{
		NotifyRefresh();
	}

}

void CTxScrollBar::NotifyRefresh( int nHotPart /*= TXBARPART_NONE*/ )
{
	if (nHotPart == TXBARPART_NONE)
	{
// 		TRACE("\nbarshow = %d, min= %d,%d, mid = %d,%d, maxbk = %d,%d", m_nBarShowState, 
// 			m_vrcPart[TXBARPART_MINBK].top, m_vrcPart[TXBARPART_MINBK].Height(),
// 			m_vrcPart[TXBARPART_MID].top, m_vrcPart[TXBARPART_MID].Height(),
// 			m_vrcPart[TXBARPART_BKMAX].top, m_vrcPart[TXBARPART_BKMAX].Height());
		m_pParent->Invalidate();
		m_pParent->UpdateWindow();
	}
	else
	{
		m_pParent->InvalidateRect(m_vrcPart[nHotPart]);
	}
}

void CTxScrollBar::OnKillFocus( WPARAM wparam, LPARAM lparam )
{
	if (m_bCaptured)
	{
		m_bCaptured = FALSE;
		ReleaseCapture();
		if (TxUTHasState(m_nBarState, TXBARSTATE_LDOWN))
		{
			TxUTModifyState(m_nBarState, TXBARSTATE_LDOWN, 0);
			NotifyRefresh();
		}
	}
}

BOOL CTxScrollBar::LoadBarBmp( LPCTSTR szFileIn, UINT nMinStartPos, UINT nMaxStartPos, UINT nMidStart, UINT nBkStart )
{
	m_pBarBmp = CTxImageLoader::LoadBitmap(szFileIn);
	TXMARCO_ENSURE2(m_pBarBmp);
	m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_BTNMIN] = nMinStartPos;
	m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_BTNMAX] = nMaxStartPos;
	m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_BTNMIN] = nMidStart;
	m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_BK] = nBkStart;
	m_BarBmpSec.nImgHeight = m_pBarBmp->GetHeight();
	m_BarBmpSec.nImgWidth = m_pBarBmp->GetWidth();
  
	if (m_nBarStyle == SB_VERT)
	{
		ASSERT(nBkStart < m_BarBmpSec.nImgHeight);

		m_BarBmpSec.nBarWidth =  m_BarBmpSec.nImgWidth/4;
		m_BarBmpSec.vrcImgNormal[TXBARPART_MIN] = CRect(0, nMinStartPos, m_BarBmpSec.nBarWidth, nMaxStartPos);
		m_BarBmpSec.vrcImgNormal[TXBARPART_MAX] = CRect(0, nMaxStartPos, m_BarBmpSec.nBarWidth, nMidStart);
		m_BarBmpSec.vrcImgNormal[TXBARPART_MID] = CRect(0, nMidStart, m_BarBmpSec.nBarWidth, nBkStart);
		m_BarBmpSec.vrcImgNormal[TXBARPART_MINBK] = CRect(0, nBkStart, m_BarBmpSec.nBarWidth, m_BarBmpSec.nImgHeight);
		m_BarBmpSec.vrcImgNormal[TXBARPART_BKMAX] = m_BarBmpSec.vrcImgNormal[TXBARPART_MINBK];
	}
	else
	{
		m_BarBmpSec.nBarWidth =  m_BarBmpSec.nImgHeight/4;
		ASSERT(nBkStart < m_BarBmpSec.nImgWidth);

		m_BarBmpSec.vrcImgNormal[TXBARPART_MIN] = CRect(nMinStartPos, 0, nMaxStartPos,m_BarBmpSec.nBarWidth);
		m_BarBmpSec.vrcImgNormal[TXBARPART_MAX] = CRect(nMaxStartPos, 0, nMidStart, m_BarBmpSec.nBarWidth);
		m_BarBmpSec.vrcImgNormal[TXBARPART_MID] = CRect(nMidStart, 0, nBkStart, m_BarBmpSec.nBarWidth);
		m_BarBmpSec.vrcImgNormal[TXBARPART_MINBK] = CRect(nBkStart, 0, m_BarBmpSec.nImgWidth, m_BarBmpSec.nBarWidth);
		m_BarBmpSec.vrcImgNormal[TXBARPART_BKMAX] = m_BarBmpSec.vrcImgNormal[TXBARPART_MINBK];
	}

	return TRUE;
}

void CTxScrollBar::DrawBar( CDC* pDC )
{
	if (m_nBarShowState != TXBARSHOW_SHOWING
		||m_rcBarRect.IsRectEmpty())
	{
		return;
	}
	pDC->FillSolidRect(m_rcBarRect, RGB(0,0,0));

	Graphics gc(pDC->GetSafeHdc());
	GraphicsContainer container =  gc.BeginContainer();
	gc.SetClip(CRect2Rect(m_rcBarRect));
	int nBarWidth = m_nBarStyle== SB_VERT? m_rcBarRect.Height():m_rcBarRect.Width();
// 	Rect rImgMin = Rect(m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMIN].left, m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMIN].top, m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMIN].Width(), m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMIN].Height());
// 	Rect rcImgMax = Rect(m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMAX].left, m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMAX].top, m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMAX].Width(), m_BarBmpSec.vrcImg[TXBARDRAWSEC_BTNMAX].Height());
// 	Rect rcImgMid = Rect(m_BarBmpSec.vrcImg[TXBARDRAWSEC_MID].left, m_BarBmpSec.vrcImg[TXBARDRAWSEC_MID].top, m_BarBmpSec.vrcImg[TXBARDRAWSEC_MID].Width(), m_BarBmpSec.vrcImg[TXBARDRAWSEC_MID].Height());
// 	Rect rcImgMid = Rect(m_BarBmpSec.vrcImg[TXBARDRAWSEC_BAK].left, m_BarBmpSec.vrcImg[TXBARDRAWSEC_BAK].top, m_BarBmpSec.vrcImg[TXBARDRAWSEC_BAK].Width(), m_BarBmpSec.vrcImg[TXBARDRAWSEC_BAK].Height());

// 	Rect rcMin = Rect(m_vrcPart[TXBARPART_MIN].left, m_vrcPart[TXBARPART_MIN].top, m_vrcPart[TXBARPART_MIN].Width(), m_vrcPart[TXBARPART_MIN].Height());
// 	Rect rcMinBk = Rect(m_vrcPart[TXBARPART_MINBK].left, m_vrcPart[TXBARPART_MINBK].top, m_vrcPart[TXBARPART_MINBK].Width(), m_vrcPart[TXBARPART_MINBK].Height());
// 	Rect rcMid = Rect(m_vrcPart[TXBARPART_MID].left, m_vrcPart[TXBARPART_MID].top, m_vrcPart[TXBARPART_MID].Width(), m_vrcPart[TXBARPART_MID].Height());
// 	Rect rcBkMax = Rect(m_vrcPart[TXBARPART_BKMAX].left, m_vrcPart[TXBARPART_BKMAX].top, m_vrcPart[TXBARPART_BKMAX].Width(), m_vrcPart[TXBARPART_BKMAX].Height());
// 	Rect rcMax = Rect(m_vrcPart[TXBARPART_MAX].left, m_vrcPart[TXBARPART_MAX].top, m_vrcPart[TXBARPART_MAX].Width(), m_vrcPart[TXBARPART_MAX].Height());
// 
	if (m_pBarBmp)
	{
		ENUM_TXBMPSTRETCHTYPE nStype = m_nBarStyle == SB_VERT? TXBMP_STRETCH_MID_TB : TXBMP_STRETCH_MID_LR;
		for (int i = TXBARPART_MIN; i <= TXBARPART_MAX; i++ )
		{
			if (!m_vrcPart[i].IsRectEmpty())
			{
				CRect rcImg = m_BarBmpSec.vrcImgNormal[i];
				if (m_nBarHotPart == i)
				{
					if (m_nBarState & TXBARSTATE_HOVER)
					{
						int nOffSet = 2*m_BarBmpSec.nBarWidth;
						if (m_nBarState == TXBARSTATE_PRESSED)
						{
							nOffSet =m_BarBmpSec.nBarWidth;
						}
						m_nBarStyle == SB_VERT? rcImg.OffsetRect(nOffSet, 0):rcImg.OffsetRect(0, nOffSet);

					}

				}
				GPDrawStretchImage(gc, m_pBarBmp, m_vrcPart[i], &rcImg, nStype);
			}

		}
		
	}
	else
	{
		ASSERT(FALSE);
		//to be added
	}

	gc.EndContainer(container);
	
}

HRESULT CTxScrollBar::PostRelayMsg( UINT message, WPARAM wParam, LPARAM lParam )
{
	HRESULT hr = S_OK;
	if (!TxUTHasState(m_nBarShowState, TXBARSHOW_READY) || !m_bBarEnabled)
	{
		return hr;
	}

	if (message == WM_PAINT)
	{
		if (m_pParent && ::IsWindow(m_pParent->GetSafeHwnd())
			&& m_pParent->IsWindowVisible())
		{
			CClientDC dc(m_pParent);
			DrawBar(&dc);
		}
	}

	return hr;
}

void CTxScrollBar::ReCalScrollPartSize()
{
	int nBarWidth = m_nBarStyle== SB_VERT? m_rcBarRect.Height():m_rcBarRect.Width();
	int nBtnLen = m_BarBmpSec.vnBarStartPos[TXBARDRAWSEC_BTNMAX];

	int nBarSlideLen = nBarWidth - 2*nBtnLen;

	//calc mid btn len
	int nMidBtnLen = m_nBarStyle== SB_VERT? m_vrcPart[TXBARPART_MID].Height():m_vrcPart[TXBARPART_MID].Width();

	//calc upbk len
	int nBkUpLen = int (((double)(nBarSlideLen - nMidBtnLen))/m_slInfo.nMax *m_slInfo.nPos);
	int nBkDownLen = nBarSlideLen - nMidBtnLen - nBkUpLen;
	if (m_nBarStyle == SB_VERT)
	{
		m_vrcPart[TXBARPART_MINBK] = CRect(m_rcBarRect.left, m_vrcPart[TXBARPART_MIN].bottom, m_rcBarRect.right, m_vrcPart[TXBARPART_MIN].bottom + nBkUpLen);
		m_vrcPart[TXBARPART_MID] = CRect(m_rcBarRect.left, m_vrcPart[TXBARPART_MINBK].bottom, m_rcBarRect.right, m_vrcPart[TXBARPART_MINBK].bottom +nMidBtnLen);
		m_vrcPart[TXBARPART_BKMAX] = CRect(m_rcBarRect.left, m_vrcPart[TXBARPART_MID].bottom, m_rcBarRect.right, m_vrcPart[TXBARPART_MAX].top);
	}
	else
	{
		m_vrcPart[TXBARPART_MINBK] = CRect(m_vrcPart[TXBARPART_MIN].right, m_rcBarRect.top, m_vrcPart[TXBARPART_MIN].right + nBkUpLen, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_MID] = CRect(m_vrcPart[TXBARPART_MINBK].right, m_rcBarRect.top, m_vrcPart[TXBARPART_MINBK].right + nMidBtnLen, m_rcBarRect.bottom);
		m_vrcPart[TXBARPART_BKMAX] = CRect(m_vrcPart[TXBARPART_MID].right, m_rcBarRect.top, m_vrcPart[TXBARPART_MAX].left, m_rcBarRect.bottom);
	}
}

void CTxScrollBar::SetViewSize( const CSize& szView )
{
	m_szView = szView;
	ReCalSize();
}

BOOL CTxScrollBar::SetScrollPos( int nPos, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(nPos >= 0);
	m_slInfo.nPos = nPos > m_slInfo.nMax? m_slInfo.nMax: nPos;
	if (m_bBarEnabled && bRedraw)
	{
		NotifyRefresh();
	}
	return TRUE;

}

void CTxScrollBar::OnMouseWHeel( WPARAM wparam, LPARAM lparam )
{
	int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
	//TRACE("\n deta = %d", zDelta);
	UINT nOldShowState = m_nBarShowState;
	UINT nOldHotPart = m_nBarHotPart;
	UINT nOldBarState = m_nBarState;

	int nOldPos = m_slInfo.nPos;
	int nNewPos = m_slInfo.nPos + ((zDelta < 0)? m_nScrollPixPerWhell: (-m_nScrollPixPerWhell));
	nNewPos = (nNewPos < m_slInfo.nMin)? m_slInfo.nMin:((nNewPos > m_slInfo.nMax)? m_slInfo.nMax:nNewPos);
	if (nOldPos != nNewPos)
	{
		m_slInfo.nPos = nNewPos;
		ReCalScrollPartSize();
	}
	if (m_nBarHotPart != nOldHotPart || m_nBarShowState != nOldShowState
		|| m_nBarState != nOldBarState ||nOldPos != m_slInfo.nPos)
	{
		NotifyRefresh();

	}
}

BOOL CTxScrollBar::IsShowing()
{
	return m_bBarEnabled && m_nBarShowState == TXBARSHOW_SHOWING;
}
