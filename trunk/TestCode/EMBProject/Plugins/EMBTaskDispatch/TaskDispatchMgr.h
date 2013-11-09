#pragma once
#include "IEMBBaseInterface.h"
#include "ActorHolder.h"
#include "EmbStructDef.h"
#include "FGlobal.h"
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

namespace EMB{

class CTaskDispatchMgr:
	public IPluginBaseInterface,
	public IPluginControlInterface, 
	public IPluginTaskCommit, 
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
	virtual HRESULT OnActorReportInfo(const ACTORID& szActorGuid, CString& szActorInfoIn);
	virtual HRESULT OnActorDispatchTask(const ACTORID& szActorGuid, CString& szActorInfoIn);

public:
	virtual HRESULT GetSelfState(ST_SVRLIVEINFO& infoOut);
public:
	//func for thread
	BOOL LoopProcFileTask(); 
	BOOL LoopProcCheck();
private:
	BOOL ExamTask(ST_FILETASKDATA& taskIn);

private:
	HRESULT CommitFileTask(TXGUID& guidIn, ST_FILETASKDATA& taskIn);



private:
	MAPFILETASKS m_mapTasks;
private:
	CAutoCritSec m_csFTask; //lock for m_mapTasks
	CActorHolder m_actHolder;
	IPluginStorageInterface * m_pIStorage;
	HANDLE m_hThdFtask;
	HANDLE m_hThdCheck;
	HANDLE m_hEventQuitLoop;
	int m_nActived;
	int m_nMaster;
	int m_nSvrId;
};

}
extern EMB::CTaskDispatchMgr* g_pPluginInstane;
