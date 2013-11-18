#include "StdAfx.h"
#include "WorkSample.h"

using namespace EMB;
CWorkSample::CWorkSample(void)
{
}

CWorkSample::~CWorkSample(void)
{
}

HRESULT EMB::CWorkSample::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IStorage;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_WorkSample;
	vInfoOut.push_back(info);
	return S_OK;

}

HRESULT EMB::CWorkSample::DoTask( const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	return S_OK;
}

HRESULT EMB::CWorkSample::CancelTask()
{
	return S_OK;
}

HRESULT EMB::CWorkSample::GetTaskProgress( CTaskString& szInfo )
{
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
