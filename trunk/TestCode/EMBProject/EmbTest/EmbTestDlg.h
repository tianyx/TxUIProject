
// EmbTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CEmbTestDlg �Ի���
class CEmbTestDlg : public CDialog
{
// ����
public:
	CEmbTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EMBTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	HWND m_hwndExcMsg;
	HANDLE m_hEventMsgWndCreated;
	BOOL m_bRunit;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedBtnTestado();
	CEdit m_edtdbStr;
	afx_msg void OnBnClickedBtnAutogentask();
	afx_msg void OnBnClickedBtnPerformcounter();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedBtnPerformcounter2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CEdit m_edtXmltrans;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedBtntimetostr();
};
