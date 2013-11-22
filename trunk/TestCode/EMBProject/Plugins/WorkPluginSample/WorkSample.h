#pragma once
#include "IEMBBaseInterface.h"

namespace EMB
{

class CWorkSample:
	public IPluginBaseInterface,
	public ITaskWorkerCallInterface
{
public:
	CWorkSample(void);
	virtual ~CWorkSample(void);

public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CTaskString& szInfo);

	BOOL RunTaskLoop();
	BOOL m_bTaskRunning;
	HANDLE m_hThreadTask;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;

};

}

extern EMB::CWorkSample* g_pPluginInstane;
