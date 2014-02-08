// DlgTaskErr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "DlgTaskErr.h"
#include "FGlobal.h"

// CDlgTaskErr �Ի���

IMPLEMENT_DYNAMIC(CDlgTaskErr, CDialog)

CDlgTaskErr::CDlgTaskErr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTaskErr::IDD, pParent)
{

}

CDlgTaskErr::~CDlgTaskErr()
{
}

void CDlgTaskErr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASKERR, m_lstTaskErr);
	DDX_Control(pDX, IDC_EDIT1, m_editTaskLog);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
}


BEGIN_MESSAGE_MAP(CDlgTaskErr, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CDlgTaskErr::OnBnClickedBtnRefresh)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TASKERR, &CDlgTaskErr::OnLvnItemchangedListTaskerr)
END_MESSAGE_MAP()


// CDlgTaskErr ��Ϣ�������

void CDlgTaskErr::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_lstTaskErr.GetSafeHwnd())
	{
		return;
	}

	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcBtnRefresh(0,0,90,32);
	rcBtnRefresh.MoveToXY(rcThis.right - rcBtnRefresh.Width()-4, 2);
	m_btnRefresh.MoveWindow(rcBtnRefresh);
	rcThis.DeflateRect(0,rcBtnRefresh.bottom +2,0,0);
	
	CRect rcLst(rcThis);
	rcLst.DeflateRect(0,0,0,200);
	m_lstTaskErr.MoveWindow(rcLst);

	CRect rcEdit(rcThis);
	rcEdit.top = rcEdit.bottom -200;
	m_editTaskLog.MoveWindow(rcEdit);
	// TODO: �ڴ˴������Ϣ����������
}

BOOL CDlgTaskErr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_lstTaskErr.SetExtendedStyle(m_lstTaskErr.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstTaskErr.InsertColumn(0,TEXT("ִ��ʱ��"),LVCFMT_LEFT, 200);
	m_lstTaskErr.InsertColumn(1,TEXT("��������"),LVCFMT_LEFT, 1000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgTaskErr::OnBnClickedBtnRefresh()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Refresh();
}

void CDlgTaskErr::Refresh()
{
	m_lstTaskErr.DeleteAllItems();
	mmapTasks.clear();
	GetTaskInFolder(g_GlobalInfo.szWorkErrDir, mmapTasks);
	if (mmapTasks.size() > 1000)
	{
		AfxMessageBox(TEXT("��ʧ��������������1000�����뼰ʱ������ʧ�����񱸷�Ŀ¼"));
	}
	MMAPTIMETASKS::iterator itb = mmapTasks.begin();
	MMAPTIMETASKS::iterator ite = mmapTasks.end();
	for (int i = 0; itb != ite; ++itb, ++i)
	{
		CTime tmModify(itb->first);
		m_lstTaskErr.InsertItem(i, tmModify.Format(TEXT("%Y-%m-%d %H:%M:%S")));
		m_lstTaskErr.SetItemText(i, 1, TxGetFileTitle(itb->second));
	}
}

void CDlgTaskErr::OnLvnItemchangedListTaskerr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if ((pNMLV->uNewState & LVIS_SELECTED) ||  ((pNMLV->uOldState & LVIS_SELECTED) && (pNMLV->uNewState&LVIS_SELECTED) == 0))
	{
		static int nsLastSel = -1;
		int nSel = pNMLV->iItem;
		if ((pNMLV->uNewState & LVIS_SELECTED)== 0)
		{
			nSel= -1;
		}
		if (nSel != nsLastSel)
		{
			nsLastSel = nSel;
			SendMessage(MSG_ERRSELCHANGED, (WPARAM)nSel, 0);
		}
	}
	
	

	*pResult = 0;
}

LRESULT CDlgTaskErr::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message == MSG_ERRSELCHANGED)
	{
		int nSel = (int)wParam;
		int nSize = mmapTasks.size();
		if (nSel < 0 || nSel > mmapTasks.size() -1)
		{
			m_editTaskLog.SetWindowText(TEXT(""));
		}
		else
		{
			MMAPTIMETASKS::iterator itb = mmapTasks.begin();
			for (int i = 0; i < nSel; ++i, ++itb)
			{
			}
			CString strFile = itb->second;
			ShowLogFile(strFile);
		}
		return TRUE;
	}
	else if (message == MSG_TASKSHOW)
	{
		Refresh();
		return TRUE;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgTaskErr::ShowLogFile( CString& strLogIn )
{
	if (_access(strLogIn, 04) == -1)
	{
		CFWriteLog(0, TEXT("�޷������ļ�:%s"), strLogIn);
		return;
	}

	CFile file;
	if (file.Open(strLogIn, CFile::modeRead, NULL))
	{
		long nLen = file.GetLength();
		if (nLen > 0 && nLen < 0x00ffffff)
		{
			char* buff = new char[nLen];
			ZeroMemory(buff, nLen);
			long nRead = file.Read(buff, nLen);
			ASSERT(nRead == nLen);
			m_editTaskLog.SetWindowText(buff);
			delete buff;
		}
		file.Close();
	}
}
