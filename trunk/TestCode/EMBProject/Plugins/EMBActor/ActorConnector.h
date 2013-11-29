/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	ActorConnector.h
	author:		tianyx
	
	purpose:	负责client.exe与Actor.exe连接
*********************************************************************/
#pragma once
#include "MBCHeartBeatObj.h"
#include "IEMBBaseInterface.h"
#include "EMBDefine.h"

// 向Actor.exe发送消息: 提交任务
interface IActorConnectorCallback
{
	virtual HRESULT OnActorConnectorMsg(CString& strInfo, CString& strRet) = 0;
};

class CActorConnector: public CMBCHeartBeatObj
{
public:
	CActorConnector(void);
	virtual ~CActorConnector(void);

	/*
    Description：消息处理
	Input：		pMBCSock tcp连接; nMsgType 消息类型
	Return:		S_OK 成功处理消息
	History：
	*/
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);
	virtual HRESULT SendtoDispatcher(CString& strInfo);
	void SetActorId(ACTORID actoridIn){m_nActorId = actoridIn;}
	void SetActorConnectorCallback(IActorConnectorCallback* pCallback){m_pITaskCommit = pCallback;}
	IActorConnectorCallback* m_pITaskCommit;
	ACTORID m_nActorId;
};
