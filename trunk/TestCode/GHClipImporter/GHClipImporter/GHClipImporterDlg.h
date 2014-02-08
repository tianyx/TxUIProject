
// GHClipImporterDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
#include "AutoCritSec.h"
#include "TransferClip.h"
#include "DlgRunningTask.h"
#include "DlgTaskFinished.h"
#include "DlgTaskErr.h"
#include "afxcmn.h"

#include "MiniDump.h"

using namespace std;

typedef map<int, HANDLE> MAPEXECS;


class CGHClipImporterDlg;
struct ST_EXECPARAM
{
	CGHClipImporterDlg* pDlg;
	int nExcId;
};


// CGHClipImporterDlg 对话框
class CGHClipImporterDlg : public CDialog, public ITaskReporter
{
// 构造
public:
	CGHClipImporterDlg(CWnd* pParent = NULL);	// 标准构造函数

	CMiniDump m_dump;
private:
	void LoadLastNotFinishedTask();
	void ScanTask();
	BOOL TryFetchWork(const int nExcId, ST_TASKINFO& infoOut);
public:
	void TaskScanLoop();
	void TaskExecLoop(const int nExcID);

	void ReportTask(const ST_TASKINFO&taskIn, vector<CString>& vExcMsgsIn);
	void UpdateTaskExcInfo(ST_REALTRASINFO& infoIn);

	void UpdateTaskList();
private:
	HANDLE m_hThreadDispatch;
	MAPEXECS m_mapExecHandls;
	HANDLE m_hEventQuit;
	CAutoCritSec m_csTask;
	INT64	m_nNextTaskId;
	MAPTASKS m_mapTasks;

	BOOL Run();
	BOOL Stop();

	INT64 GenNextTaskId(){return m_nNextTaskId++;};
	
private:
	CDlgRunningTask		m_dlgRunning;
	CDlgTaskFinished	m_dlgFinished;
	CDlgTaskErr			m_dlgErr;

	HWND m_vPageDlgWnd[3];

	void OnTabChange(int nTabId);
	void Relayout();
// 对话框数据
	enum { IDD = IDD_GHCLIPIMPORTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_tabBtnRunning;
	CButton m_tabBtnErr;
	CButton m_tabBtnFinished;
	afx_msg void OnClose();
	CTabCtrl m_tabTask;
	afx_msg void OnTcnSelchangeTabtask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
