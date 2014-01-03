/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	GDIDrawFunc.h
	file base:	GDIDrawFunc
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include "GdiPlusNewHeader.h"
//#include "preDef.h"
using namespace Gdiplus;
//

extern CRect g_rcDefStretchEdge;

enum ENUM_TXBMPSTRETCHTYPE
{
	TXBMP_STRETCH_NONE	= 0,
	TXBMP_STRETCH_LR	= 1,
	TXBMP_STRETCH_TB	= 2,
	TXBMP_STRETCH_MID_LR =3,
	TXBMP_STRETCH_MID_TB =4,
	TXBMP_STRETCH_MID_LRTB	= 5
};

//check if rc1 in rc2
BOOL IsInRect(CRect& rcSmall, CRect& rcBig);
Rect CRect2Rect(const CRect& rcIn);
RectF CRect2RectF(const CRect& rcIn);
RectF Rect2RectF(Rect& rcIn);

void GPReDrawStatic(CWnd* pWndIn);
void GPInvalidateItemBk(CWnd* pWndIn);
Color RGB2Color(DWORD clrIn);
Color ARGB2Color(ARGB clrIn);


void GPDraw3DFrame(CDC* pDc, CRect& rcFrame, LPCRECT rcClip = NULL);

void GPDrawFrame(CDC* pDc, CRect& rcFrame);
void GPDrawCtrlFrame(CWnd* pWndChild, CDC* pParentDC);
void GPDrawCtrlRoundBk(CWnd* pChild, CDC* pDC, int nAngle = 2, int nOffX = 3, int nOffY = 2, COLORREF clrFrame = RGB(84,165,213), COLORREF clrBk = RGB(255,255,255));

void GPDrawShadowText(Graphics& gc, CString& strTxtIn, CRect& rcIn, Gdiplus::Font& fontIn, ARGB BrushClrIn, ARGB PenClrIn , ARGB shadowPenClrIn = 0xff000000, ARGB shadowBrushClrIn = 0xff000000, int nOffXIn = 2, int nOffYIn = 2, StringFormat* fmtIn = NULL);
void GPDrawShadowTextSimple(Graphics&gc, CString& strTextIn, CRect& rcIn,  Gdiplus::Font& fontIn, ARGB BrushClrIn, ARGB shadowBrushClrIn = 0xff000000, int nOffXIn = 2, int nOffYIn = 2, StringFormat* fmtIn = NULL);

int GPGetTextLineWidth(CDC* pDC, LPCTSTR szText);


//draw line
void GPDrawSeparateLine(CDC* pDC, CPoint& ptStart, CPoint& ptEnd, COLORREF clrIn = RGB(168,117,208), COLORREF clrShadow = 0xffffff);

void GPInvalidateBk(CWnd& item);

//bmp draw
void GPDrawStretchImage(HDC hdc, Bitmap* pImage, CRect& rcDes, CRect* prcImgUseIn = NULL, ENUM_TXBMPSTRETCHTYPE nSTypeIn = TXBMP_STRETCH_NONE, CRect* prcEdgeIn = NULL, CRect* prcEdge2In = NULL);
void GPDrawStretchImage(Graphics& gc, Bitmap* pImage, CRect& rcDes, CRect* prcImgUseIn = NULL, ENUM_TXBMPSTRETCHTYPE nSTypeIn = TXBMP_STRETCH_NONE, CRect* prcEdgeIn = NULL, CRect* prcEdge2In = NULL);

//FillSlashFrame
void GPCreateFlashFrame(Bitmap& bmpSrc, CRect& rcBegin, CRect& rcEnd, Bitmap& bmpDes, CRect& rcDes, double dPercent);

//gdiplus create  roundrect path
void GPCreateRoundRect(Rect& rcIn, int nRidum, GraphicsPath& pathOut);
