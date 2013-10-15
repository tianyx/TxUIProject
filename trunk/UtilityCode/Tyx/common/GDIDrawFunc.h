/********************************************************************
	created:	2011/08/10
	created:	10:8:2011   11:31
	filename: 	e:\Source\EZIM\Src\EZMessenger\common\GDIDrawFunc.h
	file path:	e:\Source\EZIM\Src\EZMessenger\common
	file base:	GDIDrawFunc
	file ext:	h
	author:		tian_yuanxin
	
	purpose:	
*********************************************************************/
#pragma once
#include "GdiPlusNewHeader.h"
#include "dbstretchimage.h"
#include "preDef.h"
using namespace Gdiplus;

//check if rc1 in rc2
BOOL IsInRect(CRect& rcSmall, CRect& rcBig);
Rect CRect2Rect(const CRect& rcIn);
RectF CRect2RectF(const CRect& rcIn);
RectF Rect2RectF(Rect& rcIn);

void GPReDrawStatic(CWnd* pWndIn);
void GPInvalidateItemBk(CWnd* pWndIn);
Color RGB2Color(DWORD clrIn);
Color ARGB2Color(ARGB clrIn);


//use GDI+
void GPDrawSunkenText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF foreClr, COLORREF backClr,COLORREF shadowPenClr = 0, COLORREF shadowBrushClr = 0, int nOffXIn = 1, int nOffYIn = 2);
void GPDrawGradientSunkenText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF foreClr, COLORREF backClr,COLORREF shadowPenClr = 0, COLORREF shadowBrushClr = 0, int nOffXIn = 1, int nOffYIn = 2);

//ndrawMode == DT_CENTER, DT_LEFT or DT_RIGHT
void GPDrawGradientShadowText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF BrushClrIn, COLORREF penClrIn, COLORREF shadowPenClr /*= 0*/, COLORREF shadowBrushClr /*= 0*/, int nOffXIn /*= 2*/, int nOffYIn /*= 2*/);


void GPDraw3DFrame(CDC* pDc, CRect& rcFrame, LPCRECT rcClip = NULL);

void GPDrawFrame(CDC* pDc, CRect& rcFrame);
void GPDrawCtrlFrame(CWnd* pWndChild, CDC* pParentDC);
void GPDrawCtrlRoundBk(CWnd* pChild, CDC* pDC, int nAngle = 2, int nOffX = 3, int nOffY = 2, COLORREF clrFrame = RGB(84,165,213), COLORREF clrBk = RGB(255,255,255));

//use GDI
void GPDrawShadowText(CDC* pDc,  CString& strTxtIn, CRect& rcIn, int nDrawMode, COLORREF foreClr, COLORREF shadowClr = 0, int nOffXIn = 1, int nOffYIn = 2);
//use gdi plus
void GPDrawShadowText( HDC hDc, CString& strTxtIn, CRect& rcIn, int nDrawMode, CFont& fontIn, COLORREF BrushClrIn, COLORREF PenClr ,COLORREF shadowPenClr = 0, COLORREF shadowBrushClr = 0, int nOffXIn = 2, int nOffYIn = 2);
void GPDrawShadowText(Graphics& gc, CString& strTxtIn, CRect& rcIn, Gdiplus::Font& fontIn, ARGB BrushClrIn, ARGB PenClrIn , ARGB shadowPenClrIn = 0xff000000, ARGB shadowBrushClrIn = 0xff000000, int nOffXIn = 2, int nOffYIn = 2, StringFormat* fmtIn = NULL);
void GPDrawShadowTextSimple(Graphics&gc, CString& strTextIn, CRect& rcIn,  Gdiplus::Font& fontIn, ARGB BrushClrIn, ARGB shadowBrushClrIn = 0xff000000, int nOffXIn = 2, int nOffYIn = 2, StringFormat* fmtIn = NULL);

//draw frame bk of rect
void GPDrawRectFrameBk(CWnd& m_wndRoomList, CWnd* pParent, CRect& rcThis, CDC* pDC, DBStretchImage&m_imgChatBkTitle,
					 DBStretchImage& m_imgChatBkleft, DBStretchImage& m_imgChatBkRight, DBStretchImage& m_imgChatBkBottom, CRect rcOffSetIn);

int GPGetTextLineWidth(CDC* pDC, LPCTSTR szText);

void GPImageToIcon(HICON& hIcnDesIn, Gdiplus::Bitmap* pImageSrcIn, CSize* pIconSizeIn = NULL);


//the returned image must be deleted out side
Gdiplus::Bitmap* GPCreateGrayImage(Gdiplus::Bitmap* pBmpIn);
Gdiplus::Bitmap* GPCreateResizedImage(Gdiplus::Bitmap* pImageIn, CRect& rcNewIn);


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void GetMatchedEncoderClsid(const WCHAR* wszfilename, CLSID* pClsid);
BOOL GPSaveStreamImage(IStream* pStm, WCHAR* wszFile);

//draw line
void GPDrawSeparateLine(CDC* pDC, CPoint& ptStart, CPoint& ptEnd, COLORREF clrIn = RGB(168,117,208), COLORREF clrShadow = 0xffffff);

void GPInvalidateBk(CWnd& item);

//bmp draw
void GPDrawStretchImage(HDC& hdc, Bitmap* pImage, CRect& rcDes, CRect* prcImgUseIn = NULL, ENUM_TXBMPSTRETCHTYPE nSTypeIn = TXBMP_STRETCH_NONE, CRect* prcEdgeIn = NULL, CRect* prcEdge2In = NULL);
void GPDrawStretchImage(Graphics& gc, Bitmap* pImage, CRect& rcDes, CRect* prcImgUseIn = NULL, ENUM_TXBMPSTRETCHTYPE nSTypeIn = TXBMP_STRETCH_NONE, CRect* prcEdgeIn = NULL, CRect* prcEdge2In = NULL);

//FillSlashFrame
void GPCreateFlashFrame(Bitmap& bmpSrc, CRect& rcBegin, CRect& rcEnd, Bitmap& bmpDes, CRect& rcDes, double dPercent);