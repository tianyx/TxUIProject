
// QosTestSenderDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CQosTestSenderDlg �Ի���
class CQosTestSenderDlg : public CDialog
{
// ����
public:
	CQosTestSenderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QOSTESTSENDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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
	afx_msg void OnBnClickedBtnStart();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
public:
	CEdit m_edtLog;
	CEdit m_edttoSend;
	afx_msg void OnBnClickedBtnSend();
};
