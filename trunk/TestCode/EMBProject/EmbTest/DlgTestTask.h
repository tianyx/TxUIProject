#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"

// CDlgTestTask dialog

class CDlgTestTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestTask)

public:
	CDlgTestTask(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTestTask();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

public:
	int m_nTaskType;
	CString m_vtaskfile[5];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
void CDlgTestTask::OnBnClickedBtnTestado();

	void QueryTask();
	void DelTask();
	void DelTaskLog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAutogentask();
	CEdit m_edtDbconn;
	CEdit m_edtSrcfilename;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	virtual BOOL OnInitDialog();
	CButton m_rdoTasktype;
	CListCtrl m_lstTask;
	afx_msg void OnBnClickedButtonDeltask();
	afx_msg void OnBnClickedButtonDeltasklog();
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedBtnOther();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
