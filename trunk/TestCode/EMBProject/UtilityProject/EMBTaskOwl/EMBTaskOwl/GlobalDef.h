#pragma once
#include <string>
#include <vector>
#include "MBCCommonDef.h"
#include "EmbStructDef.h"
#include "AutoCritSec.h"

#define LUASTGFUNCNAMEFORMAT		TEXT("function StgCallFunc")

#define  STGLUACALLFUNC TEXT("StgCallFunc")

#define stgtype_unknow	-1
#define stgtype_offline		0
#define stgtype_online		1
#define stgtype_idle		2  //when emb is idle, get incoming task using idle time range and apply idle strategy
#define stgtype_emergency	3

#define DBKEY_EMB		11
#define DBKEY_PLAYLIST	12
#define DBKEY_PLAYLIST2	13

#define EMBERR_LUARUNFAIL		0X800B0011
#define EMBERR_SRCFTPNOTFOUND		0X800B0012

#define EMBERR_DUPLICATE		0X010


#define ftpGrouptype_unknow		0
#define ftpGrouptype_mezi		1
#define ftpGrouptype_zhugan		2

enum ENUMCOL_FTP
{
	colftp_no,
	colftp_group,
	colftp_name,
	colftp_storename,
	colftp_ip,
	colftp_port,
	colftp_mode,
	colftp_user,
	colftp_pw


};

struct ST_MBCOBJCFG
{
	SOCKADDR_IN addr;
	SOCKADDR_IN localBindAddr;
	std::string szChId;
	std::string szName;
	
};

typedef map<int, CString> MAPKEYTEMPLATE; //key = stg return val, val = template file name

struct ST_STGPARAM
{
	//runtime info
	BOOL bValid;

	//saved info
	int nStgPriority;
	int nStgType; //0= offline strategy, 1= online strategy
	BOOL bEnable;
	CString strName;
	CString strLua;
	CString strStgFile;
	
	MAPKEYTEMPLATE mapKt;

	BOOL ToString(CString& strOut);
	BOOL FromString(CString& strIn);
	
};


struct  ST_STGCONFIG 
{
	vector<ST_STGPARAM> vStgs;
};

struct ST_EMBTEMPLATEINFO
{
	CString strKey;
	CString strFile;
	CString strContent;
	CString strTaskSequence;
};


struct ST_APPCONFIGINFO
{
	vector<ST_FTPSITEINFO> vDefSrcFtpSiteInfo; //if task no src ftp info, use this instead

};

typedef vector<ST_FTPSITEINFO> VECFTPSITES;

typedef map<int, VECFTPSITES> MAPGPFTPSITES;
typedef map<int, CString> MAPGROUPIDNAME;


typedef map<CString, ST_EMBTEMPLATEINFO> MAPTPLCONTENT;


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

typedef map<CString, CString> MAPEXTERNALCHID; //key = external chid, val = in chid
typedef map<CString, int> MAPEXTERNALCLIPTYPE; //key = external clipid, val = in clipid
typedef map<CString, int> MAPEXTERNALCLIPSOURCEID; //key = external sourceid, val = in sourceid

struct ST_LISTHEADERITEM
{
	int nId;
	int nWidth;
	CString strName;
};

typedef vector<ST_LISTHEADERITEM> VECLSTHEADERS;

struct ST_LISTCONFINFO
{
	VECLSTHEADERS vListHeader;
	UINT clrText;
	UINT clrTextBk;
	UINT clrWarnText;
	UINT clrWarnTextBk;
	UINT clrErrText;
	UINT clrErrTextBk;
	ST_LISTCONFINFO()
	{
		clrText = RGB(0,0,0);
		clrTextBk = RGB(255,255,255);
		clrWarnText =RGB(0,200,200);
		clrWarnTextBk = RGB(255,255,255);
		clrErrText =RGB(200,0,0);
		clrErrTextBk = RGB(255,255,255);

	}
};

struct ST_GLOBAL
{
	std::string szAppPath;
	std::string szIniPath;
	std::string szCfgSrcPath;
	
	//////////////////////////////////////////////////////////////////////////
	//config to serialize
	int nAutoLoopInterval;
	int nTimelineClose;		//hour after now
	int nTimeLineOffline;   //hour after now

	int nMaxTaskCoutOnceCheck; //

	CString strEmbTaskDBConn;
	CString strPlayListCheckDBConn;
	CString strPlayListCheckDBConn2;

	CString strPlaylistDBName;

	MAPGPFTPSITES mapFtpSites;  //local pre defined ftp
	MAPGROUPIDNAME mapGroupIdName;  //ftp group <id, name>

	MAPEXTERNALCHID mapExternalCHId; //chid <extChID, localChID>
	MAPEXTERNALCLIPTYPE mapExternalClipType;
	MAPEXTERNALCLIPSOURCEID mapExternalClipSrcId;
	//time enter idle mode //if not use, set nIdleHourNum = -1
	int nIdleHourStart;
	int nIdleHourNum;  
	int nIdleTimelineClose;		//hour after now
	int nIdleTimeLineOffline;   //hour after now

	//clip gen
	char chClipMachineID;

	//other config
	UINT nDesFtpMaxsize;
	int nClipKeepDays;
	int nAfdInfo;
	int nDefBitrate;
	int nHDBitRate;
	CString strDefMediaType;
	CString strDefChnID;
	int nDefClipType;
	int nDefClipSourceId;

	//for org task check
	int nEmergencyPriority;		//if >, skip stg check and apply emergency template
	int nEmbtaskRetryCount;

	BOOL bShowManualAlert; //show alert if autotask is running when doing manual task
	BOOL bEnableTaskTrace; //show task trace dlg;
	//////////////////////////////////////////////////////////////////////////
	//following not do serialize
	//runtime check state
	BOOL bIsTemplateOk;
	BOOL bIsStrategyOk;
	BOOL bIsAllOk;
	ST_STGCONFIG stgConfig;
	MAPTPLCONTENT mapTaskTemplates;


	//chn info
	vector<STCHNSECTIONINFO> vChnInfo;

	//listheader info
	//uiconfig
	ST_LISTCONFINFO listInfoAutoTask;
	ST_LISTCONFINFO listInfoManualTask;
	ST_LISTCONFINFO listInfoPlaylistTask;

	BOOL ToString(CString& strOut);
	BOOL FromString(CString& strXml);
	ST_GLOBAL()
	{
		nTimelineClose = 1;
		nTimeLineOffline = 240;
		nIdleTimelineClose = 1;		
		nIdleTimeLineOffline = 48;  
		nMaxTaskCoutOnceCheck = 1000;

		nIdleHourStart = 1;
		nIdleHourNum = 0;

		chClipMachineID = 'X';
		nDesFtpMaxsize = 3;
		nClipKeepDays = 5;
		nAfdInfo = 0;
		nDefBitrate = 8;
		nHDBitRate = 8;
		strDefMediaType = TEXT("OMN");
		strPlaylistDBName = TEXT("zjtv");

		nAutoLoopInterval = 10;

		nEmergencyPriority = 1;
		nEmbtaskRetryCount = 2;

		bIsTemplateOk = FALSE;
		bIsStrategyOk = FALSE;
		bIsAllOk = TRUE;
		bEnableTaskTrace = 0;
#ifdef _DEBUG
		bEnableTaskTrace = 1;
#endif
	}
};




BOOL InitGlobalConfig();

BOOL LoadEmbTemplate();
BOOL AnalyseEmbTemplateFile(CString& strFileIn, ST_EMBTEMPLATEINFO& tOut);

BOOL LoadStgConfig();
BOOL AnalyseStgFile(CString& strFileIn, ST_STGPARAM& stgOut);
BOOL SortStgByPriority();
BOOL TestLuaValid(CString& strLuaIn);

//check if embtemplate that strategy's return val mapping to  is existed
BOOL CheckStgValMappingValid(ST_STGPARAM& stgIn);

//gen clipid from db
CString GenClipIDFromDB();
CString GenClipId2();
//////////////////////////////////////////////////////////////////////////
CTimeSpan TimeCodeToSpan( INT64 nTimeCodeIn );

CString  TimeCodeToString( INT64 nTimeCodeIn );

CString FtpList2String(vector<ST_FTPSITEINFO>& vFtps, BOOL bSrc);
CString TransDBInfo2String(CString strClipLogicId, CString strClipId, ST_DB_BVSIDINFO& bvsInfoIn, ST_DB_LOCATIONINFO& locInfoIn);

BOOL LoadSectionInfo();
CString GetChnName(LPCTSTR szChId);

CString GetSectionName(LPCTSTR szSecId,  VECSECTIONS& vSecIn);

CString GetFtpListColText(const int nColTypeIn, int nGroupId, ST_FTPSITEINFO& ftpIn);

CString FtpGroupID2Str(int nGroupId);
CString ExternalChId2LocalChid(CString& strExternalChid);
int ExternalClipType2Local(CString& strClipId);
int ExternalClipSourceId2Local(CString& strClipSrcId);

BOOL IsIdleHour();

int GetListColPos(VECLSTHEADERS& vHeaders, int nColType);
//////////////////////////////////////////////////////////////////////////
extern ST_GLOBAL g_GlobalInfo;
extern HWND g_hFrame;

class CMainFrame;
extern CMainFrame* g_pMainFrame;
extern CAutoCritSec g_csStg;

class CAutoTaskFunc;
extern CAutoTaskFunc* g_pAutoTaskFunc;

