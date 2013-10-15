
// MultiBroadcastTestDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "winsock2.h"

// CMultiBroadcastTestSvrDlg 对话框
class CMultiBroadcastTestSvrDlg : public CDialog
{
// 构造
public:
	CMultiBroadcastTestSvrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MULTIBROADCASTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	SOCKET m_socSvr;
	SOCKET m_mcsocNew; //after joinleaf
	SOCKADDR_IN m_addrDes;
	SOCKADDR_IN m_addrSrc;


	BOOL StartMulticast();
	void CloseMulticast();
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
	CEdit m_edtIPLocal;
	CEdit m_edtIPMB;
	CEdit m_edtPortMB;
	CButton m_btnStart;
	CEdit m_edtLog;
	afx_msg void OnBnClickedBtnStart();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CEdit m_edtPortLocal;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


