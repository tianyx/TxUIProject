#include "StdAfx.h"
#include "TaskDispatchMgr.h"
#include "IEMBBaseInterface.h"
#include "EMBCommonFunc.h"
#include "EMBDocDef.h"
#include "TxAutoComPtr.h"
#include "TxLogManager.h"
#include "EmbStructDef.h"
#include "TxDBLogManager.h"
#include "DBLogFunc.h"
using namespace EMB;
#define MAXACTORPALOAD 0x00FFFFFF
#define ACTORSTATETIMEOUT 10
//////////////////////////////////////////////////////////////////////////
/*
*Description：任务分配线程
*Input Param：
*		paramIn：传入指针
*Return Param：
*History：
*/
DWORD __stdcall  TDFTaskProc(LPVOID paramIn)
{
	CTaskDispatchMgr* pMgr = (CTaskDispatchMgr*)paramIn;
	if (pMgr)
	{
		pMgr->LoopProcFileTask();
	}

	return 0;
}

/*
*Description：其他任务线程
*Input Param：
*		paramIn：传入指针
*Return Param：
*History：
*/
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
	m_bCheckBackSvr = FALSE;
	m_pIbackSvr = NULL;
	m_hThdCheck = NULL;
	m_hThdFtask = NULL;
	m_nMaster = embSvrType_master;
	m_nSvrId = INVALID_ID;
	m_bDBLogEnabled = FALSE;
	//task check param
	nfgTaskPoolSizeMax = 9999;
	nfgTaskDispatchCD = 60;
	nfgTaskReDispatchMaxCount = 5;
	nfgTaskReportIntervalMax = 20;
	nfgTaskCheckProgressIntervalMax = 5;
	nfgTaskLostTimeOutMax = 60;

	nfgActorLostTimeOutMax = 30;
	nfgActorCheckInterval = 8;
	nfgActorStateOutdate = 10;
	nfgActorAssignTaskCD = 16;

	nfgCpuWeight = 1;
	nfgMemWeight = 1;
	nfgDiskIOWeight =1;
	nfgNetIOWeight = 1;

	nfgMaxActorLoad = 100*(nfgCpuWeight + nfgMemWeight + nfgDiskIOWeight +nfgNetIOWeight)*4/5;
	nfgLowActorLoad = 100*(nfgCpuWeight + nfgMemWeight + nfgDiskIOWeight +nfgNetIOWeight)/5;

}

CTaskDispatchMgr::~CTaskDispatchMgr(void)
{
	g_pPluginInstane = NULL;
}

/*
*Description：查询接收插件信息，对外提供插件管理者管理功能
*Input Param：
*Return Param：
*History：
*/
HRESULT EMB::CTaskDispatchMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PTaskDispatch;
	info.nPlugInType = PluginType_Dispatcher;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

/*
*Description：查询接收插件信息，对外提供插件管理者管理功能
*Input Param：
*Return Param：
*History：
*/
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
	else if (guidIn == GuidEMBPlugin_IConfig)
	{
		pInterfaceOut = dynamic_cast<IPluginConfigInterface*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_IUIMessageProcess)
	{
		pInterfaceOut = dynamic_cast<IUIMessageProcessInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (GuidEMBServer_IUI == guidIn)
	{
		pInterfaceOut = dynamic_cast<IServerUI*>(this);
		AddRef();
		return S_OK;
	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

/*
*Description：释放函数
*Input Param：
*Return Param：
*History：
*/
void EMB::CTaskDispatchMgr::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	CTxADODBMgr::ReleaseDBMgr();
	TRACE("\nCTaskDispatchMgr::OnFinalRelease() ");

	delete this;
}

/*
*Description：初始化函数
*Input Param：
*Return Param：
*History：
*/
HRESULT EMB::CTaskDispatchMgr::OnFirstInit()
{
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskDispath.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKDISPATCH, strFile);

	return S_OK;
}

/*
*Description：启动插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CTaskDispatchMgr::Run_Plugin()
{
	Stop_Plugin();

	m_nSvrCode = (m_nSvrId*10)+m_nMaster;
	if (!m_config.strDBLogConn.IsEmpty())
	{
		DWORD dwLogKey = CTxDBLogManager::GetTxDBLogMgr()->AddNewDBConn(DBLOGKEY_TASKDISPATCH, DBLOGDBKEY_TASKDISPATCH, m_config.strDBLogConn);
		if (dwLogKey == 0)
		{
			CFWriteLog(0, TEXT("dblog init failed, conn = %s"), m_config.strDBLogConn);
			CTxDBLogManager::ReleaseTxDBLogMgr();
		}
		else
		{
			m_bDBLogEnabled = TRUE;
			CFWriteLog(0, TEXT("dblog inited"));

		}
	}
	else
	{
		CFWriteLog(0, TEXT("dblog connection string is null, db log not enabled"));
	}

	WriteDBLog_EMB(embdblogtype_main, TEXT("server starting"));

	SOCKADDR_IN addrHolder;
	addrHolder = GetAddrFromStr(m_config.strIpActorHolder);
	SOCKADDR_IN addrMaster;
	addrMaster = GetAddrFromStr(m_config.strIpMaster);
	SOCKADDR_IN addrLocal;
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port =  htons(0);
	addrLocal.sin_addr.S_un.S_addr =htonl(INADDR_ANY);
	if (addrHolder.sin_family != AF_INET || addrMaster.sin_family != AF_INET)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_actHolder.SetScokAddr(&addrHolder, &addrHolder);
	m_actHolder.SetSvrState(m_nActive, m_nMaster);
	m_actHolder.Run();
	CMBCBaseObj* pRemote = NULL;
	if(m_nMaster == embSvrType_master)
	{
		CMasterHeartBeat* pObj = new CMasterHeartBeat;
		pObj->SetLiveCallback(this);
		pRemote = pObj;
		pRemote->SetScokAddr(&addrMaster, &addrMaster);

	}
	else
	{
		CSlaveHeartBeat* pObj = new CSlaveHeartBeat;
		pObj->SetLiveCallback(this);
		pRemote = pObj;
		pRemote->SetScokAddr(&addrMaster, &addrLocal);
	}
	pRemote->Run();
	ASSERT(m_pIbackSvr == NULL);
	m_pIbackSvr = dynamic_cast<IRemoteSvrLiveInterface*>(pRemote) ;
	ASSERT(m_pIbackSvr);

	// 启动任务分配线程
	m_bCheckBackSvr = TRUE;
	m_hThdCheck = CreateThread(NULL, 0, TDCheckProc,  (LPVOID)this, 0, 0);
	ASSERT(m_hThdCheck);

	// 启动主备监听线程
	m_hThdFtask = CreateThread(NULL, 0, TDFTaskProc, (LPVOID)this, 0, 0);
	ASSERT(m_hThdFtask);

	if (m_hThdCheck && m_hThdFtask)
	{
		WriteDBLog_EMB(embdblogtype_main, TEXT("server start successfully"));

		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

/*
*Description：停止插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CTaskDispatchMgr::Stop_Plugin()
{
	//退出主备心跳、分配任务线程
	SetEvent(m_hEventQuitLoop);
	if (m_hThdCheck)
	{
		if(WAIT_OBJECT_0 != WaitForSingleObject(m_hThdCheck, INFINITE))
		{
			ASSERT(FALSE);
			TerminateThread(m_hThdCheck, 0);
		}

		m_hThdCheck = NULL;
	}
	
	if (m_hThdFtask)
	{
		if (WAIT_OBJECT_0 != WaitForSingleObject(m_hThdFtask, INFINITE))
		{
			ASSERT(FALSE);
			TerminateThread(m_hThdFtask, 0);
		}

		m_hThdFtask = NULL;
	}

	ResetEvent(m_hEventQuitLoop);

	ResetUnionTask();

	// 清空任务和设备列表
	m_mapTasks.clear();
	m_mapActors.clear();
	//finished
	// 停止设备控制
	SetActive(embSvrState_deactive);
	m_actHolder.Stop();
	// 断开远程连接
	if (m_pIbackSvr)
	{
		CMBCBaseObj* pRemoteObj = dynamic_cast<CMBCBaseObj*>(m_pIbackSvr) ;
		pRemoteObj->Stop();
		delete pRemoteObj;
		m_pIbackSvr = NULL;
	}
	
	if (m_bDBLogEnabled)
	{
		m_bDBLogEnabled = FALSE;
		CTxDBLogManager::ReleaseTxDBLogMgr();
	}
	return S_OK;
}

/*
*Description：从任务存储容器中领取若干任务，然后提交到任务分配任务列表中
*Input Param：
*		szTaskIn：任务信息
*		szRet   ：返回信息
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CTaskDispatchMgr::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	ST_AUTOUPDATEPARAM<ST_EMBRET> stRet(szRet);
	int& hr = stRet.nRetVal = S_OK;
	CString strHeader;
	//it's a file task, put into the task pool;
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(szTaskIn, mainInfo);
	if (mainInfo.nType != embxmltype_task)
	{
		hr = EMBERR_INVALIDARG;
		ASSERT(FALSE);
		return hr;
	}
	//getbasic info
	ST_FILETASKDATA ftask;
	GetTaskBasicInfo(szTaskIn, ftask.taskBasic);
	TXGUID guid = String2Guid(ftask.taskBasic.strGuid);
	if(ftask.taskBasic.strGuid.IsEmpty() || guid == GUID_NULL)
	{
		hr = EMBERR_INVALIDARG;
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("task format error: %s"), szTaskIn);
		return hr;
	}
	time_t tmNow = time(NULL);
	ftask.taskRunState.guid = guid;
	ftask.taskRunState.nState = embtaskstate_zero;
	ftask.taskRunState.tmLastReport = tmNow;
	ftask.taskRunState.tmLastCheck = tmNow;
	ftask.taskRunState.tmCommit = tmNow;
	ftask.strTask = szTaskIn;
	CAutoLock lock(&m_csFTask);
	if (m_mapTasks.find(guid) == m_mapTasks.end())
	{
		//push it;
		CFWriteLog(0, TEXT("task received %s"), ftask.taskBasic.strGuid);
		m_mapTasks[guid] = ftask;
	}
	else
	{
		CFWriteLog(0, TEXT("task already existed! %s"), ftask.taskBasic.strGuid);
		ASSERT(FALSE);
	}


		return S_OK;
}

/*
* Description：连接任务存储插件
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
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

/*
* Description：断开任务存储插件
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
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

/*
* Description：连接任务存储插件与传入插件
* Input Param：
*		pInterfaceIn：接收插件指针
* Return Param：返回成功或失败
* History：
*/
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

/*
* Description：存储插件与接收插件断开
* Input Param：
*		pInterfaceIn：接收插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pIStorage)
	{
		m_pIStorage->Release();
		m_pIStorage = NULL;
	}
	return S_OK;
}

/*
* Description：连接设备
* Input Param：
*		szActorGuid：ActorID
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::OnActorConnect( const ACTORID& szActorGuid, const CString& strPcName )
{
	CAutoLock lock(&m_csCacheDisActor);
	ST_ACTORSTATE actState;
	actState.actorId = szActorGuid;
	actState.strPcName = strPcName;
	actState.nConnState = embActorConnState_ok;
	actState.tmLastCheck = 0;
	actState.tmLastReport = 0;
	m_mapActors[actState.actorId] = actState;
	CFWriteLog(0, TEXT("actor connected %d"), actState.actorId);

	return S_OK;
}

/*
* Description：断开设备连接
* Input Param：
*		szActorGuid：ActorID
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::OnActorDisConnect( const ACTORID& szActorGuid )
{
	CAutoLock lock(&m_csCacheDisActor);
	ST_ACTDISCONNINFO info;
	info.actorid = szActorGuid;
	info.tmReport = time(NULL);
	m_vCachedDisconnActor.push_back(info);
	CFWriteLog(0, TEXT("actor disconnected %d"), szActorGuid);

	return S_OK;
}

/*
* Description：Actor消息调用回调函数
* Input Param：
*		nActorIdIn    ：ActorID
*		szActorInfoIn ：传入消息
*		szRet         ：传出消息
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::OnActorMessage( const ACTORID& nActorIdIn, CString& szActorInfoIn ,  CString& szRet)
{
	CTxParamString txParam(szActorInfoIn);
	int nXmlType = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
	if (nXmlType == embxmltype_taskReport)
	{
		//是否为任务广播，更新任务状态
		//CFWriteLog(0, TEXT("actor %d report %s"),nActorIdIn, szActorInfoIn);
		//report task percent
		ST_TASKREPORT report;
		report.FromString(szActorInfoIn);
		if (!report.strGuid.IsEmpty())
		{
			UpdateTaskRunState(report);
		}
		else
		{
			CFWriteLog(0, TEXT("recv unknow actor report %s"),szActorInfoIn );
			ASSERT(FALSE);
		}
		
		
	}
	else if (nXmlType == embxmltype_actorState)
	{
		// 是否为执行端状态，更新执行端状态
		ST_ACTORSTATE actState;
		actState.FromString(szActorInfoIn);
		if (actState.actorId != INVALID_ID)
		{
			UpdateActorState(actState);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (nXmlType == embxmltype_excCallback)
	{
		ST_EXCCALLBACKINFO excCallInfo;
		excCallInfo.FromString(szActorInfoIn);
		OnExcCallback(excCallInfo);
	}

	else
	{
		//to be add...
	}
	return S_OK;
}


/*
* Description：添加媒体处理中心执行任务
* Input Param：
*		guidIn    ：任务ID
*		taskIn    ：任务信息
* Return Param：返回成功或失败
* History：
*/
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

/*
* Description：获取自身状态
* Input Param：
*		infoOut    ：返回自身状态存储变量
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::GetSelfState( ST_SVRLIVEINFO& infoOut )
{
	infoOut.nActive = m_nActive;
	infoOut.nMaster = m_nMaster;
	infoOut.nsvrId = m_nSvrId;
	return S_OK;
}

/*
* Description：分配任务函数
* Input Param：
* Return Param：返回成功或失败
* History：
*/
BOOL EMB::CTaskDispatchMgr::LoopProcFileTask()
{
	while (WaitForSingleObject(m_hEventQuitLoop, 1000) != WAIT_OBJECT_0)
	{
		if (GetActive() != embSvrState_active)
		{
			continue;
		}

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
				if (itb->second.taskBasic.nTaskSplitFlag == 1)
				{
					SubSplitTaskRef();
				}
				vIdToRemove.push_back(itb->first);
			}
		}

		if (vIdToRemove.size() > 0)
		{
			for (size_t i = 0; i < vIdToRemove.size(); ++i)
			{
				CFWriteLog(TEXT("task removed! %s"), Guid2String(vIdToRemove[i].guid));
				m_mapTasks.erase(vIdToRemove[i]);
			}
		}
	
	}
	return TRUE;
}

/*
* Description：
* 本函数功能：
* 1-主备心跳
* 2-从存储领取任务
* 3-检查任务状态
* 4-检查Actor状态
* Input Param：
* Return Param：返回成功或失败
* History：
*/
BOOL EMB::CTaskDispatchMgr::LoopProcCheck()
{
	//wait for 4 second;
	if(WaitForSingleObject(m_hEventQuitLoop, 4000) == WAIT_OBJECT_0)
	{
		return TRUE;
	}
	//check other state;
	ConfirmActive();
	int nTaskFetchInterval = 5;
	while(WaitForSingleObject(m_hEventQuitLoop, 1000) != WAIT_OBJECT_0)
	{
		nTaskFetchInterval = (++nTaskFetchInterval)%5;
		// 判断自己是否为Active
		if (GetActive() != embSvrState_active)
		{
			//check backsvr
			ST_SVRLIVEINFO backSvrInfo;
			m_pIbackSvr->GetRemoteSvrState(backSvrInfo);
			if (backSvrInfo.nConnState == embConnState_error)
			{
				SetActive(embSvrState_active);
			}	
		}
		CheckDisconnActorCacheList();
		CheckActorState();
		if (nTaskFetchInterval == 0)
		{
			TryFetchTaskNew();
		}
	}
	return TRUE;
}

/*
* Description：分配任务函数
* Input Param：
*		taskIn ：任务信息
* Return Param：返回成功或失败
* History：
*/
BOOL EMB::CTaskDispatchMgr::ExamTask( ST_FILETASKDATA& taskIn )
{
	BOOL bRet = FALSE;
	time_t tmNow = time(NULL);
	if (taskIn.taskRunState.nState == embtaskstate_finished ||
		taskIn.taskRunState.nState == embtaskstate_error)
	{
		CFWriteLog(0, TEXT("任务[%s]%s,更新到存储"), taskIn.taskRunState.nState == embtaskstate_finished? TEXT("完成"):TEXT("失败"), taskIn.taskBasic.strGuid);
		CString strRemark = taskIn.taskRunState.nState == embtaskstate_error? TEXT("任务失败"):TEXT("任务完成");
		// 切片任务失败时，发送消息：切片任务取消
	    if (taskIn.taskRunState.nSplited && taskIn.taskRunState.nState == embtaskstate_error)
	    {
			CString strCancleMsg;
			strCancleMsg.Format("<edoc_main ver=\"1.0\" type=\"%d\" guid=\"\" MergeGuid=\"%s\"></edoc_main>", 
				embxmltype_taskCancel,
				taskIn.taskBasic.strGuidBase);

			m_actHolder.BroadcastToActor(strCancleMsg);
			// log
// 			OutputDebugString("---切片任务取消");
// 			OutputDebugString(strCancleMsg);
			CFWriteLog(0, "---切片任务取消 %s", strCancleMsg);
			strRemark.Format(TEXT("任务失败, 切片任务取消."),taskIn.taskBasic.strGuidBase);
	    }

		//write task back to storage;
		ST_TASKUPDATE upInfo;
		upInfo.nUpdateType = embtaskupdatetype_finish;
		upInfo.strTaskID = taskIn.taskBasic.strGuid;
		upInfo.guid = taskIn.taskRunState.guid;
		upInfo.data_end.dispatchid = m_nSvrId;
		upInfo.data_end.actorid = taskIn.taskRunState.actorId;
		upInfo.data_end.excutorid = taskIn.taskRunState.excId;
		upInfo.data_end.nEndState = taskIn.taskRunState.nState;
		CString strUpInfo;
		upInfo.ToString(strUpInfo);
		m_pIStorage->UpdateTaskToStorage(m_nSvrId, strUpInfo);
		CFUpdateTaskRunTime(DBLOGKEY_TASKDISPATCH, taskIn.taskBasic.strGuid,0, tmNow);

		CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, taskIn.taskBasic.strGuid, taskIn.taskRunState.nState, strRemark);
		bRet = TRUE;		
	
	}
	else if (taskIn.taskRunState.nState == embtaskstate_zero)
	{
		BOOL bTryDispatch = FALSE;
		if (taskIn.taskBasic.nTaskSplitFlag == 1)
		{
			//check if dispatched split task is full
			int nDispedCount = GetDispatchedSplitTaskCount();
			if (nDispedCount < m_config.nfgSplitTaskParallelRunMax)
			{
				//get idle actor
				vector<ACTORID> vActors;
				int nTotalActor = GetTotalActors();
				GetIdleActors(TEXT(""), taskIn.taskBasic.nPriority, vActors);
				if (vActors.size() >= m_config.nfgMinTaskSplitActorCount
					||vActors.size() == nTotalActor)
				{
					//try dispath task
					bTryDispatch = TRUE;
				}
			}
			else
			{
				//wait for next 
			}
		}
		else
		{
			bTryDispatch = TRUE;
		}

		if (bTryDispatch)
		{
			ACTORID actid = GetFirstIdleActor(taskIn.taskBasic.nFixActor, taskIn.taskBasic.nPriority);
			if (actid != INVALID_ID)
			{
				HRESULT hr = m_actHolder.SendToActor(actid, taskIn.strTask);
				if (hr == S_OK)
				{
					taskIn.taskRunState.actorId = actid;
					taskIn.taskRunState.nState = embtaskstate_dispatching;
					taskIn.taskRunState.tmLastCheck = tmNow;
					SetActorInCD(actid);
					CFWriteLog(0, TEXT("任务[%s]分配到 actor %d"), taskIn.taskBasic.strGuid, actid);

					m_pIStorage->UpdateActorID(taskIn.taskBasic.strGuid, actid);
					CFUpdateTaskRunTime(DBLOGKEY_TASKDISPATCH, taskIn.taskBasic.strGuid,1, tmNow);


					CString strRemark;
					strRemark.Format(TEXT("任务分配到 actor %d"), actid);
					CFWriteLog(0,strRemark);
					CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, taskIn.taskBasic.strGuid, embtaskstate_dispatched,  strRemark);
				}
				else
				{
					ASSERT(FALSE);
					//waiting for next loop 
				}
			}
			else
			{
				//CFWriteLog("no idle actor wait next exam task");
			}
		}
	
	}
	else if (taskIn.taskRunState.nState == embtaskstate_dispatching)
	{
		CTimeSpan tmSpan(tmNow - taskIn.taskRunState.tmLastCheck);
		if (tmSpan.GetTotalSeconds() > nfgTaskDispatchCD)
		{
			if (taskIn.taskRunState.nRetry < nfgTaskReDispatchMaxCount)
			{
				//timeout, reset state 
				CFWriteLog(0, TEXT("task dispatch time out %s"), taskIn.taskBasic.strGuid);
				taskIn.taskRunState.nState = embtaskstate_zero;
				taskIn.taskRunState.tmLastCheck = tmNow;
				taskIn.taskRunState.nRetry++;
			}
			else
			{
				//
				taskIn.taskRunState.nState = embtaskstate_error;
				CString strRemark;
				strRemark.Format(TEXT("分配任务次数达到 %d, 任务分配失败"), nfgTaskReDispatchMaxCount);
				CFWriteLog(0,strRemark);
				CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, taskIn.taskBasic.strGuid, embtaskstate_error,  strRemark);

				bRet = FALSE;
			}
		}
	}
	else if (taskIn.taskRunState.nState == embtaskstate_dispatched)
	{
		//check state
		if (taskIn.taskRunState.nPercent >= 100 && taskIn.taskRunState.nCurrStep == (int)(taskIn.taskBasic.vSubTask.size()-1))
		{
			taskIn.taskRunState.nState = embtaskstate_finished;
			CFWriteLog(0, TEXT("task %s finished!"), taskIn.taskBasic.strGuid);
			bRet = FALSE;
		}
		else
		{
			CTimeSpan tmSpan(tmNow - taskIn.taskRunState.tmLastReport);
			if (tmSpan.GetTotalSeconds() > nfgTaskLostTimeOutMax)
			{
				//task lost retset
				taskIn.taskRunState.nState = embtaskstate_error;
				CString strRemark;
				strRemark.Format(TEXT("actor %s 长时间未响应, 任务标记为失败"),taskIn.taskBasic.strGuid);
				CFWriteLog(0,strRemark);
				CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, taskIn.taskBasic.strGuid, embtaskstate_error,  strRemark);

			}
			else
			{
				if (tmSpan.GetTotalSeconds() > nfgTaskReportIntervalMax)
				{
					CTimeSpan tmSpan2(tmNow - taskIn.taskRunState.tmLastCheck);
					if (tmSpan2.GetTotalSeconds() > nfgTaskCheckProgressIntervalMax)
					{
						//check it now
						if (taskIn.taskRunState.actorId != INVALID_ID)
						{
							taskIn.taskRunState.tmLastCheck = tmNow;
							CString strMsg;
							strMsg.Format(EDOC_TASKHEADERFMT, embxmltype_taskReport, taskIn.taskBasic.strGuid);
							HRESULT hr = m_actHolder.SendToActor(taskIn.taskRunState.actorId, strMsg);
							if (hr != S_OK)
							{
								ASSERT(FALSE);
							}
						}
						else
						{
							if (tmSpan2.GetTotalSeconds() > nfgTaskCheckProgressIntervalMax*2)
							{
								//task may not assigned
								taskIn.taskRunState.nState = embtaskstate_zero;
								taskIn.taskRunState.tmLastCheck = tmNow;
								CString strRemark;
								strRemark.Format(TEXT("actor id[%d] 无效 并且 任务长时间没有actor汇报,任务被重置"), taskIn.taskRunState.actorId);
								CFWriteLog(0,strRemark);
								CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, taskIn.taskBasic.strGuid, embtaskstate_error,  strRemark);

							}
							else
							{
								//may be switched active now, wait for actor report 

							}
						}


					}
				}
			}
		}
		

		
		
	}
	else
	{
		//
		ASSERT(FALSE);
	}

	return bRet;
}

/*
* Description：生成拆分任务XML
* Input Param：
*		strTaskIn：原始任务XML
*       nStart: 分成几个模块
*       nSize：拆分个数
* Return Param：返回XML
* History：
*/
CString EMB::CTaskDispatchMgr::CreateSplitTaskXml(CString strTaskIn,int nStart,int nSize, ST_FILETASKDATA& subTask)
{
	CString strOut = "";
	// check nStart
	if (nStart < 1 || nStart > nSize)
	{
		return strOut;
	}
	// -------------------------------------------

	CTxStrConvert strVal;

	//写到FCVSTask层
	CTxParamString txParam1(strTaskIn);
	txParam1.GoIntoKey(EK_MAIN);
	txParam1.GoIntoKey("step1");
	txParam1.GoIntoKey("FCVS");
	txParam1.GoIntoKey("FCVSTask");

	strVal.SetVal(nStart);
	txParam1.SetAttribVal(NULL, TEXT("nSectionID"), strVal);

	strVal.SetVal(nSize);
	txParam1.SetAttribVal(NULL, TEXT("nTotalSectionCount"), strVal);

	txParam1.UpdateData();
	strOut = txParam1;

	if (1 == nStart) // 第1个任务：负责合并
	{
		strOut = CreateCombinedTaskXml(strOut, nSize);
	}
	else
	{
		// 切片任务，信息中需要使用合并任务标识MergeGuid
		CTxParamString txMergeParam(strOut);
		txMergeParam.GoIntoKey(EK_MAIN);

		CString strMergeGuid = txMergeParam.GetAttribVal(NULL, "guid").GetAsString();
		// new guid
		GUID   newGuid;
		::CoCreateGuid(&newGuid);
		// -------------------
		strVal.SetVal(Guid2String(newGuid)); // 新的guid
		txMergeParam.SetAttribVal(NULL, "guid", strVal);
		strVal.SetVal(strMergeGuid);
		txMergeParam.SetAttribVal(NULL, "MergeGuid", strVal);

		// basic guid
		txMergeParam.GoIntoKey(EK_TASKBASIC);
		strVal.SetVal(Guid2String(newGuid));
		txMergeParam.SetAttribVal(NULL, "guid", strVal);

		// update xml
		txMergeParam.UpdateData();

		strOut = txMergeParam;

		// log-----------------------------------------------
		CString strLog;
		strLog.Format("--切片%d %s", nStart, strOut);
		OutputDebugString(strLog);
		CFWriteLog(0, TEXT("%s"), strLog);

		subTask.strTask = strOut;
		subTask.taskBasic.strGuid = Guid2String(newGuid);
		subTask.taskBasic.strGuidBase = strMergeGuid;
	}

	return strOut;
}

/*
* Description：生成合并任务XML
* Input Param：
*		strTaskIn：原始任务XML
*       nSize：拆分个数
* Return Param：返回XML
* History：
*/
CString EMB::CTaskDispatchMgr::CreateCombinedTaskXml(CString strTaskIn,int nSize)
{
	CString strOut = "";

	CTxParamString txParam(strTaskIn);
	txParam.GoIntoKey(EK_MAIN);
	txParam.GoIntoKey(EK_TASKBASIC);

	CTxStrConvert strVal;

	strVal.SetVal("step1,step2");
	txParam.SetAttribVal(NULL, "excList", strVal);

	// get filename
	CTxParamString txTem(strTaskIn);
	txTem.GoIntoKey(EK_MAIN);
	txTem.GoIntoKey("step1");
	txTem.GoIntoKey("FCVS");
	txTem.GoIntoKey("FCVSTask");
	CString strFilePath = txTem.GetAttribVal(NULL, "FilePath").GetAsString();
	CString strFileName = txTem.GetAttribVal(NULL, "FileName").GetAsString();

	// step2
	/*<step2 acttype="64" actname ="FCVSResultTask">
		<FCVSResultTask FilePath="F:\embtest\500000.mpg" NTotalSectionCount=3>
		</FCVSResultTask>	
	  </step2>
	*/
	ST_FCVSRESULTTASK tsk;
	tsk.nTotalSectionCount = nSize;
	tsk.filePath = strFilePath.TrimRight('\\');
	tsk.filePath = tsk.filePath + "\\" + strFileName;
	CString strTsk;
	tsk.ToString(strTsk);

	//
	CTxParamString xStep2(TEXT("<step2></step2>"));
	xStep2.GoToPath(TEXT(".\\step2"));
	strVal.SetVal(SubType_MCResultMerge);
	xStep2.SetAttribVal(NULL, "acttype", strVal);
	strVal.SetVal("FCVSResultTask");
	xStep2.SetAttribVal(NULL, "actname", strVal);

	CTxParamString xResultTask(strTsk);
	xStep2.SetSubNodeString(".\\step2", xResultTask);

	txParam.SetSubNodeString(".\\edoc_main", xStep2);

	txParam.UpdateData();
	strOut = txParam;

	OutputDebugString("--技审切片信息");
	OutputDebugString(strOut);
	CFWriteLog(0, TEXT("--技审切片信息 %s"), strOut);

	return strOut;
}

/*
* Description：获取插件配置函数
* Input Param：
*		szIn：
*       szOut:
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	m_config.ToString(szOut);

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
HRESULT EMB::CTaskDispatchMgr::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	ST_TASKDISPATCHCONFIG tmpCfg;
	tmpCfg.FromString(szIn);
	if (tmpCfg.nMaster == INVALID_ID
		|| tmpCfg.nSvrID == INVALID_ID)
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}

	m_config = tmpCfg;
	m_nMaster = m_config.nMaster;
	m_nSvrId = m_config.nSvrID;

	//set optional param
	nfgTaskPoolSizeMax = (m_config.nfgTaskPoolSizeMax > 0)? m_config.nfgTaskPoolSizeMax:nfgTaskPoolSizeMax;
	nfgTaskDispatchCD = m_config.nfgTaskDispatchCD > 0? m_config.nfgTaskDispatchCD: nfgTaskDispatchCD;
	nfgTaskReDispatchMaxCount = m_config.nfgTaskReDispatchMaxCount > 0? m_config.nfgTaskReDispatchMaxCount: nfgTaskReDispatchMaxCount;
	nfgTaskReportIntervalMax =m_config.nfgTaskReportIntervalMax > 0? m_config.nfgTaskReportIntervalMax:nfgTaskReportIntervalMax;

	nfgTaskCheckProgressIntervalMax =m_config.nfgTaskCheckProgressIntervalMax > 0? m_config.nfgTaskCheckProgressIntervalMax:nfgTaskCheckProgressIntervalMax;
	nfgTaskLostTimeOutMax = m_config.nfgTaskLostTimeOutMax > 0? m_config.nfgTaskLostTimeOutMax:nfgTaskLostTimeOutMax;

	nfgActorLostTimeOutMax =m_config.nfgActorLostTimeOutMax > 0? m_config.nfgActorLostTimeOutMax:nfgActorLostTimeOutMax;
	nfgActorCheckInterval = m_config.nfgActorCheckInterval > 0? m_config.nfgActorCheckInterval : nfgActorCheckInterval;
	nfgActorStateOutdate = m_config.nfgActorStateOutdate > 0? m_config.nfgActorStateOutdate:nfgActorStateOutdate;
	nfgActorAssignTaskCD = m_config.nfgActorAssignTaskCD > 0? m_config.nfgActorAssignTaskCD:nfgActorAssignTaskCD;

	nfgCpuWeight = m_config.nfgCpuWeight >=0? m_config.nfgCpuWeight:nfgCpuWeight;
	nfgMemWeight = m_config.nfgMemWeight >= 0? m_config.nfgMemWeight:nfgMemWeight;
	nfgDiskIOWeight = m_config.nfgDiskIOWeight >= 0? m_config.nfgDiskIOWeight: nfgDiskIOWeight;
	nfgNetIOWeight = m_config.nfgNetIOWeight >= 0? m_config.nfgNetIOWeight : nfgNetIOWeight;
	CFWriteLog(TEXT("nfgMinFcvsSplitFileSizeMB = %d"), m_config.nfgMinFcvsSplitFileSizeMB);
	CFWriteLog(TEXT("nfgMaxFcvsSplitCount = %d"), m_config.nfgMaxFcvsSplitCount);
	CFWriteLog(TEXT("nfgMinTaskSplitActorCount = %d"), m_config.nfgMinTaskSplitActorCount);
	CFWriteLog(TEXT("nfgSplitTaskParallelRunMax = %d"), m_config.nfgSplitTaskParallelRunMax);

	return S_OK;
}

/*
* Description：主备切换逻辑函数
* Input Param：
* Return Param：成功表示切换成功，失败表示未切换
* History：
*/
BOOL EMB::CTaskDispatchMgr::ConfirmActive()
{
	if (!m_pIbackSvr)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	// 判断如果本身自己是主，则RETURN
	if (m_nActive == embSvrState_active)
	{
		return FALSE;
	}

	// 如果是备，则要时刻关注主机状态
	ST_SVRLIVEINFO backSvrInfo;
	m_pIbackSvr->GetRemoteSvrState(backSvrInfo);
	// 连接异常
	if (backSvrInfo.nConnState == embConnState_error)
	{
		//start self
		SetActive(embSvrState_active);
		return TRUE;
		//
	}
	else 
	{
		// 连接正常，但已经不工作
		if (backSvrInfo.nActive == embSvrState_deactive)
		{
			if (m_nMaster == embSvrType_master)
			{
				//start self
				SetActive(embSvrState_active);
				return TRUE;
			}
		}
		
	}

	return FALSE;

}

/*
* Description：设置本机工作章台
* Input Param：
*		nActive ：活动或非活动状态
* Return Param：
* History：
*/
BOOL EMB::CTaskDispatchMgr::SetActive( int nActive )
{
	if (nActive != embSvrState_active
		&& nActive != embSvrState_deactive)
	{
		return FALSE;
	}

	if (nActive != m_nActive)
	{
		m_nActive = nActive;
		OnActiveStateChanged();
	}
	else
	{
		//state not change
		ASSERT(FALSE);
	}
	return TRUE;
}

/*
* Description：主备状态切换
* Input Param：
* Return Param：返回切换成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::OnActiveStateChanged()
{
	HRESULT hr = S_OK;
	CFWriteLog(0, TEXT("svr[%d](master=%d) active change to %d"), m_nSvrId, m_nMaster,  m_nActive);
	if (GetActive() == embSvrState_active)
	{
		//get tasks that already assigned to me
		if (m_pIStorage)
		{
			VECTASKS vTasks;
			m_pIStorage->GetDispatchedTaskFromStorage(m_nSvrId, vTasks);
			CFWriteLog(0, TEXT("try get previous tasks before active. num = %d"), vTasks.size());
			CAutoLock lock(&m_csFTask);
			//set task state all to dispatched, add task to check loop
			for (size_t i = 0; i < vTasks.size(); ++i)
			{
				CString szTaskIn = vTasks[i];
				ST_EMBXMLMAININFO mainInfo;
				GetEmbXmlMainInfo(szTaskIn, mainInfo);
				if (mainInfo.nType != embxmltype_task)
				{
					hr = EMBERR_INVALIDARG;
					ASSERT(FALSE);
					continue;
				}
				//getbasic info
				ST_FILETASKDATA ftask;
				GetTaskBasicInfo(szTaskIn, ftask.taskBasic);
				TXGUID guid = String2Guid(ftask.taskBasic.strGuid);
				if(ftask.taskBasic.strGuid.IsEmpty() || guid == GUID_NULL)
				{
					hr = EMBERR_INVALIDARG;
					ASSERT(FALSE);
					continue;
				}
				ftask.strTask = szTaskIn;
				time_t tmNow = time(NULL);
				ftask.taskRunState.guid = guid;
				ftask.taskRunState.nState = embtaskstate_dispatched;
				ftask.taskRunState.tmLastReport = tmNow;
				ftask.taskRunState.tmLastCheck = tmNow;
				ftask.taskRunState.tmCommit = tmNow;
				m_mapTasks[guid] = ftask;
				if (ftask.taskBasic.nTaskSplitFlag == 1)
				{
					AddSplitTaskRef();
				}
			}
		}
	}
	else 
	{
		//remove all task;
		CAutoLock lock(&m_csFTask);
		m_mapTasks.clear();
	}

	//inform actorHolder
	m_actHolder.SetSvrState(m_nActive, m_nMaster);

	//inform notifier
	ST_SVRLIVEINFO livInfo;
	livInfo.nMaster = m_nMaster;
	livInfo.nActive = m_nActive;
	livInfo.nsvrId = m_nSvrId;
	livInfo.nConnState = embConnState_ok;
	CString strLivInfo;
	livInfo.ToString(strLivInfo);
	BroadcastToNotifier(strLivInfo);
	return S_OK;
}

/*
* Description：执行端心跳状态
* Input Param：
* Return Param：返回成功或失败
* History：
*/
BOOL EMB::CTaskDispatchMgr::ExamActor( ST_ACTORSTATE& actorStateIn )
{
	BOOL bRet = FALSE;
	time_t tmNow = time(NULL);
	CTimeSpan tmSpanCheck(tmNow - actorStateIn.tmLastCheck);
	//get actor info
	CTimeSpan tmSpanReport(tmNow - actorStateIn.tmLastReport);
	if (actorStateIn.nConnState == embConnState_error)
	{
		//check if time out;
		if (tmSpanReport.GetTotalSeconds() > nfgActorLostTimeOutMax)
		{
			bRet= TRUE;
		}
		else
		{
			//try update state from actorholder
			if (m_actHolder.HasActor(actorStateIn.actorId))
			{
				//set to connected;
				actorStateIn.nConnState = embConnState_ok;
				actorStateIn.tmLastCheck = tmNow;
			}
		}
	}
	else
	{
		if (tmSpanCheck.GetTotalSeconds() > nfgActorCheckInterval)
		{
			actorStateIn.tmLastCheck = tmNow;
			CString strMsg;
			strMsg.Format(EDOC_ACTORRESFMT, embxmltype_actorState);
			HRESULT hr = m_actHolder.SendToActor(actorStateIn.actorId, strMsg);
			if (hr != S_OK)
			{
				ASSERT(FALSE);
			}

		}
	}

	return bRet;
}

/*
* Description：移除ACTOR
* Input Param：
*		nActorid : ActorID
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskDispatchMgr::OnRemoveActor( const ACTORID nActorid )
{
	//revert all task that assign to the actor
	CAutoLock lock(&m_csFTask);
	MAPFILETASKS::iterator itb = m_mapTasks.begin();
	MAPFILETASKS::iterator ite = m_mapTasks.end();
	for (; itb != ite; ++itb)
	{
		if (itb->second.taskRunState.actorId == nActorid)
		{
			itb->second.taskRunState.actorId = INVALID_ID;
			itb->second.taskRunState.nState = embtaskstate_zero;
		}
	}
	return TRUE;
}

/*
* Description：获取空闲ACTOR信息
* Input Param：
*		nDesiredActor : 
*       nPriority ：优先级
* Return Param：返回ACTORID
* History：
*/
ACTORID EMB::CTaskDispatchMgr::GetFirstIdleActor( const ACTORID nDesiredActor, int nPriority )
{
	CAutoLock lock(&m_csActor);
	//max figure is 80% all usage

	time_t tmNow = time(NULL);
	//get min resource used actor;
	ACTORID nActorRet = INVALID_ID;
	int nMinValue = nfgMaxActorLoad;
	
	if (nDesiredActor != INVALID_ID)
	{
		MAPACTORSTATES::iterator itf = m_mapActors.find(nDesiredActor);
		if (itf != m_mapActors.end())
		{	
			ST_ACTORSTATE& actInfoRef = itf->second;
			CTimeSpan tmSpan(tmNow - actInfoRef.tmLastReport);
			CTimeSpan tmSpanLastAssign(tmNow - actInfoRef.tmLastAssignTask);
			if (actInfoRef.nConnState == embConnState_ok
				&& tmSpanLastAssign.GetTotalSeconds() > nfgActorAssignTaskCD
				&& tmSpan.GetTotalSeconds() < nfgActorStateOutdate
				&& actInfoRef.nCpuUsage > INVALID_VALUE
				&& actInfoRef.nActorLevel <= nPriority)
			{
				int nVal = CalcActorPayload(actInfoRef);
				if ( nVal < nfgMaxActorLoad)
				{
					//found it;
					return actInfoRef.actorId;
				}
			}
		}
	}

	//go normal process
	MAPACTORSTATES::iterator itb = m_mapActors.begin();
	MAPACTORSTATES::iterator ite = m_mapActors.end();
	for (; itb != ite; ++itb)
	{
		ST_ACTORSTATE& actInfoRef = itb->second;
		CTimeSpan tmSpan(tmNow - itb->second.tmLastReport);
		CTimeSpan tmSpanLastAssign(tmNow - actInfoRef.tmLastAssignTask);

		if (actInfoRef.nConnState == embConnState_ok
			&& tmSpanLastAssign.GetTotalSeconds() > nfgActorAssignTaskCD
			&& tmSpan.GetTotalSeconds() < nfgActorStateOutdate
			&& actInfoRef.nCpuUsage > INVALID_VALUE
			&& actInfoRef.nActorLevel <= nPriority)
		{
			int nVal = CalcActorPayload(actInfoRef);
			if ( nVal < nfgMaxActorLoad && nVal < nMinValue)
			{
				nMinValue = nVal;
				nActorRet = actInfoRef.actorId;
				if (nMinValue < nfgLowActorLoad)
				{
					//stop check return now
					return nActorRet;
				}
			}
		}
	}
	

	return nActorRet;
}

/*
* Description：获取空闲ACTOR信息
* Input Param：
*		strActorTeam : 指定的执行组
*       nPriority ：优先级
* Return Param：返回ACTORID
* History：
*/
BOOL EMB::CTaskDispatchMgr::GetIdleActors( CString strActorTeam,int nPriority, vector<ACTORID>& vActorOut , ACTORID excludeActor/* = INVALID_ID*/)
{
	CAutoLock lock(&m_csActor);
	//max figure is 80% all usage

	time_t tmNow = time(NULL);
	//get min resource used actor;
	ACTORID nActorRet = INVALID_ID;
	//go normal process
	MAPACTORSTATES::iterator itb = m_mapActors.begin();
	MAPACTORSTATES::iterator ite = m_mapActors.end();
	for (; itb != ite; ++itb)
	{
		ST_ACTORSTATE& actInfoRef = itb->second;
		CTimeSpan tmSpan(tmNow - itb->second.tmLastReport);
		if (actInfoRef.nConnState == embConnState_ok
			&& tmSpan.GetTotalSeconds() < nfgActorStateOutdate
			&& actInfoRef.nCpuUsage > INVALID_VALUE
			&& actInfoRef.nActorLevel <= nPriority)
		{
			int nVal = CalcActorPayload(actInfoRef);
			if ( nVal < nfgMaxActorLoad)
			{
				nActorRet = actInfoRef.actorId;
				//判断是否获取组内执行端，如果没有设置则返回
				if((strActorTeam.GetLength() > 0 && actInfoRef.strTeam == strActorTeam) || (strActorTeam.GetLength() == 0))
				{
					if (nActorRet != excludeActor)
					{
						vActorOut.push_back(nActorRet);
					}
				}

			}
		}
	}
	return TRUE;
}

/*
* Description：刷新检查未连接ACTOR状态
* Input Param：
*		nDesiredActor : 
*       nPriority ：优先级
* Return Param：返回ACTORID
* History：
*/
BOOL EMB::CTaskDispatchMgr::CheckDisconnActorCacheList()
{
	//check actor disconn list
	if (m_vCachedDisconnActor.size() > 0)
	{
		CAutoLock locDis(&m_csCacheDisActor);
		CAutoLock lock(&m_csActor);
		for (size_t i = 0; i < m_vCachedDisconnActor.size(); ++i)
		{
			ST_ACTDISCONNINFO& info = m_vCachedDisconnActor[i];
			MAPACTORSTATES::iterator itf = m_mapActors.find(info.actorid);
			if (itf != m_mapActors.end())
			{
				//check if info is old
				if (itf->second.nConnState == embConnState_ok
					&& itf->second.tmLastCheck > info.tmReport)
				{
					//state is outofdate,

				}
				else
				{
					itf->second.nConnState = embConnState_error;
					itf->second.tmLastCheck = time(NULL);
				}
			}
		}
		m_vCachedDisconnActor.clear();
	}
	return TRUE;
}

/*
* Description：检查Actor状态
* Input Param：
* Return Param：
* History：
*/
BOOL EMB::CTaskDispatchMgr::CheckActorState()
{
	//check act state;
	vector<ACTORID> vIdRemove;
	{//for lock
		CAutoLock lock(&m_csActor);
		MAPACTORSTATES::iterator itb = m_mapActors.begin();
		MAPACTORSTATES::iterator ite = m_mapActors.end();
		for (; itb != ite; ++itb)
		{				
			ST_ACTORSTATE& actState = itb->second;
			if (ExamActor(actState))
			{
				vIdRemove.push_back(itb->first);
			}
		}
	}
	


	if (vIdRemove.size() > 0)
	{
		for (size_t i =0; i < vIdRemove.size(); ++i)
		{
			OnRemoveActor(vIdRemove[i]);
			CAutoLock lock(&m_csActor);
			m_mapActors.erase(vIdRemove[i]);
		}
	}

	return TRUE;
}

/*
* Description：分配任务主函数
* Input Param：
* Return Param：
* History：
*/
BOOL EMB::CTaskDispatchMgr::TryFetchTask()
{
	if (GetActive() != embSvrState_active)
	{
		return FALSE;
	}

	if (m_mapTasks.size() < nfgTaskPoolSizeMax && m_pIStorage)
	{	
		if(GetFirstIdleActor(INVALID_ID, embtaskPriority_normal) != INVALID_ID)
		{
			CFWriteLog(0, TEXT("try fetch tasks. current = %d "),m_mapTasks.size());

				VECTASKS vTasks;
				m_pIStorage->FetchTaskFromStorage(m_nSvrId, embtaskPriority_normal, nfgTaskPoolSizeMax - m_mapTasks.size(), vTasks);

				if (vTasks.size() > 0)
				{
					CFWriteLog(0, TEXT("try fetch tasks. max = %d num = %d"), nfgTaskPoolSizeMax - m_mapTasks.size(),  vTasks.size());
					for (size_t i = 0; i < vTasks.size(); ++i)
					{
						CTaskString strRet;
						HRESULT hr = SubmitTask(vTasks[i], strRet);
						ASSERT(hr == S_OK);
					}
				}
		
		}
		else if (GetFirstIdleActor(INVALID_ID, embActorLevel_high) != INVALID_ID)
		{
			//try get high priority task when normal task is full
			VECTASKS vTasks;
			m_pIStorage->FetchTaskFromStorage(m_nSvrId, embtaskPriority_high, 10, vTasks);
			if (vTasks.size() > 0)
			{
				CFWriteLog(0, TEXT("try fetch high pri tasks. num = %d"), vTasks.size());

				for (size_t i = 0; i < vTasks.size(); ++i)
				{
					CTaskString strRet;
					HRESULT hr = SubmitTask(vTasks[i], strRet);
					ASSERT(hr == S_OK);
				}
			}
		}

	}
	return TRUE;
}

int EMB::CTaskDispatchMgr::CalcActorPayload( ST_ACTORSTATE& actInfo )
{
	if (actInfo.nCpuUsage < 0 || actInfo.nMemUsage < 0 || actInfo.nDiscIOUsage < 0
		|| actInfo.nExcResUsage < 0 || actInfo.nDiscIOUsage < 0
		|| actInfo.nNetIOUsage < 0)
	{
		CFWriteLog(TEXT("actor %d not inited!"), actInfo.actorId);
		return MAXACTORPALOAD;
	}

	if (actInfo.nCpuUsage > 80 || actInfo.nMemUsage > 80 || actInfo.nDiscIOUsage > 90
		||actInfo.nExcResUsage > 90 || actInfo.nDiscIOUsage > 80
		||actInfo.nNetIOUsage > 80)
	{
		return MAXACTORPALOAD;
	}
	return (actInfo.nCpuUsage* nfgCpuWeight + actInfo.nMemUsage* nfgMemWeight + actInfo.nDiscIOUsage * nfgDiskIOWeight
		+actInfo.nNetIOUsage * nfgNetIOWeight);
}

/*
* Description：更新执行任务状态
* Input Param：
*		reportIn：状态报告信息
* Return Param：
* History：
*/
BOOL EMB::CTaskDispatchMgr::UpdateTaskRunState( ST_TASKREPORT& reportIn )
{
	TXGUID guid = String2Guid(reportIn.strGuid);
	if (guid == GUID_NULL)
	{
		return FALSE;
	}
	CAutoLock lock(&m_csFTask);
	MAPFILETASKS::iterator itf = m_mapTasks.find(guid);
	if (itf != m_mapTasks.end())
	{
		//update taskstate
		ST_TASKRUNSTATE& taskRun = itf->second.taskRunState;
		taskRun.tmLastReport = time(NULL);
		if (reportIn.nState == embtaskstate_error)
		{
			if(reportIn.actorId != taskRun.actorId)
			{
				//ASSERT(FALSE);
				return FALSE;
			}
			// if actor is full
			if (reportIn.nSubErrorCode == EMBERR_FULL && reportIn.excutorId == INVALID_ID)
			{
				//reset the task
				taskRun.nState = embtaskstate_zero;
				taskRun.actorId = INVALID_ID;
				taskRun.excId = INVALID_ID;
				taskRun.tmLastCheck = time(NULL);
				taskRun.tmLastReport =taskRun.tmLastCheck;
				SetActorInCD(reportIn.actorId);
				CFWriteLog(0, TEXT(" [actor %d,excid = %d] report no idle excutor and reset the task[%s], nSubErrorCode:%X"), 
					reportIn.actorId, reportIn.excutorId, reportIn.strGuid, reportIn.nSubErrorCode);
			}
			else
			{
				// 不需要重置, 结束任务
				taskRun.nState = embtaskstate_error;
				taskRun.nPercent = reportIn.nPercent;
				taskRun.nCurrStep = reportIn.nStep;
				taskRun.tmLastReport = time(NULL);
				CFWriteLog(0, TEXT("[actor %d, excid = %d] 报告任务[%s]出错, nSubErrorCode:%X"), 
					reportIn.actorId, reportIn.excutorId, reportIn.strGuid, reportIn.nSubErrorCode);
				CString strRemark;
				strRemark.Format(TEXT("[actor %d, excid = %d] 报告任务出错, nSubErrorCode:%X"), reportIn.actorId, reportIn.excutorId, reportIn.nSubErrorCode);
				CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, reportIn.strGuid, taskRun.nState, strRemark);
			}	
		}
		else if (reportIn.nState == embtaskstate_dispatched)
		{
			if (taskRun.nState == embtaskstate_dispatching)
			{
				//
				if(taskRun.actorId == reportIn.actorId)
				{
					ASSERT(reportIn.excutorId != INVALID_ID);
					taskRun.nState = embtaskstate_dispatched;
					taskRun.excId= reportIn.excutorId;
					taskRun.nPercent = reportIn.nPercent;
					taskRun.nCurrStep = reportIn.nStep;
					taskRun.nExcType = reportIn.nExcType;
					taskRun.tmLastReport = time(NULL);
					taskRun.tmExcute = taskRun.tmLastReport;

					ST_TASKUPDATE upInfo;
					upInfo.nUpdateType = embtaskupdatetype_runState;
					upInfo.strTaskID = Guid2String(guid);
					upInfo.guid = guid;
					upInfo.data_end.dispatchid = m_nSvrId;
					upInfo.data_end.actorid = taskRun.actorId;
					upInfo.data_end.excutorid = reportIn.excutorId;
					upInfo.data_end.nEndState = embtaskstate_dispatched;

					CString strUpInfo = "";
					upInfo.ToString(strUpInfo);
					m_pIStorage->UpdateTaskToStorage(m_nSvrId,strUpInfo);
					CString strRemark;
					strRemark.Format(TEXT("任务[%s] 分配到 actor:%d, exc = %d"), reportIn.strGuid, reportIn.actorId, reportIn.excutorId);
					CFWriteLog(0, strRemark);
					strRemark.Format(TEXT("任务分配到 actor:%d, exc = %d"), reportIn.actorId, reportIn.excutorId);
					CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, reportIn.strGuid, taskRun.nState, strRemark);

				}
				
			}
			else if (taskRun.nState == embtaskstate_dispatched)
			{
				if(taskRun.actorId == INVALID_ID)
				{
					taskRun.actorId = reportIn.actorId;
					taskRun.excId= reportIn.excutorId;
					CString strRemark;
					strRemark.Format(TEXT("任务[%s] 分配信息更新! actor:%d, exc = %d"), reportIn.strGuid, reportIn.actorId, reportIn.excutorId);
					CFWriteLog(0, strRemark);
					strRemark.Format(TEXT("任务分配信息更新! actor:%d, exc = %d"), reportIn.actorId, reportIn.excutorId);
					CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, reportIn.strGuid, taskRun.nState, strRemark);

				}
				else
				{
					ASSERT(taskRun.actorId == reportIn.actorId
						&& taskRun.excId == reportIn.excutorId);
				}

				if (m_config.nfgLogTaskExcDetail == 1 
					&& reportIn.nStep >= 0
					&& taskRun.nCurrStep != reportIn.nStep)
				{
					CString strRemark;
					strRemark.Format(TEXT("任务[%s]执行信息：[step=%d]type[=%d] finished"), reportIn.strGuid, reportIn.nStep, reportIn.nExcType);
					CFWriteLog(0, strRemark);
					strRemark.Format(TEXT("任务执行信息：[step=%d]type[=%d] finished"), reportIn.nStep, reportIn.nExcType);
					CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH, m_nSvrCode, reportIn.strGuid, taskRun.nState, strRemark);
				}
				taskRun.nPercent = reportIn.nPercent;
				taskRun.nCurrStep = reportIn.nStep;
				taskRun.nExcType = reportIn.nExcType;

				taskRun.tmLastReport = time(NULL);
			
				//CFWriteLog(0, TEXT(" [actor %d,excid = %d] report task (%s),  step = %d, percent = %d%%"), reportIn.actorId,reportIn.excutorId, reportIn.strGuid,reportIn.nStep, reportIn.nPercent);

			}
		}
		else if (reportIn.nState == embtaskstate_finished)
		{
			ASSERT(taskRun.actorId == reportIn.actorId
				&& taskRun.excId == reportIn.excutorId
				&& (taskRun.nState == embtaskstate_dispatched
					||taskRun.nState == embtaskstate_finished));
			taskRun.nState = embtaskstate_finished;
			taskRun.nPercent = reportIn.nPercent;
			taskRun.nCurrStep = reportIn.nStep;
			taskRun.nExcType = reportIn.nExcType;
			taskRun.tmLastReport = time(NULL);
			CFWriteLog(0, TEXT(" [actor %d,excid = %d] report task finished [%s],  step = %d, percent = %d%%"), reportIn.actorId,reportIn.excutorId, reportIn.strGuid,reportIn.nStep, reportIn.nPercent);
		}
		
	}

	return TRUE;
}

/*
* Description：更新ACTOR状态
* Input Param：
*		stateIn：状态报告信息
* Return Param：
* History：
*/
BOOL EMB::CTaskDispatchMgr::UpdateActorState( ST_ACTORSTATE& stateIn )
{
	CAutoLock lock(&m_csActor);

	MAPACTORSTATES::iterator itf = m_mapActors.find(stateIn.actorId);
	if (itf != m_mapActors.end())
	{
		ST_ACTORSTATE& actState = itf->second;
		time_t tmLastAssignTask = actState.tmLastAssignTask;
		CString strPcName = actState.strPcName;

		actState = stateIn;
		actState.strPcName = strPcName;
		actState.tmLastAssignTask = tmLastAssignTask;
		actState.nConnState = embConnState_ok;
		actState.tmLastCheck = time(NULL);
		actState.tmLastReport = actState.tmLastCheck;	

	}

	return TRUE;
}

BOOL EMB::CTaskDispatchMgr::TaskNeedSplit( CString strTaskIn )
{
	//should pre check that if task need split after fetchtask() 
	BOOL need = (strTaskIn.Find("<FCVSTask") > 0); // 技审需要切片
	if (need)
	{
		need = strTaskIn.Find("BUseSection=\"1\"") > 0;
	}

	return need;
}

BOOL EMB::CTaskDispatchMgr::TryDispatchSplitTask( ST_FILETASKDATA& taskIn )
{
	BOOL bRet = FALSE;
	vector<ACTORID> vActorIDs;
	GetIdleActors(taskIn.taskBasic.strTaskFrom,taskIn.taskBasic.nPriority, vActorIDs);
	int nSize = vActorIDs.size();

	if(nSize > 1)
	{
		//if(taskIn.taskBasic.nTaskSplit > 0) 临时注释
		{
			//拆分任务
			for(int i = 0 ; i < nSize ; i++)
			{
				ST_FILETASKDATA subTask = taskIn; // 切片任务
				CString strXml = CreateSplitTaskXml(taskIn.strTask,i + 1,nSize, subTask); // nStart 从1开始计数

				if(strXml.GetLength() == 0)
				{
					break;
				}

				ACTORID actorid =vActorIDs[i];

				HRESULT hr = m_actHolder.SendToActor(actorid, strXml);
				if (hr == S_OK)
				{
					if (i == 0)
					{
						taskIn.taskRunState.actorId = actorid;
						taskIn.taskRunState.nState = embtaskstate_dispatching;
					}
					else  // add subtask
					{
						subTask.taskRunState.actorId = actorid;
						subTask.taskRunState.nState = embtaskstate_dispatching;
						TXGUID newGuid = String2Guid(subTask.taskBasic.strGuid);
						if (m_mapTasks.find(newGuid) == m_mapTasks.end())
						{
							m_mapTasks[newGuid] = subTask;
							CFWriteLog(0, TEXT("add 切片任务 %s"), subTask.taskBasic.strGuid);
						}
					}

					CFWriteLog(0, TEXT("assign task (%s) to actor %d"), subTask.taskBasic.strGuid, actorid);
				}
				else
				{
					ASSERT(FALSE);
					//waiting for next loop 
				}
			}
		}
	}
	
	return bRet;
}

BOOL EMB::CTaskDispatchMgr::IsLiveActor( const ACTORID nActorId )
{
	CAutoLock lock(&m_csActor);
	MAPACTORSTATES::iterator itf = m_mapActors.find(nActorId);
	return itf != m_mapActors.end();
}

BOOL EMB::CTaskDispatchMgr::SetActorInCD( const ACTORID actorId )
{
	CAutoLock lock(&m_csActor);
	MAPACTORSTATES::iterator itf = m_mapActors.find(actorId);
	if (itf != m_mapActors.end())
	{
		itf->second.tmLastAssignTask = time(NULL);
	}

	return TRUE;
}

HRESULT EMB::CTaskDispatchMgr::OnUIMessage( CTaskString& strMsg, CTaskString& szRet )
{
	HRESULT hr = S_OK;
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(strMsg, mainInfo);
	if (mainInfo.nType == embxmltype_svrInfo)
	{
		hr = GetSvrInfo(szRet);
	}
	else if (mainInfo.nType == embxmltype_actorList)
	{
		hr = GetActorList(szRet);
	}
	else if (mainInfo.nType == embxmltype_actorState)
	{
		ST_ACTORSTATE tmpState;
		tmpState.FromString(strMsg);
		hr = GetActorState(tmpState.actorId, szRet);
	}
	else if (mainInfo.nType == embxmltype_taskList)
	{
		hr = GetTaskList(szRet);
	}
	else if (mainInfo.nType == embxmltype_taskRunState)
	{
		ST_TASKRUNSTATE bsInfo;
		bsInfo.FromString(strMsg, TRUE);
		TXGUID taskid(bsInfo.guid);
		hr = GetTaskRunState(taskid, szRet);
	}
	else if (mainInfo.nType == embxmltype_taskupdate)
	{
		//to be added...
		ASSERT(FALSE);
	}
	else
	{
		hr = EMBERR_INVALIDARG;
	}
	
	if (szRet.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CTxParamString txParam(szRet);
	txParam.SetAttribVal(EK_MAIN, EA_MAIN_TYPE, mainInfo.nType);
	txParam.SetAttribVal(EK_MAIN, EA_MAIN_ERRCODE, hr);
	txParam.SetAttribVal(EK_MAIN, EA_MAIN_GUID, mainInfo.guid);
	txParam.UpdateData();
	szRet = txParam;

	return hr;
}

HRESULT EMB::CTaskDispatchMgr::GetSvrInfo( CString& strRet )
{
	ST_SVRLIVEINFO livInfo;
	livInfo.nActive = m_nActive;
	livInfo.nMaster = m_nMaster;
	livInfo.nsvrId = m_nSvrId;
	CString strTmp;
	livInfo.ToString(strRet);
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::GetActorState( ACTORID actorID, CString& strRet )
{
	HRESULT hr = S_OK;
	ST_ACTORSTATE stateRet;
	CAutoLock lock(&m_csActor);
	MAPACTORSTATES::iterator itf = m_mapActors.find(actorID);
	if (itf != m_mapActors.end())
	{
		//found it
		stateRet = itf->second;
	}
	else
	{
		hr = EMBERR_NOTFOUND;
	}
	
	stateRet.ToString(strRet);
	return hr;
}

HRESULT EMB::CTaskDispatchMgr::GetTaskList( CString& strRet )
{
	ST_TASKLISTINFO tsInfo;
	{//for lock
		CAutoLock lock(&m_csFTask);
		MAPFILETASKS::iterator itb =m_mapTasks.begin();
		MAPFILETASKS::iterator ite = m_mapTasks.end();
		for (; itb != ite; ++itb)
		{
			tsInfo.vlist.push_back(itb->first);
		}
	}

	tsInfo.ToString(strRet);
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::GetTaskRunState( TXGUID& taskGuid, CString& strRet )
{
	HRESULT hr = S_OK;
	ST_TASKRUNSTATE taskState;
	taskState.guid = taskGuid;
	{//for lock
		CAutoLock lock(&m_csFTask);
		MAPFILETASKS::iterator itf = m_mapTasks.find(taskGuid);
		if (itf != m_mapTasks.end())
		{
			taskState  = itf->second.taskRunState;
		}
		else
		{
			hr = EMBERR_NOTFOUND;
		}
	}

	taskState.ToString(strRet, TRUE);
	
	return hr;
}

HRESULT EMB::CTaskDispatchMgr::GetActorList( CString& strRet )
{
	ST_ACTORLISTINFO tsInfo;
	{//for lock

		CAutoLock lock(&m_csActor);
		MAPACTORSTATES::iterator itb =m_mapActors.begin();
		MAPACTORSTATES::iterator ite = m_mapActors.end();
		for (; itb != ite; ++itb)
		{
			CTxStrConvert val(itb->first);
			tsInfo.vlist.push_back(val.GetAsString());
		}
	}
	tsInfo.ToString(strRet);

	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::RegisterNotifier( IDispatchNotifyCallbackInterface* pNotifier )
{
	if (!pNotifier)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CAutoLock lock(&m_csNotifier);
	MAPDISNOTIFYS::iterator itf = m_mapDisNotifys.find(pNotifier);
	if (itf == m_mapDisNotifys.end())
	{
		m_mapDisNotifys[pNotifier] = pNotifier;
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT EMB::CTaskDispatchMgr::UnRegisterNotifier( IDispatchNotifyCallbackInterface* pNotifier )
{
	CAutoLock lock(&m_csNotifier);
	MAPDISNOTIFYS::iterator itf = m_mapDisNotifys.find(pNotifier);
	if (itf != m_mapDisNotifys.end())
	{
		m_mapDisNotifys.erase(itf);
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

BOOL EMB::CTaskDispatchMgr::BroadcastToNotifier( CString& strInfo )
{
	CAutoLock lock(&m_csNotifier);
	MAPDISNOTIFYS::iterator itb = m_mapDisNotifys.begin();
	MAPDISNOTIFYS::iterator ite = m_mapDisNotifys.end();
	for (; itb != ite; ++itb)
	{
		itb->first->OnDispatchNotify(strInfo);
	}

	return TRUE;
}

void EMB::CTaskDispatchMgr::WriteDBLog_Task(LPCTSTR szTaskID, int nState, LPCTSTR szRemark )
{
	if (m_bDBLogEnabled)
	{
		CFWriteLog(0, TEXT("dblog_Task: task =%s state= %d, remark=%s"), szTaskID, nState, szRemark);

		CFWriteDBLog_Task(DBLOGKEY_TASKDISPATCH,m_nSvrCode, szTaskID, nState, szRemark);
	}
}

void EMB::CTaskDispatchMgr::WriteDBLog_EMB(int nType, LPCTSTR szRemark )
{
	if (m_bDBLogEnabled)
	{
		CFWriteLog(0, TEXT("dblog_Emb: type= %d, remark=%s"),  nType, szRemark);
		CFWriteDBLog_EMB(DBLOGKEY_TASKDISPATCH, m_nSvrCode, nType, szRemark);
	}
}

BOOL EMB::CTaskDispatchMgr::OnExcCallback( ST_EXCCALLBACKINFO& infoIn )
{
	//process
	BOOL bRet = FALSE;
	if (infoIn.nRetType == embrecalltype_spfcvs)
	{
		//try split fcvs task
		DWORD dcount = GetTickCount();
		bRet = TrySplitFCVSSubTask(infoIn);
		dcount = GetTickCount() - dcount;
		CFWriteLog(TEXT("split use time %d"), dcount);
	}
	else if (infoIn.nRetType == embrecalltype_RequestTaskRunInfo)
	{
		bRet = OnExcRequestTaskRunInfo(infoIn);
	}
	return bRet;
}

BOOL EMB::CTaskDispatchMgr::TrySplitFCVSSubTask( ST_EXCCALLBACKINFO& infoIn )
{
	CFWriteLog(0, TEXT("receive splitFcvs Request, try gen fcvs split task..., id = %s"), infoIn.taskId);
	BOOL bCanSp = FALSE;
	ST_FCVS fcvsInfo;
	fcvsInfo.taskInfo.FromString(infoIn.strExtInfo);
	fcvsInfo.configInfo.FromString(infoIn.strExtInfo);
	std::map<int, CString>::iterator itf = infoIn.mapExchInfo.find(embruntimeInfoType_filedest);
	if (itf != infoIn.mapExchInfo.end())
	{
		//time to change task dest file
		ST_TRANSRESULT desNew;
		desNew.FromString(itf->second);
		if ((desNew.nPathType & embpathtype_unc) &&!desNew.strDestUncDir.IsEmpty())
		{
			fcvsInfo.taskInfo.filePath = desNew.strDestUncDir;
			fcvsInfo.taskInfo.fileName = desNew.strDestFileName;
			CFWriteLog(0, TEXT("runtime file dest info changed to %s\\%s"), desNew.strDestUncDir, desNew.strDestFileName);
		}
	}

	//try get media file length;
	BOOL bHasRealMediainfo = FALSE;
	ST_RUNTIMEMEDIAINFO mediaInfo;
	itf = infoIn.mapExchInfo.find(embruntimeInfoType_meidaInfo);
	if (itf != infoIn.mapExchInfo.end())
	{
		mediaInfo.FromString(itf->second);
		bHasRealMediainfo = TRUE;
	}

	ST_FILETASKDATA taskData;

	{//for lock release
		CAutoLock lock(&m_csFTask);
		TXGUID taskGuid = String2Guid(infoIn.taskId);
		MAPFILETASKS::iterator itf = m_mapTasks.find(taskGuid);
		if (itf == m_mapTasks.end())
		{
			//task not found
			ASSERT(FALSE);
			CFWriteLog(TEXT("TrySplitFCVSSubTask, task not found. id = %s"), infoIn.taskId);
			return FALSE;
		}
		else
		{
			taskData = itf->second;
		}
	}

	vector<ACTORID> vActors;
	int nRealSplitNum = 0;
	mediaInfo.nFileLength /= 1024*1024;
	CFWriteLog(0, TEXT("runtime meidainfo file = %s, length = %dM"), mediaInfo.strFileName, mediaInfo.nFileLength);
	if (m_config.nfgMaxFcvsSplitCount >1 && fcvsInfo.configInfo.bUseSection &&
		(mediaInfo.nFileLength > 0 || !bHasRealMediainfo))
	{
		GetIdleActors(taskData.taskBasic.strTaskFrom,taskData.taskBasic.nPriority, vActors, infoIn.nActorId);
		if (vActors.size() > 0)
		{
			int nMaxSplitNum = (vActors.size()+1) > m_config.nfgMaxFcvsSplitCount? m_config.nfgMaxFcvsSplitCount: (vActors.size()+1);
			
			if (bHasRealMediainfo && mediaInfo.nFileLength> m_config.nfgMinFcvsSplitFileSizeMB*2)
			{//must can split > 1
				
				if (mediaInfo.nFileLength /(nMaxSplitNum) < m_config.nfgMinFcvsSplitFileSizeMB)
				{
					//actor was exceeded, use
					nRealSplitNum = mediaInfo.nFileLength/m_config.nfgMinFcvsSplitFileSizeMB;
				}
				else
				{
					nRealSplitNum = vActors.size()+1;
				}
				bCanSp = nRealSplitNum > 1;
			}

			if (!bHasRealMediainfo)
			{
				CFWriteLog(0, TEXT("not runtime media info, use task fcvs nTotalSectionCount"), nRealSplitNum);
				nRealSplitNum =fcvsInfo.taskInfo.nTotalSectionCount;
				nRealSplitNum = nRealSplitNum >nMaxSplitNum ? nMaxSplitNum:nRealSplitNum;
				bCanSp = nRealSplitNum > 1;
			}
		}
	}

	
	if (nRealSplitNum > m_config.nfgMaxFcvsSplitCount)
	{
		nRealSplitNum = m_config.nfgMaxFcvsSplitCount;
	}

	CFWriteLog(0, TEXT("runtime fcvs split count = %d"), bCanSp? nRealSplitNum:1);

	//update split mark in real task
	if (nRealSplitNum > 1)
	{
		CAutoLock lock(&m_csFTask);
		TXGUID taskGuid = String2Guid(infoIn.taskId);
		MAPFILETASKS::iterator itf = m_mapTasks.find(taskGuid);
		if (itf == m_mapTasks.end())
		{
			itf->second.taskRunState.nSplited = 1;
		}
	}

	if (bCanSp)
	{
		//generate split task
		vector<CString> vSubIds;
		int nTotalSpSize = nRealSplitNum;
		ST_EXCCALLBACKINFO reCallInfo;

		for (size_t i = 0 ; i < nTotalSpSize; ++i)
		{

			ST_FILETASKDATA subTask; // 切片任务
			CreateFcvsSubTask(infoIn.taskId, fcvsInfo, i+1, nTotalSpSize, subTask);
			if (i == 0)
			{
				vSubIds.push_back(infoIn.taskId);
				//create first split task and send back
				reCallInfo = infoIn;
				reCallInfo.nRetType = embrecalltype_WorkChange;
				reCallInfo.nWorkType = SubType_MediaCheck;
				
				//add runtime merge info for fcvsmerge plugin
				CTxParamString txParTask(subTask.strTask);
				CTxParamString txParSub;
				txParTask.GetSubNodeString(TEXT(".\\edoc_main\\step1\\FCVS"), txParSub);
				txParSub.UpdateData();
				reCallInfo.strExtInfo =txParSub;
				
				continue;
			}

			vSubIds.push_back(subTask.taskBasic.strGuid);

			ACTORID actorid =vActors[i-1]; 
			{
				TXGUID newGuid = subTask.taskRunState.guid;
				subTask.taskRunState.actorId = actorid;
				subTask.taskRunState.nState = embtaskstate_dispatching;
				subTask.taskRunState.tmLastCheck = time(NULL);
				subTask.taskRunState.nSplited = nRealSplitNum > 1;
				CAutoLock lock(&m_csFTask);
				if(m_mapTasks.find(newGuid) != m_mapTasks.end())
				{
					CFWriteLog(0, TEXT("fatal error: subtask already existed! %s"), subTask.taskBasic.strGuid);
				}
				else
				{
					m_mapTasks[newGuid] = subTask;
					CFWriteLog(0, TEXT("add 切片任务 %s"), subTask.taskBasic.strGuid);
				}
			}	
			//assing sub task over 2
			HRESULT hr = m_actHolder.SendToActor(actorid, subTask.strTask);
			if (hr == S_OK)
			{
				CFWriteLog(0, TEXT("assign task (%s) to actor %d"), subTask.taskBasic.strGuid, actorid);
			}
			else
			{
				ASSERT(FALSE);
				//waiting for next loop 
			}
		}

		//send first task to org task actor
		ST_FCVSRUNTIMEMERGEINFO mergeInfo;
		mergeInfo.nTotalSplit = nRealSplitNum;
		mergeInfo.vSubIds = vSubIds;
		mergeInfo.strFileName = fcvsInfo.taskInfo.fileName;
		mergeInfo.strPath = fcvsInfo.taskInfo.filePath;
		CString strMgrInfo;
		mergeInfo.ToString(strMgrInfo);
		reCallInfo.mapExchInfo[embruntimeInfoType_fcvsMergeInfo] = strMgrInfo;
		CString strRet;
		reCallInfo.ToString(strRet);
		m_actHolder.SendToActor(reCallInfo.nActorId, strRet);

	}
	else
	{
		//whatever generate replace task;
		ST_FCVS fcvs;
		fcvs.taskInfo.FromString(infoIn.strExtInfo);
		fcvs.configInfo.FromString(infoIn.strExtInfo);
		fcvs.configInfo.bUseSection = FALSE;
		fcvs.taskInfo.nTotalSectionCount = 1;
		ST_EXCCALLBACKINFO reCallInfo;
		reCallInfo = infoIn;

		ST_FCVSRUNTIMEMERGEINFO mergeInfo;
		mergeInfo.nTotalSplit = 1;
		mergeInfo.vSubIds.push_back(infoIn.taskId);
		mergeInfo.strFileName = fcvsInfo.taskInfo.fileName;
		mergeInfo.strPath = fcvsInfo.taskInfo.filePath;
		CString strMgrInfo;
		mergeInfo.ToString(strMgrInfo);
		reCallInfo.mapExchInfo[embruntimeInfoType_fcvsMergeInfo] = strMgrInfo;
		fcvs.ToString(reCallInfo.strExtInfo);
		reCallInfo.nRetType = embrecalltype_WorkChange;
		reCallInfo.nWorkType = SubType_MediaCheck;
		CString strRet;
		reCallInfo.ToString(strRet);
		m_actHolder.SendToActor(reCallInfo.nActorId, strRet);
	}
	return bCanSp;
}

BOOL EMB::CTaskDispatchMgr::CreateFcvsSubTask(const CString strOrgGuid, const ST_FCVS& taskOrgIn, const int nPartNumIn, const int nTotalPartIn, ST_FILETASKDATA& taskDataOut)
{
	CString strMain;
	TXGUID guidNew =TxGenGuid();
	CString strGuidNew = Guid2String(guidNew);
	ST_EMBXMLMAININFO mainInfo;
	mainInfo.guid = strGuidNew;
	mainInfo.mergeGuid = strOrgGuid;
	mainInfo.nType = embxmltype_task;
	mainInfo.ToString(strMain);
	CTxParamString txParam(strMain);
	CString strTaskBasic;
	ST_TASKBASIC taskBasic;
	taskBasic.nStartStep = 0;
	taskBasic.nFixActor = -1;
	taskBasic.nTaskSplitFlag = 0;
	taskBasic.vSubTask.push_back(TEXT("step1"));
	taskBasic.strGuid = strGuidNew;
	taskBasic.strGuidBase = strOrgGuid;
	taskBasic.ToString(strTaskBasic);
	CTxParamString txParBasic(strTaskBasic);
	txParam.SetSubNodeString(TEXT(".\\edoc_main"), txParBasic);
	txParam.GoIntoKey(EK_MAIN);
	txParam.SetElemVal(TEXT("step1"), TEXT(""));
	txParam.SetAttribVal(TEXT("step1"), EA_SUBTASK_TYPE, SubType_MediaCheck);
	txParam.SetAttribVal(TEXT("step1"), EA_SUBTASK_NAME, TEXT("FCVS"));
	
	//write fcvs
	ST_FCVS fcvs = taskOrgIn;
	fcvs.configInfo.bUseSection = TRUE;
	fcvs.taskInfo.nSectionID = nPartNumIn;
	fcvs.taskInfo.nTotalSectionCount = nTotalPartIn;
	CString strFcvs;
	fcvs.ToString(strFcvs);
	CTxParamString txParFcvs(strFcvs);
	txParam.SetSubNodeString(TEXT(".\\edoc_main\\step1"), txParFcvs);
	txParam.UpdateData();
	taskDataOut.taskBasic = taskBasic;
	taskDataOut.strTask = txParam;
	taskDataOut.taskRunState.guid = guidNew;
	taskDataOut.taskRunState.tmCommit = time(NULL);
	return TRUE;

}

BOOL EMB::CTaskDispatchMgr::CreateFcvsMergeSubTask( const CString strOrgGuid, vector<CString>vSubGuids, const int nTotalPartIn, const CString& strFilePathIn,   CString& taskOut )
{
	ST_FCVSRESULTTASK task;
	task.filePath = strFilePathIn;
	task.strGuidBase = strOrgGuid;
	task.vSubIds = vSubGuids;
	task.ToString(taskOut);

	return TRUE;
}

BOOL EMB::CTaskDispatchMgr::OnExcRequestTaskRunInfo( ST_EXCCALLBACKINFO& infoIn )
{
	ST_TASKRUNSTATE runState;
	runState.FromString(infoIn.strExtInfo, TRUE);
	if (runState.guid == GUID_NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	{//for lock
		CAutoLock lock(&m_csFTask);
		MAPFILETASKS::iterator itf = m_mapTasks.find(runState.guid);
		if (itf != m_mapTasks.end())
		{
			runState = itf->second.taskRunState;
		}
		else
		{
			runState.nState = embtaskstate_finished;
		}
	}

	//send back to exc
	infoIn.strExtInfo.Empty();
	runState.ToString(infoIn.strExtInfo, TRUE);
	CString strRet;
	infoIn.ToString(strRet);
	m_actHolder.SendToActor(infoIn.nActorId, strRet);
	return TRUE;
}

HRESULT EMB::CTaskDispatchMgr::GetActors( vector<CString>& vActor )
{
	CAutoLock lock(&m_csActor);
	vActor.clear();

	MAPACTORSTATES::iterator itor = m_mapActors.begin();
	for (; itor != m_mapActors.end(); ++itor)
	{
		CString str;
		itor->second.ToString(str);
		vActor.push_back(str);
	}

	return S_OK;
}


int EMB::CTaskDispatchMgr::GetWaitingTaskCount()
{
	int nCount = 0;
	CAutoLock lock(&m_csFTask);
	MAPFILETASKS::iterator itb = m_mapTasks.begin();
	MAPFILETASKS::iterator ite = m_mapTasks.end();
	for (; itb != ite; ++itb)
	{
		if (itb->second.taskRunState.nState != embtaskstate_dispatched)
		{
			++nCount;
		}
	}

	return nCount;
}

BOOL EMB::CTaskDispatchMgr::TryFetchTaskNew()
{
	if (GetActive() != embSvrState_active)
	{
		return FALSE;
	}
	if (!m_pIStorage)
	{
		return FALSE;
	}

	int nWaitTaskCount = GetWaitingTaskCount();
	if (nWaitTaskCount > 0)
	{
		CFWriteLog(TEXT("waiting task count = %d"), nWaitTaskCount);
		return TRUE;
	}

	VECTASKS vTasks;
	m_pIStorage->FetchTaskFromStorage(m_nSvrId, embtaskPriority_normal, 1, vTasks);

	if (vTasks.size() > 0)
	{
		CFWriteLog(0, TEXT("try fetch tasks. count = %d"), vTasks.size());
		for (size_t i = 0; i < vTasks.size(); ++i)
		{
			CTaskString strRet;
			HRESULT hr = SubmitTask(vTasks[i], strRet);
			ASSERT(hr == S_OK);
		}
	}

	return TRUE;
}

int EMB::CTaskDispatchMgr::GetDispatchedSplitTaskCount()
{
	int nCount = 0;
	CAutoLock lock(&m_csFTask);
	MAPFILETASKS::iterator itb = m_mapTasks.begin();
	MAPFILETASKS::iterator ite = m_mapTasks.end();
	for (; itb != ite; ++itb)
	{
		if (itb->second.taskRunState.nState > embtaskstate_zero)
		{
			++nCount;
		}
	}
	return nCount;
}

int EMB::CTaskDispatchMgr::GetTotalActors()
{
	CAutoLock lock(&m_csActor);
	return m_mapActors.size();
}
