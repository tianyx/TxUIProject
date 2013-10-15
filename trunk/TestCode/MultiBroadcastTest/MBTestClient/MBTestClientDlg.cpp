
// MBTestClientDlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
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


// CMBTestClientDlg �Ի���




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


// CMBTestClientDlg ��Ϣ�������

BOOL CMBTestClientDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMBTestClientDlg::OnPaint()
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (IDTIMER_MCRECV == nIDEvent)
	{
		
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CMBTestClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	MACRO_EDITLOGOUTPUT
	return CDialog::WindowProc(message, wParam, lParam);
}
