#pragma once
#include <vector>
#include "FGlobal.h"
#include "EMBDefine.h"
#include "IEMBBaseInterface.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
#define EDOC_COMMON_STRUCT TEXT("<emb></emb>")

#define EDOC_ST_EXCUTORREG_STRUCT TEXT("<ST_EXCUTORREG></ST_EXCUTORREG>")
struct ST_EXCUTORREG
{
	EXCUTORID guid;
	HWND hwndActor;
	HWND hwndExcutor;
	ST_EXCUTORREG()
	{
		guid = -1;
		hwndActor = NULL;
		hwndExcutor = NULL;
	}

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
#define EDOC_ST_ACTORREG_STRUCT TEXT("<ST_ACTORREG></ST_ACTORREG>")
struct ST_ACTORREG
{
	int guid;
	SOCKADDR_IN addrMain;
	SOCKADDR_IN addrSlave;
	int nExcutorMinId;
	int nExcutorMaxId;
	CString strExcPath;
	ST_ACTORREG()
	{
		guid = -1;
		nExcutorMinId = -1;
		nExcutorMaxId = -1;
		addrMain.sin_family = 0;
		addrSlave.sin_family = 0;
	}

	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

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

typedef vector<CString> VECTASKEXCACTS;
//////////////////////////////////////////////////////////////////////////
struct ST_TASKBASIC
{
	CString strGuid;
	int nPriority;
	VECTASKEXCACTS vexclist;
	ACTORID nFixActor;
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
	INT64 tmCommit;
	INT64 tmExcute;
	INT64 tmLastReport;
	INT64 tmLastCheck;
	int nPercent;
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
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};


struct ST_EXCUTORINFO
{
	EXCUTORID excutorId;
	HWND hwnd;
	DWORD hProcessId;
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
		SOCKADDR_IN addrLocal;
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