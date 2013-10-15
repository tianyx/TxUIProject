
// MBTestClientDlg.h : 头文件
//

#pragma once
#include "winsock2.h"
#include "afxwin.h"


// CMBTestClientDlg 对话框
class CMBTestClientDlg : public CDialog
{
// 构造
public:
	CMBTestClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MBTESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	SOCKET m_socRecv;
	SOCKADDR_IN m_addrDes;
	SOCKET m_socNew;
	HANDLE m_hrecvThread;
public:
	BOOL StartRecv();
	void CloseRecv();
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
	CEdit m_edtLog;
	CEdit m_edtLocalIp;
	CEdit m_edtMCIp;
	CEdit m_edtLocalPort;
	CEdit m_edtMCPort;
	afx_msg void OnBnClickedBtnStart();
	CButton m_btnStart;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

