/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	ActorHolder.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "MBCBaseObj.h"
#include "TxParamString.h"
#include "AutoCritSec.h"
#include "EMBDefine.h"
using namespace std;



interface IEMBActorHolderCallBackInterface
{
	virtual HRESULT OnActorConnect(const ACTORID& szActorGuid) = 0;
	virtual HRESULT OnActorDisConnect(const ACTORID& szActorGuid) = 0;
	virtual HRESULT OnActorReportInfo(const ACTORID& szActorGuid, CString& szActorInfoIn) = 0;
	virtual HRESULT OnActorDispatchTask(const ACTORID& szActorGuid, CString& szActorInfoIn) = 0;

};

interface IEMBActorSenderInterface
{
	virtual HRESULT SendToActor(const GUID& szActorGuid, CString& szMsg) = 0;
};

struct ST_ACTORDATA
{
	ACTORID strGuid;
	int  nActorConnState;
	CMBCSocket* pSock;

	ST_ACTORDATA()
	{
		pSock = NULL;
		strGuid = INVALID_ID;
		nActorConnState = ActorConnState_none;
	}
};

typedef map<CMBCSocket*, ST_ACTORDATA> MAPSOCKINS;

class CActorHolder :
	public CMBCBaseObj,
	public IEMBActorSenderInterface
{
public:
	CActorHolder(void);
	virtual ~CActorHolder(void);


	virtual HRESULT Run();
	virtual HRESULT Stop();

	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

	//for IEMBActorSenderInterface
	virtual HRESULT SendToActor(const GUID& szActorGuid, CString& szMsg);

	//
	BOOL SetActorCallbackInterface(IEMBActorHolderCallBackInterface* pCallIn);

private:
	//
	HRESULT DoSockSend(CMBCSocket* pSock, const char* pbufferIn, const int nSizeIn);
	void RemoveSock(CMBCSocket* pSock);
	void AddSock(CMBCSocket* pSock);
	ACTORID GetSockGuid(CMBCSocket* pSock);
	IEMBActorHolderCallBackInterface* m_pActorCallbackInterface;
	MAPSOCKINS m_mapSockIns;
	CAutoCritSec m_locSockMap;
};
