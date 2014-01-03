
// EMBServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EMBServer.h"
#include "EMBServerDlg.h"
#include "EmbStructDef.h"
#include "MainDef.h"
#include "TxParamString.h"
#include "Resource.h"
#include "ADOCtrl.h"
#include "IEMBBaseInterface.h"
#include "TxAutoComPtr.h"
#include "GDIDrawFunc.h"
#include "TxImageLoader.h"
using namespace EMB;
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CEMBServerDlg �Ի���




CEMBServerDlg::CEMBServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMBServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRunning = FALSE;
}

void CEMBServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Actor, m_actorList);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(CEMBServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CEMBServerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTNTEST, &CEMBServerDlg::OnBnClickedBtntest)
	ON_BN_CLICKED(IDC_BUTTON_XML, &CEMBServerDlg::OnBnClickedButtonXml)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CEMBServerDlg ��Ϣ�������

BOOL CEMBServerDlg::OnInitDialog()
{
	m_btnStart.LoadBitmap(TEXT("btnstart.png"), TXBMP_STRETCH_NONE);
	m_btnStart.SetForceRedrawParentBk(TRUE, this);
	m_pbmpBack = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("back.bmp"));
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitUI();

	AfxOleInit();       //Ϊ����ADO���ݿ�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEMBServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMBServerDlg::OnPaint()
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
HCURSOR CEMBServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
*Description���ֶ���������ť
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
void CEMBServerDlg::OnBnClickedBtnStart()
{
	/*CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase("Provider=SQLOLEDB; Server=ZHOU-LIANG; Database=Dbas_henyang; uid=sa; pwd=123;");
	if(!dbCtrl.OpenDB())
	{
		return ;
	}*/
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bRunning)
	{
		RunServer(FALSE);
		m_bRunning = !m_bRunning;

	}
	else
	{
		m_bRunning = RunServer(TRUE);
	}
	GetDlgItem(IDC_BTN_START)->SetWindowText(m_bRunning? TEXT("Stop"):TEXT("Start"));
}

/*
*Description���ֶ��������һ������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
void CEMBServerDlg::OnBnClickedBtntest()
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

void CEMBServerDlg::InitUI()
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

void CEMBServerDlg::OnBnClickedButtonXml()
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
void CEMBServerDlg::RefreshActorList()
{
	m_actorList.DeleteAllItems();

	if (!m_bRunning) // δ����ʱ
	{
		return;
	}

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

void CEMBServerDlg::OnTimer( UINT_PTR nIDEvent )
{
	if (1001 == nIDEvent)
	{
		RefreshActorList();
	}

	CDialog::OnTimer(nIDEvent);
}

void CEMBServerDlg::OnClose()
{
	// ��ʾ
	if (IDNO == MessageBox("ȷ���˳�����?", "��ʾ", MB_YESNO))
	{
		return;
	}

	CDialog::OnClose();
}

BOOL CEMBServerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pbmpBack)
	{
		TRACE(TEXT("\n dlg back redraw"));
		CRect rcThis;
		GetClientRect(rcThis);
		CRect rcEdge(5,5,5,5);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpBack, rcThis, NULL, TXBMP_STRETCH_MID_LRTB, NULL, NULL);
	}
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CEMBServerDlg::GetParentBack( CDC* pDc )
{
	OnEraseBkgnd(pDc);
}
