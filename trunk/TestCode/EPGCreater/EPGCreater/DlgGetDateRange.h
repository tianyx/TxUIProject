#pragma once
#include "afxdtctl.h"


// CDlgGetDateRange 对话框

class CDlgGetDateRange : public CDialog
{
	DECLARE_DYNAMIC(CDlgGetDateRange)

public:
	CDlgGetDateRange(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGetDateRange();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_dtStart;
	CDateTimeCtrl m_dtEnd;
	CTime m_tmStart;
	CTime m_tmEnd;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
