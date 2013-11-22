#pragma once
#include "IEMBBaseInterface.h"
#include "ActorHolder.h"
#include "EmbStructDef.h"
#include "FGlobal.h"
#include "MasterHeartBeat.h"
#include "SlaveHeartBeat.h"
#include <map>
using namespace std;
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

namespace EMB{

class CTaskDispatchMgr:
	public IPluginBaseInterface,
	public IPluginControlInterface, 
	public IPluginTaskCommit, 
	public IPluginConfigInterface,
	public IPluginConnectorInterce,
	public IEMBActorHolderCallBackInterface,
	public IServerLiveInterface
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
	virtual HRESULT OnActorConnect(const ACTORID& szActorGuid);
	virtual HRESULT OnActorDisConnect(const ACTORID& szActorGuid);
	virtual HRESULT OnActorMessage(const ACTORID& szActorGuid, CString& szActorInfoIn,  CString& szRet);

	//for  IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);

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
	HRESULT OnRemoveActor(const ACTORID nActorid);
	BOOL UpdateTaskRunState(ST_TASKREPORT& reportIn);
	BOOL UpdateActorState(ST_ACTORSTATE& stateIn);

private:
	MAPFILETASKS m_mapTasks;
	MAPACTORSTATES m_mapActors;
	//for  OnActorDisConnect
	VECACTORDISCONNCACHE m_vCachedDisconnActor;
private:
	CAutoCritSec m_csFTask; //lock for m_mapTasks
	CAutoCritSec m_csActor; //lock for m_mapActors
	CAutoCritSec m_csCacheDisActor; //lock for m_vCachedDisconnActor

	CActorHolder m_actHolder;
	IPluginStorageInterface * m_pIStorage;
	HANDLE m_hThdFtask;
	HANDLE m_hThdCheck;
	HANDLE m_hEventQuitLoop;
	//for master/slave check
	BOOL m_bCheckBackSvr;
	IRemoteSvrLiveInterface* m_pIbackSvr;
	int m_nActive;
	int m_nMaster;
	int m_nSvrId;

	//for config
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
	
	//
	int nfgCpuWeight;
	int nfgMemWeight;
	int nfgDiskWeight;
	int nfgMaxActorLoad;
	int nfgLowActorLoad;
};

}
extern EMB::CTaskDispatchMgr* g_pPluginInstane;
