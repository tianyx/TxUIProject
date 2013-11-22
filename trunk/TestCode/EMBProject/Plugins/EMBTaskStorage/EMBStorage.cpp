#include "StdAfx.h"
#include "EMBStorage.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "EMBDocDef.h"

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
}

CEMBStorageDB::~CEMBStorageDB(void)
{
}



//////////////////////////////////////////////////////////////////////////

HRESULT EMB::CEMBStorageMem::UpdateTaskToStorage( const DISPATCHID nDispatchID, CTaskString& szTaskIn )
{
	return S_OK;
	ST_TASKUPDATE updateInfo;
	updateInfo.FromString(szTaskIn);
	if (updateInfo.nUpdateType == embtaskupdatetype_none)
	{
		ASSERT(FALSE);
		return E_INVALIDARG;
	}

	TXGUID guid = updateInfo.guid;
	if (updateInfo.nUpdateType == embtaskupdatetype_finish)
	{
		int nEndState = updateInfo.data_end.nEndState;
		if (nEndState == embtaskstate_finished)
		{
			//remove from all map
			CAutoLock lock(&m_csPoolLock);
			MAPTASKASSIGNED::iterator itDispath = m_mapAssigned.find(nDispatchID);
			if (itDispath != m_mapAssigned.end())
			{
				itDispath->second.erase(guid);
			}

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
		//to be add...
		ASSERT(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
	


}

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
			(itDispatch->second)[itb->second];
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
