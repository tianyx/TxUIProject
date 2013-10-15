/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxScrollBar.h
	file base:	TxScrollBar
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

//showstate for scroll bar
#define TXBARSHOW_READY			0x1
#define TXBARSHOW_HOVER			0x2
#define TXBARSHOW_SHOWING		0x3
//bar part num
#define TXBARPART_NONE	0xff00
#define TXBARPART_MIN	0
#define TXBARPART_MINBK	1
#define TXBARPART_MID	2
#define TXBARPART_BKMAX	3
#define TXBARPART_MAX	4
//hot state
#define TXBARSTATE_NORMAL	0x0
#define TXBARSTATE_LDOWN	0x1
#define TXBARSTATE_HOVER	0x2
#define TXBARSTATE_PRESSED	(0x1|0x2)
#define TXBARSTATE_GRAYED	0x4

#define TXBARDRAWSEC_BTNMIN		0
#define TXBARDRAWSEC_BTNMAX		1
#define TXBARDRAWSEC_MID		2
#define TXBARDRAWSEC_BK		3

struct ST_SBARBMPSECTION
{
	UINT vnBarStartPos[4];
	UINT nBarWidth;
	UINT nImgWidth;
	UINT nImgHeight;
	CRect vrcImgNormal[5];

	ST_SBARBMPSECTION()
	{
		vnBarStartPos[TXBARDRAWSEC_BTNMIN] = 0;
		vnBarStartPos[TXBARDRAWSEC_BTNMAX] = g_globalInfo.scrollbarParam.nScrollBarWidth;
		vnBarStartPos[TXBARDRAWSEC_MID] = vnBarStartPos[TXBARDRAWSEC_BTNMAX] + g_globalInfo.scrollbarParam.nScrollBarWidth;
		vnBarStartPos[TXBARDRAWSEC_BK] = vnBarStartPos[TXBARDRAWSEC_MID] + 20;
		nBarWidth = vnBarStartPos[TXBARDRAWSEC_BTNMAX];
		nImgWidth = 0;
		nImgHeight = 0;
	}

};

class CTxScrollWnd;
class CTxScrollBar
{
public:
	CTxScrollBar();
	~CTxScrollBar(void);

public:
	BOOL m_bBarEnabled;
	UINT m_nBarStyle;
	SCROLLINFO m_slInfo;
	CRect m_rcBarRect;
	Bitmap* m_pBarBmp;
	ST_SBARBMPSECTION m_BarBmpSec; //splitte bar image
	UINT m_nBarShowState;
	int m_nBarHotPart;
	UINT m_nBarState;
	CTxScrollWnd* m_pParent;
	CTxScrollBar* m_pOtherbar;
	CRect m_rcBar; //rect in parent
	CRect m_vrcPart[5];
	BOOL m_bCaptured;
	CPoint m_ptDragPre;
	int m_nMinPixScrollPerBtnClick;
	int m_nScrollPixPerWhell;
	int m_nPixPerScrollBkClick;
	int m_nPixsPerScrollBarMove;
	CSize m_szView;


private:
	void ReCalSize();
	void ReCalScrollPartSize();
	void CheckHotPart();
	int	 GetHotPart();
	int GetBarPixsPerClientArea();
	void NotifyRefresh(int nHotPart = TXBARPART_NONE);
	void OnMouseLeave(WPARAM wparam, LPARAM lparam);
	void OnLButtonDown(WPARAM wparam, LPARAM lparam);
	void OnLButtonUp(WPARAM wparam, LPARAM lparam);
	void OnMouseMove(WPARAM wparam, LPARAM lparam);
	void OnKillFocus(WPARAM wparam, LPARAM lparam);
	void OnMouseWHeel(WPARAM wparam, LPARAM lparam);
	BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);


public:
	BOOL IsShowing();
	BOOL GetBarWidth(){return m_BarBmpSec.nBarWidth;}
	void DrawBar(CDC* pDC);
	void SetViewSize(const CSize& szView);
	void SetParent(CTxScrollWnd* pWnd){m_pParent = pWnd;}
	void SetOtherBar(CTxScrollBar* pBar){ASSERT(pBar != this); m_pOtherbar = pBar;}
	//SB_VERT or SB_HORZ
	void SetBarStyle(UINT nBarStyle){m_nBarStyle = nBarStyle;}
	//set nMinStartPos = 0 to easy use;
	BOOL LoadBarBmp(LPCTSTR szFileIn, UINT nMinStartPos, UINT nMaxStartPos, UINT nMidStart, UINT nBkStart);
	void EnableBar(BOOL bEnable = TRUE){m_bBarEnabled = bEnable;}
	BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo);
	BOOL SetScrollPos(int nPos, BOOL bRedraw = TRUE);
	int GetScrollPos(){return m_slInfo.nPos;}
	HRESULT RelayMsg(MSG* pMsg);
	HRESULT PostRelayMsg(UINT message, WPARAM wParam, LPARAM lParam);

};
