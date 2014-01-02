/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	TaskDispatchMgr.h
	author:		tianyx
	
	purpose:	主心跳检测
*********************************************************************/

#pragma once
#include "IEMBBaseInterface.h"
#include "ActorHolder.h"
#include "EmbStructDef.h"
#include "FGlobal.h"
#include "MasterHeartBeat.h"
#include "SlaveHeartBeat.h"
#include "IEMBBaseInterface.h"
#include <map>
using namespace std;

// 任务信息状态结构体
struct ST_FILETASKDATA
{
	ST_TASKBASIC taskBasic;
	ST_TASKRUNSTATE taskRunState;
	CString strTask;
};
typedef vector<TXGUID> VECTXIDS;
typedef map<TXGUID, ST_FILETASKDATA> MAPFILETASKS;

typedef map<ACTORID, ST_ACTORSTATE> MAPACTORSTATES;

//temp save disconnected actor
struct ST_ACTDISCONNINFO
{
	ACTORID actorid;
	time_t tmReport;
};
typedef vector<ST_ACTDISCONNINFO> VECACTORDISCONNCACHE;

typedef map<EMB::IDispatchNotifyCallbackInterface*, EMB::IDispatchNotifyCallbackInterface*> MAPDISNOTIFYS;

namespace EMB{

class CTaskDispatchMgr:
	public IPluginBaseInterface,
	public IPluginControlInterface, 
	public IPluginTaskCommit, 
	public IPluginConfigInterface,
	public IPluginConnectorInterce,
	public IEMBActorHolderCallBackInterface,
	public IServerLiveInterface,
	public IUIMessageProcessInterface,
	public IDispatchNotifyRegisterInterface,
	public IServerUI
{
public:
	CTaskDispatchMgr(void);
	virtual ~CTaskDispatchMgr(void);

	//interface for Iunknow
	virtual void	OnFinalRelease();
	virtual HRESULT OnFirstInit();
	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//for IPluginConnectorInterce
	//you riseup a action to connect/disconnect to other plugin
	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);

	//other plugin inform you after riseup a action that Connect()/Disconnect() you
	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);

	//interface for IPluginControlInterface
	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();
	//for IPluginTaskDispatch
	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);

	//for IEMBActorHolderCallBackInterface
	virtual HRESULT OnActorConnect(const ACTORID& szActorGuid, const CString& strPcName);
	virtual HRESULT OnActorDisConnect(const ACTORID& szActorGuid);
	virtual HRESULT OnActorMessage(const ACTORID& szActorGuid, CString& szActorInfoIn,  CString& szRet);

	//for  IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);

	//for IUIMessageProcessInterface
	virtual HRESULT OnUIMessage(CTaskString& strMsg, CTaskString& szRet);

	//for IDispatchNotifyRegisterInterface
	virtual HRESULT RegisterNotifier(IDispatchNotifyCallbackInterface* pNotifier);
	virtual HRESULT UnRegisterNotifier(IDispatchNotifyCallbackInterface* pNotifier);

	// Server UI
	virtual HRESULT GetActors(vector<CString>& vActor);


public:
	virtual HRESULT GetSelfState(ST_SVRLIVEINFO& infoOut);
public:
	//func for thread
	BOOL LoopProcFileTask(); 
	BOOL LoopProcCheck();
private:
	//to Remove the task from map return true;
	BOOL ExamTask(ST_FILETASKDATA& taskIn);
	BOOL ExamActor(ST_ACTORSTATE& actorStateIn);
	
	BOOL CheckDisconnActorCacheList();
	BOOL CheckActorState();
	int CalcActorPayload(ST_ACTORSTATE& actorStateIn);
	BOOL TryFetchTask();
private:
	HRESULT CommitFileTask(TXGUID& guidIn, ST_FILETASKDATA& taskIn);

	BOOL ConfirmActive();
	BOOL SetActive(int nActive);
	int GetActive(){return m_nActive;}

	HRESULT OnActiveStateChanged();

	ACTORID GetFirstIdleActor(const ACTORID nDesiredActor, int nPriority);
	BOOL SetActorInCD(const ACTORID actorId);

	HRESULT OnRemoveActor(const ACTORID nActorid);
	BOOL UpdateTaskRunState(ST_TASKREPORT& reportIn);
	BOOL UpdateActorState(ST_ACTORSTATE& stateIn);
	BOOL IsLiveActor(const ACTORID nActorId);

	BOOL GetIdleActors( CString strActorTeam,int nPriority );
	CString CreateSplitTaskXml(CString strTaskIn,int nStart,int nSize);
	CString CreateCombinedTaskXml(CString strTaskIn,int nSize);
	BOOL TaskNeedSplit(CString strTaskIn);
	BOOL TryDispatchSplitTask(ST_FILETASKDATA& taskIn);

	BOOL BroadcastToNotifier(CString& strInfo);
private:
	//func for ui interaction
	//
	HRESULT GetSvrInfo(CString& strRet);
	HRESULT GetActorList(CString& strRet);
	HRESULT GetActorState(ACTORID actorID, CString& strRet);
	HRESULT GetTaskList(CString& strRet);
	HRESULT GetTaskRunState(TXGUID& taskGuid, CString& strRet);

private:
	// 全部任务列表
	MAPFILETASKS m_mapTasks;
	// ACTOR列表
	MAPACTORSTATES m_mapActors;
	//for  OnActorDisConnect
	// 未连接设备列表
	VECACTORDISCONNCACHE m_vCachedDisconnActor;
	//notifiers, will called when dispatcher state changed.
	MAPDISNOTIFYS m_mapDisNotifys;
private:
	CAutoCritSec m_csFTask; //lock for m_mapTasks
	CAutoCritSec m_csActor; //lock for m_mapActors
	CAutoCritSec m_csCacheDisActor; //lock for m_vCachedDisconnActor
	CAutoCritSec m_csNotifier;

	CActorHolder m_actHolder;                 // ACTOR执行端控制着，控制所有ACTOR
	IPluginStorageInterface * m_pIStorage;    // 任务存储指针
	HANDLE m_hThdFtask;                       // 主备监听线程指针
	HANDLE m_hThdCheck;                       // 分配任务线程指针
	HANDLE m_hEventQuitLoop;
	//for master/slave check
	BOOL m_bCheckBackSvr;
	IRemoteSvrLiveInterface* m_pIbackSvr;
	int m_nActive;
	int m_nMaster;
	int m_nSvrId;

	//for config
	// 任务分配配置文件
	ST_TASKDISPATCHCONFIG m_config;

	int nfgTaskPoolSizeMax;
	//
	int nfgTaskReDispatchMaxCount;
	int nfgTaskDispatchCD;
	int nfgTaskReportIntervalMax;
	int nfgTaskCheckProgressIntervalMax;
	int nfgTaskLostTimeOutMax;
	
	int nfgActorLostTimeOutMax;
	int nfgActorCheckInterval;
	int nfgActorStateOutdate;
	int nfgActorAssignTaskCD;
	//
	int nfgCpuWeight;
	int nfgMemWeight;
	int nfgDiskIOWeight;
	int nfgNetIOWeight;
	int nfgMaxActorLoad;
	int nfgLowActorLoad;

	CArray<ST_ACTDISCONNINFO,ST_ACTDISCONNINFO&> m_FreeActorIds;    //free actor list
};

}
extern EMB::CTaskDispatchMgr* g_pPluginInstane;
