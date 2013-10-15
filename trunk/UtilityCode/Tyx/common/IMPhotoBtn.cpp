#include "StdAfx.h"
#include "IMPhotoBtn.h"
#include "GdiPlusNewHeader.h"
#include "GDIDrawFunc.h"
#include "IMDataOperator.h"
#include "ChatMgr.h"

#include "UserNodeItem.h"
using namespace Gdiplus;

#define DB_NORMAL		 0
#define DB_HOVER		 1
#define DB_SELECTED		 2
#define DB_DISABLE		 3




Gdiplus::Bitmap * CIMPhotoBtn::m_spImgFrame =NULL;
Gdiplus::Bitmap * CIMPhotoBtn::m_spImgFrameSmall =NULL;

Gdiplus::Bitmap* CIMPhotoBtn::m_spBmpGray = NULL;
Gdiplus::Bitmap* CIMPhotoBtn::m_spBmpPhotoCommon = NULL;
Gdiplus::Bitmap* CIMPhotoBtn::m_spBmpSmallCommon = NULL;
Gdiplus::Bitmap* CIMPhotoBtn::m_spBmpSmallGray = NULL;

CIMPhotoBtn::CIMPhotoBtn(void)
{
	m_dwUserStatus = IM_USER_STATUS_OFFLINE;
	m_bUseCommonPhoto = FALSE;
	bIntervalCallDraw = FALSE;
	m_pBmpGray = NULL;
	m_pBmpSmallGray = NULL;
	m_pBmpPhoto = NULL;
	m_pBmpSmallPhoto = NULL;
	m_bFocusStyle = 1;
	m_bDrawSelfState = FALSE;
}

CIMPhotoBtn::~CIMPhotoBtn(void)
{
}


void CIMPhotoBtn::LoadSingleBitmaps( LPCTSTR lpszBitmaps, BOOL bAutoStretch /*= TRUE*/ )
{

	if (lpszBitmaps == NULL)
	{
		m_bUseCommonPhoto  = TRUE;
		LoadDefaultBitmap();

	}
	else
	{
		CString strFilePath = lpszBitmaps;
		CString strFullFile = GetFullFilePath(strFilePath);
		BSTR bstrTmp = strFullFile.AllocSysString();
		Bitmap* m_pBmpPhoto = Bitmap::FromFile(bstrTmp);
		SysFreeString(bstrTmp);
		m_pBmpGray = CreateGrayImage(m_pBmpPhoto);
		m_pBmpSmallPhoto = CreateResizedImage(m_pBmpPhoto, CRect(0,0, PHOTO_SMALL_WIDTH,PHOTO_SMALL_WIDTH));
		m_pBmpSmallGray = CreateResizedImage(m_pBmpGray, CRect(0,0, PHOTO_SMALL_WIDTH,PHOTO_SMALL_WIDTH));
	}

	
}


void CIMPhotoBtn::SetUserState( DWORD dwUserStatus )
{
	m_dwUserStatus = dwUserStatus;
	if (GetSafeHwnd())
	{
		Invalidate();
	}
}

void CIMPhotoBtn::DrawEffect( CDC* pDC,UINT style )
{
	CRect rcThis;
	GetClientRect(rcThis);

	if (m_dwUserStatus == IM_USER_STATUS_OFFLINE  ||( m_dwUserStatus == IM_USER_STATUS_HIDE && !m_bDrawSelfState))
	{
		Gdiplus::Image* pImg = rcThis.Width() < PHOTO_NORMAL_WIDTH ? m_pBmpSmallGray:m_pBmpGray;

		if (pImg != NULL)
		{
			//pDC->FillSolidRect(rcThis, RGB(0,0,0));
			Graphics gc(pDC->GetSafeHdc());
			gc.DrawImage(pImg, 0,0, pImg->GetWidth(), pImg->GetHeight());
		}

	}

	if (style == DB_HOVER && ((m_bFocusStyle & EFF_FOCUS_DRAW_FRAME) != 0))
	{
		//draw photo frame
		Graphics gc(pDC->GetSafeHdc());
		Gdiplus::Image* pImgFrame = rcThis.Width() < PHOTO_NORMAL_WIDTH? m_spImgFrameSmall:m_spImgFrame;
		if (!pImgFrame)
		{
			gc.DrawImage(pImgFrame, 0,0, pImgFrame->GetWidth(), pImgFrame->GetHeight());
		}
	}

	//draw big frame
	if (rcThis.Width() > PHOTO_SMALL_WIDTH)
	{
		CRect rcFrame(rcThis);
		CRgn rgn, rgn1,rgn2;
		rgn.CreateRoundRectRgn(rcFrame.left,rcFrame.top, rcFrame.Width(),rcFrame.Height(), 4,4);
		rcFrame.DeflateRect(1,1,0,0);
		rgn1.CreateRoundRectRgn(rcFrame.left,rcFrame.top, rcFrame.Width(),rcFrame.Height(), 4,4);
		rcFrame.DeflateRect(1,1,0,0);
		rgn2.CreateRoundRectRgn(rcFrame.left,rcFrame.top, rcFrame.Width(),rcFrame.Height(), 4,4);
		CBrush brushFrame, brushFrame1, brushFrame2;
		brushFrame.CreateSolidBrush(RGB(224,224,224));
		brushFrame1.CreateSolidBrush(RGB(170,205,192));
		brushFrame2.CreateSolidBrush(RGB(255,255,255));
		pDC->FrameRgn(&rgn, &brushFrame,  1,1);
		pDC->FrameRgn(&rgn1, &brushFrame1,  1,1);
		pDC->FrameRgn(&rgn2, &brushFrame2,  1,1);

	}
	
	CMultiBitmapButtonEx::DrawEffect(pDC, style);

}

void CIMPhotoBtn::LoadDefaultBitmap()
{
	if (m_spBmpPhotoCommon == NULL)
	{
		ASSERT(FALSE);
	}

	m_pBmpPhoto = m_spBmpPhotoCommon;
	m_pBmpSmallPhoto = m_spBmpSmallCommon;
	m_pBmpGray = m_spBmpGray;
	m_pBmpSmallGray = m_spBmpSmallGray;

}


void CIMPhotoBtn::DrawButton( CDC *pDC,UINT style )
{
// 	if (!m_bUseCommonPhoto)
// 	{
// 		CMultiBitmapButtonEx::DrawButton(pDC, style);
// 		return;
// 	}

	CRect rcClient;
	GetClientRect(rcClient);

	if (!m_pBmpPhoto)
	{
		pDC->FillSolidRect(rcClient, RGB(255,255,255));
		return;
	}
	
	CWnd* pParWnd = GetParent();
	if(!pParWnd)
	{
		ASSERT(FALSE);
		return;
	}

	CRect rect(rcClient);

	CRect * p = 0;
	CRect*& refp = p;
	CMemDC memDC(pDC,rcClient.Width(),rcClient.Height());//首先绘到内存DC,然后再绘到pDC,避免显砾
	memDC.SetBkMode(OPAQUE);
	CRect rcInPar;
	GetWindowRect(rcInPar);
	pParWnd->ScreenToClient(rcInPar);
	CClientDC dcPar(pParWnd);
	memDC.BitBlt(0,0, rcClient.Width(), rcClient.Height(), &dcPar, rcInPar.left, rcInPar.top, SRCCOPY);

	Gdiplus::Image* pImgDraw = rcClient.Width() < PHOTO_NORMAL_WIDTH? m_pBmpSmallPhoto:m_pBmpPhoto;
	
	BOOL bDrawCornerIcon = FALSE;
	if ((m_dwUserStatus != IM_USER_STATUS_OFFLINE) && !(m_dwUserStatus == IM_USER_STATUS_HIDE && !m_bDrawSelfState) && pImgDraw)
	{
		Graphics gc(memDC.GetSafeHdc());
		gc.DrawImage(pImgDraw, 0,0, pImgDraw->GetWidth(), pImgDraw->GetHeight());
		//draw corner icon
		if (m_bFocusStyle & USER_ITEM_STYLE_CORNER_STATE)
		{
			if (m_dwUserStatus == IM_USER_STATUS_ONLINE)
			{
				bDrawCornerIcon = FALSE;
			}
			else
			{
				bDrawCornerIcon = TRUE;
			}
		}	
	}

	DrawEffect(&memDC, style);
	if (bDrawCornerIcon)
	{
		stu_UserStateImage stateImg;
		g_pChatMgr->m_UserOperator.GetCornerStateImage(m_dwUserStatus, stateImg );
		Gdiplus::Image* pImgCorner = rcClient.Width() < PHOTO_NORMAL_WIDTH? stateImg.pImageSmall:stateImg.pImage;
		if (pImgCorner)
		{
			Graphics gc(memDC.GetSafeHdc());
			CRect rcCorner(rcClient);
			rcCorner.left = rcCorner.right - pImgCorner->GetWidth();
			rcCorner.top = rcCorner.bottom - pImgCorner->GetHeight();
			gc.DrawImage(pImgCorner, rcCorner.left,rcCorner.top, rcCorner.Width(), rcCorner.Height());
		}
	}

	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);
}

void CIMPhotoBtn::InitStaticImage()
{
	if (m_spBmpPhotoCommon == NULL)
	{

/*
		CString strFile = GetFullFilePath(UsertItem_Btn_Pic);
		BSTR bstrTmp = strFile.AllocSysString();
		m_spBmpPhotoCommon = Bitmap::FromFile(bstrTmp);
		SysFreeString(bstrTmp);
		m_spBmpGray = CreateGrayImage(m_spBmpPhotoCommon);

		m_spBmpSmallCommon = CreateResizedImage(m_spBmpPhotoCommon, CRect(0,0,PHOTO_SMALL_WIDTH,PHOTO_SMALL_WIDTH));
		m_spBmpSmallGray = CreateResizedImage(m_spBmpGray, CRect(0,0, PHOTO_SMALL_WIDTH,PHOTO_SMALL_WIDTH));
*/
		stu_UserImage imgInfo;
		g_pChatMgr->m_UserOperator.GetUserImage(0, imgInfo);
		m_spBmpPhotoCommon = imgInfo.pImgBig;
		m_spBmpSmallCommon = imgInfo.pImgSmall;
		m_spBmpSmallGray = imgInfo.pImgSmallGray;
		m_spBmpGray = imgInfo.pImgBigGray;
	
		if (!m_spImgFrame)
		{
			CString strTmp = GetFullFilePath(UsertItem_Btn_PhotoFrame);
			BSTR bstrTmp = strTmp.AllocSysString();
			m_spImgFrame = Bitmap::FromFile(bstrTmp);

			SysFreeString(bstrTmp);
			ASSERT(m_spImgFrame);
			m_spImgFrameSmall = CreateResizedImage(m_spImgFrame, CRect(0,0,PHOTO_SMALL_WIDTH,PHOTO_SMALL_WIDTH));

		}
	
	}
}

void CIMPhotoBtn::ReleaseStaticImage()
{
	if (m_spImgFrame)
	{
		delete m_spImgFrame;
		delete m_spImgFrameSmall;
// 		delete m_spBmpPhotoCommon;
// 		delete m_spBmpSmallCommon;
// 		delete m_spBmpGray;
// 		delete m_spBmpSmallGray;
	}
	
}

void CIMPhotoBtn::LoadUserImages( ULONG nImgId, BOOL bAutoStretch /*= TRUE*/ )
{
	stu_UserImage imgInfo;
	BOOL bRet = g_pChatMgr->m_UserOperator.GetUserImage(nImgId, imgInfo);
	if (!bRet)
	{
		//no img
		ASSERT(FALSE);
		g_pChatMgr->m_UserOperator.GetUserImage(0, imgInfo);
	}

	m_pBmpPhoto = imgInfo.pImgBig;
	m_pBmpGray = imgInfo.pImgBigGray;
	m_pBmpSmallGray  = imgInfo.pImgSmallGray;
	m_pBmpSmallPhoto  = imgInfo.pImgSmall;
	
}

LRESULT CIMPhotoBtn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_LBUTTONDBLCLK)
	{
		GetParent()->SendMessage(WM_LBUTTONDBLCLK, wParam, lParam);
	}
	if (message == WM_LBUTTONDOWN)
	{
		GetParent()->SendMessage(WM_LBUTTONDOWN, wParam, lParam);
	}
	if (message == WM_KILLFOCUS)
	{
		GetParent()->Invalidate();
	}
	if (message == WM_SETFOCUS)
	{
		GetParent()->Invalidate();
	}
	if (message == WM_MOUSEMOVE)
	{
		GetParent()->SendMessage(WM_MOUSEMOVE, wParam, lParam);
	}
	return CMultiBitmapButtonEx::WindowProc(message, wParam, lParam);
}

void CIMPhotoBtn::SetFocusStyle( BOOL bFocusStyle )
{
	this->ActiveEffect(0, 0, FALSE);
	m_bFocusStyle = bFocusStyle;
	if ((m_bFocusStyle & EFF_FOCUS_DRAW_FRAME) == 0)
	{
		this->ActiveEffect(0, 0, FALSE);
	}
	
}

void CIMPhotoBtn::EnableFrameInfo( BOOL bEnable )
{
	m_bEnableFrameInfo = bEnable;
}

BOOL CIMPhotoBtn::PlayEffect( int nEffectType, BOOL bPlay /*= TRUE*/ )
{
	if (nEffectType == 1)
	{
		::PostMessage(g_hFrameUserInfo, MSG_USER_FRAME_ENTER, 0,0);
	}
	return TRUE;
}

void CIMPhotoBtn::ChangeUserImages( ULONG nImgId, BOOL bAutoStretch /*= TRUE*/ )
{
	LoadUserImages(nImgId, bAutoStretch);
	Invalidate(TRUE);
}


void CIMPhotoBtn::Invalidate( BOOL bErase /*= TRUE*/ )
{
	CWnd* pParWnd = GetParent();
	if(!pParWnd)
	{
		ASSERT(FALSE);
		return;
	}
	CRect rcInPar;
	GetWindowRect(rcInPar);
	pParWnd->ScreenToClient(rcInPar);
	bIntervalCallDraw = TRUE;
	pParWnd->InvalidateRect(rcInPar);
	pParWnd->UpdateWindow();
	bIntervalCallDraw = FALSE;
	CMultiBitmapButtonEx::Invalidate(TRUE);
}