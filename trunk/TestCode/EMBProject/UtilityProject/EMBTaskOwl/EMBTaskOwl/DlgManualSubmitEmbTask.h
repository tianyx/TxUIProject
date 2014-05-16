#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "GlobalDef.h"
#include "AutoTaskFunc.h"

// CManualAddEmbTask �Ի���
#define MSG_TASKINFOUPDATE WM_USER+1500

class CDlgManualSubmitEmbTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgManualSubmitEmbTask)

public:
	CDlgManualSubmitEmbTask(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgManualSubmitEmbTask();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstTask;
	CButton m_btnSubmit;
	afx_msg void OnBnClickedBtnsubmit();
	virtual BOOL OnInitDialog();
	CComboBox m_cmbTpl;
	CStatic m_stcOutput;
};
