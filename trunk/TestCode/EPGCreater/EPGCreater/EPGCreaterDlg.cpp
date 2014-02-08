
// EPGCreaterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EPGCreater.h"
#include "EPGCreaterDlg.h"
#include "DlgTaskEdit.h"
#include "StrConvert.h"
#include "FGlobal.h"
#include "TxADOConn.h"
#include "Markup.h"
#include "DlgGetDateRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXOUTPUTLENGTH 10240000
#define IDTIMER_AUTOGENTASK 1041

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEPGCreaterDlg �Ի���




CEPGCreaterDlg::CEPGCreaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEPGCreaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEPGCreaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TASKNEW, m_btnTaskNew);
	DDX_Control(pDX, IDC_BTN_TASKEDIT, m_btnTaskEdit);
	DDX_Control(pDX, IDC_BTN_TASKDEL, m_btnTaskDel);
	DDX_Control(pDX, IDC_BTN_GENMANUAL, m_btnTaskGenNow);
	DDX_Control(pDX, IDC_CHECK_GENONTIME, m_chkEnableTimeGen);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
}

BEGIN_MESSAGE_MAP(CEPGCreaterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_TASKNEW, &CEPGCreaterDlg::OnBnClickedBtnTasknew)
	ON_BN_CLICKED(IDC_BTN_TASKEDIT, &CEPGCreaterDlg::OnBnClickedBtnTaskedit)
	ON_BN_CLICKED(IDC_BTN_TASKDEL, &CEPGCreaterDlg::OnBnClickedBtnTaskdel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TASK, &CEPGCreaterDlg::OnLvnItemchangedListTask)
	ON_BN_CLICKED(IDC_BTN_GENMANUAL, &CEPGCreaterDlg::OnBnClickedBtnGenmanual)
	ON_BN_CLICKED(IDC_CHECK_GENONTIME, &CEPGCreaterDlg::OnBnClickedCheckGenontime)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TASK, &CEPGCreaterDlg::OnNMCustomdrawListTask)
END_MESSAGE_MAP()


// CEPGCreaterDlg ��Ϣ�������

BOOL CEPGCreaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_hwndLog = GetSafeHwnd();

	m_lstTask.SetExtendedStyle( (m_lstTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));
	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	m_edtLog.SetLimitText(MAXOUTPUTLENGTH);

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//init list
	m_lstTask.InsertColumn(0, TEXT("���"), LVCFMT_LEFT, 50);
	m_lstTask.InsertColumn(1, TEXT("Ƶ��"), LVCFMT_LEFT, 150);
	m_lstTask.InsertColumn(2, TEXT("����"), LVCFMT_LEFT, 100);
	m_lstTask.InsertColumn(3, TEXT("��һ������Ŀ��"), LVCFMT_CENTER, 150);
	m_lstTask.InsertColumn(4, TEXT("��һ������ʱ��"), LVCFMT_CENTER, 150);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//init data from file
	if (_access(g_GlobalInfo.szEpgDatafile, 04) != -1)
	{
		//load from file
		if (!SerializeEPGData(TRUE))
		{
			AfxMessageBox(TEXT("����EPG��������ʧ��"));
		}
	}

	for (size_t i = 0; i < m_epgData.vTasks.size(); ++i)
	{
		int nIdx = m_lstTask.InsertItem(i, CTxStrConvert(i +1).GetAsString());
		if (nIdx < 0)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		STEPGTASK& taskIn =  m_epgData.vTasks[i];
		CString strChnName;
		for (int j = 0; j < taskIn.vChnTask.size(); ++j)
		{
			if (j != 0)
			{
				strChnName += TEXT(",");
			}
			strChnName += GetChnName(taskIn.vChnTask[j].strChId);
		}

		m_lstTask.SetItemText(nIdx, 1, strChnName);
		m_lstTask.SetItemText(nIdx, 2, taskIn.strDesc);
	}

	UpdateCtrl();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEPGCreaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEPGCreaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEPGCreaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEPGCreaterDlg::OnClose()
{
	if (AfxMessageBox(TEXT("ȷ���˳�����"), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	CDialog::OnClose();
}

BOOL CEPGCreaterDlg::SerializeEPGData( BOOL bReadIn /*= TRUE*/ )
{
	BOOL bRet = FALSE;
	CFile file;
	if (bReadIn)
	{
		if (file.Open(g_GlobalInfo.szEpgDatafile, CFile::modeRead, NULL))
		{
			LONG nLen = file.GetLength();
			if (nLen == 0)
			{
				ASSERT(FALSE);
			}
			else
			{
				char* pBuff = new char[nLen+1];
				LONG nRead = file.Read(pBuff, nLen);
				ASSERT(nRead == nLen);
				pBuff[nLen] = '\0';
				bRet = m_epgData.FromString(pBuff);
				delete pBuff;
			}

			file.Close();
		}
	}
	else
	{
		if (file.Open(g_GlobalInfo.szEpgDatafile, CFile::modeCreate|CFile::modeWrite, NULL))
		{
			CString strBuff;
			m_epgData.ToString(strBuff);
			file.Write(strBuff.LockBuffer(), strBuff.GetLength());
			strBuff.UnlockBuffer();
			bRet = TRUE;
			file.Flush();
			file.Close();

		}
	}
	return bRet;
}

void CEPGCreaterDlg::OnBnClickedBtnTasknew()
{
	CDlgTaskEdit dlg;
	dlg.Init();
	if (dlg.DoModal() == IDOK)
	{
		m_epgData.vTasks.push_back(dlg.GetTaskRef());
		SerializeEPGData(FALSE);
		AddTask(dlg.GetTaskRef());
	}
}

void CEPGCreaterDlg::OnBnClickedBtnTaskedit()
{
	POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_lstTask.GetNextSelectedItem(pos);
	if (nItem >= 0 && nItem < m_epgData.vTasks.size())
	{
		CDlgTaskEdit dlg;
		dlg.Init(&m_epgData.vTasks[nItem]);
		if (dlg.DoModal() == IDOK)
		{
			//save data back
			m_epgData.vTasks[nItem] = dlg.GetTaskRef();
			SerializeEPGData(FALSE);
			RefreshTask(nItem);
		}
	}
}

void CEPGCreaterDlg::OnBnClickedBtnTaskdel()
{
	POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int nItem = m_lstTask.GetNextSelectedItem(pos);
	if (nItem >= 0 && nItem < m_epgData.vTasks.size())
	{
		if (AfxMessageBox(TEXT("ȷ��Ҫɾ��ѡ�в���?"), MB_OKCANCEL|MB_ICONQUESTION)== IDOK)
		{
			vector<STEPGTASK>::iterator it = m_epgData.vTasks.begin();
			advance(it, nItem);
			m_epgData.vTasks.erase(it);
			SerializeEPGData(FALSE);
			DelTask(nItem);

		}
	}
}

BOOL CEPGCreaterDlg::AddTask( STEPGTASK& taskIn )
{
	int nCount = m_lstTask.GetItemCount();
	int nIdx = m_lstTask.InsertItem(nCount, CTxStrConvert(nCount +1).GetAsString());
	if (nIdx < 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CString strChnName;
	for (int i = 0; i < taskIn.vChnTask.size(); ++i)
	{
		if (i != 0)
		{
			strChnName += TEXT(",");
		}
		strChnName += GetChnName(taskIn.vChnTask[i].strChId);
	}

	m_lstTask.SetItemText(nIdx, 1, strChnName);
	m_lstTask.SetItemText(nIdx, 2, taskIn.strDesc);
	return TRUE;
}

BOOL CEPGCreaterDlg::DelTask( int nNumber )
{
	if (nNumber >= m_lstTask.GetItemCount())
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("del error id not in list count range"));
		return FALSE;
	}
	
	m_lstTask.DeleteItem(nNumber);

	return TRUE;
}

BOOL CEPGCreaterDlg::RefreshTask( int nNumber )
{
	
	STCHNSECTIONINFO info;
	STEPGTASK task;
	task = m_epgData.vTasks[nNumber];
	CString strChnName;
	for (int i = 0; i < task.vChnTask.size(); ++i)
	{
		if (i != 0)
		{
			strChnName += TEXT(",");
		}
		strChnName += GetChnName(task.vChnTask[i].strChId);
	}
	m_lstTask.SetItemText(nNumber, 1, strChnName);
	m_lstTask.SetItemText(nNumber, 2, task.strDesc);
	return TRUE;
}

void CEPGCreaterDlg::OnLvnItemchangedListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	UpdateCtrl();
	*pResult = 0;
}

LRESULT CEPGCreaterDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message == MSG_LOGWRITE)
	{
		if (m_edtLog.GetWindowTextLength() > MAXOUTPUTLENGTH -1000)
		{
			m_edtLog.SetWindowText("");
		}
	}
	MACRO_EDITLOGOUTPUT
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CEPGCreaterDlg::UpdateCtrl()
{
	BOOL bCheckAutoGen = m_chkEnableTimeGen.GetCheck() == BST_CHECKED;
	POSITION pos = m_lstTask.GetFirstSelectedItemPosition();

	m_btnTaskNew.EnableWindow(!bCheckAutoGen);
	m_btnTaskEdit.EnableWindow(!bCheckAutoGen && pos != NULL);
	m_btnTaskDel.EnableWindow(!bCheckAutoGen && pos != NULL);
	m_btnTaskGenNow.EnableWindow(pos != NULL);
}

class CAutoOpenDB
{
public:
	BOOL bOpen;
	CAutoOpenDB()
	{
		if(!CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEYEGP, g_GlobalInfo.szDBConnString))
		{
			CFWriteLog(TEXT("�����ݿ�ʧ��!"));
			bOpen = FALSE;
		}
		else
		{
			bOpen = TRUE;
		}

	}
	~CAutoOpenDB()
	{
		if (bOpen)
		{
			CTxADODBMgr::GetADODBMgr()->CloseDB(DBKEYEGP);
		}

	}
};


BOOL CEPGCreaterDlg::GenEPGFile( CTime& tmStartIn, CTime& tmEndIn, int nTaskId, BOOL bAutoGenIn, LPCTSTR szFileIn /*= NULL*/ )
{

	if (tmStartIn > tmEndIn)
	{
		CFWriteLog("������ֹ���ڲ��Ϸ���");
		return FALSE;
	}

	if (nTaskId < 0 || nTaskId >= m_epgData.vTasks.size())
	{
		CFWriteLog(TEXT("����Id���Ϸ�"));
		return FALSE;
	}

	CAutoOpenDB db;
	if (!db.bOpen)
	{
		return FALSE;
	}

	STEPGTASK& taskRef = m_epgData.vTasks[nTaskId];

	MAPCHNPGMS mapChnPgms;
	
	CTime tmScheduleStart(2199,1,1,1,0,0);
	CTime tmScheduleEnd(0);
	BOOL bSubmittStateErr = FALSE;
	BOOL bPSICountErr = FALSE;
	for (size_t i = 0; i < taskRef.vChnTask.size(); ++i)
	{
		ST_ONECHN& oneChnRef = taskRef.vChnTask[i];
		VCHNPGMS vChnPgm;
		CTimeSpan dtOneDay(1, 0,0,0);
		CTime tmBegin = tmStartIn;
		tmBegin -=dtOneDay;
		do 
		{
			tmBegin += dtOneDay;

			int nCurrWeekDay = tmBegin.GetDayOfWeek() -1;
			CString strCurrSecId;
			if (oneChnRef.vWeekDays[nCurrWeekDay].bEnable)
			{
				strCurrSecId = oneChnRef.vWeekDays[nCurrWeekDay].strSecID;
				ASSERT(!strCurrSecId.IsEmpty());
			}

			if (!strCurrSecId.IsEmpty())
			{
				CString strSql;
				strSql.Format(TEXT("  select strPSID, nState from t_programsheet where strSectionGuid = '%s' and strChnID = '%s' and tPlayDate = '%s'"),
					strCurrSecId, oneChnRef.strChId, tmBegin.Format(TEXT("%Y-%m-%d 00:00:00")));
				CTxADORecordSet rst;
				if (rst.ExecuteSQL(strSql))
				{
					int nPSICount = 0;
					while(!rst.IsEOF())
					{
						STCHNPGMINFO chnPgmInfo;

						chnPgmInfo.strPISId = rst.GetVal(TEXT("strPSID")).GetAsString();
						chnPgmInfo.nState = rst.GetVal(TEXT("nState")).GetAsInt();
						if (chnPgmInfo.nState  != 2 && taskRef.bMustSubmitted)
						{
							bSubmittStateErr = TRUE;
							CFWriteLog(TEXT("Ƶ��%s, %s, ��Ŀ��δ�ύ����=%d"), oneChnRef.strChId, tmBegin.Format(TEXT("%Y-%m-%d")));

						}
						chnPgmInfo.tDate = tmBegin;
						vChnPgm.push_back(chnPgmInfo);
						nPSICount +=1;

						rst.MoveNext();
					}

					if (nPSICount != 1)
					{
						bPSICountErr = TRUE;
						CFWriteLog(TEXT("Ƶ��%s, %s, ��Ŀ����������=%d"), oneChnRef.strChId, tmBegin.Format(TEXT("%Y-%m-%d")), nPSICount);
					}
				}
				else
				{
					CFWriteLog(TEXT("sql exc err! %s"), strSql);
				}

			}


		}while(!(tmBegin.GetYear() == tmEndIn.GetYear() && tmBegin.GetMonth() == tmEndIn.GetMonth()
			&& tmBegin.GetDay() == tmEndIn.GetDay()));

		mapChnPgms[oneChnRef.strChId] = vChnPgm;
	}


	//check submit state and pgm
	if (bPSICountErr || bSubmittStateErr)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bPgmGetErr = FALSE;
	MAPCHNPGMS::iterator itb = mapChnPgms.begin();
	MAPCHNPGMS::iterator ite = mapChnPgms.end();
	for (; itb != ite; ++itb)
	{
		VCHNPGMS& vChnPgms = itb->second;
		for (size_t i = 0; i < vChnPgms.size(); ++i)
		{
			STCHNPGMINFO& chnPgmRef = vChnPgms[i];

			CString strSql;
			strSql.Format(TEXT("  select nIndex, strPGMName, tPGMSchPlayDate, nPGMSchStartTime, nPGMDuration from t_PlaylistCompile where strPSID = '%s' order by nIndex"), chnPgmRef.strPISId);
			CTxADORecordSet rst;
			if (rst.ExecuteSQL(strSql))
			{
				while(!rst.IsEOF())
				{
					STBIDINFO bidInfo;

					bidInfo.strPgmName = rst.GetVal(TEXT("strPGMName")).GetAsString();
					CTime tmStart(chnPgmRef.tDate.GetYear(), chnPgmRef.tDate.GetMonth(), chnPgmRef.tDate.GetDay(), 0,0,0);
					INT64 ntBegin =  (rst.GetVal(TEXT("nPGMSchStartTime")).GetAsInt64())/40;
					if (ntBegin >= 0)
					{
						tmStart += TimeCodeToSpan(ntBegin);
					}
					else
					{
						tmStart -=TimeCodeToSpan(ntBegin);
					}
					if (tmStart < tmScheduleStart)
					{
						tmScheduleStart = tmStart;
					}
					bidInfo.tBeginPlay = tmStart;
					INT64 nDurn =rst.GetVal(TEXT("nPGMDuration")).GetAsInt64();
					tmStart += TimeCodeToSpan(nDurn);
					bidInfo.tEndPlay = tmStart;
					if (tmStart > tmScheduleEnd)
					{
						tmScheduleEnd = tmStart;
					}
					chnPgmRef.vBidList.push_back(bidInfo);
					rst.MoveNext();
				}
			}
			else
			{
				CFWriteLog(TEXT("sql exc err! %s"), strSql);
				bPgmGetErr = TRUE;
				break;
			}
		}
	}

	if (bPgmGetErr)
	{
		return FALSE;
	}



	//prepare doc
	CString strTitle;
	CString strChannels;
	CString strSchedule;
	CString strExportTime;
	CTime tmNow(time(NULL));
	strExportTime = tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S"));
	CString strStartTime = tmScheduleStart.Format(TEXT("%Y-%m-%d %H:%M:%S"));
	CString strEndTime = tmScheduleEnd.Format(TEXT("%Y-%m-%d %H:%M:%S"));
	CString strMode = TEXT("replace");


	CMarkup markDoc;
	//into ListOfModel
	markDoc.AddElem(TEXT("ListOfModel"));
	markDoc.AddAttrib(TEXT("exportTime"), strExportTime);
	markDoc.AddAttrib(TEXT("startTime"), strStartTime);
	markDoc.AddAttrib(TEXT("endTime"), strEndTime);
	markDoc.AddAttrib(TEXT("mode"), strMode);
	markDoc.IntoElem();
	//into ListOfChannel
	markDoc.AddElem(TEXT("ListOfChannel"));
	markDoc.IntoElem();
	itb = mapChnPgms.begin();
	ite = mapChnPgms.end();
	for (; itb != ite; ++itb)
	{
		//into Channel
		markDoc.AddElem(TEXT("Channel"));
		markDoc.AddAttrib(TEXT("channelID"), GetChnNum(itb->first));
		markDoc.AddAttrib(TEXT("channelName"), GetChnName(itb->first));
	}
	//out ListOfChannel
	markDoc.OutOfElem();
	markDoc.AddElem(TEXT("ListOfProfile"));
	markDoc.AddElem(TEXT("ListOfSeries"));
	markDoc.AddElem(TEXT("ListOfProgram"));
	//into ListOfSchedule
	markDoc.AddElem(TEXT("ListOfSchedule"));
	markDoc.IntoElem();

	itb = mapChnPgms.begin();
	ite = mapChnPgms.end();
	for (; itb != ite; ++itb)
	{
		VCHNPGMS& vChnPgms = itb->second;
		CString strChNum = GetChnNum(itb->first);
		CString strChName = GetChnName(itb->first);
		for (size_t i = 0; i < vChnPgms.size(); ++i)
		{
			STCHNPGMINFO& chnPgmRef = vChnPgms[i];
			for (size_t j = 0; j < chnPgmRef.vBidList.size(); ++j)
			{
				STBIDINFO& bidInfoRef = chnPgmRef.vBidList[j];
				markDoc.AddElem(TEXT("Schedule"));
				//into Schedule
				CString strcheduleid;
				strcheduleid.Format(TEXT("%03d%s0"), atoi(strChNum), bidInfoRef.tBeginPlay.Format(TEXT("%y%m%d%H%M%S")));

				markDoc.AddAttrib(TEXT("scheduleID"), strcheduleid);
				markDoc.AddAttrib(TEXT("programID"), strcheduleid);
				markDoc.AddAttrib(TEXT("channelID"), strChNum);
				markDoc.AddAttrib(TEXT("channelName"),strChName);
				markDoc.AddAttrib(TEXT("startTime"), bidInfoRef.tBeginPlay.Format(TEXT("%Y-%m-%d %H:%M:%S")));
				markDoc.AddAttrib(TEXT("endTime"),bidInfoRef.tEndPlay.Format(TEXT("%Y-%m-%d %H:%M:%S")));
				markDoc.AddAttrib(TEXT("catchedByOperator"), TEXT(""));
				markDoc.AddAttrib(TEXT("programName"), bidInfoRef.strPgmName);
				markDoc.AddAttrib(TEXT("providerID"), TEXT(""));
				markDoc.AddAttrib(TEXT("providerName"), TEXT(""));
				markDoc.AddAttrib(TEXT("providerAssetID"), TEXT(""));
				markDoc.AddAttrib(TEXT("canBeRecorded"), TEXT("1"));
			}
		}
	}

	//out ListOfSchedule
	markDoc.OutOfElem();

	//out ListOfModel
	markDoc.OutOfElem();




	//start file check
	CFile fileOut;
	CString strfilePath;
	if (szFileIn != NULL)
	{
		strfilePath = szFileIn;
	}
	else
	{
		if (bAutoGenIn)
		{
			strfilePath.Format(TEXT("%s\\EPGAutoSave"), GetAppPath().c_str());
		}
		else
		{
			strfilePath.Format(TEXT("%s\\EPGSave"), GetAppPath().c_str());
		}
		CreateDirectory(strfilePath, NULL);
		CString strtmp = strfilePath;
		strfilePath.Format(TEXT("%s\\task%d"),strtmp, nTaskId);
		CreateDirectory(strfilePath, NULL);
		strtmp = strfilePath;
		strfilePath.Format(TEXT("%s\\%s-%s"),strtmp, tmStartIn.Format(TEXT("%Y��%m��%d��")), tmEndIn.Format(TEXT("%Y��%m��%d��")));
		CreateDirectory(strfilePath, NULL);
		CTime tmNow(time(NULL));
		strtmp.Format(TEXT("\\%s.xml"), tmNow.Format(TEXT("%Y%m%d-%H%M%S")));
		strfilePath += strtmp;
	}

	if (!fileOut.Open(strfilePath, CFile::modeCreate|CFile::modeWrite, NULL))
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("�ļ�д��ʧ��.%s"), strfilePath);
		return FALSE;
	}
	else
	{
		CString strOut = TEXT("<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n");
		fileOut.Write(strOut, strOut.GetLength());
		strOut = markDoc.GetDoc();
		fileOut.Write(strOut, strOut.GetLength());
		fileOut.Flush();
		fileOut.Close();
		return TRUE;
	}


	

}

void CEPGCreaterDlg::OnBnClickedBtnGenmanual()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_lstTask.GetFirstSelectedItemPosition();

	int nSel = m_lstTask.GetNextSelectedItem(pos);
	if (nSel < 0)
	{
		return;
	}

	CDlgGetDateRange dlg;
	if (dlg.DoModal() == IDOK)
	{
		//gen task
		CString strTarget;
		strTarget.Format(TEXT("%s-%s"),dlg.m_tmStart.Format(TEXT("%Y-%m-%d")), dlg.m_tmEnd.Format(TEXT("%Y-%m-%d")));

		CFWriteLog(TEXT("�ֶ��������ɿ�ʼ:%s"), strTarget);
		if (GenEPGFile(dlg.m_tmStart, dlg.m_tmEnd, nSel))
		{
			CString strMsg;
			strMsg.Format(TEXT("����%d���ɳɹ�"), nSel);
			AfxMessageBox(strMsg);
		}
		else
		{
			CString strMsg;
			strMsg.Format(TEXT("����%d����ʧ�ܣ���鿴��־���"), nSel);
			AfxMessageBox(strMsg);
		}
		CFWriteLog(TEXT("�ֶ��������ɽ���:%s"), strTarget);

	}
}

void CEPGCreaterDlg::OnBnClickedCheckGenontime()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bEnableAutoGen = m_chkEnableTimeGen.GetCheck() == BST_CHECKED;
	UpdateCtrl();
	if (bEnableAutoGen)
	{
		AutoCheckTask();
		SetTimer(IDTIMER_AUTOGENTASK, g_GlobalInfo.nAutoCheckInterval*1000, 0);
	}
	else
	{
		KillTimer(IDTIMER_AUTOGENTASK);
		//clean list col 3,4
		int nCount = m_lstTask.GetItemCount();
		for (int i = 0; i < nCount; ++i)
		{
			m_lstTask.SetItemText(i, 3, TEXT(""));
			m_lstTask.SetItemText(i, 4, TEXT(""));
		}
	}
}

void CEPGCreaterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDTIMER_AUTOGENTASK)
	{
		AutoCheckTask();
	}
	CDialog::OnTimer(nIDEvent);
}

void CEPGCreaterDlg::AutoCheckTask()
{
	for (size_t i = 0; i< m_epgData.vTasks.size(); ++i)
	{
		STEPGTASK& taskRef =m_epgData.vTasks[i];
		if (!taskRef.bEnableTimer)
		{
			m_lstTask.SetItemText(i, 3, TEXT("�ֶ�"));
			m_lstTask.SetItemText(i, 4, TEXT("---"));

			continue;
		}
		CTime tmNow(time(NULL));
		int nWeekDay = tmNow.GetDayOfWeek();
		CTime tmTimeUse(taskRef.tTimeToGen);
		CTime tmNextTarget(tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(),0, 0, 0);

		tmNextTarget += CTimeSpan(taskRef.nOffsetDay, 0,0,0);

		int nRemainDay = 0;
		int nCurrDay = tmNextTarget.GetDayOfWeek()-1; //0-6 sunday--saturday
		const int nsStartWeekDay = taskRef.nWeekLoopMode == 0? 1:0;
		while ((nCurrDay) %7 != nsStartWeekDay)
		{
			nCurrDay +=1;
			nCurrDay %=7;
			nRemainDay++;
		}

		

		//next next
		CTime tmTmp(tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), tmTimeUse.GetHour(), tmTimeUse.GetMinute(), tmTimeUse.GetSecond());
		if (tmNow > tmTmp && nRemainDay == 0)
		{
			nRemainDay+=7;
		}

		tmNextTarget += CTimeSpan(nRemainDay, 0,0,0);
		
		CTime tmNextGen = tmNextTarget - CTimeSpan(taskRef.nOffsetDay, 0, 0, 0) +CTimeSpan(0, tmTimeUse.GetHour(), tmTimeUse.GetMinute(), tmTimeUse.GetSecond());
		//write next gen to list ctrl
		CTime tmNextTargetEnd = tmNextTarget + CTimeSpan(6,0,0,0);
		CString strTarget;
		strTarget.Format(TEXT("%s-%s"),tmNextTarget.Format(TEXT("%Y-%m-%d")), tmNextTargetEnd.Format(TEXT("%Y-%m-%d")));
		m_lstTask.SetItemText(i, 3, strTarget);
		m_lstTask.SetItemText(i, 4, tmNextGen.Format(TEXT("%Y-%m-%d %H:%M:%S")));
		CTimeSpan tspDetaRemain = tmNextGen - tmNow;
		LONGLONG nTotalSecRemain = tspDetaRemain.GetTotalSeconds();
		if (nTotalSecRemain > 0 && nTotalSecRemain < (g_GlobalInfo.nAutoCheckInterval -10))
		{
			//time to gen now
			CFWriteLog(TEXT("�Զ��������ɿ�ʼ:%s"), strTarget);
			if (!GenEPGFile(tmNextTarget, tmNextTargetEnd, i, TRUE, NULL))
			{
				CFWriteLog(TEXT("�Զ���������ʧ�ܣ�������־��"));
			}
			CFWriteLog(TEXT("�Զ��������ɽ���:%s"), strTarget);

		}


		
	}
}

void CEPGCreaterDlg::OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
	LPNMLVCUSTOMDRAW plvCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult =CDRF_NOTIFYITEMDRAW;
		return;
	}

	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{

		int nId = pNMCD->dwItemSpec;
		if( m_lstTask.GetItemState(nId, LVIS_SELECTED) & LVIS_SELECTED)
		{
			plvCD->clrTextBk = RGB(53,152,255);
			plvCD->clrText = RGB(255,255,255);
			*pResult = 0;
			return;
		}
	}
}
