#include "StdAfx.h"
#include "TaskActor.h"
#include "TxLogManager.h"
#include "io.h"
#include "StrConvert.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "EMBDocDef.h"
#include "Util.h"
#include "SystemResourceInfo.h"

using namespace EMB;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

CTaskActor::CTaskActor(void)
{
	m_bRuning = FALSE;
	m_nActiveConn = 0;
	nfgRetryMax = 3;
	m_actorconnMain.SetActorConnectorCallback(this);
	m_actorconnSlave.SetActorConnectorCallback(this);
	m_pExcutorMgr = CExcutorMgr::GetExcutorMgr();
	ASSERT(m_pExcutorMgr);
	// -------------------------
	m_strTaskXmlPath = GetAppPath().c_str(); // 执行程序目录
	m_strTaskXmlPath += "\\TaskXml";
	CreateDirectory(m_strTaskXmlPath, NULL); // 创建目录
	// -----------------------------
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
	else if (guidIn == GuidEMBPlugin_IConfig)
	{
		pInterfaceOut = dynamic_cast<IPluginConfigInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (GuidEMBPlugin_IActorUI == guidIn)
	{
		pInterfaceOut = dynamic_cast<IActorUI*>(this);
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
	addrLocal.sin_addr.S_un.S_addr = htonl( INADDR_ANY );

	m_actorconnMain.SetScokAddr(&m_ActRegInfo.addrMain, &addrLocal);
	m_actorconnMain.SetActorId(m_ActRegInfo.actorId);
	m_actorconnSlave.SetScokAddr(&m_ActRegInfo.addrSlave, &addrLocal);
	m_actorconnSlave.SetActorId(m_ActRegInfo.actorId);

	m_pExcutorMgr->Init(m_ActRegInfo.strExcPath, m_ActRegInfo, this);
	HRESULT hr = m_actorconnMain.Run();
	MUSTBESOK(hr);
	hr = m_actorconnSlave.Run();
	MUSTBESOK(hr);
	hr = m_pExcutorMgr->Run();
	MUSTBESOK(hr);
	
	m_resMon.Run();
	m_bRuning = TRUE;
	return S_OK;
}

HRESULT CTaskActor::Stop_Plugin()
{
	m_bRuning = FALSE;
	m_actorconnMain.Stop();
	m_actorconnSlave.Stop();
	m_pExcutorMgr->Stop();

	m_resMon.Stop();

	return S_OK;
}

HRESULT EMB::CTaskActor::OnActorConnectorMsg(CString& strInfo, CString& strRet)
{
	//getxmltype
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(strInfo, mainInfo);
	// 提交任务
	if (mainInfo.nType == embxmltype_task)
	{
		CFWriteLog(0, TEXT("receive task %s"),mainInfo.guid);
		TXGUID guid = String2Guid(mainInfo.guid);
		if (guid == GUID_NULL)
		{
			ASSERT(FALSE);
			return EMBERR_INVALIDARG;
		}
		//dispatched a task
		// 启动执行者进程
		EXCUTORID excId = m_pExcutorMgr->CreateNewExcutor();
		if (excId != INVALID_ID)
		{
			CFWriteLog(0, TEXT("assign task to excutor %d"),excId);

			//save task to it
			ST_TASKINACTOR task;
			task.taskGuid = guid;
			task.strTask = strInfo;
			task.nCurrStep = 0;
			task.nState = embtaskstate_dispatching;
			CAutoLock lock(&m_csmapLock);
			
				// 12-24,执行过的任务可以重新执行
			MAPTASKINACTOR::iterator temItor = m_mapTaskinActor.find(guid);
			if(temItor != m_mapTaskinActor.end())
			{
				CFWriteLog(0, TEXT("---task already exist in actor, remove task"));
				m_mapTaskinActor.erase(temItor);
			}

			//if(m_mapTaskinActor.find(guid) == m_mapTaskinActor.end())
			{
				//add to map and wait for embxmltype_excOnIdle message of actor
				AddTask(guid, task);
				m_mapExcTask[excId] = guid;
				m_pExcutorMgr->SetExecutorState(excId, EXE_ASSIGN); // 置为已分配
			}
			//else
			//{
			//	CFWriteLog(0, TEXT("---task already exist in actor"));
			//	//ASSERT(FALSE);
			//	// 任务已提交
			//	/*ST_TASKREPORT tskReport;
			//	tskReport.actorId = m_ActRegInfo.actorId;
			//	tskReport.strGuid = guid;
			//	tskReport.nSubErrorCode = EMBERR_EXISTED;
			//	tskReport.nState = embtaskstate_error;

			//	tskReport.ToString(strRet);*/
			//}
		}
		else // 无可用的Executor
		{
			CFWriteLog(0, TEXT("no available executor"));
			// 提交失败
			ST_TASKREPORT tskReport;
			tskReport.strGuid = guid;
			tskReport.actorId = m_ActRegInfo.actorId;
			tskReport.nSubErrorCode = EMBERR_FULL; // 任务已达上限数量
			tskReport.nState = embtaskstate_error;

			tskReport.ToString(strRet);
		}
	}
	else if (mainInfo.nType == embxmltype_taskReport) // 查询任务
	{
		//report task progress
		ST_TASKREPORT report;
		report.FromString(strInfo);
		TXGUID guid = String2Guid(report.strGuid);

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
					report.actorId = m_ActRegInfo.actorId;
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
				// 存在情况：管理Server.exe临时退出了，Actor.exe 正常运行，任务结束后从m_mapTaskinActor删除了任务；
				//           Server.exe启动后，查询任务状态时，对应任务已不存在于m_mapTaskinActor
				// 此时查询TaskXml\目录中是否有对应的xml文件?
				ST_TASKREPORT tskInfor;
				if (QueryXmlFile(guid, tskInfor))
				{
					report = tskInfor;
				}
				else
				{
					report.nState = embtaskstate_error;
				}
			}

			//report
			report.ToString(strRet);
		}
	}
	else if (mainInfo.nType == embxmltype_svrActive) // 任务状态改变
	{
		//report actor state 
		ST_SVRACTIVEINFO activeInfo;
		activeInfo.FromString(strInfo);
		CFWriteLog(0, TEXT("svr(%d) state change to %d"),activeInfo.nMaster,  activeInfo.nActive);
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
				//safe, not change
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else if (mainInfo.nType == embxmltype_actorState) // 查询Actor.exe状态
	{
		//active svr changed;
		ST_ACTORSTATE actorInfo;
		actorInfo.actorId = m_ActRegInfo.actorId;
		actorInfo.nActorLevel = m_ActRegInfo.nActorLevel;
		actorInfo.nConnState = embConnState_ok;
		// cpu 使用率
		actorInfo.nCpuUsage = m_resMon.GetUsage(restype_Processor);
		actorInfo.nDiscIOUsage = m_resMon.GetUsage(restype_PhysicalDisk);
		// memory 使用率
		CMemoryRes memRes;
		memRes.GetInfor();
		actorInfo.nMemUsage = m_resMon.GetUsage(restype_Memory);
		actorInfo.nExcResUsage = m_pExcutorMgr->GetExcResUsage();
		actorInfo.nNetIOUsage = m_resMon.GetUsage(restype_Network);;
		TRACE(TEXT("svr request report actor state! cpu =%d, mem = %d, disc = %d, net = %d, exc = %d"), actorInfo.nCpuUsage, actorInfo.nMemUsage, actorInfo.nDiscIOUsage, actorInfo.nNetIOUsage, actorInfo.nExcResUsage);


		actorInfo.ToString(strRet);
	}
	else
	{
		ASSERT(FALSE);
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
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKACTOR, TEXT("TaskActor"));

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
		ST_ACTORCONFIG regIn;
		regIn.FromString(szIn);
		if (regIn.strExcPath.IsEmpty())
		{
			//set default excutor path
			CString strPath = GetAppPath().c_str();
			strPath += TEXT("\\exc\\EMBExternalExcutor.exe");
			regIn.strExcPath = strPath;
		}
		
		//check valid
		if (regIn.actorId  < 0
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
			ret.nRetVal = S_OK;
		}
	}

	ret.ToString(szOut);
	return ret.nRetVal;
}

HRESULT EMB::CTaskActor::OnExcutorMessage( const EXCUTORID excutorId, CString& szInfoIn )
{
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(szInfoIn, mainInfo);
	if (mainInfo.nType == embxmltype_excOnIdle) // 执行者进程空闲
	{
		CFWriteLog(0, TEXT("excutor %d report idle"), excutorId);
		OnExcutorIdle(excutorId);
	}
	else if (mainInfo.nType == embxmltype_taskReport) // 任务状态更新
	{
		//excutor report
		//CFWriteLog(0, TEXT("exc%d report %s"), excutorId, szInfoIn);
		ST_TASKREPORT report;
		report.FromString(szInfoIn);
		TXGUID guid = String2Guid(report.strGuid);
		ASSERT (guid != GUID_NULL);
		BOOL bReport = FALSE;
		ST_TASKINACTOR tmpReport;
		{
			CAutoLock lock(&m_csmapLock);
			MAPEXCTASKS::iterator itf = m_mapExcTask.find(excutorId);
			if (itf != m_mapExcTask.end())
			{
				ASSERT(itf->second == guid);
			}
			else
			{
				m_mapExcTask[excutorId] = guid;
			}
			//check task state that assigned to the excutor
			MAPTASKINACTOR::iterator itftask = m_mapTaskinActor.find(guid);
			if (itftask != m_mapTaskinActor.end())
			{
				ST_TASKINACTOR& taskRef = itftask->second;
				taskRef.nCurrStep = report.nStep;
				taskRef.nState = report.nState;
				taskRef.nPercent = report.nPercent;
				taskRef.nSubErrorCode = report.nSubErrorCode; // 具体错误

				tmpReport = taskRef;
				bReport = TRUE;
				if (taskRef.nState == embtaskstate_finished) // 任务完成，m_mapTaskinActor 删除任务
				{
					CFWriteLog(0, TEXT("task finished %s"), Guid2String(taskRef.taskGuid));
					
					m_mapExcTask.erase(excutorId);
					m_pExcutorMgr->SetExecutorState(excutorId, EXE_IDLE); // 置空闲

					// 保存任务结果
					TaskResultSaveXmlFile(report);
				}
				else if (embtaskstate_error == taskRef.nState)
				{
					CFWriteLog(0, TEXT("excutor report error: %s SubError:%X"), 
							Guid2String(taskRef.taskGuid), report.nSubErrorCode);
// 					// 不需重置任务
// 					if (!report.NeedResetTask())
// 					{
// 						CFWriteLog(0, TEXT("task submit error: %s SubError:%X"), 
// 							Guid2String(taskRef.taskGuid), report.nSubErrorCode);
// 						
// 						m_pExcutorMgr->SetExecutorState(excutorId, EXE_IDLE); // 置空闲
// 					}
					TXGUID guidTmp = taskRef.taskGuid;
					m_mapTaskinActor.erase(guidTmp);
					m_mapExcTask.erase(excutorId);


					// 保存任务结果
					TaskResultSaveXmlFile(report);
				}
			}
		}

		if (bReport)
		{
			// 更新任务状态
			ReportTaskState(tmpReport);
		}
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
			}
			else
			{
				BOOL bReSuc = FALSE;
				/*++taskInfoRef.nRetry;
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
						m_pExcutorMgr->SetExecutorState(newExcId, EXE_ASSIGN); // 已分配
					}
					else
					{
						CFWriteLog(0, TEXT("retry, but not find available executor"));
						//ASSERT(FALSE);
					}
				}
				else
				{
					ASSERT(FALSE);
				}*/

				if (!bReSuc)
				{
					taskInfoRef.nState = embtaskstate_error;
					ReportTaskState(taskInfoRef);
				}
				TXGUID tmpGuid= taskInfoRef.taskGuid;
				m_mapTaskinActor.erase(tmpGuid);

				
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
	report.actorId = m_ActRegInfo.actorId;
	report.excutorId = infoIn.excId;
	report.nState = infoIn.nState;
	report.nPercent = infoIn.nPercent;
	report.nStep = infoIn.nCurrStep;
	report.strGuid = infoIn.taskGuid;
	report.strGuid = Guid2String(infoIn.taskGuid);
	report.nSubErrorCode = infoIn.nSubErrorCode;   // 具体错误

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
		MAPEXCTASKS::iterator itf = m_mapExcTask.find(excutorId); // 查询任务
		if (itf != m_mapExcTask.end())
		{
			MAPTASKINACTOR::iterator itftask = m_mapTaskinActor.find(itf->second);
			if (itftask != m_mapTaskinActor.end())
			{
				//check not assigned;
				ST_TASKINACTOR& taskInfoRef = itftask->second;
				if (taskInfoRef.excId == INVALID_ID)
				{
					strTask = taskInfoRef.strTask; // 任务 xml信息
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

		CFWriteLog(0, TEXT("send task to excutor %d"), excutorId);
		m_pExcutorMgr->SendToExcutor(excutorId, strTask);

	}

	return TRUE;
}

bool EMB::CTaskActor::TaskResultSaveXmlFile( ST_TASKREPORT& tskReport )
{
	// 按天保存xml
	CTime tCurrent = CTime::GetCurrentTime();
	CString strFileName;
	strFileName.Format("%s\\%s.xml", m_strTaskXmlPath, tCurrent.Format("%Y-%m-%d"));

	return CUtil::XmlFileAppend(strFileName, tskReport);
}

bool EMB::CTaskActor::QueryXmlFile( const CString& strTaskGuid, ST_TASKREPORT& tskInfor )
{
	CTime tCurrent = CTime::GetCurrentTime();
	CString strFileName;
	strFileName.Format("%s\\%s.xml", m_strTaskXmlPath, tCurrent.Format("%Y-%m-%d"));

	bool suc = CUtil::QueryXmlFile(strFileName, strTaskGuid, tskInfor);

	return suc;
}

HRESULT EMB::CTaskActor::GetExecutors( vector<CString>& vExecutor )
{
	vExecutor.clear();

	// get running executor
	vector<ST_EXCUTORINFO> vST;
	HRESULT hr = m_pExcutorMgr->GetExecutors(vST);
	CString strInfo;
	ST_TASKINACTOR tsk;

	// get task in executor
	for (int i = 0; i < vST.size(); ++i)
	{
		if (FindTask(vST[i].excutorId, tsk))
		{
			vST[i].m_strTaskGuid = tsk.taskGuid; // taskguid
			vST[i].m_strRunStep.Format("%d", tsk.nCurrStep);
			vST[i].m_nPercent = tsk.nPercent;
		}
		else
		{
			vST[i].m_strTaskGuid.Empty();
			vST[i].m_strRunStep.Empty();
			vST[i].m_nPercent = 0;
		}

		// insert
		vST[i].ToString(strInfo);
		vExecutor.push_back(strInfo);
	}

	return hr;
}

bool EMB::CTaskActor::FindTask( const EXCUTORID& strExecutorId, ST_TASKINACTOR& tsk )
{
	CAutoLock lock(&m_csmapLock);
	bool suc = false;

	// 获得任务信息
	MAPEXCTASKS::iterator itor = m_mapExcTask.find(strExecutorId);
	if (itor != m_mapExcTask.end())
	{
		MAPTASKINACTOR::iterator tskItor = m_mapTaskinActor.find(itor->second);

		if (tskItor != m_mapTaskinActor.end())
		{
			tsk = tskItor->second;
			suc = true;
		}
	}

	return suc;
}

HRESULT EMB::CTaskActor::GetTaskInActor( vector<CString>& vTask )
{
	vTask.clear();

	//
	CAutoLock lock(&m_csmapLock);

	MAPTASKINACTOR::iterator itor = m_mapTaskinActor.begin();
	CString strInfo;

	for (;itor != m_mapTaskinActor.end(); ++itor )
	{
		ST_TASKRUNSTATE tsk;
		tsk.guid = itor->second.taskGuid.guid;
		tsk.nState = itor->second.nState;
		tsk.nCurrStep = itor->second.nCurrStep;
		tsk.tmLastReport = itor->second.tmLastReport;
		tsk.tmCommit = itor->second.tmCommit; // 提交时间

		tsk.ToString(strInfo);
		vTask.push_back(strInfo);
	}

	return S_OK;
}

bool EMB::CTaskActor::AddTask( TXGUID& tskGuid, const ST_TASKINACTOR& tsk )
{
	if (tskGuid.guid == GUID_NULL)
	{
		return false;
	}

	m_mapTaskinActor[tskGuid] = tsk;

	if (m_mapTaskinActor.size() > 100)
	{
		// 保留1天之内的任务信息
		time_t tNow = time(NULL);
		static long sec = 24 * 60 * 60;
		tNow -= sec; // 减去1天

		for (MAPTASKINACTOR::iterator itor = m_mapTaskinActor.begin(); itor != m_mapTaskinActor.end();)
		{
			if (itor->second.tmCommit < tNow)
			{
				m_mapTaskinActor.erase(itor++);
			}
			else
			{
				++itor;
			}
		}
	}
	
	return true;
}


