// DlgDbgTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EMBServer.h"
#include "DlgDbgTest.h"

#include "EmbStructDef.h"
#include "MainDef.h"
#include "TxParamString.h"
#include "Resource.h"
#include "ADOCtrl.h"
#include "IEMBBaseInterface.h"
#include "TxAutoComPtr.h"
using namespace EMB;
using namespace Gdiplus;
// CDlgDbgTest �Ի���

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
	DDX_Control(pDX, IDC_LIST_Actor, m_actorList);

}


BEGIN_MESSAGE_MAP(CDlgDbgTest, CDialog)
	ON_BN_CLICKED(IDC_BTNTEST, &CDlgDbgTest::OnBnClickedBtntest)
	ON_BN_CLICKED(IDC_BUTTON_XML, &CDlgDbgTest::OnBnClickedButtonXml)
	ON_WM_TIMER()

END_MESSAGE_MAP()


// CDlgDbgTest ��Ϣ�������
void CDlgDbgTest::OnTimer( UINT_PTR nIDEvent )
{
	if (1001 == nIDEvent)
	{
		RefreshActorList();
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgDbgTest::InitUI()
{
	CString strfile =GetAppPath().c_str();
	strfile += TEXT("\\testTask.xml");

	SetDlgItemText(IDC_EDIT_TESTXML, strfile);
	UpdateData(FALSE);

	// ��ʼ��actor�б�
	m_actorList.InsertColumn(0, "Actor��ʶ", LVCFMT_LEFT, 100);
	m_actorList.InsertColumn(1, "״̬", LVCFMT_LEFT, 100);
	m_actorList.InsertColumn(2, "�������", LVCFMT_LEFT, 100);

	// ����timer
	this->SetTimer(1001, 3000, NULL); // ���3��
}

void CDlgDbgTest::OnBnClickedButtonXml()
{
	// ѡ�� XML File
	CFileDialog xmlFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Xml File (*.xml)|*.xml"), (CWnd *)this, 0);

	if (IDOK == xmlFileDlg.DoModal() )
	{
		CString xmlFile = xmlFileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_TESTXML, xmlFile);
		UpdateData(FALSE);
	}
}

// ˢ��Actor�б�
void CDlgDbgTest::RefreshActorList()
{
	m_actorList.DeleteAllItems();

	if (NULL == g_pTaskDispatchMgr)
	{
		return;
	}

	CTxAutoComPtr<IServerUI> ptrServerUI;

	if (S_OK == g_pTaskDispatchMgr->QueryInterface(GuidEMBServer_IUI, (LPVOID&)(*&ptrServerUI)))
	{
		vector<CString> vInfor;
		ptrServerUI->GetActors(vInfor);

		CString strTem;

		for (int i = 0; i < vInfor.size(); ++i)
		{
			ST_ACTORSTATE st;
			st.FromString(vInfor[i]);

			strTem.Format("%d", i);
			m_actorList.InsertItem(i, strTem);

			// actorid
			strTem.Format("%d", st.actorId);
			m_actorList.SetItemText(i, 0, strTem);
			// ����״̬
			m_actorList.SetItemText(i, 1, "����");
			// �������
			m_actorList.SetItemText(i, 2, st.strPcName);
		}
	}

}
BOOL CDlgDbgTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


/*
*Description���ֶ��������һ������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
void CDlgDbgTest::OnBnClickedBtntest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strfile;
	UpdateData();
	GetDlgItemText(IDC_EDIT_TESTXML, strfile);

	if (strfile.IsEmpty())
	{
		MessageBox("��ѡ�����xml�ļ�");
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
		file.Close();
		strTask = pBuff;
		delete[] pBuff;
		EMB::IPluginTaskCommit* pIcall = dynamic_cast<EMB::IPluginTaskCommit* >(g_GlobalInfo.vPlugins[2].pIface);
		if (pIcall)
		{
			CString strRet;
			pIcall->SubmitTask(strTask, strRet);
		}
	}


}
