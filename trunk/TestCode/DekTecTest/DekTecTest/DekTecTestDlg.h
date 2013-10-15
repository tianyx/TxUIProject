
// DekTecTestDlg.h : 头文件
//

#pragma once

#include "dtapi.h"

// CDekTecTestDlg 对话框
class CDekTecTestDlg : public CDialog
{
// 构造
public:
	CDekTecTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DEKTECTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HANDLE m_hDtaThrd;
	HANDLE m_hReadThrd;

//	CRITICAL_SECTION m_buflock2;

	DtDevice m_dvc;
	DtOutpChannel m_tsOut;

	int InitDtaDevice();
	int UnInitDtaDevice();

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
	afx_msg void OnBnClickedBtnSelectfile();
	afx_msg void OnClose();
};

