#pragma once

#include "MainDef.h"

// CDlgDbgTest �Ի���

class CDlgDbgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgDbgTest)

public:
	CDlgDbgTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDbgTest();

// �Ի�������
	enum { IDD = IDD_DLG_TEST };
void InitUI();
	/*
    Description����ѯִ���߽�����Ϣ
	Input��		m_arrExecutors �����е�Executor.exe
	Return:		
	History��
	*/
	void GetExecutors(CArray<tagExecutorProcess, tagExecutorProcess>& arrExecutors, CArray<ST_TASKRUNSTATE, ST_TASKRUNSTATE>& arrTask); 

	// ����
	CString m_strVideoPath;
	CString m_strXmlTemplate;
	CArray<CString, CString> m_arrVideo;
	int m_nIdx;
	// ------------

	CListCtrl m_lstTask;
	CListCtrl m_lstExecutor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnBnClickedLuanchexec();
	afx_msg void OnBnClickedButtonAutotest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
afx_msg void OnBnClickedButtonRefresh();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
