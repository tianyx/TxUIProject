#pragma once


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
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo);
	CFCVSClass *m_pFCVSClass;
};
}

extern EMB::CMediaCheck* g_pPluginInstane;
extern HMODULE g_hFCVS;