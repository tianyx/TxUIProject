// DlgAutoTask.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgAutoTask.h"
#include "TxFontLoader.h"

#define MSG_UPDATE_AUTOINFO WM_USER+1400
#define MSG_UPDATE_LASTINFO WM_USER+1401

#define msgupdatetype_downcount 1
#define msgupdatetype_start		2
#define msgupdatetype_stop		3

using namespace EMB;
DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CDlgAutoTask* pObj = (CDlgAutoTask*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}

// CDlgAutoTask 对话框

IMPLEMENT_DYNAMIC(CDlgAutoTask, CDialog)

CDlgAutoTask::CDlgAutoTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoTask::IDD, pParent)
{
	m_hThreadTask = NULL;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_nColPosTaskid =-1;
}

CDlgAutoTask::~CDlgAutoTask()
{

}

void CDlgAutoTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RUN, m_btnRun);
	DDX_Control(pDX, IDC_STC_AUTOMSG, m_stcAutoMsg);
	DDX_Control(pDX, IDC_EDIT_AUTOLOG, m_edtAutoLog);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
}


BEGIN_MESSAGE_MAP(CDlgAutoTask, CDialog)
	ON_BN_CLICKED(IDC_BTN_RUN, &CDlgAutoTask::OnBnClickedBtnRun)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TASK, &CDlgAutoTask::OnNMCustomdrawListTask)
END_MESSAGE_MAP()


// CDlgAutoTask 消息处理程序

BOOL CDlgAutoTask::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN 
		&& (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_F1 || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE )
	{
		CWnd* pWnd = GetFocus();
		if ( !(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))||pWnd->IsKindOf(RUNTIME_CLASS(CEdit)))) )
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgAutoTask::OnBnClickedBtnRun()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_hThreadTask)
	{
		if (!m_autoFun.InitStg())
		{
			UpdateAutoMsg(TEXT("策略初始化失败！"));
			return;
		}
		ResetEvent(m_hQuitEvent);
		m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
		ASSERT(m_hThreadTask != NULL);
	}
	else
	{
		SetEvent(m_hQuitEvent);
		WaitForSingleObject(m_hThreadTask, INFINITE);
		m_hThreadTask = NULL;
	}
	UpdateCtrl();
}

void CDlgAutoTask::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Relayout();
	// TODO: 在此处添加消息处理程序代码
}

void CDlgAutoTask::Relayout()
{
	if (!m_btnRun.GetSafeHwnd())
	{
		return;
	}

	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcStcMsg(rcThis);
	rcStcMsg.bottom = rcStcMsg.top+100;
	rcStcMsg.right = rcThis.right - 120;
	m_stcAutoMsg.MoveWindow(rcStcMsg);
	CRect rcBtn(rcStcMsg);
	rcBtn.left = rcStcMsg.right+10;
	rcBtn.right = rcThis.right -10;
	m_btnRun.MoveWindow(rcBtn);

	CRect rcLog(rcThis);
	rcLog.top = rcStcMsg.bottom;
	rcLog.bottom = rcLog.top+120;
	m_edtAutoLog.MoveWindow(rcLog);

	CRect rcLst(rcThis);
	rcLst.top = rcLog.bottom;
	m_lstTask.MoveWindow(rcLst);


}

void CDlgAutoTask::UpdateAutoMsg( LPCTSTR szMsg )
{
	m_stcAutoMsg.SetWindowText(szMsg);
}

BOOL CDlgAutoTask::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_lstTask.SetExtendedStyle( (m_lstTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));

	InitListHeader();
	// TODO:  在此添加额外的初始化
	int nFontIdx =g_pFontLoader->AddFont(TEXT("Arial"), 32, TRUE);
	m_stcAutoMsg.SetFont(g_pFontLoader->GetCFont(nFontIdx));
	UpdateCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAutoTask::UpdateCtrl()
{
	m_btnRun.SetWindowText(m_hThreadTask? TEXT("停止"):TEXT("启动"));
}

void CDlgAutoTask::RunTaskLoop()
{
	int nRemainSec = g_GlobalInfo.nAutoLoopInterval;
	while(WaitForSingleObject(m_hQuitEvent, 1000) != WAIT_OBJECT_0)
	{
		--nRemainSec;
		if (nRemainSec > 0)
		{
			PostMessage(MSG_UPDATE_AUTOINFO, msgupdatetype_downcount, (LPARAM)nRemainSec);
			continue;
		}
		else
		{
			nRemainSec = g_GlobalInfo.nAutoLoopInterval;
			PostMessage(MSG_UPDATE_AUTOINFO, msgupdatetype_start, (LPARAM)nRemainSec);
		}

		DWORD dwTickCount = GetTickCount();
		//find uncommit srcTask
		CTime tmNow(time(NULL));
		BOOL bInIdleTime = IsIdleHour();
		CTime tmBegin = tmNow + CTimeSpan(0, 0, bInIdleTime? g_GlobalInfo.nIdleTimelineClose : g_GlobalInfo.nTimelineClose, 0);
		CTime tmEnd = tmNow + CTimeSpan(0, 0, bInIdleTime?  g_GlobalInfo.nIdleTimeLineOffline : g_GlobalInfo.nTimeLineOffline,0);
		VECINPOOLTASK vInPoolTask;
		CFWriteLog(TEXT("auto task begin..."));
		CFWriteLog(0,TEXT("try get online task%s, play time range: [%s -%s]"), bInIdleTime? TEXT("[in idle mode]"):TEXT(""), Time2Str(tmBegin.GetTime()), Time2Str(tmEnd.GetTime()));
		m_autoFun.GetOrgTasksFromDB(tmBegin, tmEnd, vInPoolTask);
		CFWriteLog(0, TEXT("got task count = %d"), vInPoolTask.size());
		int nCommitCount = 0;
		//try strategy
		for (size_t i = 0; i < vInPoolTask.size(); ++i)
		{
			BOOL bSubmited = FALSE;
			ST_INPOOLTASKDATA& taskRef = vInPoolTask[i];
			if (taskRef.nPriority >= g_GlobalInfo.nEmergencyPriority)
			{
				CFWriteLog(TEXT("emergency task found pgmcode= %s, try emergency strategy"), taskRef.strPgmCode);
				//apply emergency strategy
				for (size_t j = 0; j < m_autoFun.m_stgEmergency.size(); ++j)
				{
					HRESULT hr = m_autoFun.ApplyStgs(taskRef, m_autoFun.m_stgEmergency[j]);
					if (SUCCEEDED(hr) && !taskRef.strTaskId.IsEmpty() && taskRef.strTaskId != TEXT("#"))
					{
						//task commited continue
						bSubmited = TRUE;
						++nCommitCount;
						SendMessage(MSG_EMBTASKCOMMITINAUTO, (WPARAM)&taskRef, 0);
						break;
					}
				}
			}

			if (bSubmited)
			{
				continue;
			}

			for (size_t j = 0; j < m_autoFun.m_stgOnline.size(); ++j)
			{
				HRESULT hr = m_autoFun.ApplyStgs(taskRef, m_autoFun.m_stgOnline[j]);
				if (SUCCEEDED(hr) && !taskRef.strTaskId.IsEmpty() && taskRef.strTaskId != TEXT("#"))
				{
					//task commited continue
					bSubmited = TRUE;
					++nCommitCount;
					SendMessage(MSG_EMBTASKCOMMITINAUTO, (WPARAM)&taskRef, 0);
					break;
				}
			}


			if (bSubmited)
			{
				continue;
			}

			if (bInIdleTime)
			{
				for (size_t j = 0; j < m_autoFun.m_stgIdle.size(); ++j)
				{
					HRESULT hr = m_autoFun.ApplyStgs(taskRef, m_autoFun.m_stgIdle[j]);
					if (SUCCEEDED(hr) && !taskRef.strTaskId.IsEmpty() && taskRef.strTaskId != TEXT("#"))
					{
						//task commited continue
						bSubmited = TRUE;
						++nCommitCount;
						SendMessage(MSG_EMBTASKCOMMITINAUTO, (WPARAM)&taskRef, 0);

						break;
					}
				}

			}

		}
		CFWriteLog(TEXT("auto task end use time = %d s..."), (time(NULL) - tmNow.GetTime())/1000);
		
		PostMessage(MSG_UPDATE_LASTINFO, (WPARAM)(vInPoolTask.size()), (LPARAM)nCommitCount);

	}
	PostMessage(MSG_UPDATE_AUTOINFO, msgupdatetype_stop, 0);
}


LRESULT CDlgAutoTask::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_UPDATE_AUTOINFO)
	{
		int nType = wParam;
		CString strMsg;
		if (nType == msgupdatetype_downcount)
		{
			int nSec = lParam;
			strMsg.Format(TEXT("下一次扫描倒计时：%02d:%02d"), nSec/60, nSec%60);
		}
		else if (nType == msgupdatetype_start)
		{
			strMsg.Format(TEXT("正在扫描在线任务..."));
			m_edtAutoLog.SetWindowText(TEXT(""));

		}
		else if (nType == msgupdatetype_stop)
		{
			strMsg.Format(TEXT("自动任务已停止..."));
		}

		UpdateAutoMsg(strMsg);
	}
	else if (message == MSG_UPDATE_LASTINFO)
	{
		CString strMsg;
		strMsg.Format(TEXT("最近一次扫描[%s]: \r\n在线任务数: %d\r\n提交任务数: %d"),Time2Str(time(NULL)), wParam, lParam);
		m_edtAutoLog.SetWindowText(strMsg);
	}
	else if (message == MSG_EMBTASKCOMMITINAUTO)
	{
		if (wParam && lParam)
		{
			ST_INPOOLTASKDATA& orgDataRef = *((ST_INPOOLTASKDATA*)wParam);
			CString& strStgNameRef = *((CString*)lParam);
			LogAutoTaskItem(orgDataRef);
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgAutoTask::InitListHeader()
{
	if (g_GlobalInfo.listInfoAutoTask.vListHeader.size() == 0)
	{
		ST_LISTHEADERITEM item;
		item.nId = mtaskcol_no;
		item.nWidth = 100;
		item.strName = TEXT("序号");
		m_vListCols.push_back(item);


		item.nId = mtaskcol_pgmcode;
		item.nWidth = 100;
		item.strName = TEXT("节目代码");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_embtaskid;
		item.nWidth = 100;
		item.strName = TEXT("任务ID");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_embsubmittime;
		item.nWidth = 100;
		item.strName = TEXT("EMB任务提交时间");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_stgname;
		item.nWidth = 100;
		item.strName = TEXT("应用策略");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_tplname;
		item.nWidth = 100;
		item.strName = TEXT("应用模板");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_clipname;
		item.nWidth = 100;
		item.strName = TEXT("节目名称");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_sourcetype;
		item.nWidth = 100;
		item.strName = TEXT("来源");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_submittime;
		item.nWidth = 100;
		item.strName = TEXT("提交时间");
		m_vListCols.push_back(item);



		item.nId = mtaskcol_playtime;
		item.nWidth = 100;
		item.strName = TEXT("素材播出时间");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_chid;
		item.nWidth = 100;
		item.strName = TEXT("频道ID");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_cliptype;
		item.nWidth = 100;
		item.strName = TEXT("素材类型");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_duration;
		item.nWidth = 100;
		item.strName = TEXT("长度");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_npri;
		item.nWidth = 100;
		item.strName = TEXT("紧急度");
		m_vListCols.push_back(item);


		item.nId = mtaskcol_version;
		item.nWidth = 100;
		item.strName = TEXT("版本");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_errcode;
		item.nWidth = 100;
		item.strName = TEXT("错误代码");
		m_vListCols.push_back(item);

		g_GlobalInfo.listInfoAutoTask.vListHeader = m_vListCols;
	}
	else
	{
		m_vListCols = g_GlobalInfo.listInfoAutoTask.vListHeader;
	}
	

	for (size_t i = 0; i < m_vListCols.size(); ++i)
	{
		ST_LISTHEADERITEM& itmRef = m_vListCols[i];
		m_lstTask.InsertColumn(i, itmRef.strName, LVCFMT_LEFT, itmRef.nWidth);
		if (itmRef.nId == mtaskcol_embtaskid)
		{
			m_nColPosTaskid = i;
		}
	}
}

void CDlgAutoTask::LogAutoTaskItem( ST_INPOOLTASKDATA& orgDataIn)
{

	int nItem = m_lstTask.InsertItem(m_lstTask.GetItemCount(), TEXT(""));
	CString strIdx;
	strIdx.Format(TEXT("%d"), nItem);
	m_lstTask.SetItemText(nItem, 0, strIdx);

	for (int j = 1; j < m_vListCols.size(); ++j)
	{
		if (m_vListCols[j].nId == mtaskcol_embsubmittime)
		{
			CString strText = Time2Str(time(NULL));
		}
		else
		{
			CString strText = GetTaskColText(m_vListCols[j].nId, orgDataIn);
			m_lstTask.SetItemText(nItem, j, strText);
		}
	}
}

void CDlgAutoTask::OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	LPNMLVCUSTOMDRAW plvCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult =CDRF_NOTIFYITEMDRAW;
		return;
	}
// 	else if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
// 	{
// 		*pResult =CDRF_NOTIFYSUBITEMDRAW;
// 		return;
// 
// 	}

	//if (pNMCD->dwDrawStage == (CDDS_ITEMPREPAINT|CDDS_SUBITEM))
	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		int nId = pNMCD->dwItemSpec;
		int nCol =plvCD->iSubItem;

		if( m_lstTask.GetItemState(nId, LVIS_SELECTED) & LVIS_SELECTED)
		{
			plvCD->clrTextBk = RGB(53,152,255);
			plvCD->clrText = RGB(255,255,255);
			*pResult = 0;
		}
		else
		{
			plvCD->clrTextBk = g_GlobalInfo.listInfoAutoTask.clrTextBk;
			plvCD->clrText = g_GlobalInfo.listInfoAutoTask.clrText;
		}

		if (m_nColPosTaskid >= 0 && nCol == 0)
		{
			CString strEmbTaskID = m_lstTask.GetItemText(nId, m_nColPosTaskid);
			if (strEmbTaskID == TEXT("#"))
			{
				plvCD->clrTextBk = g_GlobalInfo.listInfoAutoTask.clrErrTextBk;
				plvCD->clrText = g_GlobalInfo.listInfoAutoTask.clrErrText;

			}
		}
		
	}
}
