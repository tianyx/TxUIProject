
// MBTestClientDlg.h : ͷ�ļ�
//

#pragma once
#include "winsock2.h"
#include "afxwin.h"


// CMBTestClientDlg �Ի���
class CMBTestClientDlg : public CDialog
{
// ����
public:
	CMBTestClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MBTESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	SOCKET m_socRecv;
	SOCKADDR_IN m_addrDes;
	SOCKET m_socNew;
	HANDLE m_hrecvThread;
public:
	BOOL StartRecv();
	void CloseRecv();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

