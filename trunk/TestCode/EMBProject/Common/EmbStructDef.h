#pragma once
#include <vector>
#include "FGlobal.h"
#include "EMBDefine.h"
#include "IEMBBaseInterface.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
struct ST_EMBWNDMSG
{
	HWND hwnd;
	UINT message;
	WPARAM wparam;
	LPARAM lparam;
	char* pStr;
	int nBuffLen;
	ST_EMBWNDMSG()
	{
		nBuffLen = 0;
		pStr = NULL;
		hwnd= NULL;
		message = NULL;
	}
};
typedef vector<ST_EMBWNDMSG> VECWNDMSG;
//////////////////////////////////////////////////////////////////////////
//call GetEmbXmlMainInfo
struct ST_EMBXMLMAININFO
{
	int ver;
	int nType;
	CString guid;
	ST_EMBXMLMAININFO()
	{
		ver = INVALID_VALUE;
		nType = embxmltype_none;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};
//////////////////////////////////////////////////////////////////////////
#define EDOC_COMMON_STRUCT TEXT("<emb></emb>")

#define EDOC_ST_EXCUTORREG_STRUCT TEXT("<excutorreg></excutorreg>")
//note: ST_EXCUTORREG's ToString
struct ST_EXCUTORREG
{
	EXCUTORID guid;
	ACTORID actorId;
	HWND hwndActor;
	HWND hwndExcutor;
	ST_EXCUTORREG()
	{
		actorId = INVALID_ID;
		guid = -1;
		hwndActor = NULL;
		hwndExcutor = NULL;
	}

	BOOL ToExcParamString(CString& strOut);
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
};

//////////////////////////////////////////////////////////////////////////
#define EDOC_ST_EMBRET_STRUCT TEXT("<ST_EMBRET></ST_EMBRET>")

struct ST_EMBRET
{
	int nRetVal;
	CString strMsg;
	ST_EMBRET()
	{
		nRetVal = -1;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
};

//////////////////////////////////////////////////////////////////////////
#define EDOC_ST_ACTORREG_STRUCT TEXT("<TaskActorConfig></TaskActorConfig>")
struct ST_ACTORCONFIG
{
	int actorId;
	SOCKADDR_IN addrMain;
	SOCKADDR_IN addrSlave;
	int nExcutorMinId;
	int nExcutorMaxId;
	int nActorLevel;
	CString strExcPath;
	ST_ACTORCONFIG()
	{
		actorId = -1;
		nExcutorMinId = -1;
		nExcutorMaxId = -1;
		addrMain.sin_family = 0;
		addrSlave.sin_family = 0;
		nActorLevel = embActorLevel_normal;
	}

	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};
//////////////////////////////////////////////////////////////////////////
struct ST_STORAGECONFIG
{
	int nType;
	union
	{
		struct  
		{
			char szdbSvr[64];
			char szdbName[64];
			char szUser[64];
			char szPw[64];
		}dbCfg;
		
	}data;
};


//////////////////////////////////////////////////////////////////////////
struct ST_EDOCMAINHEADER
{
	int nVer;
	int nType;
	CString strGuid;
	ST_EDOCMAINHEADER()
	{
		nVer = -1;
		nType = -1;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};



typedef vector<CString> VECSUBTASKS;
//////////////////////////////////////////////////////////////////////////
struct ST_TASKBASIC
{
	CString strGuid;
	int nPriority;
	time_t tmSubmit;
	int nStartStep;
	ACTORID nFixActor;
	VECSUBTASKS vSubTask;
	ST_TASKBASIC()
	{
		nStartStep = 0;
		tmSubmit = 0;
		nPriority = embtaskPriority_normal;
		nFixActor = INVALID_ID;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

//////////////////////////////////////////////////////////////////////////
#define EDOC_ST_TASKRUNSTATE_STRUCT TEXT("<ST_TASKRUNSTATE></ST_TASKRUNSTATE>")

struct ST_TASKRUNSTATE
{
	GUID guid;
	ACTORID actorId;
	EXCUTORID excId;
	int nState;
	int nExcType;
	int nCurrStep;
	int nPercent;
	int nRetry;
	INT64 tmCommit;
	INT64 tmExcute;
	INT64 tmLastReport;
	INT64 tmLastCheck;
	ST_TASKRUNSTATE()
	{
		guid = GUID_NULL;
		actorId = INVALID_ID;
		excId = INVALID_ID;
		nState = embtaskstate_zero;
		nExcType = embexcuteType_none;
		tmCommit = 0;
		tmExcute = 0;
		tmLastReport = 0;
		nRetry = 0;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};


struct ST_EXCUTORINFO
{
	EXCUTORID excutorId;
	HWND hwnd;
	DWORD hProcessId;
	HANDLE hmemMap; //to write data for sending to excutor
	CString strDesExcMappingName;
	time_t tmLastcheck;
	ST_EXCUTORINFO()
	{
		excutorId = INVALID_ID;
		hwnd = NULL;
		hProcessId = NULL;
		tmLastcheck = 0;
	}
};

struct ST_SVRLIVEINFO
{
	int nsvrId;
	int nMaster;
	int nActive;
	int nConnState;
	ST_SVRLIVEINFO()
	{
		nsvrId = INVALID_ID;
		nMaster = -1;
		nActive = -1;
		nConnState = -1;
	}
};

//////////////////////////////////////////////////////////////////////////
interface IServerLiveInterface
{
	virtual HRESULT GetSelfState(ST_SVRLIVEINFO& infoOut) = 0;
};
interface IRemoteSvrLiveInterface
{
	virtual HRESULT GetRemoteSvrState(ST_SVRLIVEINFO& infoOut) = 0;
};

//////////////////////////////////////////////////////////////////////////
struct ST_PROBERDATA
{
	struct  ST_IPPROBERDATA
	{
		SOCKADDR_IN addrListen;
	};
	CString strProberName;
	int nType;
	union
	{
		ST_IPPROBERDATA ipdata;
	}data;


};

typedef vector<ST_PROBERDATA> VECPROBERS;
struct ST_TASKRISERCONFIG
{
	VECPROBERS vProbes;
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};


struct ST_LOADEDPLUGIN
{
	HANDLE handle;
	EMB::IPluginBaseInterface* pIface;
	CString strParam;
	ST_LOADEDPLUGIN()
	{
		handle = NULL;
		pIface = NULL;
	}
};
typedef vector<ST_LOADEDPLUGIN> VECLOADEDPLUGINS;


struct ST_TASKDISPATCHCONFIG
{
	int nMaster;
	int nSvrID;
	CString strIpActorHolder;
	CString strIpMaster;
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};


//////////////////////////////////////////////////////////////////////////
//
struct ST_TASKUP_END
{
	
	int nEndState;
	DISPATCHID dispatchid;
	ACTORID actorid;
	EXCUTORID excutorid;
	ST_TASKUP_END()
	{
		nEndState = embtaskstate_none;
		dispatchid = INVALID_ID;
		actorid = INVALID_ID;
		excutorid = INVALID_ID;
	}
};

struct ST_TASKUP_PRIORITY
{
	int nPriority;
};

struct ST_TASKUPDATE
{
	int nUpdateType;
	TXGUID guid;

	ST_TASKUP_END data_end;
	ST_TASKUP_PRIORITY data_pri;

	ST_TASKUPDATE()
	{
		nUpdateType = embtaskupdatetype_none;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

typedef map<CString, int> MAPDISKUSEAGE;
struct ST_ACTORSTATE
{
	ACTORID actorId;
	int nActorLevel;
	int nConnState;
	//runtime state;
	int nCpuUsage;
	int nMemUsage;
	int nDiscUsage;
// 	CString strHost;
// 	MAPDISKUSEAGE mapDiskUse;
	
	time_t tmLastReport;
	time_t tmLastCheck;

	ST_ACTORSTATE()
	{
		nConnState = embActorConnState_ok;
		actorId = INVALID_ID;
		nCpuUsage = INVALID_VALUE;
		nMemUsage = INVALID_VALUE;
		nDiscUsage = INVALID_VALUE;
		nActorLevel = embActorLevel_normal;
		tmLastReport = 0;
		tmLastCheck = 0;
	}

	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};


//////////////////////////////////////////////////////////////////////////
struct ST_TASKREPORT
{
	CString strGuid;
	ACTORID actorId;
	EXCUTORID excutorId;
	int nState;
	int nSubErrorCode;
	int nPercent;
	int nStep;
	ST_TASKREPORT()
	{
		actorId = INVALID_ID;
		excutorId = INVALID_ID;
		nState = embtaskstate_none;
		nPercent = 0;
		nStep = INVALID_VALUE;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

struct ST_SVRACTIVEINFO
{
	int nActive;
	int nMaster;
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

struct ST_MD5TASKINFO
{
	CString strFileToCheck;
	CString strFileToWriteResult;
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

struct ST_WORKERREPORT
{
	int nPercent;
	int code;

	ST_WORKERREPORT()
	{
		nPercent = 0;
		code = 0;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);


};