// DlgDbgTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EMBActorHost.h"
#include "DlgDbgTest.h"
#include "..\..\Plugins\EMBActor\TaskActor.h"
#include "TxAutoComPtr.h"
#include "IEMBBaseInterface.h"

// CDlgDbgTest �Ի���
using namespace EMB;

IMPLEMENT_DYNAMIC(CDlgDbgTest, CDialog)

CDlgDbgTest::CDlgDbgTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDbgTest::IDD, pParent)
{

}

CDlgDbgTest::~CDlgDbgTest()
{
}

void CDlgDbgTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXECUTOR, m_lstExecutor);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);

}


BEGIN_MESSAGE_MAP(CDlgDbgTest, CDialog)
	ON_BN_CLICKED(IDC_LUANCHEXEC, &CDlgDbgTest::OnBnClickedLuanchexec)
	ON_BN_CLICKED(IDC_BUTTON_AUTOTEST, &CDlgDbgTest::OnBnClickedButtonAutotest)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CDlgDbgTest::OnBnClickedButtonRefresh)

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgDbgTest ��Ϣ�������

BOOL CDlgDbgTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CDlgDbgTest::OnBnClickedButtonRefresh()
{
	CArray<tagExecutorProcess, tagExecutorProcess> arrExecutor;
	CArray<ST_TASKRUNSTATE, ST_TASKRUNSTATE> arrTask;
	GetExecutors(arrExecutor, arrTask);

	// ִ�н�����Ϣ
	m_lstExecutor.DeleteAllItems();

	for (int i = 0; i < arrExecutor.GetSize(); ++i)
	{
		tagExecutorProcess tag = arrExecutor[i];

		m_lstExecutor.InsertItem(i, tag.m_strID);
		m_lstExecutor.SetItemText(i, 0, tag.m_strName);
		m_lstExecutor.SetItemText(i, 1, tag.m_strID);
		m_lstExecutor.SetItemText(i, 2, tag.m_strState);
		// ����
		CString strTem;
		strTem.Format("%d", tag.m_nPercent);
		m_lstExecutor.SetItemText(i, 3, strTem);
		m_lstExecutor.SetItemText(i, 4, tag.m_strTaskGuid);
	}

	// ������Ϣ
	m_lstTask.DeleteAllItems();
	CString strTem;

	for (int i = 0; i < arrTask.GetSize(); ++i)
	{
		ST_TASKRUNSTATE tsk = arrTask[i];
		strTem.Format("%d", i);
		m_lstTask.InsertItem(i, strTem);
		m_lstTask.SetItemText(i, 0, Guid2String(tsk.guid));

		strTem.Format("%d", tsk.nCurrStep);
		m_lstTask.SetItemText(i, 1, strTem);

		// ״̬
		m_lstTask.SetItemText(i, 2, tsk.StateDes());

		tm tCommit;
		localtime_s(&tCommit, &(tsk.tmCommit));
		CString strTime;
		strTime.Format("%.4d-%.2d-%.2d %.2d:%.2d", tCommit.tm_year + 1900, tCommit.tm_mon + 1, tCommit.tm_mday, tCommit.tm_hour, tCommit.tm_min);
		m_lstTask.SetItemText(i, 3, strTime);
	}

	UpdateData(FALSE);
}

// ��ʼ��������Ϣ
void CDlgDbgTest::InitUI()
{
	// Executor list
	m_lstExecutor.InsertColumn(0, "����", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(1, "���̱�ʶ", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(2, "״̬", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(3, "ִ�н���", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(4, "�����ʶ", LVCFMT_LEFT, 100);

	// Task list
	m_lstTask.InsertColumn(0, "�����ʶ", LVCFMT_LEFT, 260);
	m_lstTask.InsertColumn(1, "����", LVCFMT_LEFT, 50);
	m_lstTask.InsertColumn(2, "״̬", LVCFMT_LEFT, 50);
	m_lstTask.InsertColumn(3, "�ύʱ��", LVCFMT_LEFT, 120);

	m_nIdx = 0;
}



void CDlgDbgTest::GetExecutors(CArray<tagExecutorProcess, tagExecutorProcess>& arrExecutors, CArray<ST_TASKRUNSTATE, ST_TASKRUNSTATE>& arrTask)
{
	// ��EMBActor.dll ��ѯ��Ϣ, ���޸� to do
	if (g_pIActorPlugin != NULL)
	{
		CTxAutoComPtr<IActorUI> ptrActorUI;

		if (S_OK == g_pIActorPlugin->QueryInterface(GuidEMBPlugin_IActorUI, (LPVOID&)(*&ptrActorUI)))
		{
			vector<CString> vExecutor;
			ptrActorUI->GetExecutors(vExecutor);

			for(int i = 0; i < vExecutor.size(); ++i)
			{
				ST_EXCUTORINFO tem;
				tem.FromString(vExecutor[i]);

				tagExecutorProcess tag;
				tag.m_strID.Format("%d", tem.hProcessId);
				tag.m_strName.Format("%d", tem.excutorId);
				tag.m_strState = tem.m_strRunStep;
				tag.m_strTaskGuid = tem.m_strTaskGuid;
				tag.m_nPercent = tem.m_nPercent;

				arrExecutors.Add(tag);
			}

			// ������Ϣ
			vector<CString> vTask;
			ptrActorUI->GetTaskInActor(vTask);

			for (int i = 0; i < vTask.size(); ++i)
			{
				ST_TASKRUNSTATE tsk;
				tsk.FromString(vTask[i]);

				arrTask.Add(tsk);
			}
		}
	}
}

void CDlgDbgTest::OnBnClickedLuanchexec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strfile =GetAppPath().c_str();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this, 0 );
	if (dlg.DoModal() == IDOK)
	{
		strfile = dlg.GetOFN().lpstrFile;
	}
	else
	{
		return;
	}

	CFile file;
	BOOL bOPen =file.Open(strfile, CFile::modeRead, NULL);
	CString strTask;
	if (bOPen)
	{
		int nLen = file.GetLength()+1;
		char* pBuff = new char[nLen];
		ZeroMemory(pBuff, nLen);
		file.Read(pBuff, file.GetLength());
		strTask = pBuff;
		delete pBuff;
		file.Close();
	}

	if (g_pIActorPlugin && !strTask.IsEmpty())
	{
		EMB::CTaskActor* pActor = dynamic_cast<EMB::CTaskActor*>(g_pIActorPlugin);
		if (pActor)
		{
			CString strFile;
			DWORD dwProcessId = 0;
			CString strRet;
			pActor->OnActorConnectorMsg(strTask, strRet);

		}
	}
}

void CDlgDbgTest::OnBnClickedButtonAutotest()
{
	// �Զ��ύ�������
	CString strfile =GetAppPath().c_str();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this, 0 );
	if (dlg.DoModal() == IDOK)
	{
		strfile = dlg.GetOFN().lpstrFile;
	}
	else
	{
		return;
	}

	CFile file;
	BOOL bOPen =file.Open(strfile, CFile::modeRead, NULL);
	CString strTask;
	if (bOPen)
	{
		int nLen = file.GetLength()+1;
		char* pBuff = new char[nLen];
		ZeroMemory(pBuff, nLen);
		file.Read(pBuff, file.GetLength());
		strTask = pBuff;
		delete pBuff;
		file.Close();

		// ������Ϣ
		int nIdx = strfile.ReverseFind('\\');
		if (nIdx > 0)
		{
			m_strVideoPath = strfile.Left(nIdx);
			m_strXmlTemplate = strTask;

			// ����m_strVideoPathĿ¼�µ��ļ�,����m_arrVideo
			WIN32_FIND_DATA FF;
			HANDLE hF = FindFirstFile(m_strVideoPath  + "\\*.*", &FF);
			while (hF != INVALID_HANDLE_VALUE)
			{
				//
				bool isDir = FF.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
				if (!isDir)
				{
					m_arrVideo.Add(FF.cFileName);
				}

				if (!FindNextFile(hF, &FF))
				{
					break;
				}
			}

			// test
			m_nIdx = 0;
			SetTimer(1000, 60000, NULL); // ���Ϊ1���� 
		}

	}
}

void CDlgDbgTest::OnTimer( UINT_PTR nIDEvent )
{
	if (nIDEvent == 1000 && m_arrVideo.GetSize() > 0)
	{
		if (!m_strXmlTemplate.IsEmpty())
		{
			if (m_nIdx >= m_arrVideo.GetSize())
			{
				m_nIdx = 0;
			}

			CString strXml = m_strXmlTemplate; // ģ��xml
			CString strTaskGuid;
			GUID   guid; 

			if (S_OK == ::CoCreateGuid(&guid)) 
			{
				strTaskGuid = Guid2String(guid);

				CString strSrcFile = m_arrVideo[m_nIdx]; // �ļ���

				strXml.Replace("{TaskGuid}", strTaskGuid);
				strXml.Replace("{SrcFile}", strSrcFile);

				// �ύ����
				if (g_pIActorPlugin && !strXml.IsEmpty())
				{
					EMB::CTaskActor* pActor = dynamic_cast<EMB::CTaskActor*>(g_pIActorPlugin);
					if (pActor)
					{
						CString strFile;
						DWORD dwProcessId = 0;
						CString strRet;
						HRESULT hr = pActor->OnActorConnectorMsg(strXml, strRet);
						if (hr == S_OK)
						{
							m_nIdx++;
						}

						OutputDebugString("------�ύ����-----------------------------");
						OutputDebugString(strXml);
					}
				}
			}
		}

	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CDlgDbgTest::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	

	return CDialog::PreTranslateMessage(pMsg);
}
