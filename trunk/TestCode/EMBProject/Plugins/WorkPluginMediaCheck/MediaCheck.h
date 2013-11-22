#pragma once
#include "IEMBBaseInterface.h"
namespace EMB
{

	class CMediaCheck:
		public IPluginBaseInterface,
		public ITaskWorkerCallInterface
{
public:
	CMediaCheck(void);
	~CMediaCheck(void);

public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CTaskString& szInfo);

};
}

extern EMB::CMediaCheck* g_pPluginInstane;