
// QosTestSenderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QosTestSender.h"
#include "QosTestSenderDlg.h"
#include "winsock2.h"
#include <map>
#include "FGlobal.h"
#include "qossp.h"
#pragma comment(lib, "ws2_32.lib")

#define MSG_TEST_NETMSG WM_USER + 111

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET m_socListener = INVALID_SOCKET;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
typedef map<SOCKET, SOCKET> MAPACCEPTEDSOCKS;
MAPACCEPTEDSOCKS m_mapSocks;
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


// CQosTestSenderDlg 对话框




CQosTestSenderDlg::CQosTestSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQosTestSenderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQosTestSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_EDIT_TOSEND, m_edttoSend);
}

BEGIN_MESSAGE_MAP(CQosTestSenderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CQosTestSenderDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_SEND, &CQosTestSenderDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CQosTestSenderDlg 消息处理程序

BOOL CQosTestSenderDlg::OnInitDialog()
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
	WSADATA data;
	WSAStartup(MAKEWORD(2,2), &data);
	g_hwndLog = this->GetSafeHwnd();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQosTestSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQosTestSenderDlg::OnPaint()
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
HCURSOR CQosTestSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQosTestSenderDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_socListener != INVALID_SOCKET)
	{
		return;
	}

	m_socListener = socket(AF_INET, SOCK_STREAM, 0);
	ASSERT(m_socListener != INVALID_SOCKET);
	
	SOCKADDR_IN addrLocal;
	addrLocal.sin_family = PF_INET;
	addrLocal.sin_port = htons(14444);
	addrLocal.sin_addr.S_un.S_addr = inet_addr(TEXT("172.19.52.57"));
	int naddrLen = sizeof(sockaddr);
	int nRet = bind(m_socListener, (sockaddr*)&addrLocal, naddrLen);
	ASSERT(nRet != SOCKET_ERROR);
	listen(m_socListener, 10);

	//set qos on listener, the incoming sock will inherit it;
	QOS qosParm;
	qosParm.SendingFlowspec.TokenRate = QOS_NOT_SPECIFIED; //传输速率 bytes/s
	qosParm.SendingFlowspec.TokenBucketSize =QOS_NOT_SPECIFIED; //最大累积数据量 bytes/s
	qosParm.SendingFlowspec.ServiceType = SERVICETYPE_CONTROLLEDLOAD;//服务类型
	qosParm.SendingFlowspec.PeakBandwidth = QOS_NOT_SPECIFIED;//定点带宽 bytes/s
	qosParm.SendingFlowspec.MinimumPolicedSize = QOS_NOT_SPECIFIED;//最小数据包长度,字节
	qosParm.SendingFlowspec.MaxSduSize = QOS_NOT_SPECIFIED;//最大数据包长度,字节
	qosParm.SendingFlowspec.Latency = QOS_NOT_SPECIFIED; //最大延迟，毫秒
	qosParm.SendingFlowspec.DelayVariation=QOS_NOT_SPECIFIED;//最大最小延迟差值，毫秒
	qosParm.ReceivingFlowspec = qosParm.SendingFlowspec;
	qosParm.ReceivingFlowspec.ServiceType = SERVICETYPE_NETWORK_CONTROL; //highest priority

	QOS_DESTADDR qosDest;
	qosDest.ObjectHdr.ObjectLength = sizeof(qosDest);
	qosDest.ObjectHdr.ObjectType = QOS_OBJECT_DESTADDR;
	SOCKADDR_IN socaddrDes;
	socaddrDes.sin_family = PF_INET;
	socaddrDes.sin_port = htons(0);
	qosDest.SocketAddress = htonl (INADDR_ANY);
	qosDest.SocketAddress = & socaddrDes;
	qosDest.SocketAddressLength = sizeof(socaddrDes);
	qosParm.ProviderSpecific.buf = &qosDest;
	qosParm.ProviderSpecific.len = qosDest.ObjectHdr.ObjectLength;
	nRet = WSAIoctl(m_socListener, SIO_SET_QOS, &qosParm, sizeof(qosParm),NULL, NULL,NULL,NULL,NULL);

	nRet = WSAAsyncSelect(m_socListener, this->GetSafeHwnd(), MSG_TEST_NETMSG, FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE|FD_QOS);
	ASSERT(nRet != SOCKET_ERROR);

	CFWriteLog(TEXT("listen successfully. soc = %d"), m_socListener);

}

int ProcNetInform(WPARAM wParam, LPARAM lParam)
{

	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;

	if (nEvent == FD_ACCEPT)
	{
		SOCKADDR_IN addrDes;
		int naddrLen = sizeof(SOCKADDR_IN);
		socRecv = accept(m_socListener, (sockaddr*)&addrDes, &naddrLen);
		CFWriteLog("svrmsg accepted, soc = %d, des ip = %s, des port = %d", socRecv, inet_ntoa(addrDes.sin_addr), ntohs(addrDes.sin_port));
		m_mapSocks[socRecv] = socRecv;
	}
	else if (nEvent == FD_READ)
	{
		WSABUF wsaBuff;
		char szbuff[256];
		ZeroMemory(szbuff, 256);
		wsaBuff.buf = szbuff;
		wsaBuff.len = 255;
		DWORD nReaded = 0;
		DWORD dwFlag = 0;
		int nRet = WSARecv(socRecv, &wsaBuff, 1, &nReaded, &dwFlag, NULL, NULL);

		if (nRet == SOCKET_ERROR)
		{
			nRet = WSAGetLastError();
		}
		if ( nReaded != 0)
		{
			CFWriteLog("svrmsg ReadData, sock = %d, msg= %s", (int)wParam, wsaBuff.buf);
		}

	}
	else if (nEvent == FD_CLOSE)
	{
		MAPACCEPTEDSOCKS::iterator itb = m_mapSocks.find(socRecv);
		if (itb != m_mapSocks.end())
		{
			m_mapSocks.erase(itb);
		}
		CFWriteLog("svrmsg client close, sock = %d", (int)wParam);
	}
	else if (nEvent == FD_QOS)
	{
		QOS qosParam;
		byte bBuffer[2048];
		int nReturned = 0;
		int nRet = WSAIoctl(socRecv, SIO_GET_QOS, 0,0, bBuffer, 2048, &nReturned,NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			nRet = WSAGetLastError();
		}
	}
	else
	{
		CFWriteLog("svrmsg sock = %d, msg = %d, err = %d", (int)wParam, nEvent, nErr);
	}

	return 0;
}


LRESULT CQosTestSenderDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	MACRO_EDITLOGOUTPUT
	if (message == MSG_TEST_NETMSG)
	{
		ProcNetInform(wParam, lParam);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CQosTestSenderDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_socListener != INVALID_SOCKET)
	{
		closesocket(m_socListener);
		m_socListener = INVALID_SOCKET;
	}

	MAPACCEPTEDSOCKS::iterator itb = m_mapSocks.begin();
	MAPACCEPTEDSOCKS::iterator ite = m_mapSocks.end();

	for (; itb != ite; ++itb)
	{
		if (itb->second != INVALID_SOCKET)
		{
			closesocket(itb->second);
			itb->second = INVALID_SOCKET;
		}
	}

	m_mapSocks.clear();
	
	WSACleanup();
	CDialog::PostNcDestroy();
}

void CQosTestSenderDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	WSABUF wsaBuff;
	char szbuff[256];
	ZeroMemory(szbuff, 256);
	m_edttoSend.GetWindowText(szbuff, 255);
	wsaBuff.buf = szbuff;
	wsaBuff.len = 255;
	DWORD nReaded = 0;

	DWORD dwSendLen = strlen(szbuff);
	MAPACCEPTEDSOCKS::iterator itb = m_mapSocks.begin();
	MAPACCEPTEDSOCKS::iterator ite = m_mapSocks.end();
	for (; itb != ite; ++itb)
	{
		if (itb->second != INVALID_SOCKET)
		{
			int nRet = WSASend(itb->second, &wsaBuff, 1, &dwSendLen,  0, NULL, NULL);
			ASSERT(nRet != SOCKET_ERROR);
		}
	}


	CFWriteLog(TEXT("send to client = %s"), szbuff);
}
