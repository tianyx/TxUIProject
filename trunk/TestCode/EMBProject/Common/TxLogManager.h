/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:06
	filename: 	TxLogManager.h
	author:		tianyx
	
	purpose:	log utility tool 
	usage:		on app start: GetTxLogMgr()->AddNewLogFile();
				use: CFWriteLog() macro to write log
				on app exit:  ReleaseTxLogMgr();


*********************************************************************/
#pragma once

#include <map>
#include "Log.h"
#include "AutoCritSec.h"
#include "Log.h"
#include <vector>
#include "LogKeyDef.h"


using namespace std;

//pre defined log key
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

	//*Description: add a log file to the logmanager
	//*Input Param: dwLogKey: a id to identify the log file
	//				strFile: can be a full file path or a short file name, it will auto recognized by ":" in the string.
	//if not full path, the file will be created in app\log\ folder.
	//				bAddDateToName: add date string at end of file name.
	//*Return Param: return assigned logkey, if log key already defined, return old log key 
	//*History: 
	DWORD AddNewLogFile(DWORD dwLogKey, CString strFile, BOOL bAddDateToName = TRUE);

	//write log, recommend to use CFWriteLog macro
	virtual DWORD WriteLog(DWORD dwLogKey, CString strDataIn); 

	void Start();
	void Stop(BOOL bForce = FALSE);

protected:

	CAutoCritSec m_qlock;
	CAutoCritSec m_fLock;

	VECLOGDATA m_vLogData;

	HANDLE m_hLoopThread;
	HANDLE m_hEventDataInQueue;
	HANDLE m_hQuitEvent;
	
private:
	MAPFILELOGS m_mapFileLogs;

	virtual void CheckLog();

};

//get a singletone logmanager instance;
CTxLogManager* GetTxLogMgr();
//release the logmanager
void ReleaseTxLogMgr();

