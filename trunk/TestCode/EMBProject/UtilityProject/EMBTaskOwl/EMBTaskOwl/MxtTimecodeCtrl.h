#pragma once
//#pragma title("Timecode Custom Control Definitions")
#include "CcType_Timecode.h"

/////////////////////////////////////////////////////////////////////////////
// CMxtValueEdit_ window

using namespace Coco;

class CMxtTimecodeCtrl;

class CMxtValueEdit_ : public CEdit
{
// Construction
public:
	inline CMxtValueEdit_()	:m_nMinValue(0),m_nMaxValue(100),m_pParent(NULL){};

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMxtValueEdit_)
	//}}AFX_VIRTUAL

// Implementation
public:
	CMxtTimecodeCtrl* m_pParent;
	void SetParent(CMxtTimecodeCtrl* pParent);

public:
	int m_nMaxValue;
	int m_nMinValue;
	virtual void IncOne();
	virtual void DecOne();
	virtual int GetValue();
	virtual int SetValue(int nValue,int nSilent=FALSE);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMxtValueEdit_)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CMxtHourEdit_ : public CMxtValueEdit_
{
public:
	CMxtHourEdit_() : m_nSign(1) {}; 
	int m_nSign;
	int GetValue();
	int SetValue(int nValue,int nSilent=FALSE,BOOL bIs24Format = TRUE);
	void IncOne();
	void DecOne();
protected:
	//{{AFX_MSG(CMxtHourEdit_)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//CMxtTimecodeCtrlwindow

class CMxtTimecodeCtrl : public CWnd
{
public:
// Construction
public:
	CMxtTimecodeCtrl(BOOL bCanNegative = FALSE);

public:
	static BOOL Register(LPCSTR lpClassName="CMxtTimecodeCtrl");

// Attributes
public:
	BOOL m_bEnabled; // indication of the ctrl' enable status 

	CMxtHourEdit_  m_HourEdit;
	CMxtValueEdit_ m_MinuteEdit;
	CMxtValueEdit_ m_SecondEdit;
	CMxtValueEdit_ m_FrameEdit;
	
	CMxtValueEdit_*  m_Edit[4];// HOUR,MINUTE,SECOND,FRAME
	
	CRect	m_rcEdit[4];   // for display HOUR,MINUTE,SECOND,FRAME
	
	CRect	m_rcDot1;    // for display the first ':'
	CRect	m_rcDot2;    // for display the second ':'
	CRect	m_rcDot3;    // for display the third ':'

// Operations
public:
	void _fastcall EnableNegative(BOOL bCanNegative = FALSE);
	BOOL HasFocus();
	friend class CMxtValueEdit_;	
	friend class CMxtHourEdit_;	

	int GetTimecode();
	void SetTimecode(short nHour, short nMinute, short nSecond, short nFrame=0, int nSign = 1, BOOL bIs24Format = TRUE);
	void SetTimecode(int timecode);
	inline void SetTimecode(timecode_t& t)	{ SetTimecode(t.m_nFrames);};

	void InitEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMxtTimecodeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bCanNegative;
	int OnValueChanged();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMxtTimecodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg LONG OnSetFont(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()

	void OnChildChar(UINT nChar, UINT nRepCnt, UINT nFlags, CMxtValueEdit_& child);

private:
	// Call back functions
	static LRESULT CALLBACK TimecodeWndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
};


void DDX_Timecode(CDataExchange*  pDX, int nDC, int& tm);
inline void DDX_Timecode(CDataExchange*  pDX, int nDC, Coco::timecode_t& tm)
	{
		DDX_Timecode(pDX,nDC,tm.m_nFrames);
	}
