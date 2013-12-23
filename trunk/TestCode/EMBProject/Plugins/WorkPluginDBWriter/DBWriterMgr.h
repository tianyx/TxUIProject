#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

namespace EMB{

class CDBWriterMgr:
	public IPluginBaseInterface,
	public ITaskWorkerCallInterface
{
public:
	CDBWriterMgr(void);
	virtual ~CDBWriterMgr(void);

public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo);

	BOOL RunTaskLoop();


	ST_DBWRITERTASKINFO m_taskInfo;
	int m_nPercent;
	CString m_strTask;
	HANDLE m_hThreadTask;
	ITaskReportToExcutorInterface* m_pReportCallback;

};

}

extern EMB::CDBWriterMgr* g_pPluginInstane;
