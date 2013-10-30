#include "StdAfx.h"
#include "TaskRiserMgr.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "TxAutoComPtr.h"

#define MAXTASKLIMIT 1000

using namespace EMB;

DWORD __stdcall TaskLoopProc(LPVOID parmIn)
{
	if (parmIn == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	CTaskRiserMgr* pProber = (CTaskRiserMgr*)parmIn;
	pProber->TaskCheckProc();
	return 0;
}

CTaskRiserMgr::CTaskRiserMgr()
{
	m_pTaskDispatcher = NULL;
}

CTaskRiserMgr::~CTaskRiserMgr()
{
}

HRESULT CTaskRiserMgr::OnFirstInit()
{
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\PluginMgr.log");
	GetTxLogMgr()->AddNewLogFile(TASKRISER_LOGKEY, strFile);
	m_nMaxTaskLimit = MAXTASKLIMIT;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	return S_OK;
}

void CTaskRiserMgr::OnFinalRelease()
{
	if (m_pTaskDispatcher)
	{
		m_pTaskDispatcher->Release();
	}
}

HRESULT CTaskRiserMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IPluginManager;
	info.nPlugInType = PluginType_TaskRiser;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT CTaskRiserMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}


HRESULT CTaskRiserMgr::Run_Plugin()
{
	Stop_Plugin();
	m_hTaskCheckProc = CreateThread(NULL, NULL, TaskLoopProc, (LPVOID)this, 0, 0);
	if(m_hTaskCheckProc != NULL)
	{
		return S_OK;
	}
	return S_FALSE;
}

HRESULT CTaskRiserMgr::Stop_Plugin()
{
	SetEvent(m_hQuitEvent);
	if (m_hTaskCheckProc)
	{
		if (WaitForSingleObject(m_hTaskCheckProc, 5000) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hTaskCheckProc, 0);
		}
		m_hTaskCheckProc = NULL;
	}
	m_vTasks.clear();
	ResetEvent(m_hQuitEvent);
	ResetEvent(m_hTaskEvent);

	return S_OK;
}

BOOL CTaskRiserMgr::TaskCheckProc()
{

		while(TRUE)
		{
			DWORD dwWait = TxWaitObjWithQuit(m_hTaskEvent, m_hQuitEvent);
			if (dwWait == WAIT_OBJECT_0)
			{
				//have task
				VECTASKDATAS vTasks;
				PopTask(vTasks);
				for (size_t i = 0; i < vTasks.size(); ++i)
				{
					if (m_pTaskDispatcher)
					{
						CString strRet;
						m_pTaskDispatcher->SubmitTask(vTasks[i].taskDesc, strRet);
					}
				}
			}
			else
			{
				break;
			}
		}

		return TRUE;


}

HRESULT CTaskRiserMgr::TaskCollectCallback( CString& taskIn )
{
	if(taskIn.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_FORMAT;
	}
	if (m_vTasks.size() == m_nMaxTaskLimit)
	{
		CFWriteLog(TASKRISER_LOGKEY, TEXT("task list is full!!"));
		return EMBERR_FULL;
	}

	CAutoLock locTask(&m_lockTask);
	ST_EMBTASKDATA task;
	task.taskDesc = taskIn;
	m_vTasks.push_back(task);
	SetEvent(m_hTaskEvent);
	return S_OK;
}

BOOL CTaskRiserMgr::PopTask( VECTASKDATAS& vTaskOut )
{

		CAutoLock locTask(&m_lockTask);
		vTaskOut = m_vTasks;
		m_vTasks.clear();
		ResetEvent(m_hTaskEvent);
		return TRUE;

}


HRESULT EMB::CTaskRiserMgr::Connect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pTaskDispatcher)
	{
		return S_OK;
	}
	if (m_pTaskDispatcher)
	{
		ASSERT(FALSE);
		Disconnect(m_pTaskDispatcher);
		m_pTaskDispatcher = NULL;
	}
	pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskDispatcher, (LPVOID&) m_pTaskDispatcher);
	if (m_pTaskDispatcher)
	{
		CTxAutoComPtr<IPluginConnectorInterce> pConn;
		m_pTaskDispatcher->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
		pConn->OnConnect(this);
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

HRESULT EMB::CTaskRiserMgr::Disconnect( ITxUnkown* pInterfaceIn )
{
	//find if have connected this interface
	HRESULT hr = S_FALSE;

	BOOL bFound = FALSE;
	if (m_pTaskDispatcher == pInterfaceIn)
	{
		bFound = TRUE;
	}
	else
	{
		CTxAutoComPtr<IPluginTaskDispatch> pDispath;
		if (pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskDispatcher, (LPVOID&)(*&pDispath))
			&& pDispath == m_pTaskDispatcher)
		{
			bFound = TRUE;
		}
	}
	
	if (bFound)
	{
		//found it, disconnect the interface
		CTxAutoComPtr<IPluginConnectorInterce> pConn;
		m_pTaskDispatcher->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
		pConn->OnDisconnect(this);
		m_pTaskDispatcher->Release();
		hr =S_OK;
	}

	
	return S_OK;
}

HRESULT EMB::CTaskRiserMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	ASSERT(FALSE);
	return E_FAIL;
}

HRESULT EMB::CTaskRiserMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	ASSERT(FALSE);
	return E_FAIL;
}
