/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBStorage.h
	author:		tianyx
	
	purpose:	任务存储类，将任务存储于内存中
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include <map>
#include "FGlobal.h"
#include "AutoCritSec.h"
#include "EmbStructDef.h"
using namespace std;

//基本任务信息结构体
struct ST_TASKSAVEDATA
{
	ST_TASKBASIC basic;            //基本任务信息
	CTaskString strTask;           //实际任务XML内容

	TXGUID guid;                   //任务GUID
	int nRetry;                    //重试次数
	DISPATCHID nDispatchID;        //任务管理标识ID
	time_t stSubmit;               //提交时间

	ST_TASKSAVEDATA()
	{
		nRetry = 0;
		nDispatchID = INVALID_ID;
	}
};

//任务优先级信息
struct ST_TASKPRIKEY
{
	int nPriority;
	time_t stSubmit;
	TXGUID guid;
	//比较优先级
	//当优先级不同时，比较优先级
	//优先级相同时，比较提交时间，越早提交优先级越高
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

//任务GUID与任务信息对应图
typedef map<TXGUID, ST_TASKSAVEDATA> MAPMEMTASKPOOL;
//任务优先级与任务GUID对应图
typedef map<ST_TASKPRIKEY, TXGUID> MAPTASKNOTASSIGNED;
// 已分配任务列表
typedef map<TXGUID, TXGUID> MAPTXGUID;
// 已分配队列，一个任务管理可能有多个已分配任务
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
	//智能锁
	CAutoCritSec m_csPoolLock;

	// 所有任务列表
	MAPMEMTASKPOOL m_mapTaskPool;
	//未分配任务对应表
	MAPTASKNOTASSIGNED m_mapNotAssigned;
	//已分配任务对应表
	MAPTASKASSIGNED m_mapAssigned;

private:
	//最大重试次数
	int m_nMaxRetry;
};


class CEMBStorageDB:
	public IPluginStorageInterface,
	public IPluginTaskCommit
{
public:
	CEMBStorageDB(void);
	virtual ~CEMBStorageDB(void);

	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);


	virtual HRESULT UpdateTaskToStorage(const DISPATCHID nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage( const DISPATCHID nDispatchID,int nMinPriority, int nDesiredNum, VECTASKS& vTasks );
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks);

	// 设置数据库连接字符串
	void SetDBConnectString(CString strDBCon);

private:
	CString GetTaskKind(ST_TASKSAVEDATA taskInfo);
	CString Timet2CString(time_t tm);

private:
	CString m_strDBCon;

	//智能锁
	CAutoCritSec m_csPoolLock;
};

}