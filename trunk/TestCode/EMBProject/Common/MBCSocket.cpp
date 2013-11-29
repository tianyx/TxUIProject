#include "StdAfx.h"
#include "MBCSocket.h"
#include "FGlobal.h"
#include "EMBCommonFunc.h"
//#include "MBCCommonDef.h"
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegisterSockWnd();
BOOL g_bRegisterSockWnd = RegisterSockWnd();
static TCHAR g_szSockWndClsName[] = TEXT ("mbcnetreveiverWnd");

BOOL RegisterSockWnd()
{
	HINSTANCE hInstance = GetSelfModuleHandle();

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = WndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = g_szSockWndClsName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if ( 0== RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		ASSERT(hr == 0x00000582);
		
	}

	return TRUE;
}

//CreateThreadWindow
DWORD __stdcall CreateSockWndThread( void* lparam )
{
	CMBCSocket* pSock = (CMBCSocket*)lparam;
	HINSTANCE hInstance = GetSelfModuleHandle();
	HWND& hwnd = pSock->m_hSockWnd;
	ASSERT(hwnd == NULL);
	hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		g_szSockWndClsName, 
		TEXT (""),
		WS_OVERLAPPEDWINDOW,
		-10, 
		-10, 
		2, 
		2, 
		NULL, 
		NULL, 
		hInstance,0);
	if (!hwnd)
	{
		ASSERT(FALSE);
		return 0;
	}

	ASSERT(::IsWindow(hwnd));
	::SetWindowLong(hwnd,  GWL_USERDATA, (LONG)pSock);

	//CFWriteLog(TEXT("SockmsgWnd = %x, bind sock %d"), hwnd, this->m_hSock);

	ShowWindow (hwnd, SW_HIDE);
	UpdateWindow (hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete pSock;
	pSock = NULL;
  
	return 0;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		{
		}
		return 1;
	case WM_PAINT:
// 		hdc = BeginPaint (hwnd, &ps);
// 		EndPaint (hwnd, &ps);
		return (0);

	case WM_DESTROY:
		//PostQuitMessage(0);
		return 1;

	case MSG_MBCNETSOCK:
		{	
			int nEvent = WSAGETSELECTEVENT(lParam);
			LONG nData = ::GetWindowLong(hwnd,  GWL_USERDATA);
			if (nData != 0)
			{
				CMBCSocket* pbaseObj =	(CMBCSocket*)nData;
				if (nEvent == FD_ACCEPT)
				{
					//the wnd can't create in thread proc...
					pbaseObj->SockMsgCallback(wParam, lParam);
				}
				else
				{
					MBCSOCKMSG tmpMsg;
					tmpMsg.lparam = lParam;
					tmpMsg.wparam = wParam;
					pbaseObj->PushMsg(tmpMsg);
				}

			}


			return 0;
		}
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

DWORD __stdcall SockMsgLoopProc( void* lparam )
{
	CMBCSocket* pSock = (CMBCSocket*)lparam;
	if (pSock->m_pCallBack == NULL)
	{
		return -1;
	}
	pSock->HasMsg();

	return 0;
}


//////////////////////////////////////////////////////////////////////////

CMBCSocket::CMBCSocket( void )
{
	SOCKDOWN_RECONN_INTERVAL = g_SOCK_INTERVAL_RECONN;
	m_hSock = INVALID_SOCKET;
	m_hMBCSock = INVALID_SOCKET;
	m_hSockLoopProc = NULL;
	m_vecSockMsg.reserve(1000);
	m_nSockType = SOCK_DGRAM;
	m_nProtocolType = 0;
	m_nFavMsgType = 0;
	m_pCallBack = NULL;
	m_vecSockMsg.clear();
	m_hEventSockMsgArrival = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_nCreateFlag = 0;
	//m_hSockLoopProc = CreateThread(NULL, NULL, SockLoopProc, (LPVOID)this, 0, 0);
	m_hSockWnd = NULL;
	m_nSelfState = MBCSOCKSTATE_UNINIT;
	m_hWndThread = NULL;
	CreateMsgWindow();

}

CMBCSocket::~CMBCSocket(void)
{
	m_timerReconnect.KillTimer();

// 	if (m_hSockLoopProc)
// 	{
// 		StopMsgLoopThd();
// 
// 	}

	//ASSERT(!::IsWindow(m_hSockWnd));
// 	if (m_hSockWnd)
// 	{
// 		if (m_hWndThread)
// 		{
// 			//::DestroyWindow(m_hSockWnd);
// 			PostMessage(m_hSockWnd, WM_QUIT, 0,0);
// 			WaitForSingleObject(m_hWndThread, INFINITE);
// 			ASSERT(!::IsWindow(m_hSockWnd));
// 		}
// 		m_hSockWnd = NULL;
// 		m_hWndThread = NULL;
// 	}


	StopMsgLoopThd();
	if (m_hSock != INVALID_SOCKET)
	{
		closesocket(m_hSock);
		m_hSock = INVALID_SOCKET;
	}
	if (m_hMBCSock != INVALID_SOCKET)
	{
		closesocket(m_hMBCSock);
		m_hMBCSock = INVALID_SOCKET;
	}

	ResetEvent(m_hEventSockMsgArrival);
	m_vecSockMsg.clear();

	if (m_hEventSockMsgArrival)
	{
		CloseHandle(m_hEventSockMsgArrival);
		m_hEventSockMsgArrival = NULL;
	}

	if (m_hEventQuit)
	{
		CloseHandle(m_hEventQuit);
		m_hEventQuit = NULL;
	}
	


}

HRESULT CMBCSocket::PushMsg( const MBCSOCKMSG& msg )
{
	CAutoLock lock(&m_lockdeque);
	m_vecSockMsg.push_back(msg);
	SetEvent(m_hEventSockMsgArrival);
	return S_OK;
}


HRESULT CMBCSocket::CopyMsg( MBCSOCKMSG* pmsgOut, const int inBuffCount, int& nOut )
{
	CAutoLock lock(&m_lockdeque);
	nOut = m_vecSockMsg.size() > inBuffCount? inBuffCount:m_vecSockMsg.size();
	for (size_t i = 0; i < nOut; ++i)
	{
		pmsgOut[i] = m_vecSockMsg[i];
	}
	if (nOut < m_vecSockMsg.size())
	{
		CFWriteLog("\nsock msg overflow!!!!!");
	}
	m_vecSockMsg.clear();
	ResetEvent(m_hEventSockMsgArrival);
	return S_OK;
}

CMBCSocket* CMBCSocket::CreateUDPSocket( MCBSOCKADDRS& addrIn, int nFavMsgType, ISockMsgCallbackInterFace* pCallbackProc, int nFlag /*= 0*/)
{
	CMBCSocket* pSock = new CMBCSocket;
	pSock->m_nSockType = SOCK_DGRAM;
	pSock->m_nProtocolType = IPPROTO_UDP;
	pSock->m_nFavMsgType = nFavMsgType;
	pSock->m_pCallBack = pCallbackProc;
	pSock->m_nCreateFlag = nFlag;
// 	pSock->m_addrs.addrFrom.sin_family = AF_INET;
// 	pSock->m_addrs.addrFrom.sin_port = htons(0);
// 	pSock->m_addrs.addrFrom.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	pSock->m_addrs = addrIn;

	if (S_OK != pSock->Init())
	{
		ASSERT(FALSE);
		delete pSock;
		pSock = NULL;
		return NULL;
	}

	return pSock;
}

HRESULT CMBCSocket::Init()
{
	CFWriteLog("^^^socInit %x", this);
	CFWriteLog("^^^socInit Localip =%s, remoteIp =%s", Addr2String(m_addrs.addrLocal).c_str(),Addr2String(m_addrs.addrRemote).c_str());

	if ((m_nCreateFlag & MBCSOCKTYPE_UDP_MBCTYPE) != 0)
	{
		m_hSock=WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,
			WSA_FLAG_MULTIPOINT_C_LEAF|WSA_FLAG_MULTIPOINT_D_LEAF);
		DWORD dwOptVal = TRUE;
		int nRet = setsockopt(m_hSock, SOL_SOCKET, SO_REUSEADDR, (char*)&dwOptVal, sizeof(dwOptVal));
		ASSERT(nRet == S_OK);
	}
	else
	{
		m_hSock = socket(AF_INET, m_nSockType, m_nProtocolType);
	}
	if (m_hSock == INVALID_SOCKET)
	{
		return E_HANDLE;
	}

	HRESULT hr= S_OK;
	if ((m_nCreateFlag & MBCSOCKTYPE_UDP_MBC_RECVER) == 0
		&& (m_nCreateFlag & MBCSOCKTYPE_UDP_MBC_BOTH) == 0)
	{
		//udb receiver can't bind
		hr = bind(m_hSock, (sockaddr*)&m_addrs.addrLocal, sizeof(sockaddr));

		char szCSBuff[256];
		CSADDR_INFO* pcsInfo = (CSADDR_INFO*)szCSBuff;
		int noptLen = 256;
		HRESULT hRet = getsockopt(m_hSock, SOL_SOCKET, SO_BSP_STATE, (char*)pcsInfo, &noptLen);
		SOCKADDR_IN* paddrLocal = NULL;

		SOCKADDR_IN* paddrRemote = NULL;

		if (hRet !=SOCKET_ERROR)
		{
			if (pcsInfo->LocalAddr.iSockaddrLength > 0)
			{
				SOCKADDR_IN* paddrLocal = (SOCKADDR_IN*)pcsInfo->LocalAddr.lpSockaddr;
			}
			if (pcsInfo->RemoteAddr.iSockaddrLength > 0)
			{
				paddrRemote = 	(SOCKADDR_IN*)pcsInfo->RemoteAddr.lpSockaddr;
			}
			CFWriteLog("^^^getsockopt Localip =%s, remoteIp =%s", paddrLocal? Addr2String(*paddrLocal).c_str():"-", paddrRemote? Addr2String(*paddrRemote).c_str():"-");

		}
		

	}
	MUSTBESOK(hr);
	if ((m_nCreateFlag & MBCSOCKTYPE_TCP_LISTENER) != 0)
	{
		listen(m_hSock, 10);
		if((m_nFavMsgType & FD_ACCEPT) == 0)
		{
			ASSERT(FALSE);
			CFWriteLog(TEXT("listener not set FD_ACCEPT msg!"));
		}
	}
	
	if (m_nCreateFlag & MBCSOCKTYPE_UDP_MBCTYPE )
	{
		if (m_nSockType != SOCK_DGRAM)
		{
			return E_INVALIDARG;
		}
		m_hMBCSock = WSAJoinLeaf(m_hSock, (SOCKADDR*)&(m_addrs.addrRemote), sizeof(m_addrs.addrRemote), NULL,NULL, NULL,NULL, (m_nCreateFlag&MBCSOCKTYPE_UDP_MBCTYPE));
		
		if (m_hMBCSock == INVALID_SOCKET)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}
		//disable loopback
		DWORD bFlag = TRUE;
		DWORD dwRet = 0;
		hr = WSAIoctl (m_hMBCSock,                        /* socket */
			SIO_MULTIPOINT_LOOPBACK,         /* LoopBack on or off */
			&bFlag,                            /* input */
			sizeof (bFlag),                    /* size */
			NULL,                                      /* output */ 
			0,                                         /* size */
			&dwRet,                                   /* bytes returned */
			NULL,                                      /* overlapped */
			NULL); 
		//set TTL 
		DWORD nIP_TTL = g_nMBCSockOptTTL;      // 0表示仅本机，不在网上传播，1表示在本子网中传播。>1表示跨多少个路由传播，如果要跨路由器，则路由器必须支持IGMP协议, 
		DWORD cbRet = 0;
		hr = WSAIoctl(m_hMBCSock, SIO_MULTICAST_SCOPE, &nIP_TTL, sizeof(nIP_TTL), 
			NULL, 0,&cbRet, NULL, NULL);

	}


	if (m_hSockWnd == NULL)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}
	if ((m_nCreateFlag & (MBCSOCKTYPE_UDP_MBC_SENDER|MBCSOCKTYPE_UDP_MBC_RECVER)) != 0)
	{
		//sender us sync sock
		TRACE("\ncreate sync udp");
	}
	else
	{
		hr = WSAAsyncSelect(m_hSock, m_hSockWnd, MSG_MBCNETSOCK, m_nFavMsgType);

	}
	MUSTBESOK(hr);
	if ((m_nCreateFlag & MBCSOCKTYPE_TCP_CTRL) != 0)
	{
		BOOL bIs = ::IsWindow(m_hSockWnd);
		hr = connect(m_hSock, (sockaddr*)&m_addrs.addrRemote, sizeof(sockaddr));
		//MUSTBESOK(hr);
	}

	if ((m_nCreateFlag & (MBCSOCKTYPE_UDP_MBC_SENDER|MBCSOCKTYPE_UDP_MBC_RECVER)) == 0)
	{
		StartMsgLoopThd();
	}
	
	m_nSelfState = MBCSOCKSTATE_OK;
	
	return S_OK;
}

void CMBCSocket::UnInit()
{
	CFWriteLog("^^^socUnInit %x", this);
	SetState(MBCSOCKSTATE_UNINIT);
	
	StopMsgLoopThd();
	if (m_hSock != INVALID_SOCKET)
	{
		closesocket(m_hSock);
		m_hSock = INVALID_SOCKET;
	}
	if (m_hMBCSock != INVALID_SOCKET)
	{
		closesocket(m_hMBCSock);
		m_hMBCSock = INVALID_SOCKET;
	}
	
	ResetEvent(m_hEventSockMsgArrival);
	m_vecSockMsg.clear();
	//ReleaseSemaphore(m_hLockQue, 1,NULL);
}

CMBCSocket* CMBCSocket::CreateTCPSocket( MCBSOCKADDRS& addrIn, int nFavMsgType, ISockMsgCallbackInterFace* pCallbackProc, int nFlag /*= 0*/)
{
	CMBCSocket* pSock = new CMBCSocket;
	pSock->m_nSockType = SOCK_STREAM;
	pSock->m_nProtocolType = IPPROTO_TCP;
	pSock->m_nFavMsgType = nFavMsgType;
	pSock->m_pCallBack = pCallbackProc;
	pSock->m_nCreateFlag = nFlag;
// 	pSock->m_addrs.addrFrom.sin_family = AF_INET;
// 	pSock->m_addrs.addrFrom.sin_port = htons(0);
// 	pSock->m_addrs.addrFrom.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	pSock->m_addrs = addrIn;
	if (S_OK != pSock->Init())
	{
		CMBCSocket::ReleaseSock(pSock);
		pSock = NULL;
		return NULL;
	}

	return pSock;
}

BOOL CMBCSocket::IsOpen()
{
	return m_hSock != NULL;

}



BOOL CMBCSocket::CreateMsgWindow()
{
	//CreateThreadWindow
	//create window
	
	//m_hWndhread = CreateThread(NULL, NULL, SockWndLoopProc, (LPVOID)this, 0, 0);
	m_hWndThread = CreateThread(NULL, 0, CreateSockWndThread, (LPVOID)this, 0, 0);
	if (m_hWndThread)
	{
		int i = 0;
		while(m_hSockWnd == 0 && WaitForSingleObject(m_hWndThread, 1) != WAIT_OBJECT_0)
		{
			
			Sleep(1);
		}
	}
	

	return m_hSockWnd != NULL;
}

HRESULT CMBCSocket::SockMsgCallback( WPARAM wParam, LPARAM lParam )
{
	//process ctrl msg
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	if (nEvent == FD_ACCEPT)
	{
		return (m_pCallBack != NULL)? m_pCallBack->NetCall_Accept(this, wParam, lParam) :SockCall_Accept(wParam, lParam);
	}
	else if (nEvent == FD_CONNECT)
	{
		HRESULT hr = (m_pCallBack != NULL)? m_pCallBack->NetCall_Connect(this, wParam, lParam) :SockCall_Connect(wParam, lParam);

		if (nErr ==S_OK)
		{	
			SetState(MBCSOCKSTATE_OK);
		}
		else 
		{
			TRACE("\nsock %x connect failed...", this);
			if (((m_nCreateFlag & MBCSOCKTYPE_AUTORECONNECT) != 0)
				&& (m_nSelfState != MBCSOCKSTATE_UNINIT))
			{
				SetState(MBCSOCKSTATE_CLOSE);
				TRACE("\nsock %x connect  after 5s", this);
				m_timerReconnect.SetTimer(IDTIMER_RECONN, SOCKDOWN_RECONN_INTERVAL, this, (LPARAM)this, WT_EXECUTEONLYONCE|WT_EXECUTEDEFAULT, SOCKDOWN_RECONN_INTERVAL);
			}
		}

		return hr;

	}
	else if (nEvent == FD_READ)
	{
		return (m_pCallBack != NULL)? m_pCallBack->NetCall_Read(this, wParam, lParam) :SockCall_Read(wParam, lParam);
	}
	else if (nEvent == FD_CLOSE)
	{
		SetState(MBCSOCKSTATE_CLOSE);
		HRESULT hr = (m_pCallBack != NULL)? m_pCallBack->NetCall_Close(this, wParam, lParam) :SockCall_Close(wParam, lParam);

		if ((m_nCreateFlag & MBCSOCKTYPE_AUTORECONNECT) != 0 && (m_nSelfState !=MBCSOCKSTATE_UNINIT))
		{
			TRACE("\nsock %x closed Reconnect after 5 s", this);
			m_timerReconnect.SetTimer(IDTIMER_RECONN, SOCKDOWN_RECONN_INTERVAL, this, (LPARAM)this, WT_EXECUTEONLYONCE|WT_EXECUTEDEFAULT, SOCKDOWN_RECONN_INTERVAL);
		}
		return hr;
	}

	return S_OK;
}

HRESULT CMBCSocket::SockCall_Connect( WPARAM wParam, LPARAM lParam )
{
	return S_OK;
}

HRESULT CMBCSocket::SockCall_Read( WPARAM wParam, LPARAM lParam )
{
	return S_OK;
}

HRESULT CMBCSocket::SockCall_Accept( WPARAM wParam, LPARAM lParam )
{
	return S_OK;
}

HRESULT CMBCSocket::SockCall_Write( WPARAM wParam, LPARAM lParam )
{
	return S_OK;
}

HRESULT CMBCSocket::TxTimerCallbackProc( DWORD dwEvent, LPARAM lparam )
{
	if (dwEvent == IDTIMER_RECONN)
	{
		//try reconnect

		if (GetState() == MBCSOCKSTATE_OK)
		{
			ASSERT(FALSE);
			return S_OK;
		}

		TRACE("\nsock %x Reconnecting...", this);
		UnInit();
		HRESULT hr = Init();
		if (hr != S_OK)
		{
			ASSERT(FALSE);
			//net down, try again
		}

	}

	return S_OK;
}

HRESULT CMBCSocket::SockCall_Close( WPARAM wParam, LPARAM lParam )
{
	return S_OK;
}

CMBCSocket* CMBCSocket::AttachSock(SOCKET sockIn, int nFavMsgType, ISockMsgCallbackInterFace* pCallbackProc, int nFlag /*= 0*/ )
{
	char szCSBuff[256];
	CMBCSocket* pSock = new CMBCSocket;

	CSADDR_INFO* pcsInfo = (CSADDR_INFO*)szCSBuff;
	int noptLen = 256;
	HRESULT hRet = getsockopt(sockIn, SOL_SOCKET, SO_BSP_STATE, szCSBuff, &noptLen);
	if (hRet ==SOCKET_ERROR)
	{
		
	}
	else
	{
		ASSERT((*pcsInfo).LocalAddr.iSockaddrLength <= sizeof(SOCKADDR_IN));
		memcpy(&pSock->m_addrs.addrLocal, (*pcsInfo).LocalAddr.lpSockaddr, sizeof(SOCKADDR_IN));
		ASSERT((*pcsInfo).RemoteAddr.iSockaddrLength <= sizeof(SOCKADDR_IN));
		memcpy(&pSock->m_addrs.addrRemote, (*pcsInfo).RemoteAddr.lpSockaddr, sizeof(SOCKADDR_IN));
	}

	pSock->m_hSock = sockIn;
	pSock->m_nFavMsgType = nFavMsgType;
	pSock->m_pCallBack = pCallbackProc;
	pSock->m_nSelfState = MBCSOCKSTATE_OK;
	pSock->m_nCreateFlag = nFlag;

	TRACE("\n attach sock soc= %d, src= %s, des = %s ", sockIn, Addr2String(pSock->m_addrs.addrRemote).c_str(), Addr2String(pSock->m_addrs.addrLocal).c_str());
	HRESULT hr = E_FAIL;
	if (pSock->m_hSockWnd != NULL)
	{
		pSock->StartMsgLoopThd();
		hr = WSAAsyncSelect(pSock->m_hSock, pSock->m_hSockWnd, MSG_MBCNETSOCK, pSock->m_nFavMsgType);
		//start thread loop;
		pSock->m_nSelfState = MBCSOCKSTATE_OK;

	}
	if (hr != S_OK)
	{
		ASSERT(FALSE);
		CMBCSocket::ReleaseSock(pSock);
		pSock = NULL;
		pSock = NULL;
	}

	return pSock;
}

void CMBCSocket::SetState( ENUMMBCSOCKSTATE stateIn )
{
	if (m_nSelfState != stateIn)
	{
		m_nSelfState = stateIn;
		if (m_pCallBack)
		{
			m_pCallBack->SockStateChange(this, stateIn);
		}
	}
}


BOOL CMBCSocket::HasMsg()
{
#define MAX_SOCKMSG_BUFF 1000
	MBCSOCKMSG msgs[MAX_SOCKMSG_BUFF];
	while(TRUE)
	{
		DWORD hr = TxWaitObjWithQuit(m_hEventSockMsgArrival, m_hEventQuit, INFINITE);
		if (hr != WAIT_OBJECT_0)
		{
			//quit
			return TRUE;
		}
	
		int nLenOut = 0;
		CopyMsg(msgs, MAX_SOCKMSG_BUFF, nLenOut);
		for (size_t i = 0; i < nLenOut; ++i)
		{
			SockMsgCallback(msgs[i].wparam, msgs[i].lparam);
		}

	}
}

BOOL CMBCSocket::StartMsgLoopThd()
{
	StopMsgLoopThd();
	m_hSockLoopProc = CreateThread(NULL, 0, SockMsgLoopProc, (LPVOID)this, 0,0);

	return m_hSockLoopProc != NULL;
}

BOOL CMBCSocket::StopMsgLoopThd()
{
	if (m_hSockLoopProc)
	{
		SetEvent(m_hEventQuit);
		WaitForSingleObject(m_hSockLoopProc, INFINITE);
		CloseHandle(m_hSockLoopProc);
		m_hSockLoopProc = NULL;
	}
	ResetEvent(m_hEventQuit);
	//m_lockdeque.ResetLock();
	CAutoLock lock(&m_lockdeque);
	m_vecSockMsg.clear();
	return TRUE;
}

BOOL CMBCSocket::ReleaseSock( CMBCSocket* pSockIn )
{
	if (pSockIn && ::IsWindow(pSockIn->m_hSockWnd))
	{
		PostMessage(pSockIn->m_hSockWnd, WM_QUIT,0,0);
		return TRUE;
	}
	else
	{
		delete pSockIn;
		ASSERT(FALSE);
	}

	return FALSE;
}



