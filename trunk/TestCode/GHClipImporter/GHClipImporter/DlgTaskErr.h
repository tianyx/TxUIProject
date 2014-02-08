#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MainDef.h"


// CDlgTaskErr �Ի���

class CDlgTaskErr : public CDialog
{
	DECLARE_DYNAMIC(CDlgTaskErr)

public:
	CDlgTaskErr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTaskErr();

// �Ի�������
	enum { IDD = IDD_DIALOG_TASKERR };

public:
	void Refresh();
	void ShowLogFile(CString& strLogIn);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	MMAPTIMETASKS mmapTasks;
	CListCtrl m_lstTaskErr;
	CEdit m_editTaskLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CButton m_btnRefresh;
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnLvnItemchangedListTaskerr(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
