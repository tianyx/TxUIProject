
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
	afx_msg void OnBnClickedButton5();
	CEdit m_edtdbStr;
	afx_msg void OnBnClickedBtnAutogentask();
};
