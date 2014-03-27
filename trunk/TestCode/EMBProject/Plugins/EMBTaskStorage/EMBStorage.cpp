#include "StdAfx.h"
#include "EMBStorage.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "EMBDocDef.h"
#include "ADOCtrl.h"
#include "Log.h"
#include "LogKeyDef.h"

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
*Description��update Task Status to TaskList for TaskStorage
*Input Param��
*      nDispatchID ����������ID
*      szTaskIn ��������Ϣ
*Return Param�����سɹ���ʧ��
*History��
*/
HRESULT EMB::CEMBStorageMem::UpdateTaskToStorage( const DISPATCHID nDispatchID, CTaskString& szTaskIn )
{
	// ������Ϣ
	ST_TASKUPDATE updateInfo;
	updateInfo.FromString(szTaskIn);
	// �ж�����״̬
	if (updateInfo.nUpdateType == embtaskupdatetype_none)
	{
		ASSERT(FALSE);
		return E_INVALIDARG;
	}

	TXGUID guid = updateInfo.guid;
	//�ж��Ƿ�Ϊ�������״̬
	if (updateInfo.nUpdateType == embtaskupdatetype_finish)
	{
		//�Ƿ��������
		int nEndState = updateInfo.data_end.nEndState;
		if (nEndState == embtaskstate_finished)
		{
			//remove from all map
			CAutoLock lock(&m_csPoolLock);
			//�Ƴ��ѷ������
			MAPTASKASSIGNED::iterator itDispath = m_mapAssigned.find(nDispatchID);
			// �������˵��û���ҵ������ҵ��������
			if (itDispath != m_mapAssigned.end())
			{
				//secondҲ��һ��map,��һ��map�еĵڶ���map�е�һ����Ϣ�Ƴ�
				itDispath->second.erase(guid);
			}

			//���������
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
		//�����ɾ������
		//to be add...
		ASSERT(FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
	
	return S_OK;

}

/*
*Description��������洢�л�ȡ����
*Input Param��
*      nDispatchID �����������������ID
*      nMinPriority ���������������ض�ACTORִ�У���ʵ��
*      nDesiredNum����ȡ������
*      vTasks �����������б�
*Return Param�����سɹ���ʧ��
*History��
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
	//���û���ҵ��������ѷ��������¼���ѷ��������б�
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
			//������������������ӵ������б���
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
*Description���ύ����
*Input Param��
*      szTaskIn ����������XML��ĸ��
*      szRet ��������Ϣ
*Return Param�����سɹ���ʧ��
*History��
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
*Description����ȡ�ѷ��������б�
*Input Param��
*      nDispatchID �������������ID
*      vTasks �����������б�
*Return Param�����سɹ���ʧ��
*History��
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
	// �������ݿ�����
	CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase(m_strDBCon);
	if(!dbCtrl.OpenDB())
	{
		return FALSE;
	}

	CString strSQL = "";
	// ������Ϣ
	ST_TASKUPDATE updateInfo;
	updateInfo.FromString(szTaskIn);
	// �ж�����״̬
	if (updateInfo.nUpdateType == embtaskupdatetype_none)
	{
		// ��������״̬
		strSQL = "update T_EMBTask set nState = 0 where strTaskID='";
		strSQL += Guid2String(updateInfo.guid);
		strSQL += "'";

		if(!dbCtrl.ExecuteSQL(strSQL))
		{
			CFWriteLog(LOGKEY_TASKSTORAGE,"���ݿ�洢ִ��SQL���ʧ��!");
			CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);

			dbCtrl.CloseDB();
			return S_FALSE;
		}
		
	}

	TXGUID guid = updateInfo.guid;
	//�ж��Ƿ�Ϊ�������״̬
	if (updateInfo.nUpdateType == embtaskupdatetype_finish)
	{
		//�Ƿ��������
		int nEndState = updateInfo.data_end.nEndState;
		if (nEndState == embtaskstate_finished)
		{
			// ��������ֲ����־����
			CTime tmNow(time(NULL));
			strSQL.Format(TEXT("insert into T_EMBTaskLog (nSvrCode, strTaskID, nState, Remark, time) values (%d, '%s', %d, '%s', '%s')"), 0, Guid2String(guid), 3, TEXT("�������"), tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")));

			if(!dbCtrl.ExecuteSQL(strSQL))
			{
				CFWriteLog(LOGKEY_TASKSTORAGE,"���ݿ�洢����T_EMBTASKLog SQL���ʧ��!");
				CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
			}
// 			// �������б���ɾ��
// 
// 			strSQL =  "delete from t_EMBTask where strTaskID='";
// 			strSQL += Guid2String(updateInfo.guid);
// 			strSQL += "'";
// 

			strSQL = "update t_EMBTask set nState= ";
			CTxStrConvert conval(embtaskstate_finished);
			strSQL +=conval.GetAsString();
			strSQL +=" where strTaskID='";
			strSQL += Guid2String(guid);
			strSQL += "'";

			if(!dbCtrl.ExecuteSQL(strSQL))
			{
				CFWriteLog(LOGKEY_TASKSTORAGE,"��������������ݿ����Ϣʧ�ܣ� SQL���ʧ��!");
				CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
			}
		}
		else if (nEndState == embtaskstate_error)
		{
			//if retry time not max,reset task;
			//else remove from all map;
			strSQL = "select nRetry from t_EMBTask where strTaskID='";
			strSQL += Guid2String(guid);
			strSQL += "'";

			if(dbCtrl.OpenRecordset(strSQL))
			{
				if(dbCtrl.GetCount() > 0)
				{
					long nRetry = 0;
					dbCtrl.GetSpecialValue("nRetry",&nRetry);
					if(nRetry > 0)
					{
						nRetry--;
						dbCtrl.SetSpecialValue("nRetry",nRetry);
						//������״̬����Ϊ0, svr id ��Ϊnull, actor id ��Ϊnull
						strSQL.Format(TEXT("update t_EMBTask set nState=0, nDispatchID = NULL, nActorID = NULL where strTaskID='%s'"),Guid2String(guid));

					}
					else
					{
						strSQL = "update t_EMBTask set nState= ";
						CTxStrConvert conval(embtaskstate_error);
						strSQL +=conval.GetAsString();
						strSQL +=" where strTaskID='";
						strSQL += Guid2String(guid);
						strSQL += "'";
// 						strSQL = "delete from t_EMBTask where strTaskID='";
// 						strSQL += Guid2String(guid);
// 						strSQL += "'";
					}

					if(!strSQL.IsEmpty())
					{
						if (!dbCtrl.ExecuteSQL(strSQL))
						{
							CFWriteLog(LOGKEY_TASKSTORAGE,"����ʧ������ʧ�ܣ� SQL���ʧ��!");
							CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
						}
						else
						{
							// ������״̬д����־����
							CTime tmNow(time(NULL));
							CString strRemark = TEXT("��������ʧ��");
							if (nRetry >= 0)
							{
								strRemark.Format(TEXT("����ʧ�ܣ���ʼ���ԣ�����ʣ�� %d"), nRetry+1);
							}
							strSQL.Format(TEXT("insert into T_EMBTaskLog (nSvrCode, strTaskID, nState, Remark, time) values (%d, '%s', %d, '%s', '%s')"), 0, Guid2String(guid), embtaskstate_error, TEXT("����ʧ��"), tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")));

							if(!dbCtrl.ExecuteSQL(strSQL))
							{
								CFWriteLog(LOGKEY_TASKSTORAGE,"���ݿ�洢����T_EMBTASKLog SQL���ʧ��!");
								CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
							}
						}
					}
				}
			}

			dbCtrl.CloseRecordset();
		}
	}
	else if (updateInfo.nUpdateType == embtaskupdatetype_del)
	{
		//�����ɾ������
		strSQL = "delete from t_EMBTask where strTaskID='";
		strSQL += Guid2String(guid);
		strSQL += "'";

		if(!dbCtrl.ExecuteSQL(strSQL))
		{
			CFWriteLog(LOGKEY_TASKSTORAGE,"ɾ������ʧ�ܣ� SQL���ʧ��!");
			CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
		}
		else
		{
			CTime tmNow(time(NULL));
			strSQL.Format(TEXT("insert into T_EMBTaskLog (nSvrCode, strTaskID, nState, Remark, time) values (%d, '%s', %d, '%s', '%s')"), 0, Guid2String(guid), 0, TEXT("����ɾ��"), tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")));

			if(!dbCtrl.ExecuteSQL(strSQL))
			{
				CFWriteLog(LOGKEY_TASKSTORAGE,"���ݿ�洢����T_EMBTASKLog SQL���ʧ��!");
				CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
			}
		}

		

	}
	else if (updateInfo.nUpdateType == embtaskupdatetype_change)
	{
		// ���������СΪ20
		char cNum[20];
		memset(cNum, 0, sizeof(cNum));
		// ��������״̬
		itoa(updateInfo.data_pri.nPriority,cNum,10);
		strSQL = "update T_EMBTask set nPRI = ";
		strSQL += cNum;
		strSQL += ",";

		itoa(updateInfo.data_end.dispatchid,cNum,10);
		strSQL += "nDispatchID=";
		strSQL += cNum;
		strSQL += ",";

		itoa(updateInfo.data_end.actorid,cNum,10);
		strSQL += "nActorID=";
		strSQL += cNum;

		strSQL += " where strTaskID='";
		strSQL += updateInfo.strTaskID;
		strSQL += "'";

		if(!dbCtrl.ExecuteSQL(strSQL))
		{
			CFWriteLog(LOGKEY_TASKSTORAGE,"���ݿ�洢ִ��SQL���ʧ��!");
			CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);

			dbCtrl.CloseDB();
			return S_FALSE;
		}
		else
		{
// 			CTime tmNow(time(NULL));
// 			strSQL.Format(TEXT("insert into T_EMBTaskLog (nSvrCode, strTaskID, nState, Remark, time) values (%d, '%s', %d, '%s', '%s')"), 0, Guid2String(guid), 0, TEXT("���񱻸���"), tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")));
// 
// 			if(!dbCtrl.ExecuteSQL(strSQL))
// 			{
// 				CFWriteLog(LOGKEY_TASKSTORAGE,"���ݿ�洢����T_EMBTASKLog SQL���ʧ��!");
// 				CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
// 			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	dbCtrl.CloseDB();

	return S_OK;
}
HRESULT EMB::CEMBStorageDB::FetchTaskFromStorage( const DISPATCHID nDispatchID, int nMinPriority, int nDesiredNum, VECTASKS& vTasks )
{
	long nSize = 0;
	CString strTaskID = "";
	CString strXml = "";
	CString strSQL = "";
	char cNum[5];

	CAutoLock lock(&m_csPoolLock);
	
	CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase(m_strDBCon);
	if(!dbCtrl.OpenDB())
	{
		CFWriteLog(0, TEXT("Manager DB Connect Error ,ConnectString : %s"),m_strDBCon);
		return FALSE;
	}

	

	strSQL = "select * from t_EMBTask where (nState = 0) and (nDispatchID is null) and (nPRI > ";
	itoa(nMinPriority,cNum,10);
	strSQL += cNum;
	
	strSQL += ") order by nPRI,tSubmitTime desc";
	if(dbCtrl.OpenRecordset(strSQL))
	{
		nSize = dbCtrl.GetCount();

		if(nSize > 0)
		{
			if(nSize < nDesiredNum)
				nDesiredNum = nSize;

			dbCtrl.MoveFirst();
			for(int i = 0 ; i < nDesiredNum ; i++)
			{
				// �������
				dbCtrl.GetSpecialValue("strTaskID",&strTaskID);
				dbCtrl.GetSpecialValue("strExtendInfoXml",&strXml);
				vTasks.push_back(strXml);

				// ��״̬Ϊ��������
				dbCtrl.SetSpecialValue("nState",1);
				dbCtrl.SetSpecialValue("nDispatchID",nDispatchID);

				dbCtrl.MoveNext();
			}
		}
	}

	dbCtrl.CloseRecordset();
	dbCtrl.CloseDB();
	return S_OK;
}

HRESULT EMB::CEMBStorageDB::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	CString strSQL = "";
	char cNum[5];
	CAutoLock lock(&m_csPoolLock);

	CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase(m_strDBCon);
	if(!dbCtrl.OpenDB())
	{
		return FALSE;
	}

	ST_TASKSAVEDATA sdata;
	GetTaskBasicInfo(szTaskIn, sdata.basic);
	if (!sdata.basic.strGuid.IsEmpty())
	{
		strSQL = "insert into t_EMBTask(strTaskID,strTaskKind,nPRI,tSubmitTime,nState,nRetry,strExtendInfoXml) values('";

		strSQL += sdata.basic.strGuid;
		strSQL += "','";

		strSQL += GetTaskKind(sdata);
		strSQL += "',";

		itoa(sdata.basic.nPriority,cNum,10);
		strSQL += ",'";

		strSQL += Timet2CString(sdata.basic.tmSubmit);
		strSQL += "',";

		strSQL += "0,3,'";

		strSQL += szTaskIn;
		strSQL += "')";

		if(!dbCtrl.ExecuteSQL(strSQL))
		{
			CFWriteLog(LOGKEY_TASKSTORAGE,"Submit Task Error�� SQL:");
			CFWriteLog(LOGKEY_TASKSTORAGE,strSQL);
		}
	}
	else
	{
		dbCtrl.CloseDB();
		return S_FALSE;
	}

	dbCtrl.CloseDB();
	return S_OK;
}

HRESULT EMB::CEMBStorageDB::GetDispatchedTaskFromStorage( const DISPATCHID nDispatchID, VECTASKS& vTasks )
{
	CString strSQL = "";
	CString strXml = "";
	char cNum[5];
	CAutoLock lock(&m_csPoolLock);

	CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase(m_strDBCon);
	if(!dbCtrl.OpenDB())
	{
		return FALSE;
	}

	itoa(nDispatchID,cNum,10);
	/*strSQL = "select * from t_EMBTask where nDispatchID = ";
	strSQL += cNum;*/
	// ȡ nState !=3 ������(not finished task) 
	strSQL.Format("select * from t_EMBTask where nDispatchID = %s and nState <> %d and nState <> %d", cNum, embtaskstate_finished, embtaskstate_error);

	if(dbCtrl.OpenRecordset(strSQL))
	{
		if(dbCtrl.GetCount() > 0)
		{
			dbCtrl.MoveFirst();
			do 
			{
				dbCtrl.GetSpecialValue("strExtendInfoXml",&strXml);
				vTasks.push_back(strXml);

				dbCtrl.MoveNext();
			} while (!dbCtrl.IsEnd());
		}
	}

	dbCtrl.CloseRecordset();
	dbCtrl.CloseDB();

	return S_OK;
}

void EMB::CEMBStorageDB::SetDBConnectString(CString strDBCon)
{
	m_strDBCon = strDBCon;
}

CString EMB::CEMBStorageDB::GetTaskKind(ST_TASKSAVEDATA taskInfo)
{
	return "15";
}

CString EMB::CEMBStorageDB::Timet2CString(time_t tm)
{
	struct tm *local;
	CString stime;
	int nYear, nMonth, nDate, nHour, nMin, nSec;

	local = localtime(&tm);
	nYear = local->tm_year + 1900;
	nMonth = local->tm_mon + 1;
	nDate = local->tm_mday;
	nHour = local->tm_hour;
	nMin = local->tm_min;
	nSec = local->tm_sec;

	CTime ptime(nYear, nMonth, nDate, nHour, nMin, nSec);
	stime = ptime.Format("%Y-%m-%d %H:%M:%S");

	return stime;
}

HRESULT EMB::CEMBStorageDB::UpdateActorID( CTaskString& strTaskGuid, ACTORID actorId )
{
	CString sql;
	sql.Format("update T_EMBTask set nActorID='%d' where strTaskID = '%s'", actorId, strTaskGuid);

	CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase(m_strDBCon);
	if(!dbCtrl.OpenDB())
	{
		return EMBERR_DBOPEN;
	}

	if (!dbCtrl.ExecuteSQL(sql))
	{
		CFWriteLog(LOGKEY_TASKSTORAGE, "UpdateActorID() Fail ,SQL:");
		CFWriteLog(LOGKEY_TASKSTORAGE, sql);
	}

	dbCtrl.CloseDB();
	return S_OK;
}
