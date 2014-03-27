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

/*
*Description：启动TCP SERVER服务，等待执行端连接
*Input Param：
*Return Param：返回成功或失败
*History：
*/
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

/*
*Description：停止TCP SERVER服务，断开所有连接
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CActorHolder::Stop()
{
	
	EMB::CAutoLock lock(&m_csSockMap);

	MAPSOCKACTORS::iterator itb = m_mapSockIns.begin();
	MAPSOCKACTORS::iterator ite = m_mapSockIns.end();
	for (; itb != ite; ++itb)
	{
		CMBCSocket::ReleaseSock(itb->first);
	}
	m_mapSockIns.clear();
	m_mapActorMirrs.clear();

	return  __super::Stop();
}

// 连接方会收到的返回消息
HRESULT CActorHolder::NetCall_Connect( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	ASSERT(FALSE);
	return CMBCBaseObj::NetCall_Connect(pMBCSock, wParam, lParam);

}
// 连接方会收到的返回消息
HRESULT CActorHolder::NetCall_Close( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	//only process incoming sock
	RemoveSock(pMBCSock);
	return CMBCBaseObj::NetCall_Close(pMBCSock, wParam, lParam);
}

// ActorHolder当有Actor连接传入时，会收到此消息
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

/*
*Description：接收消息函数
*Input Param：
*		pMBCSock ：消息通讯指针
*	    nMsgType ：消息类型
*       bufferIn ：消息指针
*       nUsed    ：消息长度
*Return Param：返回成功或失败
*History：
*/
HRESULT CActorHolder::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	HRESULT hr = S_OK;
	int nLen = 0;
	
	int nRetUsed = 0;
	if (nMsgType == embmsgtype_ActorReportGuid)
	{
		// 为ACTOR广播消息
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (!msgIn.strData.IsEmpty())
		{
			ACTORID actoridNew = atoi(msgIn.strData);
			ASSERT(actoridNew != INVALID_ID);
			//find sock
			//如果是已经加入的ACTOR，则不处理，否则加入到ACTOR连接中
			{
				BOOL bFind = TRUE;
				{
					EMB::CAutoLock lock(&m_csSockMap);
					MAPSOCKACTORS::iterator itf = m_mapSockIns.find(pMBCSock);
					if (itf != m_mapSockIns.end())
					{
						ASSERT(itf->second.strGuid == INVALID_ID);
						itf->second.strGuid = actoridNew;
						itf->second.strPcName = msgIn.strPcName;
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
				m_pActorCallbackInterface->OnActorConnect(actoridNew, msgIn.strPcName);
			}
		}
	}
	else if (nMsgType == embmsgtype_ActorToDispatchMsg)
	{
		//如果为ACTOR返回的分配任务消息？
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
		char buffer[SMALLBUFSIZE];
		HRESULT hr = PackMBCMsg(msgIn,  buffer, SMALLBUFSIZE, nRetUsed);
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

/*
*Description：设置回调函数指针
*Input Param：
*		pCallIn ：回调函数指针
*Return Param：返回成功或失败
*History：
*/
BOOL CActorHolder::SetActorCallbackInterface( IEMBActorHolderCallBackInterface* pCallIn )
{
	m_pActorCallbackInterface = pCallIn;
	return TRUE;
}

/*
*Description：移除连接
*Input Param：
*		pSock ：SOCKET连接指针
*Return Param：
*History：
*/
void CActorHolder::RemoveSock( CMBCSocket* pSock )
{
	EMB::CAutoLock lock(&m_csSockMap);
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

/*
*Description：添加任务
*Input Param：
*		pSock ：添加的SOCEKT指针
*Return Param：
*History：
*/
void CActorHolder::AddSock( CMBCSocket* pSock )
{
	{
		EMB::CAutoLock lock(&m_csSockMap);
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

/*
*Description：发送SOCEKT消息
*Input Param：
*		pSock ：    发送的SOCEKT指针
*       pbufferIn ：传送字符串头指针
*       nSizeIn：   传送字符串长度
*Return Param：
*History：
*/
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

/*
*Description：通过连接指针获取ACTORID
*Input Param：
*		pSock ：    SOCEKT指针
*Return Param：
		ACTORID ：返回ACTORID
*History：
*/
ACTORID CActorHolder::GetSockGuid( CMBCSocket* pSock )
{
	EMB::CAutoLock loc(&m_csSockMap);
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

/*
*Description：根据ACTORID发送消息给ACTOR
*Input Param：
*		actorId ：执行端ID
*       szMsg   ：消息内容
*Return Param：返回成功或失败
*History：
*/
HRESULT CActorHolder::SendToActor(const ACTORID actorId, CString& szMsg )
{
	HRESULT hr = S_OK;
	CMBCSocket* pSock = NULL;
	SOCKET sock = INVALID_SOCKET;
	{//to auto release lock
		//通过ID寻找pSock指针

		EMB::CAutoLock loc(&m_csSockMap);
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
		//发送消息
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

/*
*Description：ACTOR连接中是否有此ACTORID连接
*Input Param：  
*		actorId ：执行端ID
*Return Param：有返回TRUE，没有返回FALSE
*History：
*/
BOOL CActorHolder::HasActor( const ACTORID actorId )
{
	EMB::CAutoLock lock(&m_csSockMap);
	MAPACTORSOCKS::iterator itf = m_mapActorMirrs.find(actorId);
	return itf != m_mapActorMirrs.end();

}

/*
*Description：设置任务管理状态
*Input Param：  
*		nStateIn ：当前状态
*       nMaster：默认本身逻辑是主还是备
*Return Param：有返回TRUE，没有返回FALSE
*History：
*/
HRESULT CActorHolder::SetSvrState( int nStateIn, int nMaster)
{
	//broadcast to all actors whatever？
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

/*
*Description：定向广播，向所有ACTOR发送相同消息
*Input Param：  
*		szMsg ：消息内容
*Return Param：返回状态
*History：
*/
HRESULT CActorHolder::BroadcastToActor( CString& szMsg )
{
	vector<ST_SOCKMBCSOCK> vSocks;
	{
		EMB::CAutoLock lock(&m_csSockMap);
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

