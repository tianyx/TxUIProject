#pragma once

#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

namespace EMB
{
	class CMergeDBWriter:
		public IPluginBaseInterface,
		public ITaskWorkerCallInterface
	{
	public:
		CMergeDBWriter(void);
		~CMergeDBWriter(void);

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

	private:
		HANDLE m_hThreadTask;
		ITaskReportToExcutorInterface* m_pReportCallback;
		int m_nPercent;
		CString m_strTask;
		ST_MERGETODBWRITER m_taskInfo;
		int m_nMaxSplitCount;
	};

}

extern EMB::CMergeDBWriter* g_pPluginInstane;

