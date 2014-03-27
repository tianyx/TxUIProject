#include "StdAfx.h"
#include "MBCHeartBeatObj.h"
#include "MBCTransMsg.h"
#include "FGlobal.h"

CMBCHeartBeatObj::CMBCHeartBeatObj(void)
{
	INTERVAL_LIVECHECK = g_INTERVAL_LIVECHECK;
	DELAY_BEFORELIVECHECK = g_DELAY_BEFORELIVECHECK;
	m_nLiveRequestCount = 0;
	m_nLiveReplyCount = 0;
}

CMBCHeartBeatObj::~CMBCHeartBeatObj(void)
{
}

HRESULT CMBCHeartBeatObj::ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == msgtype_LIVEQA)
	{
		ST_TXMSG_LIVEQA msg;
		HRESULT hr =  UnPackMBCMsg(bufferIn, nUsed, msg);
		ASSERT(msg.nMsgState == msgState_A);
		if (hr == S_OK)
		{
			m_nLiveReplyCount = msg.nMsgId;
			//CFWriteLog("\n-receive heart = %s live pack reply counting = %d", this->m_strObjName, msg.nMsgId);
			EMB::CAutoLock lock(&m_lockLastInfo);
			m_LastReceivedInfo = msg;
		}
		else
		{
			ASSERT(FALSE);
		}
		return S_OK;
	}
	return CMBCBaseObj::ProcessIncomingMsg( pMBCSock,nMsgType, bufferIn, nUsed);

}


HRESULT CMBCHeartBeatObj::TxTimerCallbackProc( DWORD dwEvent, LPARAM lparam )
{
	if (dwEvent == IDTIMER_LIVECHECK)
	{
		if (m_pSockBase->GetState() != MBCSOCKSTATE_OK )
		{
			return E_FAIL;
		}
		ASSERT(m_nLiveRequestCount >= m_nLiveReplyCount);
		
		INT64 nDeta = m_nLiveRequestCount - m_nLiveReplyCount;
		if (nDeta > 5 )
		{
			//ASSERT(FALSE);
			CFWriteLog2Wnd(m_hLogWnd, TEXT("obj = %s not replied over 5 second, deta = %d "), this->m_strObjName, nDeta);
			m_LastReceivedInfo.nRemoteState = MBCSTATE_NOTRESPOND;
			ChangeSelfState(MBCSTATE_NOTRESPOND);
			//send msg to graph
		}
		else
		{
			ChangeSelfState(MBCSTATE_OK);
			
		}

		if (m_nLiveRequestCount > 0x7FFFFFFFFFFFFFF0)
		{
			//reset
			m_nLiveRequestCount = 0;
			m_nLiveReplyCount = 0;
		}
		//send live info request msg;
		ST_TXMSG_LIVEQA msg;
		msg.nMsgState = msgState_Q;
		msg.nMsgId = ++m_nLiveRequestCount;
		FillLivePack(msg);
		char buffer[SMALLBUFSIZE];
		int nUsed = 0;
		HRESULT hr = S_OK;
		PackMBCMsg(msg, buffer, SMALLBUFSIZE, nUsed);
		if (nUsed > 0 && m_pSockBase->IsOpen())
		{
			hr = send(*m_pSockBase, buffer, nUsed, 0);
			//_RPT3(0, "\n-send live pack to soc= %d,request counting = %d, replycounting = %d", m_pSockBase->m_hSock, m_nLiveRequestCount, m_nLiveReplyCount);
			if(hr == SOCKET_ERROR)
			{
				//ASSERT(FALSE);
				TRACE("send data err = %d",WSAGetLastError());
			}
		}
		return hr;
	}


	return CMBCBaseObj::TxTimerCallbackProc(dwEvent, lparam);
}

HRESULT CMBCHeartBeatObj::NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	//i am client
	ASSERT(socRecv == *pMBCSock);
	if (nErr ==S_OK)
	{	
		m_timerLive.SetTimer(IDTIMER_LIVECHECK, INTERVAL_LIVECHECK, this, (LPARAM)this, WT_EXECUTEDEFAULT, 1000);
		TRACE("\n Sock Connected");

	}
	return CMBCBaseObj::NetCall_Connect(pMBCSock, wParam, lParam);
}

HRESULT CMBCHeartBeatObj::NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	if (socRecv ==  *m_pSockBase)
	{
		ChangeSelfState(MBCSTATE_NETERR);
	}
	TRACE("\n Sock Closed");
	m_timerLive.KillTimer();

	return CMBCBaseObj::NetCall_Close(pMBCSock, wParam, lParam);
}

HRESULT CMBCHeartBeatObj::Run()
{
	HRESULT hr = S_OK;
	MCBSOCKADDRS addrs;
	addrs.addrLocal = m_addrLocal;
	addrs.addrRemote = m_addrRemote;
	m_pSockBase = CMBCSocket::CreateTCPSocket(addrs, FD_READ|FD_CLOSE|FD_CONNECT, this, MBCSOCKTYPE_TCP_CTRL|MBCSOCKTYPE_AUTORECONNECT);
	if (m_pSockBase == NULL)
	{
		ASSERT(FALSE);
		hr =  E_FAIL;
		return hr;
	}
	return CMBCBaseObj::Run();

}

HRESULT CMBCHeartBeatObj::Stop()
{
	m_timerLive.KillTimer();
	m_nLiveRequestCount = 0;
	m_nLiveReplyCount = 0;

	return CMBCBaseObj::Stop();
}

HRESULT CMBCHeartBeatObj::NetCall_Read( CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	return CMBCBaseObj::NetCall_Read(pMBCSock, wParam, lParam);
}

HRESULT CMBCHeartBeatObj::GetStateInfo( ST_OBJSTATEINFO& infoOut )
{
	EMB::CAutoLock lock(&m_lockLastInfo);
	infoOut.nSelfState = m_nSelfState;
	infoOut.nObjType = m_nObjType;
	infoOut.remoteInfo = m_LastReceivedInfo;
	infoOut.strName = m_strObjName;
	infoOut.addrLocal = m_addrLocal;
	infoOut.addrRemote = m_addrRemote;

	return S_OK;
}
