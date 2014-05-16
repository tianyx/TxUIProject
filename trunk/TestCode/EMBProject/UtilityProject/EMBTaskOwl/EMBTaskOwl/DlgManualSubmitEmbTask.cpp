// ManualAddEmbTask.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgManualSubmitEmbTask.h"


// CManualAddEmbTask 对话框

IMPLEMENT_DYNAMIC(CDlgManualSubmitEmbTask, CDialog)

CDlgManualSubmitEmbTask::CDlgManualSubmitEmbTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManualSubmitEmbTask::IDD, pParent)
{

}

CDlgManualSubmitEmbTask::~CDlgManualSubmitEmbTask()
{
}

void CDlgManualSubmitEmbTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
	DDX_Control(pDX, IDC_BTNSUBMIT, m_btnSubmit);
	DDX_Control(pDX, IDC_COMBO_TPL, m_cmbTpl);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, m_stcOutput);
}


BEGIN_MESSAGE_MAP(CDlgManualSubmitEmbTask, CDialog)
	ON_BN_CLICKED(IDC_BTNSUBMIT, &CDlgManualSubmitEmbTask::OnBnClickedBtnsubmit)
END_MESSAGE_MAP()


// CManualAddEmbTask 消息处理程序

void CDlgManualSubmitEmbTask::OnBnClickedBtnsubmit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTplName;
	m_cmbTpl.GetWindowText(strTplName);

	int nSel = m_cmbTpl.GetCurSel();
	if (nSel == -1)
	{
		AfxMessageBox(TEXT("请选择模板"));
		return;
	}

	ASSERT(!strTplName.IsEmpty());
	MAPTPLCONTENT::iterator itf = g_GlobalInfo.mapTaskTemplates.find(strTplName);

	if (itf == g_GlobalInfo.mapTaskTemplates.end())
	{
		AfxMessageBox(TEXT("模板未找到！"));
	}

	int nSucCount = 0;
	int nAllCount = 0;
	ST_EMBTEMPLATEINFO& tplRef = itf->second;
	int nSizeTask = m_pvOrgTasks->size();
	for (size_t i = 0; i< m_vSubmitIds.size(); ++i)
	{
		int nPos = m_vSubmitIds[i];
		if (nPos >= nSizeTask)
		{
			ASSERT(FALSE);
			continue;
		}

		ST_INPOOLTASKDATA& orgtaskRef = (*m_pvOrgTasks)[nPos];
		if (!orgtaskRef.strTaskId.IsEmpty())
		{
			continue;
		}
		nAllCount++;

		CAutoTaskFunc func;
		orgtaskRef.nErrcode = func.CommitEmbTask(orgtaskRef, tplRef);
		if (orgtaskRef.nErrcode ==S_OK)
		{
			nSucCount++;
			CFWriteLog(0, TEXT("manual submit task success, pgmcode = %s, embtaskid = '%s'"), orgtaskRef.strPgmCode, orgtaskRef.strTaskId);

		}
		else
		{
			CFWriteLog(0, TEXT("manual submit task failed, pgmcode = %s, err = %x"), orgtaskRef.strPgmCode, orgtaskRef.nErrcode);
		}
	}

	CString strMsg;
	strMsg.Format(TEXT("提交完毕，共%d个任务，成功%d个，失败%d个"), nAllCount, nSucCount, nAllCount - nSucCount);
	WriteOutput(strMsg);
	if (m_hwndNotify)
	{
		::SendMessage(m_hwndNotify, MSG_TASKINFOUPDATE, (WPARAM)&m_vSubmitIds,0);
	}
	AfxMessageBox(strMsg, MB_OK|MB_ICONINFORMATION);
}

BOOL CDlgManualSubmitEmbTask::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_lstTask.SetExtendedStyle( (m_lstTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));

	// TODO:  在此添加额外的初始化
	m_btnSubmit.EnableWindow(FALSE);

	InitListHeader();
	InitTask();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgManualSubmitEmbTask::InitTask()
{
	if (!m_pvOrgTasks)
	{
		ASSERT(FALSE);
		return;
	}

	if (g_GlobalInfo.mapTaskTemplates.size() == 0)
	{
		return;
	}

	MAPTPLCONTENT::iterator itb = g_GlobalInfo.mapTaskTemplates.begin();
	MAPTPLCONTENT::iterator ite = g_GlobalInfo.mapTaskTemplates.end();
	for (; itb != ite; ++itb)
	{
		m_cmbTpl.AddString(itb->first);
	}

	
	RefreshTaskList();

	m_btnSubmit.EnableWindow(m_lstTask.GetItemCount() > 0);

	WriteOutput(TEXT("共%d个任务可提交"), m_lstTask.GetItemCount());

}


void CDlgManualSubmitEmbTask::InitListHeader()
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

	item.nId = mtaskcol_clipname;
	item.nWidth = 100;
	item.strName = TEXT("节目名称");
	m_vListCols.push_back(item);

	item.nId = mtaskcol_playtime;
	item.nWidth = 100;
	item.strName = TEXT("素材播出时间");
	m_vListCols.push_back(item);

	item.nId = mtaskcol_embtaskid;
	item.nWidth = 100;
	item.strName = TEXT("任务ID");
	m_vListCols.push_back(item);

	item.nId = mtaskcol_errcode;
	item.nWidth = 100;
	item.strName = TEXT("错误代码");
	m_vListCols.push_back(item);

	for (size_t i = 0; i < m_vListCols.size(); ++i)
	{
		ST_LISTHEADERITEM& itmRef = m_vListCols[i];
		m_lstTask.InsertColumn(i, itmRef.strName, LVCFMT_LEFT, itmRef.nWidth);
	}

}

void CDlgManualSubmitEmbTask::RefreshTaskList()
{
	int nSizeTask = m_pvOrgTasks->size();


	for (size_t i = 0; i< m_vSubmitIds.size(); ++i)
	{
		int nPos = m_vSubmitIds[i];
		if (nPos >= nSizeTask)
		{
			ASSERT(FALSE);
			continue;
		}

		ST_INPOOLTASKDATA& orgtaskRef = (*m_pvOrgTasks)[nPos];

		CString strIdx;
		strIdx.Format(TEXT("%d"), i);
		m_lstTask.InsertItem(i, strIdx);
		for (int j = 1; j < m_vListCols.size(); ++j)
		{

			CString strText = GetTaskColText(m_vListCols[j].nId, orgtaskRef);
			m_lstTask.SetItemText(i, j, strText);
		}
	}
}

void CDlgManualSubmitEmbTask::WriteOutput( LPCTSTR format,... )
{
	const int LINE_BUFFER_SIZE = 4000;
	va_list ap;
	va_start(ap, format);

	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));

#ifdef _UNICODE
	vswprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	va_end(ap);
	_RPTW0(0, TEXT("\n"));
	_RPTW0(0, line);
#else
	vsprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	va_end(ap);
	_RPT0(0, TEXT("\n"));
	_RPT0(0, line);
#endif // _UNICODE

	m_stcOutput.SetWindowText(line);
}
