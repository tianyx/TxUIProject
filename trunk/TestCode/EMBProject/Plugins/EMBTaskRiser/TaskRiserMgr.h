/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	TaskRiserMgr.h
	author:		tianyx
	
	purpose:	主任务接收类
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include "TaskProbertcp.h"

//任务接收接口列表，支持多接收接口同时工作
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
	//插件释放函数
	virtual void	OnFinalRelease();
	//插件初始化函数
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
	//退出事件
	HANDLE m_hQuitEvent;
	//接收任务时间
	HANDLE m_hTaskEvent;
	//接收线程句柄
	HANDLE m_hTaskCheckProc;

	//可接收最大任务数
	int m_nMaxTaskLimit;
	//EMB接收到的任务列表
	VECTASKDATAS m_vTasks;
	//接收接口列表（TCP,WEBSERVICE等）
	VECPROBERPTR m_vProbers;
	//任务存储插件指针
	IPluginTaskCommit* m_pTaskStorage;

	//配置文件结构体
	ST_TASKRISERCONFIG m_config;

};

}

extern EMB::CTaskRiserMgr* g_pPluginInstane;
