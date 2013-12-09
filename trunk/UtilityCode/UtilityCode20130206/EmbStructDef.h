/********************************************************************
	created:	2013/11/25
	created:	$2013:11:25
	filename: 	EmbStructDef.h
	file base:	EmbStructDef
	file ext:	h
	author:		tianyx
	
	purpose:	struct define for emb project
*********************************************************************/
#pragma once
#include <vector>
#include "FGlobal.h"
#include "EMBDefine.h"
#include "IEMBBaseInterface.h"
#include "MBCCommonDef.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
//struct for windows message trans
struct ST_EMBWNDMSG
{
	HWND hwnd;		//handle that send the message
	UINT message;	//message id
	WPARAM wparam;	//
	LPARAM lparam;	//
	char* pStr;		//buffer
	int nBuffLen;	//buffer len
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
//struct for emb xml main node info
//call GetEmbXmlMainInfo() to fill the struct from a string.
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
//struct for excutor register
#define EDOC_COMMON_STRUCT TEXT("<emb></emb>")

#define EDOC_ST_EXCUTORREG_STRUCT TEXT("<excutorreg></excutorreg>")
//note: ST_EXCUTORREG's ToString
struct ST_EXCUTORREG
{
	EXCUTORID guid;		//excutor id
	ACTORID actorId;	//actor id
	HWND hwndActor;		//actor hwnd for message receive/send
	HWND hwndExcutor;	//excutor hwnd for message receive/send
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
//struct for emb return string
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
//struct for actor config
#define EDOC_ST_ACTORREG_STRUCT TEXT("<TaskActorConfig></TaskActorConfig>")
struct ST_ACTORCONFIG
{
	int actorId;				//actor id
	SOCKADDR_IN addrMain;		//master svr addr
	SOCKADDR_IN addrSlave;		//slave svr addr
	int nExcutorMinId;			//min excutor to use
	int nExcutorMaxId;			//max excutor to use
	int nActorLevel;			//actor level that corresponding with task priority, task can assign to the actor if the nActorLevel <= task priority
	CString strExcPath;			//executor path to call excutor.exe, if not full path, will check executor at default folder
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
//config for storage
struct ST_TASKSTORAGECONFIG
{
	int nType;
	CString strDBConnect;
	union
	{
		struct  
		{
			char szdbSvr[64];		//db server
			char szdbName[64];		//db name
			char szUser[64];		//db user
			char szPw[64];			//db pw
		}dbCfg;
		
	}data;

	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
};


//////////////////////////////////////////////////////////////////////////
//for doc main header not used
// struct ST_EDOCMAINHEADER
// {
// 	int nVer;
// 	int nType;
// 	CString strGuid;
// 	ST_EDOCMAINHEADER()
// 	{
// 		nVer = -1;
// 		nType = -1;
// 	}
// 	BOOL ToString(CString& strOut);
// 	BOOL FromString(const CString& strIn);
// 	
// };



typedef vector<CString> VECSUBTASKS;
//////////////////////////////////////////////////////////////////////////
//for sub-node "basic" in task xml
struct ST_TASKBASIC
{
	CString strGuid;		//task guid
	int nPriority;			//task priority
	time_t tmSubmit;		//time of task submited
	int nStartStep;			//first step to process
	ACTORID nFixActor;		//specify a actor to executor
	VECSUBTASKS vSubTask;	//sub task name list
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
//for task running state
#define EDOC_ST_TASKRUNSTATE_STRUCT TEXT("<ST_TASKRUNSTATE></ST_TASKRUNSTATE>")

struct ST_TASKRUNSTATE
{
	GUID guid;			//task guid
	ACTORID actorId;	//actor id to which the task assigned. 
	EXCUTORID excId;	//executor id to which the task assigned
	int nState;			//task running state
	int nExcType;		//the running sub task type on the executor
	int nCurrStep;		//current running NO. of sub task
	int nPercent;		//progress of current running sub task 
	int nRetry;			//retry times of task
	INT64 tmCommit;		//time of commiting
	INT64 tmExcute;		//time of executing
	INT64 tmLastReport;	//time of last reporting by executor
	INT64 tmLastCheck;	//time of last checking
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
	CString StateDes();
	
};

//////////////////////////////////////////////////////////////////////////
//executor info that hold by actor
#define EDOC_ST_ST_EXCUTORINFO TEXT("<ExecutorInfo></ExecutorInfo>")

enum EXE_STATE
{
	EXE_IDLE = 1, // ����
	EXE_ASSIGN = 2, // �ѷ���
	EXE_RUN  = 3, // ����
};

struct ST_EXCUTORINFO
{
	EXCUTORID excutorId;	
	HWND hwnd;						//hwnd for trans message with executor
	DWORD hProcessId;
	HANDLE hmemMap;					//mapping buffer for sending message to executor
	CString strDesExcMappingName;	//mapping buffer name
	time_t tmLastcheck;				//time for last check executor state
	EXE_STATE m_eState;             // ״̬

	CString m_strTaskGuid;          // �����ʶ
	CString m_strRunStep;           // ���в�������
	int		m_nPercent;             // ����

	ST_EXCUTORINFO()
	{
		excutorId = INVALID_ID;
		hwnd = NULL;
		hProcessId = NULL;
		tmLastcheck = 0;
		m_eState = EXE_IDLE;   // Ĭ��Ϊ����

		m_strTaskGuid.Empty();
		m_strRunStep.Empty();
	}

	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
};

//////////////////////////////////////////////////////////////////////////
//server live info 
struct ST_SVRLIVEINFO
{
	int nsvrId;			//server id
	int nMaster;		//see embSvrType_xxx
	int nActive;		//see embSvrState_xxx
	int nConnState;		//see embConnState_xxx
	ST_SVRLIVEINFO()
	{
		nsvrId = INVALID_ID;
		nMaster = -1;
		nActive = -1;
		nConnState = -1;
	}
};

//////////////////////////////////////////////////////////////////////////
//interface for master/slave live pack
interface IServerLiveInterface
{
	virtual HRESULT GetSelfState(ST_SVRLIVEINFO& infoOut) = 0;
};
interface IRemoteSvrLiveInterface
{
	virtual HRESULT GetRemoteSvrState(ST_SVRLIVEINFO& infoOut) = 0;
};

//////////////////////////////////////////////////////////////////////////
//struct for prober of task riser
struct ST_PROBERDATA
{
	struct  ST_IPPROBERDATA
	{
		SOCKADDR_IN addrListen;			//listen addr
		SOCKADDR_IN addrLocal;			//local bind adder
	};
	CString strProberName;
	int nType;
	union
	{
		ST_IPPROBERDATA ipdata;
	}data;
	
	
};

//for prober config
typedef vector<ST_PROBERDATA> VECPROBERS;
struct ST_TASKRISERCONFIG
{
	VECPROBERS vProbes;
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
	
};

//plugIn info when using LoadPluginByPluginMgr
struct ST_LOADEDPLUGIN
{
	HANDLE handle;
	EMB::ITxUnkown* pIface;
	CString strParam;
	ST_LOADEDPLUGIN()
	{
		handle = NULL;
		pIface = NULL;
	}
};
typedef vector<ST_LOADEDPLUGIN> VECLOADEDPLUGINS;


//////////////////////////////////////////////////////////////////////////
//for task dispatch config
struct ST_TASKDISPATCHCONFIG
{
	int nMaster;					//see embSvrType_xxx
	int nSvrID;
	CString strIpActorHolder;		//linsten addr for actor holder. actor will connect to this addr
	CString strIpMaster;			//if nMaster == embSvrType_slave, fill the master ip to connect
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
//for actor report task state to dispatcher
struct ST_TASKREPORT
{
	CString strGuid;			//task id
	ACTORID actorId;			//actor id  to which the task assigned
	EXCUTORID excutorId;		//executor id on which the task running
	int nState;					//see embtaskstate_xxxx
	int nSubErrorCode;			//detail error code, see EMBERR_xxxx
	int nPercent;				//sub task progress
	int nStep;					//current running sub task NO.
	ST_TASKREPORT()
	{
		actorId = INVALID_ID;
		excutorId = INVALID_ID;
		nState = embtaskstate_none;
		nPercent = 0;
		nStep = INVALID_VALUE;
		nSubErrorCode = 0;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
	BOOL NeedResetTask() // �Ƿ�����?
	{
		BOOL notReset = (EMBERR_SUBTASKLOADDLL == nSubErrorCode || EMBERR_TASKSUBMIT == nSubErrorCode);

		return !notReset;
	}

};

//////////////////////////////////////////////////////////////////////////
//for server state trans
struct ST_SVRACTIVEINFO
{
	int nActive;							//see embSvrState_xxx
	int nMaster;							//see embSvrType_xxx
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

//////////////////////////////////////////////////////////////////////////
// for md5 task xml
struct ST_MD5TASKINFO
{
	CString strFileToCheck;
	CString strFileToWriteResult;
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);

};

//////////////////////////////////////////////////////////////////////////
//for excutor report work state
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
//������궨��
#define FCVS_BLACKFRAME			0x00000001L		//�ڳ�
#define FCVS_STATICFRAME		0x00000010L		//��֡
#define FCVS_COLORFRAME			0x00000100L		//�ʳ�
#define FCVS_COLORSTRIP			0x00001000L		//����
#define FCVS_LOUDNESS			0x00010000L		//���
#define FCVS_VOLUME				0x00100000L		//����
struct ST_FCVSCONFIGSERVICEINFO   // ���������Ϣ
{
	CString Name;
	CString IP;
	int Port;
	CString TYPE;
	CString Protocol;
	BOOL isServer ;
	ST_FCVSCONFIGSERVICEINFO()
	{
		Name = "";
		IP = "";
		Port = 2000;
		TYPE = TEXT("Operation");
		Protocol = TEXT("TCP");
		isServer = TRUE;
	}
};
struct ST_FCVSCONFIGFILEDRIVERINFO  //����������Ϣ
{
	CString name;
	CString classString;
	CString driverName;
	CString fileType;
	BOOL isDefault;
	int DetectedCLIP_WIDTH;
	int DetectedCLIP_HEIGHT;
	ST_FCVSCONFIGFILEDRIVERINFO()
	{
		name = "";
		classString = "";
		driverName = "";
		fileType = "";
		isDefault = FALSE;
		DetectedCLIP_WIDTH = 0;
		DetectedCLIP_HEIGHT = 0;
	}
};
struct ST_FCVSCONFIGDETECTLEVEL  //���󼶱�
{
	int levelID;
	CString levelName;
	int blackFrame;
	int colorFrame;
	int colorSripe;
	int staticFrame;
	int muteDur;
	int HighDur;
	int LowDur;
	ST_FCVSCONFIGDETECTLEVEL()
	{
		levelID = 0;
		levelName = "";
		blackFrame =0;
		colorFrame = 0;
		colorSripe = 0;
		staticFrame =0;
		muteDur = 0;
		HighDur = 0;
		LowDur = 0;
	}
};
struct ST_FCVSCONFIGDETECTTHRESHOLD
{
	CString name;
	double TOP_SAFE_SECTION;
	double BOTTOM_SAFE_SECTION;
	double	LEFT_SAFE_SECTION;
	double RIGHT_SAFE_SECTION;
	double BLACK_SCENE_THRESHOLD;
	double COLOR_SCENE_PERCENTAGE;
	double COLOR_STRIP_PERCENTAGE;
	double STATIC_FRAME_PERCENTAGE	;
	double LINE_COUNTS_FOR_STRIP_DETECT;
	double AMBIT_DEEMED_TO_SAME_PIXEL	;
	double UNDULATE_AMBIT_OF_Y	;
	double UNDULATE_AMBIT_OF_U	;
	double UNDULATE_AMBIT_OF_V	;
	//��������Ƶ����������Ƶ�ļ����ֵ
	double AUDIO_Low_THRESHOLD ;
	double AUDIO_Low_PERIOD   ;
	double AUDIO_High_THRESHOLD        ;
	double AUDIO_High_PERIOD           ;
	double AUDIO_Mute_THRESHOLD        ;
	double AUDIO_Mute_PERIOD           ;  
	ST_FCVSCONFIGDETECTTHRESHOLD()
	{
		name = "";
		TOP_SAFE_SECTION = 0.200000;
		BOTTOM_SAFE_SECTION = 0.200000;
		LEFT_SAFE_SECTION = 0.200000;
		RIGHT_SAFE_SECTION	=0.100000;
		BLACK_SCENE_THRESHOLD	=30;
		COLOR_SCENE_PERCENTAGE	=0.985000;
		COLOR_STRIP_PERCENTAGE	=0.900000;
		STATIC_FRAME_PERCENTAGE	=0.980000;
		LINE_COUNTS_FOR_STRIP_DETECT	=100;
		AMBIT_DEEMED_TO_SAME_PIXEL	=1;
		UNDULATE_AMBIT_OF_Y	=3;
		UNDULATE_AMBIT_OF_U	=3;
		UNDULATE_AMBIT_OF_V	=3;

		AUDIO_Low_THRESHOLD         =-38;  //��Ƶ������ֵ����λ���ֱ�
		AUDIO_Low_PERIOD            =1;   //��Ƶ���ͳ���ʱ�䣬��λ����
		AUDIO_High_THRESHOLD        =-9;  //��Ƶ������ֵ����λ���ֱ�
		AUDIO_High_PERIOD           =0.5; //��Ƶ���߳���ʱ�䣬��λ����
		AUDIO_Mute_THRESHOLD        =-60;  //��Ƶ������ֵ����λ���ֱ�
		AUDIO_Mute_PERIOD           =1;    //��Ƶ��������ʱ�䣬��λ����
	}
};
//��������
struct ST_FCVSCONFIGINFO
{
	CString serverName ;
	//CString dataBaseInfo ;
	BOOL bSaveXML ;
	//BOOL bSaveDB;
	BOOL bCheckAudio;
	BOOL bUseSection; // �Ƿ����ü����� Ĭ��FALSE
	
// 	int serviceCount;
// 	ST_FCVSCONFIGSERVICEINFO serVice[10]; 
	//����ע��count
	
	int fileDriverCount;
	ST_FCVSCONFIGFILEDRIVERINFO fileDriver[20];
	
	int detectLevelCount;
	ST_FCVSCONFIGDETECTLEVEL detectLevel[5];
	
	int detectThresholodCount;
	ST_FCVSCONFIGDETECTTHRESHOLD detectThresholod[5];
	ST_FCVSCONFIGINFO()
	{
		serverName = TEXT("�������");
		//dataBaseInfo = "";
		bSaveXML = FALSE;
		//bSaveDB = TRUE;
		bCheckAudio = TRUE;
		bUseSection = FALSE;
		//serviceCount = 0;
		fileDriverCount = 0;
		detectLevelCount = 0;
		detectThresholodCount = 0;
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
};


//����������Ϣ
struct ST_FCVSTASKINFO
{
	CString clipType;    
	CString fileMediaType;    
	int checkItem ;  //����������궨��
	
	//BOOL FCVSTaskCut ; //��Ƭ����Ĭ�Ϸ�
	LONGLONG TaskCutSOM;  //��Ƭ���
	LONGLONG TaskCutEOM; //��Ƭ����
	CString usedDetectLevel;
	
	CString fileAdrType;
	CString filePath;
	CString fileName;
	
	CString fileAdrIP;
	CString fileAdrUser;
	CString fileAdrPwd;
	CString fileAdrPort;
	ST_FCVSTASKINFO()
	{
		clipType = TEXT("��Ӱ");
		fileMediaType = "";
		checkItem = 0x00111111L;
		//FCVSTaskCut = FALSE;
		TaskCutSOM = 0;
		TaskCutEOM = 0;
		usedDetectLevel  = TEXT("B");
		fileAdrType = TEXT("����");
		filePath = "";
		fileName = "";
		fileAdrIP = "";
		fileAdrUser = "";
		fileAdrPwd = "";
		fileAdrPort = "";
	}
	BOOL ToString(CString& strOut);
	BOOL FromString(const CString& strIn);
};
struct ST_FCVS  
{
	ST_FCVSTASKINFO taskInfo;
	ST_FCVSCONFIGINFO configInfo;
	BOOL ToString(CString& strOut);
	//BOOL FromString(const CString& strIn);
	//������Ҫ ��ʱ���������������FromString
};
