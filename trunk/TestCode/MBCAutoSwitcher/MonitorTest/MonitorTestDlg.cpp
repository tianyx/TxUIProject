
// MonitorTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MonitorTest.h"
#include "MonitorTestDlg.h"
#include "MBCTransMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMonitorTestDlg 对话框




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


// CMonitorTestDlg 消息处理程序

BOOL CMonitorTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	int nLen = sizeof(SOCKADDR_IN);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMonitorTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	m_pMgr->Stop();
}

HRESULT CMonitorTestDlg::TransActData(int nmsgType, char* bufferIn, int nLenIn, char* bufferToFill, int nMaxLen, int& nFillLen )
{
	return E_NOTIMPL;
}
