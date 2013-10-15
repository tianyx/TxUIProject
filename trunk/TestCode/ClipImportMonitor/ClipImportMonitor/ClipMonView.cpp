/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipMonView.cpp
	file base:	ClipMonView
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipMonView.cpp : 实现文件
//

#include "stdafx.h"
#include "ClipImportMonitor.h"
#include "ClipMonView.h"
#include "TxScrollWnd.h"
#include "GDIDrawFunc.h"
#include "MonDataMgr.h"
#include "TxFontLoader.h"

#define  IDTIMER_REMAINTIME_DRAW 1122
// CClipMonView
IMPLEMENT_DYNAMIC(CClipMonView, CTxListCtrl)

CClipMonView::CClipMonView():m_BkBufferlock(NULL, TEXT("{A8DCE9B1-B0FB-49bd-BCF0-5C8D9E324B10}"))
{
	m_szbmpCache = CSize(2000, 2000);
	m_szbmpBackBuffer = CSize(g_globalInfo.cacheSetting.szCacheSize.cx,g_globalInfo.cacheSetting.szCacheSize.cy);
	m_pBmpBackBuffer = new Bitmap(m_szbmpBackBuffer.cx, m_szbmpBackBuffer.cy,PixelFormat32bppARGB );
	m_pBmpRemainTime = new Bitmap(2000, 2000,PixelFormat32bppARGB );
	m_bmpCache.CreateBitmap(m_szbmpCache.cx,m_szbmpCache.cy,1,32,NULL);
	int nFont = g_FontLoader.AddFont(m_drawParam.strFontFace, m_drawParam.nFontHeight, m_drawParam.bFontBlod);
	m_drawParam.pFont = g_FontLoader.GetFont(nFont);
	m_rcViewBkBuffer.SetRectEmpty();
	m_bUseBkBuffer = TRUE;
	m_nItemSize = 0;
}

CClipMonView::~CClipMonView()
{
	if (m_pBmpRemainTime)
	{
		delete m_pBmpRemainTime;
		m_pBmpRemainTime = NULL;
	}

	if (m_pBmpBackBuffer)
	{
		delete m_pBmpBackBuffer;
		m_pBmpBackBuffer = NULL;
	}
}


BEGIN_MESSAGE_MAP(CClipMonView, CTxListCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CClipMonView::OnDraw( CDC *pDC)
{
	CRect rcClient;
	TxGetClientRect(rcClient);
	Graphics gc(pDC->GetSafeHdc());
	SolidBrush bkBrush(m_drawParam.bkColor);
	Rect rCet = CRect2Rect(rcClient);
	gc.FillRectangle(&bkBrush, rCet);
	CSize szView = GetScrollViewSize();
	CRect rcView(0, 0, szView.cx, szView.cy);
	rcView.OffsetRect(rcClient.left, rcClient.top);

	if (rcView.IsRectEmpty())
	{
		return;
	}
	rcView.OffsetRect(-GetScrollPos(SB_HORZ), -GetScrollPos(SB_VERT));

	GraphicsContainer container = gc.BeginContainer();
	Rect rClip = CRect2Rect(rcClient);
	gc.SetClip(rClip);
	

	
	if (m_bUseBkBuffer)
	{
		CRect rcBuffToDraw(0,0,rcClient.Width(), rcClient.Height());
		rcBuffToDraw.OffsetRect(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
		if (!IsInRect(rcBuffToDraw, m_rcViewBkBuffer))
		{
			TRACE("\nbufferRedraw %d, %d", rcBuffToDraw.bottom, m_rcViewBkBuffer.bottom);
			ReDrawBkBuffer();
		}
		//draw bkbuffer
		m_BkBufferlock.Lock();
		rcBuffToDraw.OffsetRect(-m_rcViewBkBuffer.left, -m_rcViewBkBuffer.top);
		gc.DrawImage(m_pBmpBackBuffer, rClip, rcBuffToDraw.left, rcBuffToDraw.top, rcBuffToDraw.Width(), rcBuffToDraw.Height(), UnitPixel);
		m_BkBufferlock.Unlock();

	}
	else
	{
		int nBegin = (rcClient.top - rcView.top) / m_drawParam.nItemHeight;
		int nCount = rcClient.Height() /m_drawParam.nItemHeight +1;
		VECTMPITEM vData;
		g_monDataMgr.GetRangeData(nBegin, nCount, vData);
		nCount = vData.size();
		CTxListHeader& headerCtrl = GetListHeader();

		int nTopPos = rcView.top + (nBegin * m_drawParam.nItemHeight);
		Gdiplus::StringFormat fmt;
		fmt.SetAlignment(StringAlignmentNear);
		fmt.SetLineAlignment(StringAlignmentCenter);
		fmt.SetTrimming(StringTrimmingEllipsisCharacter);
		fmt.SetFormatFlags(StringFormatFlagsLineLimit);
		Pen pen(g_globalInfo.viewSetting.clrSeparateLine, 1.0);
		Rect rRowBk(rcClient.left, nTopPos, rcClient.Width(), m_drawParam.nItemHeight);

		for (int i = 0; i < nCount; i++)
		{
			ARGB clr = 0xff000000;
			ARGB clrBk = 0xffffffff;
			vData[i].GetMonColor(clr, clrBk);
			bkBrush.SetColor(clrBk);
			gc.FillRectangle(&bkBrush, rRowBk);
			int nLeftPos = rcView.left;
			CRect rcItem(nLeftPos,nTopPos, 0, nTopPos + m_drawParam.nItemHeight);
			for (int cIdx = 0;  cIdx < m_ColSetting.m_vTmpCols.size(); cIdx++)
			{
				if (rcItem.left > rcClient.right)
				{
					break;
				}
				rcItem.right = rcItem.left + headerCtrl.GetHeaderWidth(cIdx);
				if (rcItem.right >= rcClient.left)
				{
					CString strText = vData[i].GetValue((ENUM_MONTYPE)(m_ColSetting.m_vTmpCols[cIdx].nPosInType));
					GPDrawShadowTextSimple(gc, strText, rcItem, *m_drawParam.pFont, clr, 0, 2,2, &fmt);
					//GPDrawShadowText(gc, strText, rcItem, *m_drawParam.pFont, clr, 0xff000000,0,0,0,0,&fmt);
					//draw separate line
					gc.DrawLine(&pen, rcItem.right-1, rcItem.top, rcItem.right-1, rcItem.bottom);

				}
				rcItem.OffsetRect(rcItem.Width(), 0);

			}
			gc.DrawLine(&pen, rcClient.left, rcItem.bottom-1, rcClient.right, rcItem.bottom-1);
			nTopPos += m_drawParam.nItemHeight;

			rRowBk.Offset(0, m_drawParam.nItemHeight);
		}
	}
	

	//calc In Bound Item
	gc.EndContainer(container);
	ReDrawRemainTime(pDC);

}



// CClipMonView 消息处理程序


int CClipMonView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTxListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	EnableTxScrollBar(SB_VERT);
	//EnableTxScrollBar(SB_HORZ);
	CTxListHeader& headerCtrl = GetListHeader();
	headerCtrl.EnableHeader();
	headerCtrl.SetTotalWidthFixed(g_globalInfo.viewSetting.bHeaderFitClient);
	if (g_globalInfo.viewSetting.bEnableHBar)
	{
		EnableTxScrollBar(SB_HORZ);
	}
	headerCtrl.LoadBkImage(TEXT("skins\\listheaderbk.bmp"));

	m_ColSetting =  g_CoInfoMgr.GetTemplateViewInfo();
	for (int i = 0; i < m_ColSetting.m_vTmpCols.size(); i++)
	{
		ST_LISTHEADERITEM headItem;
		headItem.strText = m_ColSetting.m_vTmpCols[i].strTitle;
		headItem.nMinWidth = m_ColSetting.m_vTmpCols[i].nMinWidth;
		headItem.nWidth = m_ColSetting.m_vTmpCols[i].nColWidth;
		headItem.bAutoWidth = !m_ColSetting.m_vTmpCols[i].bWidthFixed;
		headerCtrl.AddHeaderItem(headItem);
	}

	CRect rcThis;
	GetClientRect(rcThis);
	SetScrollViewSize(CSize(headerCtrl.GetHeaderWidth(), rcThis.Height()));
	
	g_monDataMgr.RegisterObserverWnd(this->GetSafeHwnd());

	SetTimer(IDTIMER_REMAINTIME_DRAW, 1000, NULL);
	return 0;
}

BOOL CClipMonView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	HGDIOBJ oldObj = memDC.SelectObject(&m_bmpCache);
	CRect rcThis;
	GetClientRect(rcThis);
	OnDraw(&memDC);
	CTxListCtrl::OnEraseBkgnd(&memDC);
	pDC->BitBlt(rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(oldObj);
	return TRUE;
}

LRESULT CClipMonView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_MONDATACHANGED)
	{
		m_nItemSize = (UINT) wParam;
		CSize szView = GetScrollViewSize();
		int nNewHeight = g_monDataMgr.GetDataCount() * m_drawParam.nItemHeight;
		CRect rcThis;
		TxGetClientRect(rcThis);
		if (nNewHeight < rcThis.Height())
		{
			nNewHeight = rcThis.Height();
		}
		if (szView.cy != nNewHeight)
		{
			szView.cy = nNewHeight;
			CTxListCtrl::SetScrollViewSize(szView);
		}
		ReDrawBkBuffer();
		Invalidate();
		GetParent()->PostMessage(MSG_MONDATACHANGED, 0,0);

	}
	else if (message == MSG_TXHEADER_WIDTHCHANGED)
	{
		CSize szView = GetScrollViewSize();
		int nNewWidth = GetListHeader().GetHeaderWidth();
		if (szView.cx != nNewWidth)
		{
			szView.cx = nNewWidth;
			SetScrollViewSize(szView);
		}
		ReDrawBkBuffer();
		Invalidate();
		return TRUE;
	}
	return CTxListCtrl::WindowProc(message, wParam, lParam);
}

void CClipMonView::OnSize(UINT nType, int cx, int cy)
{
	CTxListCtrl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (!g_globalInfo.viewSetting.bHeaderFitClient)
	{
		return;
	}
	CSize szView = GetScrollViewSize();
	CRect rcThis;
	TxGetClientRect(rcThis);
	if (rcThis.Width() != szView.cx)
	{
		szView.cx = rcThis.Width();
		SetScrollViewSize(szView);
	}
}

void CClipMonView::ReSizeBkBuffer( CSize& szIn )
{
	if (!m_bUseBkBuffer)
	{
		return;
	}
	if (szIn.cx <= m_szbmpBackBuffer.cx)
	{
		return;
	}

	m_BkBufferlock.Lock();
	if (szIn.cx > m_szbmpBackBuffer.cx)
	{
		m_szbmpBackBuffer.cx =2;

	}
	if (m_pBmpBackBuffer)
	{
		delete m_pBmpBackBuffer;
		m_pBmpBackBuffer = NULL;
	}
// 	if (m_bmpBackBuffer.GetSafeHandle())
// 	{
// 		m_bmpBackBuffer.DeleteObject();
// 	}
	
	//m_bmpBackBuffer.CreateBitmap(m_szbmpBackBuffer.cx, m_szbmpBackBuffer.cy,1,32,NULL);
	if (m_pBmpBackBuffer)
	{
		delete m_pBmpBackBuffer;
	}
	m_pBmpBackBuffer = new Bitmap(m_szbmpBackBuffer.cx, m_szbmpBackBuffer.cy, PixelFormat32bppARGB );
	ReDrawBkBuffer();
	m_BkBufferlock.Unlock();
}

void CClipMonView::SetScrollViewSize( CSize szView )
{
	ReSizeBkBuffer(szView);
	CTxListCtrl::SetScrollViewSize(szView);
}

void CClipMonView::ReDrawBkBuffer()
{
	if (!m_bUseBkBuffer)
	{
		return;
	}
	CSize szView = GetScrollViewSize();
	if (szView.cx == 0 || szView.cy == 0)
	{
		return;
	}
	
	CRect rcClient;
	TxGetClientRect(rcClient);
	CRect rcViewShowBuffer(rcClient);
	rcViewShowBuffer.MoveToXY(0,0);

	rcViewShowBuffer.OffsetRect(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	
	CRect rcBk(0,0, m_szbmpBackBuffer.cx, m_szbmpBackBuffer.cy);
	rcBk = GetCenterRect(rcViewShowBuffer, rcBk);
	
	if (rcBk.left < 0)
	{
		rcBk.MoveToX(0);
	}
	if (rcBk.top < 0)
	{
		rcBk.MoveToY(0);
	}
	m_rcViewBkBuffer = rcBk;
	m_BkBufferlock.Lock();
	ASSERT(m_pBmpBackBuffer != NULL);
	Graphics gc(m_pBmpBackBuffer);
	SolidBrush bkBrush(m_drawParam.bkColor);
	Rect rCet = CRect2Rect(CRect(0,0, rcBk.Width(),  rcBk.Height()));
	gc.FillRectangle(&bkBrush, rCet);
	CRect rcView(0, 0, szView.cx, szView.cy);
	//calc In Bound Item
	int nBegin = (rcBk.top - rcView.top) / m_drawParam.nItemHeight;
	int nCount = rcBk.Height() /m_drawParam.nItemHeight +1;
	VECTMPITEM vData;
	g_monDataMgr.GetRangeData(nBegin, nCount, vData);
	nCount = vData.size();
	if (nCount == 0)
	{
		return;
	}
	CTxListHeader& headerCtrl = GetListHeader();
	int nTopPos = rcView.top + (nBegin * m_drawParam.nItemHeight)- rcBk.top;
	Gdiplus::StringFormat fmt;
	fmt.SetAlignment(StringAlignmentCenter);
	fmt.SetLineAlignment(StringAlignmentCenter);
	fmt.SetTrimming(StringTrimmingEllipsisCharacter);
	fmt.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
	Pen pen(g_globalInfo.viewSetting.clrSeparateLine, 1.0);
	Rect rRowBk(0, nTopPos, rcBk.Width(), m_drawParam.nItemHeight);

	for (int i = 0; i < nCount; i++)
	{
		ARGB clr = 0xff000000;
		ARGB clrBk = 0xffffffff;
		vData[i].GetMonColor(clr, clrBk);
		bkBrush.SetColor(clrBk);
		gc.FillRectangle(&bkBrush, rRowBk);
		int nLeftPos = 0-rcBk.left;
		CRect rcItem(nLeftPos,nTopPos, 0, nTopPos + m_drawParam.nItemHeight);
		for (int cIdx = 0;  cIdx < m_ColSetting.m_vTmpCols.size(); cIdx++)
		{
			if (rcItem.left > rcBk.right)
			{
				break;
			}
			rcItem.right = rcItem.left + headerCtrl.GetHeaderWidth(cIdx);
			if (rcItem.right >= rcBk.left)
			{
				ENUM_MONTYPE nMonType = (ENUM_MONTYPE)(m_ColSetting.m_vTmpCols[cIdx].nPosInType);
				if (nMonType != MONTYPE_TIMEREMAIN)
				{
					CString strText = vData[i].GetValue(nMonType);
					GPDrawShadowTextSimple(gc, strText, rcItem, *m_drawParam.pFont, clr, 0, 2,2, &fmt);
					//GPDrawShadowText(gc, strText, rcItem, *m_drawParam.pFont, clr, 0x22000000,0,0,0,0,&fmt);
				}
				//draw separate line
				gc.DrawLine(&pen, rcItem.right-1, rcItem.top, rcItem.right-1, rcItem.bottom);

			}
			rcItem.OffsetRect(rcItem.Width(), 0);

		}
		gc.DrawLine(&pen, rcBk.left, rcItem.bottom-1, rcBk.right, rcItem.bottom-1);
		nTopPos += m_drawParam.nItemHeight;

		rRowBk.Offset(0, m_drawParam.nItemHeight);
	}

	m_BkBufferlock.Unlock();
}

void CClipMonView::ReDrawRemainTime(CDC* pDC)
{
	CRect rcClient;
	TxGetClientRect(rcClient);
	if (m_scBar[SB_VERT].IsShowing())
	{
		rcClient.DeflateRect(0,0,m_scBar[SB_VERT].GetBarWidth(), 0);
	}
	if (m_scBar[SB_HORZ].IsShowing())
	{
		rcClient.DeflateRect(0,0, 0, m_scBar[SB_HORZ].GetBarWidth());
	}
// 	Rect rCet = CRect2Rect(rcClient);
// 	gc.FillRectangle(&bkBrush, rCet);
	CSize szView = GetScrollViewSize();
	CRect rcView(0, 0, szView.cx, szView.cy);
	rcView.OffsetRect(rcClient.left, rcClient.top);

	if (rcView.IsRectEmpty())
	{
		return;
	}
	rcView.OffsetRect(-GetScrollPos(SB_HORZ), -GetScrollPos(SB_VERT));

	int nBegin = (rcClient.top - rcView.top) / m_drawParam.nItemHeight;
	int nCount = rcClient.Height() /m_drawParam.nItemHeight +1;
	VECTMPITEM vData;
	g_monDataMgr.GetRangeData(nBegin, nCount, vData);
	nCount = vData.size();
	if (nCount <= 0)
	{
		return;
	}
	CTxListHeader& headerCtrl = GetListHeader();

	int nLeftPos = rcView.left;
	int nRightPos = 0;
	for (int cIdx = 0;  cIdx < m_ColSetting.m_vTmpCols.size(); cIdx++)
	{

		ENUM_MONTYPE nMonType =  (ENUM_MONTYPE)(m_ColSetting.m_vTmpCols[cIdx].nPosInType);
		if (nMonType == MONTYPE_TIMEREMAIN)
		{
			nRightPos = nLeftPos + headerCtrl.GetHeaderWidth(cIdx);
			break;
		}
		else
		{
			nLeftPos += headerCtrl.GetHeaderWidth(cIdx);
		}
	}

	if (nRightPos < rcClient.left || nLeftPos > rcClient.right)
	{
		return;
	}

	CPoint ptOffSetBmp;
	int nTopPos = rcView.top + (nBegin * m_drawParam.nItemHeight);
	ptOffSetBmp.x = nLeftPos < 0? -nLeftPos:0;
	ptOffSetBmp.y = nTopPos < rcClient.top? rcClient.top - nTopPos: 0;
	CRect rcRemainTimeClient(nLeftPos, rcClient.top, nRightPos-1, rcClient.bottom);
	Graphics gc(m_pBmpRemainTime);
	SolidBrush bkBrush(m_drawParam.bkColor);
	GraphicsContainer container = gc.BeginContainer();
	int nClipHeight = m_drawParam.nItemHeight*nCount;
	if (nClipHeight < rcClient.Height())
	{
		nClipHeight = rcClient.Height();
	}
	Rect rClip(0,0, rcRemainTimeClient.Width(), nClipHeight);
	gc.SetClip(rClip);
	Rect rDes(nLeftPos, nTopPos, rClip.Width, rClip.Height);
	gc.FillRectangle(&bkBrush, rClip);
	Gdiplus::StringFormat fmt;
	fmt.SetAlignment(StringAlignmentCenter);
	fmt.SetLineAlignment(StringAlignmentCenter);
	fmt.SetTrimming(StringTrimmingEllipsisCharacter);
	fmt.SetFormatFlags(StringFormatFlagsLineLimit);
	Pen pen(g_globalInfo.viewSetting.clrSeparateLine, 1.0);
	Rect rRowBk(0, 0, rcRemainTimeClient.Width(), m_drawParam.nItemHeight);
	CRect rcItem(0,0, rRowBk.Width,rRowBk.Height);
	for (int i = 0; i < nCount; i++)
	{
		ARGB clr = 0xff000000;
		ARGB clrBk = 0xffffffff;
		vData[i].GetMonColor(clr, clrBk);
		bkBrush.SetColor(clrBk);
		gc.FillRectangle(&bkBrush, rRowBk);
		CString strText = vData[i].GetValue(MONTYPE_TIMEREMAIN);
		GPDrawShadowTextSimple(gc, strText, rcItem, *m_drawParam.pFont, clr, 0, 2,2, &fmt);
		//GPDrawShadowText(gc, strText, rcItem, *m_drawParam.pFont, clr, 0xff000000,0,0,0,0,&fmt);
		gc.DrawLine(&pen, rcItem.left, rcItem.bottom-1, rcItem.right, rcItem.bottom-1);
		nTopPos += m_drawParam.nItemHeight;
		rcItem.OffsetRect(0, m_drawParam.nItemHeight);
		rRowBk.Offset(0, m_drawParam.nItemHeight);
	}

	gc.EndContainer(container);

	Graphics gcDes(pDC->GetSafeHdc());
	CRect rcDesClip(rcRemainTimeClient);
	Rect rDesClip = CRect2Rect(rcDesClip);
	gcDes.SetClip(rDesClip);
	gcDes.DrawImage(m_pBmpRemainTime, rDesClip, ptOffSetBmp.x, ptOffSetBmp.y, (INT)rDesClip.Width, (INT)rDesClip.Height, UnitPixel);

}

void CClipMonView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDTIMER_REMAINTIME_DRAW)
	{
		if (m_nItemSize != 0)
		{
			CClientDC dc(this);
			ReDrawRemainTime(&dc);
		}
		return;
	}
	CTxListCtrl::OnTimer(nIDEvent);
}

void CClipMonView::OnDestroy()
{
	KillTimer(IDTIMER_REMAINTIME_DRAW);
	CTxListCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
