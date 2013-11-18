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

public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CTaskString& szInfo);

};

}