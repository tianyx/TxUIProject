// BitmapButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "BitmapButtonEx.h"
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

/////////////////////////////////////////////////////////////////////////////
//BitmapButtonEx

CBitmapButtonEx::CBitmapButtonEx()
{
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
	}
}

CBitmapButtonEx::~CBitmapButtonEx()
{
}


BEGIN_MESSAGE_MAP(CBitmapButtonEx, CButton)
	//{{AFX_MSG_MAP(CBitmapButtonEx)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapButtonEx message handlers

void CBitmapButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
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
			else if (lpDrawItemStruct->itemState & ODS_FOCUS)
			{
				DrawButton(pDC, DB_NORMAL);
				nNaborState = ODS_FOCUS;
			}
			else if (m_pNaborBtn && m_pNaborBtn->GetSafeHwnd() && rcNabour.PtInRect(pt))
			{
				DrawButton(pDC, DB_HOVER);//这时鼠标在按钮上，当鼠标离开如果有焦点才用"DrawButton(DB_FOCUS);
				nNaborState = ODS_HOTLIGHT;	
				//_RPT0(0, "\n self draw hotlight");
			}
			else
			{
				DrawButton(pDC, DB_NORMAL);
				nNaborState = 0;
				//_RPT0(0, "\n self draw normal");
			}
			//
			if (m_pNaborBtn && m_pNaborBtn->IsWindowVisible())
			{
				m_pNaborBtn->m_nNaborSetState = nNaborState;
				m_pNaborBtn->m_bNaborCalledReDraw = TRUE;
				//call nabour Draw
				m_pNaborBtn->Invalidate();
			}
			//
			if (m_pNaborBtn2 && m_pNaborBtn2->IsWindowVisible())
			{
				m_pNaborBtn2->m_nNaborSetState = nNaborState;
				m_pNaborBtn2->m_bNaborCalledReDraw = TRUE;
				//call nabour Draw
				m_pNaborBtn2->Invalidate();
			}
		}
	}
	//modified by tianyx	
}

void CBitmapButtonEx::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point))
	{   
		if (m_bMouseOver == FALSE)
		{   
			m_bMouseOver = TRUE;
			Invalidate(FALSE);
			SetCapture();
		}
	}
	else
	{   
		if (m_bPushed == FALSE)
		{
			m_bMouseOver = FALSE;
			Invalidate(FALSE);
			ReleaseCapture();
		}
	}
	CButton::OnMouseMove(nFlags, point);
}

void CBitmapButtonEx::DrawButton(CDC *pDC,UINT style)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemDC memDC(pDC,rcClient.Width(),rcClient.Height());//首先绘到内存DC,然后再绘到pDC,避免显砾
	CRect rect(rcClient);
    switch (style)
	{   
		case DB_DISABLE:
		{   
			m_imageDisable.Render(&memDC,rect.TopLeft(),rect.Size());
			break;
		}
		case DB_HOVER:
		{
			m_imageHover.Render(&memDC,rect.TopLeft(),rect.Size());
			break;
		}
		case DB_SELECTED:
		{	
			m_imageSel.Render(&memDC,rect.TopLeft(),rect.Size());
			rect.OffsetRect(1,1);
			break;
		}
		   
		default://正常
		{
			m_imageNormal.Render(&memDC,rect.TopLeft(),rect.Size());
		}
	}

	//水平居中绘制文本和图标
	//取得文本长度
	CString str,strTemp;
	GetWindowText(strTemp);
	int iFind = strTemp.Find(TEXT("\%s"),0);

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
	if (style == DB_DISABLE)
	{
		COLORREF clrTextOld = memDC.SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		
		CRect centerRect = rect;
		memDC.DrawText(str, rect, DT_WORDBREAK | DT_CALCRECT | DT_CENTER);
		rect.OffsetRect((centerRect.Width() - rect.Width()) / 2, 
			((centerRect.Height() - rect.Height()) / 2));
		memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);

		rect.OffsetRect(1, 1);
		memDC.SetTextColor(RGB(253, 253, 255));
		if (rect.Width() > iStrLength)
		{
			memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
		}
		else
		{
			memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);
		}
		memDC.SetTextColor(clrTextOld);
	}
	else
	{
		COLORREF clrTextOld = memDC.SetTextColor(m_clrText);
	
		CRect centerRect = rect;
		memDC.DrawText(str, rect, DT_WORDBREAK | DT_CALCRECT | DT_CENTER);
		rect.OffsetRect((centerRect.Width() - rect.Width()) / 2, 
			((centerRect.Height() - rect.Height()) / 2));
		memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);

		if (rect.Width() > iStrLength)
			memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK);
		else
			memDC.DrawText(str, &rect, DT_CENTER | DT_WORDBREAK | DT_END_ELLIPSIS);


		memDC.SetTextColor(clrTextOld);
	}
	memDC.SelectObject(pOldFont);

	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

	if (rect.Width() < iStrLength)
		m_bHasToolTip = TRUE;
	else
		m_bHasToolTip = FALSE;
}

void CBitmapButtonEx::PreSubclassWindow() 
{
	UINT nStyle = GetButtonStyle();
	if (nStyle & BS_CHECKBOX)
		m_bCheckBtn = TRUE;
		
	nStyle |= BS_OWNERDRAW;
	SetButtonStyle(nStyle, FALSE);
	
	//设置按钮高度和背景位图一致
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	if (m_bAutoStretch)
	{
		SetWindowPos(NULL, 0, 0, rcWindow.Width(),
			m_imageNormal.Height(),
			SWP_NOZORDER | SWP_NOMOVE);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, m_imageNormal.Width(),
			m_imageNormal.Height(),
			SWP_NOZORDER | SWP_NOMOVE);
	}

	//{modified by tian_yx 2009-04-15 fix bug that show tips twice on btn;
	EnableToolTips(FALSE);
	//}end by tian_yx 2009-04-15

	CButton::PreSubclassWindow();
}


void CBitmapButtonEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect(&rect);
	if (m_bCheckBtn && rect.PtInRect(point))
	{
		m_bCheck = !m_bCheck;
		Invalidate(FALSE);
	}

	CButton::OnLButtonUp(nFlags, point);

	if (!::IsWindow(m_hWnd))
		return;

	if (rect.PtInRect(point))
	{   
		if (GetCapture() != this && this->IsWindowVisible())
			SetCapture();
		m_bPushed = FALSE;
	}
}

void CBitmapButtonEx::LoadBitmaps(LPCTSTR lpszBmpNormal, 
	LPCTSTR lpszBmpSel,
	LPCTSTR lpszBmpHover,
	LPCTSTR lpszBmpDisable,
	BOOL bAutoStretch)
{
	m_bAutoStretch = bAutoStretch;

	//装载背景位图
	m_imageNormal.LoadBitmap(lpszBmpNormal, 8, 12);
	m_imageHover.LoadBitmap(lpszBmpSel, 8, 12);
	m_imageSel.LoadBitmap(lpszBmpHover, 8, 12);
	m_imageHover.LoadBitmap(lpszBmpDisable, 8, 12);
}

void CBitmapButtonEx::LoadBitmaps(LPCTSTR lpszBitmaps,
	BOOL bAutoStretch)
{
	ASSERT(lpszBitmaps != NULL);
	ASSERT(lpszBitmaps != TEXT(""));
	m_bAutoStretch = bAutoStretch;

	//装载位图
	CBitmap bitmap;
	bitmap.Attach(::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBitmaps), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
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
	
	//提取 bmpSel
	dcDes.SelectObject(&bmpSel);
	dcDes.BitBlt(0, 0, szButton.cx, szButton.cy, &dcSrc,
		szButton.cx * 1, 0, SRCCOPY);
	
	//提取 bmpHover
	dcDes.SelectObject(&bmpHover);
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


	m_imageNormal.LoadBitmap((HBITMAP)bmpNormal.GetSafeHandle(), 8, 12);
	m_imageSel.LoadBitmap((HBITMAP)bmpSel.GetSafeHandle(), 8, 12);
	m_imageHover.LoadBitmap((HBITMAP)bmpHover.GetSafeHandle(), 8, 12);
	m_imageDisable.LoadBitmap((HBITMAP)bmpDisable.GetSafeHandle(), 8, 12);

	bmpNormal.DeleteObject();
	bmpSel.DeleteObject();
	bmpHover.DeleteObject();
	bmpDisable.DeleteObject();
	bitmap.DeleteObject();
}


void CBitmapButtonEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bPushed = TRUE;
	SetFocus();
	CButton::OnLButtonDown(nFlags, point);
}

COLORREF CBitmapButtonEx::SetTextColor(COLORREF clrText)
{
	COLORREF clrOld = m_clrText;
	m_clrText = clrText;
	
	return clrOld;
}


LRESULT CBitmapButtonEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

	return CButton::WindowProc(message, wParam, lParam);
}

void CBitmapButtonEx::OnCaptureChanged(CWnd *pWnd) 
{
	if (pWnd != this)
	{
		m_bPushed = FALSE;
		m_bMouseOver = FALSE;
		Invalidate(FALSE);
	}

	CButton::OnCaptureChanged(pWnd);
}

void CBitmapButtonEx::DrawCheckButton(CDC *pDC, UINT style)
{
	ASSERT(pDC != NULL);
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemDC memDC(pDC,rcClient.Width(),rcClient.Height());//首先绘到内存DC,然后再绘到pDC,避免显砾
	CRect rcImage = rcClient;
	rcImage.right = rcImage.left + m_imageNormal.Width();
    switch (style)
	{   
	case DB_CHECK_DISABLE:
		m_imageDisable.Render(&memDC, rcImage.TopLeft(), rcImage.Size());
		break;
	case DB_CHECK_TRUE:
		m_imageSel.Render(&memDC, rcImage.TopLeft(), rcImage.Size());
		break;
	case DB_CHECK_FALSE:
		m_imageNormal.Render(&memDC, rcImage.TopLeft(), rcImage.Size());
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

int CBitmapButtonEx::OnToolHitTest(CPoint point, TOOLINFO *pTI) const
{
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point) && m_bHasToolTip)
	{
		
		CString strText;
		GetWindowText(strText);
		strText.Replace(TEXT("\%s"),TEXT(" "));
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

void CBitmapButtonEx::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CButton::OnShowWindow(bShow, nStatus);

	if (!bShow && nStatus == 0)
	{
		m_bPushed = FALSE;
		m_bMouseOver = FALSE;
	}

	if (!bShow && GetCapture() == this && nStatus == 0)
		ReleaseCapture();
}

void CBitmapButtonEx::DrawWithNabourState(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
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
}