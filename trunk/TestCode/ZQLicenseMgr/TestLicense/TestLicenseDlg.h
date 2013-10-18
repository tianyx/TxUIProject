
// TestLicenseDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CTestLicenseDlg 对话框
class CTestLicenseDlg : public CDialog
{
// 构造
public:
	CTestLicenseDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTLICENSE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	BOOL CheckLc();

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
	afx_msg void OnBnClickedBtnTestlc();
	afx_msg void OnBnClickedBtnGetlc();
	CEdit m_edtRet;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
