#include "StdAfx.h"
#include "GDIDrawFunc.h"
#include "GdiPlusNewHeader.h"

using namespace Gdiplus;

Rect CRect2Rect( const CRect& rcIn )
{
	return Rect(rcIn.left, rcIn.top, rcIn.Width(), rcIn.Height());

}

RectF CRect2RectF( const CRect& rcIn )
{
	return RectF(rcIn.left, rcIn.top, rcIn.Width(), rcIn.Height());
}

void GPReDrawStatic( CWnd* pWndIn )
{
	if (!pWndIn || !::IsWindow(pWndIn->GetSafeHwnd()))
	{
		return;
	}
	CWnd* pSubWnd = pWndIn->GetWindow(GW_CHILD);
	while(pSubWnd != NULL)
	{
		if (pSubWnd->IsKindOf(RUNTIME_CLASS(CStatic)))
		{
			pSubWnd->Invalidate(TRUE);
		}

		pSubWnd = pSubWnd->GetWindow(GW_HWNDNEXT);
	}
}




void GPInvalidateItemBk( CWnd* pWndIn )
{
	if (!pWndIn)
	{
		return;
	}

	CWnd* pPar = pWndIn->GetParent();
	if (!pPar)
	{
		return;
	}

	CRect rcWnd;
	pWndIn->GetWindowRect(rcWnd);
	pPar->ScreenToClient(rcWnd);
	pPar->InvalidateRect(&rcWnd, TRUE);
}

void GPDrawShadowText( Graphics& gc, CString& strTxtIn, CRect& rcIn, Gdiplus::Font& fontIn, ARGB BrushClrIn, ARGB PenClrIn , ARGB shadowPenClrIn /*= 0xff000000*/, ARGB shadowBrushClrIn /*= 0xff000000*/, int nOffXIn /*= 2*/, int nOffYIn /*= 2*/, StringFormat* fmtIn /*= NULL*/ )
{
	Gdiplus::Font& gcfont = fontIn;
	FontFamily fmy;
	gcfont.GetFamily(&fmy);
	int nfontStyle  = gcfont.GetStyle();
	REAL dFontSize = gcfont.GetSize();
	Rect rcText = CRect2Rect(rcIn);
	StringFormat fmt;
	fmt.SetAlignment(StringAlignmentCenter);
	fmt.SetTrimming(StringTrimmingEllipsisWord);
	fmt.SetLineAlignment(StringAlignmentCenter);
	StringFormat& fmtUse = fmtIn == NULL? fmt:*fmtIn;

	GraphicsContainer  gcContainer = gc.BeginContainer();
	gc.SetSmoothingMode(SmoothingModeAntiAlias);
	CComBSTR btrTxtIn(strTxtIn);
	GraphicsPath textPath;
	textPath.AddString(btrTxtIn, -1,  &fmy, nfontStyle, dFontSize, rcText, &fmtUse);

	Matrix mx;
	mx.Translate(nOffXIn, nOffYIn);
	textPath.Transform(&mx);

	Pen textPen(ARGB2Color(shadowPenClrIn), 1);
	SolidBrush textbrush(ARGB2Color(shadowBrushClrIn));
	textPen.SetLineJoin(LineJoinRound);
	if (shadowBrushClrIn != 0)
	{
		gc.FillPath(&textbrush, &textPath);
	}
	if (shadowPenClrIn != 0)
	{
		gc.DrawPath(&textPen, &textPath);
	}



	mx.Invert();
	textPath.Transform(&mx);
	textPen.SetColor(ARGB2Color(PenClrIn));
	textbrush.SetColor(ARGB2Color(BrushClrIn));
	if (BrushClrIn != 0)
	{
		gc.FillPath(&textbrush, &textPath);
	}
	if (PenClrIn != 0)
	{
		gc.DrawPath(&textPen, &textPath);
	}
	gc.EndContainer(gcContainer);
}

void GPDrawShadowTextSimple( Graphics&gc, CString& strTxtIn, CRect& rcIn, Gdiplus::Font& fontIn, ARGB BrushClrIn, ARGB shadowBrushClrIn /*= 0xff000000*/, int nOffXIn /*= 2*/, int nOffYIn /*= 2*/, StringFormat* fmtIn /*= NULL*/ )
{
	Gdiplus::Font& gcfont = fontIn;
	Rect rcText = CRect2Rect(rcIn);
	StringFormat fmt;
	fmt.SetAlignment(StringAlignmentCenter);
	fmt.SetTrimming(StringTrimmingEllipsisWord);
	fmt.SetLineAlignment(StringAlignmentCenter);
	StringFormat& fmtUse = fmtIn == NULL? fmt:*fmtIn;

	GraphicsContainer  gcContainer = gc.BeginContainer();
	gc.SetSmoothingMode(SmoothingModeAntiAlias);
	CComBSTR btrTxtIn(strTxtIn);

	SolidBrush textbrush(ARGB2Color(shadowBrushClrIn));
	RectF rfText = Rect2RectF(rcText);
	if (shadowBrushClrIn != 0)
	{
		rfText.Offset(1.0, 1.0);
		gc.DrawString(btrTxtIn, -1, &gcfont, rfText, &fmtUse, &textbrush);
	}

	textbrush.SetColor(ARGB2Color(BrushClrIn));
	gc.DrawString(btrTxtIn, -1, &gcfont, rfText, &fmtUse, &textbrush);
	gc.EndContainer(gcContainer);
}

void GPDrawGradientShadowText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF BrushClrIn, COLORREF penClrIn, COLORREF shadowPenClr /*= 0*/, COLORREF shadowBrushClr /*= 0*/, int nOffXIn /*= 2*/, int nOffYIn /*= 2*/)
{
	LOGFONT lf;
	fontIn.GetLogFont(&lf);
	Graphics gc(hDc);
	CString m_strGCFontName = lf.lfFaceName;
	CRect rc = rcIn;
	Color clrFrame(Color::MakeARGB(255, GetRValue(BrushClrIn), GetGValue(BrushClrIn), GetBValue(BrushClrIn)));
	{
		//draw countdown text
		BSTR btrFontName = m_strGCFontName.AllocSysString();
		FontFamily  fontFamily(btrFontName);
		SysFreeString(btrFontName);
		Gdiplus::Font        gcfont(&fontFamily, 32, FontStyleRegular, UnitPixel);
		RectF rcText;
		rcText.X = rc.left;
		rcText.Y = rc.top;
		rcText.Width = rc.Width();
		rcText.Height = rc.Height();
		//Color clrText(Color(100,255,255,255));
		SolidBrush textbrush(clrFrame);
		StringFormat fmt;
		fmt.SetAlignment((StringAlignment)nDrawMode);
		fmt.SetTrimming(StringTrimmingEllipsisWord);
		//fmt.SetLineAlignment(StringAlignmentCenter);
		GraphicsContainer  gcContainer;

		gcContainer = gc.BeginContainer();
		gc.SetSmoothingMode(SmoothingModeAntiAlias);
		GraphicsPath textPath;
		Pen textPen(Color(200, clrFrame.GetR(),clrFrame.GetG(),clrFrame.GetB()), 1);
		textPen.SetLineJoin(LineJoinRound);
		BSTR btrTxtIn = strTxtIn.AllocSysString();
		textPath.AddString(btrTxtIn, -1,  &fontFamily, FontStyleBold, -lf.lfHeight, rcText, &fmt);
		SysFreeString(btrTxtIn);
		//gc.DrawString(m_strCountDownText.AllocSysString(), -1, &gcfont, rcText, &fmt, &textbrush);

		Matrix mx;
		mx.Translate(nOffXIn, nOffYIn);
		textPath.Transform(&mx);
		textPen.SetColor(Color(150, GetRValue(shadowPenClr), GetGValue(shadowPenClr), GetBValue(shadowPenClr)));
		textbrush.SetColor(Color(150,GetRValue(shadowBrushClr), GetGValue(shadowBrushClr), GetBValue(shadowBrushClr)));

		gc.FillPath(&textbrush, &textPath);
		gc.DrawPath(&textPen, &textPath);
		mx.Invert();
		textPath.Transform(&mx);
		textPen.SetColor(Color::MakeARGB(255, GetRValue(penClrIn), GetGValue(penClrIn), GetBValue(penClrIn)));
		textbrush.SetColor(Color::MakeARGB(255, GetRValue(BrushClrIn), GetGValue(BrushClrIn), GetBValue(BrushClrIn)));
		
		RectF rcGd(rcText);
		rcGd.Height = 48;
		LinearGradientBrush gradientBrush(rcGd, Color(255, 232,242,254), Color(255, 255,255), LinearGradientModeVertical);
		REAL midPoss[] = {0.0, 0.3,  0.9, 1.0};
		Color gdClr1(Color::MakeARGB(255, GetRValue(BrushClrIn), GetGValue(BrushClrIn), GetBValue(BrushClrIn)));
		Color gdClr2(Color::MakeARGB(255, GetRValue(penClrIn), GetGValue(penClrIn), GetBValue(penClrIn)));
		Color midClrs[] = {gdClr1,  gdClr1, gdClr2, gdClr2}; /*Color(255,244,176,77), Color(255, 114,73,17), Color(255, 114,73,17)};*/
		gradientBrush.SetInterpolationColors(midClrs, midPoss, 4);
		gc.FillPath(&gradientBrush, &textPath);
		gc.DrawPath(&textPen, &textPath);
		gc.EndContainer(gcContainer);
	}
}



void GPDrawShadowText( CDC* pDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, COLORREF foreClr, COLORREF shadowClr /*= 0*/, int nOffXIn /*= 1*/, int nOffYIn /*= 2*/ )
{
	CRect rcShadow(rcIn);
	rcShadow.OffsetRect(nOffXIn, nOffYIn);
	pDc->SetTextColor(shadowClr);
	pDc->DrawText(strTxtIn, -1, rcShadow, nDrawMode|DT_NOPREFIX);
	pDc->SetTextColor(foreClr);
	pDc->DrawText(strTxtIn, -1, rcIn, nDrawMode|DT_NOPREFIX);
}


void GPDrawShadowText( Graphics& gc, CString& strTxtIn, CRect& rcIn, CFont& fontIn, ARGB BrushClrIn, ARGB PenClrIn , ARGB shadowPenClrIn /*= 0xff000000*/, ARGB shadowBrushClrIn /*= 0xff000000*/, int nOffXIn /*= 2*/, int nOffYIn /*= 2*/, StringFormat* fmtIn /*= NULL*/ )
{
	LOGFONT lf;
	fontIn.GetLogFont(&lf);
	CString m_strGCFontName = lf.lfFaceName;
	//draw countdown text
	CComBSTR btrFontName(m_strGCFontName);
	FontFamily  fontFamily(btrFontName);
	Gdiplus::Font gcfont(&fontFamily, 32, FontStyleRegular, UnitPixel);
	Rect rcText = CRect2Rect(rcIn);
	StringFormat fmt;
	fmt.SetAlignment(StringAlignmentCenter);
	fmt.SetTrimming(StringTrimmingEllipsisWord);
	fmt.SetLineAlignment(StringAlignmentCenter);
	StringFormat& fmtUse = fmtIn == NULL? fmt:*fmtIn;

	GraphicsContainer  gcContainer = gc.BeginContainer();
	gc.SetSmoothingMode(SmoothingModeAntiAlias);
	CComBSTR btrTxtIn(strTxtIn);
	GraphicsPath textPath;
	textPath.AddString(btrTxtIn, -1,  &fontFamily, FontStyleBold, -lf.lfHeight, rcText, &fmtUse);

	Matrix mx;
	mx.Translate(nOffXIn, nOffYIn);
	textPath.Transform(&mx);

	Pen textPen(ARGB2Color(shadowPenClrIn), 1);
	SolidBrush textbrush(ARGB2Color(shadowBrushClrIn));
	textPen.SetLineJoin(LineJoinRound);
	gc.FillPath(&textbrush, &textPath);
	gc.DrawPath(&textPen, &textPath);

	mx.Invert();
	textPath.Transform(&mx);
	textPen.SetColor(ARGB2Color(PenClrIn));
	textbrush.SetColor(ARGB2Color(BrushClrIn));
	gc.FillPath(&textbrush, &textPath);
	gc.DrawPath(&textPen, &textPath);
	gc.EndContainer(gcContainer);
}

void GPDrawSunkenText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF foreClr, COLORREF backClr , COLORREF shadowPenClr/* = 0*/, COLORREF shadowBrushClr/* = RGB(255,255,255)*/, int nOffXIn/* = 1*/, int nOffYIn/* = 2*/)
{
	LOGFONT lf;
	fontIn.GetLogFont(&lf);
	Graphics gc(hDc);
	CString m_strGCFontName = lf.lfFaceName;
	CRect rc = rcIn;
	Color clrFrame(Color::MakeARGB(255, GetRValue(foreClr), GetGValue(foreClr), GetBValue(foreClr)));
	{
		//draw countdown text
		BSTR btrFontName = m_strGCFontName.AllocSysString();
		FontFamily  fontFamily(btrFontName);
		SysFreeString(btrFontName);
		Gdiplus::Font        gcfont(&fontFamily, 32, FontStyleRegular, UnitPixel);
		RectF rcText;
		rcText.X = rc.left;
		rcText.Y = rc.top;
		rcText.Width = rc.Width();
		rcText.Height = rc.Height();
		//Color clrText(Color(100,255,255,255));
		SolidBrush textbrush(clrFrame);
		StringFormat fmt;
		fmt.SetAlignment(StringAlignmentCenter);
		fmt.SetLineAlignment(StringAlignmentCenter);
		GraphicsContainer  gcContainer;

		gcContainer = gc.BeginContainer();
		gc.SetSmoothingMode(SmoothingModeAntiAlias);
		GraphicsPath textPath;
		Pen textPen(Color(200, clrFrame.GetR(),clrFrame.GetG(),clrFrame.GetB()), 1);
		textPen.SetLineJoin(LineJoinRound);
		
		BSTR btrTxtIn = strTxtIn.AllocSysString();
		textPath.AddString(btrTxtIn, -1,  &fontFamily, FontStyleRegular, -lf.lfHeight, rcText, &fmt);
		SysFreeString(btrTxtIn);
		//gc.DrawString(m_strCountDownText.AllocSysString(), -1, &gcfont, rcText, &fmt, &textbrush);

		Matrix mx;
		mx.Translate(-nOffXIn,-nOffYIn);
		textPath.Transform(&mx);
		textPen.SetColor(Color(200,GetRValue(shadowBrushClr), GetGValue(shadowBrushClr), GetBValue(shadowBrushClr)));
		textbrush.SetColor(Color(200,GetRValue(shadowPenClr), GetGValue(shadowPenClr), GetBValue(shadowPenClr)));

		gc.FillPath(&textbrush, &textPath);
		gc.DrawPath(&textPen, &textPath);
		mx.Invert();
		textPath.Transform(&mx);
		textPen.SetColor(Color::MakeARGB(255, GetRValue(backClr), GetGValue(backClr), GetBValue(backClr)));
		textbrush.SetColor(Color::MakeARGB(255, GetRValue(foreClr), GetGValue(foreClr), GetBValue(foreClr)));
		//LinearGradientBrush gradientBrush(rcText, Color(255, 232,242,254), Color(255, 255,255), 0);
		//REAL midPoss[] = {0.0, 0.4,  0.9, 1.0};
		//Color midClrs[] = {Color(255,255,255,255), Color(255,255,255,255), Color(100,255,255,255), Color(100, 255,255,255)};
		//gradientBrush.SetInterpolationColors(midClrs, midPoss, 4);
		//gc.FillRectangle(&gradientBrush, rcText);
		gc.FillPath(&textbrush, &textPath);
		gc.DrawPath(&textPen, &textPath);
		gc.EndContainer(gcContainer);
	}
}


void GPDrawGradientSunkenText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF foreClr, COLORREF backClr , COLORREF shadowPenClr/* = 0*/, COLORREF shadowBrushClr/* = RGB(255,255,255)*/, int nOffXIn/* = 1*/, int nOffYIn/* = 2*/)
{
	LOGFONT lf;
	fontIn.GetLogFont(&lf);
	Graphics gc(hDc);
	CString m_strGCFontName = lf.lfFaceName;
	CRect rc = rcIn;
	Color clrFrame(Color::MakeARGB(255, GetRValue(foreClr), GetGValue(foreClr), GetBValue(foreClr)));
	{
		//draw countdown text
		BSTR btrFontName = m_strGCFontName.AllocSysString();
		FontFamily  fontFamily(btrFontName);
		SysFreeString(btrFontName);
		Gdiplus::Font        gcfont(&fontFamily, 32, FontStyleRegular, UnitPixel);
		RectF rcText;
		rcText.X = rc.left;
		rcText.Y = rc.top;
		rcText.Width = rc.Width();
		rcText.Height = rc.Height();
		//Color clrText(Color(100,255,255,255));
		SolidBrush textbrush(clrFrame);
		StringFormat fmt;
		fmt.SetAlignment(StringAlignmentCenter);
		//fmt.SetLineAlignment(StringAlignmentCenter);
		GraphicsContainer  gcContainer;

		gcContainer = gc.BeginContainer();
		gc.SetSmoothingMode(SmoothingModeAntiAlias);
		GraphicsPath textPath;
		Pen textPen(Color(200, clrFrame.GetR(),clrFrame.GetG(),clrFrame.GetB()), 1);
		textPen.SetLineJoin(LineJoinRound);
		BSTR btrText = strTxtIn.AllocSysString();
		textPath.AddString(btrText, -1,  &fontFamily, FontStyleRegular, -lf.lfHeight, rcText, &fmt);
		SysFreeString(btrText);
		//gc.DrawString(m_strCountDownText.AllocSysString(), -1, &gcfont, rcText, &fmt, &textbrush);

		Matrix mx;
		mx.Translate(-nOffXIn,-nOffYIn);
		textPath.Transform(&mx);
		textPen.SetColor(Color(200,GetRValue(shadowBrushClr), GetGValue(shadowBrushClr), GetBValue(shadowBrushClr)));
		textbrush.SetColor(Color(200,GetRValue(shadowPenClr), GetGValue(shadowPenClr), GetBValue(shadowPenClr)));

		gc.FillPath(&textbrush, &textPath);
		gc.DrawPath(&textPen, &textPath);
		mx.Invert();
		textPath.Transform(&mx);
		textPen.SetColor(Color::MakeARGB(255, GetRValue(backClr), GetGValue(backClr), GetBValue(backClr)));
		textbrush.SetColor(Color::MakeARGB(255, GetRValue(foreClr), GetGValue(foreClr), GetBValue(foreClr)));
		RectF rcGd(rcText);
		rcGd.Height = 48;
		LinearGradientBrush gradientBrush(rcGd, Color(255, 232,242,254), Color(255, 255,255), LinearGradientModeVertical);
		REAL midPoss[] = {0.0, 0.3,  0.9, 1.0};
		Color gdClr1(Color::MakeARGB(255, GetRValue(foreClr), GetGValue(foreClr), GetBValue(foreClr)));
		Color gdClr2(Color::MakeARGB(255, GetRValue(backClr), GetGValue(backClr), GetBValue(backClr)));
		Color midClrs[] = {gdClr1,  gdClr1, gdClr2, gdClr2}; /*Color(255,244,176,77), Color(255, 114,73,17), Color(255, 114,73,17)};*/
		gradientBrush.SetInterpolationColors(midClrs, midPoss, 4);
		//gc.FillRectangle(&gradientBrush, rcText);
		gc.FillPath(&gradientBrush, &textPath);
		gc.DrawPath(&textPen, &textPath);
		gc.EndContainer(gcContainer);
	}
}



void GPDraw3DFrame( CDC* pDC, CRect& rcDayFrame, LPCRECT pRcClip)
{
	CBrush brush1, brush2;
	brush1.CreateSolidBrush( RGB(149,149,149));
	brush2.CreateSolidBrush( RGB(255,255,255));
	
	if (pRcClip)
	{
		pDC->ExcludeClipRect(pRcClip);
	}
	

	pDC->FrameRect(rcDayFrame, &brush1);
	pDC->SelectObject(&brush2);
	rcDayFrame.InflateRect(1,1,1,1);
	pDC->FrameRect(rcDayFrame, &brush2);
	rcDayFrame.DeflateRect(2,2,2,2);
	pDC->FrameRect(rcDayFrame, &brush2);


	if (pRcClip)
	{
		CRgn rgn;	
		rgn.CreateRectRgn(pRcClip->left, pRcClip->top, pRcClip->right, pRcClip->bottom);
		pDC->SelectClipRgn(&rgn, RGN_OR);
	}
}

void GPDrawFrame( CDC* pDC, CRect& rcFrame )
{
	CRect rcFm(rcFrame);
	rcFm.InflateRect(1,1,1,1);
	CBrush brush1;
	brush1.CreateSolidBrush( RGB(0,0,0));
	pDC->FrameRect(rcFm, &brush1);

}

void GPDrawCtrlFrame(CWnd* pWndChild, CDC* pParentDC)
{
	CRect rcFmTmp;
	pWndChild->GetWindowRect(rcFmTmp);
	pWndChild->GetParent()->ScreenToClient(rcFmTmp);
	rcFmTmp.InflateRect(1,1,1,1);
	GPDrawFrame(pParentDC, rcFmTmp);

}

void GPDrawRectFrameBk(CWnd& m_wndRoomList, CWnd* pParent, CRect& rcThis, CDC* pDC, DBStretchImage&m_imgChatBkTitle,
					 DBStretchImage& m_imgChatBkleft, DBStretchImage& m_imgChatBkRight, DBStretchImage& m_imgChatBkBottom, CRect rcOffSetIn)
{

	CRect rcList;
	m_wndRoomList.GetWindowRect(rcList);
	pParent->ScreenToClient(rcList);
	CRect rcLstFrame = rcList;
	rcList.InflateRect(8+rcOffSetIn.left, 70+ rcOffSetIn.top , 8 + rcOffSetIn.right,5 + rcOffSetIn.bottom);

	CRect rcTitle(rcList);
	rcTitle.bottom =rcTitle.top + m_imgChatBkTitle.Height();
	m_imgChatBkTitle.Render(pDC, rcTitle.TopLeft(), rcTitle.Size());

	CRect rcBkLeft = rcList;
	rcBkLeft.right = rcBkLeft.left + m_imgChatBkleft.Width();
	CRect rcBkMid = rcList;
	rcBkMid.top = rcTitle.bottom;
	rcBkMid.left = rcBkLeft.right;
	rcBkMid.right = rcList.right - 8;
	rcBkMid.bottom = rcList.bottom;
	//m_imgChatBkMid.Render(pDC, rcBkMid.TopLeft(), rcBkMid.Size());
	pDC->FillSolidRect(rcBkMid, RGB(234,232,232));
	rcBkLeft.bottom = rcBkMid.bottom;
	rcBkLeft.top = rcBkMid.top;
	m_imgChatBkleft.Render(pDC, rcBkLeft.TopLeft(), rcBkLeft.Size());

	CRect rcBkRight = rcList;
	rcBkRight.top = rcBkMid.top;
	rcBkRight.left = rcBkMid.right;
	rcBkRight.right = rcBkRight.left + m_imgChatBkRight.Width();
	rcBkRight.bottom = rcBkMid.bottom;

	m_imgChatBkRight.Render(pDC,rcBkRight.TopLeft(),rcBkRight.Size());

	CRect rcBkBottom(rcTitle);
	rcBkBottom.top = rcBkMid.bottom;
	rcBkBottom.bottom = rcBkBottom.top + m_imgChatBkBottom.Height();
	rcBkBottom.DeflateRect(0,0,0,0);
	m_imgChatBkBottom.Render(pDC, rcBkBottom.TopLeft(), rcBkBottom.Size());


	rcLstFrame.InflateRect(1,1,1,1);
	pDC->FillSolidRect(rcLstFrame, RGB(255,255,255));
	CBrush brush(RGB(200,207,219));
	pDC->FrameRect(rcLstFrame, &brush);
}

int GPGetTextLineWidth( CDC* pDC, LPCTSTR szText )
{
	CRect rcText(0,0, 20,200);
	pDC->DrawText(szText, rcText, DT_LEFT|DT_WORDBREAK|DT_CALCRECT);
	return rcText.Width();
}

Gdiplus::Bitmap* GPCreateGrayImage( Gdiplus::Bitmap* pBmpIn )
{
	ASSERT(pBmpIn);
	UINT nWidth = pBmpIn->GetWidth();
	UINT nHeight = pBmpIn->GetHeight();
	Gdiplus::Bitmap* pBmp = pBmpIn->Clone(0,0, nWidth, nHeight, PixelFormat32bppARGB);
	for (int i = 0; i < nHeight; i ++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			Color clr;
			pBmp->GetPixel(j, i, &clr);
			int nRVal = (clr.GetR());
			int nGVal = (clr.GetG());
			int nBVal = (clr.GetB());
			double dRVal = double(nRVal)/ 255.0;
			double dGVal = double(nGVal) / 255.0;
			double dBVal = double(nBVal) / 255.0;
			int nGray = (int)((dRVal* 0.229 + dGVal* 0.587 + dBVal * 0.114) * 255);
			pBmp->SetPixel(j,i, Color::MakeARGB(clr.GetA(), nGray,nGray, nGray));
		}
	}

	return pBmp;
}




Gdiplus::Bitmap* GPCreateResizedImage( Gdiplus::Bitmap* pImageIn, CRect& rcNewIn )
{
	ASSERT(pImageIn);
	ASSERT(!(rcNewIn.IsRectEmpty() || rcNewIn.IsRectNull()));

	Gdiplus::Image* pNewImge = pImageIn->GetThumbnailImage(rcNewIn.Width(), rcNewIn.Height(), NULL, NULL);
	ASSERT(pNewImge);
	Gdiplus::Bitmap* pNewBmp = new Gdiplus::Bitmap(pNewImge->GetWidth(), pNewImge->GetHeight());
	ASSERT(pNewBmp);
	Gdiplus::Graphics gc(pNewBmp);
	gc.DrawImage(pNewImge, 0,0, pNewImge->GetWidth(), pNewImge->GetHeight());
	delete pNewImge;
	return pNewBmp;
}

void GPDrawCtrlRoundBk( CWnd* pWndChild, CDC* pDC, int nAngle /*= 2*/, int nOffX /*= 3*/, int nOffY /*= 2*/, COLORREF clrFrame /*= RGB(0,0, 255)*/, COLORREF clrBk /*= RGB(0,0,0)*/ )
{
	CRect rcFmTmp;
	pWndChild->GetWindowRect(rcFmTmp);
	pWndChild->GetParent()->ScreenToClient(rcFmTmp);
	rcFmTmp.InflateRect(nOffX,nOffX,nOffY,nOffY);
	CBrush brush1, brush2;
	brush1.CreateSolidBrush(clrBk);
	brush2.CreateSolidBrush(clrFrame);
	CRgn rgn;
	rgn.CreateRoundRectRgn(rcFmTmp.left, rcFmTmp.top, rcFmTmp.right, rcFmTmp.bottom, nAngle, nAngle);
	pDC->FillRgn(&rgn,  &brush1);
	pDC->FrameRgn(&rgn, &brush2, 1, 1);
}

void GPImageToIcon( HICON& hIcnOut, Gdiplus::Bitmap* pImageSrcIn, CSize* pIconSizeIn/* = NULL*/)
{
	CSize icnSize(16,16);
	if (pIconSizeIn)
	{
		icnSize = *pIconSizeIn;
	}
	Bitmap* pBmpIcnClr = new Bitmap(icnSize.cx, icnSize.cy);
	Bitmap* pBmpIcnMask = new Bitmap(icnSize.cx, icnSize.cy); 
	
	Rect rcIcn;
	rcIcn.Width = pBmpIcnClr->GetWidth();
	rcIcn.Height = pBmpIcnClr->GetHeight();

	Rect rcSrc;
	rcSrc.Width = pImageSrcIn->GetWidth();
	rcSrc.Height = pImageSrcIn->GetHeight();

	Bitmap* pBmpSrc = pImageSrcIn;

	if (rcSrc.Width != rcIcn.Width || rcSrc.Height != rcIcn.Height)
	{
		pBmpSrc = GPCreateResizedImage(pImageSrcIn, CRect(0,0, rcIcn.Width, rcIcn.Height));
		ASSERT(pBmpSrc);
	}

	UINT* pixels = new UINT[rcIcn.Width * rcIcn.Height ];
	UINT* pixelMask = new UINT[rcIcn.Width * rcIcn.Height ];

	BitmapData bitSrc;

	pBmpSrc->LockBits(&rcIcn, ImageLockModeRead, PixelFormat32bppARGB, &bitSrc);
	
	UINT* pixelSrc = (UINT*)bitSrc.Scan0;
	for(UINT row = 0; row < rcIcn.Height; ++row)
	{
		CString strRow;
		CString strRowMask;
		for(UINT col = 0; col < rcIcn.Width; ++col)
		{
			UINT nPixel = pixelSrc[row*bitSrc.Stride/4 + col];
			pixels[row*bitSrc.Stride/4 + col] = nPixel;
			pixelMask[row*bitSrc.Stride/4 + col] = nPixel; //(nPixel == 0 || (nPixel & 0xff000000) == 0)? 0x0:0xffffffff;
// 			TRACE1("%0x-", pixels[row*bitSrc.Stride/4 + col]);
// 			TRACE1("%0x", pixelMask[row*bitSrc.Stride/4 + col]);
		}
//		TRACE("\n");
	}

	BitmapData bitData = bitSrc;
	bitData.Scan0 = pixels;
	BitmapData bitDataMask = bitSrc;
	bitDataMask.Scan0 = pixelMask;
	pBmpIcnClr->LockBits(&rcIcn, ImageLockModeWrite|ImageLockModeUserInputBuf, PixelFormat32bppARGB, &bitData);
	pBmpIcnClr->UnlockBits(&bitData);

	pBmpIcnMask->LockBits(&rcIcn,  ImageLockModeWrite|ImageLockModeUserInputBuf, PixelFormat32bppARGB, &bitDataMask);
	pBmpIcnMask->UnlockBits(&bitDataMask);

	pBmpSrc->UnlockBits(&bitSrc);

// 	bitData.Scan0 = NULL;
// 	pBmpIcnClr->LockBits(&rcIcn, ImageLockModeRead, PixelFormat32bppARGB, &bitData);
// 	pBmpIcnClr->UnlockBits(&bitData);
// 
// 	bitDataMask.Scan0 = NULL;
// 	pBmpIcnMask->LockBits(&rcIcn, ImageLockModeRead, PixelFormat32bppARGB, &bitDataMask);
// 	pBmpIcnMask->UnlockBits(&bitDataMask);


	//create bitmap
	ICONINFO customIconInfo;
	customIconInfo.fIcon = TRUE;
	customIconInfo.xHotspot = 8;
	customIconInfo.yHotspot = 8;
	Color clrBk(0xffffffff);
	pBmpIcnClr->GetHBITMAP(clrBk, &customIconInfo.hbmColor);
	pBmpIcnMask->GetHBITMAP(clrBk, &customIconInfo.hbmMask);
	hIcnOut = CreateIconIndirect(&customIconInfo);

	if (pBmpSrc != pImageSrcIn)
	{
		delete pBmpSrc;
	}

	delete pBmpIcnClr;
	delete pBmpIcnMask;

	DeleteObject(customIconInfo.hbmColor);
	DeleteObject(customIconInfo.hbmMask);

	if (pixels)
	{
		delete[] pixels;
	}

	if (pixelMask)
	{
		delete[] pixelMask;
	}
}

BOOL GPSaveStreamImage( IStream* pStm, WCHAR* wszFile )
{
	Gdiplus::Image* pRetImg = Gdiplus::Image::FromStream(pStm);
	CLSID jpgClsid;
	GetMatchedEncoderClsid(wszFile, &jpgClsid);
	int nRet = pRetImg->Save(wszFile, &jpgClsid);

	if (nRet != 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	return TRUE;
}



int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num= 0;
	UINT size= 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo= NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size== 0)
	{
		return -1;
	}
	pImageCodecInfo= (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo== NULL)
	{
		return -1;
	}

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j=0; j< num; ++j)
	{
		if(wcscmp(pImageCodecInfo[j].MimeType, format)== 0)
		{
			*pClsid= pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}

void GetMatchedEncoderClsid( const WCHAR* wszfilename, CLSID* pClsid )
{
	CString strFile(wszfilename);
	int nPos =strFile.ReverseFind(L'.');
	CString strExten =TEXT(".jpg");
	if (nPos > 0)
	{
		strExten = strFile.Mid(nPos);
	}

	strExten.MakeLower();
	if(strExten.Compare(TEXT(".jpg")) == 0)
	{
		GetEncoderClsid(L"image/jpeg", pClsid);
	}
	else if(strExten.Compare(TEXT(".bmp")) == 0)
	{
		GetEncoderClsid(L"image/bmp", pClsid);
	}
	else if (strExten.Compare(TEXT(".gif")) == 0)
	{
		GetEncoderClsid(L"image/gif", pClsid);
	}
	else if (strExten.Compare(TEXT(".tif")) == 0)
	{
		GetEncoderClsid(L"image/tiff", pClsid);
	}
	else if (strExten.Compare(TEXT(".png")) == 0)
	{
		GetEncoderClsid(L"image/png", pClsid);
	}
	else 
	{
		GetEncoderClsid(L"image/jpeg", pClsid);
	}
}

void GPDrawSeparateLine( CDC* pDC, CPoint& ptStart, CPoint& ptEnd, COLORREF clrIn /*= RGB(168,117,208)*/, COLORREF clrShadow /*= 0xffffff*/ )
{
	CDC& dc = *pDC;
	LOGPEN logpen;
	logpen.lopnColor = clrIn;
	logpen.lopnStyle = PS_SOLID;
	logpen.lopnWidth.x =0;
	logpen.lopnWidth.y = 0;
	CPen pen1;
	pen1.CreatePenIndirect(&logpen);
	CGdiObject* poldObj = dc.SelectObject(&pen1);
	dc.MoveTo(ptStart);
	dc.LineTo(ptEnd);
	CPen pen2;
	logpen.lopnColor = clrShadow;
	pen2.CreatePenIndirect(&logpen);
	dc.SelectObject(&pen2);
	ptStart.y++;
	ptEnd.y++;
	dc.MoveTo(ptStart);
	dc.LineTo(ptEnd);
	dc.SelectObject(poldObj);
	pen2.DeleteObject();
	pen1.DeleteObject();

}

void GPInvalidateBk( CWnd& item )
{
	CWnd* pWnd = item.GetParent();
	if (!pWnd)
	{
		return;
	}
	CRect rcThis;
	item.GetWindowRect(rcThis);
	pWnd->ScreenToClient(rcThis);
	pWnd->InvalidateRect(rcThis);
}

void GPDrawStretchImage( HDC hdc, Bitmap* pImage, CRect& rcDes, CRect* prcImgUseIn /*= NULL*/, ENUM_TXBMPSTRETCHTYPE nSTypeIn /*= TXBMP_STRETCH_NONE*/, CRect* prcEdgeIn /*= NULL*/, CRect* prcEdge2In /*= NULL*/ )
{
	if (pImage == NULL || hdc == NULL || rcDes.IsRectEmpty())
	{
		ASSERT(FALSE);
		return;
	}

	Graphics gc(hdc);
	GPDrawStretchImage(gc, pImage, rcDes, prcImgUseIn, nSTypeIn, prcEdgeIn, prcEdge2In);
}

void GPDrawStretchImage( Graphics& gc, Bitmap* pImage, CRect& rcDes, CRect* prcImgUseIn /*= NULL*/, ENUM_TXBMPSTRETCHTYPE nSTypeIn /*= TXBMP_STRETCH_NONE*/, CRect* prcEdgeIn /*= NULL*/, CRect* prcEdge2In /*= NULL*/ )
{
	if (pImage == NULL || rcDes.IsRectEmpty())
	{
		ASSERT(FALSE);
		return;
	}

	CSize szImage(pImage->GetWidth(), pImage->GetHeight());
	CRect rcImgUse;
	if (prcImgUseIn == NULL)
	{
		rcImgUse = CRect(0,0, szImage.cx, szImage.cy);
	}
	else
	{
		rcImgUse = *prcImgUseIn;
	}

	CRect rcEdgeUse = g_globalInfo.txDrawParam.rcStretchEdge;
	if (prcEdgeIn != NULL)
	{
		rcEdgeUse = *prcEdgeIn;
	}

	GraphicsContainer container = gc.BeginContainer();
	Rect rDes = CRect2Rect(rcDes);
	gc.SetClip(rDes);

	if (rcDes.Size() == rcImgUse.Size())
	{
		nSTypeIn = TXBMP_STRETCH_NONE;
	}


	switch(nSTypeIn)
	{
	case TXBMP_STRETCH_NONE:
		{
			gc.DrawImage(pImage, rDes, rcImgUse.left, rcImgUse.top, rcImgUse.Width(), rcImgUse.Height(), UnitPixel);
		}break;
	case TXBMP_STRETCH_LR:
		{

		}break;
	case TXBMP_STRETCH_TB:
		{
			ASSERT(FALSE);

		}break;
	case TXBMP_STRETCH_MID_LR:
		{
			CRect rcMid(rcDes);
			rcMid.DeflateRect(rcEdgeUse.left, 0, rcEdgeUse.right, 0);

			//draw left edge
			Rect rLeft = rDes;
			rLeft.Width =rcEdgeUse.left;
			gc.DrawImage(pImage, rLeft, rcImgUse.left, rcImgUse.top, rLeft.Width, rLeft.Height, UnitPixel);

			//draw right edge
			Rect rRight = Rect(rcMid.right, rcDes.top, rcEdgeUse.right, rcDes.bottom);
			CPoint ptImgUse(rcImgUse.right - rcEdgeUse.right, rcImgUse.top);
			gc.DrawImage(pImage, rRight, ptImgUse.x, ptImgUse.y, rRight.Width, rRight.Height, UnitPixel);

			//draw mid
			Rect rMid = CRect2Rect(rcMid);
			CRect rcImgMidUse(rcImgUse);
			rcImgMidUse.DeflateRect(rcEdgeUse.left, 0, rcEdgeUse.right, 0);
			gc.DrawImage(pImage, rMid, rcImgMidUse.left, rcImgMidUse.top, rcImgMidUse.Width(), rcImgMidUse.Height(), UnitPixel);

		}break;
	case TXBMP_STRETCH_MID_TB:
		{
			CRect rcMid(rcDes);
			rcMid.DeflateRect(0, rcEdgeUse.top, 0, rcEdgeUse.bottom);

			//draw top
			Rect rTop(rcDes.left,rcDes.top, rcDes.Width(), rcEdgeUse.top);
			Rect rImgTop(rcImgUse.left, rcImgUse.top, rcImgUse.Width(), rcEdgeUse.top);
			gc.DrawImage(pImage, rTop, rImgTop.X, rImgTop.Y, rImgTop.Width, rImgTop.Height, UnitPixel);

			//draw bottom
			Rect rBottom = Rect(rcDes.left, rcMid.bottom, rcDes.Width(), rcEdgeUse.bottom);
			Rect rImgBottom(rcImgUse.left, rcImgUse.bottom - rcEdgeUse.bottom, rcImgUse.Width(), rcEdgeUse.bottom);
			gc.DrawImage(pImage, rBottom, rImgBottom.X, rImgBottom.Y, rImgBottom.Width, rImgBottom.Height, UnitPixel);

			//draw Mid
			Rect rMid = CRect2Rect(rcMid);
			Rect rImgMid(rcImgUse.left, rcImgUse.top + rcEdgeUse.top, rcImgUse.Width(), rcImgUse.Height() - rcEdgeUse.top - rcEdgeUse.bottom);
			gc.DrawImage(pImage, rMid, rImgMid.X, rImgMid.Y, rImgMid.Width, rImgMid.Height, UnitPixel);


		}break;
	case TXBMP_STRETCH_MID_LRTB:
		{
			CRect rcMid(rcDes);
			rcMid.DeflateRect(rcEdgeUse);
			CRect rcImgMid(rcImgUse);
			rcImgMid.DeflateRect(rcEdgeUse);
			//left -top corner
			Rect rLeftTop(rcDes.left, rcDes.top, rcEdgeUse.left, rcEdgeUse.top);
			Rect rImgDraw(rcImgUse.left, rcImgUse.top, rcEdgeUse.left, rcEdgeUse.top);
			gc.DrawImage(pImage, rLeftTop, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//top
			Rect rTop(rcMid.left, rcDes.top, rcMid.Width(), rcEdgeUse.top);
			rImgDraw = Rect(rcImgMid.left, rcImgUse.top, rcImgMid.Width(), rcEdgeUse.top);

			gc.DrawImage(pImage, rTop, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//top - right corner
			Rect rTopRight(rcMid.right, rcDes.top, rcEdgeUse.right, rcEdgeUse.top);
			rImgDraw = Rect(rcImgMid.right, rcImgUse.top, rcEdgeUse.right, rcEdgeUse.top);
			gc.DrawImage(pImage, rTopRight, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//right
			Rect rRight(rcMid.right, rcMid.top, rcEdgeUse.right, rcMid.Height());
			rImgDraw = Rect(rcImgMid.right, rcImgMid.top, rcEdgeUse.right, rcImgMid.Height());
			gc.DrawImage(pImage, rRight, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);


			//right-bottom corner
			Rect rRightBottom(rcMid.right, rcMid.bottom, rcEdgeUse.right, rcEdgeUse.bottom);
			rImgDraw = Rect(rcImgMid.right, rcImgMid.bottom, rcEdgeUse.right, rcEdgeUse.bottom);
			gc.DrawImage(pImage, rRightBottom, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//bottom
			Rect rBottom(rcMid.left, rcMid.bottom+1, rcMid.Width(), rcEdgeUse.bottom);
			rImgDraw = Rect(rcImgMid.left, rcImgMid.bottom, rcImgMid.Width(), rcEdgeUse.bottom);
			gc.DrawImage(pImage, rBottom, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//left-bottom corner
			Rect rLeftBottom(rcDes.left, rcMid.bottom, rcEdgeUse.left, rcEdgeUse.bottom);
			rImgDraw = Rect(rcImgUse.left, rcImgMid.bottom, rcEdgeUse.left, rcEdgeUse.bottom);
			gc.DrawImage(pImage, rLeftBottom, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//left
			Rect rLeft(rcDes.left, rcMid.top, rcEdgeUse.left, rcMid.Height());
			rImgDraw = Rect(rcImgUse.left, rcImgMid.top, rcEdgeUse.left, rcImgMid.Height());
			gc.DrawImage(pImage, rLeft, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

			//mid
			Rect rMid = CRect2Rect(rcMid);
			rImgDraw = CRect2Rect(rcImgMid);
			gc.DrawImage(pImage, rMid, rImgDraw.X, rImgDraw.Y, rImgDraw.Width, rImgDraw.Height, UnitPixel);

		}break;
	}

	gc.EndContainer(container);
}


Color RGB2Color( COLORREF clrIn )
{
	return Color::MakeARGB(255, GetRValue(clrIn), GetGValue(clrIn), GetBValue(clrIn));
}

Color ARGB2Color( ARGB clrIn )
{
	return Color(clrIn);

}


RectF Rect2RectF( Rect& rcIn )
{
	return RectF(rcIn.X, rcIn.Y, rcIn.Width, rcIn.Height);
}

void GPCreateFlashFrame( Bitmap& bmpSrc, CRect& rcBegin, CRect& rcEnd, Bitmap& bmpDes, CRect& rcDes, double dPercent )
{
	int  nWidth = bmpSrc.GetWidth();
	int  nHeight = bmpSrc.GetHeight();
	double dPercent2 = 1.0 - dPercent;
	int  nWidthDes = bmpDes.GetWidth();
	int  nHeightDes = bmpDes.GetHeight();
	CRect rcBmpSrc(0,0, nWidth, nHeight);
	CRect rcBmpDes(0,0, nWidthDes, nHeightDes);

	ASSERT(IsInRect(rcBegin, rcBmpSrc));
	ASSERT(IsInRect(rcEnd, rcBmpSrc));
	ASSERT(IsInRect(rcDes, rcBmpDes));
	ASSERT(rcBegin.Size() == rcEnd.Size());
	ASSERT(rcEnd.Size() == rcDes.Size());
	int nLoopWidth = rcBegin.Width();
	int nLoopHeight = rcBegin.Height();
	for (int i = 0; i <nLoopWidth; i++)
	{
		for (int j = 0; j < nLoopHeight; j++)
		{
			Color clrBegin;
			bmpSrc.GetPixel(i+ rcBegin.left, j+ rcBegin.top, &clrBegin);
			Color clrEnd;
			bmpSrc.GetPixel(i+ rcEnd.left, j+ rcEnd.top, &clrEnd);
			Color clrDes = Color::MakeARGB(clrBegin.GetA()*dPercent + clrEnd.GetA()* dPercent2, 
				clrBegin.GetR()*dPercent + clrEnd.GetR()* dPercent2,
				clrBegin.GetG()*dPercent + clrEnd.GetG()* dPercent2,
				clrBegin.GetB()*dPercent + clrEnd.GetB()* dPercent2);
			bmpDes.SetPixel(rcDes.left + i, rcDes.top+j, clrDes);
		}
	}


}

BOOL IsInRect( CRect& rcSmall, CRect& rcBig )
{
	return rcSmall.left >= rcBig.left && rcSmall.top >= rcBig.top && rcSmall.right <= rcBig.right && rcSmall.bottom <= rcBig.bottom;
}

