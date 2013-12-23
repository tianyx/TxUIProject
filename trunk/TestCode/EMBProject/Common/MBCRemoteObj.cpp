#include "StdAfx.h"
#include "MBCRemoteObj.h"
#include "FGlobal.h"


CMBCRemoteObj::CMBCRemoteObj(void)
{
	m_hLogWnd = NULL;
	m_nObjType =MBCOBJTYPE_REMOTEHOST;
}

CMBCRemoteObj::~CMBCRemoteObj(void)
{
}

HRESULT CMBCRemoteObj::Run()
{
	Stop();
	
	HRESULT hr = S_OK;
	MCBSOCKADDRS addrs;
	addrs.addrLocal = m_addrLocal;
	addrs.addrRemote = m_addrRemote;
	m_pSockBase = CMBCSocket::CreateTCPSocket(addrs, FD_ACCEPT, this, MBCSOCKTYPE_TCP_LISTENER|MBCSOCKTYPE_AUTORECONNECT);
	if (m_pSockBase == NULL)
	{
		ASSERT(FALSE);
		hr =  E_FAIL;
		return hr;
	}

	return hr;
}


HRESULT CMBCRemoteObj::Stop()
{
	{//for release lock
		CAutoLock lock(&m_csSockIn);
		MAPSOCKINS::iterator itb = m_mapSockIns.begin();
		MAPSOCKINS::iterator ite = m_mapSockIns.end();
		for (; itb != ite; ++itb)
		{
			CMBCSocket::ReleaseSock(itb->first);
		}
		m_mapSockIns.clear();
	}

	return __super::Stop();
}

HRESULT CMBCRemoteObj::ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	HRESULT hr = S_OK;
	int nLen = 0;

	int nRetUsed = 0;
	char buffer[SMALLBUFSIZE];

	if (nMsgType == msgtype_LIVEQA)
	{
		//extract msg
		//CFWriteLog("recv live Q");
		ST_TXMSG_LIVEQA msgIn;
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		//get live info
		msgIn.nMsgState = msgState_A;
		HRESULT hr = PackMBCMsg(msgIn,  buffer, SMALLBUFSIZE, nRetUsed);
		//send info back
		if (nRetUsed > 0)
		{
			hr = send(*pMBCSock, buffer, nRetUsed, 0);
			//CFWriteLog2Wnd(m_hLogWnd, TEXT("=send back to soc %d"), pMBCSock->m_hSock);
			if(hr == SOCKET_ERROR)
			{
				ASSERT(FALSE);
				hr = WSAGetLastError();
			}
		}
	}

	return S_OK;
}


HRESULT CMBCRemoteObj::NetCall_Connect( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	ASSERT(FALSE);
	return CMBCBaseObj::NetCall_Connect(pMBCSock, wParam, lParam);
}

HRESULT CMBCRemoteObj::NetCall_Close( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	//only process incoming sock
	RemoveSock(pMBCSock);


	return CMBCBaseObj::NetCall_Close(pMBCSock, wParam, lParam);
}

HRESULT CMBCRemoteObj::NetCall_Accept( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;

		ASSERT(socRecv == *pMBCSock);
		SOCKADDR_IN addrDes;
		int naddrLen = sizeof(SOCKADDR_IN);
		SOCKET socIn = accept(*m_pSockBase, (sockaddr*)&addrDes, &naddrLen);
		CMBCSocket* pSock = CMBCSocket::AttachSock(socIn, FD_READ|FD_CLOSE, this, MBCSOCKTYPE_TCP_CTRL);
		if (pSock)
		{
			CFWriteLog("==remote accepted, soc = %d, ip =%s", socRecv, Addr2String(addrDes).c_str());
			AddSock(pSock);
		}
		else
		{
			ASSERT(FALSE);
		}

		return S_OK;
}


void CMBCRemoteObj::RemoveSock( CMBCSocket* pSock )
{
	CMBCSocket* pSockFound = NULL;
	{
		CAutoLock lock(&m_csSockIn);
		MAPSOCKINS::iterator itf = m_mapSockIns.find(pSock);
		if (itf != m_mapSockIns.end())
		{
			pSockFound = pSock;
			m_mapSockIns.erase(itf);
		}
	}

	if (pSockFound)
	{
		CFWriteLog("==remote closed, ip =%s",  Addr2String(pSock->m_addrs.addrRemote).c_str());
		CMBCSocket::ReleaseSock(pSockFound);
	}

}

void CMBCRemoteObj::AddSock( CMBCSocket* pSock )
{
	CAutoLock lock(&m_csSockIn);
	m_mapSockIns[pSock] = pSock;
}
