// DlgTopPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "MBCAutoSwitcher.h"
#include "DlgTopPanel.h"
#include "MainFrm.h"


#include "GlobalDef.h"
// CDlgTopPanel 对话框

IMPLEMENT_DYNAMIC(CDlgTopPanel, CDialog)

CDlgTopPanel::CDlgTopPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTopPanel::IDD, pParent)
{
	m_bRuning = FALSE;

}

CDlgTopPanel::~CDlgTopPanel()
{
}

void CDlgTopPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RUN, m_btnCtlRun);
	DDX_Control(pDX, IDC_BTN_TEST, m_btnTest);
}


BEGIN_MESSAGE_MAP(CDlgTopPanel, CDialog)
	ON_BN_CLICKED(IDC_BTN_RUN, &CDlgTopPanel::OnBnClickedBtnRun)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgTopPanel::OnBnClickedBtnTest)
END_MESSAGE_MAP()

// CDlgTopPanel 消息处理程序



void CDlgTopPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Relayout();
	// TODO: 在此处添加消息处理程序代码
}

void CDlgTopPanel::Relayout()
{
	if (m_btnCtlRun.GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcBtn(rcThis);
	rcBtn.right = rcBtn.left + rcBtn.Height();
	rcBtn.DeflateRect(5,5,5,5);
	m_btnCtlRun.MoveWindow(rcBtn);
	rcBtn.OffsetRect(rcBtn.Width() +5, 0);

}

void CDlgTopPanel::OnBnClickedBtnRun()
{
	// TODO: 在此添加控件通知处理程序代码.
	if (!g_pMainFrame)
	{
		return;
	}
	if (!m_bRuning)
	{

			m_bRuning = (g_pMainFrame->m_Mongraph.LoadGraph() == S_OK
				&&g_pMainFrame->m_Mongraph.Run() == S_OK);

	}
	else
	{

			g_pMainFrame->m_Mongraph.Stop();
			m_bRuning = FALSE;

	}

	m_btnCtlRun.SetWindowText(m_bRuning? "停止":"启动");
}


void CDlgTopPanel::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pMainFrame->m_Mongraph.Test();
}

BOOL CDlgTopPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
#ifndef _DEBUG
	m_btnTest.ShowWindow(SW_HIDE);
#endif
	m_btnCtlRun.SetWindowText("启动");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
