/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	TaskRiserMgr.h
	author:		tianyx
	
	purpose:	main taskriser class
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include "TaskProbertcp.h"

typedef vector<CTaskProber*> VECPROBERPTR;
namespace EMB{

class CTaskRiserMgr:
	public IPluginBaseInterface, 
	public IPluginControlInterface, 
	public ITaskCollectCallbackInterface,
	public IPluginConnectorInterce,
	public IPluginConfigInterface
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

	//IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);


	//for task
	virtual HRESULT TaskCollectCallback(CString& taskIn);

	virtual BOOL TaskCheckProc();
	virtual BOOL PopTask(VECTASKDATAS& vTaskOut);

private:
	CAutoCritSec m_lockTask;
	HANDLE m_hQuitEvent;
	HANDLE m_hTaskEvent;
	HANDLE m_hTaskCheckProc;

	int m_nMaxTaskLimit;
	VECTASKDATAS m_vTasks;
	VECPROBERPTR m_vProbers;
	IPluginTaskCommit* m_pTaskDispatcher;
	BOOL m_bRunning;

	ST_TASKRISERCONFIG m_config;

};

}

extern EMB::CTaskRiserMgr* g_pPluginInstane;
