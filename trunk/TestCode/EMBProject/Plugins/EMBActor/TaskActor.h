/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	TaskActor.h
	author:		tianyx
	
	purpose:	处理client发送的xml消息: 任务提交，状态查询
*********************************************************************/

#pragma once
#include "IEMBBaseInterface.h"
#include "ActorConnector.h"
#include "ExcutorMgr.h"
#include "EmbStructDef.h"
#include <map>
#include <deque>
using namespace std;

struct ST_TASKINACTOR
{
	TXGUID taskGuid;
	int nCurrStep;
	int nState;
	EXCUTORID excId;
	int nRetry;
	int nPercent;
	int nSubErrorCode; // 具体错误
	time_t tmLastReport;
	CString strTask;
	ST_TASKINACTOR()
	{
		taskGuid = GUID_NULL;
		nCurrStep = INVALID_VALUE;
		nState = embtaskstate_none;
		tmLastReport = 0;
		nRetry = 0;
		excId = INVALID_ID;
		nPercent = 0;
		nSubErrorCode = 0;
	}
};

typedef map<TXGUID, ST_TASKINACTOR> MAPTASKINACTOR;
typedef map<EXCUTORID, TXGUID> MAPEXCTASKS;
typedef deque<TXGUID> DQTASKS;
namespace EMB
{
class CTaskActor:
	public IPluginBaseInterface,
	public IPluginControlInterface,
	public IPluginConfigInterface,
// 	public IPluginConnectorInterce,
// 	public IActorMsgCallBackInterface,
	public IActorConnectorCallback,
	public IExcutorMsgCallBack,
	public IActorUI

{
public:
	CTaskActor(void);
	virtual ~CTaskActor(void);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();

	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//for IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);

	//for IPluginControlInterface
	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();
	//for IPluginConnectorInterce
// 	//you riseup a action to connect/disconnect to other plugin
// 	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
// 	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);
// 
// 	//other plugin inform you after riseup a action that Connect()/Disconnect() you
// 	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
// 	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);

	//interface IActorMsgCallBackInterface
// 	virtual HRESULT ActCallbackProc(CTaskString& szActMsg, CTaskString& szRet);

	//for IActorConnectorCallback
	/*
    Description：解析xml消息
	Input：		strInfo xml格式消息, strRet 返回信息
	Return:		S_OK 成功处理消息
	History：
	*/
	virtual HRESULT OnActorConnectorMsg(CString& strInfo, CString& strRet);
	//for IExcutorMsgCallBack
	/*
    Description：执行者消息处理
	Input：		excutorId 执行进程标识 szInfoIn xml消息
	Return:		S_OK 成功处理消息
	History：
	*/
	HRESULT OnExcutorMessage(const EXCUTORID excutorId, CString& szInfoIn);
	HRESULT OnExcutorExit(const EXCUTORID excutorId);

	// IActorUI 接口
	/*
    Description：获取运行中的Executor.exe 信息
	Input：		mapExecutor 返回运行的Executor信息
	Return:		S_OK 成功
	History：
	*/
	virtual HRESULT GetExecutors(vector<CString>& vExecutor);
	// ------------------------------------------------------

private:
	BOOL SwitchActorConn(BOOL bMainConn);
	BOOL ReportTaskState(ST_TASKINACTOR& infoIn);
	BOOL OnExcutorIdle(const EXCUTORID excutorId);
	CActorConnector* GetActiveActorConnector(){return (m_nActiveConn == 1)?(&m_actorconnMain):(m_nActiveConn == 2)? (&m_actorconnSlave):NULL;}
	/*
    Description：任务处理完后，将结果保存为xml文件
	Input：		tskReport 任务信息 
	Return:		true 成功
	History：
	*/
	bool TaskResultSaveXmlFile(ST_TASKREPORT& tskReport);
	bool QueryXmlFile(const CString& strTaskGuid, ST_TASKREPORT& tskInfor);
	bool FindTask(const EXCUTORID& strExecutorId, ST_TASKINACTOR& tsk);

private:
	CActorConnector m_actorconnMain;
	CActorConnector m_actorconnSlave;
	int m_nActiveConn;
	CExcutorMgr* m_pExcutorMgr;
	ST_ACTORCONFIG m_ActRegInfo;

	CAutoCritSec m_csmapLock;
	MAPTASKINACTOR m_mapTaskinActor;
	MAPEXCTASKS m_mapExcTask;
	DQTASKS m_dqRecentFinishedTasks;
	BOOL m_bRuning;
	int nfgRetryMax;
	CString m_strTaskXmlPath; // 保存xml文件的路径
};


}
extern EMB::CTaskActor* g_pPluginInstane;
