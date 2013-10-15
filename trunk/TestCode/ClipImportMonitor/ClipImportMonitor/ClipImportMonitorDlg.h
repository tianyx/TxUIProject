/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipImportMonitorDlg.h
	file base:	ClipImportMonitorDlg
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipImportMonitorDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "TxImgButton.h"
#include "ClipMonView.h"
#include "TxImgButton.h"
#include "TxSlashCtrl.h"
#include "DlgMovedBase.h"
using namespace Gdiplus;
// CClipImportMonitorDlg 对话框
class CClipImportMonitorDlg : public CDlgMovedBase
{
// 构造
public:
	CClipImportMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIPIMPORTMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void Relayout();
	void CheckAlert();
// 实现
protected:
	HICON m_hIcon;

	CTxImgButton m_btnMin;
	CTxImgButton m_btnMax;
	CTxImgButton m_btnRestore;
	CTxImgButton m_btnClose;
	CTxImgButton m_btnHideTitle;
	CTxSlashCtrl m_ctlSlash;
	Bitmap* m_pImgBk;
	Bitmap* m_pBmpAlert;
	int m_nAlertWidth;
	CSize m_szAlertBmpSize;
	int m_nTitleHeight;


	CString m_strTitle;

	CRect m_rcTime;
	CRect m_rcAlert;

	CBitmap m_bmpCache;


	//
	BOOL m_nAlertType;

	Gdiplus::Font* m_pTimerFont;
	Gdiplus::Font* m_pTitleFont;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CClipMonView* m_pView;
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
public:
	afx_msg BOOL OnNcActivate(BOOL bActive);
public:
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	afx_msg void OnBnClickSysClose();
	afx_msg void OnBnClickSysMin();
	afx_msg void OnBnClickSysMax();
	afx_msg void OnBnClickSysRestore();
	afx_msg void OnBnClickHideTitle();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
