#include "StdAfx.h"
#include "MediaCheck.h"
using namespace EMB;
CMediaCheck::CMediaCheck(void)
{
}

CMediaCheck::~CMediaCheck(void)
{
}

HRESULT EMB::CMediaCheck::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;
	if (guidIn == GuidEMBPlugin_IBaseVC6)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterfaceVC6*>(this);
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
		return IPluginBaseInterfaceVC6::QueryInterface(guidIn, pInterfaceOut);
	}
}

HRESULT EMB::CMediaCheck::QueryPluginInfo( ST_PluginInfo& InfoOut )
{
	InfoOut.pluginGuid = GuidEMBPlugin_PWorkerMediaCheck;
	InfoOut.nPlugInType = PluginType_Wroker;
	InfoOut.nSubType = SubType_MediaCheck;
	return S_OK;
}

HRESULT EMB::CMediaCheck::OnFirstInit()
{
	return S_OK;
}

void EMB::CMediaCheck::OnFinalRelease()
{
}

HRESULT EMB::CMediaCheck::DoTask( const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	m_FCVSClass.MSGget = szTaskIn;
	int ret = m_FCVSClass.AnalysisGetMSG();
	if ((ret & ANA_TASK_CONFIG) != 0) 
	{
		m_FCVSClass.ConfigInit();	
	}
	else
	{
		return -1;
	}
	if ((ret & ANA_TASK_RUN)!=0)
	{
		m_FCVSClass.Taskstart();
	}
	return S_OK;
}

HRESULT EMB::CMediaCheck::CancelTask()
{
	m_FCVSClass.TaskCancel();
	return S_OK;
}

HRESULT EMB::CMediaCheck::GetTaskProgress( CTaskString& szInfo )
{
return S_OK;
}
