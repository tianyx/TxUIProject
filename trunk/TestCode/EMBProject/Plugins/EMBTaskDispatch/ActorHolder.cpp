#include "StdAfx.h"
#include "ActorHolder.h"
#include "FGlobal.h"
#include "EMBDefine.h"
#include "MBCTransMsg.h"
#include "EMBCommonFunc.h"

CActorHolder::CActorHolder(void)
{
	m_nObjType =MBCOBJTYPE_REMOTEHOST;
}

CActorHolder::~CActorHolder(void)
{
}

HRESULT CActorHolder::Run()
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

HRESULT CActorHolder::Stop()
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

HRESULT CActorHolder::NetCall_Connect( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	ASSERT(FALSE);
	return CMBCBaseObj::NetCall_Connect(pMBCSock, wParam, lParam);

}

HRESULT CActorHolder::NetCall_Close( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	//only process incoming sock
	RemoveSock(pMBCSock);
	return CMBCBaseObj::NetCall_Close(pMBCSock, wParam, lParam);
}

HRESULT CActorHolder::NetCall_Accept( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
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
		CFWriteLog(TEXT("==remote accepted, soc = %d, ip =%s"), socRecv, Addr2String(addrDes).c_str());
		AddSock(pSock);
	}
	else
	{
		ASSERT(FALSE);
	}

	return S_OK;
}

HRESULT CActorHolder::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	HRESULT hr = S_OK;
	int nLen = 0;
	
	int nRetUsed = 0;
	if (nMsgType == embmsgtype_ReportGuid)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (!msgIn.strGuid.IsEmpty())
		{
			GUID strGuidNew = String2Guid(msgIn.strGuid);
			ASSERT(strGuidNew != GUID_NULL);
			//find sock
			{
				CAutoLock lock(&m_locSockMap);
				MAPSOCKINS::iterator itf = m_mapSockIns.find(pMBCSock);
				if (itf != m_mapSockIns.end())
				{
					ASSERT(itf->second.strGuid == GUID_NULL);
					itf->second.strGuid = strGuidNew;
				}
				
			}
			if (m_pActorCallbackInterface)
			{
				m_pActorCallbackInterface->OnActorConnect(strGuidNew);
			}
		}
	}
	if (nMsgType == embmsgtype_ReportActorInfo
		||nMsgType == embmsgtype_TaskDispath)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if(msgIn.nMsgState != embmsgstate_A)
		{
			ASSERT(FALSE);
			return S_FALSE;
		}
		if (m_pActorCallbackInterface)
		{
			nMsgType == embmsgtype_ReportActorInfo?
				m_pActorCallbackInterface->OnActorReportInfo(GetSockGuid(pMBCSock), msgIn.strData):
				m_pActorCallbackInterface->OnActorDispatchTask(GetSockGuid(pMBCSock), msgIn.strData);
		}
	}
	
	return hr;
}

BOOL CActorHolder::SetActorCallbackInterface( IEMBActorHolderCallBackInterface* pCallIn )
{
	m_pActorCallbackInterface = pCallIn;
	return TRUE;
}

void CActorHolder::RemoveSock( CMBCSocket* pSock )
{
	CAutoLock lock(&m_locSockMap);
	MAPSOCKINS::iterator itf = m_mapSockIns.find(pSock);
	if (itf != m_mapSockIns.end())
	{
		if (itf->second.nActorConnState == ActorConnState_ok
			&& m_pActorCallbackInterface)
		{
			m_pActorCallbackInterface->OnActorDisConnect(itf->second.strGuid);
		}
		m_mapSockIns.erase(itf);
		CFWriteLog(TEXT("==remote closed, ip =%s"),  Addr2String(pSock->m_addrs.addrRemote).c_str());
		delete pSock;
	}
}

void CActorHolder::AddSock( CMBCSocket* pSock )
{
	{
		CAutoLock lock(&m_locSockMap);
		ST_ACTORDATA actData;
		actData.nActorConnState = ActorConnState_conn;
		actData.pSock = pSock;
		m_mapSockIns[pSock] = actData;
	}
	//send request info to pSock
	ST_EMBTRANSMSG msg(embmsgtype_ReportActorInfo);

	msg.nMsgState = embmsgstate_Q;

	int nRetUsed = 0;
	char buffer[MAXRECVBUFF];
	HRESULT hr = PackMBCMsg(msg, buffer, MAXRECVBUFF, nRetUsed);
	DoSockSend(pSock, buffer, nRetUsed);
	
}

HRESULT CActorHolder::DoSockSend( CMBCSocket* pSock, const char* pbufferIn, const int nSizeIn )
{
	HRESULT hr = S_OK;
	//send info back
	if (nSizeIn > 0)
	{
		hr = send(*pSock, pbufferIn, nSizeIn, 0);
		//CFWriteLog2Wnd(m_hLogWnd, TEXT("=send back to soc %d"), pMBCSock->m_hSock);
		if(hr == SOCKET_ERROR)
		{
			ASSERT(FALSE);
			hr = WSAGetLastError();
			RemoveSock(pSock);
		}
	}

	return hr > 0? S_OK:hr;
}

GUID CActorHolder::GetSockGuid( CMBCSocket* pSock )
{
	CAutoLock loc(&m_locSockMap);
	MAPSOCKINS::iterator itf = m_mapSockIns.find(pSock);
	if (itf != m_mapSockIns.end())
	{
		return itf->second.strGuid;
	}
	else
	{
		ASSERT(FALSE);
		return GUID_NULL;
	}
}

HRESULT CActorHolder::SendToActor(const GUID& szActorGuid, CString& szMsg )
{
	 return S_OK;
}
