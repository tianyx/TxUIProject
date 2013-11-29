#include "StdAfx.h"
#include "TaskDispatchMgr.h"
#include "IEMBBaseInterface.h"
#include "EMBCommonFunc.h"
#include "EMBDocDef.h"
#include "TxAutoComPtr.h"
#include "TxLogManager.h"
#include "EmbStructDef.h"
using namespace EMB;
#define MAXACTORPALOAD 0x00FFFFFF
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

	//task check param
	nfgTaskPoolSizeMax = 2000;
	nfgTaskDispatchCD = 60;
	nfgTaskReDispatchMaxCount = 5;
	nfgTaskReportIntervalMax = 20;
	nfgTaskCheckProgressIntervalMax = 5;
	nfgTaskLostTimeOutMax = 60;

	nfgActorLostTimeOutMax = 30;
	nfgActorCheckInterval = 10;


	nfgCpuWeight = 1;
	nfgMemWeight = 1;
	nfgDiskWeight =0;

	nfgMaxActorLoad = 100*(nfgCpuWeight + nfgMemWeight + nfgDiskWeight)*4/5;
	nfgLowActorLoad = 100*(nfgCpuWeight + nfgMemWeight + nfgDiskWeight)/5;

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

	// 启动任务分配线程
	m_bCheckBackSvr = TRUE;
	m_hThdCheck = CreateThread(NULL, 0, TDCheckProc,  (LPVOID)this, 0, 0);
	ASSERT(m_hThdCheck);

	// 启动主备监听线程
	m_hThdFtask = CreateThread(NULL, 0, TDFTaskProc, (LPVOID)this, 0, 0);
	ASSERT(m_hThdFtask);

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
		pRemote = new CMasterHeartBeat;
		pRemote->SetScokAddr(&addrMaster, &addrMaster);

	}
	else
	{
		pRemote = new CSlaveHeartBeat;
		pRemote->SetScokAddr(&addrMaster, &addrLocal);
	}
	pRemote->Run();
	ASSERT(m_pIbackSvr == NULL);
	m_pIbackSvr = dynamic_cast<IRemoteSvrLiveInterface*>(pRemote) ;
	ASSERT(m_pIbackSvr);

	if (m_hThdCheck && m_hThdFtask)
	{
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
HRESULT EMB::CTaskDispatchMgr::OnActorConnect( const ACTORID& szActorGuid )
{
	CAutoLock lock(&m_csCacheDisActor);
	ST_ACTORSTATE actState;
	actState.actorId = szActorGuid;
	actState.tmLastCheck = time(NULL);
	actState.tmLastReport = time(NULL);
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
		if (report.excutorId != INVALID_ID
			&& !report.strGuid.IsEmpty())
		{
			UpdateTaskRunState(report);
		}
		else
		{
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
	while(WaitForSingleObject(m_hEventQuitLoop, 1000) != WAIT_OBJECT_0)
	{
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
		TryFetchTask();
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
		CFWriteLog(0, TEXT("task %s finished,update to storage"), taskIn.taskBasic.strGuid);

		//write task back to storage;
		ST_TASKUPDATE upInfo;
		upInfo.nUpdateType = embtaskupdatetype_finish;
		upInfo.guid = taskIn.taskRunState.guid;
		upInfo.data_end.dispatchid = m_nSvrId;
		upInfo.data_end.actorid = taskIn.taskRunState.actorId;
		upInfo.data_end.excutorid = taskIn.taskRunState.excId;
		upInfo.data_end.nEndState = taskIn.taskRunState.nState;
		CString strUpInfo;
		upInfo.ToString(strUpInfo);
		m_pIStorage->UpdateTaskToStorage(m_nSvrId, strUpInfo);
		bRet = TRUE;		
	
	}
	else if (taskIn.taskRunState.nState == embtaskstate_zero)
	{
		//not assigned,assign it;
		ACTORID actid = GetFirstIdleActor(taskIn.taskBasic.nFixActor, taskIn.taskBasic.nPriority);
		if (actid != INVALID_ID)
		{
			HRESULT hr = m_actHolder.SendToActor(actid, taskIn.strTask);
			if (hr == S_OK)
			{
				taskIn.taskRunState.actorId = actid;
				taskIn.taskRunState.nState = embtaskstate_dispatching;
				CFWriteLog(0, TEXT("assign task (%s) to actor %d"), taskIn.taskBasic.strGuid, actid);

				
			}
			else
			{
				ASSERT(FALSE);
				//waiting for next loop 
			}
		}
	}
	else if (taskIn.taskRunState.nState == embtaskstate_dispatching)
	{
		CTimeSpan tmSpan(tmNow - taskIn.taskRunState.tmLastCheck);
		if (tmSpan.GetSeconds() > nfgTaskDispatchCD)
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
							//may be switched active now wait for actor report 
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
	CFWriteLog(0, TEXT("svr(%d) active change to %d"), m_nMaster,  m_nActive);
	if (GetActive() == embSvrState_active)
	{
		//get tasks that already assigned to me
		if (m_pIStorage)
		{
			VECTASKS vTasks;
			m_pIStorage->GetDispatchedTaskFromStorage(m_nSvrId, vTasks);

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
				time_t tmNow = time(NULL);
				ftask.taskRunState.guid = guid;
				ftask.taskRunState.nState = embtaskstate_dispatched;
				ftask.taskRunState.tmLastReport = tmNow;
				ftask.taskRunState.tmLastCheck = tmNow;
				ftask.taskRunState.tmCommit = tmNow;
				m_mapTasks[guid] = ftask;
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
			if (actInfoRef.nConnState == embConnState_ok
				&& tmSpan.GetTotalSeconds() < 10
				&& actInfoRef.nCpuUsage > INVALID_VALUE
				&& actInfoRef.nActorLevel < nPriority)
			{
				int nVal = CalcActorPayload(actInfoRef);
				if ( nVal < nfgMaxActorLoad && nVal < nMinValue)
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
		if (actInfoRef.nConnState == embConnState_ok
			&& tmSpan.GetTotalSeconds() < 10
			&& actInfoRef.nCpuUsage > INVALID_VALUE
			&& actInfoRef.nActorLevel < nPriority)
		{
			int nVal = CalcActorPayload(actInfoRef);
			if ( nVal < nfgMaxActorLoad && nVal < nMinValue)
			{
				nMinValue = nVal;
				nActorRet = actInfoRef.actorId;
				if (nMinValue < nfgLowActorLoad)
				{
					//
					return nActorRet;
				}
			}
		}
	}
	

	return nActorRet;
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
	CAutoLock lock(&m_csActor);
	vector<ACTORID> vIdRemove;
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


	if (vIdRemove.size() > 0)
	{
		for (size_t i =0; i < vIdRemove.size(); ++i)
		{
			OnRemoveActor(vIdRemove[i]);
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
			
				VECTASKS vTasks;
				m_pIStorage->FetchTaskFromStorage(m_nSvrId, embtaskPriority_normal, 10, vTasks);
				if (vTasks.size() > 0)
				{
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
	if (actInfo.nCpuUsage > 80 || actInfo.nMemUsage > 80 || actInfo.nDiscUsage > 90)
	{
		return MAXACTORPALOAD;
	}
	return (actInfo.nCpuUsage* nfgCpuWeight + actInfo.nMemUsage* nfgMemWeight + actInfo.nDiscUsage * nfgDiskWeight);
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
			ASSERT(reportIn.actorId == taskRun.actorId);
			// 判断具体错误，是否重置任务?
			if (reportIn.NeedResetTask())
			{
				//reset the task
				CFWriteLog(0, TEXT("reset task"));
				taskRun.nState = embtaskstate_zero;
				taskRun.actorId = INVALID_ID;
				taskRun.excId = INVALID_ID;
			}
			else
			{
				// 不需要重置, 结束任务
				taskRun.nState = embtaskstate_finished;
				taskRun.nPercent = reportIn.nPercent;
				taskRun.nCurrStep = reportIn.nStep;
				taskRun.tmLastReport = time(NULL);
				CFWriteLog(0, TEXT(" [actor %d,excid = %d] report task finished (%s), nSubErrorCode:%X"), 
					reportIn.actorId, reportIn.excutorId, reportIn.strGuid, reportIn.nSubErrorCode);
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
					taskRun.tmLastReport = time(NULL);
				}
				
			}
			else if (taskRun.nState == embtaskstate_dispatched)
			{
				if(taskRun.actorId == INVALID_ID)
				{
					taskRun.actorId = reportIn.actorId;
					taskRun.excId= reportIn.excutorId;
				}
				else
				{
					ASSERT(taskRun.actorId == reportIn.actorId
						&& taskRun.excId == reportIn.excutorId);
				}
				taskRun.nPercent = reportIn.nPercent;
				taskRun.nCurrStep = reportIn.nStep;
				taskRun.tmLastReport = time(NULL);
				CFWriteLog(0, TEXT(" [actor %d,excid = %d] report task (%s),  step = %d, percent = %d%%"), reportIn.actorId,reportIn.excutorId, reportIn.strGuid,reportIn.nStep, reportIn.nPercent);

			}
		}
		else if (reportIn.nState == embtaskstate_finished)
		{
			ASSERT(taskRun.actorId == reportIn.actorId
				&& taskRun.excId == reportIn.excutorId
				&& taskRun.nState == embtaskstate_dispatched);
			taskRun.nState = embtaskstate_finished;
			taskRun.nPercent = reportIn.nPercent;
			taskRun.nCurrStep = reportIn.nStep;
			taskRun.tmLastReport = time(NULL);
			CFWriteLog(0, TEXT(" [actor %d,excid = %d] report task finished (%s),  step = %d, percent = %d%%"), reportIn.actorId,reportIn.excutorId, reportIn.strGuid,reportIn.nStep, reportIn.nPercent);
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
		actState.nConnState = embConnState_ok;
		actState.nCpuUsage = stateIn.nCpuUsage;
		actState.nMemUsage = stateIn.nMemUsage;
		actState.nDiscUsage = stateIn.nDiscUsage;
		actState.nActorLevel = stateIn.nActorLevel;
		actState.tmLastCheck = time(NULL);
		actState.tmLastReport = actState.tmLastCheck;	
	}

	return TRUE;
}

