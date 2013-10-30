#include "StdAfx.h"
#include "MBCRemoteObj.h"
#include "FGlobal.h"


CMBCRemoteObj::CMBCRemoteObj(void)
{
	m_hLogWnd = NULL;
	m_pIRemoteCallInterface = NULL;
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
	MAPSOCKINS::iterator itb = m_mapSockIns.begin();
	MAPSOCKINS::iterator ite = m_mapSockIns.end();
	for (; itb != ite; ++itb)
	{
		delete (itb->first);
	}
	m_mapSockIns.clear();


	return __super::Stop();
}

HRESULT CMBCRemoteObj::ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	HRESULT hr = S_OK;
	int nLen = 0;

	if(m_pIRemoteCallInterface == NULL)
	{
		ASSERT(FALSE);
		return E_NOINTERFACE;
	}

	int nRetUsed = 0;
	char buffer[MAXRECVBUFF];

		if (nMsgType == msgtype_LIVEQA)
		{
			//extract msg
			//CFWriteLog("recv live Q");
			ST_TXMSG_LIVEQA msgIn;
			UnPackMBCMsg(bufferIn, nUsed, msgIn);
			//get live info
			ST_MBCCHANNELINFO_FORVC6 chInfos[50];
			int nChUsed = 0;
			m_pIRemoteCallInterface->GetLiveInfo(msgIn,msgIn.nRemoteState, msgIn.nSelfType, chInfos, 50, nChUsed);
			for (int i = 0; i < nChUsed; ++i)
			{
				ST_TXCHANNELINFO infotmp;
				CHInfoV62CHInfo(chInfos[i],infotmp);
				msgIn.vCHInfo.push_back(infotmp);
			}
			HRESULT hr = PackMBCMsg(msgIn,  buffer, MAXRECVBUFF, nRetUsed);


		}
		else if (nMsgType == msgtype_ACTQA)
		{
			//extract msg
			_RPT1(0, "--recved actQA use size = %d", nUsed);
			ST_TXMSG_ACTQA msgIn;
			UnPackMBCMsg(bufferIn, nUsed, msgIn);
			//do action
			m_pIRemoteCallInterface->DoAction(msgIn);

			HRESULT hr = PackMBCMsg(msgIn, buffer, MAXRECVBUFF, nRetUsed);
		}

	


	
	

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

	return hr;
}


HRESULT CMBCRemoteObj::NetCall_Connect( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	ASSERT(FALSE);
	return CMBCBaseObj::NetCall_Connect(pMBCSock, wParam, lParam);
}

HRESULT CMBCRemoteObj::NetCall_Close( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	//only process incoming sock
	MAPSOCKINS::iterator itf = m_mapSockIns.find(pMBCSock);
	if (itf != m_mapSockIns.end())
	{
		CMBCSocket* pSock = itf->first;
		if (pSock)
		{
			delete pSock;
		}
		m_mapSockIns.erase(itf);
		CFWriteLog("==remote closed, ip =%s",  Addr2String(pMBCSock->m_addrs.addrRemote).c_str());
	}


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
			m_mapSockIns[pSock] = pSock;
		}
		else
		{
			ASSERT(FALSE);
		}

		return S_OK;
}
