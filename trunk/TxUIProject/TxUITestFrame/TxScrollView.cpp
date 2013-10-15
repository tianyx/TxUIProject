// TxScrollView.cpp : 实现文件
//

#include "stdafx.h"
#include "TxPreDef.h"
#include "TxScrollView.h"

#define  SCROLL_TROORENT 10
#define  SCROLL_VIEWPERTIME 20
#define  SCROLL_BARMOVEPERTIME 10
#define  MIN_SCROLL_MIDBAR_WIDTH 20
// CTxScrollView

IMPLEMENT_DYNAMIC(CTxScrollView, CTxObjectBase)

CTxScrollView::CTxScrollView():m_szPixPerStepView(SCROLL_VIEWPERTIME, SCROLL_VIEWPERTIME),m_ptViewOffset(0,0)
				,m_szBarbtn(20,20),m_szViewSize(0,0),m_ptScrollLast(-1,-1), m_nBarStyle(0),m_nHitState(0),m_nEnabledStyle(TXBAR_BOTH)
{
	m_dPixPerStepBarMoveV = 0.0;
	m_dPixPerStepBarMoveH = 0.0;
}

CTxScrollView::~CTxScrollView()
{
}

BOOL CTxScrollView::CalculateScrollBar()
{
	m_nBarStyle = 0;
	CSize szWnd = GetSize();
	if (szWnd.cx <= (SCROLL_TROORENT+ m_szBarbtn.cx*2) || szWnd.cy <= (SCROLL_TROORENT + m_szBarbtn.cy*2))
	{
		return FALSE;
	}

	if ((m_nEnabledStyle& TXBAR_HORZ)!=0 && (m_szViewSize.cx >= szWnd.cx + SCROLL_TROORENT))
	{
		m_nBarStyle |= TXBAR_HORZ;
	}

	if ((m_nEnabledStyle& TXBAR_VERT)!=0 && (m_szViewSize.cy >= szWnd.cy + SCROLL_TROORENT))
	{
		m_nBarStyle |= TXBAR_VERT;
	}

	int nCorner = (m_nBarStyle & TXBAR_BOTH) != 0? m_szBarbtn.cx:0;

	if ((m_nBarStyle & TXBAR_HORZ) != 0)
	{
		int nTotalStepH = (m_szViewSize.cx - szWnd.cx);
		if (nTotalStepH > (szWnd.cx - m_szBarbtn.cx*2- nCorner-MIN_SCROLL_MIDBAR_WIDTH)*SCROLL_VIEWPERTIME)
		{
			//view is too large setdefault btn width
			m_nThumbWidthH = MIN_SCROLL_MIDBAR_WIDTH;
			m_dPixPerStepBarMoveH = (double)nTotalStepH / (double)m_szPixPerStepView.cx;

		}
		else
		{
			int nThumbH = MIN_SCROLL_MIDBAR_WIDTH;
			int nBarSetp = SCROLL_BARMOVEPERTIME;
			while(nThumbH <= MIN_SCROLL_MIDBAR_WIDTH && nBarSetp > 0)
			{
				nThumbH = szWnd.cx - m_szBarbtn.cx*2 -nCorner - ((m_szViewSize.cx - szWnd.cx) /m_szPixPerStepView.cx * nBarSetp);
				if (nThumbH <= MIN_SCROLL_MIDBAR_WIDTH)
				{
					nBarSetp --;
				}
			}

			if (nThumbH <= 0)
			{
				ASSERT(FALSE);
				m_nBarStyle &= ~TXBAR_HORZ;
			}
			else
			{
				m_nThumbWidthH = nThumbH;
				m_dPixPerStepBarMoveH = nBarSetp;

			}
		}
		
		
	}

	if ((m_nBarStyle & TXBAR_VERT) != 0)
	{

		int nTotalStepV = (m_szViewSize.cy - szWnd.cy);
		if (nTotalStepV > (szWnd.cy - m_szBarbtn.cy*2- nCorner -MIN_SCROLL_MIDBAR_WIDTH)*SCROLL_VIEWPERTIME)
		{
			//view is too large setdefault btn width
			m_nThumbWidthH = MIN_SCROLL_MIDBAR_WIDTH;
			m_dPixPerStepBarMoveV = (double)nTotalStepV / (double)m_szPixPerStepView.cy;

		}
		else
		{
			int nThumbV = -1;
			int nBarSetp = SCROLL_BARMOVEPERTIME;
			while(nThumbV <= MIN_SCROLL_MIDBAR_WIDTH && nBarSetp > 0)
			{
				nThumbV = szWnd.cy - m_szBarbtn.cy*2 - nCorner - ((m_szViewSize.cy - szWnd.cy) /m_szPixPerStepView.cy * nBarSetp);
				if (nThumbV <= MIN_SCROLL_MIDBAR_WIDTH)
				{
					nBarSetp --;
				}
			}

			if (nThumbV <= 0)
			{
				ASSERT(FALSE);
				m_nBarStyle &= ~TXBAR_VERT;
			}
			else
			{
				m_nThumbWidthV = nThumbV;
				m_dPixPerStepBarMoveV = nBarSetp;
			}
		}
	}

	return TRUE;
}

HRESULT CTxScrollView::DefaultMsgProc( UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	HRESULT hr = CTxObjectBase::DefaultMsgProc(nMsg, wparam, lparam);

	switch(nMsg)
	{
	case WM_LBUTTONDOWN:
		{
			m_nHitState = HitTestBar();
			if ((m_nHitState &TXBAR_BOTH)!= 0 && (m_nHitState & TXBAR_BTN_MID)!= 0)
			{
				SetCapture();
				BeginScrolling();
			}
			else if ((m_nHitState &TXBAR_BOTH)!= 0 && (m_nHitState & TXBAR_BTN_NOT_MID)!= 0)
			{
				//do scroll click action
				DoScrollBarClick();
			}
			else
			{
				EndScrolling();
			}

		}break;
	case WM_LBUTTONDBLCLK:
		{	
			m_nHitState = HitTestBar();
			if ((m_nHitState &TXBAR_BOTH)!= 0 && (m_nHitState & TXBAR_BTN_NOT_MID)!= 0)
			{
				//do scroll click action
				DoScrollBarClick();
			}
		}break;
	case WM_MOUSEMOVE:
		{
			if ((IsInState(TS_SETCAPTURED)) && m_bScrollIng && (MK_LBUTTON & wparam))
			{
				DoScrollBarMove();
			}
			else
			{
				m_nHitState = HitTestBar();
				if (m_nHitState == 0)
				{
					EndScrolling();
				}
				else if ((MK_LBUTTON & wparam) != 0 && m_bScrollIng
					&& (m_nHitState &(TXBAR_BOTH|TXBAR_BTN_MID))!= 0)
				{
					//do scroll bar move action
					//TRACE("\nDoScrollBarMove %x", m_nHitState);

					DoScrollBarMove();
				}
				else
				{
					EndScrolling();
				}
			}
			
		}break;
	case WM_MOUSELEAVE:
		{
			m_nHitState = 0;
			EndScrolling();
		}break;
	case WM_LBUTTONUP:
		{	
			if (IsInState(TS_SETCAPTURED))
			{
				ReleaseCapture();
			}
			EndScrolling();
		}break;

	case WM_SIZE:
		{
			AdJustViewOffSet();
			//TRACE("\n %d,%d", m_ptViewOffset.x, m_ptViewOffset.y);
			CalculateScrollBar();
			Invalidate();
		}
	}

	return hr;
}

void CTxScrollView::BeginScrolling()
{
	m_bScrollIng = TRUE;
	GetCursorPos(&m_ptScrollLast);
}

void CTxScrollView::EndScrolling()
{
	m_bScrollIng = FALSE;
	m_ptScrollLast = CPoint(-1,-1);
}

BOOL CTxScrollView::DoScrollBarClick()
{
	BOOL bHorz = (m_nHitState & TXBAR_HORZ)!= 0;
	int nStep =bHorz? m_szPixPerStepView.cx : m_szPixPerStepView.cy;
	ASSERT(m_nBarStyle & TXBAR_BOTH);
	int nDeta = 0;
	if (m_nHitState & TXBAR_BTN_UP)
	{
		nDeta = -nStep;
	}
	else if (m_nHitState & TXBAR_BTN_DOWN)
	{
		nDeta = nStep;
	}
	else if (m_nHitState & TXBAR_BTN_BKUP)
	{
		nDeta = -nStep;
	}
	else if (m_nHitState & TXBAR_BTN_BKDOWN)
	{
		nDeta = nStep;
	}
	if (nDeta != 0)
	{
		CPoint ptViewNew = m_ptViewOffset;
		if (bHorz)
		{
			ptViewNew.x += nDeta;
		}
		else
		{
			ptViewNew.y += nDeta;
		}
		ScrollView(ptViewNew);
	}
	
	return TRUE;
}

UINT CTxScrollView::HitTestBar()
{
	UINT nHitState = 0;
	if ((m_nBarStyle& TXBAR_BOTH) == 0)
	{
		return nHitState;
	}
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(pt);

	int nCorner = (m_nBarStyle & TXBAR_BOTH) != 0? m_szBarbtn.cx:0;

	
	if (m_nBarStyle & TXBAR_HORZ)
	{
		CRect rcWnd = GetClientRect();
		CRect rcHbar(rcWnd.left, rcWnd.bottom - m_szBarbtn.cy, rcWnd.right -nCorner, rcWnd.bottom);
		if (rcHbar.PtInRect(pt))
		{
			nHitState |=TXBAR_HORZ;
			CRect rcUpBtn;
			CRect rcUpBk;
			CRect rcThumb;
			CRect rcDownBk;
			CRect rcDownBtn;
			GetBarRect(TXBAR_HORZ, rcUpBtn, rcUpBk, rcThumb, rcDownBk, rcDownBtn);
			if (rcUpBtn.PtInRect(pt))
			{
				nHitState |=TXBAR_BTN_UP;
			}
			else if (rcUpBk.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_BKUP;
			}
			else if (rcThumb.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_MID;
			}
			else if (rcDownBk.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_BKDOWN;
			}
			else if(rcDownBtn.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_DOWN;
			}
		}
	}

	if (nHitState & TXBAR_HORZ)
	{
		//hited
		return nHitState;
	}
	//hit vertbar
	if (m_nBarStyle & TXBAR_VERT)
	{
		CRect rcWnd = GetClientRect();
		CRect rcVbar(rcWnd.right - m_szBarbtn.cx, rcWnd.top, rcWnd.right, rcWnd.bottom- nCorner);
		if (rcVbar.PtInRect(pt))
		{
			nHitState |=TXBAR_VERT;
			CRect rcUpBtn;
			CRect rcUpBk;
			CRect rcThumb;
			CRect rcDownBk;
			CRect rcDownBtn;
			GetBarRect(TXBAR_VERT, rcUpBtn, rcUpBk, rcThumb, rcDownBk, rcDownBtn);
			if (rcUpBtn.PtInRect(pt))
			{
				nHitState |=TXBAR_BTN_UP;
			}
			else if (rcUpBk.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_BKUP;
			}
			else if (rcThumb.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_MID;
			}
			else if (rcDownBk.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_BKDOWN;
			}
			else if(rcDownBtn.PtInRect(pt))
			{
				nHitState |= TXBAR_BTN_DOWN;
			}
		}
	}


	return nHitState;
}

int CTxScrollView::GetScrollBarUpBkWidth( UINT nBarStyle )
{
	if((nBarStyle& TXBAR_HORZ) != 0)
	{
		return ((double)(m_ptViewOffset.x))*m_dPixPerStepBarMoveH/m_szPixPerStepView.cx;
	}
	else
	{
		return ((double)(m_ptViewOffset.y))*m_dPixPerStepBarMoveV/m_szPixPerStepView.cy;
	}

}

BOOL CTxScrollView::DoScrollBarMove()
{
	if (m_nHitState & TXBAR_HORZ)
	{
		ASSERT(m_nBarStyle & TXBAR_HORZ);
		CPoint pt;
		GetCursorPos(&pt);
		if (m_ptScrollLast.x < 0)
		{
			//first scroll
			m_ptScrollLast.x = pt.x;
			return TRUE;
		}
		else
		{
			int nDeta = pt.x - m_ptScrollLast.x;
			
				//scroll to right
			int nMinBarStep = m_dPixPerStepBarMoveH;
			nMinBarStep = (nMinBarStep==0)? 1:nMinBarStep;
			if (abs(nDeta) >= nMinBarStep)
			{
				if (FLOATISEZERO(m_dPixPerStepBarMoveH))
				{
					ASSERT(FALSE);
				}
				//change step
				int nSteps =((double)nDeta) /m_dPixPerStepBarMoveH; 
				ASSERT(abs(nSteps) > 0);
				CPoint ptViewNew = m_ptViewOffset;
				ptViewNew.x += nSteps * m_szPixPerStepView.cx;
				m_ptScrollLast.x = pt.x;
				ScrollView(ptViewNew);
				
			}
		}
	}
	else if (m_nHitState & TXBAR_VERT)
	{
		ASSERT(m_nBarStyle & TXBAR_VERT);
		CPoint pt;
		GetCursorPos(&pt);
		if (m_ptScrollLast.y < 0)
		{
			//first scroll
			m_ptScrollLast.y = pt.y;
			return TRUE;
		}
		else
		{
			int nDeta = pt.y - m_ptScrollLast.y;
			//scroll to right
			int nMinBarStep = m_dPixPerStepBarMoveV;
			nMinBarStep = (nMinBarStep==0)? 1:nMinBarStep;
			if (abs(nDeta) >= nMinBarStep)
			{
				//change step
				if (FLOATISEZERO(m_dPixPerStepBarMoveV))
				{
					ASSERT(FALSE);
				}
				int nSteps =(double)nDeta /m_dPixPerStepBarMoveV; 
				ASSERT(abs(nSteps) > 0);
				CPoint ptViewNew = m_ptViewOffset;
				ptViewNew.y += nSteps * m_szPixPerStepView.cy;
				m_ptScrollLast.y = pt.y;
				ScrollView(ptViewNew);
				//TRACE("--%d, %d", ptViewNew.x, ptViewNew.y);
				
			}
		}
	}
	return TRUE;
}

BOOL CTxScrollView::ScrollView( CPoint& ptViewInOut )
{
	if (m_ptViewOffset == ptViewInOut)
	{
		return TRUE;
	}
	
	//correct view
	ptViewInOut.x = ptViewInOut.x < 0? 0:ptViewInOut.x;
	ptViewInOut.y = ptViewInOut.y < 0? 0:ptViewInOut.y;

	CSize m_szWnd = GetSize();
	int nMaxOrgX =	m_szViewSize.cx- m_szWnd.cx;
	int nMaxOrgY = m_szViewSize.cy- m_szWnd.cy;
	ptViewInOut.x = ptViewInOut.x > nMaxOrgX? nMaxOrgX:ptViewInOut.x;
	ptViewInOut.y = ptViewInOut.y > nMaxOrgY? nMaxOrgY:ptViewInOut.y;
	ptViewInOut.x = ptViewInOut.x < 0? 0:ptViewInOut.x;
	ptViewInOut.y = ptViewInOut.y < 0? 0:ptViewInOut.y;
	if (ptViewInOut == m_ptViewOffset)
	{
		return TRUE;
	}

	m_ptViewOffset = ptViewInOut;
	Invalidate();

	return TRUE;
}

CRect CTxScrollView::GetClientViewRect()
{
	CRect rc = GetClientRect();
	if (m_nBarStyle & TXBAR_HORZ)
	{
		rc.bottom -= m_szBarbtn.cy;
	}
	if (m_nBarStyle & TXBAR_VERT)
	{
		rc.right -= m_szBarbtn.cx;
	}

	if(rc.IsRectEmpty())
	{
		ASSERT(FALSE);
		rc = CRect(0,0,0,0);
	}
	return rc;
}

void CTxScrollView::Draw( CDC* pDC )
{
	CRect rcView = GetClientViewRect();
 	pDC->IntersectClipRect(rcView);
	DrawView(pDC);
	pDC->SelectClipRgn(NULL);
	if (m_nBarStyle == 0)
	{
		return;
	}
	int nCorner = (m_nBarStyle & TXBAR_BOTH) != 0? m_szBarbtn.cx:0;
	CRect rcWnd = GetClientRect();
	if (nCorner)
	{
		CRect rcCorner(rcWnd.right - m_szBarbtn.cx, rcWnd.bottom - m_szBarbtn.cy, rcWnd.right, rcWnd.bottom);
		pDC->FillSolidRect(rcCorner, RGB(0,0,0));
	}
	CSize szSize = GetSize();

	CRect rcUpBtn;
	CRect rcUpBk;
	CRect rcThumb;
	CRect rcDownBk;
	CRect rcDownBtn;

	if (m_nBarStyle & TXBAR_HORZ)
	{
		//draw horizon bar
		GetBarRect(TXBAR_HORZ, rcUpBtn, rcUpBk, rcThumb, rcDownBk, rcDownBtn);
		pDC->FillSolidRect(rcUpBtn, RGB(255,0,0));
		//TRACE("\nclient.bottom = %d hbar.top = %d", rcWnd.bottom, rcUpBtn.top);
		pDC->FillSolidRect(rcUpBk, RGB(255,150,0));
		pDC->FillSolidRect(rcThumb, RGB(255,150,150));
		pDC->FillSolidRect(rcDownBk, RGB(255,150,0));
		pDC->FillSolidRect(rcDownBtn, RGB(255,0,0));

	}
	if (m_nBarStyle & TXBAR_VERT)
	{
		GetBarRect(TXBAR_VERT, rcUpBtn, rcUpBk, rcThumb, rcDownBk, rcDownBtn);
		pDC->FillSolidRect(rcUpBtn, RGB(255,0,0));
		pDC->FillSolidRect(rcUpBk, RGB(255,150,0));
		pDC->FillSolidRect(rcThumb, RGB(255,0,150));
		pDC->FillSolidRect(rcDownBk, RGB(255,150,0));
		pDC->FillSolidRect(rcDownBtn, RGB(255,0,0));
	}

}

BOOL CTxScrollView::GetBarRect( UINT nBarStyleIn, CRect& rcUpBtn, CRect& rcUpBk, CRect& rcThumb, CRect& rcDownBk, CRect& rcDownBtn )
{
	rcUpBtn.SetRectEmpty();
	rcUpBk.SetRectEmpty();
	rcThumb.SetRectEmpty();
	rcDownBk.SetRectEmpty();
	rcDownBtn.SetRectEmpty();

	int nCorner = (m_nBarStyle & TXBAR_BOTH) != 0? m_szBarbtn.cx:0;
	CRect rcWnd = GetClientRect();
	if (nBarStyleIn & TXBAR_HORZ)
	{
		if ((m_nBarStyle & TXBAR_HORZ )== 0)
		{
			return FALSE;
		}

		CRect rcHbar(rcWnd.left, rcWnd.bottom - m_szBarbtn.cy, rcWnd.right -nCorner, rcWnd.bottom);
		rcUpBtn = CRect (rcHbar.left, rcHbar.top, m_szBarbtn.cy, rcHbar.bottom);
		rcDownBtn =CRect (rcHbar.right - m_szBarbtn.cx, rcHbar.top, rcHbar.right, rcHbar.bottom);
		rcUpBk =CRect (rcUpBtn.right, rcHbar.top, rcUpBtn.right+ GetScrollBarUpBkWidth(TXBAR_HORZ), rcHbar.bottom);
		rcThumb =CRect (rcUpBk.right, rcHbar.top, rcUpBk.right+m_nThumbWidthH, rcHbar.bottom);
		rcDownBk =CRect (rcThumb.right, rcHbar.top, rcDownBtn.left, rcHbar.bottom);
	}
	else if (nBarStyleIn & TXBAR_VERT)
	{
		if ((m_nBarStyle & TXBAR_VERT )== 0)
		{
			return FALSE;
		}

		CRect rcVbar(rcWnd.right - m_szBarbtn.cx, rcWnd.top, rcWnd.right, rcWnd.bottom- nCorner);
		rcUpBtn= CRect (rcVbar.left, rcVbar.top, rcVbar.right, rcVbar.top + m_szBarbtn.cy);
		rcDownBtn =CRect (rcVbar.left, rcVbar.bottom - m_szBarbtn.cy, rcVbar.right, rcVbar.bottom);
		rcUpBk =CRect (rcVbar.left, rcUpBtn.bottom, rcVbar.right,  rcUpBtn.bottom + GetScrollBarUpBkWidth(TXBAR_VERT));
		rcThumb =CRect (rcVbar.left, rcUpBk.bottom, rcVbar.right, rcUpBk.bottom + m_nThumbWidthV);
		rcDownBk =CRect (rcVbar.left, rcThumb.bottom, rcVbar.right,  rcDownBtn.top);
	}

	return TRUE;
}

BOOL CTxScrollView::SetViewSize( CSize& szViewInOut )
{
	if (szViewInOut == m_szViewSize)
	{
		return FALSE;
	}
	CSize szWnd = GetSize();
	m_szViewSize.cx = szViewInOut.cx < szWnd.cx?szWnd.cx :szViewInOut.cx;
	m_szViewSize.cy = szViewInOut.cy < szWnd.cy? szWnd.cy:szViewInOut.cy;
	m_ptViewOffset.x = m_ptViewOffset.x > m_szViewSize.cx? m_szViewSize.cx:m_ptViewOffset.x;
	m_ptViewOffset.y = m_ptViewOffset.y > m_szViewSize.cy? m_szViewSize.cy:m_ptViewOffset.y;
	Invalidate();

	return TRUE;
}


/*
CRect CTxScrollView::GetInWndClientRect()
{
	CRect rcWnd = GetClientViewRect();
	if (m_nBarStyle & TXBAR_HORZ)
	{
		rcWnd.bottom -= m_szBarbtn.cy;
	}
	if (m_nBarStyle & TXBAR_VERT)
	{
		rcWnd.right -= m_szBarbtn.cx;
	}
	return rcWnd;
}*/

CRect CTxScrollView::GetInWndViewRect()
{
	CRect rcWnd = GetInWndRect();
	rcWnd.right = rcWnd.left + m_szViewSize.cx;
	rcWnd.bottom = rcWnd.top + m_szViewSize.cy;
	rcWnd.OffsetRect(-m_ptViewOffset.x, -m_ptViewOffset.y);
	return rcWnd;
}

void CTxScrollView::DrawView( CDC* pDC )
{
	return;
}

void CTxScrollView::AdJustViewOffSet()
{
	CRect rcViewClient = GetClientViewRect();
	
	if (m_ptViewOffset.y > 0 && (m_szViewSize.cy - m_ptViewOffset.y)< rcViewClient.Height())
	{
		m_ptViewOffset.y = m_szViewSize.cy - rcViewClient.Height();
	}

	if (m_ptViewOffset.x > 0 && (m_szViewSize.cx - m_ptViewOffset.x) < rcViewClient.Width())
	{
		m_ptViewOffset.x = m_szViewSize.cx - rcViewClient.Width();
	}

	m_ptViewOffset.x = m_ptViewOffset.x < 0? 0:m_ptViewOffset.x;
	m_ptViewOffset.y = m_ptViewOffset.y < 0? 0:m_ptViewOffset.y;

}





// CTxScrollView 消息处理程序


