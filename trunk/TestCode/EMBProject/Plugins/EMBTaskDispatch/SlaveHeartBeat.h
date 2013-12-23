/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	SlaveHeartBeat.h
	author:		tianyx
	
	purpose:	���������
*********************************************************************/
#pragma once
#include "MBCHeartBeatObj.h"
#include "EmbStructDef.h"
class CSlaveHeartBeat :
	public CMBCHeartBeatObj,
	public IRemoteSvrLiveInterface
{
public:
	CSlaveHeartBeat(void);
	virtual ~CSlaveHeartBeat(void);
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed );
	BOOL SetLiveCallback(IServerLiveInterface* pCallbackIn){m_pSvrLiveCallback = pCallbackIn; return TRUE;}

	virtual HRESULT FillLivePack(ST_TXMSG_LIVEQA& msg);

	HRESULT OnLiveMsgIn( ST_TXMSG_LIVEQA& msg );
	//for IRemoteSvrLiveInterface
	virtual HRESULT GetRemoteSvrState(ST_SVRLIVEINFO& infoOut);

	// �ص�����ָ��
	IServerLiveInterface* m_pSvrLiveCallback;
	// Զ�̷����״̬��Ϣ
	ST_SVRLIVEINFO m_RemoteInfo;
};
