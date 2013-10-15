// DBProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DBProgressCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBProgressCtrl

CDBProgressCtrl::CDBProgressCtrl()
{
}

CDBProgressCtrl::~CDBProgressCtrl()
{
}


BEGIN_MESSAGE_MAP(CDBProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CDBProgressCtrl)
	ON_WM_PAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBProgressCtrl message handlers

void CDBProgressCtrl::OnPaint() 
{
	CWindowDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	ValidateRect(rcClient);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);

	int iLower, iUpper, iPos;
	GetRange(iLower, iUpper);
	iPos = GetPos();

	CMemDC memDC(&dc, rcWnd.Width(), rcWnd.Height());
	memDC.FillSolidRect(rcWnd, TRANS_PARENT_CLR);
	if (!m_imageBk.IsEmpty())
		m_imageBk.RenderTransparent(&memDC, rcWnd.TopLeft(), rcWnd.Size());
	else
	{
		memDC.FillSolidRect(rcWnd, RGB(255, 255, 255));
		memDC.Draw3dRect(rcWnd, RGB(128, 128, 128),
			RGB(255, 255, 255));			
	}
	//draw padding
	CRect rcClip = rcWnd;
	rcClip.right = (int)(rcWnd.left
		+ rcWnd.Width() * ((float)iPos/(iUpper - iLower)));
	memDC.IntersectClipRect(rcClip);

	if (!m_imagePadding.IsEmpty())
		m_imagePadding.RenderTransparent(&memDC, rcWnd.TopLeft(), rcWnd.Size());
	else
	{
		CRect rcPadding = rcWnd;
		rcPadding.DeflateRect(1, 1);
		DrawPadding(&memDC, rcPadding);
	}
	
	dc.TransparentBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), 
		&memDC, 0, 0, rcWnd.Width(), rcWnd.Height(), TRANS_PARENT_CLR);
}

void CDBProgressCtrl::DrawPadding(CDC *pDC, CRect rect)
{
	ASSERT(pDC != NULL);

	CRect rcFragment(rect);
	int iWidth = max(rect.Height() - 2, 3);
	rcFragment.right = rcFragment.left + iWidth;
	COLORREF clrFill;

	while (rcFragment.left < rect.right)
	{
		if (rcFragment.right - rect.left < rect.Width()*0.6)
			clrFill = RGB(0, 255, 64);
		else if (rcFragment.right - rect.left < rect.Width()*0.8)
			clrFill = RGB(255, 255, 128);
		else
			clrFill = RGB(255, 30, 30);
		
		pDC->FillSolidRect(rcFragment, clrFill);
		rcFragment.OffsetRect(iWidth + 1, 0);
	}
}

void CDBProgressCtrl::LoadImages(LPCTSTR lpszBmpBk, LPCTSTR lpszBmpPadding)
{
	if (lpszBmpBk != NULL)
	{
		m_imageBk.LoadBitmap(lpszBmpBk, 10, 20);
		ASSERT(lpszBmpPadding != NULL);
		m_imagePadding.LoadBitmap(lpszBmpPadding, 10, 20);
	}
}



void CDBProgressCtrl::PreSubclassWindow() 
{	
	CProgressCtrl::PreSubclassWindow();

	CRect rect;
	GetWindowRect(&rect);
	if (!m_imageBk.IsEmpty())
		SetWindowPos(NULL, 0, 0, rect.Width(), m_imageBk.Height(),
			SWP_NOZORDER | SWP_NOMOVE);

	//促使窗口调用OnNcCalcSize函数
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER
		| SWP_NOMOVE | SWP_NOSIZE);
}

void CDBProgressCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{	
	//CProgressCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}
