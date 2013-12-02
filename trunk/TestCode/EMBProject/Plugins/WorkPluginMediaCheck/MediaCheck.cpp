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

HRESULT EMB::CMediaCheck::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerMediaCheck;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_MediaCheck;
	vInfoOut.push_back(info);
	return S_OK;
}

HRESULT EMB::CMediaCheck::OnFirstInit()
{
	return S_OK;
}

void EMB::CMediaCheck::OnFinalRelease()
{
}

HRESULT EMB::CMediaCheck::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
return S_OK;
}

HRESULT EMB::CMediaCheck::CancelTask()
{
return S_OK;
}

HRESULT EMB::CMediaCheck::GetTaskProgress( CEMBWorkString& szInfo )
{
return S_OK;
}
