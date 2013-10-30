#pragma once
#include "MBCBaseObj.h"
#include "TxParamString.h"
#include "AutoCritSec.h"
using namespace std;

#define ActorConnState_none 0
#define ActorConnState_conn 1
#define ActorConnState_ok	2
#define ActorConnState_del	3

interface IEMBActorHolderCallBackInterface
{
	virtual HRESULT OnActorConnect(const GUID& szActorGuid) = 0;
	virtual HRESULT OnActorDisConnect(const GUID& szActorGuid) = 0;
	virtual HRESULT OnActorReportInfo(const GUID& szActorGuid, CString& szActorInfoIn) = 0;
	virtual HRESULT OnActorDispatchTask(const GUID& szActorGuid, CString& szActorInfoIn) = 0;

};

interface IEMBActorSenderInterface
{
	virtual HRESULT SendToActor(const GUID& szActorGuid, CString& szMsg) = 0;
};

struct ST_ACTORDATA
{
	GUID strGuid;
	int  nActorConnState;
	CMBCSocket* pSock;

	ST_ACTORDATA()
	{
		pSock = NULL;
		strGuid = GUID_NULL;
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
	GUID GetSockGuid(CMBCSocket* pSock);
	IEMBActorHolderCallBackInterface* m_pActorCallbackInterface;
	MAPSOCKINS m_mapSockIns;
	CAutoCritSec m_locSockMap;
};
