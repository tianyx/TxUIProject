#include "StdAfx.h"
#include "AutoTaskFunc.h"
#include "TxADOConn.h"
#include "Markup.h"
#include "FGlobal.h"


CAutoTaskFunc::CAutoTaskFunc(void)
{
}

CAutoTaskFunc::~CAutoTaskFunc(void)
{
	UnInitStg();
}

BOOL CAutoTaskFunc::InitStg()
{
	UnInitStg();
	if (!(g_GlobalInfo.bIsStrategyOk && g_GlobalInfo.bIsTemplateOk))
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("CDlgAutoTask::InitStg failed."));
		return FALSE;
	}

	EMB::CAutoLock lock(&g_csStg);
	for (size_t i = 0; i < g_GlobalInfo.stgConfig.vStgs.size(); ++i)
	{
		ST_STGPARAM& stgRef = g_GlobalInfo.stgConfig.vStgs[i];
		if (stgRef.nStgType <= stgtype_offline)
		{
			continue;
		}

		ST_STGRUNTIMEDATA stgTmp;
		stgTmp.stgParam = stgRef;
		MAPKEYTEMPLATE::iterator itb = stgRef.mapKt.begin();
		MAPKEYTEMPLATE::iterator ite = stgRef.mapKt.end();
		for (; itb != ite; ++itb)
		{
			MAPTPLCONTENT::iterator itf = g_GlobalInfo.mapTaskTemplates.find(itb->second);
			if (itf != g_GlobalInfo.mapTaskTemplates.end())
			{
				stgTmp.mapRC[itb->first] = itf->second;
			}
		}

		stgTmp.pLua = new CLuaStrategy;
		stgTmp.pLua->InitStrategyFromString(stgRef.strName, stgRef.strLua, this);

		
		if (stgRef.nStgType == stgtype_online)
		{
			m_stgOnline.push_back(stgTmp);
		}
		else if (stgRef.nStgType == stgtype_idle)
		{
			m_stgIdle.push_back(stgTmp);
		}
		else if (stgRef.nStgType == stgtype_emergency)
		{
			m_stgEmergency.push_back(stgTmp);
		}
	}

	m_mapTaskTemplates = g_GlobalInfo.mapTaskTemplates;

	if (m_stgOnline.size() == 0)
	{
		CFWriteLog(0, TEXT("online strategy not found..."));
		return FALSE;
	}

	CFWriteLog(0, TEXT("strategy loaded, online=%d, idle=%d, emergency=%d"), m_stgOnline.size(), 
		m_stgIdle.size(), m_stgEmergency.size());
	if (m_stgEmergency.size() == 0)
	{
		CFWriteLog(0, TEXT("[!]emergency strategy not found, will use first online strategy instead."));
	}

	return TRUE;
}

HRESULT CAutoTaskFunc::GetOrgTasksFromDB( CTime& tmBeginIn, CTime& tmEndIn, VECINPOOLTASK& vOrgTaskOut, const int nMaxCountIn/*=1000*/ )
{
	HRESULT hr = S_OK;
	CTime tmNow(time(NULL));
	CString strSql;
	strSql.Format(STRGETINCOMMINGONLINETASKFMT2, nMaxCountIn, g_GlobalInfo.strPlaylistDBName, g_GlobalInfo.strPlaylistDBName,
		g_GlobalInfo.strPlaylistDBName,tmBeginIn.Format(TEXT("%Y-%m-%d %H:%M:%S")), tmEndIn.Format(TEXT("%Y-%m-%d %H:%M:%S")),
		tmBeginIn.Format(TEXT("%Y-%m-%d %H:%M:%S")), tmEndIn.Format(TEXT("%Y-%m-%d %H:%M:%S")), g_GlobalInfo.nEmergencyPriority);
	
	CTxADORecordSet rst(DBKEY_EMB);
	if (rst.ExecuteSQL(strSql))
	{
		int nCount = 0;
		while(!rst.IsEOF())
		{
			ST_INPOOLTASKDATA tmpData;
			tmpData.strPgmCode = rst.GetVal(TEXT("PGMCode")).GetAsString();
			if (tmpData.strPgmCode.IsEmpty() || tmpData.nDuration == 0)
			{
				CFWriteLog(TEXT("error task found!"));
// 				CTxADOCommand cmd;
// 				CString strSqlWr;
// 				strSqlWr.Format(TEXT("update T_IncomingTaskPool set TaskID = '', nErrcode = %d where PGMCode = '%s' "),EMBERR_INVALIDARG);
// 				cmd.ExecuteSQL(strSqlWr);
// 				rst.SetVal(TEXT("TaskID"), TEXT(""));
// 				rst.SetVal(TEXT("nErrcode"), EMBERR_INVALIDARG);
			}
			else
			{
				tmpData.nDuration = rst.GetVal(TEXT("nDuration")).GetAsInt64();
				tmpData.strClipName = rst.GetVal(TEXT("PGMName")).GetAsString();
				tmpData.nClipSrctype = ExternalClipSourceId2Local(rst.GetVal(TEXT("SourceID")).GetAsString());
				tmpData.nClipType = ExternalClipType2Local(rst.GetVal(TEXT("nClipType")).GetAsString());
				tmpData.strChID = ExternalChId2LocalChid(rst.GetVal(TEXT("ChnID")).GetAsString());
				tmpData.nPriority = rst.GetVal(TEXT("nPRI")).GetAsInt();
				tmpData.tmPlayTime = rst.GetVal(TEXT("PlayDateTime")).GetAsInt64();
				tmpData.strTaskXml = rst.GetVal(TEXT("SourceStrXML")).GetAsString();
				tmpData.nVersion = rst.GetVal(TEXT("Version")).GetAsInt();
				tmpData.tmPlayTimeInPGM = rst.GetVal(TEXT("tPlaylistPlayDateTime")).GetAsInt64();
				BOOL bfind = FALSE;
				for (int i = vOrgTaskOut.size()-1; i >= 0; --i)
				{
					if (vOrgTaskOut[i].strPgmCode == tmpData.strPgmCode)
					{
						bfind = TRUE;
						if (tmpData.tmPlayTimeInPGM > 0 && tmpData.tmPlayTimeInPGM < vOrgTaskOut[i].tmPlayTimeInPGM)
						{
							//update pgm playtime only
							vOrgTaskOut[i].tmPlayTimeInPGM = tmpData.tmPlayTimeInPGM;
						}
					}
				}
				if (!bfind)
				{
					vOrgTaskOut.push_back(tmpData);
					if (++nCount > nMaxCountIn)
					{
						break;
					}
				}
												
				
			}
			rst.MoveNext();
		}

		CFWriteLog(0, TEXT("get incoming task finished, num = %d"), nCount);
	}
	else
	{
		hr = EMBERR_SQLEXEC;
		CFWriteLog(0, TEXT("get incoming task failed."));
	}

	return hr;
}

BOOL CAutoTaskFunc::InitDBConn()
{
	//return CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_EMB, g_GlobalInfo.strEmbTaskDBConn);
	return TRUE;
}

HRESULT CAutoTaskFunc::ApplyStgs( ST_INPOOLTASKDATA& orgDataIn, ST_STGRUNTIMEDATA& stgInfoIn )
{
	HRESULT hr = S_OK;
	INT64 tmNow = time(NULL);

	//start stg
	if (stgInfoIn.pLua)
	{
		//prepare param
		VECPARTOLUA vPar;
		//tmPlayTimeIn, strChidIn, nClipTypeIn, tClipDurIn
		INT64 tPlayTime = (orgDataIn.tmPlayTimeInPGM < tmNow || orgDataIn.tmPlayTimeInPGM == 0) ? orgDataIn.tmPlayTime:orgDataIn.tmPlayTimeInPGM;
		vPar.push_back(ST_PARAMTOLUA(toluapartype_int64, luaparamkey_playtime, tPlayTime));
		vPar.push_back(ST_PARAMTOLUA(toluapartype_string, luaparamkey_chid, orgDataIn.strChID));
		vPar.push_back(ST_PARAMTOLUA(toluapartype_int32, luaparamkey_cliptype, orgDataIn.nClipType));
		vPar.push_back(ST_PARAMTOLUA(toluapartype_int64, luaparamkey_duration, orgDataIn.nDuration));
		int nRetVal = -1;
		if (stgInfoIn.pLua->RunStrategy(STGLUACALLFUNC, vPar, nRetVal)
			&& nRetVal >= 0)
		{
			MAPTEMPLATERC ::iterator itf = stgInfoIn.mapRC.find(nRetVal);
			if (itf != stgInfoIn.mapRC.end())
			{
				//stg applied, call template
				hr =CommitEmbTask(orgDataIn, itf->second);
				if (SUCCEEDED(hr))
				{
					orgDataIn.strTplName =  itf->second.strKey;
					orgDataIn.strStgName = stgInfoIn.stgParam.strName;

					CFWriteLog(0, TEXT("[%s]in pool task commited! pgmcode = %s, taskid = %s, stgname = %s, hr = %x"), 
						Time2Str(time(NULL)), orgDataIn.strPgmCode, orgDataIn.strTaskId, stgInfoIn.stgParam.strName, orgDataIn.nErrcode);
				}
				else
				{
					CFWriteLog(0, TEXT("in pool task commit failed! pgmcode = %s, stgname = %s, hr = %x"),
						orgDataIn.strPgmCode, stgInfoIn.stgParam.strName, orgDataIn.nErrcode);
				}

			}
			else
			{
				CFWriteLog(0, TEXT("lua <stgRet, template> key pair not found, stg = %s,ret = %d"),stgInfoIn.stgParam.strName, nRetVal);
			}
		}
		else
		{
			hr = EMBERR_LUARUNFAIL;
			CFWriteLog(TEXT("apply stg lua failed!, pgmcode =%s,  stg name = %s"),orgDataIn.strPgmCode, stgInfoIn.stgParam.strName);
		}
	}
	else
	{
		ASSERT(FALSE);
		hr = EMBERR_LUARUNFAIL;

	}

	
	return hr;
}

BOOL CAutoTaskFunc::OnStrategyRunMessage( int nMsgType, CString& strValIn )
{

	CFWriteLog(TEXT("lua callback msg type = %d, msg = %s"), nMsgType, strValIn);
	return TRUE;
}

HRESULT CAutoTaskFunc::CommitEmbTask( ST_INPOOLTASKDATA& orgDataIn, ST_EMBTEMPLATEINFO& tplInfoIn , BOOL bExtractXml /*= TRUE*/)
{
	HRESULT hr = S_OK;
	//extract incoming task info
	if (bExtractXml)
	{
		hr = ExtractTaskInfoFromXml(orgDataIn);
	}
	if (hr != S_OK)
	{
		CFWriteLog(0, TEXT("get incoming task info failed! pgmcode = %d"), orgDataIn.strPgmCode);
		//fatal error update to db
		CString strSqlerr;
		strSqlerr.Format(TEXT("update T_IncomingTaskPool set nerrcode = %d, TaskID = '#' where PGMCode = '%s' "), EMBERR_INVALIDARG, orgDataIn.strPgmCode);
		CTxADORecordSet rsup(DBKEY_EMB);
		if (!rsup.ExecuteSQL(strSqlerr))
		{
			CFWriteLog(0, TEXT("update taskpool failed! when taskxml is not valid"));
		}


		return hr;
	}
	
	//replace template key val

	//gen clipid
	CString strClipID = orgDataIn.strClipID;

	if (orgDataIn.strClipID.IsEmpty())
	{
		strClipID = GenClipIDFromDB();
		if (strClipID.IsEmpty())
		{
			ASSERT(FALSE);
			return EMBERR_DBOPEN;
		}
	}
	
	//gen embtaskid
	CString strEmbTaskID = Guid2String(TxGenGuid());
	if (strEmbTaskID.IsEmpty())
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	CString strTaskTpXml = tplInfoIn.strContent;
	//replace {TASKGUID} with strEmbTaskID
	strTaskTpXml.Replace(TEXT("{TASKGUID}"), strEmbTaskID);

	//replace {CLIPID}	with strClipID
	strTaskTpXml.Replace(TEXT("{CLIPID}"), strClipID);

	//replace {CLIPLOGICID} with pgmcode
	strTaskTpXml.Replace(TEXT("{CLIPLOGICID}"), orgDataIn.strPgmCode);

	//replace {SRCFTP}	with srcftp info
	vector<ST_FTPSITEINFO> vSrcftp = orgDataIn.vFtpSrc;
	if (vSrcftp.size() == 0)
	{
		MAPGPFTPSITES::iterator itf = g_GlobalInfo.mapFtpSites.find(orgDataIn.nClipSrctype);
		if (itf != g_GlobalInfo.mapFtpSites.end())
		{
			vSrcftp = itf->second;
		}
		else
		{
			CFWriteLog(0, "fatal error: src ftp list not found!, task gen failed. pgmcode =%s, srctype =%d", orgDataIn.strPgmCode, orgDataIn.nClipSrctype);
			hr = EMBERR_INVALIDARG;
			CString strSqlerr;
			strSqlerr.Format(TEXT("update T_IncomingTaskPool set nerrcode = %d, TaskID = '#' where PGMCode = '%s' "), EMBERR_SRCFTPNOTFOUND, orgDataIn.strPgmCode);
			CTxADORecordSet rsup(DBKEY_EMB);
			if (!rsup.ExecuteSQL(strSqlerr))
			{
				CFWriteLog(0, TEXT("update taskpool failed! when taskxml is not valid"));
			}

			return hr;
		}
	}

	CString strFtp = FtpList2String(vSrcftp, TRUE);
	strTaskTpXml.Replace(TEXT("{SRCFTP}"), strFtp);

	//replace {NSRCSITETRYSTART}  //ftp try begin with
	static UINT snTryStart = 0;
	++snTryStart;
	CString strStart;
	strStart.Format(TEXT("%d"), vSrcftp.size() <= 0? 0:(snTryStart%vSrcftp.size()));
	strTaskTpXml.Replace(TEXT("{NSRCSITETRYSTART}"), strStart);


	//replace {NDESSITETRYSTART}
	strStart.Format(TEXT("%d"), snTryStart%g_GlobalInfo.nDesFtpMaxsize);
	strTaskTpXml.Replace(TEXT("{NDESSITETRYSTART}"), strStart);

	//replace {SRCFILENAME} with src file name
	strTaskTpXml.Replace(TEXT("{SRCFILENAME}"), orgDataIn.strFileName);

	//replace {SRCDIR} with src dir
	strTaskTpXml.Replace(TEXT("{SRCDIR}"), orgDataIn.strftpPath);
	
	//replace db info
	if (strTaskTpXml.Find(TEXT("{TRANSDBINFO}")) != -1)
	{
		ST_DB_BVSIDINFO bvsid;
		bvsid.nBitRate = orgDataIn.nHDType == 0? g_GlobalInfo.nDefBitrate:g_GlobalInfo.nHDBitRate;
		bvsid.nClipType = orgDataIn.nClipType;
		bvsid.nTapeType = 0;
		bvsid.strChID = orgDataIn.strChID;
		bvsid.strClipName = orgDataIn.strClipName;
		bvsid.strDuration = TimeCodeToString(orgDataIn.nDuration);
		bvsid.strEOM = TimeCodeToString(orgDataIn.nEom);
		bvsid.strLDuration = bvsid.strDuration;
		bvsid.strLEOM = bvsid.strEOM;
		bvsid.strLSOM = TimeCodeToString(orgDataIn.nSom);
		bvsid.strSOM = bvsid.strLSOM;
		bvsid.strTapeID  = TEXT("");
		bvsid.strTypeSOM = TEXT("");
		bvsid.tPlanAirTime = Time2Str(orgDataIn.tmPlayTime);
		bvsid.tPlanLastAirTime =  Time2Str(orgDataIn.tmPlayTime + g_GlobalInfo.nClipKeepDays*24*3600);
	

	
		ST_DB_LOCATIONINFO locinfo;
		locinfo.nAfdType = g_GlobalInfo.nAfdInfo;
		locinfo.nBitRate =orgDataIn.nHDType == 0? g_GlobalInfo.nDefBitrate:g_GlobalInfo.nHDBitRate;
		locinfo.strFileSize =TimeCodeToString(orgDataIn.nDuration);
		locinfo.strMediaType = g_GlobalInfo.strDefMediaType;

		CString strDbinfo = TransDBInfo2String(orgDataIn.strPgmCode, strClipID, bvsid, locinfo);
		strTaskTpXml.Replace(TEXT("{TRANSDBINFO}"), strDbinfo);
	}

	//replace md5
	strTaskTpXml.Replace(TEXT("{MD5VAL}"), orgDataIn.strMd5);


	//insert task into db
	CString strTmp;
	strTmp.Format(TEXT("insert into T_EMBTask (strTaskID,strProgramID,strTaskName, nPRI, nState,strExtendInfoXml, nRetry, nDeleted, strTaskSequence) values ('%s','%s','%s', '%d','0', '%s', '%d','0', '%s')"), strEmbTaskID, orgDataIn.strPgmCode, orgDataIn.strClipName, orgDataIn.nPriority, strTaskTpXml, g_GlobalInfo.nEmbtaskRetryCount, tplInfoIn.strTaskSequence);

	CTxADORecordSet rs(DBKEY_EMB);
	BOOL bSuc = rs.ExecuteSQL(strTmp);
	if (!bSuc)
	{
		CFWriteLog(0, TEXT("submit emb task to db failed, pgmcode = %s"), orgDataIn.strPgmCode);
		hr = EMBERR_SQLEXEC;
	}
	else
	{
		orgDataIn.strTaskId = strEmbTaskID;
		CString strSqlUp;
		strSqlUp.Format(STRUPDATEINCOMINGPOOLFMT, orgDataIn.strPgmCode, strEmbTaskID, orgDataIn.strPgmCode, EMBERR_DUPLICATE,orgDataIn.strPgmCode, orgDataIn.strPgmCode);
		if (!rs.ExecuteSQL(strSqlUp))
		{
			CFWriteLog(0, TEXT("update incoming pool failed, pgmcode = %s"), orgDataIn.strPgmCode);
		}
		else
		{
			if (!rs.IsEOF())
			{
				orgDataIn.nErrcode = rs.GetVal(TEXT("nErrcode")).GetAsInt();
				if (orgDataIn.nErrcode == EMBERR_DUPLICATE)
				{
					CFWriteLog(0, TEXT("duplicate submit task!! pgmcode = %s, oldtaskid = %s"), orgDataIn.strPgmCode, orgDataIn.strTaskId);
				}
			}

		}
	}

	if (SUCCEEDED(hr))
	{
		//register program break points
		RegisterBreakPointsInfo(orgDataIn, DBKEY_PLAYLIST, DBKEY_PLAYLIST2);

	}

	return hr;
}

HRESULT CAutoTaskFunc::ExtractTaskInfoFromXml( ST_INPOOLTASKDATA& orgDataIn , const BOOL bGetAll/* = FALSE*/)
{
	//only need ftp list, src type
	HRESULT hr = S_OK;
	ST_FTPSITEINFO ftpInfo;
	CMarkup mdoc;
	mdoc.SetDoc(orgDataIn.strTaskXml);
	if (!mdoc.IsWellFormed())
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("incoming task form not valid, %s"), orgDataIn.strTaskXml);
		return EMBERR_INVALIDARG;
	}
	if (mdoc.FindElem(TEXT("OutMessage")))
	{
		mdoc.IntoElem();
		while(mdoc.FindElem())
		{
			CString strTag = mdoc.GetTagName();
			if (bGetAll)
			{
				if (strTag.Compare(TEXT("programID")) == 0)
				{
					orgDataIn.strPgmCode = mdoc.GetElemContent();
				}
				else if (strTag.Compare(TEXT("programName")) == 0)
				{
					orgDataIn.strClipName = mdoc.GetElemContent();
				}
				else if  (strTag.Compare(TEXT("playDate")) == 0)
				{
					CString strTm = mdoc.GetElemContent();
					orgDataIn.tmPlayTime = Str2Time(strTm);
				}
			}
			if (strTag.Compare(TEXT("HD")) == 0)
			{
				orgDataIn.nHDType = CTxStrConvert(mdoc.GetElemContent()).GetAsInt();
			}
			
			else if (strTag.Compare(TEXT("startpoint")) == 0)
			{
				orgDataIn.nSom = CTxStrConvert(mdoc.GetElemContent()).GetAsInt();
			}
			else if  (strTag.Compare(TEXT("endpoint")) == 0)
			{
				orgDataIn.nEom = CTxStrConvert(mdoc.GetElemContent()).GetAsInt();
			}
			else if (strTag.Compare(TEXT("fileList"))==0)
			{
				mdoc.IntoElem();
				if (mdoc.FindElem(TEXT("OutMessageFile")))
				{
					if(mdoc.FindChildElem(TEXT("name")))
					{
						orgDataIn.strFileName = mdoc.GetChildData();
					}
					else
					{
						ASSERT(FALSE);
						CFWriteLog(0, TEXT("task file name not found! in xml"));
						hr = EMBERR_INVALIDARG;
						break;
					}

					if (mdoc.FindChildElem(TEXT("md5")))
					{
						orgDataIn.strMd5 = mdoc.GetChildData();
					}
				}
				mdoc.OutOfElem();  //out fileList
			}
			else if (strTag.Compare(TEXT("ip"))==0)
			{
				ftpInfo.strFtpIp = mdoc.GetElemContent();
			}
			else if (strTag.Compare(TEXT("path"))==0)
			{
				orgDataIn.strftpPath = mdoc.GetElemContent();
			}
			else if (strTag.Compare(TEXT("port"))==0)
			{
				ftpInfo.nFtpPort = CTxStrConvert(mdoc.GetElemContent()).GetAsInt();
			}
			else if (strTag.Compare(TEXT("user"))==0)
			{
				ftpInfo.strUser = mdoc.GetElemContent();
			}
			else if (strTag.Compare(TEXT("pass"))==0)
			{
				ftpInfo.strPw = mdoc.GetElemContent();
			}
			else if (strTag.Compare(TEXT("BreakPointList"))==0)
			{
				mdoc.IntoElem(); //into BreakPointList
				while(mdoc.FindElem())
				{
					CString strTag = mdoc.GetTagName();
					if (strTag.Compare(TEXT("BreakPoints"))==0)
					{
						mdoc.IntoElem(); //into BreakPoints;
						
						int nBreakPtPos = -1;
						CString strBreakPtDesc; 

						while(mdoc.FindElem())
						{
							CString strTagPt = mdoc.GetTagName();
							if (strTagPt.Compare(TEXT("PointLocation")) == 0)
							{
								nBreakPtPos = CTxStrConvert(mdoc.GetElemContent()).GetAsInt(-1);
							}
							else if (strTagPt.Compare(TEXT("Description")) == 0)
							{
								strBreakPtDesc = mdoc.GetElemContent();
							}
						}
						mdoc.OutOfElem(); //out BreakPoints
						if (nBreakPtPos >= 0)
						{
							orgDataIn.vBreakPts.push_back(ST_BREAKPTDATA(nBreakPtPos, strBreakPtDesc));
						}
						else
						{
							CFWriteLog(0, TEXT("found breakpt error int org task xml!"));
						}
						
					}

				}
				mdoc.OutOfElem();  //out BreakPointList
			}

			/*else if (strTag.Compare(TEXT("ftpList")) == 0)
			{
				mdoc.IntoElem();
				while(mdoc.FindElem())
				{
					CString strTag = mdoc.GetTagName();
					if (strTag.Compare(TEXT("ftpNode")) == 0)
					{
						mdoc.IntoElem(); //into ftpNode;
						while(mdoc.FindElem())
						{
							ST_FTPSITEINFO tmpFtp;
							CString strTagFtp = mdoc.GetTagName();
							if (strTagFtp.Compare(TEXT("ip")== 0)
							{
								tmpFtp.strFtpIp = mdoc.GetElemContent();
							}
							else if (strTagFtp.Compare(TEXT("path")== 0)
							{
								if (orgDataIn.strftpPath.IsEmpty())
								{
									//only get first path
									orgDataIn.strftpPath = mdoc.GetElemContent();
								}
							}
							else if (strTagFtp.Compare(TEXT("port")== 0)
							{
								tmpFtp.nFtpPort = CTxStrConvert(mdoc.GetElemContent()).getas
							}
							else if (strTagFtp.Compare(TEXT("user")== 0)
							{
							}
							else if (strTagFtp.Compare(TEXT("pass")== 0)
							{
							}
						}

						mdoc.OutOfElem();  //out ftpNode
					}
				}
				mdoc.OutOfElem();  //out ftpList
			}*/

		}
	}

	if (!ftpInfo.strFtpIp.IsEmpty())
	{
		orgDataIn.vFtpSrc.push_back(ftpInfo);
	}
	return hr;
}

BOOL CAutoTaskFunc::UnInitStg()
{
	for (size_t i = 0; i < m_stgOnline.size(); ++i)
	{
		if (m_stgOnline[i].pLua)
		{
			delete m_stgOnline[i].pLua;
		}
	}

	for (size_t i = 0; i < m_stgEmergency.size(); ++i)
	{
		if (m_stgEmergency[i].pLua)
		{
			delete m_stgEmergency[i].pLua;
		}
	}

	for (size_t i = 0; i < m_stgIdle.size(); ++i)
	{
		if (m_stgIdle[i].pLua)
		{
			delete m_stgIdle[i].pLua;
		}
	}

	m_mapTaskTemplates.clear();
	m_stgOnline.clear();
	m_stgIdle.clear();
	m_stgEmergency.clear();

	return TRUE;
}

BOOL CAutoTaskFunc::RegisterBreakPointsInfo( ST_INPOOLTASKDATA& orgDataIn, DWORD dbKeyIn, DWORD dbKeyIn2 /*= -1*/ )
{
	if (orgDataIn.vBreakPts.size() == 0
		||orgDataIn.strPgmCode.IsEmpty())
	{
		return TRUE;
	}

	VECSTRINGS vSqls;
	CString strSql;
	strSql.Format(TEXT("  delete from t_ProgramBreakPoints where strProgramCode = '%s'"), orgDataIn.strPgmCode);
	vSqls.push_back(strSql);
	for (size_t i = 0; i < orgDataIn.vBreakPts.size(); ++i)
	{
		ST_BREAKPTDATA& bpRef = orgDataIn.vBreakPts[i];
		CString strTmp;
		strTmp.Format(TEXT("  insert into t_ProgramBreakPoints (strProgramCode, nPointLocation, nBreakID, strDescription, strChnID, nState, tPlayDate) values('%s', %d, %d, '%s', '%s', 1, '%s')"), 
			orgDataIn.strPgmCode, bpRef.nBreakPt, i, bpRef.strDesc, orgDataIn.strChID, Time2Str(orgDataIn.tmPlayTime));
		vSqls.push_back(strTmp);
	}

	CTxADOCommand cmd(dbKeyIn);
	if (cmd.ExecuteSQLs(vSqls))
	{

	}
	else
	{
		CFWriteLog("register breakpoints to dbkey1 failed!");
	}

	if (dbKeyIn2 != -1)
	{
		CTxADOCommand cmd(dbKeyIn2);
		if (cmd.ExecuteSQLs(vSqls))
		{

		}
		else
		{
			CFWriteLog("register breakpoints to dbkey2 failed!");
		}

	}
	
	return TRUE;

}


CString GetTaskColText( const int ColTypeIn,  ST_INPOOLTASKDATA& dataIn )
{
	CString strRet;

	if (ColTypeIn == mtaskcol_pgmcode)
	{
		return dataIn.strPgmCode;
	}
	else if (ColTypeIn == mtaskcol_clipname)
	{
		return dataIn.strClipName;
	}
	else if (ColTypeIn == mtaskcol_submittime)
	{
		return Time2Str(dataIn.tmSubmit);
	}
	else if (ColTypeIn == mtaskcol_sourcetype)
	{
		strRet.Format(TEXT("%d"), dataIn.nClipSrctype);
		return strRet;
	}
	else if (ColTypeIn == mtaskcol_playtime)
	{
		return Time2Str(dataIn.tmPlayTime);
	}else if (ColTypeIn == mtaskcol_chid)
	{
		return dataIn.strChID;
	}else if (ColTypeIn == mtaskcol_duration)
	{
		return TimeCodeToString(dataIn.nDuration);
	}else if (ColTypeIn == mtaskcol_cliptype)
	{
		strRet.Format(TEXT("%d"), dataIn.nClipType);
		return strRet;
	}else if (ColTypeIn == mtaskcol_npri)
	{
		strRet.Format(TEXT("%d"), dataIn.nPriority);
		return strRet;
	}else if (ColTypeIn == mtaskcol_embtaskid)
	{
		return dataIn.strTaskId;
	}else if (ColTypeIn == mtaskcol_version)
	{
		strRet.Format(TEXT("%d"), dataIn.nVersion);
		return strRet;
	}else if (ColTypeIn == mtaskcol_errcode)
	{
		strRet.Format(TEXT("%d"), dataIn.nErrcode);
		return strRet;
	}else if (ColTypeIn == mtaskcol_stgname)
	{
		return  dataIn.strStgName;
		
	}else if (ColTypeIn == mtaskcol_tplname)
	{
		return dataIn.strTplName;
	}
	else 
	{
		return strRet;
	}


}


