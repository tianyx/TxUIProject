#pragma once
#include "afxwin.h"


// CdlgTopBar �Ի���

class CDlgTopBar : public CDialog
{
	DECLARE_DYNAMIC(CDlgTopBar)

public:
	CDlgTopBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTopBar();

// �Ի�������
	enum { IDD = IDD_DLGTOPBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtntest();
	CButton m_rdoTab;
	afx_msg void OnBnClickedBtnNewtask();
	afx_msg void OnBnClickedRdoAuto();
	afx_msg void OnBnClickedRdoManual();
	afx_msg void OnBnClickedRdoPlaylist();


	virtual BOOL OnInitDialog();
	CButton m_btnTest;
};
