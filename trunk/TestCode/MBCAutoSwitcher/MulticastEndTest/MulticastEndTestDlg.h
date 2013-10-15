
// MulticastEndTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MBCSocket.h"

// CMulticastEndTestDlg �Ի���
class CMulticastEndTestDlg : public CDialog, public ISockMsgCallbackInterFace
{
// ����
public:
	CMulticastEndTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTICASTENDTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	virtual HRESULT NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) ;
	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) ;
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) ;
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) ;
	virtual HRESULT NetCall_Write(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) ;
	virtual HRESULT SockStateChange(CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn) ;

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
	afx_msg void OnBnClickedBtnstart();
	CEdit m_edtIp;
	CMBCSocket*  m_psock;
	SOCKET m_hMBCSock;
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_edtLocaPort;
	afx_msg void OnBnClickedBtnRecvtest();
};
