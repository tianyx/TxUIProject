
// QosTestRecverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CQosTestRecverDlg 对话框
class CQosTestRecverDlg : public CDialog
{
// 构造
public:
	CQosTestRecverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QOSTESTRECVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
public:
	CEdit m_edtLog;
	CEdit m_edttoSend;
	afx_msg void OnBnClickedBtnSend();
};
