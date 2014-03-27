#pragma once

#include "MainDef.h"

// CDlgDbgTest 对话框

class CDlgDbgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgDbgTest)

public:
	CDlgDbgTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDbgTest();

// 对话框数据
	enum { IDD = IDD_DLG_TEST };
void InitUI();
	/*
    Description：查询执行者进程信息
	Input：		m_arrExecutors 运行中的Executor.exe
	Return:		
	History：
	*/
	void GetExecutors(CArray<tagExecutorProcess, tagExecutorProcess>& arrExecutors, CArray<ST_TASKRUNSTATE, ST_TASKRUNSTATE>& arrTask); 

	// 测试
	CString m_strVideoPath;
	CString m_strXmlTemplate;
	CArray<CString, CString> m_arrVideo;
	int m_nIdx;
	// ------------

	CListCtrl m_lstTask;
	CListCtrl m_lstExecutor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnBnClickedLuanchexec();
	afx_msg void OnBnClickedButtonAutotest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
afx_msg void OnBnClickedButtonRefresh();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
