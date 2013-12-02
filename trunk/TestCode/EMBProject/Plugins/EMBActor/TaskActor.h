/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	TaskActor.h
	author:		tianyx
	
	purpose:	����client���͵�xml��Ϣ: �����ύ��״̬��ѯ
*********************************************************************/

#pragma once
#include "IEMBBaseInterface.h"
#include "ActorConnector.h"
#include "ExcutorMgr.h"
#include "EmbStructDef.h"
#include <map>
#include <deque>
using namespace std;

struct ST_TASKINACTOR
{
	TXGUID taskGuid;
	int nCurrStep;
	int nState;
	EXCUTORID excId;
	int nRetry;
	int nPercent;
	int nSubErrorCode; // �������
	time_t tmLastReport;
	CString strTask;
	ST_TASKINACTOR()
	{
		taskGuid = GUID_NULL;
		nCurrStep = INVALID_VALUE;
		nState = embtaskstate_none;
		tmLastReport = 0;
		nRetry = 0;
		excId = INVALID_ID;
		nPercent = 0;
		nSubErrorCode = 0;
	}
};

typedef map<TXGUID, ST_TASKINACTOR> MAPTASKINACTOR;
typedef map<EXCUTORID, TXGUID> MAPEXCTASKS;
typedef deque<TXGUID> DQTASKS;
namespace EMB
{
class CTaskActor:
	public IPluginBaseInterface,
	public IPluginControlInterface,
	public IPluginConfigInterface,
// 	public IPluginConnectorInterce,
// 	public IActorMsgCallBackInterface,
	public IActorConnectorCallback,
	public IExcutorMsgCallBack,
	public IActorUI

{
public:
	CTaskActor(void);
	virtual ~CTaskActor(void);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();

	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//for IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);

	//for IPluginControlInterface
	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();
	//for IPluginConnectorInterce
// 	//you riseup a action to connect/disconnect to other plugin
// 	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
// 	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);
// 
// 	//other plugin inform you after riseup a action that Connect()/Disconnect() you
// 	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
// 	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);

	//interface IActorMsgCallBackInterface
// 	virtual HRESULT ActCallbackProc(CTaskString& szActMsg, CTaskString& szRet);

	//for IActorConnectorCallback
	/*
    Description������xml��Ϣ
	Input��		strInfo xml��ʽ��Ϣ, strRet ������Ϣ
	Return:		S_OK �ɹ�������Ϣ
	History��
	*/
	virtual HRESULT OnActorConnectorMsg(CString& strInfo, CString& strRet);
	//for IExcutorMsgCallBack
	/*
    Description��ִ������Ϣ����
	Input��		excutorId ִ�н��̱�ʶ szInfoIn xml��Ϣ
	Return:		S_OK �ɹ�������Ϣ
	History��
	*/
	HRESULT OnExcutorMessage(const EXCUTORID excutorId, CString& szInfoIn);
	HRESULT OnExcutorExit(const EXCUTORID excutorId);

	// IActorUI �ӿ�
	/*
    Description����ȡ�����е�Executor.exe ��Ϣ
	Input��		mapExecutor �������е�Executor��Ϣ
	Return:		S_OK �ɹ�
	History��
	*/
	virtual HRESULT GetExecutors(vector<CString>& vExecutor);
	// ------------------------------------------------------

private:
	BOOL SwitchActorConn(BOOL bMainConn);
	BOOL ReportTaskState(ST_TASKINACTOR& infoIn);
	BOOL OnExcutorIdle(const EXCUTORID excutorId);
	CActorConnector* GetActiveActorConnector(){return (m_nActiveConn == 1)?(&m_actorconnMain):(m_nActiveConn == 2)? (&m_actorconnSlave):NULL;}
	/*
    Description����������󣬽��������Ϊxml�ļ�
	Input��		tskReport ������Ϣ 
	Return:		true �ɹ�
	History��
	*/
	bool TaskResultSaveXmlFile(ST_TASKREPORT& tskReport);
	bool QueryXmlFile(const CString& strTaskGuid, ST_TASKREPORT& tskInfor);
	bool FindTask(const EXCUTORID& strExecutorId, ST_TASKINACTOR& tsk);

private:
	CActorConnector m_actorconnMain;
	CActorConnector m_actorconnSlave;
	int m_nActiveConn;
	CExcutorMgr* m_pExcutorMgr;
	ST_ACTORCONFIG m_ActRegInfo;

	CAutoCritSec m_csmapLock;
	MAPTASKINACTOR m_mapTaskinActor;
	MAPEXCTASKS m_mapExcTask;
	DQTASKS m_dqRecentFinishedTasks;
	BOOL m_bRuning;
	int nfgRetryMax;
	CString m_strTaskXmlPath; // ����xml�ļ���·��
};


}
extern EMB::CTaskActor* g_pPluginInstane;
