
// MultiBroadcastTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MultiBroadcastTest.h"
#include "MultiBroadcastTestDlg.h"
#include "FGlobal.h"
#include "winsock2.h"
#include "StrConvert.h"
#include "NetCommon.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  IDTIMER_RECVMCDATA 1011
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


// CMultiBroadcastTestSvrDlg 对话框




CMultiBroadcastTestSvrDlg::CMultiBroadcastTestSvrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiBroadcastTestSvrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_socSvr = INVALID_SOCKET;
	m_mcsocNew = INVALID_SOCKET;
}

void CMultiBroadcastTestSvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP_LOCAL, m_edtIPLocal);
	DDX_Control(pDX, IDC_EDIT_IP_MB, m_edtIPMB);
	DDX_Control(pDX, IDC_EDIT_PORT_MB, m_edtPortMB);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_EDIT_PORT_LOCAL, m_edtPortLocal);
}

BEGIN_MESSAGE_MAP(CMultiBroadcastTestSvrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CMultiBroadcastTestSvrDlg::OnBnClickedBtnStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMultiBroadcastTestSvrDlg 消息处理程序

BOOL CMultiBroadcastTestSvrDlg::OnInitDialog()
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
	g_hwndLog = this->GetSafeHwnd();

	hostent* pHostent = gethostbyname("");
	char* localIP;
	localIP = inet_ntoa (*(struct in_addr *)*pHostent->h_addr_list);
	std::string strLocalIp = localIP;
	std::wstring wszLocalIp = Ansi2W(strLocalIp);
	m_edtIPLocal.SetWindowText(wszLocalIp.c_str());
	m_edtIPMB.SetWindowText(TEXT("235.6.7.8"));
	m_edtPortMB.SetWindowText(TEXT("51001"));
	m_edtPortLocal.SetWindowText(TEXT("41001"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMultiBroadcastTestSvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMultiBroadcastTestSvrDlg::OnPaint()
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
HCURSOR CMultiBroadcastTestSvrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMultiBroadcastTestSvrDlg::OnBnClickedBtnStart()
{
	if (m_socSvr != INVALID_SOCKET)
	{
		CloseMulticast();
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	//step 1. call wsastartup check support winsocket2.0
	//setp 2. check multibroadcast support IGMP, XP1_SUPPORT_MULTIPOINT
	DWORD dwEnumBufferLen = 0;
	int nRet = WSAEnumProtocols(NULL, NULL, &dwEnumBufferLen);
	if (WSAGetLastError() == WSAENOBUFS)
	{
		byte * pInfoList = new byte[dwEnumBufferLen];
		nRet = WSAEnumProtocols(NULL, (LPWSAPROTOCOL_INFO)pInfoList, &dwEnumBufferLen);
		WSAPROTOCOL_INFO* pProtInfo = (WSAPROTOCOL_INFO*)pInfoList;
		for (int i = 0; i < nRet; i++)
		{
			TRACE(TEXT("\nprotocol = %s, id = %d"), pProtInfo[i].szProtocol, pProtInfo[i].iProtocol);
			if ((pProtInfo[i].dwServiceFlags1 & (XP1_QOS_SUPPORTED|XP1_SUPPORT_MULTIPOINT)) != 0)
			{
				
				CFWriteLog(TEXT("found protocol support multicast, name = %s"), pProtInfo[i].szProtocol);
			}
		}
		delete[] pInfoList;
		pInfoList = NULL;
	}
	else
	{
		CFWriteLog(TEXT("WSAEnumProtocols err, errcode = %d"), WSAGetLastError());
		return;
	}

	WSAPROTOCOL_INFO proInfo;
	ZeroMemory(&proInfo, sizeof(proInfo));
	//SOCK_RDM 必须使用类似tcp连接方式。
	//无root模式，全部为叶子
	m_socSvr = socket(AF_INET, SOCK_RDM, IPPROTO_PGM);
	//m_socSvr = WSASocket(AF_INET,SOCK_RDM, IPPROTO_PGM, NULL, 0,  WSA_FLAG_MULTIPOINT_C_LEAF|WSA_FLAG_MULTIPOINT_D_LEAF);
	if (m_socSvr == INVALID_SOCKET)
	{
		CFWriteLog(TEXT("sock create error! code = %d"), WSAGetLastError());
		return;
	}
	else
	{
		CFWriteLog(TEXT("sock create success..."));
	}


	if (!StartMulticast())
	{
		CloseMulticast();
		return;
	}

	m_btnStart.SetWindowText(TEXT("Stop"));

}

LRESULT CMultiBroadcastTestSvrDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	MACRO_EDITLOGOUTPUT
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CMultiBroadcastTestSvrDlg::StartMulticast()
{

	if (m_socSvr == NULL)
	{
		return FALSE;
	}
	//step3 create socket use SOCK_RDM, IPPROTO_PGM,  WSA_FLAG_OVERLAPPED|WSA_FLAG_MULTIPOINT_C_LEAF|WSA_FLAG_MULTIPOINT_D_LEAF
	CString strIPLocal;
	CString strIPMB;
	CString strPortMB;
	CString strPortLocal;
	m_edtIPLocal.GetWindowText(strIPLocal);
	m_edtIPMB.GetWindowText(strIPMB);
	m_edtPortMB.GetWindowText(strPortMB);
	m_edtPortLocal.GetWindowText(strPortLocal);
	unsigned short nPortMB = _wtoi(strPortMB);
	unsigned short nPortLocal = _wtoi(strPortLocal);


	//step 4:set reused option
/*
	DWORD dwOptVal = TRUE;
	int nRet = setsockopt(m_socSvr, SOL_SOCKET, SO_REUSEADDR, (char*)&dwOptVal, sizeof(dwOptVal));
	if (nRet == SOCKET_ERROR)
	{
		CFWriteLog(TEXT("set reused opt err, code = %d"), WSAGetLastError());
		return FALSE;
	}
	else
	{
		CFWriteLog(TEXT("sock set reused opt success..."));
	}*/

	//step 5: bind local ip
// 	hostent* pHostent = gethostbyname("");
//	char* localIP;
// 	localIP = inet_ntoa (*(struct in_addr *)*pHostent->h_addr_list);
	
	std::wstring wszIpLocal = strIPLocal.GetBuffer();
	std::string szIpLocal = W2Ansi(wszIpLocal);
	const char* localIP = szIpLocal.c_str();
	m_addrSrc.sin_family = PF_INET;
	m_addrSrc.sin_port = htons(nPortLocal);
	m_addrSrc.sin_addr.S_un.S_addr = INADDR_ANY;
	//m_addrSrc.sin_addr.S_un.S_addr = inet_addr(localIP);

	int nRet = bind(m_socSvr, (struct sockaddr FAR *)&m_addrSrc, sizeof(sockaddr));

	std::string strIP = localIP;
	std::wstring wszIp = Ansi2W(strIP);
	if (nRet == SOCKET_ERROR)
	{

		CFWriteLog(TEXT("bind ip error!, ip = %s, port = %d, errcode = %d"), wszIp.c_str(), nPortLocal, WSAGetLastError());

		return FALSE;
	}
	else
	{
		CFWriteLog(TEXT("bind ip success., ip = %s, port = %d"), wszIp.c_str(), nPortLocal);

	}

/*
	//step 6: set multicast range (TTL)
	DWORD nIP_TTL = 1;      // 0表示仅本机，不在网上传播，1表示在本子网中传播。>1表示跨多少个路由传播，如果要跨路由器，则路由器必须支持IGMP协议, 
	DWORD cbRet = 0;
 	nRet = WSAIoctl(m_socSvr, SIO_MULTICAST_SCOPE, &nIP_TTL, sizeof(nIP_TTL), 
 		NULL, 0,&cbRet, NULL, NULL);
	//nRet = setsockopt(m_socSvr, IPPROTO_IP,IP_MULTICAST_TTL,(char*)&nIP_TTL, sizeof(nIP_TTL));
	if (nRet == SOCKET_ERROR)
	{
		CFWriteLog(TEXT("set SIO_MULTICAST_SCOPE error. errcode =%d"), WSAGetLastError());
		return FALSE;
	}
*/
	//step 7 :(optional) set 缺省允许内部回送（LOOPBACK）。Windows 95不支持改选项

	DWORD bFlag = TRUE;

// 	nRet = WSAIoctl (m_socSvr,                        /* socket */
// 		SIO_MULTIPOINT_LOOPBACK,         /* LoopBack on or off */
// 		&bFlag,                            /* input */
// 		sizeof (bFlag),                    /* size */
// 		NULL,                                      /* output */ 
// 		0,                                         /* size */
// 		&cbRet,                                    /* bytes returned */
// 		NULL,                                      /* overlapped */
// 		NULL);                                     /* completion routine */
// 
// 	if (nRet == SOCKET_ERROR)
// 	{
// 		CFWriteLog(TEXT("set SIO_MULTIPOINT_LOOPBACK error. errcode =%d"), WSAGetLastError());
// 		return FALSE;
// 	}

	//step 8: join multicast group, set as sender and receiver JL_SENDER_ONLY,JL_RECEIVER_ONLY,(JL_BOTH).

	m_addrDes.sin_family = PF_INET;
	m_addrDes.sin_port = htons(nPortMB);
	std::wstring wszDesIp = strIPMB.GetBuffer();
	std::string szDesIp = W2Ansi(wszDesIp);
	m_addrDes.sin_addr.S_un.S_addr = inet_addr(szDesIp.c_str());
	
	nRet = connect (m_socSvr, (SOCKADDR *)&m_addrDes, sizeof(SOCKADDR));
	if (nRet == SOCKET_ERROR)
	{
		CFWriteLog(TEXT("connect failed..., ip = %s, port = %d"), wszDesIp.c_str(), nPortMB);
		return FALSE;
	}
	m_mcsocNew = m_socSvr;
	//m_mcsocNew = WSAJoinLeaf(m_socSvr,(sockaddr*)&m_addrDes, sizeof(sockaddr), NULL, NULL, NULL, NULL, JL_SENDER_ONLY);	
	if (m_mcsocNew == INVALID_SOCKET)
	{
		CFWriteLog(TEXT("WSAJoinLeaf failed..., ip = %s, port = %d"), wszDesIp.c_str(), nPortMB);
		return FALSE;
	}
	else
	{
		CFWriteLog(TEXT("WSAJoinLeaf success..., ip = %s, port = %d"), wszDesIp.c_str(), nPortMB);

	}

	SetTimer(IDTIMER_RECVMCDATA, 2000, NULL);
	return TRUE;
}

void CMultiBroadcastTestSvrDlg::CloseMulticast()
{
	KillTimer(IDTIMER_RECVMCDATA);
	if (m_mcsocNew == INVALID_SOCKET)
	{
		closesocket(m_mcsocNew);
		m_mcsocNew = INVALID_SOCKET;
	}
	if (m_socSvr != INVALID_SOCKET)
	{
		closesocket(m_socSvr);
		m_socSvr = INVALID_SOCKET;
	}

	m_btnStart.SetWindowText(TEXT("Start"));
}

void CMultiBroadcastTestSvrDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDTIMER_RECVMCDATA)
	{
		if (m_socSvr == INVALID_SOCKET)
		{
			KillTimer(IDTIMER_RECVMCDATA);
			return;
		}
		static int nsMCVal = 0;

		//send data
		++nsMCVal;
		wchar_t szBuff[64];
		ZeroMemory(szBuff, sizeof(szBuff));
		wsprintf(szBuff, TEXT("\nsvr send = %d"), nsMCVal);
		DWORD dwBuffLen = wcslen(szBuff)* 2;
		DWORD dwSend = 0;
		WSABUF wsaBuff;
		wsaBuff.buf = (char*)szBuff;
		wsaBuff.len = dwBuffLen;
		//int nRet = WSASendTo(m_mcsocNew, &wsaBuff, 1, &dwSend,  0,(sockaddr*)&m_addrDes, sizeof(sockaddr), 0,0);
		int nRet = WSASend(m_mcsocNew, &wsaBuff, 1, &dwSend, 0, NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			CFWriteLog(TEXT("send data error code = %d"), WSAGetLastError());
		}
		else
		{
			TRACE(TEXT("send data success, data = %s"), szBuff);
		}
	
		/*//receive loopback sdata
		wchar_t szRecvBuff[128];
		ZeroMemory(szRecvBuff, sizeof(szRecvBuff));
		WSABUF wsaRecvBuff;
		wsaRecvBuff.buf = (char*)szRecvBuff;
		wsaRecvBuff.len = 126;
		DWORD dwByteRecv= 0;
		DWORD dwFlag = 0;
		int naddrLen = sizeof(sockaddr);
		nRet = WSARecvFrom(m_socSvr, & wsaRecvBuff, 1, &dwByteRecv, &dwFlag, (sockaddr*)&m_addrDes, &naddrLen, 0,0 );
		if (nRet == SOCKET_ERROR)
		{
			CFWriteLog(TEXT("recv loopback data error code = %d"), WSAGetLastError());
		}
		else
		{
			TRACE(TEXT("recv loopback data = %s"), szRecvBuff);
		}*/
	}
	CDialog::OnTimer(nIDEvent);
}
