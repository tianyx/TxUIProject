
// QosTestSenderDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
typedef map<SOCKET, SOCKET> MAPACCEPTEDSOCKS;
MAPACCEPTEDSOCKS m_mapSocks;
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


// CQosTestSenderDlg �Ի���




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


// CQosTestSenderDlg ��Ϣ�������

BOOL CQosTestSenderDlg::OnInitDialog()
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQosTestSenderDlg::OnPaint()
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
HCURSOR CQosTestSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQosTestSenderDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	qosParm.SendingFlowspec.TokenRate = QOS_NOT_SPECIFIED; //�������� bytes/s
	qosParm.SendingFlowspec.TokenBucketSize =QOS_NOT_SPECIFIED; //����ۻ������� bytes/s
	qosParm.SendingFlowspec.ServiceType = SERVICETYPE_CONTROLLEDLOAD;//��������
	qosParm.SendingFlowspec.PeakBandwidth = QOS_NOT_SPECIFIED;//������� bytes/s
	qosParm.SendingFlowspec.MinimumPolicedSize = QOS_NOT_SPECIFIED;//��С���ݰ�����,�ֽ�
	qosParm.SendingFlowspec.MaxSduSize = QOS_NOT_SPECIFIED;//������ݰ�����,�ֽ�
	qosParm.SendingFlowspec.Latency = QOS_NOT_SPECIFIED; //����ӳ٣�����
	qosParm.SendingFlowspec.DelayVariation=QOS_NOT_SPECIFIED;//�����С�ӳٲ�ֵ������
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
	// TODO: �ڴ����ר�ô����/����û���
	MACRO_EDITLOGOUTPUT
	if (message == MSG_TEST_NETMSG)
	{
		ProcNetInform(wParam, lParam);
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CQosTestSenderDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
