/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBServerDlg.h
	author:		zhouliang
	
	purpose:	任务管理对话框
*********************************************************************/

#pragma once


// CEMBServerDlg 对话框
class CEMBServerDlg : public CDialog
{
// 构造
public:
	CEMBServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void InitUI();

public:

	BOOL m_bRunning;    //是否正在运行
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
	afx_msg void OnBnClickedBtnStart();


	afx_msg void OnBnClickedBtntest();
	afx_msg void OnBnClickedButtonXml();
};
