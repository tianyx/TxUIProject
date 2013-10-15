#include "StdAfx.h"
#include "MagicButton.h"
#include "skinfiles.h"
#define BTN_MSG_ID 2010
#define BTN_FACE_ID 2011

#define DECORATE_REDRAW_TIMER 1000

#define ID_TIMER_DECORATE_EFFECT_IN 2020
#define ID_TIMER_DECORATE_EFFECT_OUT 2021


using namespace Gdiplus;
CMagicButton::CMagicButton(void)
{
}

CMagicButton::~CMagicButton(void)
{
}
BEGIN_MESSAGE_MAP(CMagicButton, CMultiBitmapButtonEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CMagicButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	LoadBitmaps(DlgBtn_tmp, TRUE);
	CMultiBitmapButtonEx::PreSubclassWindow();

	ActiveEffect();
	//create msg btn
	CRect rcFrm;
	GetWindowRect(rcFrm);
	rcFrm.InflateRect(10,10,10,10);
	m_pFmDecorate = new CDecorateFrame;
	m_pFmDecorate->Create(NULL, NULL, WS_POPUP|WS_VISIBLE, rcFrm, this);
	ASSERT(m_pFmDecorate->GetSafeHwnd());	
}

void CMagicButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);
	if (!this->GetSafeHwnd())
	{
		return;
	}

	if (!m_pFmDecorate->GetSafeHwnd())
	{
		return;
	}
	
	Relayout();

	// TODO: 在此处添加消息处理程序代码
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDecorateFrame, CMiniFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CMagicButton::OnMove(int x, int y)
{
	CMultiBitmapButtonEx::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}

void CMagicButton::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CMultiBitmapButtonEx::OnWindowPosChanged(lpwndpos);

	// TODO: 在此处添加消息处理程序代码
}

LONG CMagicButton::RelayEvent(UINT message)
{
	if (!GetSafeHwnd())
	{
		return FALSE;
	}

	if (/*
		pMsg->message == WM_SIZE
				||pMsg->message == WM_SIZING
				||*/
		message == WM_MOVE
		||message == WM_WINDOWPOSCHANGING
		||message == WM_WINDOWPOSCHANGED)
	{
		Relayout();
	}

	return TRUE;
}

void CMagicButton::Relayout()
{
	CRect rcThis;
	GetWindowRect(rcThis);
	rcThis.InflateRect(20, 20, 20,20);
	m_pFmDecorate->MoveWindow(rcThis);
	m_pFmDecorate->SendMessage(WM_PAINT, 0,0);
}


//////////////////////////////////////////////////////////////////////////
int CDecorateFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyleEx(0,WS_EX_LAYERED);
	//BOOL bSuc =SetLayeredWindowAttributes( RGB(1,1,1), 0, LWA_COLORKEY);
	m_pDecBmp =Bitmap::FromFile(GetFullFilePath(MagicFormPic).AllocSysString());
	if (m_pDecBmp)
	{
		m_nBmpHeight =m_pDecBmp->GetHeight();
		m_nBmpWidth = m_pDecBmp->GetWidth();
	}
	
	// TODO:  在此添加您专用的创建代码
	//SetTimer(DECORATE_REDRAW_TIMER, 1000, NULL);
	return 0;
}

void CDecorateFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CMiniFrameWnd::OnPaint()

	CDC memDc;
	memDc.CreateCompatibleDC(&dc);

	CRect rcwindow;
	GetWindowRect(rcwindow);

	CBitmap bitmap;
	bitmap.CreateBitmap(rcwindow.Width(), rcwindow.Height(), 1, 32, NULL);
	HGDIOBJ oldBmp = memDc.SelectObject(bitmap);


	CRect rcThis;
	GetClientRect(rcThis);
	//dc.FillSolidRect(rcThis, RGB(255,0,255));
 	Graphics gc(memDc.GetSafeHdc());
//  	SolidBrush brush(Color::MakeARGB(0,0,0,0));
//  	gc.FillRectangle(&brush, rcThis.left,rcThis.top, rcThis.Width(),rcThis.Height());
//  	Pen pen(Color::MakeARGB(200, 255,0,255), 5);
// 	gc.DrawRectangle(&pen, rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height());
	if (m_pDecBmp)
	{
		Point pt(0,0);
		gc.TranslateTransform(rcwindow.Width()/2, rcwindow.Height()/2);
		gc.RotateTransform(36.0 *m_nEffectCounting);
		gc.DrawImage(m_pDecBmp, -rcwindow.Width()/2, -rcwindow.Height()/2, m_nBmpWidth, m_nBmpHeight);
	}

	BLENDFUNCTION bf;
	bf.BlendOp =AC_SRC_OVER;
	bf.BlendFlags =0;
	bf.SourceConstantAlpha =255;
	bf.AlphaFormat = AC_SRC_ALPHA;
	CRect rc2;
	GetWindowRect(rc2);
	POINT ptDes=rc2.TopLeft();
	SIZE  szDes={rc2.Width(), rc2.Height()};
	POINT ptSrc={0,0};

	BOOL bet =UpdateLayeredWindow(&dc, &ptDes, &szDes, 
		&memDc, &ptSrc, 0, &bf, ULW_ALPHA);

	memDc.SelectObject(oldBmp);
	bitmap.DeleteObject();
	memDc.DeleteDC();


}

void CDecorateFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (ID_TIMER_DECORATE_EFFECT_IN == nIDEvent)
	{
		m_nEffectCounting++;
		if (m_nEffectCounting > 10)
		{
			//KillTimer(ID_TIMER_DECORATE_EFFECT_IN);
			m_nEffectCounting = 1;
		}
		
		SendMessage(WM_PAINT, 0,0);

	}
	else if (ID_TIMER_DECORATE_EFFECT_OUT == nIDEvent)
	{
		m_nEffectCounting--;
		if (m_nEffectCounting < 0)
		{
			//KillTimer(ID_TIMER_DECORATE_EFFECT_OUT);
			m_nEffectCounting = 9;
		}
		SendMessage(WM_PAINT, 0,0);
	}
	CMiniFrameWnd::OnTimer(nIDEvent);
}

CDecorateFrame::CDecorateFrame()
{
	m_pDecBmp = NULL;
	m_nBmpHeight = 0;
	m_nBmpWidth = 0;
	m_nEffectCounting = 0;


}

CDecorateFrame::~CDecorateFrame()
{
	if (m_pDecBmp)
	{
		delete m_pDecBmp;
		m_pDecBmp = NULL;
	}
}

void CDecorateFrame::StartEffectTimer( BOOL bPlay /*= TRUE*/ )
{

}

void CMagicButton::OnDestroy()
{
	CMultiBitmapButtonEx::OnDestroy();
	
	// TODO: 在此处添加消息处理程序代码
}

BOOL CMagicButton::PlayEffect( int nEffectType, BOOL bPlay /*= TRUE*/ )
{
	if (!m_pFmDecorate || !m_pFmDecorate->GetSafeHwnd())
	{
		return FALSE;
	}
#define EFFECT_INTERVAL_IN 50
#define EFFECT_INTERVAL_OUT 50
	if (nEffectType == 0)
	{
		m_pFmDecorate->KillTimer(ID_TIMER_DECORATE_EFFECT_OUT);
		if (bPlay)
		{
			m_pFmDecorate->SetTimer(ID_TIMER_DECORATE_EFFECT_IN, EFFECT_INTERVAL_IN, NULL);
		}
		else
		{
			m_pFmDecorate->KillTimer(ID_TIMER_DECORATE_EFFECT_IN);
		}
	}
	else if (nEffectType == 1)
	{
		m_pFmDecorate->KillTimer(ID_TIMER_DECORATE_EFFECT_IN);
		if (bPlay)
		{
			m_pFmDecorate->SetTimer(ID_TIMER_DECORATE_EFFECT_OUT, EFFECT_INTERVAL_OUT, NULL);
		}
		else
		{
			m_pFmDecorate->KillTimer(ID_TIMER_DECORATE_EFFECT_OUT);
		}
	}

	return CMultiBitmapButtonEx::PlayEffect(nEffectType, bPlay);
}

