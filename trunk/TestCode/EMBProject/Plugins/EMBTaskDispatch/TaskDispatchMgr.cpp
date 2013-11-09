#include "StdAfx.h"
#include "TaskDispatchMgr.h"
#include "IEMBBaseInterface.h"
#include "EMBCommonFunc.h"
#include "EMBDocDef.h"
#include "TxAutoComPtr.h"
#include "TxLogManager.h"
using namespace EMB;

//////////////////////////////////////////////////////////////////////////
DWORD __stdcall  TDFTaskProc(LPVOID paramIn)
{
	CTaskDispatchMgr* pMgr = (CTaskDispatchMgr*)paramIn;
	if (pMgr)
	{
		pMgr->LoopProcFileTask();
	}

	return 0;
}

DWORD __stdcall TDCheckProc(LPVOID paramIn)
{
	CTaskDispatchMgr* pMgr = (CTaskDispatchMgr*)paramIn;
	if (pMgr)
	{
		pMgr->LoopProcCheck();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CTaskDispatchMgr::CTaskDispatchMgr(void)
{
	m_actHolder.SetActorCallbackInterface(this);
	m_hEventQuitLoop = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_pIStorage = NULL;
}

CTaskDispatchMgr::~CTaskDispatchMgr(void)
{
	g_pPluginInstane = NULL;
}

HRESULT EMB::CTaskDispatchMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PTaskDispatch;
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
	ReleaseTxLogMgr();
	TRACE("\nCTaskDispatchMgr::OnFinalRelease() ");

	delete this;
}

HRESULT EMB::CTaskDispatchMgr::OnFirstInit()
{
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskDispath.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKDISPATCH, strFile);

	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Run_Plugin()
{
	Stop_Plugin();
	m_hThdFtask = CreateThread(NULL, 0, TDFTaskProc, (LPVOID)this, 0, 0);
	ASSERT(m_hThdFtask);
	m_hThdCheck = CreateThread(NULL, 0, TDCheckProc,  (LPVOID)this, 0, 0);
	ASSERT(m_hThdCheck);
	if (m_hThdCheck && m_hThdFtask)
	{
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

HRESULT EMB::CTaskDispatchMgr::Stop_Plugin()
{
	SetEvent(m_hEventQuitLoop);
	if (m_hThdCheck)
	{
		if(WAIT_OBJECT_0 != WaitForSingleObject(m_hThdCheck, 5000))
		{
			ASSERT(FALSE);
			TerminateThread(m_hThdCheck, 0);
		}

		m_hThdCheck = NULL;
	}
	
	if (m_hThdFtask)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_hThdFtask, 5000))
		{
			ASSERT(FALSE);
			TerminateThread(m_hThdFtask, 0);
		}

		m_hThdFtask = NULL;
	}

	ResetEvent(m_hEventQuitLoop);

	//finished
	m_actHolder.Stop();
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	ST_AUTOUPDATEPARAM<ST_EMBRET> stRet(szRet);
	int& hr = stRet.nRetVal = S_OK;
	CString strHeader;
	GetXmlFirstNode(szTaskIn, strHeader);
	if (strHeader.IsEmpty())
	{
		ASSERT(FALSE);
		hr = EMBERR_INVALIDARG;
		return hr;
	}

	CTxParamString txParam(strHeader);
	ST_EDOCMAINHEADER header;
	header.FromString(strHeader);
	if (header.nVer != EMBVER)
	{
		ASSERT(FALSE);
		hr = EMBERR_INVALIDARG;
		return hr;
	}

	if (header.nType == EMBTYPE_FILETASK)
	{
		//it's a file task, put into the task pool;
		//getbasic info
		txParam.GoIntoKey(EK_TASKBASIC);
		CTxParamString tmpBaisc("");
		txParam.GetSubNodeString(EPATH_TASKBASIC, tmpBaisc);
		ST_FILETASKDATA ftask;
		ftask.taskBasic.FromString(tmpBaisc);
		if(!ftask.taskBasic.strGuid.IsEmpty())
		{
			hr = EMBERR_INVALIDARG;
			return hr;
		}

		return S_OK;
	}

	

	
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Connect( ITxUnkown* pInterfaceIn )
{
	//check if have storage interface
	IPluginStorageInterface * pIStorage = NULL;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IStorage, (LPVOID&)pIStorage);
	if (pIStorage && m_pIStorage == NULL)
	{
		m_pIStorage = pIStorage;
	}
	
	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
	ASSERT(pConn);
	pConn->OnConnect(this);
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::Disconnect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pIStorage)
	{
		m_pIStorage->Release();
	}
	CTxAutoComPtr<IPluginConnectorInterce> pConn;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
	ASSERT(pConn);
	pConn->OnDisconnect(this);
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	IPluginStorageInterface * pIStorage = NULL;
	pInterfaceIn->QueryInterface(GuidEMBPlugin_IStorage, (LPVOID&)pIStorage);
	if (pIStorage)
	{	
		if (m_pIStorage)
		{
			ASSERT(FALSE);
			pIStorage->Release();
		}
		else
		{
			m_pIStorage = pIStorage;
		}
	}
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pIStorage)
	{
		m_pIStorage->Release();
		m_pIStorage = NULL;
	}
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorConnect( const ACTORID& szActorGuid )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorDisConnect( const ACTORID& szActorGuid )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorReportInfo( const ACTORID&szActorGuid, CString& szActorInfoIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::OnActorDispatchTask( const ACTORID& szActorGuid, CString& szActorInfoIn )
{
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::CommitFileTask( TXGUID& guidIn, ST_FILETASKDATA& taskIn )
{
	CAutoLock lock(&m_csFTask);
	MAPFILETASKS::iterator itf = m_mapTasks.find(guidIn);
	if (itf != m_mapTasks.end())
	{
		//already found
		return EMBERR_EXISTED;
	}

	m_mapTasks[guidIn] = taskIn;
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::GetSelfState( ST_SVRLIVEINFO& infoOut )
{
	infoOut.nActive = m_nActived;
	infoOut.nMaster = m_nMaster;
	infoOut.nsvrId = m_nSvrId;
	return S_OK;
}

BOOL EMB::CTaskDispatchMgr::LoopProcFileTask()
{
	while (WaitForSingleObject(m_hEventQuitLoop, 1000) != WAIT_OBJECT_0)
	{
		VECTXIDS vIdToRemove;
		CAutoLock lock(&m_csFTask);
		MAPFILETASKS::iterator itb = m_mapTasks.begin();
		MAPFILETASKS::iterator ite = m_mapTasks.end();
		for (; itb != ite; ++itb)
		{
			//check task
			BOOL bRemove = ExamTask(itb->second);
			//if bRemove,remove it
			if (bRemove)
			{
				vIdToRemove.push_back(itb->first);
			}
		}

		if (vIdToRemove.size() > 0)
		{
			for (size_t i = 0; i < vIdToRemove.size(); ++i)
			{
				m_mapTasks.erase(vIdToRemove[i]);
			}
		}
	}
	return TRUE;
}

BOOL EMB::CTaskDispatchMgr::LoopProcCheck()
{
	return TRUE;
}

BOOL EMB::CTaskDispatchMgr::ExamTask( ST_FILETASKDATA& taskIn )
{
	return TRUE;
}

