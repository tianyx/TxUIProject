/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipMonView.h
	file base:	ClipMonView
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include "TxListCtrl.h"
#include "TrCommonInfoMgr.h"

// CClipMonView
#define  TRANSSTATENUM 7
struct ST_LISTDRAWPARAM
{
	ARGB m_clrText[TRANSSTATENUM];
	ARGB bkColor;
	int nItemHeight;
	int nFontHeight;
	CString strFontFace;
	Gdiplus::Font* pFont;
	BOOL bFontBlod;
	ST_LISTDRAWPARAM()
	{
		nItemHeight = 46;
		bkColor = g_globalInfo.viewSetting.clrBk;
		nItemHeight = g_globalInfo.viewSetting.nItemHeight;
		nFontHeight = g_globalInfo.viewSetting.nFontHeight;
		strFontFace = g_globalInfo.viewSetting.strFontFace;
		pFont = NULL;
		bFontBlod = g_globalInfo.viewSetting.bFontBlod;
	}
};



class CClipMonView : public CTxListCtrl
{
	DECLARE_DYNAMIC(CClipMonView)

public:
	CClipMonView();
	virtual ~CClipMonView();

public:
	virtual void OnDraw(CDC *pDC) ;	


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CBitmap m_bmpCache;
	//CBitmap m_bmpBackBuffer;
	Bitmap*	m_pBmpBackBuffer;
	Bitmap* m_pBmpRemainTime;
	CSize m_szbmpCache;
	CSize m_szbmpBackBuffer;
	CRect m_rcViewBkBuffer;
	UINT m_nItemSize;

	BOOL m_bUseBkBuffer;
	
	ST_LISTDRAWPARAM m_drawParam;
	ST_TEMPLATEVIEW_CONFIG m_ColSetting;
	virtual void SetScrollViewSize(CSize szView);
	void ReDrawBkBuffer();

protected:
	void ReSizeBkBuffer(CSize& szIn);

	void ReDrawRemainTime(CDC* pDC);

	CMutex m_BkBufferlock;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};


