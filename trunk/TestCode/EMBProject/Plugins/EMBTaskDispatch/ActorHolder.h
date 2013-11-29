/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	ActorHolder.h
	author:		tianyx
	
	purpose:	ִ�ж�����ͨѶ��
*********************************************************************/
#pragma once
#include "MBCBaseObj.h"
#include "TxParamString.h"
#include "AutoCritSec.h"
#include "EMBDefine.h"
using namespace std;

interface IEMBActorHolderCallBackInterface
{
	// ִ�ж����ӻص�����
	virtual HRESULT OnActorConnect(const ACTORID& szActorGuid) = 0;
	// ִ�ж˶Ͽ����ӻص�����
	virtual HRESULT OnActorDisConnect(const ACTORID& szActorGuid) = 0;
	// ִ�ж˷�����Ϣ���������ص�����
	virtual HRESULT OnActorMessage(const ACTORID& szActorGuid, CString& szActorInfoIn,  CString& szRet) = 0;
};

interface IEMBActorSenderInterface
{
	// �����������Ϣ��ִ�ж˺����ӿ�
	virtual HRESULT SendToActor(const ACTORID szActorGuid, CString& szMsg) = 0;
};

// ִ�ж���Ϣ�ṹ��
struct ST_ACTORDATA
{
	// ִ�ж�GUID
	ACTORID strGuid;
	// ִ�ж�����״̬
	int  nActorConnState;
	// ִ�ж�SOCKET����ָ��
	CMBCSocket* pSock;
    // ���캯��
	ST_ACTORDATA()
	{
		pSock = NULL;
		strGuid = INVALID_ID;
		nActorConnState = embActorConnState_none;
	}
};

// ��SOCKET���ӵ�ACTOR���ݵĶ�Ӧͼ
typedef map<CMBCSocket*, ST_ACTORDATA> MAPSOCKACTORS;
// ��ACTOR ID��SOCKET�������ݵĶ�Ӧͼ
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
	
	// �������ǰ״̬���Ƿ�Ϊ��
	int m_nSvrActive;

	// ��������ʼ��״̬�������ļ����Ƿ�Ϊ��
	int m_nMaster;
};
