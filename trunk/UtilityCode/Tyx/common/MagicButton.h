#pragma once
#include "afxwin.h"

#include "Mutibitmapbuttonex.h"


//////////////////////////////////////////////////////////////////////////

using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////
class CDecorateFrame :public CMiniFrameWnd
{
public:
	CDecorateFrame();
	virtual ~CDecorateFrame();
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void StartEffectTimer(BOOL bPlay = TRUE);
	
	int  m_nEffectCounting;

	Bitmap* m_pDecBmp;
	UINT m_nBmpHeight;
	UINT m_nBmpWidth;
};

//////////////////////////////////////////////////////////////////////////

class CMagicButton : //button for message
	public CMultiBitmapButtonEx
{
public:
	CMagicButton(void);
public:
	virtual ~CMagicButton(void);

protected:
	CDecorateFrame* m_pFmDecorate;
public:
	DECLARE_MESSAGE_MAP()
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnMove(int x, int y);
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	LONG RelayEvent(UINT message);
	void Relayout();
public:
	afx_msg void OnDestroy();

	//0 = in , 1 = out
	virtual BOOL PlayEffect(int nEffectType, BOOL bPlay = TRUE);
	
};

//////////////////////////////////////////////////////////////////////////