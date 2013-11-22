#pragma once
#include "IEMBBaseInterface.h"
#include "FCVSClass.h"

namespace EMB
{

	class CMediaCheck:
		public IPluginBaseInterfaceVC6,
		public ITaskWorkerCallInterface
{
public:
	CMediaCheck(void);
	~CMediaCheck(void);

public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(ST_PluginInfo& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CTaskString& szInfo);
	FCVSClass m_FCVSClass;
};
}

extern EMB::CMediaCheck* g_pPluginInstane;
