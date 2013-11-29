#include "StdAfx.h"
#include "StorageMgr.h"
#include "TxAutoComPtr.h"
#include "TxLogManager.h"

using namespace EMB;
CStorageMgr::CStorageMgr(void)
{
	m_pIStorage = NULL;
}

CStorageMgr::~CStorageMgr(void)
{
}

/*
*Description���ͷź���
*Input Param��
*Return Param��
*History���˺�������û����ɣ�û���ͷŲ�����ӣ�
*/
void CStorageMgr::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	TRACE("\nCStorageMgr::OnFinalRelease()");
	ReleaseTxLogMgr();
	delete this;
}

/*
*Description����ʼ������
*Input Param��
*Return Param��
*History��
*/
HRESULT CStorageMgr::OnFirstInit()
{
	//��ʼ����־
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskStorage.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKSTORAGE, strFile);

	return S_OK;
}

/*
*Description����ѯ���ղ����Ϣ�������ṩ��������߹����ܣ�
*Input Param��
*Return Param��
*History��
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

//�����������
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

/*
* Description����������洢�����
* Input Param��
*		pInterfaceIn���洢���ָ��
* Return Param�����سɹ���ʧ��
* History��
*/
HRESULT CStorageMgr::Connect( ITxUnkown* pInterfaceIn )
{
	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	//�˽ӿں�����ʲô
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
* Description���Ͽ�����洢���
* Input Param��
*		pInterfaceIn���洢���ָ��
* Return Param�����سɹ���ʧ��
* History��
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
	//ΪʲôIPluginStorageInterface�ӿ���û��SubmitTask���壿���û�����������ӽ�ȥ��
	/*if (m_pIStorage)
	{
		return m_pIStorage->SubmitTask(szTaskIn,szRet);
	}
	return E_NOTIMPL;*/
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
*Description�������������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
HRESULT CStorageMgr::Run_Plugin()
{
	Stop_Plugin();
#ifdef _DEBUG
	m_cfgStorage.nType = embStorageType_mem;
	m_pIStorage = new CEMBStorageMem;
#endif // _DEBUG
	return S_OK;
}

/*
*Description��ֹͣ�������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
HRESULT CStorageMgr::Stop_Plugin()
{
	if (m_pIStorage)
	{
		// ɾ���洢�����ڴ��
		if (m_cfgStorage.nType == embStorageType_mem)
		{
			CEMBStorageMem* pMem = (CEMBStorageMem*)m_pIStorage;
			delete pMem;
		}
		else
		{
			ASSERT(FALSE);
		}
	
		m_pIStorage = NULL;
	}
	return S_OK;
}

/*
* Description����ȡ������ú���
* Input Param��
*		szIn��
*       szOut:
* Return Param�����سɹ���ʧ��
* History��
*/
HRESULT CStorageMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	return S_OK;
}

/*
* Description�����ò�����ú���
* Input Param��
*		szIn��
*       szOut:
* Return Param�����سɹ���ʧ��
* History��
*/
HRESULT CStorageMgr::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	//��ȡ����״̬��Ϣ
	return S_OK;
}


