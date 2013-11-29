#include "StdAfx.h"
#include "EMBStorage.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "EMBDocDef.h"
#include "ADOCtrl.h"
#include "Log.h"

using namespace EMB;
CEMBStorageMem::CEMBStorageMem(void)
{
	m_nMaxRetry = 10;
}

CEMBStorageMem::~CEMBStorageMem(void)
{
}

//////////////////////////////////////////////////////////////////////////
CEMBStorageDB::CEMBStorageDB(void)
{
	m_strDBCon = "";
}

CEMBStorageDB::~CEMBStorageDB(void)
{
}



//////////////////////////////////////////////////////////////////////////
/*
*Description：update Task Status to TaskList for TaskStorage
*Input Param：
*      nDispatchID ：分配任务ID
*      szTaskIn ：更新消息
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CEMBStorageMem::UpdateTaskToStorage( const DISPATCHID nDispatchID, CTaskString& szTaskIn )
{
	//Direct return?
	return S_OK;

	// 分析消息
	ST_TASKUPDATE updateInfo;
	updateInfo.FromString(szTaskIn);
	// 判断任务状态
	if (updateInfo.nUpdateType == embtaskupdatetype_none)
	{
		ASSERT(FALSE);
		return E_INVALIDARG;
	}

	TXGUID guid = updateInfo.guid;
	//判断是否为任务完成状态
	if (updateInfo.nUpdateType == embtaskupdatetype_finish)
	{
		//是否正常完成
		int nEndState = updateInfo.data_end.nEndState;
		if (nEndState == embtaskstate_finished)
		{
			//remove from all map
			CAutoLock lock(&m_csPoolLock);
			//移除已分配队列
			MAPTASKASSIGNED::iterator itDispath = m_mapAssigned.find(nDispatchID);
			// 等于最后说明没有找到，当找到的情况下
			if (itDispath != m_mapAssigned.end())
			{
				//second也是一个map,第一个map中的第二个map中的一对信息移除
				itDispath->second.erase(guid);
			}

			//任务任务池
			m_mapTaskPool.erase(guid);
			//save finished work;

		}
		else if (nEndState == embtaskstate_error)
		{
			//if retry time not max,reset task;
			//else remove from all map;
			ASSERT(FALSE);
			MAPTASKASSIGNED::iterator itDispath = m_mapAssigned.find(nDispatchID);
			if (itDispath != m_mapAssigned.end())
			{
				itDispath->second.erase(guid);
			}

			MAPMEMTASKPOOL::iterator itf = m_mapTaskPool.find(guid);
			if (itf != m_mapTaskPool.end())
			{
				ST_TASKSAVEDATA& sdata = itf->second;
				if (itf->second.nRetry > m_nMaxRetry)
				{
					//eraase it 
					m_mapTaskPool.erase(itf);
				}
				else
				{
					sdata.nRetry++;
					//add to not assign list
					ST_TASKPRIKEY prikey;
					prikey.guid = guid;
					prikey.nPriority = sdata.basic.nPriority;
					prikey.stSubmit = sdata.stSubmit;
					m_mapNotAssigned[prikey] = guid;
				}
			}

		}
	}
	else if (updateInfo.nUpdateType == embtaskupdatetype_del)
	{
		//如果是删除操作
		//to be add...
		ASSERT(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
	


}

/*
*Description：从任务存储中获取任务
*Input Param：
*      nDispatchID ：分配任务任务管理ID
*      nMinPriority ：特殊任务分配给特定ACTOR执行，待实现
*      nDesiredNum：获取任务数
*      vTasks ：返回任务列表
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CEMBStorageMem::FetchTaskFromStorage( const DISPATCHID nDispatchID,int nMinPriority, int nDesiredNum, VECTASKS& vTasks )
{
	CAutoLock lock(&m_csPoolLock);
	size_t nSize = m_mapNotAssigned.size();
	if (nSize == 0)
	{
		return EMBERR_EMPTY;
	}

	if (nDesiredNum > nSize)
	{
		nDesiredNum = nSize;
	}
	MAPTASKASSIGNED::iterator itDispatch = m_mapAssigned.find(nDispatchID);
	//如果没有找到，增加已分配任务记录到已分配任务列表
	if (itDispatch == m_mapAssigned.end())
	{
		MAPTXGUID tmpmap;
		m_mapAssigned[nDispatchID] = tmpmap;
		itDispatch = m_mapAssigned.find(nDispatchID);
	}

	MAPTASKNOTASSIGNED::iterator itb = m_mapNotAssigned.begin();
	MAPTASKNOTASSIGNED::iterator ite = m_mapNotAssigned.end();

	for (int i = 0; i < nDesiredNum; ++i, ++itb)
	{
		MAPMEMTASKPOOL::iterator itfTask = m_mapTaskPool.find(itb->second);
		if(itfTask != m_mapTaskPool.end())
		{
			//将满足条件的任务添加到任务列表中
			(itDispatch->second)[itb->second]; //?
			vTasks.push_back(itfTask->second.strTask);
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	//erase assigned form not assigned list
	--itb;
	m_mapNotAssigned.erase(m_mapNotAssigned.begin(), itb);


	return S_OK;
}

/*
*Description：提交任务
*Input Param：
*      szTaskIn ：任务描述XML字母串
*      szRet ：返回消息
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CEMBStorageMem::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	CAutoLock lock(&m_csPoolLock);
	ST_TASKSAVEDATA sdata;
	GetTaskBasicInfo(szTaskIn, sdata.basic);
	if (!sdata.basic.strGuid.IsEmpty())
	{
		TXGUID guid = String2Guid(sdata.basic.strGuid);
		if (guid != GUID_NULL)
		{
			if (m_mapTaskPool.find(guid) == m_mapTaskPool.end())
			{
				sdata.strTask = szTaskIn;
				m_mapTaskPool[guid] = sdata;
				//save to not assigned map
				ST_TASKPRIKEY prikey;
				prikey.guid = guid;
				prikey.nPriority = sdata.basic.nPriority;
				prikey.stSubmit = sdata.stSubmit;
				m_mapNotAssigned[prikey] = guid;
				
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
	else
	{
		ASSERT(FALSE);
	}
	
	return S_OK;
}

/*
*Description：获取已分配任务列表
*Input Param：
*      nDispatchID ：分配任务管理ID
*      vTasks ：返回任务列表
*Return Param：返回成功或失败
*History：
*/
HRESULT EMB::CEMBStorageMem::GetDispatchedTaskFromStorage( const DISPATCHID nDispatchID, VECTASKS& vTasks )
{
	CAutoLock lock(&m_csPoolLock);
	MAPTASKASSIGNED::iterator itf = m_mapAssigned.find(nDispatchID);
	if (itf != m_mapAssigned.end())
	{
		MAPTXGUID::iterator itb = itf->second.begin();
		MAPTXGUID::iterator ite = itf->second.end();
		for (; itb != ite; ++itb)
		{
			MAPMEMTASKPOOL::iterator itftask = m_mapTaskPool.find(itb->second);
			if (itftask != m_mapTaskPool.end())
			{
				vTasks.push_back(itftask->second.strTask);
			}
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//implement for CEMBStorageDB
//////////////////////////////////////////////////////////////////////////

HRESULT EMB::CEMBStorageDB::UpdateTaskToStorage( const DISPATCHID nDispatchID, CTaskString& szTaskIn )
{
	// 启动数据库连接
	CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase(m_strDBCon);
	if(!dbCtrl.OpenDB())
	{
		return FALSE;
	}

	CString strSQL = "";
	// 分析消息
	ST_TASKUPDATE updateInfo;
	updateInfo.FromString(szTaskIn);
	// 判断任务状态
	if (updateInfo.nUpdateType == embtaskupdatetype_none)
	{
		// 重置任务状态
		strSQL = "update T_EMBTask set nState = 0 where strTaskID='";
		strSQL += Guid2String(updateInfo.guid);
		strSQL += "'";

		if(!dbCtrl.ExecuteSQL(strSQL))
		{
			CFWriteLog(LOGKEY_TASKSTORAGE,"数据库存储执行SQL语句失败!");
			CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);

			dbCtrl.CloseDB();
			return S_FALSE;
		}
		
	}

	TXGUID guid = updateInfo.guid;
	//判断是否为任务完成状态
	if (updateInfo.nUpdateType == embtaskupdatetype_finish)
	{
		//是否正常完成
		int nEndState = updateInfo.data_end.nEndState;
		if (nEndState == embtaskstate_finished)
		{
			// 将任务移植到日志表中
			strSQL = "insert into t_EMBTaskLog(strTaskID,nState,Remark) values('";
			strSQL += Guid2String(updateInfo.guid);
			strSQL += "',";

			strSQL += embtaskstate_finished;
			strSQL += ",'";

			strSQL += "成功";
			strSQL += "'";

			if(!dbCtrl.ExecuteSQL(strSQL))
			{
				CFWriteLog(LOGKEY_TASKSTORAGE,"数据库存储插入T_EMBTASKLog SQL语句失败!");
				CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
			}

			// 从任务列表中删除

		}
		else if (nEndState == embtaskstate_error)
		{
			//if retry time not max,reset task;
			//else remove from all map;
			ASSERT(FALSE);
			MAPTASKASSIGNED::iterator itDispath = m_mapAssigned.find(nDispatchID);
			if (itDispath != m_mapAssigned.end())
			{
				itDispath->second.erase(guid);
			}

			MAPMEMTASKPOOL::iterator itf = m_mapTaskPool.find(guid);
			if (itf != m_mapTaskPool.end())
			{
				ST_TASKSAVEDATA& sdata = itf->second;
				if (itf->second.nRetry > m_nMaxRetry)
				{
					//eraase it 
					m_mapTaskPool.erase(itf);
				}
				else
				{
					sdata.nRetry++;
					//add to not assign list
					ST_TASKPRIKEY prikey;
					prikey.guid = guid;
					prikey.nPriority = sdata.basic.nPriority;
					prikey.stSubmit = sdata.stSubmit;
					m_mapNotAssigned[prikey] = guid;
				}
			}

		}
	}
	else if (updateInfo.nUpdateType == embtaskupdatetype_del)
	{
		//如果是删除操作
		//to be add...
		ASSERT(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}

	return S_OK;
}

HRESULT EMB::CEMBStorageDB::FetchTaskFromStorage( const DISPATCHID nDispatchID, int nDesiredNum, VECTASKS& vTasks )
{
	return S_OK;
}

HRESULT EMB::CEMBStorageDB::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	return S_OK;
}

HRESULT EMB::CEMBStorageDB::GetDispatchedTaskFromStorage( const DISPATCHID nDispatchID, VECTASKS& vTasks )
{
	return S_OK;
}

void EMB::CEMBStorageDB::SetDBConnectString(CString strDBCon)
{
	m_strDBCon = strDBCon;
}