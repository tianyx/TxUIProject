/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxListHeader.h
	file base:	TxListHeader
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include <vector>


#define TX_HEADERSTATE_NORMAL 0x0
#define TX_HEADERSTATE_HOVER  0x1
#define	TX_HEADERSTATE_LDOWN  0x2

class CTxScrollWnd;
struct ST_LISTHEADERITEM
{
	int nWidth;
	int nMinWidth;
	CString strText;
	WPARAM wparam;
	BOOL bAutoWidth;
	ST_LISTHEADERITEM()
	{
		bAutoWidth = TRUE;
		nWidth = 10;
		nMinWidth = 10;
		wparam = NULL;
	}

};


typedef std::vector<ST_LISTHEADERITEM> VECHEADERITEMS;

class CTxScrollWnd;
class CTxListHeader
{
public:
	CTxListHeader(CTxScrollWnd* pParent);
	~CTxListHeader(void);

	void EnableHeader(BOOL bEnable = TRUE){m_bEnabled = bEnable;}
	void SetHeight(int nHeight);
	void SetTotalWidthFixed(BOOL bFit){m_bMaxWidthFixed = bFit;}
	void LoadBkImage(LPCTSTR szBkImage, ENUM_TXBMPSTRETCHTYPE nStretchType = TXBMP_STRETCH_MID_LR);

	int GetHeaderWidth(); //return sum
	int GetHeaderWidth(int nItem);
	int GetHeaderHeight(){return m_nBarHeight;}
	BOOL IsEnable(){return m_bEnabled;}
	void AddHeaderItem(ST_LISTHEADERITEM& info);
	void AddHeaderItem(CString strText);
	BOOL IsFitView(){return m_bMaxWidthFixed;}
	void SetViewWidth(int nWidth);

	void RelayMsg(MSG* pMsg);
	virtual void Draw(CDC* pDC);

protected:
	int HitItemRightBars(CPoint& ptIn);
	void OnMouseLeave(WPARAM wparam, LPARAM lparam);
	void OnLButtonDown(WPARAM wparam, LPARAM lparam);
	void OnLButtonUp(WPARAM wparam, LPARAM lparam);
	void OnMouseMove(WPARAM wparam, LPARAM lparam);
	void OnKillFocus(WPARAM wparam, LPARAM lparam);
	void OnSize(WPARAM wparam, LPARAM lparam);
	void OnCreate();
	void RecalcSize();
	void NotifyRedraw();

	void DrawDragRect(CRect* pRcBefore, CRect* pRcAfter, int nMode = 1);


	//setting
	int m_nMinBarWidth;
	BOOL m_bCaptured;
	CPoint m_ptPreDrag;
	CPoint m_ptOrgDrag;
	CRect m_rcDragRect;
	CRect m_rcPre;
	int m_nItemHited;

	VECHEADERITEMS m_vItems;
	CTxScrollWnd* m_pParent;

	int m_nBarHeight;
	BOOL m_bEnabled;
	BOOL m_bMaxWidthFixed; //auto fit view width, else notify parent view width need change
	int m_nViewWidth; //used only when m_bFitView is TRUE

	//draw param
	Bitmap* m_pImgBk;
	CSize m_szImgBk;
	ENUM_TXBMPSTRETCHTYPE m_nBkStretchType;
	DWORD m_clrTextPen;
	DWORD m_clrTextbrush;
	Gdiplus::Font* m_pFont;
	HCURSOR m_hArrowNW;
	HCURSOR m_hNormal;

	CBitmap m_bmpCache;
};
