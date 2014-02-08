#pragma once
#include "afxcmn.h"
#include "MainDef.h"
#include "afxwin.h"

// CDlgRunningTask �Ի���

class CDlgRunningTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgRunningTask)

public:
	CDlgRunningTask(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRunningTask();

public:
	void ResetList();
	void AddTask(MAPTASKS& task);
	void DelTask(INT64 nTaskIDIn);

	void UpdateRealTaskInfo(ST_REALTRASINFO& realInfo);
// �Ի�������
	enum { IDD = IDD_DIALOG_TASKRUNNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstTask;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CStatic m_stcInfo;
};
