/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBServerDlg.h
	author:		zhouliang
	
	purpose:	�������Ի���
*********************************************************************/

#pragma once


// CEMBServerDlg �Ի���
class CEMBServerDlg : public CDialog
{
// ����
public:
	CEMBServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EMBSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void InitUI();

public:

	BOOL m_bRunning;    //�Ƿ���������
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


	afx_msg void OnBnClickedBtntest();
	afx_msg void OnBnClickedButtonXml();
};
