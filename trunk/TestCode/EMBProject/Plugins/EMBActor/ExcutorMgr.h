/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	ExcutorMgr.h
	author:		tianyx
	
	purpose:	管理Executor进程,处理Executor的消息
*********************************************************************/
#pragma once
#include "fglobal.h"
#include "EmbStructDef.h"
#include "AutoCritSec.h"
#include <map>
using namespace std;

interface IExcutorMgrInterface
{
	virtual HRESULT SendToExcutor(const EXCUTORID excutorId,CString& szInfo) = 0;
};

interface IExcutorMsgCallBack
{
	virtual HRESULT OnExcutorMessage(const EXCUTORID excutorId, CString& szInfoIn) = 0;
	virtual HRESULT OnExcutorExit(const EXCUTORID excutorId) = 0;
};


typedef map<EXCUTORID, ST_EXCUTORINFO> MAPEXCUTORS;

class CExcutorMgr
{
private:
	CExcutorMgr(void);
	virtual ~CExcutorMgr(void);

public:
	BOOL Init(LPCTSTR strExcPathIn, ST_ACTORCONFIG& actCfg, IExcutorMsgCallBack* pCallBack);
	HRESULT Run();
	HRESULT Stop();
	/*
    Description：创建执行者进程
	Input：		
	Return:		进程标识
	History：
	*/
	EXCUTORID CreateNewExcutor();
	BOOL StopExcutor(const EXCUTORID guidIn);
	//interface for IExcutorMgrInterface
	// 向Executor发送任务消息
	virtual HRESULT SendToExcutor(const EXCUTORID excutorId,CString& szInfo);
private:
	//HRESULT SendMessageToExcutor(const EXCUTORID guid, WPARAM wParam, LPARAM lParam);
	/*
    Description：查询可用的Executor
	Input：		
	Return:		Executor标识  INVALID_ID: 无可用的Executor.exe
	History：
	*/
	EXCUTORID GetFirstNotUsedExcutorId();
	HRESULT CheckExcutor();
	/*
    Description：启动Executor.exe
	Input：		excId Executor.exe编号，dwProcessId 进程标识
	Return:		TRUE 成功
	History：
	*/
	BOOL LaunchExcutorFile(const EXCUTORID excId, DWORD& dwProcessId);
public:
	/*
    Description：处理执行者进程发送的消息
	Input：		msgIn 消息内容
	Return:		S_OK 成功处理消息
	History：
	*/
	HRESULT OnExcutorMessage(ST_EMBWNDMSG& msgIn);
	/*
    Description：将消息保存在m_vMsgPool
	Input：		
	Return:		S_OK 成功处理消息
	History：
	*/
	HRESULT SaveMessageToPool(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	/*
    Description：执行者注册
	Input：		
	Return:		S_OK 成功处理消息
	History：
	*/
	HRESULT OnExcutorReg(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	HRESULT CheckExcutorLoop();
	HRESULT WndMsgPoolCheckLoop();
	HANDLE CreateExchangemapping(EXCUTORID excId);

	HRESULT GetExecutors(vector<ST_EXCUTORINFO>& vExecutor);
	HRESULT SetExecutorState( EXCUTORID id, EXE_STATE eState );
	bool QueryExecutor(const EXCUTORID& id, ST_EXCUTORINFO& infor);
	int GetExcResUsage();
public:
	HWND m_hMessageWnd;

private:
	void ClearMsgPool();
private:
	CString m_strExcPath;
	CString m_strExcFolder;
	CString m_strExcutorWorkFolder;
	CString m_strExcName;
	EXCUTORID m_nMinExcutorId;
	EXCUTORID m_nMaxExcutorId;
	ACTORID m_nActorID;

	CAutoCritSec m_csExcutors;
	MAPEXCUTORS m_mapExcutors;
	static CExcutorMgr* m_spExcMgr;

	HANDLE m_hMsgLoopThread;
	HANDLE m_hCheckThread;
	HANDLE m_hEventQuitLoop;

	HANDLE m_hEventPoolMsgArrival;
	HANDLE m_hMsgPoolCheckThread;
	CAutoCritSec m_csMsgPool;
	VECWNDMSG m_vMsgPool;

	IExcutorMsgCallBack* m_pIExcCallBack;

	int nfgExcutorLaunchTimeout;
public:
	static CExcutorMgr* GetExcutorMgr();
	static BOOL Release();
};
