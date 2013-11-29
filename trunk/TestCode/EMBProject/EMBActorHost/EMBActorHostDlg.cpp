
// EMBActorHostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EMBActorHost.h"
#include "EMBActorHostDlg.h"
#include "..\..\Plugins\EMBActor\TaskActor.h"

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


// CEMBActorHostDlg �Ի���




CEMBActorHostDlg::CEMBActorHostDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMBActorHostDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMBActorHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EXECUTOR, m_lstExecutor);
}

BEGIN_MESSAGE_MAP(CEMBActorHostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CEMBActorHostDlg::OnBnClickedButtonRefresh)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LUANCHEXEC, &CEMBActorHostDlg::OnBnClickedLuanchexec)
END_MESSAGE_MAP()


// CEMBActorHostDlg ��Ϣ�������

BOOL CEMBActorHostDlg::OnInitDialog()
{
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEMBActorHostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMBActorHostDlg::OnPaint()
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
HCURSOR CEMBActorHostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEMBActorHostDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
}

void CEMBActorHostDlg::OnBnClickedButtonRefresh()
{
	CArray<tagExecutorProcess, tagExecutorProcess> arrExecutor;
	GetExecutors(arrExecutor);

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

	UpdateData(FALSE);
}

// ��ʼ��������Ϣ
void CEMBActorHostDlg::InitUI()
{
	m_lstExecutor.InsertColumn(0, "����", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(1, "��ʶ", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(2, "״̬", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(3, "ִ�н���", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(4, "�����ʶ", LVCFMT_LEFT, 100);
}

void CEMBActorHostDlg::OnClose()
{
	// ��ʾ
	if (IDNO == MessageBox("ȷ���˳�����", "��ʾ", MB_YESNO))
	{
		return;
	}

	CDialog::OnClose();
}

void CEMBActorHostDlg::GetExecutors( CArray<tagExecutorProcess, tagExecutorProcess>& m_arrExecutors )
{
	// ��EMBActor.dll ��ѯ��Ϣ, δʵ�� to do
	tagExecutorProcess tag;
	tag.m_strID = "test001";
	tag.m_strName = "Executor1";
	tag.m_strState = "����";
	tag.m_strTaskGuid = "";
	tag.m_nPercent = 100;

	m_arrExecutors.Add(tag);
}

void CEMBActorHostDlg::OnBnClickedLuanchexec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strfile =GetAppPath().c_str();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this, 0 );
	if (dlg.DoModal() == IDOK)
	{
		strfile = dlg.GetOFN().lpstrFile;
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
		file.Close();
	}

	if (g_pIActorPlugin)
	{
		EMB::CTaskActor* pActor = dynamic_cast<EMB::CTaskActor*>(g_pIActorPlugin);
		if (pActor)
		{
			CString strFile;
			DWORD dwProcessId = 0;
			CString strRet;
			pActor->OnActorConnectorMsg(strTask, strRet);
			CString strmsg;
			strmsg.Format(TEXT("excutorlaunched id = %d, processid = %d"));
		}
	}
}
