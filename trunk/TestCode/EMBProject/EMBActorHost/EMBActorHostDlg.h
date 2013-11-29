
// EMBActorHostDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "MainDef.h"


// CEMBActorHostDlg 对话框
class CEMBActorHostDlg : public CDialog
{
// 构造
public:
	CEMBActorHostDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBACTORHOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void InitUI();
	/*
    Description：查询执行者进程信息
	Input：		m_arrExecutors 运行中的Executor.exe
	Return:		
	History：
	*/
	void GetExecutors(CArray<tagExecutorProcess, tagExecutorProcess>& m_arrExecutors); 

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CListCtrl m_lstExecutor;
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnClose();
	afx_msg void OnBnClickedLuanchexec();
};
