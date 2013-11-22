/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:06
	filename: 	TxLogManager.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once

#include <map>
#include "Log.h"
#include "AutoCritSec.h"
#include "Log.h"
#include <vector>
#include "LogKeyDef.h"


using namespace std;


#define LOGKEYBASE 0
#define LOGKEYMAIN LOGKEYBASE+1
#define LOGKEYMAIN2 LOGKEYBASE+2
//.......


struct ST_LOGFILEDATA
{
	DWORD dwLogKey;
	CString strName;
	Log* log;
	ST_LOGFILEDATA()
	{
		dwLogKey = LOGKEYBASE;
		log = NULL;
	}

};
struct ST_LOGDATA
{
	DWORD dwLogKey;
	CString strData;
	
};

typedef map<DWORD, ST_LOGFILEDATA> MAPFILELOGS;
typedef vector<ST_LOGDATA> VECLOGDATA;

DWORD __stdcall LogLoopProc(LPVOID lparam);

class CTxLogManager
{
	friend DWORD __stdcall LogLoopProc(LPVOID lparam);

//free for create, no prob
public:
	CTxLogManager(void);
	virtual ~CTxLogManager(void);


	DWORD AddNewLogFile(DWORD dwLogKey, CString strFile, BOOL bAddDateToName = TRUE);
	DWORD WriteLog(DWORD dwLogKey, CString strDataIn); 

	void Start();
	void Stop();
private:


	CAutoCritSec m_qlock;
	CAutoCritSec m_fLock;

	MAPFILELOGS m_mapFileLogs;
	VECLOGDATA m_vLogData;

	HANDLE m_hLoopThread;
	HANDLE m_hEventDataInQueue;
	BOOL m_bRunning;
	
	void CheckLog();

};

CTxLogManager* GetTxLogMgr();
void ReleaseTxLogMgr();

