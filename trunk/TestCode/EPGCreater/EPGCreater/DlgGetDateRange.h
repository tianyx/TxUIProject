#pragma once
#include "afxdtctl.h"


// CDlgGetDateRange �Ի���

class CDlgGetDateRange : public CDialog
{
	DECLARE_DYNAMIC(CDlgGetDateRange)

public:
	CDlgGetDateRange(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGetDateRange();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_dtStart;
	CDateTimeCtrl m_dtEnd;
	CTime m_tmStart;
	CTime m_tmEnd;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
