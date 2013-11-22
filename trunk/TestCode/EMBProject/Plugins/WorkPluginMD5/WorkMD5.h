#pragma once

#include "IEMBBaseInterface.h"
#include "vrmd5e.h"
#include "EmbStructDef.h"

namespace EMB
{

	class CWorkMD5:
		public IPluginBaseInterface,
		public ITaskWorkerCallInterface
	{
	public:
		CWorkMD5(void);
		virtual ~CWorkMD5(void);

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

	private:
		CString ComputeMD5(const CString& strFilePath);
		BOOL WriteMD5File(const CString& strMD5, const CString& strMd5FileIn);
		BOOL m_bTaskRunning;
		HANDLE m_hThreadTask;
		ITaskReportToExcutorInterface* m_pReportCallback;
		int m_nPercent;
		CVRMD5E m_MD5Computer;
		CString m_strTask;
		ST_MD5TASKINFO m_taskInfo;
	};

}

extern EMB::CWorkMD5* g_pPluginInstane;
