// DlgRunningTask.cpp : 实现文件
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "DlgRunningTask.h"
#include "StrConvert.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum RunColDef
{
	Runcoltype_id  = 0,
	Runcoltype_name,
	Runcoltype_excId,
	Runcoltype_state,
	Runcoltype_speed,
	Runcoltype_percent,
	Runcoltype_retry

};


// CDlgRunningTask 对话框

IMPLEMENT_DYNAMIC(CDlgRunningTask, CDialog)

CDlgRunningTask::CDlgRunningTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRunningTask::IDD, pParent)
{

}

CDlgRunningTask::~CDlgRunningTask()
{
}

void CDlgRunningTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASKRUNNING, m_lstTask);
	DDX_Control(pDX, IDC_STATIC_INFO, m_stcInfo);
}


BEGIN_MESSAGE_MAP(CDlgRunningTask, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgRunningTask 消息处理程序

void CDlgRunningTask::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_lstTask.GetSafeHwnd())
	{
		return;
	}
	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcInfo(rcThis);
	rcInfo.bottom = rcInfo.top + 28;
	rcInfo.MoveToY(6);
	m_stcInfo.MoveWindow(rcInfo);
	rcThis.top += 36;
	m_lstTask.MoveWindow(rcThis);

	// TODO: 在此处添加消息处理程序代码
}

BOOL CDlgRunningTask::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lstTask.SetExtendedStyle(m_lstTask.GetExtendedStyle()| LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lstTask.InsertColumn(Runcoltype_id, TEXT("任务Id"), LVCFMT_LEFT, 0);
	m_lstTask.InsertColumn(Runcoltype_name, TEXT("任务名称"), LVCFMT_LEFT, 400);
	m_lstTask.InsertColumn(Runcoltype_excId, TEXT("执行线程ID"), LVCFMT_LEFT, 100);
	m_lstTask.InsertColumn(Runcoltype_state, TEXT("任务状态"), LVCFMT_LEFT, 250);
	m_lstTask.InsertColumn(Runcoltype_speed, TEXT("迁移速度KB/s"),LVCFMT_LEFT, 120);
	m_lstTask.InsertColumn(Runcoltype_percent, TEXT("完成百分比"), LVCFMT_LEFT, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CDlgRunningTask::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_TASKADD)
	{
		if (wParam)
		{
			MAPTASKS* pmapTask = (MAPTASKS*)wParam;
			AddTask(*pmapTask);
		}
	}
	else if (message == MSG_TASKUPDATEREALINFO)
	{
		if (wParam)
		{
			ST_REALTRASINFO* pInfo = (ST_REALTRASINFO*) wParam;
			ST_REALTRASINFO info = *pInfo;
			delete pInfo;
			UpdateRealTaskInfo(info);
		}

	}
	else if (message == MSG_TASKDEL)
	{
		if (wParam)
		{
			INT64 nTaskId = *(INT64*)wParam;
			DelTask(nTaskId);
		}
	}
	else if (message == MSG_INFOMSG)
	{
		if (wParam)
		{
			CString* pMsg = (CString*)wParam;
			m_stcInfo.SetWindowText(*pMsg);
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgRunningTask::UpdateRealTaskInfo( ST_REALTRASINFO& realInfo )
{
	CString strTaskId = CTxStrConvert(realInfo.nTaskId).GetAsString();
	if (strTaskId.IsEmpty())
	{
		return;
	}

	int nPos = -1;
	int nItemCount = m_lstTask.GetItemCount();
	for (int i = 0; i < nItemCount; ++i)
	{
		CString strIdItem =m_lstTask.GetItemText(i, Runcoltype_id );
		if (strIdItem.Compare(strTaskId) == 0)
		{
			nPos = i;
			break;
		}
	}
	
	if (nPos >= 0)
	{
		m_lstTask.SetItemText(nPos,  Runcoltype_excId, realInfo.nExcId < 0? TEXT(""): CTxStrConvert(realInfo.nExcId).GetAsString());
		m_lstTask.SetItemText(nPos,  Runcoltype_state, realInfo.strMsg);
		CString strSpeed;
		if (realInfo.nSpeedKBPerS > 1000)
		{
			strSpeed.Format(TEXT("%d,%03d KB/s"), realInfo.nSpeedKBPerS/1000, realInfo.nSpeedKBPerS%1000);
		}
		else
		{
			strSpeed = CTxStrConvert(realInfo.nSpeedKBPerS).GetAsString() + TEXT(" KB/s");
		}
		CString strPercent;
		strPercent.Format(TEXT("%d%%"), realInfo.nPercent);
		m_lstTask.SetItemText(nPos,  Runcoltype_speed, strSpeed);
		m_lstTask.SetItemText(nPos,  Runcoltype_percent,strPercent );
		m_lstTask.SetItemText(nPos,  Runcoltype_retry, CTxStrConvert(realInfo.nRetry).GetAsString());

	}
}

void CDlgRunningTask::AddTask( MAPTASKS& mapTaskIn )
{
	int nCount = m_lstTask.GetItemCount();
	MAPTASKS::iterator itb = mapTaskIn.begin();
	MAPTASKS::iterator ite = mapTaskIn.end();
	int nPos = nCount;
	for (; itb != ite; ++itb)
	{
		ST_TASKINFO& infoRef = itb->second;
		int nItem =m_lstTask.InsertItem(nPos, TEXT(""));
		ASSERT(nItem == nPos);
		m_lstTask.SetItemText(nItem,  Runcoltype_id, CTxStrConvert(infoRef.nTaskId).GetAsString());
		m_lstTask.SetItemText(nItem,  Runcoltype_name, CTxStrConvert(TxGetFtpFileName(infoRef.strTaskXMlFile)).GetAsString());
		m_lstTask.SetItemText(nItem,  Runcoltype_excId, TEXT(""));
		m_lstTask.SetItemText(nItem,  Runcoltype_state, TEXT("等待中"));
		m_lstTask.SetItemText(nItem,  Runcoltype_speed, TEXT(""));
		m_lstTask.SetItemText(nItem,  Runcoltype_percent, TEXT(""));
		m_lstTask.SetItemText(nItem,  Runcoltype_percent, CTxStrConvert(infoRef.nRetry).GetAsString());
		nPos ++;
	}


}

void CDlgRunningTask::ResetList()
{
	m_lstTask.DeleteAllItems();
}

void CDlgRunningTask::DelTask( INT64 nTaskIDIn )
{
	CString strTaskId = CTxStrConvert(nTaskIDIn).GetAsString();
	if (strTaskId.IsEmpty())
	{
		return;
	}

	int nPos = -1;
	int nItemCount = m_lstTask.GetItemCount();
	for (int i = 0; i < nItemCount; ++i)
	{
		CString strIdItem =m_lstTask.GetItemText(i, Runcoltype_id );
		if (strIdItem.Compare(strTaskId) == 0)
		{
			m_lstTask.DeleteItem(i);
			break;
		}
	}
}
