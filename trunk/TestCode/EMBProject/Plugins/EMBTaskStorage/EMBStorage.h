/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBStorage.h
	author:		tianyx
	
	purpose:	����洢�࣬������洢���ڴ���
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include <map>
#include "FGlobal.h"
#include "AutoCritSec.h"
#include "EmbStructDef.h"
using namespace std;

//����������Ϣ�ṹ��
struct ST_TASKSAVEDATA
{
	ST_TASKBASIC basic;            //����������Ϣ
	CTaskString strTask;           //ʵ������XML����

	TXGUID guid;                   //����GUID
	int nRetry;                    //���Դ���
	DISPATCHID nDispatchID;        //��������ʶID
	time_t stSubmit;               //�ύʱ��

	ST_TASKSAVEDATA()
	{
		nRetry = 0;
		nDispatchID = INVALID_ID;
	}
};

//�������ȼ���Ϣ
struct ST_TASKPRIKEY
{
	int nPriority;
	time_t stSubmit;
	TXGUID guid;
	//�Ƚ����ȼ�
	//�����ȼ���ͬʱ���Ƚ����ȼ�
	//���ȼ���ͬʱ���Ƚ��ύʱ�䣬Խ���ύ���ȼ�Խ��
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

//����GUID��������Ϣ��Ӧͼ
typedef map<TXGUID, ST_TASKSAVEDATA> MAPMEMTASKPOOL;
//�������ȼ�������GUID��Ӧͼ
typedef map<ST_TASKPRIKEY, TXGUID> MAPTASKNOTASSIGNED;
// �ѷ��������б�
typedef map<TXGUID, TXGUID> MAPTXGUID;
// �ѷ�����У�һ�������������ж���ѷ�������
typedef map<DISPATCHID, MAPTXGUID> MAPTASKASSIGNED;

typedef multimap<int, EMB::IEMBPublishCallbackInterface*> MMAPPUBTYPEIFACE;


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
	//������
	CAutoCritSec m_csPoolLock;

	// ���������б�
	MAPMEMTASKPOOL m_mapTaskPool;
	//δ���������Ӧ��
	MAPTASKNOTASSIGNED m_mapNotAssigned;
	//�ѷ��������Ӧ��
	MAPTASKASSIGNED m_mapAssigned;

private:
	//������Դ���
	int m_nMaxRetry;
};


class CEMBStorageDB:
	public IPluginStorageInterface,
	public IPluginTaskCommit,
	public IEMBInfoPublishRegisterInterface
{
public:
	CEMBStorageDB(void);
	virtual ~CEMBStorageDB(void);

	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);


	virtual HRESULT UpdateTaskToStorage(const DISPATCHID nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage( const DISPATCHID nDispatchID,int nMinPriority, int nDesiredNum, VECTASKS& vTasks );
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks);
	virtual HRESULT UpdateActorID(CTaskString& strTaskGuid, ACTORID actorId);

	//for IEMBInfoPublishRegisterInterface interface
	virtual HRESULT RegisterPublisher(IEMBPublishCallbackInterface* pPublisher, const int nPubType);
	virtual HRESULT UnRegisterPublisher(IEMBPublishCallbackInterface* pPublisher, const int nPubType);

	// �������ݿ������ַ���
	void SetDBConnectString(CString strDBCon);

	void PublishInfo(ST_TASKPUBLISHINFO& taskPubInfo, int nPubType);

private:
	CString GetTaskKind(ST_TASKSAVEDATA taskInfo);
	CString Timet2CString(time_t tm);

private:
	CString m_strDBCon;

	//������
	CAutoCritSec m_csPoolLock;

	//for publish register
	CAutoCritSec m_csPub;
	MMAPPUBTYPEIFACE m_mmapPubface;

};

}