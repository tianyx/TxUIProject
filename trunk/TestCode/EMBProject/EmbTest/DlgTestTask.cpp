// DlgTestTask.cpp : implementation file
//

#include "stdafx.h"
#include "EmbTest.h"
#include "DlgTestTask.h"

#include "TxParamString.h"
#include "EmbStructDef.h"
#include "TxADOConn.h"
#include "EmbTestDlg.h"
#include "MainDef.h"

#include "DlgTaskQuery.h"
// CDlgTestTask dialog

IMPLEMENT_DYNAMIC(CDlgTestTask, CDialog)

CDlgTestTask::CDlgTestTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestTask::IDD, pParent)
{
	m_nTaskType = 0;
	CString strPath = GetAppPath().c_str();
	m_vtaskfile[0] = strPath + "\\" + "template.xml";
	m_vtaskfile[1] = strPath + "\\" + "templatejs.xml";
	m_vtaskfile[2] = strPath + "\\" + "templatejssec.xml";
	m_vtaskfile[3] = strPath + "\\" + "templatetran.xml";
	m_vtaskfile[4] = strPath + "\\" + "templatetranmd5.xml";

}

CDlgTestTask::~CDlgTestTask()
{
}

void CDlgTestTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DBSTR, m_edtDbconn);
	DDX_Control(pDX, IDC_EDIT1, m_edtSrcfilename);
	DDX_Control(pDX, IDC_RADIO1, m_rdoTasktype);
	DDX_Control(pDX, IDC_LIST1, m_lstTask);
}


BEGIN_MESSAGE_MAP(CDlgTestTask, CDialog)
	ON_BN_CLICKED(IDC_BTN_AUTOGENTASK, &CDlgTestTask::OnBnClickedBtnAutogentask)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgTestTask::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgTestTask::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDlgTestTask::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON_DELTASK, &CDlgTestTask::OnBnClickedButtonDeltask)
	ON_BN_CLICKED(IDC_BUTTON_DELTASKLOG, &CDlgTestTask::OnBnClickedButtonDeltasklog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CDlgTestTask::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTestTask::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_RADIO4, &CDlgTestTask::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CDlgTestTask::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_BTN_OTHER, &CDlgTestTask::OnBnClickedBtnOther)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgTestTask::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgTestTask message handlers

void CDlgTestTask::OnBnClickedBtnAutogentask()
{
	// TODO: Add your control notification handler code here
	CString strSrcfile;
	m_edtSrcfilename.GetWindowText(strSrcfile);
	if (strSrcfile.IsEmpty())
	{
		AfxMessageBox("src file is empty");
		return;
	}
	CString strConn;
	m_edtDbconn.GetWindowText(strConn);
	if(CTxADODBMgr::GetADODBMgr()->OpenDB(1, strConn))
	{
		//SetTimer(1000, 10000, NULL); // 间隔为3分钟 
		OnBnClickedBtnTestado();
		CTxADODBMgr::ReleaseDBMgr();
	}
}

void CDlgTestTask::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	m_nTaskType = 0;
}

void CDlgTestTask::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	m_nTaskType =1;
}

void CDlgTestTask::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	m_nTaskType =2;
}
CString GenClipId2()
{
	srand(time(NULL));
	static UINT64 nSeed =rand();
	nSeed++;
	CString strRet;
	strRet.Format(TEXT("%08d"), nSeed%100000000);
	return strRet;
}

void CDlgTestTask::OnBnClickedBtnTestado()
{
	CString strTaskGuid;
	GUID   guid; 
	if (S_OK != ::CoCreateGuid(&guid)) 
	{
		return;
	}

	strTaskGuid = Guid2String(guid);

	// TODO: 在此添加控件通知处理程序代码


	CTxADOCommand command;
	VECSTRINGS vStr;

	CFile file;
	CString strFileAuto = GetAppPath().c_str();
	strFileAuto += TEXT("\\autotemplate.xml");
	CString strXml;

	if(file.Open(m_vtaskfile[m_nTaskType], CFile::modeRead, NULL))
	{
		int nLen = file.GetLength();
		char szbuff[19096];
		ZeroMemory(szbuff, 19096);
		file.Read(szbuff, 19096);
		file.Close();
		strXml = szbuff;
	}

	CString strSrcfile;
	m_edtSrcfilename.GetWindowText(strSrcfile);

	if (strXml.IsEmpty() )
	{
		return;
	}



	CString strClipId = GenClipId2();
	strXml.Replace("{TaskGuid}", strTaskGuid);
	strXml.Replace("{TaskDesFile}", strClipId);
	strXml.Replace("{TaskClipID}", strClipId);
	strXml.Replace("{MxfName}", strSrcfile);

	CString strTmp;
	strTmp.Format(TEXT("insert into T_EMBTask (strTaskID, nPRI, nState,strExtendInfoXml, nRetry, nDeleted) values ('%s', '80','0', '%s', '1','0')"), strTaskGuid, strXml);

	CTxADORecordSet rs;
	BOOL bSuc = rs.ExecuteSQL(strTmp);
	if (!bSuc)
	{
		AfxMessageBox("ExecuteSQLfail");
		CFWriteLog(0, "add task failed");
	}

	//CTxADODBMgr::ReleaseDBMgr();
}


BOOL CDlgTestTask::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstTask.SetExtendedStyle( (m_lstTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));

	m_edtDbconn.SetWindowText(g_GlobalInfo.szDBConn);
	// TODO:  Add extra initialization here
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
	m_lstTask.InsertColumn(0, "NO.", LVCFMT_LEFT, 100);
	m_lstTask.InsertColumn(1, "taskID", LVCFMT_LEFT, 100);
	m_lstTask.InsertColumn(2, "state", LVCFMT_LEFT, 50);
	m_lstTask.InsertColumn(3, "actor", LVCFMT_LEFT, 50);
	m_lstTask.InsertColumn(4, "开始", LVCFMT_LEFT, 160);
	m_lstTask.InsertColumn(5, "结束", LVCFMT_LEFT, 160);
	m_lstTask.InsertColumn(6, "统计", LVCFMT_LEFT, 160);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgTestTask::OnBnClickedButtonDeltask()
{
	// TODO: Add your control notification handler code here
	if (AfxMessageBox("确定删除？", MB_OKCANCEL) !=IDOK)
	{
		return;
	}
	m_lstTask.DeleteAllItems();
	CString strConn;
	m_edtDbconn.GetWindowText(strConn);
	if(CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_EMB, strConn))
	{
		//SetTimer(1000, 10000, NULL); // 间隔为3分钟 
		DelTask();
		CTxADODBMgr::ReleaseDBMgr();
	}
}

void CDlgTestTask::OnBnClickedButtonDeltasklog()
{
	if (AfxMessageBox("确定删除？", MB_OKCANCEL) !=IDOK)
	{
		return;
	}
	// TODO: Add your control notification handler code here
	CString strConn;
	m_edtDbconn.GetWindowText(strConn);
	if(CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_EMB, strConn))
	{
		//SetTimer(1000, 10000, NULL); // 间隔为3分钟 
		DelTaskLog();
		CTxADODBMgr::ReleaseDBMgr();
	}
}

void CDlgTestTask::OnBnClickedButtonQuery()
{
	m_lstTask.DeleteAllItems();
	// TODO: Add your control notification handler code here
	CString strConn;
	m_edtDbconn.GetWindowText(strConn);
	if(CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_EMB, strConn))
	{
		//SetTimer(1000, 10000, NULL); // 间隔为3分钟 
		QueryTask();
		CTxADODBMgr::ReleaseDBMgr();
	}
}

void CDlgTestTask::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	m_lstTask.DeleteAllItems();
}

void CDlgTestTask::QueryTask()
{
	CString strTmp;
	strTmp.Format(TEXT("select * from T_EMBTask"));

	CTxADORecordSet rs;
	BOOL bOk = rs.ExecuteSQL(strTmp);
	if (!bOk)
	{
		AfxMessageBox("查询失败！");
	}

	while(!rs.IsEOF())
	{

		CString strTaskId = rs.GetVal("strTaskID").GetAsString();
		CString strActor = rs.GetVal("nActorID").GetAsString();
		CString strState = rs.GetVal("nState").GetAsString();
		CString strStartTime = Time2Str(rs.GetVal("tTimeStartRun").GetAsInt64());
		CString strEndTime = Time2Str(rs.GetVal("tTimeEndRun").GetAsInt64());
		CString strtype = rs.GetVal("tTimeEndRun").GetAsString();
		CString strTotal;
		if (!strStartTime.IsEmpty() && !strEndTime.IsEmpty())
		{
			CTime tmStart = Str2Time(strStartTime);
			CTime tmEnd = Str2Time(strEndTime);
			CTimeSpan tmSpan = tmEnd - tmStart;
			strTotal = tmSpan.Format("%H:%M:%S");
		}
		int nPos = m_lstTask.GetItemCount();
		CString strIdx;
		strIdx.Format(TEXT("%d"), strIdx);
		m_lstTask.InsertItem(nPos, strIdx);
		m_lstTask.SetItemText(nPos, 1, strTaskId);
		m_lstTask.SetItemText(nPos, 2, strState);
		m_lstTask.SetItemText(nPos, 3, strActor);
		m_lstTask.SetItemText(nPos, 4, strStartTime);
		m_lstTask.SetItemText(nPos, 5, strEndTime);
		m_lstTask.SetItemText(nPos, 6, strTotal);
		rs.MoveNext();

	}
}

void CDlgTestTask::DelTask()
{
	CString strTmp;
	//strTmp.Format(TEXT("insert into T_EMBTask (strTaskID, nPRI, nState,strExtendInfoXml, nRetry) values ('%s', '80','0', '%s', '1')"), strTaskGuid, strXml);
	strTmp.Format(TEXT("delete from T_EMBTask"));
	CTxADORecordSet rs;
	rs.ExecuteSQL(strTmp);
}

void CDlgTestTask::DelTaskLog()
{
	CString strTmp;
	//strTmp.Format(TEXT("insert into T_EMBTask (strTaskID, nPRI, nState,strExtendInfoXml, nRetry) values ('%s', '80','0', '%s', '1')"), strTaskGuid, strXml);
	strTmp.Format(TEXT("delete from T_EMBTaskLog"));
	CTxADORecordSet rs;
	rs.ExecuteSQL(strTmp);
}

void CDlgTestTask::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	m_nTaskType = 3;
}

void CDlgTestTask::OnBnClickedRadio5()
{
	// TODO: Add your control notification handler code here
	m_nTaskType = 4;
}

void CDlgTestTask::OnBnClickedBtnOther()
{
	// TODO: 在此添加控件通知处理程序代码
	CEmbTestDlg dlg;
	dlg.DoModal();
}

void CDlgTestTask::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	*pResult = 0;
	LVHITTESTINFO hitInfo;
	hitInfo.pt = pNMItemActivate->ptAction;
	m_lstTask.SubItemHitTest(&hitInfo);
	if (hitInfo.flags & LVHT_ONITEMLABEL)
	{

		CString strText = m_lstTask.GetItemText(hitInfo.iItem, hitInfo.iSubItem);
		if(!strText.IsEmpty() &&OpenClipboard())
		{
			int nBufLen = strText.GetLength()+1;
			HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, nBufLen);
			if (hmem)
			{
				void* pBuf = GlobalLock(hmem);
				memset(pBuf, 0, nBufLen);
				strcpy((char*)pBuf, strText);
				GlobalUnlock(hmem);
				EmptyClipboard();
				HANDLE hret = SetClipboardData(CF_TEXT, hmem);
				ASSERT(hret != NULL);
			}

			CloseClipboard();
		}

		if (((GetKeyState(VK_LSHIFT) & 0x8000) != 0))
		{
			if (hitInfo.iItem >= 0)
			{
				CDlgTaskQuery dlg;
				CString strTaskId = m_lstTask.GetItemText(hitInfo.iItem, 1);
				CString strEmbID;
				if (!strTaskId.IsEmpty() && strTaskId != TEXT("#"))
				{
					strEmbID = strTaskId;
				}
				CString strConn;
				m_edtDbconn.GetWindowText(strConn);
				if(CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_EMB, strConn))
				{
					dlg.InitQueryData(NULL, strEmbID);
					dlg.DoModal();
					CTxADODBMgr::ReleaseDBMgr();
				}
				
			}
			return;
		}
	}

}
