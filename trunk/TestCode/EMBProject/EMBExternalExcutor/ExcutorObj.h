/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:02
	filename: 	ExcutorObj.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "FGlobal.h"
#include "embdefine.h"
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"
#include "AutoCritSec.h"
#include "EMBMessageDef.h"
using namespace EMB;
struct ST_SUBTASKDATA
{
	int nType;
	CString strName;
	CString strSubTask;
};

interface IExcutorCallback
{

};
class CExcutorObj:public ITaskReportToExcutorInterface
{
private:
	CExcutorObj();
	~CExcutorObj();
public:
	BOOL Init(const EXCUTORID& guidIn, ACTORID actorId, HWND hActor, HWND hMain);
	BOOL Start();
	BOOL Stop();
	BOOL Quit();
	BOOL ExcuteTask(ST_EMBTRANSMSG& taskIn, ST_EMBTRANSMSG& retOut);
	BOOL SendToActor(CString& strMsg);

	HRESULT SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	HRESULT WndMsgPoolCheckLoop();
	HRESULT TaskCheckLoop();

	//interface for ITaskReportToExcutorInterface
	virtual HRESULT OnDllReportTaskProgress(const CTaskString& szInfo);

private:
	BOOL OnExcutorRegistered();
	BOOL OnExcutorUnRegister(ST_EXCUTORINFO& infoIn);
	HRESULT OnActorMessage(ST_EMBWNDMSG& msgIn);
	HRESULT OnActorMessage(CString& strInfo, CString& strRet);
	void ClearMsgPool();
	HRESULT InitTask();
	void OnTaskProgress(int nPercent);

	BOOL LunchTaskDll(int nTaskType);
	BOOL UnLoadTaskDll();
	HRESULT LaunchTask();

private:
	HANDLE CreateMappingFile();
public:
	HWND m_hwndExcMsg;
private:
	CString m_strTask;
	vector<ST_SUBTASKDATA> m_vSubTasks;
	ITaskWorkerCallInterface * m_pTaskDllInterface;
	ST_LOADEDPLUGIN m_taskDll;
	ST_TASKRUNSTATE m_runState;
	EXCUTORID m_ExcutorGuid;
	ACTORID m_actorId;
	VECPLUGINFOS m_vexcInfo;
	HWND m_hwndMain;
	HWND m_hwndActor;
	HANDLE m_hThreadMsgLoop;
	HANDLE m_hThreadMsgPoolCheck;
	HANDLE m_hThreadTaskCheck;
	HANDLE m_hEventQuitLoop;
	HANDLE m_hEventPoolMsgArrival;
	HANDLE m_hMapping;
	CString m_strActorMappingName;
	CAutoCritSec m_csPipeWrite;
	CAutoCritSec m_csMsgPool;
	VECWNDMSG m_vMsgPool;
	BOOL m_bRegistered;
	CAutoCritSec m_csTaskInfo;


	int nfgTaskCheckInterval;
	int nfgIdleReportInterval;
	int nfgIdleTimeoutForQuit;


	static CExcutorObj* m_spExcObj;


public:
	static CExcutorObj* GetExcutorObj(const EXCUTORID& guidIn, ACTORID actorID, HWND hActor, HWND hMain);
	static BOOL Release();
};