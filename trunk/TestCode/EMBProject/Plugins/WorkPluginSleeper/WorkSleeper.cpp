#include "StdAfx.h"
#include "WorkSleeper.h"
#include "TxParamString.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "EmbStructDef.h"

using namespace EMB;

//////////////////////////////////////////////////////////////////////////

DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CWorkSleeper* pObj = (CWorkSleeper*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CWorkSleeper::CWorkSleeper(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_nCodeRet = S_OK;
}

CWorkSleeper::~CWorkSleeper(void)
{
}


HRESULT EMB::CWorkSleeper::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerSleeper;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_Sleeper;
	vInfoOut.push_back(info);
	return S_OK;

}

HRESULT EMB::CWorkSleeper::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_taskInfo.FromString(szTaskIn);
	if (m_taskInfo.nSleepSec <= 0 || m_taskInfo.strExtInfo.IsEmpty())
	{
		CFWriteLog(TEXT("sleep param error!"));
		return EMBERR_INVALIDARG;
	}

	m_pReportCallback = pICallback;
	m_nCodeRet = S_OK;
	m_nPercent = 0;
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);

	//ret if need
	if (m_taskInfo.nReCallType != embrecalltype_none)
	{
		ST_WORKERRET stRet;
		stRet.nRetType = m_taskInfo.nReCallType;
		stRet.strRetInfo = m_taskInfo.strExtInfo;
		CString strRet;
		stRet.ToString(strRet);
		szRet = strRet;

	}
	return S_OK;
}

HRESULT EMB::CWorkSleeper::CancelTask()
{
	if (m_hThreadTask)
	{
		SetEvent(m_hQuitEvent);
		WaitForSingleObject(m_hThreadTask, INFINITE);
		m_hThreadTask = NULL;
	}
	return S_OK;
}

HRESULT EMB::CWorkSleeper::GetTaskProgress( CEMBWorkString& szInfo )
{
	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	report.code = m_nCodeRet;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

HRESULT EMB::CWorkSleeper::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;
	if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_ITaskWorkerCall)
	{
		pInterfaceOut = dynamic_cast<ITaskWorkerCallInterface*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

HRESULT EMB::CWorkSleeper::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKACTOR, TEXT("SleepLog"));

	return S_OK;
}

void EMB::CWorkSleeper::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCWorkSleep::OnFinalRelease() ");

	delete this;
}

BOOL EMB::CWorkSleeper::RunTaskLoop()
{
	//
	CFWriteLog(NULL, TEXT("work started sleep loop!!....."));
	BOOL bCanceled = FALSE;
	for (size_t i = 0; i <= m_taskInfo.nSleepSec; ++i)
	{
		if (WaitForSingleObject(m_hQuitEvent, 1000) == WAIT_OBJECT_0)
		{
			bCanceled = TRUE;
			break;
		}

		int nPercent = (((double)i)/m_taskInfo.nSleepSec)*100.0;
		if (nPercent >= 100)
		{
			nPercent = 99;
		}
		m_nPercent = nPercent;
	}

	if (!bCanceled)
	{
		m_nPercent = 100;
	}
	m_nCodeRet = m_taskInfo.nRetOnFinish;
// 	if (m_pReportCallback)
// 	{
// 		CString strReport;
// 		ST_WORKERREPORT report;
// 		report.nPercent = m_nPercent;
// 		report.ToString(strReport);
// 		m_pReportCallback->OnDllReportTaskProgress(strReport);
// 	}

	CFWriteLog(NULL, TEXT("work end!!....."));
	m_hThreadTask = NULL;

	return TRUE;

}
