// DBSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DBSliderCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBSliderCtrl

CDBSliderCtrl::CDBSliderCtrl()
{
	Initialize();
}

CDBSliderCtrl::~CDBSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CDBSliderCtrl, CWnd)
	//{{AFX_MSG_MAP(CDBSliderCtrl)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ENABLE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDBSliderCtrl message handlers

void CDBSliderCtrl::SetRange(int nMin, int nMax, BOOL bRedraw)
{
	ASSERT(nMin != nMax);

	m_iMin = nMin;
	m_iMax = nMax;

	if (bRedraw)
		Invalidate();
}

void CDBSliderCtrl::GetRange(int &nMin, int &nMax) const
{
	nMin = m_iMin;
	nMax = m_iMax;
}

void CDBSliderCtrl::SetPos(int nPos)
{
	m_iPos = nPos;
	Invalidate();
}

int CDBSliderCtrl::GetPos() const
{
	return m_iPos;	
}

void CDBSliderCtrl::SetPageSize(int nPage)
{
	ASSERT(nPage != NULL);

	m_iPageSize = nPage;
}

int CDBSliderCtrl::GetPageSize() const
{
	return m_iPageSize;
}

void CDBSliderCtrl::SetLineSize(int nSize)
{
	ASSERT(nSize != 0);

	m_iLineSize = nSize;
}

int CDBSliderCtrl::GetLineSize() const
{
	return m_iLineSize;
}

void CDBSliderCtrl::GetThumbRect(LPRECT lprc) const
{
	ASSERT(lprc != NULL);
	ASSERT(::IsWindow(m_hWnd));

	CPoint ptCenter; //thumb中心点
	CRect rcChannel;
	GetChannelRect(&rcChannel);
	int iMin, iMax;
	GetRange(iMin, iMax);
	int iPos = GetPos();

	if (m_bHorz)
	{
		ptCenter.y = (rcChannel.top + rcChannel.bottom)/2;
		ptCenter.x = (int)(rcChannel.left + m_szThumb.cx/2
					 + (rcChannel.Width() - m_szThumb.cx)
					 * iPos/(float)(iMax - iMin));
		
		//校验Thumb位置在左右边界线内
		if (ptCenter.x < rcChannel.left + m_szThumb.cx/2)
			ptCenter.x = rcChannel.left + m_szThumb.cx/2;
		if (ptCenter.x > rcChannel.right - m_szThumb.cx/2)
			ptCenter.x = rcChannel.right - m_szThumb.cx/2;
	}
	else
	{
		ptCenter.x = (rcChannel.left + rcChannel.right)/2;
		ptCenter.y = (int)(rcChannel.top + m_szThumb.cy/2
					 + (rcChannel.Height() - m_szThumb.cy)
					 * iPos/(float)(iMax - iMin));

		//校验Thumb位置在上下边界线内
		if (ptCenter.y < rcChannel.top + m_szThumb.cy/2)
			ptCenter.y = rcChannel.top + m_szThumb.cy/2;
		if (ptCenter.y > rcChannel.bottom - m_szThumb.cy/2)
			ptCenter.y = rcChannel.bottom - m_szThumb.cy/2;	
	}

	lprc->left		= ptCenter.x - m_szThumb.cx/2;
	lprc->top		= ptCenter.y - m_szThumb.cy/2;
	lprc->right		= lprc->left + m_szThumb.cx;
	lprc->bottom	= lprc->top  + m_szThumb.cy;

}

void CDBSliderCtrl::GetChannelRect(LPRECT lprc) const
{
	ASSERT(lprc != NULL);
	ASSERT(::IsWindow(m_hWnd));
	
	CRect rcClient;
	GetClientRect(&rcClient);
	
	if (m_bHorz)
	{
		int iHeightChannel = m_imageChannelLeft.Height();
		
		lprc->left		= rcClient.left;
		lprc->right		= rcClient.right;
		lprc->top		= rcClient.top + (rcClient.Height() - iHeightChannel)/2;
		lprc->bottom	= lprc->top + iHeightChannel;
	}
	else
	{
		int iWidthChannel = m_imageChannelLeft.Width();
		
		lprc->top 		= rcClient.top;
		lprc->bottom	= rcClient.bottom;
		lprc->left		= rcClient.left + (rcClient.Width() - iWidthChannel)/2;
		lprc->right		= rcClient.left + iWidthChannel;
	}
}

int CDBSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Initialize();

	return 0;
}

void CDBSliderCtrl::Initialize()
{
	m_iMin = 0;
	m_iMax = 100;
	m_iPos = 0;
	m_iLineSize = 1;
	m_iPageSize = 10;
	m_szThumb.cx = m_szThumb.cy = 0;
	m_bPushed = FALSE;
	m_clrBkgnd = RGB(255, 255, 255);

	if (::IsWindow(m_hWnd) && !IsWindowEnabled())
		m_thumbState = thumbStateDisable;
	else
		m_thumbState = thumbStateNormal;
}

//Call this function to verify that the current position of the slider in a slider control is between the minimum and maximum values
void CDBSliderCtrl::VerifyPos()
{
	int iMax = max(m_iMin, m_iMax);
	int iMin = min(m_iMin, m_iMax);

	if (m_iPos > iMax)
		m_iPos = iMax;

	if (m_iPos < iMin)
		m_iPos = iMin;
}

BOOL CDBSliderCtrl::Create(LPCTSTR lpszBmpChannelLeft,
						   LPCTSTR lpszBmpChannelRight,
						   LPCTSTR lpszBmpThumbs,
						   const RECT &rect,
						   CWnd *pParentWnd,
						   UINT nID, BOOL bHorz)
{
	ASSERT(pParentWnd != NULL);

	if (!CWnd::Create(AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, 
		AfxGetApp()->LoadStandardCursor(IDC_ARROW), NULL, NULL),
		NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID))
	{
		ASSERT(0);
		return FALSE;
	}
	m_bHorz = bHorz;

	//装载包含Thumb的大位图
	CBitmap bitmap;
	bitmap.Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpThumbs), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	ASSERT(bitmap.GetSafeHandle() != NULL);

	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);
	m_szThumb.cx = bmpInfo.bmWidth / 4;
	m_szThumb.cy = bmpInfo.bmHeight;

	m_bmpsThumb[thumbStateNormal].LoadBitmap(0,lpszBmpThumbs, 2, m_szThumb.cx-1);
	m_bmpsThumb[thumbStateSel].LoadBitmap(1,lpszBmpThumbs,2, m_szThumb.cx-1);
	m_bmpsThumb[thumbStateHover].LoadBitmap(2,lpszBmpThumbs, 2, m_szThumb.cx-1);
	m_bmpsThumb[thumbStateDisable].LoadBitmap(3,lpszBmpThumbs, 2, m_szThumb.cx-1);
	
	
	//装载Channel位图
	m_imageChannelLeft.LoadBitmap(lpszBmpChannelLeft, 5, 10);
	m_imageChannelRight.LoadBitmap(lpszBmpChannelRight, 5, 10);

	return TRUE;
}

void CDBSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_thumbState == thumbStateDisable)
		return;

	CRect rcThumb, rcChannel;
	GetThumbRect(&rcThumb);
	GetChannelRect(&rcChannel);

	if (m_bPushed)	//用户处于拖动状态中
	{
		if (m_thumbState != thumbStateSel)
		{
			SetCapture();
			m_thumbState = thumbStateSel;
		}

		int iPos;
		int iMin, iMax;
		GetRange(iMin, iMax);
		if (m_bHorz)
		{
			iPos = (int)((point.x - rcChannel.left - rcThumb.Width()/2)
					/ (float)(rcChannel.Width() - rcThumb.Width())
					* (iMax - iMin) + iMin); 
		}
		else
		{
			iPos = (int)((point.y - rcChannel.top - rcThumb.Height()/2)
					/ (float)(rcChannel.Height() - rcThumb.Height())
					* (iMax - iMin) + iMin); 
		}
		SetPos(iPos);
		VerifyPos();

		//向父窗口发送WM_HSCROLL或WM_VSCROLL消息
		CWnd *pParent = GetParent();
		ASSERT(pParent != NULL);
		iPos = GetPos();
		if (m_bHorz)
		{
			pParent->SendMessage(WM_HSCROLL,
				MAKEWPARAM(SB_THUMBTRACK, iPos), (LPARAM)m_hWnd);
		}
		else
		{
			pParent->SendMessage(WM_VSCROLL,
				MAKEWPARAM(SB_THUMBTRACK, iPos), (LPARAM)m_hWnd);
		}
	}
	else
	{
		if (rcThumb.PtInRect(point) && m_thumbState != thumbStateHover)
		{
			SetCapture();
			m_thumbState = thumbStateHover;

			InvalidateRect(rcThumb, TRUE);
		}
		else if (!rcThumb.PtInRect(point) && m_thumbState == thumbStateHover)
		{
			if (GetCapture() == this)
				ReleaseCapture();
			
			m_thumbState = thumbStateNormal;
				
			InvalidateRect(rcThumb, TRUE);
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CDBSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_thumbState == thumbStateDisable)
		return;
	
	CRect rcThumb, rcChannel;
	GetThumbRect(&rcThumb);
	GetChannelRect(&rcChannel);

	if (rcThumb.PtInRect(point))
	{
		m_bPushed = TRUE;
		SetCapture();		
		m_thumbState = thumbStateSel;
		InvalidateRect(rcThumb, TRUE);

		m_ptDragPos = point - rcThumb.CenterPoint();
	}
	else if (rcChannel.PtInRect(point))
	{
		int iPos;
		int iMin, iMax;
		GetRange(iMin, iMax);

		if (m_bHorz)
		{
			iPos = (int)((point.x - rcChannel.left - rcThumb.Width()/2)
					/ (float)(rcChannel.Width() - rcThumb.Width())
					* (iMax - iMin) + iMin); 
		}
		else
		{
			iPos = (int)((point.y - rcChannel.top - rcThumb.Height()/2)
					/ (float)(rcChannel.Height() - rcThumb.Height())
					* (iMax - iMin) + iMin); 
		}
		
		SetPos(iPos);
		VerifyPos();

		m_bPushed = TRUE;
		SetCapture();		
		m_thumbState = thumbStateSel;
		InvalidateRect(rcThumb, TRUE);

		m_ptDragPos.x = m_ptDragPos.y = 0;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CDBSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_thumbState == thumbStateDisable)
		return;
	
	CRect rcThumb, rcChannel;
	GetThumbRect(&rcThumb);
	GetChannelRect(&rcChannel);

	if (m_bPushed)
	{
		m_bPushed = FALSE;
		
		if (rcThumb.PtInRect(point))
		{
			m_thumbState = thumbStateHover;
			InvalidateRect(rcThumb, TRUE);
		}
		else
		{
			if (GetCapture() == this)
				ReleaseCapture();
			
			m_thumbState = thumbStateNormal;
			InvalidateRect(rcThumb, TRUE);
		}


		//向父窗口发送WM_HSCROLL或WM_VSCROLL消息
		CWnd *pParent = GetParent();
		ASSERT(pParent != NULL);
		int iPos = GetPos();
		if (m_bHorz)
		{
			pParent->SendMessage(WM_HSCROLL,
				MAKEWPARAM(SB_THUMBPOSITION, iPos), (LPARAM)m_hWnd);

			pParent->SendMessage(WM_HSCROLL,
				MAKEWPARAM(SB_ENDSCROLL, iPos), (LPARAM)m_hWnd);
		}
		else
		{
			pParent->SendMessage(WM_VSCROLL,
				MAKEWPARAM(SB_THUMBPOSITION, iPos), (LPARAM)m_hWnd);

			pParent->SendMessage(WM_VSCROLL,
				MAKEWPARAM(SB_ENDSCROLL, iPos), (LPARAM)m_hWnd);
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CDBSliderCtrl::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	if (bEnable)
		m_thumbState = thumbStateNormal;
	else
		m_thumbState = thumbStateDisable;
}

void CDBSliderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient, rcThumb, rcChannel, rcClip;
	GetClientRect(&rcClient);
	GetThumbRect(&rcThumb);
	GetChannelRect(&rcChannel);

	//填充背景
	CMemDC memDC(&dc,rcClient.Width(), rcClient.Height());
	memDC.FillSolidRect(rcClient, TRANS_PARENT_CLR);
	

	//画channel

	if (m_bHorz) //channel左半部分
	{
		rcClip = rcChannel;
		rcClip.right = (rcThumb.left + rcThumb.right)/2;
		memDC.IntersectClipRect(rcClip);
		m_imageChannelLeft.RenderTransparent(&memDC, rcChannel.TopLeft(), rcChannel.Size());
		memDC.SelectClipRgn(NULL);
	}
	else //top channel上半部分
	{
		rcClip = rcChannel;
		rcClip.bottom = (rcThumb.top + rcThumb.bottom)/2;
		memDC.IntersectClipRect(rcClip);
		m_imageChannelLeft.RenderTransparent(&memDC, rcChannel.TopLeft(), rcChannel.Size());
		memDC.SelectClipRgn(NULL);
	}

	if (m_bHorz) //channel右半部分
	{
		rcClip = rcChannel;
		rcClip.left = (rcThumb.left + rcThumb.right)/2;
		memDC.IntersectClipRect(rcClip);
		m_imageChannelRight.RenderTransparent(&memDC, rcChannel.TopLeft(), rcChannel.Size());
		memDC.SelectClipRgn(NULL);
	}
	else //top channel下半部分
	{
		rcClip = rcChannel;
		rcClip.top = (rcThumb.top + rcThumb.bottom)/2;
		memDC.IntersectClipRect(rcClip);
		m_imageChannelRight.RenderTransparent(&memDC, rcChannel.TopLeft(), rcChannel.Size());
		memDC.SelectClipRgn(NULL);
	}

	//画thumb
/*
						memDC.DrawState(rcThumb.TopLeft(),
										rcThumb.Size(),
										&m_bmpsThumb[m_thumbState],
										DST_BITMAP | DSS_NORMAL);*/
					

	m_bmpsThumb[m_thumbState].RenderTransparent(&memDC, rcThumb.TopLeft(), rcThumb.Size());

	//copy memDC to dc
	dc.TransparentBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), 
		&memDC, 0, 0,  rcClient.Width(), rcClient.Height(), TRANS_PARENT_CLR);
}

BOOL CDBSliderCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreCreateWindow(cs);
}
/*

void CDBSliderCtrl::InvalidateRect( LPCRECT lpRect, BOOL bErase / *= TRUE* / )
{
	//force backgroud redraw;
	CWnd* pWnd = GetParent();
	if (pWnd && pWnd->GetSafeHwnd())
	{
		CRect rc(lpRect);
		ClientToScreen(rc);
		pWnd->ScreenToClient(rc);
		//exclude slider ,
		CRect rcTop(rc);
		int nHeight = 6;//(m_szThumb.cy - m_imageChannelLeft.Height()) /2;
		rcTop.bottom = rcTop.top + nHeight;
		pWnd->InvalidateRect(rcTop, TRUE);

		CRect rcLeft(rc);
		rcLeft.right = rcLeft.left + 5;
		pWnd->InvalidateRect(rcLeft, TRUE);


		CRect rcBom(rc);
		rcBom.top = rcBom.bottom - nHeight;
		pWnd->InvalidateRect(rcBom, TRUE);

		CRect rcRight(rc);
		rcRight.left = rcRight.right - 5;
		pWnd->InvalidateRect(rcRight, TRUE);


	}
	CWnd::InvalidateRect(lpRect, bErase);
}

void CDBSliderCtrl::Invalidate( BOOL bErase / *= TRUE* / )
{
	CWnd* pWnd = GetParent();
	if (pWnd && pWnd->GetSafeHwnd())
	{
		CRect rc;
		GetWindowRect(rc);
		pWnd->ScreenToClient(rc);
		CRect rcTop(rc);
		int nHeight = 6;//(m_szThumb.cy - m_imageChannelLeft.Height()) /2;
		rcTop.bottom = rcTop.top + nHeight;
		pWnd->InvalidateRect(rcTop, TRUE);

		CRect rcBom(rc);
		rcBom.top = rcBom.bottom - nHeight;
		pWnd->InvalidateRect(rcBom, TRUE);



		CRect rcLeft(rc);
		rcLeft.right = rcLeft.left + 5;
		pWnd->InvalidateRect(rcLeft, TRUE);

		CRect rcRight(rc);
		rcRight.left = rcRight.right - 5;
		pWnd->InvalidateRect(rcRight, TRUE);

	}
	CWnd::Invalidate(TRUE);
}*/
