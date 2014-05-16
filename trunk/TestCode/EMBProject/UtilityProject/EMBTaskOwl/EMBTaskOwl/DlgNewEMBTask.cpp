// DlgNewEMBTask.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgNewEMBTask.h"


// CDlgNewEMBTask 对话框

IMPLEMENT_DYNAMIC(CDlgNewEMBTask, CDialog)

CDlgNewEMBTask::CDlgNewEMBTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewEMBTask::IDD, pParent)
{
	m_pClipInfo = NULL;
	nDefChnSel = 0;
}

CDlgNewEMBTask::~CDlgNewEMBTask()
{
}

void CDlgNewEMBTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PGMCODE, m_edtPgmCode);
	DDX_Control(pDX, IDC_EDIT_PGMNAME, m_edtPgmName);
	DDX_Control(pDX, IDC_COMBO_CHNID, m_cmbChnId);
	DDX_Control(pDX, IDC_EDIT_CLIPID, m_edtClipId);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_edtFilename);
	DDX_Control(pDX, IDC_LIST_FTP, m_lstSrcFtp);
	DDX_Control(pDX, IDC_COMBO_TPL, m_cmbTpl);

	// 	DDX_Control(pDX, IDC_TCODE_SOM, m_tcCtrlSom);
	// 	DDX_Control(pDX, IDC_TCODE_EOM, m_tcCtrlEom);
	// 	DDX_Control(pDX, IDC_TCODE_LENGTH, m_tcCtrlLength);

	DDX_Control(pDX, IDC_DTIMEPGMPLAY, m_dtCtrlPlaytime);
}


BEGIN_MESSAGE_MAP(CDlgNewEMBTask, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNewEMBTask::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CHANGECLIPID, &CDlgNewEMBTask::OnBnClickedBtnChangeclipid)
END_MESSAGE_MAP()

void CDlgNewEMBTask::InitInfo()
{
	int nDuration = 25*60;

	INT64 tmPlaytime = time(NULL);

	if (m_pClipInfo != NULL)
	{
		m_edtPgmCode.SetWindowText(m_pClipInfo->strPgmCode);
		m_edtPgmCode.EnableWindow(FALSE);
		m_edtPgmName.SetWindowText(m_pClipInfo->strPgmName);
		nDuration = m_pClipInfo->nPgmDuration;
		tmPlaytime = m_pClipInfo->tPgmPlaytime;
	}
	CTime tmPlay(tmPlaytime);
	m_dtCtrlPlaytime.SetTime(&tmPlay);

	m_cmbChnId.SetCurSel(nDefChnSel);

	CString strClipid = GenClipIDFromDB();
	ASSERT(!strClipid.IsEmpty());
	m_edtClipId.SetWindowText(strClipid);
	m_ptcCtrlLength->SetTimecode(nDuration);
	m_ptcCtrlSom->SetTimecode(0);
	m_ptcCtrlEom->SetTimecode(nDuration);

}


// CDlgNewEMBTask 消息处理程序

void CDlgNewEMBTask::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ST_INPOOLTASKDATA preData;
	preData.nClipSrctype = 0;
	preData.nClipType = 0;
	preData.nHDType = 0;
	
	m_edtPgmCode.GetWindowText(preData.strPgmCode);
	if (preData.strPgmCode.IsEmpty())
	{
		AfxMessageBox(TEXT("请填入节目代码!"));
		return;
	}
	
	int nChnSel = m_cmbChnId.GetCurSel();
	preData.strChID = (nChnSel >= 0 && nChnSel < g_GlobalInfo.vChnInfo.size())? g_GlobalInfo.vChnInfo[nChnSel].strChnId: TEXT("");
	if (preData.strChID.IsEmpty())
	{
		AfxMessageBox(TEXT("频道信息不正确！"));
		return;
	}

	m_edtClipId.GetWindowText(preData.strClipID);
	if (preData.strClipID.IsEmpty())
	{
		AfxMessageBox(TEXT("素材ID不正确！"));
		return;

	}

	preData.nDuration = m_ptcCtrlLength->GetTimecode();
	preData.nSom = m_ptcCtrlSom->GetTimecode();
	preData.nEom = m_ptcCtrlEom->GetTimecode();
	if (preData.nDuration == 0 || preData.nSom > preData.nEom
		||preData.nSom > preData.nDuration || preData.nEom > preData.nDuration)
	{
		AfxMessageBox(TEXT("入出点，时长不正确！"));
		return;
	}

	CTime tmPlay;
	m_dtCtrlPlaytime.GetTime(tmPlay);
	preData.tmPlayTime = tmPlay.GetTime();
	preData.tmPlayTimeInPGM = preData.tmPlayTime;

	m_edtPgmName.GetWindowText(preData.strClipName);
	
	m_edtFilename.GetWindowText(preData.strFileName);
	if (preData.strFileName.IsEmpty())
	{
		AfxMessageBox(TEXT("素材文件为空！"));
		return;
	}

	int nFtpCount = m_lstSrcFtp.GetItemCount();
	for (size_t i = 0; i < nFtpCount; ++i)
	{
		if (m_lstSrcFtp.GetCheck(i))
		{
			ST_FTPSITEINFO* pInfo = (ST_FTPSITEINFO*)m_lstSrcFtp.GetItemData(i);
			preData.vFtpSrc.push_back(*pInfo);
		}
	}
	

	if (preData.vFtpSrc.size() == 0)
	{
		AfxMessageBox(TEXT("请设定素材来源ftp"));
		return;
	}

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
		return;
	}

	ST_EMBTEMPLATEINFO& tplRef = itf->second;

	CAutoTaskFunc func;
	preData.nErrcode = func.CommitEmbTask(preData, tplRef, FALSE);
	if (preData.nErrcode ==S_OK)
	{
		CFWriteLog(0, TEXT("manual submit task success, pgmcode = %s, embtaskid = '%s'"), preData.strPgmCode, preData.strTaskId);
		
	}
	else
	{
		CFWriteLog(0, TEXT("manual submit task failed, pgmcode = %s, err = %x"), preData.strPgmCode, preData.nErrcode);
	}

	if (m_pClipInfo)
	{
		m_pClipInfo->nInPoolErrCode = preData.nErrcode;
		m_pClipInfo->strClipID = preData.strClipID;
		m_pClipInfo->strEmbTaskId = preData.strTaskId;
	}
	
	if (m_hwndNotify)
	{
		if (m_hwndNotify)
		{
			::SendMessage(m_hwndNotify, MSG_NEWEMBTASKINFOUPDATE, (WPARAM)&preData, (LPARAM)m_pClipInfo);
		}
	}


	OnOK();
}

void CDlgNewEMBTask::OnBnClickedBtnChangeclipid()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strClip = GenClipIDFromDB();
	m_edtClipId.SetWindowText(strClip);
}

void CDlgNewEMBTask::InitCtrl()
{
	m_edtPgmCode.LimitText(32);
	m_edtFilename.LimitText(126);
	m_edtClipId.LimitText(8);
	m_edtPgmName.LimitText(126);

	for (size_t i = 0; i < g_GlobalInfo.vChnInfo.size(); ++i)
	{
		m_cmbChnId.AddString(g_GlobalInfo.vChnInfo[i].strChnName);
	}
	m_cmbChnId.SetCurSel(0);


	MAPGPFTPSITES::iterator itsb = g_GlobalInfo.mapFtpSites.begin();
	MAPGPFTPSITES::iterator itse = g_GlobalInfo.mapFtpSites.end();
	int nIdx = 0;
	for (; itsb != itse; itsb++)
	{
		VECFTPSITES& vSitesRef = itsb->second;
		for (size_t i = 0; i < vSitesRef.size(); ++i)
		{
			ST_FTPSITEINFO& ftpInfoRef = vSitesRef[i];
			CString strIdx;
			strIdx.Format(TEXT("%d"), nIdx++);
			int nItem = m_lstSrcFtp.InsertItem(nIdx, strIdx);
			m_lstSrcFtp.SetItemData(nItem, (DWORD_PTR)&ftpInfoRef);

			for (int j = 1; j < m_vListCols.size(); ++j)
			{
				CString strText = GetFtpListColText(m_vListCols[j].nId, itsb->first, ftpInfoRef);
				m_lstSrcFtp.SetItemText(nItem, j, strText);
			}
		}
	}


	if (g_GlobalInfo.mapTaskTemplates.size() > 0)
	{
		MAPTPLCONTENT::iterator itb = g_GlobalInfo.mapTaskTemplates.begin();
		MAPTPLCONTENT::iterator ite = g_GlobalInfo.mapTaskTemplates.end();
		for (; itb != ite; ++itb)
		{
			m_cmbTpl.AddString(itb->first);
		}
	}

	
}

void CDlgNewEMBTask::InitFtpListHeader()
{
	ST_LISTHEADERITEM item;
	item.nId = colftp_no;
	item.nWidth = 100;
	item.strName = TEXT("序号");
	m_vListCols.push_back(item);


	item.nId = colftp_group;
	item.nWidth = 100;
	item.strName = TEXT("类别");
	m_vListCols.push_back(item);

	item.nId = colftp_name;
	item.nWidth = 100;
	item.strName = TEXT("名称");
	m_vListCols.push_back(item);

	item.nId = colftp_ip;
	item.nWidth = 100;
	item.strName = TEXT("地址");
	m_vListCols.push_back(item);

	item.nId = colftp_port;
	item.nWidth = 100;
	item.strName = TEXT("端口");
	m_vListCols.push_back(item);

	item.nId = colftp_mode;
	item.nWidth = 100;
	item.strName = TEXT("模式");
	m_vListCols.push_back(item);

	item.nId = colftp_user;
	item.nWidth = 100;
	item.strName = TEXT("用户");
	m_vListCols.push_back(item);

	item.nId = colftp_pw;
	item.nWidth = 100;
	item.strName = TEXT("密码");
	m_vListCols.push_back(item);

	for (size_t i = 0; i < m_vListCols.size(); ++i)
	{
		ST_LISTHEADERITEM& itmRef = m_vListCols[i];
		m_lstSrcFtp.InsertColumn(i, itmRef.strName, LVCFMT_LEFT, itmRef.nWidth);
	}
}

BOOL CDlgNewEMBTask::OnInitDialog()
{
	CDialog::OnInitDialog();
	DateTime_SetFormat(m_dtCtrlPlaytime.GetSafeHwnd(), TEXT("yyyy-MM-dd HH:mm:ss"));
	m_lstSrcFtp.SetExtendedStyle( (m_lstSrcFtp.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));

	// TODO:  在此添加额外的初始化
	InitFtpListHeader();

 	InitCtrl();

	m_ptcCtrlLength = (CMxtTimecodeCtrl*)GetDlgItem(IDC_TCODE_LENGTH);
	m_ptcCtrlSom = (CMxtTimecodeCtrl*)GetDlgItem(IDC_TCODE_SOM);
	m_ptcCtrlEom = (CMxtTimecodeCtrl*)GetDlgItem(IDC_TCODE_EOM);

	InitInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
