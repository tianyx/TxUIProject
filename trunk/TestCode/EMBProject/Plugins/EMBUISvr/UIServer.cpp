#include "StdAfx.h"
#include "UIServer.h"
#include "IEMBBaseInterface.h"
#include "TxLogManager.h"
#include "TxAutoComPtr.h"
#include "EMBCommonFunc.h"

using namespace EMB;
CUIServer::CUIServer(void)
{
	m_pIUIMsgProcess = NULL;
	m_pITaskCommit = NULL;
}

CUIServer::~CUIServer(void)
{
}

void EMB::CUIServer::OnFinalRelease()
{
	ASSERT(g_pPluginInstane == this);
	TRACE("\nCUIServer OnFinalRelease ");
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	delete this;
}

HRESULT EMB::CUIServer::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_UISERVER, TEXT("UIServer"));

	return S_OK;
}

HRESULT EMB::CUIServer::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerUIServer;
	info.nPlugInType = PluginType_UIServer;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT EMB::CUIServer::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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

HRESULT EMB::CUIServer::Connect( ITxUnkown* pInterfaceIn )
{
	
	CTxAutoComPtr<IUIMessageProcessInterface> apUIMsgProcess;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IUIMessageProcess, (LPVOID&)*&apUIMsgProcess);
	if (apUIMsgProcess)
	{
		if (m_pIUIMsgProcess)
		{
			ASSERT(FALSE);
			IDispatchNotifyRegisterInterface* pIRegister = dynamic_cast<IDispatchNotifyRegisterInterface*>(m_pIUIMsgProcess);
			if (pIRegister)
			{
				pIRegister->UnRegisterNotifier(this);
			}
			m_pIUIMsgProcess->Release();
		}
		m_pIUIMsgProcess = apUIMsgProcess;
		m_pIUIMsgProcess->AddRef();

		//register notifier
		IDispatchNotifyRegisterInterface* pIRegister = dynamic_cast<IDispatchNotifyRegisterInterface*>(m_pIUIMsgProcess);
		if (pIRegister)
		{
			pIRegister->RegisterNotifier(this);
		}

	}

	CTxAutoComPtr<IPluginTaskCommit> apTaskCommit;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IUIMessageProcess, (LPVOID&)*&apTaskCommit);
	if (apUIMsgProcess)
	{
		if (m_pITaskCommit)
		{
			ASSERT(FALSE);
			m_pITaskCommit->Release();
		}
		m_pITaskCommit = apTaskCommit;
		m_pITaskCommit->AddRef();
	}


	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
	ASSERT(pConn);
	pConn->OnConnect(this);

	return S_OK;

}

HRESULT EMB::CUIServer::Disconnect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pIUIMsgProcess)
	{
		IDispatchNotifyRegisterInterface* pIRegister = dynamic_cast<IDispatchNotifyRegisterInterface*>(m_pIUIMsgProcess);
		if (pIRegister)
		{
			pIRegister->UnRegisterNotifier(this);
		}

		m_pIUIMsgProcess->Release();
	}

	if (pInterfaceIn == m_pITaskCommit)
	{
		m_pITaskCommit->Release();
	}

	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
	ASSERT(pConn);
	pConn->OnDisconnect(this);
	return S_OK;

}

HRESULT EMB::CUIServer::OnConnect( ITxUnkown* pInterfaceIn )
{
	IUIMessageProcessInterface * pIUIProcess = NULL;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IUIMessageProcess, (LPVOID&)pIUIProcess);
	if (pIUIProcess)
	{	
		if (m_pIUIMsgProcess)
		{
			ASSERT(FALSE);
			IDispatchNotifyRegisterInterface* pIRegister = dynamic_cast<IDispatchNotifyRegisterInterface*>(m_pIUIMsgProcess);
			if (pIRegister)
			{
				pIRegister->UnRegisterNotifier(this);
			}
			pIUIProcess->Release();
		}
		else
		{
			m_pIUIMsgProcess = pIUIProcess;
			IDispatchNotifyRegisterInterface* pIRegister = dynamic_cast<IDispatchNotifyRegisterInterface*>(m_pIUIMsgProcess);
			if (pIRegister)
			{
				pIRegister->RegisterNotifier(this);
			}
		}
	}

	IPluginTaskCommit* pITaskCommit = NULL;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskCommit, (LPVOID&)pITaskCommit);
	if (pITaskCommit)
	{	
		if (m_pITaskCommit)
		{
			ASSERT(FALSE);
			m_pITaskCommit->Release();
		}
		else
		{
			m_pITaskCommit = pITaskCommit;
		}
	}

	return S_OK;
}

HRESULT EMB::CUIServer::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pIUIMsgProcess)
	{
		IDispatchNotifyRegisterInterface* pIRegister = dynamic_cast<IDispatchNotifyRegisterInterface*>(m_pIUIMsgProcess);
		if (pIRegister)
		{
			pIRegister->UnRegisterNotifier(this);
		}
		m_pIUIMsgProcess->Release();
		m_pIUIMsgProcess = NULL;
	}
	if (pInterfaceIn == m_pITaskCommit)
	{
		m_pITaskCommit->Release();
		m_pITaskCommit = NULL;
	}
	return S_OK;

}

HRESULT EMB::CUIServer::Run_Plugin()
{

	m_Prober.SetScokAddr(&m_cfgUISvr.addrListen, &m_cfgUISvr.addrListen);
	m_Prober.SetMsgProcessor(this);
	return m_Prober.Run_Prober();

}

HRESULT EMB::CUIServer::Stop_Plugin()
{
	return m_Prober.Stop_Prober();

}

HRESULT EMB::CUIServer::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	return S_OK;

}

HRESULT EMB::CUIServer::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	ST_UISVRCONFIG config;
	config.FromString(szIn);
	if (config.nProberType == INVALID_VALUE
		|| config.addrListen.sin_family != AF_INET)
	{
		CFWriteLog(0, TEXT("uisvr config error!"));
		ASSERT(FALSE);
		return FALSE;
	}
	m_cfgUISvr = config;
	return S_OK;

}

HRESULT EMB::CUIServer::UIMessageCallback( CString& taskIn, CString& strRet )
{
	HRESULT hr = E_FAIL;
	ST_EMBXMLMAININFO mainInfo;
	if (GetEmbXmlMainInfo(taskIn, mainInfo))
	{
		if (mainInfo.nType == embxmltype_task)
		{
			if (m_pITaskCommit)
			{
				hr = m_pITaskCommit->SubmitTask(taskIn, strRet);
			}
		}
		else
		{
			if (m_pIUIMsgProcess)
			{
				hr =  m_pIUIMsgProcess->OnUIMessage(taskIn, strRet);
			}
		}
	}

	return hr;
	
}

HRESULT EMB::CUIServer::OnDispatchNotify( CTaskString& strNotify )
{
	//broadcast to all client
	return	m_Prober.SendToUI(strNotify);
}
