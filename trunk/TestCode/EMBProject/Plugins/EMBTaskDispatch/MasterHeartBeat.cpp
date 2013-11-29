#include "StdAfx.h"
#include "MasterHeartBeat.h"

CMasterHeartBeat::CMasterHeartBeat(void)
{
	m_pLastSock = NULL;
}

CMasterHeartBeat::~CMasterHeartBeat(void)
{
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

	// 判断是否为主备心跳消息
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

/*
*Description：填写心跳消息字符串
*Input Param：
*		msg ：消息通讯指针
*Return Param：返回成功或失败
*History：
*/
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

/*
*Description：更新远程任务管理状态
*Input Param：
*		msg ：远程服务端状态信息
*Return Param：返回成功或失败
*History：
*/
HRESULT CMasterHeartBeat::OnLiveMsgIn( ST_TXMSG_LIVEQA& msg )
{
	m_RemoteInfo.nsvrId = msg.nParam1;
	m_RemoteInfo.nMaster = msg.nParam2;
	m_RemoteInfo.nActive = msg.nParam3;
	return S_OK;
}

/*
*Description：获取远程任务管理状态
*Input Param：
*		infoOut ：远程服务端状态信息
*Return Param：返回成功或失败
*History：
*/
HRESULT CMasterHeartBeat::GetRemoteSvrState( ST_SVRLIVEINFO& infoOut )
{
	infoOut = m_RemoteInfo;
	infoOut.nConnState = embConnState_error;
	if (m_pLastSock && m_mapSockIns.find(m_pLastSock)== m_mapSockIns.end())
	{
		infoOut.nConnState =(m_pLastSock->GetState() == MBCSOCKSTATE_OK)? embConnState_ok:embConnState_error;
	}

	return S_OK;
}
