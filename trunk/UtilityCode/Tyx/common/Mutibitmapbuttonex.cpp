// BitmapButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "MutiBitmapButtonEx.h"
#include "GdiPlusNewHeader.h"
#include "global.h"
#include "GDIDrawFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DB_NORMAL		 0
#define DB_HOVER		 1
#define DB_SELECTED		 2
#define DB_DISABLE		 3

#define DB_CHECK_TRUE	 4
#define DB_CHECK_FALSE	 5
#define DB_CHECK_DISABLE 6


#define ID_TIMER_EFFECT_IN 2010
#define ID_TIMER_EFFECT_OUT 2011

#define MAX_EFFECT_COUNTING 5


using namespace Gdiplus;
/////////////////////////////////////////////////////////////////////////////
//BitmapButtonEx

BOOL CMultiBitmapButtonEx::m_gbLoading = FALSE;
CMultiBitmapButtonEx::CMultiBitmapButtonEx()
{
	m_nCurImgIdx = 0;
	m_nImgCount = 0;
	m_bCheck = FALSE;
	m_bCheckBtn = FALSE;
	m_bHasToolTip = FALSE;

	m_bMouseOver = FALSE;
	m_bPushed = FALSE;
	m_clrText = RGB(0, 0, 0);
	m_bAutoStretch = TRUE;

	m_pNaborBtn = NULL;
	m_pNaborBtn2 = NULL;
	m_nNaborSetState = -1;
	m_bNaborCalledReDraw = FALSE;
	m_bEffect = FALSE;
	m_nEffectCounting = 0;
	m_bEffect2 = FALSE;  //default enable

	m_nEffectFrameWidth = 2;
	m_nEffectRound = 0;
	m_bDrawShadowText = FALSE;
	m_ntextStyle = DT_CENTER;
	m_bEdgeStretch = FALSE;
	m_pImgUserPhoto = NULL;
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_hRelayWnd = NULL;

/*
	
	LOGFONT logfont;
	if (::SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof (logfont), &logfont, 0))
	{
		VERIFY(m_font.CreateFontIndirect(&logfont));
	}
	else
	{
		memset(&logfont, 0, sizeof(logfont));
		logfont.lfHeight = -9;
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH;
		StrCpy(logfont.lfFaceName, TEXT("System"));

		VERIFY(m_font.CreateFontIndirect(&logfont));
	}*/
	LOGFONT ft;
	g_fontBase.GetLogFont(&ft);
	m_font.CreateFontIndirect(&ft);

	m_nTextOffSet = 0;
	m_bDrawWithText = FALSE;
	m_nFirstDivPos = 0;
	m_nSecDivPos = 0;
}

CMultiBitmapButtonEx::~CMultiBitmapButtonEx()
{
}


BEGIN_MESSAGE_MAP(CMultiBitmapButtonEx, CButton)
	//{{AFX_MSG_MAP(CMutiBitmapButtonEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP

	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
//	ON_CONTROL_REFLECT(BN_KILLFOCUS, &CMultiBitmapButtonEx::OnBnKillfocus)
ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMutiBitmapButtonEx message handlers

void CMultiBitmapButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (!lpDrawItemStruct->hDC)
	{
		//ASSERT(FALSE);
#ifdef _DEBUG
		TRACE(TEXT("\nerr image = %s"), strLoadedImg);
#endif // _DEBUG
		return;
	}
	if (m_pImgUserPhoto != NULL)
	{
		Gdiplus::Graphics gc(lpDrawItemStruct->hDC);
		CSize szImg;
		szImg.cx = m_pImgUserPhoto->GetWidth();
		szImg.cy = m_pImgUserPhoto->GetHeight();
		gc.DrawImage(m_pImgUserPhoto, 0,0, 0,0, szImg.cx, szImg.cy, Gdiplus::UnitPixel);
		return;
	}

	//modified by tianyx 2009-5-25--17:53 for combo btns with drop
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
    CRect rect (lpDrawItemStruct->rcItem);
	CRect rcNabour;
	if (m_pNaborBtn && m_pNaborBtn->GetSafeHwnd())
	{
		m_pNaborBtn->GetWindowRect(rcNabour);
		this->ScreenToClient(rcNabour);
	}


	if (m_bCheckBtn)
	{
		if (lpDrawItemStruct->itemState & ODS_DISABLED)
			DrawCheckButton(pDC, DB_CHECK_DISABLE);
		else
			DrawCheckButton(pDC, m_bCheck ? DB_CHECK_TRUE : DB_CHECK_FALSE);
	}
	else
	{
		if (m_pNaborBtn && m_bNaborCalledReDraw)
		{
			DrawWithNabourState(lpDrawItemStruct);
			m_bNaborCalledReDraw = FALSE;
		}
		else
		{
			int nNaborState = -1;
			if (lpDrawItemStruct->itemState & ODS_DISABLED)
			{
				DrawButton(pDC, DB_DISABLE);
				nNaborState = ODS_DISABLED;
			}
			else if (lpDrawItemStruct->itemState & ODS_SELECTED) 
			{
				DrawButton(pDC, DB_SELECTED);
				nNaborState = ODS_HOTLIGHT;
			}
			else if (m_bMouseOver || rect.PtInRect(pt))
			{
				DrawButton(pDC, DB_HOVER);//这时鼠标在按钮上，当鼠标离开如果有焦点才用"DrawButton(DB_FOCUS);
				nNaborState = ODS_HOTLIGHT;	
				//_RPT0(0, "\n self draw hotlight");
			}
 			else if (m_pNaborBtn && m_pNaborBtn->GetSafeHwnd() && rcNabour.PtInRect(pt))
			{
				m_nCurImgIdx++;
 				DrawButton(pDC, DB_HOVER);//这时鼠标在按钮上，当鼠标离开如果有焦点才用"DrawButton(DB_FOCUS);
 				m_nCurImgIdx--;
				nNaborState = ODS_HOTLIGHT;	
 				//_RPT0(0, "\n self draw hotlight");
 			}
			else if (lpDrawItemStruct->itemState & ODS_FOCUS)
			{
				DrawButton(pDC, DB_NORMAL);
				nNaborState = ODS_FOCUS;
			}
			else
			{
				DrawButton(pDC, DB_NORMAL);
				nNaborState = 0;
				//_RPT0(0, "\n self draw normal");
			}


			//
			if (m_pNaborBtn && m_pNaborBtn->IsWindowVisible() && !rcNabour.PtInRect(pt) )
			{

				TRACE3("\n selfdraw wnd =0x%x state= %x, idx = %d", this->GetSafeHwnd(), nNaborState, m_nCurImgIdx);
				m_pNaborBtn->m_nNaborSetState = nNaborState;
				TRACE2("\n nabor wnd =0x%x,state=%x", m_pNaborBtn->GetSafeHwnd(),  nNaborState);
				m_pNaborBtn->m_bNaborCalledReDraw = TRUE;
				//call nabour Draw
				m_pNaborBtn->Invalidate();
			}
			//
// 			if (m_pNaborBtn2 && m_pNaborBtn2->IsWindowVisible())
// 			{
// 				m_pNaborBtn2->m_nNaborSetState = nNaborState;
// 				m_pNaborBtn2->m_bNaborCalledReDraw = TRUE;
// 				//call nabour Draw
// 				m_pNaborBtn2->Invalidate();
// 			}
		}
	}
}

void CMultiBitmapButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (m_hRelayWnd)
	{
		::SendMessage(m_hRelayWnd, MSG_CHILD_RELAY_EVENT, WM_MOUSEMOVE, 0);
	}

	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point))
	{   
		if (m_bMouseOver == FALSE)
		{   
			m_bMouseOver = TRUE;
			Invalidate(FALSE);
			SetCapture();
			PlayEffect(0, TRUE);
		}
	}
	else
	{   
		if (m_bPushed == FALSE)
		{
			m_bMouseOver = FALSE;
			Invalidate(FALSE);
			ReleaseCapture();
			PlayEffect(1, TRUE);
		}
	}
	CButton::OnMouseMove(nFlags, point);
}

void CMultiBitmapButtonEx::DrawButton(CDC *pDC,UINT style)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemDC memDC(pDC,rcClient.Width(),rcClient.Height());//首先绘到内存DC,然后再绘到pDC,避免显砾
	CRect rect(rcClient);
    switch (style)
	{   
		case DB_DISABLE:
		{   
			m_vImageDisable[m_nCurImgIdx].Render(&memDC,rect.TopLeft(),rect.Size());
			break;
		}
		case DB_HOVER:
		{
			m_vImageHover[m_nCurImgIdx].Render(&memDC,rect.TopLeft(),rect.Size());
			break;
		}
		case DB_SELECTED:
		{	
			m_vImageSel[m_nCurImgIdx].Render(&memDC,rect.TopLeft(),rect.Size());
			rect.OffsetRect(1,1);
			break;
		}
		   
		default://正常
		{
			m_vImageNormal[m_nCurImgIdx].Render(&memDC,rect.TopLeft(),rect.Size());
		}
	}

	//水平居中绘制文本和图标
	//取得文本长度
	CString str,strTemp;
	GetWindowText(strTemp);
	int iFind = strTemp.Find(TEXT("\\%s"),0);

	str.Format(strTemp,TEXT("\r\n"));

	ASSERT (m_font.GetSafeHandle() != NULL);
    CFont *pOldFont = memDC.SelectObject(&m_font);
	int iStrLength = memDC.GetTextExtent(str).cx;
	
	//绘制图标
	if (GetStyle() & BS_ICON)
	{
		HICON hIcon = GetIcon();
		CPoint ptOrg(rect.left + (rect.Width()-iStrLength-32)/2,
			rect.top+(rect.Height()-32)/2);
		if (ptOrg.x < 0)
			ptOrg.x = 0;
		if (hIcon != NULL)
			DrawIconEx(memDC.m_hDC, ptOrg.x, ptOrg.y, hIcon, 0, 0, 0, NULL, DI_NORMAL);
		rect.left = ptOrg.x + 32;
	}
/*	else
	{
		rect.left = rect.left + (rect.Width()-iStrLength)/2;
		if (rect.left < 5)
			rect.left = 5;
	}
*/	//绘制文本
	memDC.SetBkMode(TRANSPARENT);
//	rect.right = rcClient.right - rect.left;
	rect.left += m_nTextOffSet;
	CRect centerRect = rect;
	
	if (style == DB_DISABLE)
	{
		COLORREF clrTextOld = memDC.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		
		if (m_bDrawShadowText)
		{
			GPDrawSunkenText(memDC.GetSafeHdc(), str, centerRect,  m_ntextStyle | DT_WORDBREAK, m_font, ::GetSysColor(COLOR_GRAYTEXT), ::GetSysColor(COLOR_GRAYTEXT));
		}
		else
		{
		
			memDC.DrawText(str, rect, DT_WORDBREAK | DT_CALCRECT | m_ntextStyle);
			rect.OffsetRect((centerRect.Width() - rect.Width()) / 2, 
				((centerRect.Height() - rect.Height()) / 2));
			if (m_ntextStyle == DT_LEFT)
			{
				rect.left = centerRect.left;
			}
			memDC.DrawText(str, &rect, m_ntextStyle | DT_WORDBREAK);


			rect.OffsetRect(1, 1);
			memDC.SetTextColor(RGB(253, 253, 255));
			if (rect.Width() > iStrLength)
			{
				memDC.DrawText(str, &rect, m_ntextStyle | DT_WORDBREAK);
			}
			else
			{
				memDC.DrawText(str, &rect, m_ntextStyle | DT_WORDBREAK | DT_END_ELLIPSIS);
			}

		}
		
		memDC.SetTextColor(clrTextOld);
	}
	else
	{
		COLORREF clrTextOld = memDC.SetTextColor(m_clrText);
		if (m_bDrawShadowText)
		{
			GPDrawSunkenText(memDC.GetSafeHdc(), str, centerRect,  m_ntextStyle | DT_WORDBREAK, m_font, m_clrText,m_clrText);
		}
		else
		{
			COLORREF clrTextOld = memDC.SetTextColor(m_clrText);
			memDC.DrawText(str, rect, DT_WORDBREAK | DT_CALCRECT | m_ntextStyle);
			rect.OffsetRect((centerRect.Width() - rect.Width()) / 2, 
			((centerRect.Height() - rect.Height()) / 2));
			if (m_ntextStyle == DT_LEFT)
			{
				rect.left = centerRect.left;
			}
			memDC.DrawText(str, &rect, m_ntextStyle | DT_WORDBREAK);
// 			if (rect.Width() > iStrLength)
// 			memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
// 			else
// 			memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);

			
		}
		memDC.SetTextColor(clrTextOld);
	}
	memDC.SelectObject(pOldFont);

	DrawEffect(&memDC, style);
	//DrawEffect2(&memDC, style);
	pDC->TransparentBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,rcClient.Width(),rcClient.Height(),TRANS_PARENT_CLR);

	if (rect.Width() < iStrLength)
		m_bHasToolTip = TRUE;
	else
		m_bHasToolTip = FALSE;
}

void CMultiBitmapButtonEx::PreSubclassWindow() 
{
	UINT nStyle = GetButtonStyle();
	//{{removed by tian_yx (14:1:2010) for not support check
	//if (nStyle & BS_CHECKBOX)
	//	m_bCheckBtn = TRUE;
	//end}}

	nStyle |= BS_OWNERDRAW;
	SetButtonStyle(nStyle, FALSE);
	
	//设置按钮高度和背景位图一致
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	if (m_pImgUserPhoto)
	{
		if (m_bAutoStretch)
		{
			SetWindowPos(NULL, 0, 0, rcWindow.Width(),
				m_pImgUserPhoto->GetHeight(),
				SWP_NOZORDER | SWP_NOMOVE);
		}
		else
		{
			SetWindowPos(NULL, 0, 0, m_pImgUserPhoto->GetWidth(),
				m_pImgUserPhoto->GetHeight(),
				SWP_NOZORDER | SWP_NOMOVE);
		}
	}
	else
	{
		if (m_bAutoStretch)
		{
			SetWindowPos(NULL, 0, 0, rcWindow.Width(),
				m_vImageNormal[0].Height(),
				SWP_NOZORDER | SWP_NOMOVE);
		}
		else
		{
			SetWindowPos(NULL, 0, 0, m_vImageNormal[0].Width(),
				m_vImageNormal[0].Height(),
				SWP_NOZORDER | SWP_NOMOVE);
		}
	}
	


	//{modified by tian_yx 2009-04-15 fix bug that show tips twice on btn;
	EnableToolTips(FALSE);
	//}end by tian_yx 2009-04-15

	CButton::PreSubclassWindow();	
}


void CMultiBitmapButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!::IsWindow(m_hWnd))
		return;

	if (m_hRelayWnd)
	{
		::SendMessage(m_hRelayWnd, MSG_CHILD_RELAY_EVENT, WM_MOUSEMOVE, 0);
	}

	CRect rect;
	GetClientRect(&rect);
	if (m_bCheckBtn && rect.PtInRect(point))
	{
		m_bCheck = !m_bCheck;
		Invalidate(FALSE);
	}

	CButton::OnLButtonUp(nFlags, point);

	if (rect.PtInRect(point) && IsWindowVisible())
	{   
		if (GetCapture() != this && this->IsWindowVisible())
			SetCapture();
		m_bPushed = FALSE;
	}
	else
	{
		ReleaseCapture();
		m_nEffectCounting = 0;
	}
}

void CMultiBitmapButtonEx::LoadBitmaps(LPCTSTR lpszBmpNormal, 
	LPCTSTR lpszBmpSel,
	LPCTSTR lpszBmpHover,
	LPCTSTR lpszBmpDisable,
	BOOL bAutoStretch)
{
	m_bAutoStretch = bAutoStretch;

	//装载背景位图
	m_vImageNormal[0].LoadBitmap(lpszBmpNormal, 8, 12);
	m_vImageSel[0].LoadBitmap(lpszBmpSel, 8, 12);
	m_vImageHover[0].LoadBitmap(lpszBmpHover, 8, 12);
	m_vImageDisable[0].LoadBitmap(lpszBmpDisable, 8, 12);
	m_nImgCount = 1;
}

void CMultiBitmapButtonEx::LoadBitmaps(LPCTSTR lpszBitmaps,
	BOOL bAutoStretch, BOOL bEdgeStretch /*= FALSE*/)
{
	if (m_nImgCount ==1)
	{
		return;
	}
	
	m_bEdgeStretch = bEdgeStretch;

	ASSERT(lpszBitmaps != NULL);
	m_bAutoStretch = bAutoStretch;

	//装载位图
	CBitmap bitmap;
#ifdef _DEBUG
	strLoadedImg = IM_MACRO_GET_OCX_FILE_PATH(lpszBitmaps);
#endif // _DEBUG
	HGDIOBJ hbmp = ::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBitmaps), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbmp == NULL)
	{
		CString strPath;
		strPath = IM_MACRO_GET_OCX_FILE_PATH(lpszBitmaps);
		AfxMessageBox(strPath);
	}
	bitmap.Attach(hbmp);
	ASSERT(bitmap.GetSafeHandle() != NULL);
	
	CSize szButton;
	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);
	szButton.cx = bmpInfo.bmWidth / 4;
	szButton.cy = bmpInfo.bmHeight;
	bitmap.DeleteObject();

	//提取按钮位图
// 	CDC *pDC = (CWnd::FromHandle(::GetDesktopWindow()))->GetDC();

// 	CBitmap bmpNormal, bmpSel, bmpHover, bmpDisable;
// 	bmpNormal.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
// 	bmpSel.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
// 	bmpHover.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
// 	bmpDisable.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
// 	
// 	CDC dcSrc;
// 	CDC dcDes;
// 	dcSrc.CreateCompatibleDC(pDC);
// 	dcDes.CreateCompatibleDC(pDC);
// 	
// 	//提取 bmpNormal
// 	CBitmap *pOldBmpSrc = dcSrc.SelectObject(&bitmap);
// 	CBitmap *pOldBmpDes = dcDes.SelectObject(&bmpNormal);
// 	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
// 		0, 0, SRCCOPY);


	int nExtLeft = bmpInfo.bmWidth / 8 -5;
	int nExtRight = bmpInfo.bmWidth / 8 +5;
	if (nExtLeft < 0)
	{
		nExtLeft = 2;
	}
	if (nExtRight >= bmpInfo.bmWidth / 8)
	{
		nExtRight = bmpInfo.bmWidth /8 -1;
	}

	if (bEdgeStretch)
	{
		nExtLeft = 2;
		nExtRight = bmpInfo.bmWidth / 4 -2;
	}

	if (m_nSecDivPos == bmpInfo.bmWidth /4)
	{
		m_nSecDivPos = bmpInfo.bmWidth/4 -1;
	}

	if (m_nFirstDivPos == m_nSecDivPos)
	{
		m_nFirstDivPos = nExtLeft;
		m_nSecDivPos = nExtRight;
	}

	m_vImageNormal[0].LoadBitmap(0, lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, TRUE, bEdgeStretch);
	m_vImageSel[0].LoadBitmap(2, lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, TRUE,bEdgeStretch);
	m_vImageHover[0].LoadBitmap(1, lpszBitmaps,m_nFirstDivPos, m_nSecDivPos, TRUE,bEdgeStretch);
	m_vImageDisable[0].LoadBitmap(3, lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, TRUE,bEdgeStretch);
	m_nImgCount++;

	

// 	//提取 bmpHover
// 	dcDes.SelectObject(&bmpHover);
// 	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
// 		szButton.cx * 1, 0, SRCCOPY);
// 
// 	//提取 bmpSel
// 	dcDes.SelectObject(&bmpSel);
// 	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
// 		szButton.cx * 2, 0, SRCCOPY);
// 
// 
// 	//提取 bmpDisable
// 	dcDes.SelectObject(&bmpDisable);
// 	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
// 		szButton.cx * 3, 0, SRCCOPY);
// 
// 	dcSrc.SelectObject(pOldBmpSrc);
// 	dcSrc.DeleteDC();
// 	dcDes.SelectObject(pOldBmpDes);
// 	dcDes.DeleteDC();
// 	::ReleaseDC(::GetDesktopWindow(), pDC->m_hDC);
// 
// 
// 	bmpNormal.DeleteObject();
// 	bmpSel.DeleteObject();
// 	bmpHover.DeleteObject();
// 	bmpDisable.DeleteObject();
}

void CMultiBitmapButtonEx::LoadBitmaps( CBitmap& bitmap, BOOL bAutoStretch /*= TRUE*/ )
{
	if (m_nImgCount ==1)
	{
		return;
	}

	m_bAutoStretch = bAutoStretch;

	//装载位图
	ASSERT(bitmap.GetSafeHandle() != NULL);

	CSize szButton;
	BITMAP bmpInfo;
	bitmap.GetBitmap(&bmpInfo);
	szButton.cx = bmpInfo.bmWidth / 4;
	szButton.cy = bmpInfo.bmHeight;

	//提取按钮位图
	CDC *pDC = (CWnd::FromHandle(::GetDesktopWindow()))->GetDC();

	CBitmap bmpNormal, bmpSel, bmpHover, bmpDisable;
	bmpNormal.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	bmpSel.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	bmpHover.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);
	bmpDisable.CreateCompatibleBitmap(pDC, szButton.cx, szButton.cy);

	CDC dcSrc;
	CDC dcDes;
	dcSrc.CreateCompatibleDC(pDC);
	dcDes.CreateCompatibleDC(pDC);

	//提取 bmpNormal
	CBitmap *pOldBmpSrc = dcSrc.SelectObject(&bitmap);
	CBitmap *pOldBmpDes = dcDes.SelectObject(&bmpNormal);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		0, 0, SRCCOPY);

	//提取 bmpHover
	dcDes.SelectObject(&bmpHover);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 1, 0, SRCCOPY);

	//提取 bmpSel
	dcDes.SelectObject(&bmpSel);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 2, 0, SRCCOPY);



	//提取 bmpDisable
	dcDes.SelectObject(&bmpDisable);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 3, 0, SRCCOPY);

	dcSrc.SelectObject(pOldBmpSrc);
	dcSrc.DeleteDC();
	dcDes.SelectObject(pOldBmpDes);
	dcDes.DeleteDC();
	::ReleaseDC(::GetDesktopWindow(), pDC->m_hDC);

	int nExtLeft = bmpInfo.bmWidth / 8 -5;
	int nExtRight = bmpInfo.bmWidth / 8 +5;
	m_vImageNormal[0].LoadBitmap((HBITMAP)bmpNormal.GetSafeHandle(), nExtLeft, nExtRight);
	m_vImageHover[0].LoadBitmap((HBITMAP)bmpSel.GetSafeHandle(), nExtLeft, nExtRight);
	m_vImageSel[0].LoadBitmap((HBITMAP)bmpHover.GetSafeHandle(), nExtLeft, nExtRight);
	m_vImageDisable[0].LoadBitmap((HBITMAP)bmpDisable.GetSafeHandle(), nExtLeft, nExtRight);
	m_nImgCount++;
	bmpNormal.DeleteObject();
	bmpSel.DeleteObject();
	bmpHover.DeleteObject();
	bmpDisable.DeleteObject();
}

void CMultiBitmapButtonEx::LoadBitmaps( Gdiplus::Image* pImagIn )
{
	m_pImgUserPhoto = pImagIn;
	m_bAutoStretch = FALSE;
}

void CMultiBitmapButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bPushed = TRUE;
	SetFocus();
	CButton::OnLButtonDown(nFlags, point);
}

COLORREF CMultiBitmapButtonEx::SetTextColor(COLORREF clrText)
{
	COLORREF clrOld = m_clrText;
	m_clrText = clrText;
	
	return clrOld;
}


LRESULT CMultiBitmapButtonEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == BM_SETSTYLE)
	{//总是添加自画属性
		wParam |= BS_OWNERDRAW;
	}

	if (message == BM_GETCHECK && m_bCheckBtn)
	{
		return (m_bCheck ? BST_CHECKED : BST_UNCHECKED);
	}
	if (message == BM_SETCHECK && m_bCheckBtn)
	{
		switch (wParam)
		{
		case BST_CHECKED:
			m_bCheck = TRUE;
			break;
		case BST_UNCHECKED:
			m_bCheck = FALSE;
			break;
		default:
			break;
		}
		Invalidate(FALSE);
		
		return 0;
	}
	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	}

	return CButton::WindowProc(message, wParam, lParam);
}

void CMultiBitmapButtonEx::OnCaptureChanged(CWnd *pWnd) 
{
	if (pWnd != this)
	{
		m_bPushed = FALSE;
		m_bMouseOver = FALSE;
		Invalidate(FALSE);
	}

	CButton::OnCaptureChanged(pWnd);
}

void CMultiBitmapButtonEx::DrawCheckButton(CDC *pDC, UINT style)
{
	ASSERT(pDC != NULL);
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemDC memDC(pDC,rcClient.Width(),rcClient.Height());//首先绘到内存DC,然后再绘到pDC,避免显砾
	CRect rcImage = rcClient;
	rcImage.right = rcImage.left + m_vImageNormal[0].Width();
    switch (style)
	{   
	case DB_CHECK_DISABLE:
		m_vImageDisable[0].Render(&memDC, rcImage.TopLeft(), rcImage.Size());
		break;
	case DB_CHECK_TRUE:
		m_vImageSel[0].Render(&memDC, rcImage.TopLeft(), rcImage.Size());
		break;
	case DB_CHECK_FALSE:
		m_vImageNormal[0].Render(&memDC, rcImage.TopLeft(), rcImage.Size());
		break;
	default:
		ASSERT(0);
		break;
	}

	//绘制文本
	CRect rcText = rcClient;
#ifdef TEST_ON_DEBUG
	rcText.left += 8;
#else
	rcText.left += rcImage.Width() + 8;
#endif
	CString str;
	GetWindowText(str);
	ASSERT (m_font.GetSafeHandle() != NULL);
    CFont *pOldFont = memDC.SelectObject(&m_font);	
	memDC.SetBkMode(TRANSPARENT);
	if (style == DB_CHECK_DISABLE)
	{
		COLORREF clrTextOld = memDC.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		CRect centerRect = rcText;
		memDC.DrawText(str, rcText, DT_WORDBREAK | DT_CALCRECT | DT_CENTER);
		rcText.OffsetRect((centerRect.Width() - rcText.Width()) / 2, 
			((centerRect.Height() - rcText.Height()) / 2));
		memDC.DrawText(str, &rcText, DT_CENTER | DT_WORDBREAK);
		
		rcText.OffsetRect(1, 1);
		memDC.SetTextColor(RGB(253, 253, 255));
		memDC.DrawText(str, &rcText, DT_CENTER | DT_WORDBREAK);
		memDC.SetTextColor(clrTextOld);
	}
	else
	{
		COLORREF clrTextOld = memDC.SetTextColor(m_clrText);
		CRect centerRect = rcText;
		memDC.DrawText(str, rcText, DT_WORDBREAK | DT_CALCRECT | DT_CENTER);
		rcText.OffsetRect((centerRect.Width() - rcText.Width()) / 2, 
			((centerRect.Height() - rcText.Height()) / 2));
		memDC.DrawText(str, &rcText, DT_VCENTER | DT_SINGLELINE);
		memDC.SetTextColor(clrTextOld);
	}
	memDC.SelectObject(pOldFont);

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), 
		&memDC, 0, 0, SRCCOPY);
}

int CMultiBitmapButtonEx::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && m_bHasToolTip)
	{
		
		CString strText;
		GetWindowText(strText);
		strText.Replace(TEXT("\\%s"),TEXT(" "));
		LPTSTR lpszText = (LPTSTR)malloc(sizeof (TCHAR) * (strText.GetLength() + 1));
		StrCpy(lpszText, strText);

		pTI->lpszText = lpszText;
		pTI->hwnd = m_hWnd;
		pTI->rect = rect;
		pTI->uId = 0;
		pTI->cbSize = sizeof (TOOLINFO);
		pTI->hinst = NULL;
		pTI->lParam = 0;
		pTI->uFlags = TTF_SUBCLASS;
		
		return 1;
	}

	return -1;
}

void CMultiBitmapButtonEx::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CButton::OnShowWindow(bShow, nStatus);

	if (!bShow && nStatus == 0)
	{
		m_bPushed = FALSE;
		m_bMouseOver = FALSE;
		m_nEffectCounting = 0;
	}

	if (!bShow && GetCapture() == this && nStatus == 0)
		ReleaseCapture();
}

void CMultiBitmapButtonEx::DrawWithNabourState(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_nImgCount < 2)
	{
		ASSERT(FALSE);
		return;
	}

	m_nCurImgIdx++;
	TRACE2("\n nabour called wnd =0x%x idx = %d", this->GetSafeHwnd(), m_nCurImgIdx);
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
    CRect rect (lpDrawItemStruct->rcItem);

	if (m_nNaborSetState & ODS_DISABLED)
		DrawButton(pDC, DB_DISABLE);
	else if (m_nNaborSetState & ODS_SELECTED) 
		DrawButton(pDC, DB_SELECTED);
	else if ((m_nNaborSetState & ODS_HOTLIGHT) || m_bMouseOver || rect.PtInRect(pt))
	{
		DrawButton(pDC, DB_HOVER); 
		//_RPT0(0, "\n nabour draw hotlight");
	}//这时鼠标在按钮上，当鼠标离开如果有焦点才用"DrawButton(DB_FOCUS);
	else if (m_nNaborSetState & ODS_FOCUS)
		DrawButton(pDC, DB_NORMAL);
	else 
	{
			//_RPT0(0, "\n nabour draw normal");
		DrawButton(pDC, DB_NORMAL);
		
	}
	m_nCurImgIdx--;
}

int CMultiBitmapButtonEx::AddBitmaps( LPCTSTR lpszBitmaps )
{
	if (m_nImgCount <1 || m_nImgCount > 4)
	{
		//exceed, return;
		return -1;
	}
	int nImgPos = m_nImgCount; 
	ASSERT(lpszBitmaps != NULL);

	m_vImageNormal[nImgPos].LoadBitmap(0, lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, TRUE, m_bEdgeStretch);
	m_vImageSel[nImgPos].LoadBitmap(2, lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, TRUE, m_bEdgeStretch);
	m_vImageHover[nImgPos].LoadBitmap(1, lpszBitmaps,m_nFirstDivPos, m_nSecDivPos, TRUE, m_bEdgeStretch);
	m_vImageDisable[nImgPos].LoadBitmap(3, lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, TRUE, m_bEdgeStretch);
	m_nImgCount++;


	return m_nImgCount;

}

BOOL CMultiBitmapButtonEx::SetCurrentBitmapIdx( int nIdx )
{
	BOOL bRet = FALSE;
	if (nIdx == m_nCurImgIdx)
	{
		return TRUE;
	}

	if (nIdx < 5 && nIdx > -1)
	{
		m_nCurImgIdx = nIdx;
		bRet = TRUE;
		Invalidate();
	}
	return bRet;
}

int CMultiBitmapButtonEx::GetCurrentBitmapIdx()
{
	return m_nCurImgIdx;
}

BOOL CMultiBitmapButtonEx::ActiveEffect( BOOL bActive /*= TRUE*/ )
{
	m_bEffect = bActive;
	return TRUE;
}
void CMultiBitmapButtonEx::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (ID_TIMER_EFFECT_IN == nIDEvent)
	{
		m_nEffectCounting += MAX_EFFECT_COUNTING;
		if (m_nEffectCounting > MAX_EFFECT_COUNTING -1)
		{
			KillTimer(ID_TIMER_EFFECT_IN);
			m_nEffectCounting = MAX_EFFECT_COUNTING;
		}
		Invalidate(FALSE);

	}
	else if (ID_TIMER_EFFECT_OUT == nIDEvent)
	{
		m_nEffectCounting--;
		if (m_nEffectCounting < 1)
		{
			KillTimer(ID_TIMER_EFFECT_OUT);
			m_nEffectCounting = 0;
		}
		Invalidate(FALSE);
	}

	CButton::OnTimer(nIDEvent);
}

BOOL CMultiBitmapButtonEx::PlayEffect(int nEffectType, BOOL bPlay /*= TRUE*/ )
{
#define EFFECT_INTERVAL_IN 20
#define EFFECT_INTERVAL_OUT 100

	if (!m_bEffect && !m_bEffect2)
	{
		return FALSE;
	}

	if (nEffectType == 0)
	{
		KillTimer(ID_TIMER_EFFECT_OUT);
		if (bPlay)
		{
			SetTimer(ID_TIMER_EFFECT_IN, EFFECT_INTERVAL_IN, NULL);
		}
		else
		{
			KillTimer(ID_TIMER_EFFECT_IN);
		}
	}
	else if (nEffectType == 1)
	{
		KillTimer(ID_TIMER_EFFECT_IN);
		if (bPlay)
		{
			SetTimer(ID_TIMER_EFFECT_OUT, EFFECT_INTERVAL_OUT, NULL);
		}
		else
		{
			KillTimer(ID_TIMER_EFFECT_OUT);
		}
	}

	return TRUE;
}

void CMultiBitmapButtonEx::LoadSingleBitmaps( LPCTSTR lpszBitmaps, BOOL bAutoStretch /*= TRUE*/ )
{
	m_bAutoStretch = bAutoStretch;
	//装载背景位图
	m_vImageNormal[0].LoadBitmap(lpszBitmaps, 2,4);
	m_vImageHover[0].LoadBitmap(lpszBitmaps, 2,4);
	m_vImageSel[0].LoadBitmap(lpszBitmaps, 2,4);
	m_vImageDisable[0].LoadBitmap(lpszBitmaps, 2,4);
	m_nImgCount = 1;
}

int CMultiBitmapButtonEx::AddSingleBitmaps( LPCTSTR lpszBitmaps )
{
	if (m_nImgCount <1 || m_nImgCount > 4)
	{
		//exceed, return;
		return -1;
	}
	int nImgPos = m_nImgCount; 
	ASSERT(lpszBitmaps != NULL);
	m_vImageNormal[nImgPos].LoadBitmap(lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, m_bEdgeStretch);
	m_vImageHover[nImgPos].LoadBitmap(lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, m_bEdgeStretch);
	m_vImageSel[nImgPos].LoadBitmap(lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, m_bEdgeStretch);
	m_vImageDisable[nImgPos].LoadBitmap(lpszBitmaps, m_nFirstDivPos, m_nSecDivPos, m_bEdgeStretch);
	m_nImgCount ++;
	return 1;
}

void CMultiBitmapButtonEx::DrawEffect( CDC* pDC ,UINT style)
{
	//draw effect
	if (m_bEffect)
	{	
		CRect rcClient;
		GetClientRect(rcClient);
		CRgn rgn, rgn2, rgn3;
		rgn.CreateRoundRectRgn(0,0, rcClient.Width(), rcClient.Height(), m_nEffectRound,m_nEffectRound);
		rcClient.DeflateRect(m_nEffectFrameWidth,m_nEffectFrameWidth,m_nEffectFrameWidth,m_nEffectFrameWidth);
		rgn2.CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom,m_nEffectRound,m_nEffectRound);
		rgn3.CreateRectRgn(0,0,10,10);
		rgn3.CombineRgn(&rgn, &rgn2, RGN_DIFF);

		Graphics gc(pDC->GetSafeHdc());
		//Pen pen(Color::MakeARGB(50* m_nEffectCounting, 255,255,255), 2);
		//gc.DrawRectangle(&pen, 0,0, rcClient.right, rcClient.bottom);
		Region* pRgn = Region::FromHRGN(rgn3);
		SolidBrush brush(Color::MakeARGB(50* m_nEffectCounting, 255,255,255));
		gc.FillRegion(&brush, pRgn);		
	}
}
BOOL CMultiBitmapButtonEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

/*
void CMultiBitmapButtonEx::DrawEffect2( CDC* pDC, UINT style )
{
	if (m_bEffect2 && m_nEffectCounting != 0 && g_pImageEffect2->GetLastStatus() == Gdiplus::Ok)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		CRect rcImg(0,0, g_pImageEffect2->GetWidth(), g_pImageEffect2->GetHeight());
		CRect rcDraw = GetCenterRect(rcClient, rcImg);

		Bitmap* pTransBmp = NULL;	//need delete
		Image* pBmpStrech  = NULL;   //need delete
		Bitmap* pNewTransBmp = NULL;   //need delete
		Image* pDrawImg = g_pImageEffect2;  //not delete only point
		Bitmap* pPreTransBmp = g_pImageEffect2; //not delete only point  //if not change size, use g_pImageEffect2  

		if ((rcClient.Width() - DEFAULT_EFFECT2_WIDTH ) > 5|| rcClient.Width() < DEFAULT_EFFECT2_WIDTH)
		{
			rcDraw = rcClient;
			pBmpStrech = pDrawImg->GetThumbnailImage(rcDraw.Width(), rcDraw.Height());
			pDrawImg = pBmpStrech;
			if (m_nEffectCounting != MAX_EFFECT_COUNTING)
			{	
				pNewTransBmp = new Bitmap(rcDraw.Width(),rcDraw.Height(),PixelFormat32bppARGB);//(rcClient.Width(), rcClient.Height(), PixelFormat32bppARGB);
				Graphics gcNew(pNewTransBmp);
				gcNew.DrawImage(pBmpStrech, 0,0, rcDraw.Width(), rcDraw.Height());
				pPreTransBmp = pNewTransBmp;
			}
		}


		if (m_nEffectCounting != MAX_EFFECT_COUNTING)
		{
			pTransBmp = pPreTransBmp->Clone(0,0, rcDraw.Width(), rcDraw.Height(), PixelFormat32bppARGB);
			BitmapData bitmapData;
			Rect rect(0, 0, rcDraw.Width(), rcDraw.Height());

			pTransBmp->LockBits(
				&rect,
				ImageLockModeWrite,
				PixelFormat32bppARGB,
				&bitmapData);

			UINT* pixels = NULL;
			// Write to the temporary buffer provided by LockBits.
			pixels = (UINT*)bitmapData.Scan0;
			int nWidth = bitmapData.Stride / 4;
			int nPos = 0;
			for(UINT row = 0; row < bitmapData.Height; ++row)
			{
				for(UINT col = 0; col < bitmapData.Width; ++col)
				{
					nPos = row * nWidth + col;
					Color oldClr =(pixels[nPos]);
					Color newClr(oldClr.GetA()/MAX_EFFECT_COUNTING * m_nEffectCounting, oldClr.GetR(), oldClr.GetG(),oldClr.GetB());
					pixels[nPos] = newClr.GetValue();
				}
			}
			// Commit the changes, and unlock the 50x30 portion of the bitmap.  
			pTransBmp->UnlockBits(&bitmapData);
			pDrawImg = pTransBmp;
		}


		
		Graphics gc(pDC->GetSafeHdc());
		gc.DrawImage(pDrawImg, rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height());

		if (pTransBmp)
		{
			delete pTransBmp;
		}
		if (pBmpStrech)
		{
			delete pBmpStrech;
		}
		if (pNewTransBmp)
		{
			delete pNewTransBmp;
		}

	}

}

*/
BOOL CMultiBitmapButtonEx::ActiveEffect2( BOOL bActive /*= TRUE*/ )
{
	m_bEffect2 = bActive;
	Invalidate();
	return TRUE;
}

int CMultiBitmapButtonEx::GetCheckStatus()
{
	ASSERT(m_nImgCount > 1);
	return m_nCurImgIdx;
}

void CMultiBitmapButtonEx::SetCheckStatus(int nCheck)
{
	if (nCheck>= 0 && nCheck < m_nImgCount)
	{
		SetCurrentBitmapIdx(nCheck);
	}
	else
	{
		ASSERT(FALSE);
	}
}

BOOL CMultiBitmapButtonEx::ActiveEffect( int nFrameWidth , int nRound /*= 0*/, BOOL bActive /*= TRUE*/)
{
	m_bEffect = bActive;
	m_nEffectRound = nRound;
	m_nEffectFrameWidth = nFrameWidth;
	return TRUE;

}

void CMultiBitmapButtonEx::SetCustomFont( CFont& fontIn )
{
	if (m_font.GetSafeHandle())
	{
		m_font.Detach();
	}

	LOGFONT ft;
	fontIn.GetLogFont(&ft);
	m_font.CreateFontIndirect(&ft);

}
//void CMultiBitmapButtonEx::OnBnKillfocus()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_nEffectCounting = 0;
//}

void CMultiBitmapButtonEx::OnNcDestroy()
{
	CButton::OnNcDestroy();
	m_nImgCount = 0;
	// TODO: 在此处添加消息处理程序代码
}

void CMultiBitmapButtonEx::ChangeBitmaps( LPCTSTR lpszBitmaps, BOOL bAutoStretch /*= TRUE*/, BOOL bEdgeStretch /*= FALSE*/ )
{
	//clean old
	Reset();
	LoadBitmaps(lpszBitmaps, bAutoStretch, bEdgeStretch);
	Invalidate(TRUE);

}

void CMultiBitmapButtonEx::ChangeBitmaps( Gdiplus::Image* pImgIn )
{
	Reset();
	LoadBitmaps(pImgIn);
	Invalidate(TRUE);

}

void CMultiBitmapButtonEx::Reset()
{
	for (int i = 0; i < 5; i++)
	{
		m_vImageNormal[i].Reset();	//正常状态
		m_vImageSel[i].Reset();		//按下状态
		m_vImageHover[i].Reset();	//Hover状态
		m_vImageDisable[i].Reset();	//Disable状态
	}

	m_pImgUserPhoto = NULL;
}

void CMultiBitmapButtonEx::Invalidate( BOOL bErase /*= TRUE*/ )
{
	CButton::Invalidate(bErase);
}

void CMultiBitmapButtonEx::SetDrawWithText( BOOL bWithText, int nOffSet )
{
	m_bDrawWithText = bWithText;
	if (m_bDrawWithText)
	{
		m_nTextOffSet = nOffSet;
	}
}

void CMultiBitmapButtonEx::PreSetDividPos( int nFirstPos, int nSecPos )
{
	m_nFirstDivPos = nFirstPos;
	m_nSecDivPos = nSecPos;
}

void CMultiBitmapButtonEx::SetRelayHwnd( HWND hwndIn )
{
	m_hRelayWnd = hwndIn;
}
