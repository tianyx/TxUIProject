/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	MasterHeartBeat.h
	author:		tianyx
	
	purpose:	主心跳检测
*********************************************************************/
#pragma once
#include "MBCRemoteObj.h"
#include "EmbStructDef.h"
class CMasterHeartBeat :
	public CMBCRemoteObj, public IRemoteSvrLiveInterface
{
public:
	CMasterHeartBeat(void);
	~CMasterHeartBeat(void);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);
	BOOL SetLiveCallback(IServerLiveInterface* pCallbackIn){m_pSvrLiveCallback = pCallbackIn;}

	//for IRemoteSvrLiveInterface
	virtual HRESULT GetRemoteSvrState(ST_SVRLIVEINFO& infoOut);

	virtual HRESULT FillLivePack(ST_TXMSG_LIVEQA& msg);
	virtual HRESULT OnLiveMsgIn(ST_TXMSG_LIVEQA& msg);

	// 连接状态回调函数指针
	IServerLiveInterface* m_pSvrLiveCallback;
	// 远程连接任务管理祥光信息
	ST_SVRLIVEINFO m_RemoteInfo;

	CMBCSocket* m_pLastSock;
};
