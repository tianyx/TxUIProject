
// GHClipImporterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "GHClipImporterDlg.h"
#include "AutoCritSec.h"
#include "TxLogManager.h"
#include "FGlobal.h"
#include "TxADOConn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
DWORD __stdcall TaskDispatchRunProc(LPVOID lparam)
{
	CGHClipImporterDlg* pDlg = (CGHClipImporterDlg*)lparam;
	if (pDlg == NULL)
	{
		return 0;
	}

	pDlg->TaskScanLoop();
	return 0;
}

DWORD __stdcall ExcRunProc(LPVOID lparam)
{
	ST_EXECPARAM* pParam = (ST_EXECPARAM*)lparam;
	if (!pParam)
	{
		ASSERT(FALSE);
		return 0;
	}

	ST_EXECPARAM excParam = *pParam;
	delete pParam;
	if (excParam.nExcId < 0 || excParam.pDlg == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	excParam.pDlg->TaskExecLoop(excParam.nExcId);
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////
// CGHClipImporterDlg 对话框




CGHClipImporterDlg::CGHClipImporterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGHClipImporterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hEventQuit =  CreateEvent(NULL, TRUE, FALSE, NULL);
	m_nNextTaskId = _time64(NULL);
}

void CGHClipImporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RUNNING, m_tabBtnRunning);
	DDX_Control(pDX, IDC_BTN_ERROR, m_tabBtnErr);
	DDX_Control(pDX, IDC_BTN_FINISHED, m_tabBtnFinished);
	DDX_Control(pDX, IDC_TABTASK, m_tabTask);
}

BEGIN_MESSAGE_MAP(CGHClipImporterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABTASK, &CGHClipImporterDlg::OnTcnSelchangeTabtask)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGHClipImporterDlg 消息处理程序

BOOL CGHClipImporterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_dlgRunning.Create(CDlgRunningTask::IDD, &m_tabTask);
	m_vPageDlgWnd[0] = m_dlgRunning.GetSafeHwnd();

	m_dlgErr.Create(CDlgTaskErr::IDD, &m_tabTask);
	m_vPageDlgWnd[1] = m_dlgErr.GetSafeHwnd();
	m_dlgFinished.Create(CDlgTaskFinished::IDD,  &m_tabTask);
	m_vPageDlgWnd[2] = m_dlgFinished.GetSafeHwnd();
	m_tabTask.InsertItem(0, TEXT("当前任务"));
	m_tabTask.InsertItem(1, TEXT("失败任务"));
	m_tabTask.InsertItem(2, TEXT("已完成任务"));
	Relayout();

#ifndef FORMONITOR
#else
	SetWindowText(TEXT("GHClipImportMonitor"));
	m_tabTask.DeleteItem(0);
#endif

	m_tabTask.SetCurSel(0);
	OnTabChange(0);

	// TODO: 在此添加额外的初始化代码
#ifndef FORMONITOR
	Run();
#endif
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGHClipImporterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGHClipImporterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGHClipImporterDlg::Run()
{
	Stop();
	LoadLastNotFinishedTask();
	m_hThreadDispatch = CreateThread(NULL, 0, TaskDispatchRunProc, (LPVOID)this, 0, 0);
	ASSERT(m_hThreadDispatch);

	for (int i = 0; i < g_GlobalInfo.nMaxSession; ++i)
	{
		ST_EXECPARAM* pParam = new ST_EXECPARAM;
		pParam->nExcId = i;
		pParam->pDlg = this;
		HANDLE handle = CreateThread(NULL, 0, ExcRunProc, (LPVOID)pParam, 0, 0);
		ASSERT(handle);
		m_mapExecHandls[i] = handle;
	}

	return TRUE;
}

BOOL CGHClipImporterDlg::Stop()
{		
	SetEvent(m_hEventQuit);
	if (m_hThreadDispatch)
	{
		WaitForSingleObject(m_hThreadDispatch, INFINITE);
	}

	MAPEXECS::iterator itb = m_mapExecHandls.begin();
	MAPEXECS::iterator ite = m_mapExecHandls.end();
	for (; itb != ite; ++itb)
	{
		WaitForSingleObject(itb->second, INFINITE);
	}
	ResetEvent(m_hEventQuit);
	m_mapTasks.clear();
	return TRUE;
}


void CGHClipImporterDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (AfxMessageBox(TEXT("确认退出程序?"), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	Stop();
	CDialog::OnClose();
}

void CGHClipImporterDlg::TaskScanLoop()
{
	CTime tmNext(time(NULL));
	CTimeSpan tsDeta(0,0,g_GlobalInfo.nAutoCheckInterval/60, g_GlobalInfo.nAutoCheckInterval%60);
	while(WaitForSingleObject(m_hEventQuit, 900) != WAIT_OBJECT_0)
	{
		CTime tmNow(time(NULL));
		CTimeSpan tmSpan = tmNow - tmNext;
		if (tmSpan.GetTotalSeconds() > 0 && tmSpan.GetSeconds() == g_GlobalInfo.nScanTimePos)
		{
			//start scan now;
			if (m_mapTasks.size() <= g_GlobalInfo.nMaxSession)
			{
				ScanTask();
			}
			//calc next scan time
			tmNext = tmNow +tsDeta;
			CString strNext = tmNext.Format(TEXT("<信息> 下一次扫描时间: %Y-%m-%d %H:%M:%S"));
			m_dlgRunning.SendMessage(MSG_INFOMSG, (WPARAM)&strNext,0);
		}
	}

}

void CGHClipImporterDlg::ScanTask()
{
	if (_access(g_GlobalInfo.szScanDir, 0) == -1)
	{
		CFWriteLog(0, TEXT("扫描路径无法访问! %s"), g_GlobalInfo.szScanDir);
		return;
	}
	vector<CString> vFiles;
	GetFilesInFolder(g_GlobalInfo.szScanDir, vFiles, TEXT("xml"), FALSE);

	int nAddedNum = 0;
	MAPTASKS m_mapTasksNew;
	if (vFiles.size() > 0)
	{	
		CAutoLock lock(&m_csTask);
		//Create WorkDir
		CreateDirectory(g_GlobalInfo.szWorkDir, NULL);
		for (size_t i = 0; i < vFiles.size(); ++i)
		{
			CString strDestFile = g_GlobalInfo.szWorkDir + TEXT("\\");
			strDestFile += TxGetFileName(vFiles[i]);
			if (_access(vFiles[i], 04) != -1)
			{
				if (rename(vFiles[i], strDestFile) == 0)
				{
					ST_TASKINFO taskInfo;
					taskInfo.nTaskId = GenNextTaskId();
					taskInfo.strTaskXMlFile = strDestFile;
					m_mapTasks[taskInfo.nTaskId] = taskInfo; 
					m_mapTasksNew[taskInfo.nTaskId] = taskInfo;
					nAddedNum++;
					if (nAddedNum >= g_GlobalInfo.nMaxTaskGetPerScan)
					{
						break;
					}
				}
				else
				{
					CFWriteLog(0, TEXT("file can't rename, %s"), vFiles[i]);
				}
			}
			else
			{
				//maybe note copy finished
			}
			
		}
	}

	if (nAddedNum > 0)
	{
		::SendMessage(m_dlgRunning.GetSafeHwnd(), MSG_TASKADD, (WPARAM)&m_mapTasksNew, 0);
	}
}

void CGHClipImporterDlg::LoadLastNotFinishedTask()
{
	if (_access(g_GlobalInfo.szWorkDir, 0) == -1)
	{
		CreateDirectory(g_GlobalInfo.szWorkDir, 0);
		return;
	}
	vector<CString> vFiles;
	GetFilesInFolder(g_GlobalInfo.szWorkDir, vFiles, TEXT("xml"), FALSE);
	MAPTASKS m_mapTasksNew;

	if (vFiles.size() > 0)
	{	
		CAutoLock lock(&m_csTask);
		//Create WorkDir
		for (size_t i = 0; i < vFiles.size(); ++i)
		{
			if (_access(vFiles[i], 04) != -1)
			{
				ST_TASKINFO taskInfo;
				taskInfo.nTaskId = GenNextTaskId();
				taskInfo.strTaskXMlFile = vFiles[i];
				m_mapTasks[taskInfo.nTaskId] = taskInfo; 
				m_mapTasksNew[taskInfo.nTaskId] = taskInfo;
			}
			else
			{
				//maybe note copy finished
			}

		}
		if (m_mapTasksNew.size() > 0)
		{
			::SendMessage(m_dlgRunning.GetSafeHwnd(), MSG_TASKADD, (WPARAM)&m_mapTasksNew, 0);
		}
	}
}

void CGHClipImporterDlg::TaskExecLoop( const int nExcID )
{
	CTxADODBMgr::GetADODBMgr()->OpenDB(nExcID, g_GlobalInfo.szDBConnString);

	while(WaitForSingleObject(m_hEventQuit, 1000) != WAIT_OBJECT_0)
	{
		ST_TASKINFO taskInfo;
		if (!TryFetchWork(nExcID, taskInfo))
		{
			continue;
		}
		
		ST_REALTRASINFO realInfo;
		realInfo.nTaskId = taskInfo.nTaskId;
		realInfo.nPercent = 0;
		realInfo.nSpeedKBPerS = 0;
		realInfo.nSrcFileSize = 0;
		realInfo.nRetry = taskInfo.nRetry;
		realInfo.nExcId = nExcID;
		realInfo.strMsg= TEXT("任务已分配");
		UpdateTaskExcInfo(realInfo);

		//start exc task;
		CTransferClip transClip(this);
		transClip.Init(taskInfo, m_hEventQuit);
		transClip.Start();
	}
	CTxADODBMgr::GetADODBMgr()->CloseDB(nExcID);

}

BOOL CGHClipImporterDlg::TryFetchWork( const int nExcId, ST_TASKINFO& infoOut )
{
	BOOL bRet = FALSE;
	CAutoLock lock(&m_csTask);
	MAPTASKS::iterator itb = m_mapTasks.begin();
	MAPTASKS::iterator ite = m_mapTasks.end();
	for (; itb != ite; ++itb)
	{
		if (itb->second.nRunState == TaskRunState_none)
		{
			itb->second.nRunState = TaskRunstate_dispatched;
			itb->second.nExcID = nExcId;
			infoOut = itb->second;
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

void CGHClipImporterDlg::ReportTask( const ST_TASKINFO& taskIn, vector<CString>& vExcMsgsIn )
{
	CString strTaskEnd;
	CString strLogFile;
	{
		CAutoLock lock(&m_csTask);
		MAPTASKS::iterator itf = m_mapTasks.find(taskIn.nTaskId);
		if (itf != m_mapTasks.end())
		{
			BOOL bRemove = FALSE;
			if (taskIn.nRunState == TaskRunState_finished)
			{
				strTaskEnd = g_GlobalInfo.szWorkFinishedDir;
				strTaskEnd += TEXT("\\");
				strLogFile = strTaskEnd;
				strTaskEnd += TxGetFileName(taskIn.strTaskXMlFile);
				strLogFile += TxGetFileTitle(taskIn.strTaskXMlFile) + TEXT(".log");
				bRemove = TRUE;
			}
			else if (taskIn.nRunState == TaskRunState_error)
			{
				if (itf->second.nRetry < g_GlobalInfo.nMaxRetry)
				{
					//reset
					itf->second.nRetry +=1;
					itf->second.nRunState = TaskRunState_none;
					itf->second.nExcID = -1;

					//update task
					ST_REALTRASINFO realInfo;
					realInfo.nTaskId = taskIn.nTaskId;
					realInfo.nPercent = 0;
					realInfo.nSpeedKBPerS = 0;
					realInfo.nSrcFileSize = 0;
					realInfo.nRetry = itf->second.nRetry;
					realInfo.nExcId = -1;
					realInfo.strMsg= TEXT("任务失败，等待重试");
					UpdateTaskExcInfo(realInfo);

					return;
				}
				else
				{
					bRemove = TRUE;
					strTaskEnd = g_GlobalInfo.szWorkErrDir;
					strTaskEnd += TEXT("\\");
					strLogFile = strTaskEnd;
					strTaskEnd += TxGetFileName(taskIn.strTaskXMlFile);
					strLogFile += TxGetFileTitle(taskIn.strTaskXMlFile) + TEXT(".log");
				}

			}

			if (bRemove)
			{
				m_mapTasks.erase(taskIn.nTaskId);
			}
		}
		else
		{
			//not process
			CFWriteLog(TEXT("found unknow task, %s"), taskIn.strTaskXMlFile);
			return;
		}

	}

	if (strTaskEnd.IsEmpty())
	{
		return;
	}
	//delete from map, copy task to finished dir, write exc msg file
	CreateDirectory(g_GlobalInfo.szWorkFinishedDir, NULL);
	CreateDirectory(g_GlobalInfo.szWorkErrDir, NULL);
	if (MoveFile(taskIn.strTaskXMlFile, strTaskEnd))
	{
		CString strText;
		for (size_t i = 0; i < vExcMsgsIn.size(); ++i)
		{
			strText += vExcMsgsIn[i];
			strText += TEXT("\r\n");
		}
		if (strText.GetLength() > 0)
		{
			CFile flog;
			if (flog.Open(strLogFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
			{
				flog.Write(strText.LockBuffer(), strText.GetLength());
				flog.Flush();
				flog.Close();
				strText.UnlockBuffer();
			}

		}
	}
	else
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("移动文件失败:%s, %s"), taskIn.strTaskXMlFile, strTaskEnd);
	}

	::SendMessage(m_dlgRunning.GetSafeHwnd(), MSG_TASKDEL, (WPARAM)&taskIn.nTaskId, 0);
}

void CGHClipImporterDlg::UpdateTaskExcInfo(ST_REALTRASINFO& infoIn)
{
	ST_REALTRASINFO* pInfo = new ST_REALTRASINFO;
	*pInfo = infoIn;
	::PostMessage(m_dlgRunning.GetSafeHwnd(), MSG_TASKUPDATEREALINFO, (WPARAM)pInfo, 0);
}

void CGHClipImporterDlg::UpdateTaskList()
{

}

void CGHClipImporterDlg::OnTabChange( int nTabId )
{
#ifndef FORMONITOR
	for (size_t i = 0; i < 3; ++i)
	{
		::ShowWindow(m_vPageDlgWnd[i], nTabId == i? SW_SHOW:SW_HIDE);
	}
#else
	for (size_t i = 0; i < 3; ++i)
	{
		::ShowWindow(m_vPageDlgWnd[i], nTabId == i-1? SW_SHOW:SW_HIDE);
		if (nTabId == i-1)
		{
			::SendMessage(m_vPageDlgWnd[i], MSG_TASKSHOW, 0,0);
		}
		
	}
#endif
	
}

void CGHClipImporterDlg::Relayout()
{
	if (!m_dlgRunning.GetSafeHwnd())
	{
		return;
	}

	

	CRect rcthis;
	GetClientRect(rcthis);
	rcthis.DeflateRect(0,2,0,0);
	m_tabTask.MoveWindow(rcthis);
	CRect rcTab;
	m_tabTask.GetClientRect(rcTab);
	CRect rcItem;
	m_tabTask.GetItemRect(0, rcItem);
	rcTab.DeflateRect(2,rcItem.Height()+4, 1,1);
// 	m_tabTask.ClientToScreen(&rcTab);
// 	ScreenToClient(rcTab);
	m_dlgRunning.MoveWindow(rcTab);
	m_dlgFinished.MoveWindow(rcTab);
	m_dlgErr.MoveWindow(rcTab);
}

void CGHClipImporterDlg::OnTcnSelchangeTabtask(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nItem = m_tabTask.GetCurSel();
	OnTabChange(nItem);
	*pResult = 0;
}

void CGHClipImporterDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	Relayout();
	// TODO: 在此处添加消息处理程序代码
}
