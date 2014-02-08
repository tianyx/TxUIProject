// DlgTaskFinished.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "DlgTaskFinished.h"
#include "MainDef.h"

// CDlgTaskFinished �Ի���

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


// CDlgTaskFinished ��Ϣ�������

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
	// TODO: �ڴ˴������Ϣ����������
}

BOOL CDlgTaskFinished::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lstTaskFinished.SetExtendedStyle(m_lstTaskFinished.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstTaskFinished.InsertColumn(0,TEXT("ִ��ʱ��"),LVCFMT_LEFT, 200);
	m_lstTaskFinished.InsertColumn(1, TEXT("��������"),LVCFMT_LEFT, 1000);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgTaskFinished::OnBnClickedBtnRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Refresh();
}

void CDlgTaskFinished::Refresh()
{
	m_lstTaskFinished.DeleteAllItems();
	MMAPTIMETASKS mmapTasks;
	GetTaskInFolder(g_GlobalInfo.szWorkFinishedDir, mmapTasks);
	if (mmapTasks.size() > 1000)
	{
		AfxMessageBox(TEXT("�����������������1000�����뼰ʱ������������񱸷�Ŀ¼"));
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
	// TODO: �ڴ����ר�ô����/����û���
	if (message == MSG_TASKSHOW)
	{
		Refresh();
		return TRUE;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
