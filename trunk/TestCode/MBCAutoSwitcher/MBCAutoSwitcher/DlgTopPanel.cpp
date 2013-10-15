// DlgTopPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MBCAutoSwitcher.h"
#include "DlgTopPanel.h"
#include "MainFrm.h"


#include "GlobalDef.h"
// CDlgTopPanel �Ի���

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

// CDlgTopPanel ��Ϣ�������



void CDlgTopPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Relayout();
	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������.
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

	m_btnCtlRun.SetWindowText(m_bRuning? "ֹͣ":"����");
}


void CDlgTopPanel::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pMainFrame->m_Mongraph.Test();
}

BOOL CDlgTopPanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
#ifndef _DEBUG
	m_btnTest.ShowWindow(SW_HIDE);
#endif
	m_btnCtlRun.SetWindowText("����");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
