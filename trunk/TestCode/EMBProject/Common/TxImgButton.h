/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxImgButton.h
	file base:	TxImgButton
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once


// CTxImgButton
#include "GDIDrawFunc.h"
#include <vector>
using namespace Gdiplus;

#define TX_BTNSTATE_NORMAL 0x0
#define TX_BTNSTATE_HOVER  0x1
#define	TX_BTNSTATE_LDOWN  0x2
#define TX_BTNSTATE_GRAY   0x4

#define TX_BTNSTATE_PRESSED (0x1|0x2)

typedef std::vector<Bitmap*> VECTXBITMAPS;



struct ST_TXBTNINFO 
{
	VECTXBITMAPS m_vBmps; // do not delete inside
	int nCurrBmps;
	int nBmpWidth;
	int nBmpHeight;
	int nSubBmpWidth;
	ENUM_TXBMPSTRETCHTYPE nStretchType;
	CRect rcEdge;  //edge to stretch.

	CFont* m_pFont;

	ST_TXBTNINFO()
	{
		nCurrBmps = -1;
		nBmpWidth = 0;
		nBmpHeight = 0;
		nSubBmpWidth = 0;
		nStretchType = TXBMP_STRETCH_NONE;
		rcEdge = CRect(5,5,5,5);
		m_pFont = NULL;
	}


};

class CTxImgButton : public CButton
{
	DECLARE_DYNAMIC(CTxImgButton)

public:
	CTxImgButton();
	virtual ~CTxImgButton();

public:
	BOOL LoadBitmap(LPCTSTR szFileIn, ENUM_TXBMPSTRETCHTYPE sTypeIn);
	BOOL AddBitmap(LPCTSTR szFileIn);
	int GetCurrImgIdx(){return m_txInfo.nCurrBmps;}
	int SetCurrImgIdx(int nIdx);
private:
	UINT GetBtnState();
	void CheckRedraw();
private:
	ST_TXBTNINFO m_txInfo;
	UINT m_nBtnState;
	BOOL m_bMouseOver;
	BOOL m_bSetLeaveTrack;
	BOOL m_bPressed;
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


