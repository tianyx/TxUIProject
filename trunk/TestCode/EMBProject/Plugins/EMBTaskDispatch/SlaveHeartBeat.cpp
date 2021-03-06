#include "StdAfx.h"
#include "SlaveHeartBeat.h"
#include "MBCTransMsg.h"

CSlaveHeartBeat::CSlaveHeartBeat(void)
{
	m_pSvrLiveCallback = NULL;
}

CSlaveHeartBeat::~CSlaveHeartBeat(void)
{
}

/*
*Description：更新远程任务管理状态
*Input Param：
*		msg ：远程服务端状态信息
*Return Param：返回成功或失败
*History：
*/
HRESULT CSlaveHeartBeat::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == msgtype_LIVEQA)
	{
		ST_TXMSG_LIVEQA msg;
		HRESULT hr =  UnPackMBCMsg(bufferIn, nUsed, msg);
		ASSERT(msg.nMsgState == msgState_A);
		if (hr == S_OK)
		{
			OnLiveMsgIn(msg);
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

/*
*Description：填写心跳消息字符串
*Input Param：
*		msg ：消息通讯指针
*Return Param：返回成功或失败
*History：
*/
HRESULT CSlaveHeartBeat::FillLivePack( ST_TXMSG_LIVEQA& msg )
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
HRESULT CSlaveHeartBeat::OnLiveMsgIn( ST_TXMSG_LIVEQA& msg )
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
HRESULT CSlaveHeartBeat::GetRemoteSvrState( ST_SVRLIVEINFO& infoOut )
{
	infoOut = m_RemoteInfo;
	infoOut.nConnState = embConnState_error;
	if (m_pSockBase)
	{
		infoOut.nConnState =(GetState() == MBCSTATE_OK)? embConnState_ok:embConnState_error;
	}

	return S_OK;
}
