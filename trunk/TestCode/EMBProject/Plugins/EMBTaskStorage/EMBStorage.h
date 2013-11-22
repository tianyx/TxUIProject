/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBStorage.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include <map>
#include "FGlobal.h"
#include "AutoCritSec.h"
#include "EmbStructDef.h"
using namespace std;
struct ST_TASKSAVEDATA
{
	ST_TASKBASIC basic;
	CTaskString strTask;

	TXGUID guid;
	int nRetry;
	DISPATCHID nDispatchID;
	time_t stSubmit;

	ST_TASKSAVEDATA()
	{
		nRetry = 0;
		nDispatchID = INVALID_ID;
	}
};

struct ST_TASKPRIKEY
{
	int nPriority;
	time_t stSubmit;
	TXGUID guid;
	bool operator <(const ST_TASKPRIKEY& other) const
	{
		if (nPriority != other.nPriority)
		{
			return nPriority > other.nPriority;
		}
		else if (stSubmit != other.stSubmit)
		{

			return stSubmit < other.stSubmit;
		}
		else
		{
			return guid < other.guid;
		}

	}
};

typedef map<TXGUID, ST_TASKSAVEDATA> MAPMEMTASKPOOL;
typedef map<ST_TASKPRIKEY, TXGUID> MAPTASKNOTASSIGNED;

typedef map<TXGUID, TXGUID> MAPTXGUID;
typedef map<DISPATCHID, MAPTXGUID> MAPTASKASSIGNED;

namespace EMB{
class CEMBStorageMem:
	public IPluginStorageInterface,
	public IPluginTaskCommit
{
public:
	CEMBStorageMem(void);
	virtual ~CEMBStorageMem(void);

	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);

	virtual HRESULT UpdateTaskToStorage(const DISPATCHID nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage(const DISPATCHID nDispatchID, int nMinPriority, int nDesiredNum, VECTASKS& vTasks);
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks);

private:
	CAutoCritSec m_csPoolLock;

	MAPMEMTASKPOOL m_mapTaskPool;
	MAPTASKNOTASSIGNED m_mapNotAssigned;
	MAPTASKASSIGNED m_mapAssigned;

private:
	int m_nMaxRetry;
};


class CEMBStorageDB:public IPluginStorageInterface
{
public:
	CEMBStorageDB(void);
	virtual ~CEMBStorageDB(void);

	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);


	virtual HRESULT UpdateTaskToStorage(const DISPATCHID nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage(const DISPATCHID nDispatchID, int nDesiredNum, VECTASKS& vTasks);
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks);
};

}