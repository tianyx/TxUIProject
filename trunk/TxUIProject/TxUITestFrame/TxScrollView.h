#pragma once

#include "TxObjectBase.h"
// CTxScrollView

#define TXBAR_HORZ 0x1
#define TXBAR_VERT 0x2
#define TXBAR_BOTH 0x3

#define TXBAR_BTN_UP		0x10
#define TXBAR_BTN_MID		0x20
#define TXBAR_BTN_DOWN		0x40
#define TXBAR_BTN_BKUP		0x80
#define TXBAR_BTN_BKDOWN	0x100
#define TXBAR_BTN_NOT_MID	(TXBAR_BTN_UP|TXBAR_BTN_DOWN|TXBAR_BTN_BKUP|TXBAR_BTN_BKDOWN)

class CTxScrollView : public CTxObjectBase
{
	DECLARE_DYNAMIC(CTxScrollView)

public:
	CTxScrollView();
	virtual ~CTxScrollView();

protected:
	int m_nThumbWidthH; //width of horizon thumb btn,
	int m_nThumbWidthV; //width of vertical thumb btn,

	CSize	m_szBarbtn;
	DOUBLE	m_dPixPerStepBarMoveH; //10; scrollbar move pix when btn click per time
	DOUBLE	m_dPixPerStepBarMoveV; //10; scrollbar move pix when btn click per time

	CSize	m_szViewSize;
	CPoint	m_ptViewOffset; //current view pt in window pt(0,0)
	const CSize	m_szPixPerStepView; //default 20; scroll pixs of view  per time, const value
	UINT m_nBarStyle; //
	UINT m_nHitState;
	UINT m_nEnabledStyle;

	BOOL m_bScrollIng; //start scroll
	CPoint m_ptScrollLast; //last scroll
	//////////////////////////////////////////////////////////////////////////
	virtual void Draw(CDC* pDC);
	//sub class call this func to draw view
	virtual void DrawView(CDC* pDC);
	//interface
	virtual HRESULT DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam);

public:
	BOOL SetViewSize(CSize& szViewInOut);
	inline CSize GetViewSize(){return m_szViewSize;}
	BOOL ScrollView(CPoint& ptViewInOut);
	CRect GetClientViewRect(); //get view client rect;
	CPoint GetViewOffsetPoint(){return m_ptViewOffset;}
	CRect GetInWndViewRect();
	void SetBarStyle(UINT styleIn){m_nEnabledStyle = styleIn;};
private:
	BOOL CalculateScrollBar();
	UINT HitTestBar();
	BOOL DoScrollBarMove();
	BOOL DoScrollBarClick();
	void BeginScrolling();
	void EndScrolling();
	//CRect GetInWndClientRect(); //not include bar rect.
	void AdJustViewOffSet();
	int GetScrollBarUpBkWidth(UINT nBarStyle);
	
	//pass TXBAR_HORZ or TXBAR_VERT
	BOOL GetBarRect(UINT nBarStyleIn, CRect& rcUp, CRect& rcUpBk, CRect& rcThumb, CRect& rcDownBk, CRect& rcDown);



};


