
// ZQLicenseMgrDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CZQLicenseMgrDlg 对话框
class CZQLicenseMgrDlg : public CDialog
{
// 构造
public:
	CZQLicenseMgrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ZQLICENSEMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	BOOL LoadConfig();

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
	CEdit m_edtPCCode;
	CComboBox m_cmbModule;
	CEdit m_stcModule;
	CButton m_chkExpireDate;
	CDateTimeCtrl m_tmCtlExpireDate;
	CEdit m_edtCodeResult;
	afx_msg void OnBnClickedBtnGen();
	afx_msg void OnCbnSelchangeComboModule();
	afx_msg void OnBnClickedBtnReset();
	CButton m_chkGenToFile;
	afx_msg void OnBnClickedChkExpireDate();
	CButton m_chkToFile2;
	afx_msg void OnBnClickedRegfromfile();
};
