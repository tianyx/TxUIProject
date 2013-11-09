#include "StdAfx.h"
#include "StorageMgr.h"
#include "TxAutoComPtr.h"

using namespace EMB;
CStorageMgr::CStorageMgr(void)
{
}

CStorageMgr::~CStorageMgr(void)
{
}

void CStorageMgr::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	TRACE("\nCStorageMgr::OnFinalRelease()");
	delete this;
}

HRESULT CStorageMgr::OnFirstInit()
{
	return S_OK;
}

HRESULT CStorageMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IStorage;
	info.nPlugInType = PluginType_Storage;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT CStorageMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

	// 	if (guidIn == GuidEMBPlugin_TaskRiser)
	// 	{
	// 		pInterfaceOut = dynamic_cast<IPluginTaskRiserInterface*>(this);
	// 		AddRef();
	// 		return S_OK;
	// 	}
	if (guidIn == GuidEMBPlugin_IBase)
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
	else if (guidIn == GuidEMBPlugin_IStorage)
	{
		pInterfaceOut = dynamic_cast<IPluginStorageInterface*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_ITaskCommit)
	{
		pInterfaceOut = dynamic_cast<IPluginTaskCommit*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

HRESULT CStorageMgr::Connect( ITxUnkown* pInterfaceIn )
{
	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
	if (pConn != NULL)
	{
		return pConn->OnConnect(this);
	}
	else
	{
		return S_FALSE;
	}

}

HRESULT CStorageMgr::Disconnect( ITxUnkown* pInterfaceIn )
{
	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
	if (pConn != NULL)
	{
		return pConn->OnDisconnect(this);
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT CStorageMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT CStorageMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT CStorageMgr::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	return S_OK;
}

HRESULT CStorageMgr::UpdateTaskToStorage( const int nDispatchID, CTaskString& szTaskIn )
{
	if (m_pIStorage)
	{
		return m_pIStorage->UpdateTaskToStorage(nDispatchID, szTaskIn);
	}
	return E_NOTIMPL;
}

HRESULT CStorageMgr::FetchTaskFromStorage( const int nDispatchID, int nDesiredNum, VECTASKS& vTasks )
{
	if (m_pIStorage)
	{
		return m_pIStorage->FetchTaskFromStorage(nDispatchID, nDesiredNum, vTasks);
	}
	return E_NOTIMPL;
}

HRESULT CStorageMgr::Run_Plugin()
{
	Stop_Plugin();
#ifdef _DEBUG
	m_pIStorage = new CEMBStorageMem;
#endif // _DEBUG
	return S_OK;
}

HRESULT CStorageMgr::Stop_Plugin()
{
	if (m_pIStorage)
	{
		delete m_pIStorage;
		m_pIStorage = NULL;
	}
	return S_OK;
}

HRESULT CStorageMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	return S_OK;
}

HRESULT CStorageMgr::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	return S_OK;
}
