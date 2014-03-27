/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBServerDlg.h
	author:		zhouliang
	
	purpose:	任务管理对话框
*********************************************************************/

#pragma once
#include "afxcmn.h"
#include "GdiPlusNewHeader.h"
#include "TxImgButton.h"
#include "afxwin.h"
#include "DlgDbgTest.h"


// CEMBServerDlg 对话框
class CEMBServerDlg : public CDialog,
	public IParentBackDrawInterface
{
// 构造
public:
	CEMBServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	virtual void GetParentBack(CDC* pDc);
public:

	BOOL m_bRunning;    //是否正在运行
// 实现
protected:
	HICON m_hIcon;
	CDlgDbgTest m_dlgtest;

private:
	Gdiplus::Bitmap* m_pbmpBack;
	Gdiplus::Bitmap* m_pbmpLogo2;
	Gdiplus::Bitmap* m_pbmpSublogo;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();

	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CTxImgButton m_btnStart;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
