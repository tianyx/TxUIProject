/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	GDIDrawFunc.cpp
	file base:	GDIDrawFunc
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "GDIDrawFunc.h"
#include "GdiPlusNewHeader.h"


extern CRect g_rcDefStretchEdge(8,8,8,8);

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

int GPGetTextLineWidth( CDC* pDC, LPCTSTR szText )
{
	CRect rcText(0,0, 20,200);
	pDC->DrawText(szText, rcText, DT_LEFT|DT_WORDBREAK|DT_CALCRECT);
	return rcText.Width();
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

	CRect rcEdgeUse = g_rcDefStretchEdge;
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

void GPCreateRoundRect( Rect& rcIn, int nRidum, GraphicsPath& pathOut )
{


	Rect rcArc(rcIn.X,rcIn.Y, nRidum*2,nRidum*2);
	rcArc.Offset(rcIn.Width -nRidum*2, 0);
	pathOut.AddArc(rcArc, 270,90);
	pathOut.AddLine(rcIn.GetRight(), rcIn.Y + nRidum, rcIn.GetRight(), rcIn.GetBottom() -nRidum);
	rcArc.Offset(0, rcIn.Height - nRidum*2);
	pathOut.AddArc(rcArc, 0,90);
	pathOut.AddLine(rcIn.GetRight() -nRidum, rcIn.GetBottom(), rcIn.X + nRidum, rcIn.GetBottom());
	rcArc.Offset(-rcIn.Width+nRidum*2, 0);
	pathOut.AddArc(rcArc, 90,90);
	pathOut.AddLine(rcIn.X, rcIn.GetBottom() - nRidum, rcIn.X, rcIn.Y + nRidum);
	rcArc.Offset(0, -rcIn.Height +nRidum*2);
	pathOut.AddArc(rcArc, 180,90);
	pathOut.AddLine(rcIn.X+ nRidum, rcIn.Y , rcIn.GetRight()- nRidum,  rcIn.Y);


	

}

