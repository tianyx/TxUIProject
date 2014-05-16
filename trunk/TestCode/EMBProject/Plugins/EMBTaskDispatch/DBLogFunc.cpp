#include "StdAfx.h"
#include "DBLogFunc.h"
#include "TxDBLogManager.h"

void CFWriteDBLog_Task( DWORD dwLogKey, int nSrvCode,  LPCTSTR szTaskID, int nState, LPCTSTR szRemark )
{
	CString strSql;
	CTime tmNow(time(NULL));
	strSql.Format(TEXT("insert into T_EMBTaskLog (nSvrCode, strTaskID, nState, Remark, time) values (%d, '%s', %d, '%s', '%s')"), nSrvCode, szTaskID, nState, szRemark, tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")));
	CTxDBLogManager::GetTxDBLogMgr()->WriteLog(dwLogKey, strSql);
}

void CFWriteDBLog_EMB( DWORD dwLogKey, int nSrvCode,  int nType, LPCTSTR szRemark )
{
	CString strSql;
	CTime tmNow(time(NULL));
	strSql.Format(TEXT("insert into T_EMBLog (nSvrCode, nType, strRemark, time) values (%d,%d, '%s', '%s')"), nSrvCode, nType, szRemark, tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")));
	CTxDBLogManager::GetTxDBLogMgr()->WriteLog(dwLogKey, strSql);

}

void CFUpdateTaskRunTime( DWORD dwLogKey, LPCTSTR szTaskID, int nStarttime , time_t& tmIn)
{
	CTime tmNow(tmIn);
	CString strSql;
	CString strKey = nStarttime == 1? "tTimeStartRun":"tTimeEndRun";
	strSql.Format(TEXT("update T_EMBtask set %s = '%s' where strTaskID ='%s'"), strKey,  tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S")), szTaskID);
	CTxDBLogManager::GetTxDBLogMgr()->WriteLog(dwLogKey, strSql);

}

