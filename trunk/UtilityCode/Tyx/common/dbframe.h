#if !defined(AFX_DBFRAME_H__D9ED011F_CCAA_4631_99E4_65EB2D35740A__INCLUDED_)
#define AFX_DBFRAME_H__D9ED011F_CCAA_4631_99E4_65EB2D35740A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBFrame.h : header file
//

#include "DBButton.h"	// Added by ClassView
#include "DBRestoreButton.h"	// Added by ClassView
#include "DBStretchImage.h"	// Added by ClassView

#define ID_SC_BASE		65500
#define ID_SC_MIN		ID_SC_BASE - 1
#define ID_SC_MAX		ID_SC_BASE - 2
#define ID_SC_RESTORE	ID_SC_BASE - 3
#define ID_SC_CLOSE		ID_SC_BASE - 4

/////////////////////////////////////////////////////////////////////////////
// CDBFrame frame

class CDBFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CDBFrame)
protected:
	CDBFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBFrame)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	void EnableMinSizeBtn(BOOL bEnable = TRUE);
protected:
	virtual ~CDBFrame();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	// Generated message map functions
	//{{AFX_MSG(CDBFrame)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual afx_msg void OnSCClose();
	afx_msg void OnSCMax();
	afx_msg void OnSCMin();
	afx_msg void OnSCRestore();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);	
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:

	void DrawBorder(CDC *pDC);
	void GetTitleRect(CRect &rect);
	void DrawCaption(CDC *pDC);
	void OnSetSysButtonPos();
	void DrawSysButtons(CDC *pDC);

private:
	void OnSetWndRgn();
	void DrawTitle(CDC *pDC, CRect rect);

	const CSize m_szRoundCorner;	//窗体圆角大小

	CRgn m_rgnWnd;
	CToolTipCtrl m_sysToolTipCtrl;
	
	//标题栏位图
	DBStretchImage m_imageCaption;
	DBStretchImage m_imageCaptionNA;
	DBStretchImage m_imageTitleBk;
	
	//边框位图
	enum {LEFT, RIGHT, BOTTOM};
	DBStretchImage m_imageBorders[3];
	const CRect m_rcSysBorder;
	
	//标题栏按钮
	DBButton		m_sysMinBtn;
	DBRestoreButton	m_sysMaxBtn;
	DBButton		m_sysCloseBtn;

	//按钮的有效矩形区域，相对于窗口右上角(注意：此矩形与按钮位图无关)
	CRect m_rcSysMinBtn;
	CRect m_rcSysCloseBtn;
	CRect m_rcSysMaxBtn;
	
	BOOL m_bNcActivate;			//表示当前标题栏是否激活

	DBStretchImage   m_imageLeftHeader;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBFRAME_H__D9ED011F_CCAA_4631_99E4_65EB2D35740A__INCLUDED_)
