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
	virtual HRESULT OnActorMessage(const ACTORID& szActorGuid, CString& szActorInfoIn,  CString& szRet) = 0;
};

interface IEMBActorSenderInterface
{
	virtual HRESULT SendToActor(const ACTORID szActorGuid, CString& szMsg) = 0;
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
		nActorConnState = embActorConnState_none;
	}
};

typedef map<CMBCSocket*, ST_ACTORDATA> MAPSOCKACTORS;
typedef map<ACTORID, CMBCSocket*> MAPACTORSOCKS;

struct ST_SOCKMBCSOCK
{
	SOCKET sock;
	CMBCSocket* pSock;
};

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
	virtual HRESULT SendToActor(const ACTORID actorId, CString& szMsg);
	virtual HRESULT BroadcastToActor(CString& szMsg);

	//
	BOOL SetActorCallbackInterface(IEMBActorHolderCallBackInterface* pCallIn);
	BOOL HasActor(const ACTORID actorId);

	virtual HRESULT SetSvrState(int nActiveStateIn, int nMaster);
	

private:
	//
	HRESULT DoSockSend(CMBCSocket* pSock, const char* pbufferIn, const int nSizeIn);
	void RemoveSock(CMBCSocket* pSock);
	void AddSock(CMBCSocket* pSock);
	ACTORID GetSockGuid(CMBCSocket* pSock);
	IEMBActorHolderCallBackInterface* m_pActorCallbackInterface;
	MAPSOCKACTORS m_mapSockIns;
	MAPACTORSOCKS m_mapActorMirrs;
	CAutoCritSec m_csSockMap;
	
	int m_nSvrActive;
	int m_nMaster;
};
