/********************************************************************
	created:	2013/11/23
	filename: 	EMBClientDlg.h
	author:		tianyx
	purpose:	对话框
    History: 
*********************************************************************/

#pragma once


// CEMBClientDlg 对话框
class CEMBClientDlg : public CDialog
{
// 构造
public:
	CEMBClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBCLIENT_DIALOG };

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
};
