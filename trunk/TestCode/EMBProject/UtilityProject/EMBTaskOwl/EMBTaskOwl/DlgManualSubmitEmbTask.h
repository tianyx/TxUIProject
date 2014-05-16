#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GlobalDef.h"
#include "AutoTaskFunc.h"

// CManualAddEmbTask 对话框
#define MSG_TASKINFOUPDATE WM_USER+1500

class CDlgManualSubmitEmbTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgManualSubmitEmbTask)

public:
	CDlgManualSubmitEmbTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgManualSubmitEmbTask();

// 对话框数据
	enum { IDD = IDD_DLG_MANUALADDEMBTASK };
public:
	void InitTask();
	void RefreshTaskList();
	void InitListHeader();
public:
	VECINPOOLTASK* m_pvOrgTasks;
	VECINTS m_vSubmitIds;
	HWND m_hwndNotify;

	VECLSTHEADERS m_vListCols;

	void WriteOutput(LPCTSTR format,...);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstTask;
	CButton m_btnSubmit;
	afx_msg void OnBnClickedBtnsubmit();
	virtual BOOL OnInitDialog();
	CComboBox m_cmbTpl;
	CStatic m_stcOutput;
};
