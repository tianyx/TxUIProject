/********************************************************************
	created:	2011/08/10
	created:	10:8:2011   11:32
	filename: 	e:\Source\EZIM\Src\EZMessenger\common\IMPhotoBtn.h
	file path:	e:\Source\EZIM\Src\EZMessenger\common
	file base:	IMPhotoBtn
	file ext:	h
	author:		tian_yuanxin
	
	purpose:	
*********************************************************************/
#pragma once
#include "Mutibitmapbuttonex.h"
#include "GdiPlusNewHeader.h"

#define EFF_FOCUS_DRAW_FRAME 0x01
#define MSG_USER_FRAME_ENTER		WM_USER + 2011


class CIMPhotoBtn :
	public CMultiBitmapButtonEx
{
public:
	CIMPhotoBtn(void);
public:
	virtual ~CIMPhotoBtn(void);

public:
	virtual void DrawEffect(CDC* pDC,UINT style);
	virtual void DrawButton( CDC *pDC,UINT style );
	virtual void Invalidate( BOOL bErase = TRUE );


	virtual void LoadSingleBitmaps(LPCTSTR lpszBitmaps, BOOL bAutoStretch = TRUE);
	virtual void LoadUserImages(ULONG nImgId, BOOL bAutoStretch = TRUE);
	void ChangeUserImages(ULONG nImgId, BOOL bAutoStretch = TRUE);
	void LoadDefaultBitmap();

	void SetUserState(DWORD dwUserStatus);
	void SetFocusStyle(BOOL bFocusStyle);
	void EnableFrameInfo(BOOL bEnable);

	virtual BOOL PlayEffect(int nEffectType, BOOL bPlay = TRUE);

public:
	BOOL bIntervalCallDraw;
	DWORD m_dwUserStatus;
	Gdiplus::Bitmap* m_pBmpGray;
	Gdiplus::Image* m_pBmpSmallGray;
	Gdiplus::Bitmap* m_pBmpPhoto;
	Gdiplus::Image* m_pBmpSmallPhoto;

	BOOL m_bUseCommonPhoto;	
	BOOL m_bFocusStyle;
	BOOL m_bEnableFrameInfo;
	BOOL m_bDrawSelfState;


private:
	static Gdiplus::Bitmap* m_spBmpGray;
	static Gdiplus::Bitmap* m_spBmpSmallGray;
	static Gdiplus::Bitmap* m_spBmpPhotoCommon;
	static Gdiplus::Bitmap* m_spBmpSmallCommon;
	static Gdiplus::Bitmap * m_spImgFrame;
	static Gdiplus::Bitmap * m_spImgFrameSmall;
public:
	//call this in initInstacne
	static void InitStaticImage();
	//call this in exitInstance
	static void ReleaseStaticImage();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
