
// MBTestClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MBTestClient.h"
#include "MBTestClientDlg.h"
#include <string>
#include "FGlobal.h"
#include "StrConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  IDTIMER_MCRECV 1021
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CRITICAL_SECTION g_soclock;

DWORD __stdcall RecvProc(void* lparam);

DWORD __stdcall RecvProc( void* lparam )
{

	SOCKET& socNew = *(SOCKET*)lparam;
	while (TRUE)
	{
		if (socNew == INVALID_SOCKET)
		{
			CFWriteLog(TEXT("recv thread closed"));
			return 0;
		}
		//receive loopback sdata
		wchar_t szRecvBuff[128];
		ZeroMemory(szRecvBuff, sizeof(szRecvBuff));
		WSABUF wsaRecvBuff;
		wsaRecvBuff.buf = (char*)szRecvBuff;
		wsaRecvBuff.len = 126;
		DWORD dwByteRecv= 0;
		DWORD dwFlag = 0;
		int naddrLen = sizeof(sockaddr);
		BOOL nRet = 0;
		//nRet = WSARecvFrom(socNew, & wsaRecvBuff, 1, &dwByteRecv, &dwFlag, (sockaddr*)&m_addrDes, &naddrLen, 0,0 );
		nRet = WSARecv(socNew, &wsaRecvBuff, 1, &dwByteRecv, &dwFlag, NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			CFWriteLog(TEXT("recv multicast data error code = %d"), WSAGetLastError());
		}
		else
		{
			TRACE(TEXT("recv multicast data = %s"), szRecvBuff);
			CFWriteLog(TEXT("recv:= %s"), szRecvBuff);
		}
		Sleep(100);
	}
	return 0;
}

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


// CMBTestClientDlg 对话框




CMBTestClientDlg::CMBTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMBTestClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_socRecv = INVALID_SOCKET;
	m_socNew = INVALID_SOCKET;
	m_hrecvThread = NULL;
	//InitializeCriticalSection(&g_soclock);
}

void CMBTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_EDIT_LOCALIP, m_edtLocalIp);
	DDX_Control(pDX, IDC_EDIT_MCIP, m_edtMCIp);
	DDX_Control(pDX, IDC_EDIT_LOCALPORT, m_edtLocalPort);
	DDX_Control(pDX, IDC_EDIT_MCPORT, m_edtMCPort);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(CMBTestClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CMBTestClientDlg::OnBnClickedBtnStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMBTestClientDlg 消息处理程序

BOOL CMBTestClientDlg::OnInitDialog()
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
	hostent* pHostent = gethostbyname("");
	char* localIP;
	localIP = inet_ntoa (*(struct in_addr *)*pHostent->h_addr_list);
	std::string strLocalIp = localIP;
	std::wstring wszLocalIp = Ansi2W(strLocalIp);
	g_hwndLog = this->GetSafeHwnd();
	m_edtLocalIp.SetWindowText(wszLocalIp.c_str());
	m_edtMCIp.SetWindowText(TEXT("235.6.7.8"));
	m_edtMCPort.SetWindowText(TEXT("51001"));
	m_edtLocalPort.SetWindowText(TEXT("42001"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMBTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMBTestClientDlg::OnPaint()
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
HCURSOR CMBTestClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CMBTestClientDlg::StartRecv()
{
	
	CString strIPLocal;
	CString strIPMB;
	CString strPortMB;
	CString strPortLocal;
	m_edtLocalIp.GetWindowText(strIPLocal);
	m_edtMCIp.GetWindowText(strIPMB);
	m_edtMCPort.GetWindowText(strPortMB);
	m_edtLocalPort.GetWindowText(strPortLocal);
	unsigned short nPortMB = _wtoi(strPortMB);
	unsigned short nPortLocal = _wtoi(strPortLocal);
	

	m_addrDes.sin_family = PF_INET;
	m_addrDes.sin_port = htons(nPortMB);
	std::wstring wszDesIp = strIPMB.GetBuffer();
	std::string szDesIp = W2Ansi(wszDesIp);
	m_addrDes.sin_addr.S_un.S_addr = inet_addr(szDesIp.c_str());
	
// 	BOOL bRet = bind(m_socRecv, (sockaddr*)&m_addrDes,  sizeof(sockaddr));
// 	if (bRet == SOCKET_ERROR)
// 	{
// 		CFWriteLog(TEXT("bind mc ip error, ip = %s, port = %d"), strIPMB, nPortMB);
// 		return FALSE;
// 	}


	//call bind, listen, accept all together.
	//m_socNew = WSAJoinLeaf(m_socRecv, (sockaddr*)& m_addrDes, sizeof(m_addrDes), NULL, NULL, NULL, NULL, JL_RECEIVER_ONLY);
	DWORD dwSessionPort = 51001;
	SOCKADDR_IN   salocal,sasession;

	salocal.sin_family = AF_INET;
	salocal.sin_port   = htons (dwSessionPort);    
	salocal.sin_addr.s_addr = inet_addr (szDesIp.c_str());

	int nRet = bind (m_socRecv, (SOCKADDR *)&salocal, sizeof(salocal));
	ASSERT(nRet != SOCKET_ERROR );
	//
	// Set all relevant receiver socket options here
	//

	nRet = listen (m_socRecv, 10);
	ASSERT(nRet != SOCKET_ERROR );

	int           sasessionsz;

	sasessionsz = sizeof(sasession);
	m_socNew = accept (m_socRecv, (SOCKADDR *)&sasession, &sasessionsz);

	if (m_socNew == INVALID_SOCKET)
	{
		CFWriteLog(TEXT("joinleaf error! code = %d"), WSAGetLastError());
		return FALSE;
	}

	//SetTimer(IDTIMER_MCRECV, 1000, 0);
	DWORD dwThreadId = NULL;
	m_hrecvThread = ::CreateThread(NULL,  0, RecvProc, &m_socNew, 0, &dwThreadId);
	ASSERT(m_hrecvThread != NULL);

	return TRUE;
}


void CMBTestClientDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_socRecv != INVALID_SOCKET)
	{
		CloseRecv();
		m_btnStart.SetWindowText(TEXT("Start"));
		return ;
	}

	//m_socRecv = socket(AF_INET, SOCK_RDM,IPPROTO_PGM);
	//m_socRecv = WSASocket(AF_INET, SOCK_RDM, IPPROTO_PGM, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF);
//	m_socRecv = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF);
	m_socRecv = socket (AF_INET, SOCK_RDM, IPPROTO_PGM);

	if (m_socRecv == INVALID_SOCKET)
	{
		CFWriteLog(TEXT("recv sock create error code = %d"), WSAGetLastError());
		return ;
	}

	if (!StartRecv())
	{
		CloseRecv();
		return;
	}
	m_btnStart.SetWindowText(TEXT("Stop"));

}

void CMBTestClientDlg::CloseRecv()
{

	if (m_socNew != INVALID_SOCKET)
	{
		shutdown(m_socNew, SD_RECEIVE);
		closesocket(m_socNew);
		m_socNew = INVALID_SOCKET;
	}
	if (m_socRecv != INVALID_SOCKET)
	{
		closesocket(m_socRecv);
		m_socRecv = NULL;
	}

	if (m_hrecvThread)
	{
		CloseHandle(m_hrecvThread);
		m_hrecvThread = NULL;
	}
	
}

void CMBTestClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (IDTIMER_MCRECV == nIDEvent)
	{
		
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CMBTestClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	MACRO_EDITLOGOUTPUT
	return CDialog::WindowProc(message, wParam, lParam);
}
