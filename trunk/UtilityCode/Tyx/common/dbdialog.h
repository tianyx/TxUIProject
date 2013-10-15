#if !defined(AFX_DBDIALOG_H__6D145AD3_E430_4410_9D6B_FF5649F2B56F__INCLUDED_)
#define AFX_DBDIALOG_H__6D145AD3_E430_4410_9D6B_FF5649F2B56F__INCLUDED_

#include "DBButton.h"	// Added by ClassView
#include "DBRestoreButton.h"	// Added by ClassView
#include "DBStretchImage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBDialog.h : header file
//

#include "CommonButton.h"
#include "DialogMultiLan.h"


/////////////////////////////////////////////////////////////////////////////
// CDBDialog dialog

#define ID_SC_BASE		65500
#define ID_SC_MIN		ID_SC_BASE - 1
#define ID_SC_MAX		ID_SC_BASE - 2
#define ID_SC_RESTORE	ID_SC_BASE - 3
#define ID_SC_CLOSE		ID_SC_BASE - 4

class CDBDialog : public CDialogMultiLan
{
	DECLARE_DYNAMIC(CDBDialog)

// Construction
public:
	CDBDialog(UINT nIDTemplate, LPCTSTR lpszText, CWnd* pParentWnd = NULL);   // standard constructor
	CDBDialog(UINT nIDTemplate, LPCTSTR lpszText, CWnd* pParentWnd, BOOL bChildType);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDBDialog)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDBDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSCClose();
	afx_msg void OnSCMax();
	afx_msg void OnSCMin();
	afx_msg void OnSCRestore();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	void DrawBorder(CDC *pDC);
	void DrawCaption(CDC *pDC);
	void OnSetSysButtonPos();
	void DrawSysButtons(CDC *pDC);
//	void GetTitleRect(CRect &rect);
	void DrawTitle(CDC *pDC, CRect rect);
	void SysBtnRelayEvent(MSG* pMsg);

	HICON  m_hIcon;

private:
	CBrush m_bruBkgnd;

	const CSize m_szRoundCorner;	//窗体圆角大小

	CRgn m_rgnWnd;
	CToolTipCtrl m_sysToolTipCtrl;
	
	//标题栏位图
	DBStretchImage m_imageCaption;
	DBStretchImage m_imageCaptionNA;
//	DBStretchImage m_imageTitleBk;
	
	//边框位图
	enum {LEFT, RIGHT, BOTTOM};
	DBStretchImage m_imageBorders[3];
	const CRect m_rcSysBorder;
	
	//标题栏按钮
	DBButton		m_sysMinBtn;
	DBRestoreButton	m_sysMaxBtn;
protected:
	DBButton		m_sysCloseBtn;

	//按钮的有效矩形区域，相对于窗口右上角(注意：此矩形与按钮位图无关)
	CRect m_rcSysMinBtn;
	CRect m_rcSysCloseBtn;
	CRect m_rcSysMaxBtn;
	
	BOOL m_bNcActivate;			//表示当前标题栏是否激活

	BOOL m_bChildType;
public:
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBDIALOG_H__6D145AD3_E430_4410_9D6B_FF5649F2B56F__INCLUDED_)
