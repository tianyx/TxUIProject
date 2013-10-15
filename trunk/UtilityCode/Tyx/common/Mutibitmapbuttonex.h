

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBStretchImage.h"
#include "GdiPlusNewHeader.h"

// BitmapButtonEx.h : header file
//
#define  MSG_CHILD_RELAY_EVENT WM_USER + 1208

/////////////////////////////////////////////////////////////////////////////
// CBitmapButtonExwindow

class CMultiBitmapButtonEx: public CButton
{
// Construction
public:
	CMultiBitmapButtonEx();

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMutiBitmapButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;
	COLORREF SetTextColor(COLORREF clrText);
	void LoadBitmaps(LPCTSTR lpszBitmaps, BOOL bAutoStretch = TRUE, BOOL bEdgeStretch = FALSE);
	void LoadBitmaps(CBitmap& bitmap, BOOL bAutoStretch = TRUE);

	void LoadBitmaps(LPCTSTR lpszBmpNormal, LPCTSTR lpszBmpSel,
		LPCTSTR lpszBmpHover, LPCTSTR lpszBmpDisable, 
		BOOL bAutoStretch = TRUE);
	//{{added by tian_yx (15:1:2010) for
	virtual void LoadSingleBitmaps(LPCTSTR lpszBitmaps, BOOL bAutoStretch = TRUE);
	int AddSingleBitmaps(LPCTSTR lpszBitmaps);
	//end}}
	
	int  AddBitmaps(LPCTSTR lpszBitmaps);
	BOOL SetCurrentBitmapIdx(int nIdx);
	int GetCurrentBitmapIdx();

	//
	void LoadBitmaps(Gdiplus::Image* pImagIn);

	BOOL ActiveEffect(BOOL bActive = TRUE);
	BOOL ActiveEffect(int nFrameWidth, int nRound = 0, BOOL bActive = TRUE);

	BOOL ActiveEffect2(BOOL bActive = TRUE);

	virtual void DrawEffect(CDC* pDC,UINT style);

	//draw a png file on btn
	//virtual void DrawEffect2(CDC* pDC, UINT style);

	//
	void ChangeBitmaps(LPCTSTR lpszBitmaps, BOOL bAutoStretch = TRUE, BOOL bEdgeStretch = FALSE);
	void ChangeBitmaps(Gdiplus::Image* pImgIn);
	virtual ~CMultiBitmapButtonEx();
	virtual void Invalidate(BOOL bErase = TRUE);

protected:
	//clean all bitmap;
	void Reset();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMutiBitmapButtonEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:

	//max bitmap 6
	DBStretchImage m_vImageNormal[5];	//正常状态
	DBStretchImage m_vImageSel[5];		//按下状态
	DBStretchImage m_vImageHover[5];	//Hover状态
	DBStretchImage m_vImageDisable[5];	//Disable状态

	int m_nCurImgIdx;
	int m_nImgCount;
	
	BOOL m_bMouseOver;	//鼠标是否位于按钮上方

protected:
	BOOL m_bHasToolTip;
	void DrawCheckButton(CDC *pDC, UINT style);
	BOOL m_bCheckBtn;
	BOOL m_bCheck;
	BOOL m_bAutoStretch;
	CFont m_font;
	COLORREF m_clrText;
	BOOL m_bPushed;
	BOOL m_bDrawShadowText;
	UINT m_ntextStyle;
	virtual void DrawButton(CDC *pDC,UINT style);


public:
	//added by tianyx 2009-5-25--17:54 for
	//for drop btns
	CMultiBitmapButtonEx* m_pNaborBtn;
	CMultiBitmapButtonEx* m_pNaborBtn2; //for drop btn;
	int m_nNaborSetState; 
	BOOL m_bNaborCalledReDraw;
	void DrawWithNabourState(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//ended by tianyx
	
	// for effect
	BOOL m_bEffect;
	int m_nEffectCounting;
	BOOL m_bEffect2; //enable effect2
	int m_nEffectFrameWidth;
	int m_nEffectRound;

	//0 = in , 1 = out
	virtual BOOL PlayEffect(int nEffectType, BOOL bPlay = TRUE);


private:
	static BOOL m_gbLoading;

	//static Gdiplus::Image m_gImgEffect2;
	Gdiplus::Image* m_pImgUserPhoto;  //for user photo;

	BOOL m_bEdgeStretch; //make stretch edge
public:
	void PreSetDividPos(int nFirstPos, int nSecPos);
	void SetDrawWithText(BOOL bWithText, int nOffSet);
	BOOL m_bDrawWithText;
	int m_nTextOffSet;
	int m_nFirstDivPos;
	int m_nSecDivPos;

	HWND m_hRelayWnd;
	void SetRelayHwnd(HWND hwndIn);
#ifdef _DEBUG
	CString strLoadedImg;
#endif // _DEBUG

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
//	afx_msg void OnBnKillfocus();

	//return  0,1,2
	int GetCheckStatus();
	void SetCheckStatus(int nCheck);

	void SetCustomFont(CFont& fontIn);
	void SetTextAlignStyle(UINT nStyleIn){m_ntextStyle = nStyleIn;}
public:
	afx_msg void OnNcDestroy();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
