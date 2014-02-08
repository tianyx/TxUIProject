/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	MainDef.h
	author:		zhouliang
	
	purpose:	初始化管理类
*********************************************************************/

#pragma once
#include "TxLogManager.h"
#include <vector>
#include <map>
#include "TxSerialize.h"
using namespace std;

#define MSG_TASKADD WM_USER+ 111
#define MSG_TASKUPDATEREALINFO WM_USER+112
#define MSG_TASKDEL WM_USER+ 113
#define MSG_INFOMSG WM_USER+114
#define MSG_ERRSELCHANGED WM_USER+115
#define MSG_TASKSHOW WM_USER+116
#define LOGKEYGHCI 1
#define DBKEYGHCI 1

#define	DIR_ERRORTASK TEXT("\\ErrorTasks")
#define	DIR_FINISHEDTASK TEXT("\\FinishedTasks")


#define TaskRunState_none		0
#define TaskRunstate_dispatched 1
#define TaskRunState_finished	2
#define TaskRunState_error		3



//全局信息存储结构体
struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szLogFile;
	CString szDBConnString;
	int		nAutoCheckInterval;

	CString szFtpIp;
	CString szFtpUser;
	CString szFtpPw;
	CString szFtpDir;

	CString szScanDir;
	CString szWorkDir;
	CString szWorkErrDir;
	CString szWorkFinishedDir;

	CString strLocation;
	CString strChannleID;
	CString strStorageName;
	CString strMediaType;
	int nHoldDay;
	CString strDestFilePostfix;
	
	int nMaxTaskGetPerScan;
	int nMaxFtpSpeedPerExc; //KB/s
	int nMaxSession;
	int nMaxRetry;
	int nScanTimePos; //scan at a second value per minute, to avoid multi importer access scan folder at same time;
	map<CString, long> m_clipTypeMap;

	ST_GLOBAL()
	{
		nMaxFtpSpeedPerExc = 40000;
		nMaxRetry = 1;
		nAutoCheckInterval = 10*60;
		nMaxSession = 3;
		nScanTimePos = 1;
		strDestFilePostfix = TEXT(".mpg");
		nMaxTaskGetPerScan = 10;
	}
};


struct ST_TASKINFO
{
	INT64 nTaskId;
	CString strTaskXMlFile;
	int nRunState;
	CString strLastError;
	int nRetry;
	int nExcID;
	ST_TASKINFO()
	{
		nTaskId = 0;
		nRunState = TaskRunState_none;
		nRetry = 0;
		nExcID = -1;
	}
};

struct ST_REALTRASINFO
{
	INT64 nTaskId;
	INT64 nExcId;
	INT64 nSrcFileSize;
	int nPercent;
	int nSpeedKBPerS;
	CString strMsg;
	int nRetry;
	
};
typedef map<INT64, ST_TASKINFO> MAPTASKS;

//全局存储信息
extern ST_GLOBAL g_GlobalInfo;
//插件管理插件动态库加载指针
BOOL InitGlobalConfig();

BOOL IsSameDirStarted();

CString TxGetFileName(const CString& strFileIn);
CString TxGetFileTitle(const CString& strFileIn);
BOOL RefreshClipType();

CString TxGetFtpFileName(const CString& strFileIn);

typedef multimap<INT64, CString> MMAPTIMETASKS;
BOOL GetTaskInFolder(CString strFolderIn, MMAPTIMETASKS& mapOut);