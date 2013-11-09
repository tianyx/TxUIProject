/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	ActorConnector.h
	author:		tianyx
	
	purpose:	actor connector
*********************************************************************/
#pragma once
#include "MBCHeartBeatObj.h"
#include "IEMBBaseInterface.h"
#include "EMBDefine.h"
interface IActorConnectorCallback
{
	virtual HRESULT OnActorConnectorMsg(ST_EMBTRANSMSG& pMsg) = 0;
};
class CActorConnector: public CMBCHeartBeatObj
{
public:
	CActorConnector(void);
	virtual ~CActorConnector(void);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

	IActorConnectorCallback* m_pITaskCommit;

};
