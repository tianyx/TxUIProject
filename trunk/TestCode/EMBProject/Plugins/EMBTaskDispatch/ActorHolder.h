/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	ActorHolder.h
	author:		tianyx
	
	purpose:	执行端连接通讯类
*********************************************************************/
#pragma once
#include "MBCBaseObj.h"
#include "TxParamString.h"
#include "AutoCritSec.h"
#include "EMBDefine.h"
using namespace std;

interface IEMBActorHolderCallBackInterface
{
	// 执行端连接回调函数
	virtual HRESULT OnActorConnect(const ACTORID& szActorGuid) = 0;
	// 执行端断开连接回调函数
	virtual HRESULT OnActorDisConnect(const ACTORID& szActorGuid) = 0;
	// 执行端发送消息给任务管理回调函数
	virtual HRESULT OnActorMessage(const ACTORID& szActorGuid, CString& szActorInfoIn,  CString& szRet) = 0;
};

interface IEMBActorSenderInterface
{
	// 任务管理发送消息给执行端函数接口
	virtual HRESULT SendToActor(const ACTORID szActorGuid, CString& szMsg) = 0;
};

// 执行端信息结构体
struct ST_ACTORDATA
{
	// 执行端GUID
	ACTORID strGuid;
	// 执行端连接状态
	int  nActorConnState;
	// 执行端SOCKET连接指针
	CMBCSocket* pSock;
    // 构造函数
	ST_ACTORDATA()
	{
		pSock = NULL;
		strGuid = INVALID_ID;
		nActorConnState = embActorConnState_none;
	}
};

// 从SOCKET连接到ACTOR数据的对应图
typedef map<CMBCSocket*, ST_ACTORDATA> MAPSOCKACTORS;
// 从ACTOR ID到SOCKET连接数据的对应图
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
	
	// 任务管理当前状态，是否为主
	int m_nSvrActive;

	// 任务管理初始化状态，配置文件中是否为主
	int m_nMaster;
};
