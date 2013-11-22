/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxImgButton.cpp
	file base:	TxImgButton
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// TxImgButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TxImgButton.h"
#include "TxImageLoader.h"
#include "GDIDrawFunc.h"

// CTxImgButton

IMPLEMENT_DYNAMIC(CTxImgButton, CButton)

CTxImgButton::CTxImgButton()
{
	m_bMouseOver = FALSE;
	m_bSetLeaveTrack = FALSE;
	m_bPressed = FALSE;
}

CTxImgButton::~CTxImgButton()
{
}


BEGIN_MESSAGE_MAP(CTxImgButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CTxImgButton::LoadBitmap( LPCTSTR szFileIn, ENUM_TXBMPSTRETCHTYPE sTypeIn )
{
	Bitmap* pBmp = CTxImageLoader::LoadBitmap(szFileIn);
	if (pBmp)
	{
		m_txInfo.nBmpWidth = pBmp->GetWidth();
		m_txInfo.nBmpHeight = pBmp->GetHeight();
		m_txInfo.nSubBmpWidth = m_txInfo.nBmpWidth /4;
		ASSERT(m_txInfo.nBmpWidth >= 24 && m_txInfo.nBmpHeight >=6);
		m_txInfo.m_vBmps.push_back(pBmp);
		m_txInfo.nStretchType = sTypeIn;
		m_txInfo.nCurrBmps = 0;
	}

	return pBmp != NULL;
}

BOOL CTxImgButton::AddBitmap( LPCTSTR szFileIn )
{
	if (m_txInfo.m_vBmps.size() == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	Bitmap* pBmp =  CTxImageLoader::LoadBitmap(szFileIn);
	if (pBmp)
	{
		if (pBmp->GetWidth() != m_txInfo.nBmpWidth
			|| pBmp->GetHeight() != m_txInfo.nBmpHeight)
		{
			return FALSE;
		}
	}

	m_txInfo.m_vBmps.push_back(pBmp);
	return TRUE;
}



// CTxImgButton ��Ϣ�������



void CTxImgButton::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW, FALSE);

	CButton::PreSubclassWindow();

	CRect rcWindow;
	GetWindowRect(&rcWindow);
	if (m_txInfo.nStretchType == TXBMP_STRETCH_NONE)
	{
		SetWindowPos(NULL, 0, 0, m_txInfo.nSubBmpWidth,
			m_txInfo.nSubBmpWidth,
			SWP_NOZORDER | SWP_NOMOVE);
	}
}

void CTxImgButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bMouseOver == FALSE)
	{   
		m_bMouseOver = TRUE;
 		CheckRedraw();
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

	CButton::OnMouseMove(nFlags, point);
}

void CTxImgButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bPressed = TRUE;
 	CheckRedraw();
	CButton::OnLButtonDown(nFlags, point);
}

void CTxImgButton::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bPressed = FALSE;
	m_bMouseOver = FALSE;
	CheckRedraw();
	CButton::OnMouseLeave();
}

void CTxImgButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  ������Ĵ����Ի���ָ����
	if (m_txInfo.nBmpWidth == 0 || m_txInfo.nBmpHeight == 0)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	if (rcClient.IsRectEmpty())
	{
		return;
	}

	UINT nBtnState = GetBtnState();
	//TRACE("\n draw state = %d", nBtnState);
	CRect rcImgUse(0,0, m_txInfo.nSubBmpWidth, m_txInfo.nBmpHeight);
	if (nBtnState & TX_BTNSTATE_GRAY)
	{
		rcImgUse.OffsetRect(3* m_txInfo.nSubBmpWidth, 0);
	}
	else if(nBtnState == TX_BTNSTATE_PRESSED)
	{
		rcImgUse.OffsetRect(2* m_txInfo.nSubBmpWidth, 0);
	}
	else if (nBtnState & TX_BTNSTATE_HOVER)
	{
		rcImgUse.OffsetRect(1* m_txInfo.nSubBmpWidth, 0);
	}

	GPDrawStretchImage(lpDrawItemStruct->hDC, m_txInfo.m_vBmps[m_txInfo.nCurrBmps],  rcClient, &rcImgUse, TXBMP_STRETCH_MID_LRTB, &m_txInfo.rcEdge);

}

UINT CTxImgButton::GetBtnState()
{
	UINT nState = TX_BTNSTATE_NORMAL;
	if (!IsWindowEnabled())
	{
		nState = TX_BTNSTATE_GRAY;
	}
	
	if (m_bMouseOver)
	{
		nState |= TX_BTNSTATE_HOVER;
		if (m_bPressed)
		{
			nState |= TX_BTNSTATE_LDOWN;
		}
	}
	return nState;
}

void CTxImgButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bPressed = FALSE;
	CheckRedraw();
	CButton::OnLButtonUp(nFlags, point);
}

void CTxImgButton::CheckRedraw()
{
	UINT nNewState = GetBtnState();
	if(m_nBtnState != nNewState)
	{
		m_nBtnState = nNewState;
		Invalidate(FALSE);
		UpdateWindow();
	}

}

void CTxImgButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bPressed = TRUE;
	CheckRedraw();
	CButton::OnLButtonDblClk(nFlags, point);
}

BOOL CTxImgButton::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

int CTxImgButton::SetCurrImgIdx( int nIdx )
{
	if (m_txInfo.nCurrBmps == nIdx)
	{
		return nIdx;
	}

	if (nIdx >= 0 && nIdx < m_txInfo.m_vBmps.size())
	{
		m_txInfo.nCurrBmps = nIdx;
		Invalidate();
		return nIdx;
	}
	else
	{
		return m_txInfo.nCurrBmps;
	}
}
