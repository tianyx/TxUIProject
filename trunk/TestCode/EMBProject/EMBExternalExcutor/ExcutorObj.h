/********************************************************************
	created:	2013/11/23
	filename: 	ExcutorObj.h
	author:		tianyx
	purpose:	CExcutorObj ����
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

// �ֲ�����
struct ST_SUBTASKDATA
{
	int		nType;		// ����
	CString strName;
	CString strSubTask; // ��Ϣ
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
    Description���ύ����
	Input��		taskIn ������Ϣ�� retOut ������Ϣ
	Return:		TRUE �ɹ�; FALSE ʧ��
	History��
	*/
	BOOL ExcuteTask(ST_EMBTRANSMSG& taskIn, ST_EMBTRANSMSG& retOut);
	/*
    Description����Actor������Ϣ
	Input��		strMsg ��Ϣ����
	Return:		TRUE �ɹ�; FALSE ʧ��
	History��
	*/
	BOOL SendToActor(CString& strMsg);

	/*
    Description������Ϣ������ m_vMsgPool
	Input��		hwnd ������Ϣ��HWND; message ��Ϣ��ʶ; wparam ��Ϣ������HWND; lparam ��Ϣ�ַ���
	Return:		S_OK �ɹ�
	History��
	*/
	HRESULT SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	HRESULT WndMsgPoolCheckLoop();
	HRESULT TaskCheckLoop();

	/*
    Description�����������Ϣ
	Input��		szInfo ������Ϣ
	Return:		S_OK �ɹ�
	History��
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
    Description����ʼ��
	Input��		tagExecutorReg ִ���߽�����Ϣ
	Return:		true �ɹ�
	History�� 11-26 �ӿڲ�������,ʹ��ST_EXCUTORREG
	*/
	BOOL Init(ST_EXCUTORREG& tExecutorReg, HWND hMain);

public:
	HWND m_hwndExcMsg;	// ��Ϣ��ʶ

private:
	CString						m_strTask;				// ������Ϣ
	vector<ST_SUBTASKDATA>		m_vSubTasks;			// �ֲ���������
	ITaskWorkerCallInterface	*m_pTaskDllInterface;	// �������߻ص��ӿ�
	ST_LOADEDPLUGIN				m_taskDll;				// ����������
	ST_TASKRUNSTATE				m_runState;				// ��������״̬
	ST_EXCUTORREG				m_executorReg;			// ִ���߽�����Ϣ
	VECPLUGINFOS				m_vexcInfo;				// �����Ϣ����

	HWND	m_hwndMain;
	HANDLE	m_hThreadMsgLoop;		// ��Ϣ�ַ��߳�
	HANDLE	m_hThreadMsgPoolCheck;	// ��Ϣ����߳�
	HANDLE	m_hThreadTaskCheck;		// �������߳�
	HANDLE	m_hEventQuitLoop;		// QuitLoop �¼�
	HANDLE	m_hEventPoolMsgArrival; // MsgArrival �¼�
	HANDLE	m_hMapping;

	CAutoCritSec	m_csPipeWrite;			// PipeWrite �ٽ���
	CAutoCritSec	m_csMsgPool;			// MsgPool �ٽ���
	CAutoCritSec	m_csTaskInfo;			// TaskInfo �ٽ���
	VECWNDMSG		m_vMsgPool;				// ��Ϣ����
	BOOL			m_bRegistered;
	
	int nfgTaskCheckInterval;	// ��������
	int nfgIdleReportInterval;	// 
	int nfgIdleTimeoutForQuit;

	static CExcutorObj* m_spExcObj;

public:
	static CExcutorObj* GetExcutorObj(ST_EXCUTORREG& tExecutorReg, HWND hMain);
	static BOOL Release();
};