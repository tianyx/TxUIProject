#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

namespace EMB
{
class CWorkSleeper:
	public IPluginBaseInterface,//插件必须实现接口
	public ITaskWorkerCallInterface//执行者插件必须实现的接口
{
public:
	CWorkSleeper(void);
	virtual ~CWorkSleeper(void);
public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	//任务开始接口,此函数要求立即返回。任务执行请开另一个线程
	//第三个参数为回调接口
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback);

	virtual HRESULT CancelTask();
	//进度查询接口
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo);

	BOOL RunTaskLoop();
	BOOL m_bTaskRunning;
	HANDLE m_hThreadTask;
	HANDLE m_hQuitEvent;
	ST_SLEEPTASKINFO m_taskInfo;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;
	int m_nCodeRet;


};

}

extern EMB::CWorkSleeper* g_pPluginInstane;
