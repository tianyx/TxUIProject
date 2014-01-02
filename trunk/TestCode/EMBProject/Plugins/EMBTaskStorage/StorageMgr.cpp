#include "StdAfx.h"
#include "StorageMgr.h"
#include "TxAutoComPtr.h"
#include "TxLogManager.h"

using namespace EMB;
CStorageMgr::CStorageMgr(void)
{
	m_pIStorage = NULL;
	m_pTaskCommit = NULL;
}

CStorageMgr::~CStorageMgr(void)
{
}

/*
*Description：释放函数
*Input Param：
*Return Param：
*History：此函数好像没有完成，没有释放插件连接？
*/
void CStorageMgr::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	TRACE("\nCStorageMgr::OnFinalRelease()");
	ReleaseTxLogMgr();
	delete this;
}

/*
*Description：初始化函数
*Input Param：
*Return Param：
*History：
*/
HRESULT CStorageMgr::OnFirstInit()
{
	//初始化日志
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskStorage.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKSTORAGE, strFile);

	return S_OK;
}

/*
*Description：查询接收插件信息，对外提供插件管理者管理功能？
*Input Param：
*Return Param：
*History：
*/
HRESULT CStorageMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IStorage;
	info.nPlugInType = PluginType_Storage;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

//
HRESULT CStorageMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

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
	else if (guidIn == GuidEMBPlugin_IConfig)
	{
		pInterfaceOut = dynamic_cast<IPluginConfigInterface*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

/*
* Description：连接任务存储插件？
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT CStorageMgr::Connect( ITxUnkown* pInterfaceIn )
{
	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	//此接口含义是什么
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

/*
* Description：断开任务存储插件
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
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

//?
HRESULT CStorageMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

//?
HRESULT CStorageMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	return S_OK;
}

HRESULT CStorageMgr::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	if (m_pIStorage && m_pTaskCommit)
	{
		return m_pTaskCommit->SubmitTask(szTaskIn,szRet);
	}
	return E_NOTIMPL;
}

HRESULT CStorageMgr::UpdateActorID(CTaskString& strTaskGuid, ACTORID actorId)
{
	if(m_pIStorage)
	{
		return m_pIStorage->UpdateActorID(strTaskGuid, actorId);
	}

	return E_NOTIMPL;
}

HRESULT CStorageMgr::UpdateTaskToStorage( const int nDispatchID, CTaskString& szTaskIn )
{
	if (m_pIStorage)
	{
		return m_pIStorage->UpdateTaskToStorage(nDispatchID, szTaskIn);
	}
	return E_NOTIMPL;
}

HRESULT CStorageMgr::FetchTaskFromStorage( const int nDispatchID,  int nMinPriority, int nDesiredNum, VECTASKS& vTasks )
{
	if (m_pIStorage)
	{
		return m_pIStorage->FetchTaskFromStorage(nDispatchID,  nMinPriority, nDesiredNum, vTasks);
	}
	return E_NOTIMPL;
}
HRESULT EMB::CStorageMgr::GetDispatchedTaskFromStorage( const DISPATCHID nDispatchID, VECTASKS& vTasks )
{
	if (m_pIStorage)
	{
		return m_pIStorage->GetDispatchedTaskFromStorage(nDispatchID, vTasks);
	}
	return E_NOTIMPL;
}

/*
*Description：启动插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CStorageMgr::Run_Plugin()
{
	Stop_Plugin();
	if(m_cfgStorage.nType == embStorageType_mem)
	{
		CEMBStorageMem* pTmpStorage = new CEMBStorageMem;
		m_pIStorage = pTmpStorage;
		m_pTaskCommit = pTmpStorage;

	}
	else if(m_cfgStorage.nType == embStorageType_db)
	{
		CEMBStorageDB *pIDBStorage = new CEMBStorageDB;
		m_pIStorage = pIDBStorage;
		pIDBStorage->SetDBConnectString(m_cfgStorage.strDBConnect);
		m_pTaskCommit = pIDBStorage;
	}

/*#ifdef _DEBUG
	m_cfgStorage.nType = embStorageType_mem;
	m_pIStorage = new CEMBStorageMem;
#endif */// _DEBUG
	return S_OK;
}

/*
*Description：停止插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CStorageMgr::Stop_Plugin()
{
	if (m_pIStorage)
	{
		// 删除存储任务内存表
		
		delete m_pIStorage;
		m_pIStorage = NULL;
		m_pTaskCommit = NULL;
	}
	return S_OK;
}

/*
* Description：获取插件配置函数
* Input Param：
*		szIn：
*       szOut:
* Return Param：返回成功或失败
* History：
*/
HRESULT CStorageMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	m_cfgStorage.ToString(szOut);
	return S_OK;
}

/*
* Description：设置插件配置函数
* Input Param：
*		szIn：
*       szOut:
* Return Param：返回成功或失败
* History：
*/
HRESULT CStorageMgr::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	//获取缓存状态信息
	ST_TASKSTORAGECONFIG tmpConfig;
	tmpConfig.FromString(szIn);
	m_cfgStorage = tmpConfig;

	return S_OK;
}


