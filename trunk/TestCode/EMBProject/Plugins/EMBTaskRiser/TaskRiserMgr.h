#pragma once
#include "IEMBBaseInterface.h"
#include "TaskProbertcp.h"
namespace EMB{

class CTaskRiserMgr:
	public IPluginBaseInterface, 
	public IPluginControlInterface, 
	public ITaskCollectCallbackInterface,
	public IPluginConnectorInterce
{
public:
	CTaskRiserMgr(void);
	virtual ~CTaskRiserMgr(void);

	//interface for Iunknow
	virtual void	OnFinalRelease();
	virtual HRESULT OnFirstInit();
	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//interface IPluginConnectorInterce
	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);

	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);


	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();

	//for task
	virtual HRESULT TaskCollectCallback(CString& taskIn);

	virtual BOOL TaskCheckProc();
	virtual BOOL PopTask(VECTASKDATAS& vTaskOut);

	CAutoCritSec m_lockTask;
	HANDLE m_hQuitEvent;
	HANDLE m_hTaskEvent;
	HANDLE m_hTaskCheckProc;

	int m_nMaxTaskLimit;
	VECTASKDATAS m_vTasks;

	IPluginTaskCommit* m_pTaskDispatcher;
};

}

extern EMB::CTaskRiserMgr* g_pPluginInstane;
