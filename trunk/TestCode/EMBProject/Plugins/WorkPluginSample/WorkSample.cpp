#include "StdAfx.h"
#include "WorkSample.h"
#include "TxParamString.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "EmbStructDef.h"

//////////////////////////////////////////////////////////////////////////
using namespace EMB;

DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CWorkSample* pObj = (CWorkSample*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CWorkSample::CWorkSample(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
}

CWorkSample::~CWorkSample(void)
{
}


HRESULT EMB::CWorkSample::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerSample;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_WorkSample;
	vInfoOut.push_back(info);
	return S_OK;

}

HRESULT EMB::CWorkSample::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pReportCallback = pICallback;
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
	return S_OK;
}

HRESULT EMB::CWorkSample::CancelTask()
{
	return S_OK;
}

HRESULT EMB::CWorkSample::GetTaskProgress( CEMBWorkString& szInfo )
{
	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

HRESULT EMB::CWorkSample::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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

HRESULT EMB::CWorkSample::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKACTOR, TEXT("WorkSampleLog"));

	return S_OK;
}

void EMB::CWorkSample::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCWorkSample::OnFinalRelease() ");

	delete this;
}

BOOL EMB::CWorkSample::RunTaskLoop()
{
	//
	CFWriteLog(NULL, TEXT("work started!!....."));
	for (size_t i = 0; i <= 100; ++i)
	{
		Sleep(1000);
		m_nPercent = i;
		if (m_pReportCallback)
		{
			CString strReport;
			ST_WORKERREPORT report;
			report.nPercent = m_nPercent;
			report.ToString(strReport);
			//m_pReportCallback->OnDllReportTaskProgress(strReport);
		}
	}
	
	CFWriteLog(NULL, TEXT("work end!!....."));
	m_hThreadTask = NULL;
		
	return TRUE;

}
