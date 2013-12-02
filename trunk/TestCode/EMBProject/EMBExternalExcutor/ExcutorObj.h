/********************************************************************
	created:	2013/11/23
	filename: 	ExcutorObj.h
	author:		tianyx
	purpose:	CExcutorObj 定义
    History: 
*********************************************************************/

#pragma once
#include "FGlobal.h"
#include "embdefine.h"
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"
#include "AutoCritSec.h"
#include "EMBMessageDef.h"

using namespace EMB;

// 分步任务
struct ST_SUBTASKDATA
{
	int		nType;		// 类型
	CString strName;
	CString strSubTask; // 信息
};

class CExcutorObj : public ITaskReportToExcutorInterface
{
private:
	CExcutorObj();
	~CExcutorObj();

public:
	BOOL Start();
	BOOL Stop();
	BOOL Quit();
	/*
    Description：提交任务
	Input：		taskIn 任务信息； retOut 返回信息
	Return:		TRUE 成功; FALSE 失败
	History：
	*/
	BOOL ExcuteTask(ST_EMBTRANSMSG& taskIn, ST_EMBTRANSMSG& retOut);
	/*
    Description：向Actor发送消息
	Input：		strMsg 消息内容
	Return:		TRUE 成功; FALSE 失败
	History：
	*/
	BOOL SendToActor(CString& strMsg);

	/*
    Description：将消息保存在 m_vMsgPool
	Input：		hwnd 接受消息的HWND; message 消息标识; wparam 消息发送者HWND; lparam 消息字符数
	Return:		S_OK 成功
	History：
	*/
	HRESULT SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	HRESULT WndMsgPoolCheckLoop();
	HRESULT TaskCheckLoop();

	/*
    Description：任务进度信息
	Input：		szInfo 任务信息
	Return:		S_OK 成功
	History：
	*/
	virtual HRESULT OnDllReportTaskProgress(const CTaskString& szInfo);

public://for unit test
	BOOL TestRunTask(CString& strTaskIn);
private:
	BOOL	OnExcutorRegistered();
	BOOL	OnExcutorUnRegister(ST_EXCUTORINFO& infoIn);
	HRESULT OnActorMessage(ST_EMBWNDMSG& msgIn);
	HRESULT OnActorMessage(CString& strInfo, CString& strRet);
	void	ClearMsgPool();
	HRESULT InitTask();
	void	OnTaskProgress(int nPercent);

	BOOL	LunchTaskDll(int nTaskType);
	BOOL	UnLoadTaskDll();
	HRESULT LaunchTask();

	HANDLE  CreateMappingFile();

	/*
    Description：初始化
	Input：		tagExecutorReg 执行者进程信息
	Return:		true 成功
	History： 11-26 接口参数调整,使用ST_EXCUTORREG
	*/
	BOOL Init(ST_EXCUTORREG& tExecutorReg, HWND hMain);

public:
	HWND m_hwndExcMsg;	// 消息标识

private:
	CString						m_strTask;				// 任务信息
	vector<ST_SUBTASKDATA>		m_vSubTasks;			// 分步任务数组
	ITaskWorkerCallInterface	*m_pTaskDllInterface;	// 任务工作者回调接口
	ST_LOADEDPLUGIN				m_taskDll;				// 加载任务插件
	ST_TASKRUNSTATE				m_runState;				// 任务运行状态
	ST_EXCUTORREG				m_executorReg;			// 执行者进程信息
	VECPLUGINFOS				m_vexcInfo;				// 插件信息数组

	HWND	m_hwndMain;
	HANDLE	m_hThreadMsgLoop;		// 消息分发线程
	HANDLE	m_hThreadMsgPoolCheck;	// 消息检查线程
	HANDLE	m_hThreadTaskCheck;		// 任务检查线程
	HANDLE	m_hEventQuitLoop;		// QuitLoop 事件
	HANDLE	m_hEventPoolMsgArrival; // MsgArrival 事件
	HANDLE	m_hMapping;

	CAutoCritSec	m_csPipeWrite;			// PipeWrite 临界区
	CAutoCritSec	m_csMsgPool;			// MsgPool 临界区
	CAutoCritSec	m_csTaskInfo;			// TaskInfo 临界区
	VECWNDMSG		m_vMsgPool;				// 消息数组
	BOOL			m_bRegistered;
	
	int nfgTaskCheckInterval;	// 任务检查间隔
	int nfgIdleReportInterval;	// 
	int nfgIdleTimeoutForQuit;

	static CExcutorObj* m_spExcObj;

public:
	static CExcutorObj* GetExcutorObj(ST_EXCUTORREG& tExecutorReg, HWND hMain);
	static BOOL Release();
};