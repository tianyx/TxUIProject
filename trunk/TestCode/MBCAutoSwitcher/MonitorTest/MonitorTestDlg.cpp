
// MonitorTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MonitorTest.h"
#include "MonitorTestDlg.h"
#include "MBCTransMsg.h"

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


// CMonitorTestDlg �Ի���




CMonitorTestDlg::CMonitorTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMgr = NULL;
}

void CMonitorTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CMonitorTestDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CMonitorTestDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CMonitorTestDlg ��Ϣ�������

BOOL CMonitorTestDlg::OnInitDialog()
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
	int nLen = sizeof(SOCKADDR_IN);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMonitorTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMonitorTestDlg::OnPaint()
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
HCURSOR CMonitorTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorTestDlg::OnBnClickedBtnStart()
{

/*
	ST_MBCMSG_LIVEQA msg;
	ST_MBCMSG_LIVEQA msgout;
	ST_MBCCHANNELINFO info;
	info.szChannelId = TEXT("123444");
	info.addrDest.sin_family = AF_INET;
	info.addrDest.sin_port = 1882;
	info.addrDest.sin_addr.S_un.S_addr = 1234455;
	info.addrDestSecond.sin_family = AF_INET;
	info.addrDestSecond.sin_port = 2222;
	info.addrDestSecond.sin_addr.S_un.S_addr = 34444;
	for (size_t i = 0; i < 30; i++)
	{
		//msg.vCHInfo.push_back(info);
	}
	msg.nMsgState = 101;
	msg.nSelfType = 3344;
	msg.nMsgId = 101;
	char szbuff[MAXRECVBUFF];
	int nUsed = 0;
	PackMBCMsg(msg, szbuff, MAXRECVBUFF, nUsed);
	UnPackMBCMsg(szbuff, nUsed, msgout);
	return;*/
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pMgr = GetMBCRemoteMgr();
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port =htons(31002);
	addr.sin_addr.S_un.S_addr = inet_addr(TEXT("192.168.1.191"));
	m_pMgr->Init(addr, this);
	m_pMgr->Run();

}

HRESULT CMonitorTestDlg::GetLiveInfo( ST_MBCMSG_LIVEQA& msg )
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port =htons(10001);
	addr.sin_addr.S_un.S_addr = inet_addr(TEXT("236.1.1.1"));

	msg.nSelfType = selfType_CLIENT;
	msg.nRemoteState = MBCSTATE_OK;
	return S_OK;
}

HRESULT CMonitorTestDlg::GetLiveInfo( ST_MBCMSGBASE& basemsg, int& nRemoteState, int& nSelftype, ST_MBCCHANNELINFO_FORVC6* pArrayOut, const int nArrLen, int& chUsed )
{
	basemsg.nMsgState = msgState_A;
	nSelftype = selfType_CLIENT;
	nRemoteState = MBCSTATE_OK;
	return S_OK;
}

HRESULT CMonitorTestDlg::DoAction( ST_MBCMSG_ACTQA& msg )
{
	msg.nResult = S_OK;
	return S_OK;
}

void CMonitorTestDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pMgr->Stop();
}

HRESULT CMonitorTestDlg::TransActData(int nmsgType, char* bufferIn, int nLenIn, char* bufferToFill, int nMaxLen, int& nFillLen )
{
	return E_NOTIMPL;
}
