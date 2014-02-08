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

#define LOGKEYEPG 1
#define DBKEYEGP 1



struct STWEEKDAYINFO
{
	int bEnable;
	int nDay;
	CString strSecID;
	STWEEKDAYINFO()
	{
		bEnable = 0;
	}
};

typedef vector<STWEEKDAYINFO> VECWEEKDAY;
struct ST_ONECHN
{
	CString strChId;
	VECWEEKDAY vWeekDays;
};
typedef vector<ST_ONECHN> VECCHNTASK;
struct STEPGTASK
{

	VECCHNTASK vChnTask;
	CString strDesc;
	//auto gen
	BOOL bEnableTimer;
	int nOffsetDay;
	INT64 tTimeToGen;
	BOOL bMustSubmitted;
	int nWeekLoopMode;
};

struct STEPGTASKS
{
	vector<STEPGTASK> vTasks;
	
	BOOL ToString(CString& strDataOut);
	BOOL FromString(LPCTSTR strDataIn);
};


struct STSECTION
{
	CString strSecId;
	CString strSecName;
};
typedef vector<STSECTION> VECSECTIONS;

struct STWEEKDAYSECINFO
{
	VECSECTIONS vSections;
};
struct STCHNSECTIONINFO
{
	CString strChnId;
	CString strChnName;
	STWEEKDAYSECINFO vWeekSections[7];
};


//全局信息存储结构体
struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szLogFile;
	CString szDBConnString;
	CString szEpgDatafile;
	int		nAutoCheckInterval;

	vector<STCHNSECTIONINFO> vChnInfo;
	ST_GLOBAL()
	{

		nAutoCheckInterval = 10*60;
	}
};

struct STBIDINFO
{
	CString strPgmName;
	CTime tBeginPlay;
	CTime tEndPlay;
};

struct STCHNPGMINFO
{
	CTime tDate;
	CString strSecId;
	CString strPISId;
	int  nState;
	vector<STBIDINFO> vBidList;
};

typedef vector<STCHNPGMINFO> VCHNPGMS;
typedef map<CString, VCHNPGMS> MAPCHNPGMS;



//全局存储信息
extern ST_GLOBAL g_GlobalInfo;
//插件管理插件动态库加载指针
BOOL InitGlobalConfig();
BOOL LoadSectionInfo();

BOOL GetChnInfo(LPCTSTR szChId,  STCHNSECTIONINFO& out);
CString GetChnName(LPCTSTR szChId);
CString GetSectionName(LPCTSTR szSecId,  VECSECTIONS& vSecIn);

CTimeSpan TimeCodeToSpan(INT64 nTimeCodeIn);
CString GetChnNum(const CString& strChnIDIn );