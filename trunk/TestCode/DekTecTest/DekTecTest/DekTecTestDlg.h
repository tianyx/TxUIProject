
// DekTecTestDlg.h : ͷ�ļ�
//

#pragma once

#include "dtapi.h"

// CDekTecTestDlg �Ի���
class CDekTecTestDlg : public CDialog
{
// ����
public:
	CDekTecTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DEKTECTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HANDLE m_hDtaThrd;
	HANDLE m_hReadThrd;

//	CRITICAL_SECTION m_buflock2;

	DtDevice m_dvc;
	DtOutpChannel m_tsOut;

	int InitDtaDevice();
	int UnInitDtaDevice();

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnSelectfile();
	afx_msg void OnClose();
};

