#pragma once
#include  <map>
#include <vector>
#include "GlobalDef.h"
#include "LuaStrategy.h"


using namespace std;

#define STRGETINCOMMINGONLINETASKFMT TEXT("select top %d PGMCode, PGMName, SourceID, PlayDateTime, ChnID, nDuration, nClipType, nPRI, SourceStrXML, TaskID, Version from T_IncomingTaskPool \
where TaskID is null and ((PlayDateTime > '%s' and PlayDateTime < '%s') or nPRI >= %d) order by\
 nPRI desc, PlayDateTime ")

//param 9
#define STRGETINCOMMINGONLINETASKFMT2 TEXT("select top %d PGMCode, PGMName, SourceID, PlayDateTime, Tb1.tPlaylistPlayDateTime, ChnID, nDuration, nClipType, nPRI, SourceStrXML, TaskID, Version from T_IncomingTaskPool left join \
(select T0.strPrimaryID,  DATEADD(millisecond, t1.nPGMSchStartTime, t1.tPGMSchPlayDate) as tPlaylistPlayDateTime \
 from %s.dbo.t_BIDMapCompile as T0 join %s.dbo.t_PlaylistCompile as T1 on T0.strPGMGuid = T1.strPGMGuid \
  join %s.dbo.t_ProgramSheet as T2 on t2.strPSID = t1.strPSID \
where t2.nState = 2 and DATEADD(millisecond, t1.nPGMSchStartTime, t1.tPGMSchPlayDate) !< '%s' and DATEADD(millisecond, t1.nPGMSchStartTime, t1.tPGMSchPlayDate) !> '%s') as Tb1 on tb1.strPrimaryID = PGMCode \
where TaskID is null and \
((PlayDateTime !< '%s' and PlayDateTime !> '%s') or nPRI >= %d)   order by \
nPRI desc, PlayDateTime") 

#define STRUPDATEINCOMINGPOOLFMT TEXT("if (select COUNT(*) from T_IncomingTaskPool where PGMCode = '%s' and TaskID is null) = 1 \
 update T_IncomingTaskPool set TaskID = '%s' where PGMCode = '%s' \
 else \
begin \
update T_IncomingTaskPool set nerrcode = %d where PGMCode = '%s' \
end \
select taskid, nerrcode from T_IncomingTaskPool where PGMCode = '%s'")  

enum ENUMCOL_MTASKS
{
	mtaskcol_no,
	mtaskcol_pgmcode,
	mtaskcol_clipname,
	mtaskcol_sourcetype,
	mtaskcol_submittime,
	mtaskcol_playtime,
	mtaskcol_chid,
	mtaskcol_duration,
	mtaskcol_cliptype,
	mtaskcol_npri,
	mtaskcol_embtaskid,
	mtaskcol_version,
	mtaskcol_errcode,


	//playlist task col
	mtaskcol_clipid,
	mtaskcol_strLocation1,
	mtaskcol_strLocation2,
	mtaskcol_clipstate,

	//emb info
	mtaskcol_embstate,
	mtaskcol_emberrcode,
	mtaskcol_inpoolstate,
	mtaskcol_inpoolplaytime,
	mtaskcol_inpoolerrcode,


	//auto task
	mtaskcol_embsubmittime,
	mtaskcol_stgname,
	mtaskcol_tplname,

};

typedef map<int, ST_EMBTEMPLATEINFO> MAPTEMPLATERC; 

struct ST_STGRUNTIMEDATA
{
	ST_STGPARAM stgParam;
	MAPTEMPLATERC mapRC; //<return val, template content>
	CLuaStrategy* pLua;
};

typedef vector<ST_STGRUNTIMEDATA> VECSTGRUNTIMEDATA;

struct ST_BREAKPTDATA
{
	ST_BREAKPTDATA()
	{

	}

	ST_BREAKPTDATA(int nPtin, LPCTSTR szDescIn)
	{
		nBreakPt =nPtin;
		strDesc = szDescIn;
	}
	int nBreakPt;
	CString strDesc;
};

typedef vector<ST_BREAKPTDATA> VECBREAKPT;

struct ST_INPOOLTASKDATA
{
	CString strClipName;
	CString strPgmCode;
	INT64 tmSubmit;
	INT64 tmPlayTimeInPGM; //latest play time in Playlist
	INT64 tmPlayTime;		//play time in bvsid
	CString strChID;
	INT64 nDuration;	//frame
	int nClipType;
	int nPriority;		//1, 0
	int nVersion;

	CString strTaskXml;

	//task info after extract strTaskXml;
	int nHDType;
	int nClipSrctype; //clip source: meizi, zhugan
	int nSom; //frame
	int nEom;
	CString strFileName;
	CString strMd5;
	CString strftpPath;

	vector<ST_FTPSITEINFO> vFtpSrc;
	vector<ST_BREAKPTDATA> vBreakPts;

	//task info after submit;
	CString strClipID;
	CString strTaskId;
	int nErrcode;
	CString strStgName; //strategy name
	CString strTplName; //template name

	ST_INPOOLTASKDATA()
	{
		tmPlayTimeInPGM = 0;
		nErrcode = 0;
		nPriority = 0;
	}
};



typedef vector<ST_INPOOLTASKDATA> VECINPOOLTASK;



// CDlgPlaylistTask ¶Ô»°¿ò
struct ST_PLAYSHEETINFO
{
	CString strPSId;
	CString strSectionId;
	INT64 tmlastModify;
	CString strPlayListVersion;
	CString strRemark;
	CString strLastModifyUser;
	CString strLastAuditUser;
};

typedef vector<ST_PLAYSHEETINFO> VECPLAYSHEETS;


#define psclipstate_unknow 0
#define psclipstate_inLoc1 1
#define psclipstate_inLoc2 2
#define psclipstate_inEmbtask 4
#define psclipstate_inIntaskpool 8

struct ST_PSCLIPINFO
{
	CString strPgmCode;
	CString strPgmGuid;
	CString strPgmName;
	CString strClipName;
	INT64 nPgmDuration;
	CString strClipID;
	INT64 tPgmPlaytime;
	CString strLocation1;
	CString strLocation2;

	CString strEmbTaskId;
	int nEMBErrCode;
	int nEMBState;

	int nInPoolErrCode;
	INT64 tInPoolPlaytime;
	CString strInPoolEmbtaskId;

	int nClipState;  // psclipstate_xxx

	ST_PSCLIPINFO()
	{
		tPgmPlaytime = 0;
		nInPoolErrCode = 0;
		nEMBErrCode = 0;
		nClipState = 0;
		tInPoolPlaytime = 0;
		nPgmDuration = 0;
	}
};

typedef vector<ST_PSCLIPINFO> VECPSCLIPS;
typedef vector<ST_PSCLIPINFO*>VECPSCLIPPTRS;

class CAutoTaskFunc : public IStrategyRuntimeCallback
{
public:
	CAutoTaskFunc(void);
	~CAutoTaskFunc(void);

	BOOL InitDBConn();
	BOOL InitStg();
	BOOL UnInitStg();


	HRESULT GetOrgTasksFromDB(CTime& tmBeginIn, CTime& tmEndIn, VECINPOOLTASK& vOrgTaskOut, const int nMaxCountIn=1000);
	
	HRESULT ApplyStgs(ST_INPOOLTASKDATA& orgDataIn, ST_STGRUNTIMEDATA& stgInfoIn);

	HRESULT CommitEmbTask(ST_INPOOLTASKDATA& orgDataIn, ST_EMBTEMPLATEINFO& tplInfoIn, BOOL bExtractXml= TRUE);

	HRESULT ExtractTaskInfoFromXml(ST_INPOOLTASKDATA& orgDataIn, const BOOL bGetAll = FALSE);

	//interface
	BOOL OnStrategyRunMessage(int nMsgType, CString& strValIn);

	//if not use dbkey2 set to -1
	BOOL RegisterBreakPointsInfo(ST_INPOOLTASKDATA& orgDataIn, DWORD dbKeyIn, DWORD dbKeyIn2 = -1);

	VECSTGRUNTIMEDATA m_stgOnline;
	
	VECSTGRUNTIMEDATA m_stgIdle;

	VECSTGRUNTIMEDATA m_stgEmergency;

	MAPTPLCONTENT m_mapTaskTemplates;

};

 

CString GetTaskColText(const int ColTypeIn, ST_INPOOLTASKDATA& dataIn);
