// DlgTaskFinished.cpp : 实现文件
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "DlgTaskFinished.h"
#include "MainDef.h"

// CDlgTaskFinished 对话框

IMPLEMENT_DYNAMIC(CDlgTaskFinished, CDialog)

CDlgTaskFinished::CDlgTaskFinished(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTaskFinished::IDD, pParent)
{

}

CDlgTaskFinished::~CDlgTaskFinished()
{
}

void CDlgTaskFinished::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASKFINISHED, m_lstTaskFinished);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
}


BEGIN_MESSAGE_MAP(CDlgTaskFinished, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CDlgTaskFinished::OnBnClickedBtnRefresh)
END_MESSAGE_MAP()


// CDlgTaskFinished 消息处理程序

void CDlgTaskFinished::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_lstTaskFinished.GetSafeHwnd())
	{
		return;
	}

	CRect rcBtnRefresh(0,0,90,32);

	CRect rcThis;
	GetClientRect(rcThis);
	rcBtnRefresh.MoveToXY(rcThis.right - rcBtnRefresh.Width()-4, 2);
	m_btnRefresh.MoveWindow(rcBtnRefresh);
	rcThis.DeflateRect(0,rcBtnRefresh.bottom +2,0,0);
	m_lstTaskFinished.MoveWindow(rcThis);
	// TODO: 在此处添加消息处理程序代码
}

BOOL CDlgTaskFinished::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lstTaskFinished.SetExtendedStyle(m_lstTaskFinished.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstTaskFinished.InsertColumn(0,TEXT("执行时间"),LVCFMT_LEFT, 200);
	m_lstTaskFinished.InsertColumn(1, TEXT("任务名称"),LVCFMT_LEFT, 1000);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTaskFinished::OnBnClickedBtnRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	Refresh();
}

void CDlgTaskFinished::Refresh()
{
	m_lstTaskFinished.DeleteAllItems();
	MMAPTIMETASKS mmapTasks;
	GetTaskInFolder(g_GlobalInfo.szWorkFinishedDir, mmapTasks);
	if (mmapTasks.size() > 1000)
	{
		AfxMessageBox(TEXT("已完成任务数量超过1000条，请及时清理已完成任务备份目录"));
	}

	MMAPTIMETASKS::iterator itb = mmapTasks.begin();
	MMAPTIMETASKS::iterator ite = mmapTasks.end();
	for (int i = 0; itb != ite; ++itb, ++i)
	{
		CTime tmModify(itb->first);
		m_lstTaskFinished.InsertItem(i, tmModify.Format(TEXT("%Y-%m-%d %H:%M:%S")));
		m_lstTaskFinished.SetItemText(i, 1, TxGetFileTitle(itb->second));
	}
}

LRESULT CDlgTaskFinished::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_TASKSHOW)
	{
		Refresh();
		return TRUE;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
