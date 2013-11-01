#include "StdAfx.h"
#include "TaskDispatchMgr.h"
#include "IEMBBaseInterface.h"
using namespace EMB;
CTaskDispatchMgr::CTaskDispatchMgr(void)
{
	m_actHolder.SetActorCallbackInterface(this);
}

CTaskDispatchMgr::~CTaskDispatchMgr(void)
{
}

HRESULT EMB::CTaskDispatchMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IPluginManager;
	info.nPlugInType = PluginType_Dispatcher;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

	if (guidIn == GuidEMBPlugin_ITaskCommit)
	{
		pInterfaceOut = dynamic_cast<IPluginTaskCommit*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_IControler)
	{
		pInterfaceOut = dynamic_cast<IPluginControlInterface*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_IConnector)
	{
		pInterfaceOut = dynamic_cast<IPluginConnectorInterce*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

void EMB::CTaskDispatchMgr::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	delete this;
}

HRESULT EMB::CTaskDispatchMgr::OnFirstInit()
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Run_Plugin()
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Stop_Plugin()
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Connect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Disconnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorConnect( const GUID& szActorGuid )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorDisConnect( const GUID& szActorGuid )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorReportInfo( const GUID&szActorGuid, CString& szActorInfoIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorDispatchTask( const GUID& szActorGuid, CString& szActorInfoIn )
{
	return S_OK;
}

