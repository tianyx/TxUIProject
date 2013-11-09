#include "StdAfx.h"
#include "MasterHeartBeat.h"

CMasterHeartBeat::CMasterHeartBeat(void)
{
	m_pLastSock = NULL;
}

CMasterHeartBeat::~CMasterHeartBeat(void)
{
}

HRESULT CMasterHeartBeat::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
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
		m_pLastSock = pMBCSock;
		//extract msg
		//CFWriteLog("recv live Q");
		ST_TXMSG_LIVEQA msgIn;
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		OnLiveMsgIn(msgIn);
		//get live info
		FillLivePack(msgIn);
		int nChUsed = 0;
		HRESULT hr = PackMBCMsg(msgIn,  buffer, MAXRECVBUFF, nRetUsed);


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

HRESULT CMasterHeartBeat::FillLivePack( ST_TXMSG_LIVEQA& msg )
{
	if (m_pSvrLiveCallback)
	{
		ST_SVRLIVEINFO info;
		m_pSvrLiveCallback->GetSelfState(info);
		msg.nParam1 = info.nsvrId;
		msg.nParam2 = info.nMaster;
		msg.nParam3 = info.nActive;
		return S_OK;
	}
	else
	{
		ASSERT(FALSE);
		return E_FAIL;
	}
}

HRESULT CMasterHeartBeat::OnLiveMsgIn( ST_TXMSG_LIVEQA& msg )
{
	m_RemoteInfo.nsvrId = msg.nParam1;
	m_RemoteInfo.nMaster = msg.nParam2;
	m_RemoteInfo.nActive = msg.nParam3;
	return S_OK;
}

HRESULT CMasterHeartBeat::GetRemoteSvrState( ST_SVRLIVEINFO& infoOut )
{
	infoOut = m_RemoteInfo;
	infoOut.nConnState = embConnState_fail;
	if (m_pLastSock && m_mapSockIns.find(m_pLastSock)== m_mapSockIns.end())
	{
		infoOut.nConnState =(m_pLastSock->GetState() == MBCSOCKSTATE_OK)? embConnState_ok:embConnState_fail;
	}

	return S_OK;
}
