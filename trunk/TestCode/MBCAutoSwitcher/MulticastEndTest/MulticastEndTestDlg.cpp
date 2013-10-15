
// MulticastEndTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MulticastEndTest.h"
#include "MulticastEndTestDlg.h"
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFile* g_pfile = NULL;
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


// CMulticastEndTestDlg 对话框




CMulticastEndTestDlg::CMulticastEndTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMulticastEndTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psock = NULL;
}

void CMulticastEndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtIp);
	DDX_Control(pDX, IDC_EDIT2, m_edtLocaPort);
}

BEGIN_MESSAGE_MAP(CMulticastEndTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNSTART, &CMulticastEndTestDlg::OnBnClickedBtnstart)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECVTEST, &CMulticastEndTestDlg::OnBnClickedBtnRecvtest)
END_MESSAGE_MAP()


// CMulticastEndTestDlg 消息处理程序

BOOL CMulticastEndTestDlg::OnInitDialog()
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
	m_edtIp.SetWindowText(TEXT("236.1.1.1:10001"));
	m_edtLocaPort.SetWindowText(TEXT("9940"));

	g_pfile  = new CFile;
	BOOL bret =  g_pfile->Open("g:\\out.ts", CFile::modeCreate|CFile::modeWrite, NULL);
	ASSERT(bret);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMulticastEndTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMulticastEndTestDlg::OnPaint()
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
HCURSOR CMulticastEndTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMulticastEndTestDlg::OnBnClickedBtnstart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strIp;
	m_edtIp.GetWindowText(strIp);
	CString strLocalPort;
	m_edtLocaPort.GetWindowText(strLocalPort);
	SOCKADDR_IN addr = GetAddrFromStr(strIp);
	MCBSOCKADDRS addrs;
	addrs.addrLocal.sin_family = AF_INET;
	addrs.addrLocal.sin_port = htons(0/*atoi(strLocalPort)*/);
	addrs.addrLocal.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrs.addrRemote = addr;
	m_psock = CMBCSocket::CreateUDPSocket(addrs, FD_CLOSE|FD_CONNECT, NULL, MBCSOCKTYPE_UDP_MBC_SENDER|MBCSOCKTYPE_AUTORECONNECT);
	if (m_psock == NULL)
	{
		ASSERT(FALSE);
		return ;
	}
	SetTimer(1000, 1000, NULL);

}

void CMulticastEndTestDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_psock)
	{
		delete m_psock;
	}
	if (g_pfile)
	{
		g_pfile->Flush();
		g_pfile->Close();
		delete g_pfile;
		g_pfile = NULL;
	}
	CDialog::OnClose();
}

void CMulticastEndTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1000)
	{
		if (m_psock)
		{
			send(*m_psock, "1111111111", 10, 0);
		}
	}
	else if(nIDEvent == 1001)
	{
#define BUFSIZE 1024 
		TCHAR recvbuf[BUFSIZE];
		int ret = 0;
		struct sockaddr_in from;
		int len = 0;
		
		if(( ret = recvfrom(m_hMBCSock,recvbuf,BUFSIZE,0,(struct sockaddr*)&from,&len)) == SOCKET_ERROR)
		{
			CFWriteLog("recvfrom failed with:%d\n",WSAGetLastError());
		}
		else
		{
			
			recvbuf[ret] = '\0';
			CFWriteLog("RECV:' %s ' FROM <%s:%d> \n",recvbuf,inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		}
	

	}

	CDialog::OnTimer(nIDEvent);
}

 DWORD __stdcall RECVFROMLoopProc(void* lparam)
 {
	CMulticastEndTestDlg* pDlg = (CMulticastEndTestDlg*)lparam;
#define BUFSIZE 1024 

	TCHAR recvbuf[BUFSIZE];
	int ret = 0;
	struct sockaddr_in from;
	int len = sizeof(from);
	SOCKET m_hMBCSock = pDlg->m_hMBCSock;
	while(1)
	{
		if(( ret = recvfrom(m_hMBCSock,recvbuf,BUFSIZE,0,(struct sockaddr*)&from,&len)) == SOCKET_ERROR)
		{
			CFWriteLog("recvfrom failed with:%d\n",WSAGetLastError());
		}
		else
		{
			recvbuf[ret] = '\0';
			CFWriteLog("RECV:' %s ' FROM <%s:%d> \n",recvbuf,inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		}

	}
	


	return 0;
 }
void CMulticastEndTestDlg::OnBnClickedBtnRecvtest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strIp;
	m_edtIp.GetWindowText(strIp);
	CString strLocalPort;
	m_edtLocaPort.GetWindowText(strLocalPort);
	SOCKADDR_IN addr = GetAddrFromStr(strIp);
	MCBSOCKADDRS addrs;
	addrs.addrLocal.sin_family = AF_INET;
	addrs.addrLocal.sin_port = htons(0);
	addrs.addrLocal.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrs.addrRemote = addr;

	m_psock = CMBCSocket::CreateUDPSocket(addrs, FD_READ|FD_CLOSE|FD_CONNECT, this, MBCSOCKTYPE_UDP_MBC_BOTH|MBCSOCKTYPE_AUTORECONNECT);
	if (m_psock == NULL)
	{
		ASSERT(FALSE);
	}
// 	int m_nCreateFlag = MBCSOCKTYPE_UDP_MBC_BOTH|MBCSOCKTYPE_AUTORECONNECT;
// 
// 	MCBSOCKADDRS& m_addrs = addrs;
// 	SOCKET m_hSock=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,
// 		WSA_FLAG_MULTIPOINT_C_LEAF|WSA_FLAG_MULTIPOINT_D_LEAF);
// 	DWORD dwOptVal = TRUE;
// 	int nRet = setsockopt(m_hSock, SOL_SOCKET, SO_REUSEADDR, (char*)&dwOptVal, sizeof(dwOptVal));
// 
// 
// 	HRESULT hr = bind(m_hSock, (sockaddr*)&m_addrs.addrLocal, sizeof(sockaddr));
// 
// 	m_hMBCSock = WSAJoinLeaf(m_hSock, (SOCKADDR*)&(m_addrs.addrRemote), sizeof(m_addrs.addrRemote), NULL,NULL, NULL,NULL, (m_nCreateFlag&MBCSOCKTYPE_UDP_MBCTYPE));
// 
// 	if (m_hMBCSock == INVALID_SOCKET)
// 	{
// 		ASSERT(FALSE);
// 	}
// 	//disable loopback
// 	DWORD bFlag = TRUE;
// 	DWORD dwRet = 0;
// /*
// 	hr = WSAIoctl (m_hMBCSock,                        / * socket * /
// 		SIO_MULTIPOINT_LOOPBACK,         / * LoopBack on or off * /
// 		&bFlag,                            / * input * /
// 		sizeof (bFlag),                    / * size * /
// 		NULL,                                      / * output * / 
// 		0,                                         / * size * /
// 		&dwRet,                                   / * bytes returned * /
// 		NULL,                                      / * overlapped * /
// 		NULL); 
// 	//set TTL 
// 	DWORD nIP_TTL = 5;      // 0表示仅本机，不在网上传播，1表示在本子网中传播。>1表示跨多少个路由传播，如果要跨路由器，则路由器必须支持IGMP协议, 
// 	DWORD cbRet = 0;
// 
// 	hr = WSAIoctl(m_hMBCSock, SIO_MULTICAST_SCOPE, &nIP_TTL, sizeof(nIP_TTL), 
// 		NULL, 0,&cbRet, NULL, NULL);*/
// 
// // 	hr = WSAAsyncSelect(m_hSock, m_hSockWnd, MSG_MBCNETSOCK, m_nFavMsgType);
// // 	MUSTBESOK(hr);
// 
// 
// 	//SetTimer(1001, 10, NULL);
// 	HANDLE hWndhread = CreateThread(NULL, NULL, RECVFROMLoopProc, (LPVOID)this, 0, 0);
// 


}

HRESULT CMulticastEndTestDlg::NetCall_Read( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	if (nErr != S_OK)
	{
		ASSERT(FALSE);
		return nErr;
	}
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
#define MAXSTCBFF 1000000
	static TCHAR recvbuf[MAXSTCBFF];
	int ret = 0;
	struct sockaddr_in from;
	int len = sizeof(from);

		if(( ret = recvfrom(socRecv,recvbuf,MAXSTCBFF,0,(struct sockaddr*)&from,&len)) == SOCKET_ERROR)
		{
			CFWriteLog("recvfrom failed with:%d\n",WSAGetLastError());
		}
		else
		{
			if (g_pfile)
			{
				g_pfile->Write(recvbuf, ret);
			}
			recvbuf[ret] = '\0';
			//CFWriteLog("RECV:' %s ' FROM <%s:%d> \n",recvbuf,inet_ntoa(from.sin_addr), ntohs(from.sin_port));
		}


	return S_OK;
}

HRESULT CMulticastEndTestDlg::NetCall_Connect( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
return S_OK;
}

HRESULT CMulticastEndTestDlg::NetCall_Close( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
return S_OK;
}

HRESULT CMulticastEndTestDlg::NetCall_Write( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
return S_OK;
}

HRESULT CMulticastEndTestDlg::SockStateChange( CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn )
{
return S_OK;
}

HRESULT CMulticastEndTestDlg::NetCall_Accept( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	if (nErr != S_OK)
	{
		ASSERT(FALSE);
		return nErr;
	}
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	SOCKADDR sasession;
	int sasessionsz;
	socRecv = accept (*pMBCSock, (SOCKADDR *)&sasession, &sasessionsz);
	SOCKADDR_IN addr;
	memcpy(&addr, &sasession, sizeof(sasession));

	CFWriteLog(TEXT("accept sock in Src = %s"), Addr2String(addr).c_str());

return S_OK;
}



