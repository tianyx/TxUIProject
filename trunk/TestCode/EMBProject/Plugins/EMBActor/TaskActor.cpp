#include "StdAfx.h"
#include "TaskActor.h"
#include "TxLogManager.h"
#include "io.h"
#include "StrConvert.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "EMBDocDef.h"
using namespace EMB;
CTaskActor::CTaskActor(void)
{
	m_bRuning = FALSE;
	m_pExcutorMgr = CExcutorMgr::GetExcutorMgr();
	ASSERT(m_pExcutorMgr);
}

CTaskActor::~CTaskActor(void)
{
	CExcutorMgr::Release();
	g_pPluginInstane = NULL;
}

HRESULT CTaskActor::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IPluginManager;
	info.nPlugInType = PluginType_Actor;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT CTaskActor::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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
	if (guidIn == GuidEMBPlugin_ITaskCommit)
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

HRESULT CTaskActor::Run_Plugin()
{
	Stop_Plugin();
	SOCKADDR_IN addrLocal;
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(0);
	addrLocal.sin_addr.S_un.S_addr = inet_addr( INADDR_ANY );

	m_actorconnMain.SetScokAddr(&m_ActRegInfo.addrMain, &addrLocal);
	m_actorconnSlave.SetScokAddr(&m_ActRegInfo.addrSlave, &addrLocal);
	m_pExcutorMgr->Init(m_ActRegInfo.strExcPath);
	HRESULT hr = m_actorconnMain.Run();
	MUSTBESOK(hr);
	hr = m_actorconnSlave.Run();
	MUSTBESOK(hr);
	hr = m_pExcutorMgr->Run();
	MUSTBESOK(hr);
	m_bRuning = TRUE;
	return S_OK;
}

HRESULT CTaskActor::Stop_Plugin()
{
	m_bRuning = FALSE;
	m_actorconnMain.Stop();
	m_actorconnSlave.Stop();
	m_pExcutorMgr->Stop();

	return S_OK;
}

HRESULT EMB::CTaskActor::OnActorConnectorMsg(CString& strInfo, CString& strRet)
{
	//getxmltype
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(strInfo, mainInfo);
	if (mainInfo.nType == embxmltype_task)
	{
		TXGUID guid = String2Guid(mainInfo.guid);
		if (guid == GUID_NULL)
		{
			ASSERT(FALSE);
			return EMBERR_INVALIDARG;
		}
		//dispatched a task
		EXCUTORID excId = m_pExcutorMgr->CreateNewExcutor();
		if (excId != INVALID_ID)
		{
			//save task to it
			ST_TASKINACTOR task;
			task.strTask = strInfo;
			task.nCurrStep = 0;
			task.nState = embtaskstate_dispatching;
			CAutoLock lock(&m_csmapLock);
			if(m_mapTaskinActor.find(guid) == m_mapTaskinActor.end())
			{
				//add to map and wait for embxmltype_excOnIdle message of actor
				m_mapTaskinActor[guid] = task;
				m_mapExcTask[excId] = guid;
			}
			else
			{
				ASSERT(FALSE);
			}
		}

		//process task
	}
	else if (mainInfo.nType == embxmltype_taskReport)
	{
		//report task progress
		ST_TASKREPORT report;
		TXGUID guid = String2Guid(report.strGuid);
		report.FromString(strInfo);
		if (guid != GUID_NULL)
		{
			CAutoLock lock(&m_csmapLock);
			MAPTASKINACTOR::iterator itf = m_mapTaskinActor.find(guid);
			if (itf != m_mapTaskinActor.end())
			{
				ST_TASKINACTOR& taskInfoRef = itf->second;
				//write the info
				if (taskInfoRef.excId != INVALID_ID)
				{
					report.excutorId = taskInfoRef.excId;
					report.actorId = m_ActRegInfo.guid;
					report.nPercent = taskInfoRef.nPercent;
					report.nStep = taskInfoRef.nCurrStep;
					report.nState = taskInfoRef.nState;
				}
				else
				{
					report.nState = embtaskstate_dispatching;
				}
			}
			else
			{
				//no task return error state
				report.nState = embtaskstate_error;
			}

			//report
			report.ToString(strRet);
		}
	}
	else if (mainInfo.nType == embxmltype_svrActive)
	{
		//report actor state 
		ST_SVRACTIVEINFO activeInfo;
		activeInfo.FromString(strInfo);
		if (activeInfo.nActive == embSvrState_active)
		{
			if (activeInfo.nMaster == embSvrType_master)
			{
				m_nActiveConn = 1;
			}
			else if (activeInfo.nMaster == embSvrType_slave)
			{
				m_nActiveConn = 2;
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else if (activeInfo.nActive == embSvrState_deactive)
		{
			if (activeInfo.nMaster == embSvrType_master
				&& m_nActiveConn == 1)
			{
				m_nActiveConn = 0;
			}
			else if (activeInfo.nMaster == embSvrType_slave
				&& m_nActiveConn == 2)
			{
				m_nActiveConn = 0;
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (mainInfo.nType == embxmltype_actorState)
	{
		//active svr changed;
		ST_ACTORSTATE actorInfo;
		actorInfo.actorId = m_ActRegInfo.guid;
		actorInfo.nActorLevel = m_ActRegInfo.nActorLevel;
		actorInfo.nConnState = embConnState_ok;
		actorInfo.nCpuUsage = 10;//getcpuusage
		actorInfo.nDiscUsage = 10;
		actorInfo.nMemUsage = 10;
		actorInfo.ToString(strRet);
		
	}
	else
	{

	}

	return S_OK;
}

void EMB::CTaskActor::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCTaskRiserMgr::OnFinalRelease() ");
	delete this;
}

HRESULT EMB::CTaskActor::OnFirstInit()
{
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskActor.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKACTOR, strFile);

	return S_OK;
}

HRESULT EMB::CTaskActor::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	//return all
	m_ActRegInfo.ToString(szOut);
	return S_OK;
}

HRESULT EMB::CTaskActor::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	ST_EMBRET ret;
	if (m_bRuning)
	{
		ret.nRetVal = EMBERR_NOTREADY;
	}
	else
	{
		ST_ACTORREG regIn;
		regIn.FromString(szIn);

		//check valid
		if (regIn.guid  < 0
			|| regIn.addrMain.sin_family  != AF_INET
			|| regIn.addrSlave.sin_family != AF_INET
			|| regIn.nExcutorMinId >= regIn.nExcutorMaxId
			|| _access(regIn.strExcPath, 0) ==-1)
		{
			ret.nRetVal = EMBERR_INVALIDARG;
		}
		else
		{
			m_ActRegInfo = regIn;
		}
	}

	ret.ToString(szOut);
	return ret.nRetVal;
}

HRESULT EMB::CTaskActor::OnExcutorMessage( const EXCUTORID excutorId, CString& szInfoIn )
{
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(szInfoIn, mainInfo);
	if (mainInfo.nType == embxmltype_excOnIdle)
	{
		OnExcutorIdle(excutorId);
	}
	else if (mainInfo.nType == embxmltype_taskReport)
	{
		//excutor report
	}
	else 
	{
		ASSERT(FALSE);
	}

	return S_OK;
}

HRESULT EMB::CTaskActor::OnExcutorExit( const EXCUTORID excutorId )
{
	HRESULT hr = S_OK;
	CAutoLock lock(&m_csmapLock);
	MAPEXCTASKS::iterator itf = m_mapExcTask.find(excutorId);
	if (itf != m_mapExcTask.end())
	{
		//check task state that assigned to the excutor
		MAPTASKINACTOR::iterator itftask = m_mapTaskinActor.find(itf->second);
		if (itftask != m_mapTaskinActor.end())
		{
			//
			ST_TASKINACTOR& taskInfoRef = itftask->second;
			if (taskInfoRef.nState == embtaskstate_finished)
			{
				//report task state to dispatcher
				ReportTaskState(taskInfoRef);
				//save to the recent deque;
				m_dqRecentFinishedTasks.push_back(itftask->first);
				//remove the task
				m_mapTaskinActor.erase(itftask);
			}
			else
			{
				BOOL bReSuc = FALSE;
				++taskInfoRef.nRetry;
				if (taskInfoRef.nRetry < nfgRetryMax)
				{
					//assign to a new actor
					EXCUTORID newExcId = m_pExcutorMgr->CreateNewExcutor();
					if (newExcId != INVALID_ID)
					{
						//change excid and state
						taskInfoRef.nState = embtaskstate_dispatching;
						taskInfoRef.excId = INVALID_ID;
						//change exc task map
						m_mapExcTask[newExcId] = taskInfoRef.taskGuid;
						bReSuc = TRUE;
					}
					else
					{
						ASSERT(FALSE);
					}
				}
				else
				{
					ASSERT(FALSE);
				}

				if (!bReSuc)
				{
					taskInfoRef.nState = embtaskstate_error;
					ReportTaskState(taskInfoRef);
					m_mapTaskinActor.erase(itftask);
				}
				
			}
		}

		//whatever delete the excid
		m_mapExcTask.erase(itf);

	}

	return S_OK;
}

BOOL EMB::CTaskActor::ReportTaskState( ST_TASKINACTOR& infoIn )
{
	ST_TASKREPORT report;
	report.actorId = m_ActRegInfo.guid;
	report.excutorId = infoIn.excId;
	report.nState = infoIn.nState;
	report.nPercent = infoIn.nPercent;
	report.nStep = infoIn.nCurrStep;
	report.strGuid = Guid2String(infoIn.taskGuid);
	CString strRet;
	report.ToString(strRet);
	CActorConnector* pActor = GetActiveActorConnector();
	if (pActor)
	{
		pActor->SendtoDispatcher(strRet);
	}
	return TRUE;
}

BOOL EMB::CTaskActor::OnExcutorIdle( const EXCUTORID excutorId )
{
	//find task assigned to the excutor;
	CString strTask;
	int nStartStep = 0;
	{
		CAutoLock lock(&m_csmapLock);
		MAPEXCTASKS::iterator itf = m_mapExcTask.find(excutorId);
		if (itf != m_mapExcTask.end())
		{
			MAPTASKINACTOR::iterator itftask = m_mapTaskinActor.find(itf->second);
			if (itftask != m_mapTaskinActor.end())
			{
				//check not assigned;
				ST_TASKINACTOR& taskInfoRef = itftask->second;
				if (taskInfoRef.excId == INVALID_ID)
				{
					strTask = taskInfoRef.strTask;
					nStartStep = taskInfoRef.nCurrStep;
					taskInfoRef.tmLastReport = time(NULL);
					taskInfoRef.excId = excutorId;
				}

			}
		}
	}

	if(!strTask.IsEmpty())
	{
		//send task to the excutor
		if (nStartStep > 0)
		{
			//rewrite the task, add startstep attribute to taskbasic
			CTxParamString txParam(strTask);
			txParam.GoIntoKey(EK_MAIN);
			CTxStrConvert val;
			val.SetVal(nStartStep);
			txParam.SetAttribVal(EK_TASKBASIC, EV_TBSTARTSTEP, val);
			txParam.UpdateData();
			strTask = txParam;
		}
		else
		{
			m_pExcutorMgr->SendToExcutor(excutorId, strTask);

		}
	}

	return TRUE;
}
