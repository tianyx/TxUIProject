#include "StdAfx.h"
#include "DBWriterMgr.h"
#include "EMBDefine.h"
#include "TxADOConn.h"
#include "TxLogManager.h"

using namespace EMB;
//////////////////////////////////////////////////////////////////////////
DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CDBWriterMgr* pObj = (CDBWriterMgr*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////

CDBWriterMgr::CDBWriterMgr(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
}

CDBWriterMgr::~CDBWriterMgr(void)
{
}

HRESULT EMB::CDBWriterMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;
	if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_ITaskWorkerCall)
	{
		pInterfaceOut = dynamic_cast<ITaskWorkerCallInterface*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

HRESULT EMB::CDBWriterMgr::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerDBWriter;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_DBWriter;
	vInfoOut.push_back(info);
	return S_OK;
}

HRESULT EMB::CDBWriterMgr::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_DBWRITER, TEXT("dbwriter"));

	return S_OK;
}

void EMB::CDBWriterMgr::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCDBWriterMgr::OnFinalRelease() ");

	delete this;
}

HRESULT EMB::CDBWriterMgr::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return EMBERR_WORKERBUSY;
	}

	ST_DBWRITERTASKINFO info;
	info.FromString(szTaskIn);
	//current only support sql
	if (info.nDBType != embDBTypeSql
		||info.vSqlKeys.size() == 0 || info.strConn.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}

	for (size_t i = 0; i < info.vSqls.size(); ++i)
	{
		if (info.vSqls[i].IsEmpty())
		{
			ASSERT(FALSE);
			return EMBERR_INVALIDARG;
		}
	}

	m_taskInfo = info;
	m_strTask = szTaskIn;
	m_pReportCallback = pICallback;
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
	return S_OK;

}

HRESULT EMB::CDBWriterMgr::CancelTask()
{
	return S_OK;
}

HRESULT EMB::CDBWriterMgr::GetTaskProgress( CEMBWorkString& szInfo )
{
	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

BOOL EMB::CDBWriterMgr::RunTaskLoop()
{
	//create dbconnector
	CFWriteLog(NULL, TEXT("dbwriter work started!!....."));
	ST_WORKERREPORT report;
	report.code = S_OK;
	if(CTxADODBMgr::GetADODBMgr()->OpenDB(1, m_taskInfo.strConn))
	{
		CTxADOCommand sqlCmd;
		int nDeta = 100 % m_taskInfo.vSqlKeys.size();
		sqlCmd.BeginTrans();
		for (size_t i = 0; i < m_taskInfo.vSqlKeys.size(); ++i)
		{
			if (sqlCmd.ExecuteSQL(m_taskInfo.vSqls[i]))
			{
				m_nPercent += nDeta;
				CFWriteLog(0, TEXT("sql exc success, %s"), m_taskInfo.vSqls[i]);

			}
			else
			{
				CFWriteLog(0, TEXT("sql exc err, rollback: %s"), m_taskInfo.vSqls[i]);
				sqlCmd.RollBackTrans();
				report.code = EMBERR_SQLEXEC;
				break;
			}
		}
		if (report.code == S_OK)
		{
			sqlCmd.CommitTrans();
			CFWriteLog(0, TEXT("sql exc finished!"));
		}
	}
	else
	{
		report.code = EMBERR_DBOPEN;
	}
	
	if (report.code == S_OK )
	{
		m_nPercent = 100;
	}
	else
	{
		m_nPercent = -1;
	}

	CFWriteLog(NULL, TEXT("work end!!....."));

	if (m_pReportCallback)
	{
		CString strRet;
		report.ToString(strRet);
		m_pReportCallback->OnDllReportTaskProgress(strRet);
	}

	m_hThreadTask = NULL;
	return TRUE;

}
