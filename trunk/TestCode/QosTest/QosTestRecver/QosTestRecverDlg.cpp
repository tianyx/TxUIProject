
// QosTestRecverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QosTestRecver.h"
#include "QosTestRecverDlg.h"
#include <map>
#include "FGlobal.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MSG_RECVTEST WM_USER+1201


using namespace std;
typedef map<SOCKET, SOCKET> MAPACCEPTEDSOCKS;


SOCKET socRecv  = INVALID_SOCKET;
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


// CQosTestRecverDlg �Ի���




CQosTestRecverDlg::CQosTestRecverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQosTestRecverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQosTestRecverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_EDIT_TOSEND, m_edttoSend);
}

BEGIN_MESSAGE_MAP(CQosTestRecverDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CQosTestRecverDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_SEND, &CQosTestRecverDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// CQosTestRecverDlg ��Ϣ�������

BOOL CQosTestRecverDlg::OnInitDialog()
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
	WSADATA data;
	WSAStartup(MAKEWORD(2,2), &data);
	g_hwndLog = this->GetSafeHwnd();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CQosTestRecverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQosTestRecverDlg::OnPaint()
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
HCURSOR CQosTestRecverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQosTestRecverDlg::OnBnClickedButton1()
{
	if (socRecv != INVALID_SOCKET)
	{
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int s_PortGen= time(NULL)%12000 +2000;
	socRecv = socket(AF_INET, SOCK_STREAM, 0);
	ASSERT(socRecv != INVALID_SOCKET);

	SOCKADDR_IN addrLocal;
	addrLocal.sin_family = PF_INET;
	addrLocal.sin_port = htons(s_PortGen);
	addrLocal.sin_addr.S_un.S_addr = inet_addr(TEXT("172.19.52.57"));
	int naddrLen = sizeof(sockaddr);
	int nRet = bind(socRecv, (sockaddr*)&addrLocal, naddrLen);
	ASSERT(nRet != SOCKET_ERROR);

	SOCKADDR_IN addrSvr;
	addrSvr.sin_family = PF_INET;
	addrSvr.sin_port = htons(14444);
	addrSvr.sin_addr.S_un.S_addr = inet_addr(TEXT("172.19.52.57"));
	naddrLen = sizeof(sockaddr);

	nRet = WSAConnect(socRecv, (sockaddr*)&addrSvr, naddrLen, NULL, NULL, NULL, NULL);
	if(nRet == SOCKET_ERROR)
	{
		ASSERT(FALSE);
		nRet = WSAGetLastError();
		return;
	}
	nRet = WSAAsyncSelect(socRecv, this->GetSafeHwnd(), MSG_RECVTEST, FD_ACCEPT|FD_READ|FD_WRITE|FD_CLOSE|FD_QOS);
	if(nRet == SOCKET_ERROR)
	{
		ASSERT(FALSE);
		nRet = WSAGetLastError();
		return;
	}

	CFWriteLog(TEXT("connect successfully. soc = %d"), socRecv);
}

LRESULT CQosTestRecverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	MACRO_EDITLOGOUTPUT
	if (message == MSG_RECVTEST)
	{
		int nErr =WSAGETSELECTERROR(lParam);
		int nEvent = WSAGETSELECTEVENT(lParam);
		if (nEvent == FD_ACCEPT)
		{
			CFWriteLog("\nclient accepted soc = %d, event = %d, err = %d", (int)wParam, nEvent, nErr);
		}
		else if (nEvent == FD_READ)
		{
			socRecv = (SOCKET)wParam;
			WSABUF wsaBuff;
			char szbuff[256];
			ZeroMemory(szbuff, 256);
			wsaBuff.buf = szbuff;
			wsaBuff.len = 255;
			DWORD nReaded = 0;
			DWORD dwFlag = 0;
			int nRet = WSARecv(socRecv, &wsaBuff, 1, &nReaded, &dwFlag, NULL, NULL);
			//int nRet = recv(socRecv, szbuff, wsaBuff.len,  0);
			if (nRet == SOCKET_ERROR)
			{
				nRet = WSAGetLastError();
			}
			if ( nReaded != 0)
			{
				CFWriteLog("client msg ReadData, sock = %d, msg= %s", (int)wParam, wsaBuff.buf);
			}
		}
		else if (nEvent == FD_CLOSE)
		{
			CFWriteLog("\nclient msg svr closed soc = %d, nErr = %d", (int)wParam, nErr);
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
			CFWriteLog("\nclient msg soc = %d, event = %d, err = %d", (int)wParam, nEvent, nErr);
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CQosTestRecverDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (socRecv != INVALID_SOCKET)
	{
		closesocket(socRecv);
		socRecv = INVALID_SOCKET;
	}
	WSACleanup();
	CDialog::PostNcDestroy();
}

void CQosTestRecverDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (socRecv == INVALID_SOCKET)
	{
		return;
	}

	WSABUF wsaBuff;
	char szbuff[256];
	ZeroMemory(szbuff, 256);
	m_edttoSend.GetWindowText(szbuff, 255);
	wsaBuff.buf = szbuff;
	wsaBuff.len = strlen(szbuff);
	DWORD nReaded = 0;
	
	DWORD dwSendLen = 0;
	int nRet = WSASend(socRecv, &wsaBuff, 1, &dwSendLen,  0, NULL, NULL);
	ASSERT(nRet != SOCKET_ERROR);
	CFWriteLog(TEXT("send to svr = %s"), szbuff);
}
