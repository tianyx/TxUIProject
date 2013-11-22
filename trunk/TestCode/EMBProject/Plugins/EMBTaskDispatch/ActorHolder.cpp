#include "StdAfx.h"
#include "ActorHolder.h"
#include "FGlobal.h"
#include "EMBDefine.h"
#include "MBCTransMsg.h"
#include "EMBCommonFunc.h"
#include "EMBMessageDef.h"
CActorHolder::CActorHolder(void)
{
	m_nObjType =MBCOBJTYPE_REMOTEHOST;
	m_nSvrActive = embSvrState_deactive;
	m_nMaster = embSvrType_master;
	m_pActorCallbackInterface = NULL;
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

HRESULT CActorHolder::Stop()
{
	
	CAutoLock lock(&m_csSockMap);

	MAPSOCKACTORS::iterator itb = m_mapSockIns.begin();
	MAPSOCKACTORS::iterator ite = m_mapSockIns.end();
	for (; itb != ite; ++itb)
	{
		delete (itb->first);
	}
	m_mapSockIns.clear();
	m_mapActorMirrs.clear();

	return  __super::Stop();
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
	if (nMsgType == embmsgtype_ActorReportGuid)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (!msgIn.strData.IsEmpty())
		{
			ACTORID actoridNew = atoi(msgIn.strData);
			ASSERT(actoridNew != INVALID_ID);
			//find sock
			{
				BOOL bFind = TRUE;
				{
					CAutoLock lock(&m_csSockMap);
					MAPSOCKACTORS::iterator itf = m_mapSockIns.find(pMBCSock);
					if (itf != m_mapSockIns.end())
					{
						ASSERT(itf->second.strGuid == INVALID_ID);
						itf->second.strGuid = actoridNew;
						m_mapActorMirrs[actoridNew] = pMBCSock;
					}
					else
					{
						ASSERT(FALSE);
						//sock lost
						bFind = FALSE;
					}
				}

				if (!bFind)
				{
					AddSock(pMBCSock);

				}
				
				

				//send svr activestate to sock
				CFWriteLog(0, TEXT("send active state to actor %d"), actoridNew);
				ST_SVRACTIVEINFO activeInfo;
				activeInfo.nActive = m_nSvrActive;
				activeInfo.nMaster = m_nMaster;
				ST_EMBTRANSMSG msg(embmsgtype_DispatchToActorMsg);
				activeInfo.ToString(msg.strData);
				CEMBAutoBuffer szbuff(msg);
				int nUsed = 0;
				PackMBCMsg(msg, szbuff, szbuff.GetSize(), nUsed);
				hr = send(*pMBCSock, szbuff, nUsed, 0);
				if (hr == SOCKET_ERROR)
				{
					hr = WSAGetLastError();
					ASSERT(FALSE);
				}
				
				
			}
			if (m_pActorCallbackInterface)
			{
				m_pActorCallbackInterface->OnActorConnect(actoridNew);
			}
		}
	}
	else if (nMsgType == embmsgtype_ActorToDispatchMsg)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (m_pActorCallbackInterface)
		{
			CString strRet;
			m_pActorCallbackInterface->OnActorMessage(GetSockGuid(pMBCSock), msgIn.strData, strRet);
			if (!strRet.IsEmpty())
			{
				//send back
				ST_EMBTRANSMSG msgRet(embmsgtype_DispatchToActorMsg);
				msgRet.strData = strRet;
				CEMBAutoBuffer szbuff(msgRet);
				int nUsed = 0;
				PackMBCMsg(msgRet, szbuff, szbuff.GetSize(), nUsed);
				send(*pMBCSock, szbuff, nUsed, 0);
			}
		}
	}
	else if (nMsgType == msgtype_LIVEQA)
	{
		//extract msg
		//CFWriteLog("recv live Q");
		ST_TXMSG_LIVEQA msgIn;
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		msgIn.nMsgState = msgState_A;
		//get live info
		char buffer[256];
		HRESULT hr = PackMBCMsg(msgIn,  buffer, MAXRECVBUFF, nRetUsed);
		if (nRetUsed > 0)
		{
			hr = send(*pMBCSock, buffer, nRetUsed, 0);
		}
		else 
		{
			ASSERT(FALSE);
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
	CAutoLock lock(&m_csSockMap);
	MAPSOCKACTORS::iterator itf = m_mapSockIns.find(pSock);
	if (itf != m_mapSockIns.end())
	{
		if ( m_pActorCallbackInterface)
		{
			m_pActorCallbackInterface->OnActorDisConnect(itf->second.strGuid);
		}
		m_mapActorMirrs.erase(itf->second.strGuid);
		m_mapSockIns.erase(itf);
		CFWriteLog(TEXT("==remote closed, ip =%s"),  Addr2String(pSock->m_addrs.addrRemote).c_str());
		CMBCSocket::ReleaseSock(pSock);
		pSock = NULL;
	}
}

void CActorHolder::AddSock( CMBCSocket* pSock )
{
	{
		CAutoLock lock(&m_csSockMap);
		ST_ACTORDATA actData;
		actData.nActorConnState = embActorConnState_conn;
		actData.pSock = pSock;
		m_mapSockIns[pSock] = actData;
	}
	//send request info to pSock
	CFWriteLog(0, TEXT("request actor guid"));
	ST_EMBTRANSMSG msg(embmsgtype_ActorReportGuid);

	msg.nMsgState = embmsgstate_Q;

	int nRetUsed = 0;
	CEMBAutoBuffer buffer(msg);
	HRESULT hr = PackMBCMsg(msg, buffer, buffer.GetSize(), nRetUsed);
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

ACTORID CActorHolder::GetSockGuid( CMBCSocket* pSock )
{
	CAutoLock loc(&m_csSockMap);
	MAPSOCKACTORS::iterator itf = m_mapSockIns.find(pSock);
	if (itf != m_mapSockIns.end())
	{
		return itf->second.strGuid;
	}
	else
	{
		ASSERT(FALSE);
		return INVALID_ID;
	}
}

HRESULT CActorHolder::SendToActor(const ACTORID actorId, CString& szMsg )
{
	HRESULT hr = S_OK;
	CMBCSocket* pSock = NULL;
	SOCKET sock = INVALID_SOCKET;
	{//to auto release lock


		CAutoLock loc(&m_csSockMap);
		MAPSOCKACTORS::iterator itb = m_mapSockIns.begin();
		MAPSOCKACTORS::iterator ite = m_mapSockIns.end();
		for (; itb != ite; ++itb)
		{
			if (itb->second.strGuid == actorId)
			{
				pSock = itb->first;
				break;
			}
		}

		if (pSock)
		{
			sock = *pSock;
		}

	}

	if (sock != INVALID_SOCKET)
	{
		ST_EMBTRANSMSG msg(embmsgtype_DispatchToActorMsg);
		msg.nMsgState = msgState_Q;
		msg.strData = szMsg;
		CEMBAutoBuffer szbuff(msg);
		int nUsed = 0;
		PackMBCMsg(msg, szbuff, szbuff.GetSize(), nUsed);
		hr = send(sock, szbuff, nUsed, 0);
		//CFWriteLog2Wnd(m_hLogWnd, TEXT("=send back to soc %d"), pMBCSock->m_hSock);
		if(hr == SOCKET_ERROR)
		{
			ASSERT(FALSE);
			hr = WSAGetLastError();
			RemoveSock(pSock);
		}
		else
		{
			hr = S_OK;
		}
	}
	else
	{
		hr = EMBERR_INVALIDARG;
	}

	return hr;
}

BOOL CActorHolder::HasActor( const ACTORID actorId )
{
	CAutoLock lock(&m_csSockMap);
	MAPACTORSOCKS::iterator itf = m_mapActorMirrs.find(actorId);
	return itf != m_mapActorMirrs.end();

}

HRESULT CActorHolder::SetSvrState( int nStateIn, int nMaster)
{
	//broadcast to all actors whatever
	m_nMaster = nMaster;
	m_nSvrActive = nStateIn;
	ST_SVRACTIVEINFO info;
	info.nActive= nStateIn;
	info.nMaster = nMaster;
	CString strInfo;
	info.ToString(strInfo);
	BroadcastToActor(strInfo);
	return S_OK;
}

HRESULT CActorHolder::BroadcastToActor( CString& szMsg )
{
	vector<ST_SOCKMBCSOCK> vSocks;
	{
		CAutoLock lock(&m_csSockMap);
		MAPSOCKACTORS::iterator itb = m_mapSockIns.begin();
		MAPSOCKACTORS::iterator ite = m_mapSockIns.end();
		for (; itb != ite; ++itb)
		{
			ST_SOCKMBCSOCK tmp;
			tmp.sock= *(itb->first);
			tmp.pSock = itb->first;
			vSocks.push_back(tmp);
		}
	}
	
	ST_EMBTRANSMSG msg(embmsgtype_DispatchToActorMsg);
	msg.nMsgState = msgState_Q;
	msg.strData = szMsg;
	CEMBAutoBuffer szbuff(msg);
	int nUsed = 0;
	PackMBCMsg(msg, szbuff, szbuff.GetSize(), nUsed);

	for (size_t i = 0; i < vSocks.size(); ++i)
	{
		HRESULT hr = S_OK;
		
		if (vSocks[i].sock != INVALID_SOCKET)
		{
			hr = send(vSocks[i].sock, szbuff, nUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				ASSERT(FALSE);
				hr = WSAGetLastError();
				if (hr == WSAEMSGSIZE)
				{
					ASSERT(FALSE);
					_RPT0(0, "\nmessage is too larage!");
				}
				else
				{
					RemoveSock(vSocks[i].pSock);
				}
			}
		}
		
	}

	return S_OK;
}
