/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:06
	filename: 	EmbStructDef.cpp
	author:		tianyx
	
	purpose:	emb struct define
*********************************************************************/
#include "StdAfx.h"
#include "EmbStructDef.h"
#include "TxParamString.h"
#include "EMBCommonFunc.h"
#include "FGlobal.h"
#include "EMBDocDef.h"

BOOL ST_EXCUTORREG::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_EXCUTORREG_STRUCT);
	sParm.GoIntoKey(TEXT("excutorreg"));
	CTxStrConvert strVal;
	strVal.SetVal(guid);
	sParm.SetAttribVal(NULL, TEXT("guid"), strVal);
	strVal.SetVal(actorId);
	sParm.SetAttribVal(NULL, TEXT("actorId"), strVal);

	strVal.SetVal((UINT)hwndActor);
	sParm.SetAttribVal(NULL, TEXT("hwndActor"), strVal);
	strVal.SetVal((UINT)hwndExcutor);
	sParm.SetAttribVal(NULL, TEXT("hwndExcutor"), strVal);
	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_EXCUTORREG::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoIntoKey(TEXT("excutorreg"));
	guid = sParm.GetAttribVal(NULL, TEXT("guid")).GetAsInt(INVALID_ID);
	actorId = sParm.GetAttribVal(NULL, TEXT("actorId")).GetAsInt(INVALID_ID);

	hwndActor = (HWND)sParm.GetAttribVal(NULL, TEXT("hwndActor")).GetAsInt();
	hwndExcutor = (HWND)sParm.GetAttribVal(NULL, TEXT("hwndExcutor")).GetAsInt();
	return TRUE;
}

BOOL ST_EXCUTORREG::ToExcParamString( CString& strOut )
{
	CString strTmp;
	ToString(strTmp);
	for (int i = 0; i < strTmp.GetLength(); ++i)
	{
		TCHAR ch = strTmp.GetAt(i);
		if (ch == _T('\"'))
		{
			strOut += (ch);
		}
		strOut += (ch);
		
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
BOOL ST_EMBRET::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_EMBRET_STRUCT);
	sParm.GoToPath(TEXT(".\\ST_EMBRET"));
	CTxStrConvert strVal;
	strVal.SetVal(nRetVal);
	sParm.SetAttribVal(NULL, TEXT("Ret"), strVal);
	strVal.SetVal(strMsg);
	sParm.SetAttribVal(NULL, TEXT("Msg"), strVal);

	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_EMBRET::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoToPath(TEXT(".\\ST_EMBRET"));
	strMsg = sParm.GetAttribVal(NULL, TEXT("Msg")).GetAsString();
	nRetVal = sParm.GetAttribVal(NULL, TEXT("Ret")).GetAsInt(-1);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

BOOL ST_ACTORCONFIG::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_ACTORREG_STRUCT);
	sParm.GoIntoKey(EK_TASKACTORCONFIG);
	CTxStrConvert strVal;
	strVal.SetVal(actorId);
	sParm.SetAttribVal(NULL, TEXT("actorId"), strVal);
	strVal.SetVal(Addr2String(addrMain).c_str());
	sParm.SetAttribVal(NULL, TEXT("addrMain"), strVal);
	strVal.SetVal(Addr2String(addrSlave).c_str());
	sParm.SetAttribVal(NULL, TEXT("addrSlave"), strVal);
	strVal.SetVal(nExcutorMinId);
	sParm.SetAttribVal(NULL, TEXT("nExcutorMinId"), strVal);
	strVal.SetVal(nExcutorMaxId);
	sParm.SetAttribVal(NULL, TEXT("nExcutorMaxId"), strVal);
	strVal.SetVal(strExcPath);
	sParm.SetAttribVal(NULL, TEXT("strExcPath"), strVal);
	sParm.SetAttribVal(NULL, TEXT("nMaxDiskIO"), nMaxDiskIO);
	sParm.SetAttribVal(NULL, TEXT("nMaxNetIO"), nMaxNetIO);

	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_ACTORCONFIG::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoIntoKey(EK_MAIN);
	sParm.GoIntoKey(EK_TASKACTORCONFIG);
	actorId = sParm.GetAttribVal(NULL, TEXT("actorId")).GetAsInt(-1);
	CString strAddr = sParm.GetAttribVal(NULL, TEXT("addrMain")).GetAsString();
	addrMain = GetAddrFromStr(strAddr);
	strAddr = sParm.GetAttribVal(NULL, TEXT("addrSlave")).GetAsString();
	addrSlave =GetAddrFromStr(strAddr);
	nExcutorMinId = sParm.GetAttribVal(NULL, TEXT("nExcutorMinId")).GetAsInt(-1);
	nExcutorMaxId = sParm.GetAttribVal(NULL, TEXT("nExcutorMaxId")).GetAsInt(-1);
	nMaxDiskIO = sParm.GetAttribVal(NULL, TEXT("MaxDiskIOKB")).GetAsInt(40000);
	nMaxNetIO = sParm.GetAttribVal(NULL, TEXT("nMaxNetIO")).GetAsInt(60000);

	strExcPath = sParm.GetAttribVal(NULL, TEXT("strExcPath")).GetAsString();

	return TRUE;
}

// BOOL ST_EDOCMAINHEADER::ToString( CString& strOut )
// {
// 	strOut.Format(EDOC_TASKHEADERFMT, nVer, nType, strGuid);
// 	return TRUE;
// }
// 
// BOOL ST_EDOCMAINHEADER::FromString( const CString& strIn )
// {
// 	CTxParamString sParm(strIn);
// 	sParm.GoIntoKey(EK_MAIN);
// 	nVer = sParm.GetAttribVal(NULL, EA_MAIN_VER).GetAsInt(-1);
// 	nType = sParm.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt(-1);
// 	strGuid = sParm.GetAttribVal(NULL, EA_MAIN_GUID).GetAsString();
// 	return TRUE;
// }

BOOL ST_TASKBASIC::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_TASKBASIC);
	sParm.GoIntoKey(EK_TASKBASIC);
	CTxStrConvert strVal;
	strVal.SetVal(strGuid);
	sParm.SetAttribVal(NULL, EV_TBGUID, strVal);
	strVal.SetVal((INT64)tmSubmit);
	sParm.SetAttribVal(NULL, EV_TBSUBMIT, strVal);

	strVal.SetVal(nPriority);
	sParm.SetAttribVal(NULL, EV_TBPRIORITY, strVal);
	
	strVal.SetVal(nFixActor);
	sParm.SetAttribVal(NULL, EV_TBFIXACTOR, strVal);

	strVal.SetVal(nStartStep);
	sParm.SetAttribVal(NULL, EV_TBSTARTSTEP, strVal);

	strVal.SetVal(vSubTask);
	sParm.SetAttribVal(NULL, EV_TBSUBTASKLIST, strVal);

	strVal.SetVal(strTaskFrom);
	sParm.SetAttribVal(NULL, EV_TBTASKFROM, strVal);

	strVal.SetVal(nTaskSplit);
	sParm.SetAttribVal(NULL, EV_TBTASKSPLIT, strVal);

	strVal.SetVal(strTaskID);
	sParm.SetAttribVal(NULL, EV_TBTASKID, strVal);

	sParm.UpdateData();
	strOut = sParm;
	return TRUE;
}

BOOL ST_TASKBASIC::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoIntoKey(EK_TASKBASIC);
	strGuid = sParm.GetAttribVal(NULL, EV_TBGUID).GetAsString();
	nPriority =sParm.GetAttribVal(NULL, EV_TBPRIORITY).GetAsInt(-1);
	tmSubmit = (time_t)sParm.GetAttribVal(NULL, EV_TBSUBMIT).GetAsInt64();
	nFixActor = sParm.GetAttribVal(NULL, EV_TBFIXACTOR).GetAsInt(INVALID_ID);
	nStartStep = sParm.GetAttribVal(NULL, EV_TBSTARTSTEP).GetAsInt(0);
	sParm.GetAttribVal(NULL, EV_TBSUBTASKLIST).GetAsStringArray(vSubTask);
	strTaskFrom = sParm.GetAttribVal(NULL, EV_TBTASKFROM).GetAsString();
	nTaskSplit = sParm.GetAttribVal(NULL, EV_TBTASKSPLIT).GetAsInt(0);
	strTaskID = sParm.GetAttribVal(NULL, EV_TBTASKID).GetAsString();
	return TRUE;
}

BOOL ST_TASKRUNSTATE::ToString( CString& strOut , BOOL bWithinMainHeader/* = FALSE*/)
{
	CTxParamString sParm(EDOC_ST_TASKRUNSTATE_STRUCT);
	
	if (bWithinMainHeader)
	{
		CString strFmt;
		strFmt.Format(EDOC_MAINHEADERFMT, EMBVER, embxmltype_taskRunState, TEXT(""));
		sParm = CTxParamString(strFmt);
		sParm.GoIntoKey(EK_MAIN);
		sParm.SetElemVal(TEXT("ST_TASKRUNSTATE"), TEXT(""));
	}

	sParm.GoIntoKey(TEXT("ST_TASKRUNSTATE"));

	CTxStrConvert strVal;
	strVal.SetVal(Guid2String(guid));
	sParm.SetAttribVal(NULL, "guid", strVal);
	sParm.SetAttribVal(NULL, "actorid", actorId);
	sParm.SetAttribVal(NULL, "excId", excId);
	sParm.SetAttribVal(NULL, "nState", nState);
	sParm.SetAttribVal(NULL, "nExcType", nExcType);
	sParm.SetAttribVal(NULL, "nCurrStep", nCurrStep);
	sParm.SetAttribVal(NULL, "nPercent", nPercent);
	sParm.SetAttribVal(NULL, "nRetry", nRetry);
	sParm.SetAttribVal(NULL, "tmCommit", tmCommit);
	sParm.SetAttribVal(NULL, "tmExcute", tmExcute);
	sParm.SetAttribVal(NULL, "tmLastReport", tmLastReport);
	sParm.SetAttribVal(NULL, "tmLastCheck", tmLastCheck);

	sParm.UpdateData();
	strOut = sParm;

	return TRUE;
}

BOOL ST_TASKRUNSTATE::FromString( const CString& strIn, BOOL bWithinMainHeader/* = FALSE*/ )
{
	CTxParamString txParam(strIn);
	if (bWithinMainHeader)
	{
		txParam.GoIntoKey(EK_MAIN);
		int ntype = txParam.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt();
		if (ntype != embxmltype_taskRunState)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	txParam.GoIntoKey("ST_TASKRUNSTATE");

	guid = String2Guid(txParam.GetAttribVal(NULL, "guid").GetAsString());
	actorId = txParam.GetAttribVal(NULL, "actorid").GetAsInt();
	excId = txParam.GetAttribVal(NULL, "excId").GetAsInt();
	nState = txParam.GetAttribVal(NULL, "nState").GetAsInt();
	nCurrStep = txParam.GetAttribVal(NULL, "nCurrStep").GetAsInt();
	nExcType = txParam.GetAttribVal(NULL, "nExcType").GetAsInt();
	nPercent = txParam.GetAttribVal(NULL, "nPercent").GetAsInt();
	nRetry =txParam.GetAttribVal(NULL, "nRetry").GetAsInt();
	tmCommit = txParam.GetAttribVal(NULL, "tmCommit").GetAsInt64();
	tmExcute = txParam.GetAttribVal(NULL, "tmExcute").GetAsInt64();
	tmLastReport = txParam.GetAttribVal(NULL, "tmLastReport").GetAsInt64();
	tmLastCheck = txParam.GetAttribVal(NULL, "tmLastCheck").GetAsInt64();

	return TRUE;
}

CString ST_TASKRUNSTATE::StateDes()
{
	CString strDes;
	switch (nState)
	{
	case embtaskstate_dispatching:
		strDes = "等待";
		break;

	case embtaskstate_dispatched:
		strDes = "执行";
		break;

	case embtaskstate_finished:
		strDes = "成功";
		break;

	case embtaskstate_error:
		strDes = "失败";
		break;
	}

	return strDes;
}

BOOL ST_TASKRISERCONFIG::ToString( CString& strOut )
{
	CTxParamString txParam("");
	CTxStrConvert txVal;
	txParam.SetElemVal(EK_TASKRISERCONFIG, txVal);
	txParam.GoIntoKey(EK_TASKRISERCONFIG);
	VECSTRINGS vList;
	for (size_t i =0; i < vProbes.size(); ++i)
	{
		CString& strtmpKey = vProbes[i].strProberName;
		vList.push_back(strtmpKey);
		CTxStrConvert tmpVal;
		txParam.SetElemVal(strtmpKey, tmpVal);
		tmpVal.SetVal(vProbes[i].nType);
		txParam.SetAttribVal(strtmpKey, TEXT("type"), tmpVal);
		if (vProbes[i].nType == embTaskproberType_tcp)
		{
			tmpVal.SetVal(Addr2String(vProbes[i].data.ipdata.addrListen).c_str());
			txParam.SetAttribVal(strtmpKey, TEXT("listenIp"), tmpVal);

		}

	}
	txVal.SetVal(vList);
	txParam.SetAttribVal(NULL, TEXT("proberList"), txVal);

	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_TASKRISERCONFIG::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_TASKRISERCONFIG);
	VECSTRINGS vList;
	txParam.GetAttribVal(NULL, TEXT("proberList")).GetAsStringArray(vList);
	if (vList.size() == 0)
	{
		ASSERT(FALSE);
		return S_FALSE;
	}
	for (size_t i = 0; i< vList.size(); ++i)
	{
		int nType = txParam.GetAttribVal(vList[i], TEXT("type")).GetAsInt();
		if (nType == embTaskproberType_tcp)
		{
			//
			CString strIP = txParam.GetAttribVal(vList[i], TEXT("listenIp")).GetAsString();
			CString strIPLocal = txParam.GetAttribVal(vList[i], TEXT("localIp")).GetAsString();
			if (!strIP.IsEmpty())
			{
				SOCKADDR_IN addr = GetAddrFromStr(strIP);
				if(addr.sin_family == AF_INET)
				{
					ST_PROBERDATA tmpdata;
					tmpdata.nType = nType;
					tmpdata.strProberName = vList[i];
					tmpdata.data.ipdata.addrListen = addr;
					vProbes.push_back(tmpdata);
				}
			}
			
		}
	}

	return TRUE;

}

//////////////////////////////////////////////////////////////////////////
BOOL ST_TASKSTORAGECONFIG::ToString( CString& strOut )
{
	CTxParamString txParam("");
	CTxStrConvert txVal;
	txParam.SetElemVal(EK_TASKSTORAGECONFIG, txVal);
	txParam.GoIntoKey(EK_TASKSTORAGECONFIG);
	txVal.SetVal(nType);
	txParam.SetAttribVal(NULL, TEXT("type"), txVal);
	txVal.SetVal(strDBConnect);
	txParam.SetAttribVal(NULL, TEXT("DBConnect"), txVal);

	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_TASKSTORAGECONFIG::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_TASKSTORAGECONFIG);
	nType = txParam.GetAttribVal(NULL, TEXT("type")).GetAsInt(INVALID_ID);
	strDBConnect = txParam.GetAttribVal(NULL, TEXT("DBConnect")).GetAsString();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
BOOL ST_TASKDISPATCHCONFIG::ToString( CString& strOut )
{
	CTxParamString txParam("");
	CTxStrConvert txVal;
	txParam.SetElemVal(EK_TASKDISPATCHCONFIG, txVal);
	txParam.GoIntoKey(EK_TASKDISPATCHCONFIG);
	txVal.SetVal(nSvrID);
	txParam.SetAttribVal(NULL, TEXT("nSvrID"), txVal);
	txVal.SetVal(nMaster);
	txParam.SetAttribVal(NULL, TEXT("nMaster"), txVal);
	
	txVal.SetVal(strIpActorHolder);
	txParam.SetAttribVal(NULL, TEXT("IpActorHolder"), txVal);
	txVal.SetVal(strIpMaster);
	txParam.SetAttribVal(NULL, TEXT("IpMaster"), txVal);


	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_TASKDISPATCHCONFIG::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_TASKDISPATCHCONFIG);
	nSvrID = txParam.GetAttribVal(NULL, TEXT("nSvrID")).GetAsInt(INVALID_ID);
	nMaster = txParam.GetAttribVal(NULL, TEXT("nMaster")).GetAsInt(INVALID_ID);
	strIpActorHolder = txParam.GetAttribVal(NULL, TEXT("IpActorHolder")).GetAsString();
	strIpMaster = txParam.GetAttribVal(NULL, TEXT("IpMaster")).GetAsString();

	//////////////////////////////////////////////////////////////////////////
	nfgTaskPoolSizeMax = txParam.GetAttribVal(NULL, TEXT("nfgTaskPoolSizeMax")).GetAsInt(-1);
	//
	nfgTaskReDispatchMaxCount = txParam.GetAttribVal(NULL, TEXT("nfgTaskReDispatchMaxCount")).GetAsInt(-1);;
	nfgTaskDispatchCD = txParam.GetAttribVal(NULL, TEXT("nfgTaskDispatchCD")).GetAsInt(-1);;
	nfgTaskReportIntervalMax = txParam.GetAttribVal(NULL, TEXT("nfgTaskReportIntervalMax")).GetAsInt(-1);;
	nfgTaskCheckProgressIntervalMax = txParam.GetAttribVal(NULL, TEXT("nfgTaskCheckProgressIntervalMax")).GetAsInt(-1);;
	nfgTaskLostTimeOutMax = txParam.GetAttribVal(NULL, TEXT("nfgTaskLostTimeOutMax")).GetAsInt(-1);;

	nfgActorLostTimeOutMax = txParam.GetAttribVal(NULL, TEXT("nfgActorLostTimeOutMax")).GetAsInt(-1);;
	nfgActorCheckInterval = txParam.GetAttribVal(NULL, TEXT("nfgActorCheckInterval")).GetAsInt(-1);;
	nfgActorStateOutdate = txParam.GetAttribVal(NULL, TEXT("nfgActorStateOutdate")).GetAsInt(-1);;
	nfgActorAssignTaskCD = txParam.GetAttribVal(NULL, TEXT("nfgActorAssignTaskCD")).GetAsInt(-1);;
	//
	nfgCpuWeight = txParam.GetAttribVal(NULL, TEXT("nfgCpuWeight")).GetAsInt(-1);;
	nfgMemWeight = txParam.GetAttribVal(NULL, TEXT("nfgMemWeight")).GetAsInt(-1);;
	nfgDiskIOWeight = txParam.GetAttribVal(NULL, TEXT("nfgDiskIOWeight")).GetAsInt(-1);;
	nfgNetIOWeight = txParam.GetAttribVal(NULL, TEXT("nfgNetIOWeight")).GetAsInt(-1);;
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

BOOL ST_TASKUPDATE::ToString( CString& strOut )
{
	CString strParam;
	strParam.Format(EDOC_TASKHEADERFMT, embxmltype_taskupdate, TEXT(""));
	CTxParamString txRootParam(strParam);
	txRootParam.GoIntoKey(EK_MAIN);
	CTxStrConvert val;
	val.SetVal(embxmltype_taskupdate); // update
	txRootParam.SetAttribVal(NULL, EA_MAIN_TYPE, val);

	
	CTxParamString txParam;
	txParam.SetElemVal(EK_TASKUPDATE, val);
	txParam.GoIntoKey(EK_TASKUPDATE);
	val.SetVal(Guid2String(guid));
	txParam.SetAttribVal(NULL, TEXT("guid"), val);
	val.SetVal(nUpdateType);
	txParam.SetAttribVal(NULL, TEXT("nUpdateType"), val);
	val.SetVal(strTaskID);
	txParam.SetAttribVal(NULL, TEXT("taskID"), val);

	if (nUpdateType == embtaskupdatetype_finish)
	{
		val.SetVal("");
		txParam.SetElemVal(EK_TASKUPDATE_END, val);
		txParam.GoIntoKey(EK_TASKUPDATE_END);
		val.SetVal(data_end.nEndState);
		txParam.SetAttribVal(NULL, TEXT("nEndState"), val);
		val.SetVal(data_end.dispatchid);
		txParam.SetAttribVal(NULL, TEXT("dispatchid"), val);
		val.SetVal(data_end.actorid);
		txParam.SetAttribVal(NULL, TEXT("actorid"), val);
		val.SetVal(data_end.excutorid);
		txParam.SetAttribVal(NULL, TEXT("excutorid"), val);
	}
	else if (nUpdateType == embtaskupdatetype_del)
	{
		//to be add...
		ASSERT(FALSE);
	}

	txRootParam.SetSubNodeString(".\\edoc_main", txParam);
	txRootParam.UpdateData();
	strOut = txRootParam;

	return TRUE;
}

BOOL ST_TASKUPDATE::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	int nType = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
	if (nType != embxmltype_taskupdate)
	{
		ASSERT(FALSE);
		nUpdateType = embtaskupdatetype_none;
		return FALSE;
	}
	txParam.GoIntoKey(EK_MAIN);
	nUpdateType = txParam.GetAttribVal(EK_TASKUPDATE, TEXT("nUpdateType")).GetAsInt(embtaskupdatetype_none);
	strTaskID = txParam.GetAttribVal(EK_TASKUPDATE, TEXT("taskID")).GetAsString();
	txParam.GoIntoKey(EK_TASKUPDATE);
	if (nUpdateType == embtaskupdatetype_finish)
	{
		//to be add...
		CString strGuid = txParam.GetAttribVal(NULL, TEXT("guid")).GetAsString("");
		guid = String2Guid(strGuid);
		txParam.GoIntoKey(EK_TASKUPDATE_END);
		data_end.nEndState = txParam.GetAttribVal(NULL, TEXT("nEndState")).GetAsInt(embtaskstate_none);
		data_end.actorid = txParam.GetAttribVal(NULL, TEXT("actorid")).GetAsInt(INVALID_ID);
		data_end.excutorid = txParam.GetAttribVal(NULL, TEXT("excutorid")).GetAsInt(INVALID_ID);
		data_end.dispatchid = txParam.GetAttribVal(NULL, TEXT("dispatchid")).GetAsInt(INVALID_ID);

	}
	else if (nUpdateType == embtaskupdatetype_del)
	{
		//to be add...
	}

	return TRUE;
}

BOOL ST_TASKREPORT::ToString( CString& strOut )
{
	CString strParam;
	strParam.Format(EDOC_TASKHEADERFMT, embxmltype_taskReport, TEXT(""));
	CTxParamString txParam(strParam);
	txParam.GoIntoKey(EK_MAIN);
	CTxStrConvert val;
	txParam.SetElemVal(EK_TASKREPORT, val);
	txParam.GoIntoKey(EK_TASKREPORT);
	val.SetVal(strGuid);
	txParam.SetAttribVal(NULL, TEXT("strGuid"), val);
	val.SetVal(actorId);
	txParam.SetAttribVal(NULL, TEXT("actorId"), val);
	val.SetVal(excutorId);
	txParam.SetAttribVal(NULL, TEXT("excutorId"), val);
	val.SetVal(nState);
	txParam.SetAttribVal(NULL, TEXT("nState"), val);
	val.SetVal(nPercent);
	txParam.SetAttribVal(NULL, TEXT("nPercent"), val);
	val.SetVal(nStep);
	txParam.SetAttribVal(NULL, TEXT("nStep"), val);
	val.SetVal(nSubErrorCode);
	txParam.SetAttribVal(NULL, TEXT("nSubErrorCode"), val);
	val.SetValX(nSubErrorCode);
	txParam.SetAttribVal(NULL, TEXT("nSubErrorCodeX"), val); // 十六进制错误码

	txParam.UpdateData();
	strOut = txParam;

	return TRUE;
}

BOOL ST_TASKREPORT::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	int nType = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
	if (nType != embxmltype_taskReport)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	txParam.GoIntoKey(EK_MAIN);
	txParam.GoIntoKey(EK_TASKREPORT);
	strGuid = txParam.GetAttribVal(NULL, TEXT("strGuid")).GetAsString();
	actorId = txParam.GetAttribVal(NULL, TEXT("actorId")).GetAsInt(INVALID_ID);
	excutorId = txParam.GetAttribVal(NULL, TEXT("excutorId")).GetAsInt(INVALID_ID);
	nState = txParam.GetAttribVal(NULL, TEXT("nState")).GetAsInt(embtaskstate_none);
	nPercent = txParam.GetAttribVal(NULL, TEXT("nPercent")).GetAsInt(0);
	nStep = txParam.GetAttribVal(NULL, TEXT("nStep")).GetAsInt(-1);
	nSubErrorCode = txParam.GetAttribVal(NULL, TEXT("nSubErrorCode")).GetAsInt(0);

	return TRUE;
}


BOOL ST_ACTORSTATE::ToString( CString& strOut )
{
	CString strParam;
	strParam.Format(EDOC_TASKHEADERFMT, embxmltype_actorState, TEXT(""));
	CTxParamString txParam(strParam);
	txParam.GoIntoKey(EK_MAIN);
	CTxStrConvert val;
	txParam.SetElemVal(EK_ACTORSTATE, val);
	txParam.GoIntoKey(EK_ACTORSTATE);
	val.SetVal(actorId);
	txParam.SetAttribVal(NULL, TEXT("actorId"), val);
	val.SetVal(nActorLevel);
	txParam.SetAttribVal(NULL, TEXT("nActorLevel"), val);
	val.SetVal(nCpuUsage);
	txParam.SetAttribVal(NULL, TEXT("nCpuUsage"), val);
	val.SetVal(nMemUsage);
	txParam.SetAttribVal(NULL, TEXT("nMemUsage"), val);
	val.SetVal(nDiscIOUsage);
	txParam.SetAttribVal(NULL, TEXT("nDiscIOUsage"), val);
	val.SetVal(strTeam);
	txParam.SetAttribVal(NULL, TEXT("actorTeam"), val);
	txParam.SetAttribVal(NULL, TEXT("excResUsage"), nExcResUsage);
	txParam.SetAttribVal(NULL, TEXT("nNetIOUsage"), nNetIOUsage);

	val.SetVal(strPcName);
	txParam.SetAttribVal(NULL, TEXT("PcName"), strPcName);

	txParam.UpdateData();
	strOut = txParam;

	return TRUE;
}

BOOL ST_ACTORSTATE::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	int nType = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
	if (nType != embxmltype_actorState)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	txParam.GoIntoKey(EK_MAIN);
	txParam.GoIntoKey(EK_ACTORSTATE);
	actorId = txParam.GetAttribVal(NULL, TEXT("actorId")).GetAsInt(INVALID_ID);
	nActorLevel = txParam.GetAttribVal(NULL, TEXT("nActorLevel")).GetAsInt(INVALID_ID);
	nCpuUsage = txParam.GetAttribVal(NULL, TEXT("nCpuUsage")).GetAsInt(INVALID_ID);
	nMemUsage = txParam.GetAttribVal(NULL, TEXT("nMemUsage")).GetAsInt(INVALID_ID);
	nDiscIOUsage= txParam.GetAttribVal(NULL, TEXT("nDiscIOUsage")).GetAsInt(INVALID_ID);
	nExcResUsage = txParam.GetAttribVal(NULL, TEXT("excResUsage")).GetAsInt(INVALID_ID);
	nNetIOUsage = txParam.GetAttribVal(NULL, TEXT("nNetIOUsage")).GetAsInt(INVALID_ID);

	strTeam= txParam.GetAttribVal(NULL, TEXT("actorTeam")).GetAsString("");
	strPcName = txParam.GetAttribVal(NULL, TEXT("PcName")).GetAsString("");
	//strHost= txParam.GetAttribVal(NULL, TEXT("strHost")).GetAsString();
// 	VECSTRINGS vStr;
// 	txParam.GetAttribVal(NULL, TEXT("strHost")).GetAsStringArray(vStr);
// 	if(vStr.size()%2 == 0)
// 	{
// 		for (size_t i = 0; i < vStr.size(); i+=2)
// 		{
// 			if (vStr[i].GetLength() == 1)
// 			{
// 				CTxStrConvert val;
// 				val.SetVal(vStr[i+1]);
// 				mapDiskUse[vStr[i]]= val.GetAsInt();
// 			}
// 			
// 		}
// 	}

	return TRUE;
	

}

BOOL ST_EMBXMLMAININFO::ToString( CString& strOut )
{
	if (nErrcode != S_OK)
	{
		strOut.Format(EDOC_MAINHEADERFMT2, ver, nType, guid, nErrcode);
	}
	else
	{
		strOut.Format(EDOC_MAINHEADERFMT, ver, nType, guid);
	}

	return TRUE;
}

BOOL ST_EMBXMLMAININFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	nType = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
	ASSERT(nType != -1);
	txParam.GoIntoKey(EK_MAIN);
	ver = txParam.GetAttribVal(NULL, TEXT("ver")).GetAsInt(INVALID_ID);
	guid = txParam.GetAttribVal(NULL, TEXT("guid")).GetAsString();
	nErrcode = txParam.GetAttribVal(NULL, TEXT("errcode")).GetAsInt(S_OK);
	return TRUE;
}

BOOL ST_SVRACTIVEINFO::ToString( CString& strOut )
{
	strOut.Format(EDOC_SVRACTIVEFMT, embxmltype_svrActive, nMaster, nActive);
	return TRUE;
}

BOOL ST_SVRACTIVEINFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	int type = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
	if (type != embxmltype_svrActive)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	nActive = txParam.GetAttribVal(EK_MAIN, TEXT("active")).GetAsInt(embxmltype_none);
	nMaster = txParam.GetAttribVal(EK_MAIN, TEXT("master")).GetAsInt(embxmltype_none);

	return TRUE;
}

BOOL ST_MD5TASKINFO::ToString( CString& strOut )
{	
	strOut.Format(EDOC_WORKMD5FMT, strFileToCheck, strFileToWriteResult, strMD5Compare);
	return TRUE;
}

BOOL ST_MD5TASKINFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	strFileToCheck = txParam.GetAttribVal(EK_WORKMD5, EA_MD5DES).GetAsString();
	strFileToWriteResult = txParam.GetAttribVal(EK_WORKMD5, EA_MD5WRITETO).GetAsString();
	strMD5Compare = txParam.GetAttribVal(EK_WORKMD5, EA_MD5COMP).GetAsString();
	return TRUE;
}

BOOL ST_WORKERREPORT::ToString( CString& strOut )
{
	strOut.Format(EDOC_WORKREPORTFMT, nPercent, code);
	return TRUE;

}

BOOL ST_WORKERREPORT::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	nPercent = txParam.GetAttribVal(EK_WORKREPORT, EA_REPORTPERCENT).GetAsInt(0);
	code = txParam.GetAttribVal(EK_WORKREPORT, EA_REPORTCODE).GetAsInt(0);
	return TRUE;

}
///////////////////////////////////////////////
BOOL ST_FCVSTASKINFO::FromString(const CString& strIn)
{
	CTxParamString txParam(strIn);
// 	int type = txParam.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt(-1);
// 	if (type != embxmltype_svrActive)
// 	{
// 		ASSERT(FALSE);
// 		return FALSE;
// 	}
//	txParam.GoIntoKey(EK_MAIN);
	txParam.GoIntoKey(EK_FCVSHeader);
	txParam.GoIntoKey(EK_FCVSTASK);

		
	clipType = txParam.GetAttribVal(NULL,TEXT("ClipType")).GetAsString();
	fileMediaType = txParam.GetAttribVal(NULL,TEXT("FileMediaType")).GetAsString();
	checkItem = txParam.GetAttribVal(NULL,TEXT("CheckItem")).GetAsInt(0x00111111);
// 	FCVSTaskCut = txParam.GetAttribVal(NULL,TEXT("FCVSTaskCut")).GetAsBOOL(FALSE);
// 	if (FCVSTaskCut)
	//{
		nSectionID = txParam.GetAttribVal(NULL,TEXT("nSectionID")).GetAsInt(0);
		nTotalSectionCount = txParam.GetAttribVal(NULL,TEXT("nTotalSectionCount")).GetAsInt(1);
	//}
	usedDetectLevel = txParam.GetAttribVal(NULL,TEXT("UsedDetectLevel")).GetAsString();
	fileAdrType = txParam.GetAttribVal(NULL,TEXT("FileAdrType")).GetAsString();
	filePath = txParam.GetAttribVal(NULL,TEXT("FilePath")).GetAsString();
	fileName = txParam.GetAttribVal(NULL,TEXT("FileName")).GetAsString();
	strSaveXmlPath = txParam.GetAttribVal(NULL, TEXT("SaveXmlPath")).GetAsString();

	if (fileAdrType == TEXT("FTP"))
	{
		fileAdrIP = txParam.GetAttribVal(NULL,TEXT("FileAdrIP")).GetAsString();
		fileAdrUser = txParam.GetAttribVal(NULL,TEXT("FileAdrUser")).GetAsString();
		fileAdrPwd = txParam.GetAttribVal(NULL,TEXT("FileAdrPwd")).GetAsString();
		fileAdrPort = txParam.GetAttribVal(NULL,TEXT("FileAdrPort")).GetAsString();
	}
	if ((fileName == "")||(filePath == "")||(fileMediaType==""))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL ST_FCVSTASKINFO::ToString(CString& strOut)
{
	CTxParamString txParam("");
	CTxStrConvert val;
	txParam.SetElemVal(EK_FCVSTASK, val);
	txParam.GoIntoKey(EK_FCVSTASK);
// 	val.SetVal(actorId);
// 	txParam.SetAttribVal(NULL, TEXT("actorId"), val);
	val.SetVal(clipType);
	txParam.SetAttribVal(NULL, TEXT("ClipType"),val);
	val.SetVal(fileMediaType);
	txParam.SetAttribVal(NULL, TEXT("FileMediaType"),val);
	val.SetVal(checkItem);
	txParam.SetAttribVal(NULL,TEXT("CheckItem"),val);
// 	val.SetVal(FCVSTaskCut);
// 	txParam.SetAttribVal(NULL,TEXT("FCVSTaskCut"),val);
	val.SetVal(nSectionID);
	txParam.SetAttribVal(NULL,TEXT("nSectionID"),val);
	val.SetVal(nTotalSectionCount);
	txParam.SetAttribVal(NULL,TEXT("nTotalSectionCount"),val);
	val.SetVal(usedDetectLevel);
	txParam.SetAttribVal(NULL,TEXT("UsedDetectLevel"),val);
	val.SetVal(fileAdrType);
	txParam.SetAttribVal(NULL,TEXT("FileAdrType"),val);
	val.SetVal(filePath);
	txParam.SetAttribVal(NULL,TEXT("FilePath"),val);
	val.SetVal(fileName);
	txParam.SetAttribVal(NULL,TEXT("FileName"),val);
	val.SetVal(fileAdrIP);
	txParam.SetAttribVal(NULL,TEXT("FileAdrIP"),val);
	val.SetVal(fileAdrUser);
	txParam.SetAttribVal(NULL,TEXT("FileAdrUser"),val);
	val.SetVal(fileAdrPwd);
	txParam.SetAttribVal(NULL,TEXT("FileAdrPwd"),val);
	val.SetVal(fileAdrPort);
	txParam.SetAttribVal(NULL,TEXT("FileAdrPort"),val);

	//
	val.SetVal(strSaveXmlPath);
	txParam.SetAttribVal(NULL, TEXT("SaveXmlPath"), val);

	txParam.UpdateData();
	strOut = txParam;
	return TRUE;	
}
BOOL ST_FCVSCONFIGINFO::FromString(const CString& strIn)
{
	int i;
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_FCVSHeader);
	txParam.GoIntoKey(EK_FCVSTASKCONFIG);
	serverName = txParam.GetAttribVal(NULL,TEXT("ServerName")).GetAsString(TEXT("技审服务"));
//	dataBaseInfo = txParam.GetAttribVal(NULL,TEXT("DataBaseInfo")).GetAsString();
	bSaveXML = txParam.GetAttribVal(NULL,TEXT("BSaveXML")).GetAsBOOL(FALSE);
//	bSaveDB = txParam.GetAttribVal(NULL,TEXT("BSaveXML")).GetAsBOOL(TRUE);
	bCheckAudio = txParam.GetAttribVal(NULL,TEXT("BCheckAudio")).GetAsBOOL(TRUE);
	bUseSection = txParam.GetAttribVal(NULL,TEXT("BUseSection")).GetAsBOOL(FALSE);
//	serviceCount = txParam.GetAttribVal(NULL,TEXT("ServiceCount")).GetAsInt(0);
	fileDriverCount = txParam.GetAttribVal(NULL,TEXT("FileDriverCount")).GetAsInt(0);
	detectLevelCount = txParam.GetAttribVal(NULL,TEXT("DetectLevelCount")).GetAsInt(0);
	detectThresholodCount =txParam.GetAttribVal(NULL,TEXT("DetectThresholodCount")).GetAsInt(0);
// 	if (dataBaseInfo == "" )
// 	{
// 		return FALSE;
// 	}
// 	if (bSaveDB&&bSaveXML)
// 	{
// 		return FALSE;
//	}
	if (fileDriverCount ==0)
	{
		return FALSE;
	}
// 	for ( i = 0 ;i<serviceCount;i++)
// 	{
// 		CString tempService ;
// 		tempService.Format("Service%d",i);
// 		txParam.GoIntoKey(tempService);
// 		serVice[i].Name = txParam.GetAttribVal(NULL,TEXT("Name")).GetAsString();
// 		serVice[i].IP = txParam.GetAttribVal(NULL,TEXT("IP")).GetAsString();
// 		serVice[i].Port = txParam.GetAttribVal(NULL,TEXT("Port")).GetAsInt(2000);
// 		serVice[i].TYPE = txParam.GetAttribVal(NULL,TEXT("TYPE")).GetAsString();
// 		serVice[i].Protocol = txParam.GetAttribVal(NULL,TEXT("Protocol")).GetAsString();
// 		serVice[i].isServer = txParam.GetAttribVal(NULL,TEXT("isServer")).GetAsBOOL(TRUE);
// 		txParam.OutofKey();
// 	}
	for ( i = 0;i<fileDriverCount;i++)
	{
		CString temp ;
		temp.Format("FileDriver%d",i);
		txParam.GoIntoKey(temp);
		fileDriver[i].name = txParam.GetAttribVal(NULL,TEXT("Name")).GetAsString();
		fileDriver[i].classString = txParam.GetAttribVal(NULL,TEXT("ClassString")).GetAsString();
		fileDriver[i].driverName = txParam.GetAttribVal(NULL,TEXT("DriverName")).GetAsString();
		fileDriver[i].fileType = txParam.GetAttribVal(NULL,TEXT("FileType")).GetAsString();
		fileDriver[i].isDefault = txParam.GetAttribVal(NULL,TEXT("IsDefault")).GetAsBOOL(FALSE);
		fileDriver[i].DetectedCLIP_HEIGHT = txParam.GetAttribVal(NULL,TEXT("DetectedCLIP_HEIGHT")).GetAsInt(0);
		fileDriver[i].DetectedCLIP_WIDTH = txParam.GetAttribVal(NULL,TEXT("DetectedCLIP_WIDTH")).GetAsInt(0);
		txParam.OutofKey();
	}
	for ( i =0;i<detectLevelCount;i++)
	{
		CString temp ;
		temp.Format("DetectLevel%d",i);
		txParam.GoIntoKey(temp);
		detectLevel[i].levelID = txParam.GetAttribVal(NULL,TEXT("LevelID")).GetAsInt(0);
		detectLevel[i].levelName =txParam.GetAttribVal(NULL,TEXT("LevelName")).GetAsString(0);
		detectLevel[i].blackFrame = txParam.GetAttribVal(NULL,TEXT("BlackFrame")).GetAsInt(0);
		detectLevel[i].colorFrame = txParam.GetAttribVal(NULL,TEXT("ColorFrame")).GetAsInt(0);
		detectLevel[i].colorSripe = txParam.GetAttribVal(NULL,TEXT("ColorSripe")).GetAsInt(0);
		detectLevel[i].staticFrame = txParam.GetAttribVal(NULL,TEXT("StaticFrame")).GetAsInt(0);
		detectLevel[i].muteDur = txParam.GetAttribVal(NULL,TEXT("MuteDur")).GetAsInt(0);
		detectLevel[i].HighDur = txParam.GetAttribVal(NULL,TEXT("HighDur")).GetAsInt(0);
		detectLevel[i].LowDur = txParam.GetAttribVal(NULL,TEXT("LowDur")).GetAsInt(0);
		txParam.OutofKey();
	}
	if (detectThresholodCount !=0)
	{
		for ( i = 0 ;i<detectThresholodCount;i++)
		{
			CString temp ;
			temp.Format("DetectThresholodCount%d",i);
			txParam.GoIntoKey(temp);
			detectThresholod[i].name = txParam.GetAttribVal(NULL,TEXT("Name")).GetAsString();
			detectThresholod[i].TOP_SAFE_SECTION = txParam.GetAttribVal(NULL,TEXT("TOP_SAFE_SECTION")).GetAsDouble();
			detectThresholod[i].BOTTOM_SAFE_SECTION = txParam.GetAttribVal(NULL,TEXT("BOTTOM_SAFE_SECTION")).GetAsDouble();
			detectThresholod[i].LEFT_SAFE_SECTION = txParam.GetAttribVal(NULL,TEXT("LEFT_SAFE_SECTION")).GetAsDouble();
			detectThresholod[i].RIGHT_SAFE_SECTION = txParam.GetAttribVal(NULL,TEXT("RIGHT_SAFE_SECTION")).GetAsDouble();
			detectThresholod[i].BLACK_SCENE_THRESHOLD = txParam.GetAttribVal(NULL,TEXT("BLACK_SCENE_THRESHOLD")).GetAsDouble();
			detectThresholod[i].COLOR_SCENE_PERCENTAGE = txParam.GetAttribVal(NULL,TEXT("COLOR_SCENE_PERCENTAGE")).GetAsDouble();
			detectThresholod[i].COLOR_STRIP_PERCENTAGE =txParam.GetAttribVal(NULL,TEXT("COLOR_STRIP_PERCENTAGE")).GetAsDouble();
			detectThresholod[i].STATIC_FRAME_PERCENTAGE = txParam.GetAttribVal(NULL,TEXT("STATIC_FRAME_PERCENTAGE")).GetAsDouble();
			detectThresholod[i].LINE_COUNTS_FOR_STRIP_DETECT =txParam.GetAttribVal(NULL,TEXT("LINE_COUNTS_FOR_STRIP_DETECT")).GetAsDouble();
			detectThresholod[i].AMBIT_DEEMED_TO_SAME_PIXEL =txParam.GetAttribVal(NULL,TEXT("AMBIT_DEEMED_TO_SAME_PIXEL")).GetAsDouble();
			detectThresholod[i].UNDULATE_AMBIT_OF_Y = txParam.GetAttribVal(NULL,TEXT("UNDULATE_AMBIT_OF_Y")).GetAsDouble();
			detectThresholod[i].UNDULATE_AMBIT_OF_U = txParam.GetAttribVal(NULL,TEXT("UNDULATE_AMBIT_OF_U")).GetAsDouble();
			detectThresholod[i].UNDULATE_AMBIT_OF_V = txParam.GetAttribVal(NULL,TEXT("UNDULATE_AMBIT_OF_V")).GetAsDouble();
			detectThresholod[i].AUDIO_Low_THRESHOLD = txParam.GetAttribVal(NULL,TEXT("AUDIO_Low_THRESHOLD")).GetAsDouble();
			detectThresholod[i].AUDIO_Low_PERIOD = txParam.GetAttribVal(NULL,TEXT("AUDIO_Low_PERIOD")).GetAsDouble();
			detectThresholod[i].AUDIO_High_THRESHOLD = txParam.GetAttribVal(NULL,TEXT("AUDIO_High_THRESHOLD")).GetAsDouble();
			detectThresholod[i].AUDIO_High_PERIOD =txParam.GetAttribVal(NULL,TEXT("AUDIO_High_PERIOD")).GetAsDouble();
			detectThresholod[i].AUDIO_Mute_THRESHOLD = txParam.GetAttribVal(NULL,TEXT("AUDIO_Mute_THRESHOLD")).GetAsDouble();
			detectThresholod[i].AUDIO_Mute_PERIOD = txParam.GetAttribVal(NULL,TEXT("AUDIO_Mute_PERIOD")).GetAsDouble();
			txParam.OutofKey();
		}		
	}

	return TRUE;
}
BOOL ST_FCVSCONFIGINFO::ToString(CString& strOut)
{
	CTxParamString txParam("");
	CTxStrConvert val;
	int i;
	txParam.SetElemVal(EK_FCVSTASKCONFIG, val);
	txParam.GoIntoKey(EK_FCVSTASKCONFIG);
	val.SetVal(serverName);
	txParam.SetAttribVal(NULL,TEXT("ServerName"),val);
// 	val.SetVal(dataBaseInfo);
// 	txParam.SetAttribVal(NULL,TEXT("DataBaseInfo"),val);
	val.SetVal(bSaveXML);
	txParam.SetAttribVal(NULL,TEXT("BSaveXML"),val);
// 	val.SetVal(bSaveDB);
// 	txParam.SetAttribVal(NULL,TEXT("BSaveDB"),val);
	val.SetVal(bCheckAudio);
	txParam.SetAttribVal(NULL,TEXT("BCheckAudio"),val);
	val.SetVal(bUseSection);
	txParam.SetAttribVal(NULL,TEXT("BUseSection"),val);
// 	val.SetVal(serviceCount);
// 	txParam.SetAttribVal(NULL,TEXT("ServiceCount"),val);
	val.SetVal(fileDriverCount);
	txParam.SetAttribVal(NULL,TEXT("FileDriverCount"),val);
	val.SetVal(detectLevelCount);
	txParam.SetAttribVal(NULL,TEXT("DetectLevelCount"),val);
	val.SetVal(detectThresholodCount);
	txParam.SetAttribVal(NULL,TEXT("DetectThresholodCount"),val);
// 	for (i = 0 ;i<serviceCount;i++)
// 	{
// 		CString tempService ;
// 		CTxStrConvert valService;
// 		tempService.Format("Service%d",i);
// 		txParam.SetElemVal(tempService, valService);
// 		txParam.GoIntoKey(tempService);
// 		valService.SetVal(serVice[i].Name);
// 		txParam.SetAttribVal(NULL,TEXT("Name"),valService);
// 		valService.SetVal(serVice[i].IP);
// 		txParam.SetAttribVal(NULL,TEXT("IP"),valService);
// 		valService.SetVal(serVice[i].Port);
// 		txParam.SetAttribVal(NULL,TEXT("Port"),valService);
// 		valService.SetVal(serVice[i].TYPE);
// 		txParam.SetAttribVal(NULL,TEXT("TYPE"),valService);
// 		valService.SetVal(serVice[i].Protocol);
// 		txParam.SetAttribVal(NULL,TEXT("Protocol"),valService);
// 		txParam.OutofKey();
// 	}
	for ( i = 0;i<fileDriverCount;i++)
	{
		CString tempDriver ;
		CTxStrConvert valDriver;
		tempDriver.Format("FileDriver%d",i);
		txParam.SetElemVal(tempDriver, valDriver);
		txParam.GoIntoKey(tempDriver);
		valDriver.SetVal(fileDriver[i].name);
		txParam.SetAttribVal(NULL,TEXT("Name"),valDriver);
		valDriver.SetVal(fileDriver[i].classString);
		txParam.SetAttribVal(NULL,TEXT("ClassString"),valDriver);
		valDriver.SetVal(fileDriver[i].driverName);
		txParam.SetAttribVal(NULL,TEXT("DriverName"),valDriver);
		valDriver.SetVal(fileDriver[i].fileType);
		txParam.SetAttribVal(NULL,TEXT("FileType"),valDriver);
		valDriver.SetVal(fileDriver[i].isDefault);
		txParam.SetAttribVal(NULL,TEXT("IsDefault"),valDriver);
		valDriver.SetVal(fileDriver[i].DetectedCLIP_HEIGHT);
		txParam.SetAttribVal(NULL,TEXT("DetectedCLIP_HEIGHT"),valDriver);
		valDriver.SetVal(fileDriver[i].DetectedCLIP_WIDTH);
		txParam.SetAttribVal(NULL,TEXT("DetectedCLIP_WIDTH"),valDriver);
		txParam.OutofKey();
	}
	for ( i =0;i<detectLevelCount;i++)
	{
		CString tempLevel ;
		CTxStrConvert valLevel;
		tempLevel.Format("DetectLevel%d",i);
		txParam.SetElemVal(tempLevel, valLevel);
		txParam.GoIntoKey(tempLevel);
		valLevel.SetVal(detectLevel[i].levelID);
		txParam.SetAttribVal(NULL,TEXT("LevelID"),valLevel);
		valLevel.SetVal(detectLevel[i].levelName);
		txParam.SetAttribVal(NULL,TEXT("LevelName"),valLevel);
		valLevel.SetVal(detectLevel[i].blackFrame);
		txParam.SetAttribVal(NULL,TEXT("BlackFrame"),valLevel);
		valLevel.SetVal(detectLevel[i].colorFrame);
		txParam.SetAttribVal(NULL,TEXT("ColorFrame"),valLevel);
		valLevel.SetVal(detectLevel[i].staticFrame);
		txParam.SetAttribVal(NULL,TEXT("StaticFrame"),valLevel);
		valLevel.SetVal(detectLevel[i].colorSripe);
		txParam.SetAttribVal(NULL,TEXT("ColorSripe"),valLevel);
		valLevel.SetVal(detectLevel[i].muteDur);
		txParam.SetAttribVal(NULL,TEXT("HighDur"),valLevel);
		valLevel.SetVal(detectLevel[i].LowDur);
		txParam.SetAttribVal(NULL,TEXT("LowDur"),valLevel);

		txParam.OutofKey();
	}
	for ( i = 0 ;i<detectThresholodCount;i++)
	{
		CString tempThresholod ;
		CTxStrConvert valThresholod ;
		tempThresholod.Format("DetectThresholodCount%d",i);
		txParam.SetElemVal(tempThresholod, valThresholod);
		txParam.GoIntoKey(tempThresholod);
		valThresholod.SetVal(detectThresholod[i].name);
		txParam.SetAttribVal(NULL,TEXT("Name"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].TOP_SAFE_SECTION);
		txParam.SetAttribVal(NULL,TEXT("TOP_SAFE_SECTION"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].BOTTOM_SAFE_SECTION);
		txParam.SetAttribVal(NULL,TEXT("BOTTOM_SAFE_SECTION"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].LEFT_SAFE_SECTION);
		txParam.SetAttribVal(NULL,TEXT("LEFT_SAFE_SECTION"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].RIGHT_SAFE_SECTION);
		txParam.SetAttribVal(NULL,TEXT("RIGHT_SAFE_SECTION"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].BLACK_SCENE_THRESHOLD);
		txParam.SetAttribVal(NULL,TEXT("BLACK_SCENE_THRESHOLD"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].COLOR_SCENE_PERCENTAGE);
		txParam.SetAttribVal(NULL,TEXT("COLOR_SCENE_PERCENTAGE"),valThresholod);		
		valThresholod.SetVal(detectThresholod[i].COLOR_STRIP_PERCENTAGE);
		txParam.SetAttribVal(NULL,TEXT("COLOR_STRIP_PERCENTAGE"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].STATIC_FRAME_PERCENTAGE);
		txParam.SetAttribVal(NULL,TEXT("STATIC_FRAME_PERCENTAGE"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].LINE_COUNTS_FOR_STRIP_DETECT);
		txParam.SetAttribVal(NULL,TEXT("LINE_COUNTS_FOR_STRIP_DETECT"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AMBIT_DEEMED_TO_SAME_PIXEL);
		txParam.SetAttribVal(NULL,TEXT("AMBIT_DEEMED_TO_SAME_PIXEL"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].UNDULATE_AMBIT_OF_Y);
		txParam.SetAttribVal(NULL,TEXT("UNDULATE_AMBIT_OF_Y"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].UNDULATE_AMBIT_OF_U);
		txParam.SetAttribVal(NULL,TEXT("UNDULATE_AMBIT_OF_U"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].UNDULATE_AMBIT_OF_V);
		txParam.SetAttribVal(NULL,TEXT("UNDULATE_AMBIT_OF_V"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AUDIO_Low_THRESHOLD);
		txParam.SetAttribVal(NULL,TEXT("AUDIO_Low_THRESHOLD"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AUDIO_Low_PERIOD);
		txParam.SetAttribVal(NULL,TEXT("AUDIO_Low_PERIOD"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AUDIO_High_THRESHOLD);
		txParam.SetAttribVal(NULL,TEXT("AUDIO_High_THRESHOLD"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AUDIO_High_PERIOD);
		txParam.SetAttribVal(NULL,TEXT("AUDIO_High_PERIOD"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AUDIO_Mute_THRESHOLD);
		txParam.SetAttribVal(NULL,TEXT("AUDIO_Mute_THRESHOLD"),valThresholod);
		valThresholod.SetVal(detectThresholod[i].AUDIO_Mute_THRESHOLD);
		txParam.SetAttribVal(NULL,TEXT("AUDIO_Mute_THRESHOLD"),valThresholod);
		txParam.OutofKey();
		}		
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;	
}

BOOL ST_EXCUTORINFO::ToString( CString& strOut )
{
	CTxParamString txParam(EDOC_ST_ST_EXCUTORINFO);
	txParam.GoIntoKey("ExecutorInfo");

	CTxStrConvert val;
	val.SetVal(excutorId);
	txParam.SetAttribVal(NULL, TEXT("ExecutorId"), val);
	CString strTem;
	strTem.Format("%d", hProcessId);
	val.SetVal(strTem);
	txParam.SetAttribVal(NULL, TEXT("ProcessId"), val);

	val.SetVal(m_strTaskGuid);
	txParam.SetAttribVal(NULL, TEXT("TaskGuid"), val);

	val.SetVal(m_strRunStep);
	txParam.SetAttribVal(NULL, TEXT("RunStep"), val);

	val.SetVal(m_nPercent);
	txParam.SetAttribVal(NULL, TEXT("nPercent"), val);

	txParam.UpdateData();
	strOut = txParam;
	return TRUE;	
}

BOOL ST_EXCUTORINFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey("ExecutorInfo");

	excutorId = txParam.GetAttribVal(NULL,TEXT("ExecutorId")).GetAsInt();
	hProcessId = txParam.GetAttribVal(NULL,TEXT("ProcessId")).GetAsInt();
	m_strTaskGuid = txParam.GetAttribVal(NULL,TEXT("TaskGuid")).GetAsString();
	m_strRunStep = txParam.GetAttribVal(NULL,TEXT("RunStep")).GetAsString();
	m_nPercent = txParam.GetAttribVal(NULL,TEXT("nPercent")).GetAsInt();

	return TRUE;
}
BOOL ST_FCVS::ToString(CString& strOut)
{
	CString temp = "";
	CString tempTask = "";
	CString tempConfig = "";
	taskInfo.ToString(tempTask);
	configInfo.ToString(tempConfig);
	temp.Format("<FCVS>%s%s</FCVS>",tempConfig,tempTask);
	strOut = temp;
	return TRUE;
}

BOOL ST_DBWRITERTASKINFO::ToString( CString& strOut )
{
	if (vSqlKeys.size() != vSqls.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CString strFmt;
	CTxStrConvert val(vSqlKeys);
	strFmt.Format(EDOC_WORKDBWRITERFMT, nDBType, strConn, val.GetAsString());
	CTxParamString txParam(strFmt);
	txParam.GoIntoKey(TEXT("dbwriter"));

	for (size_t i = 0; i < vSqlKeys.size();++i)
	{
		txParam.SetElemVal(vSqlKeys[i], (LPCTSTR)vSqls[i]);
	}
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_DBWRITERTASKINFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(TEXT("dbwriter"));
	nDBType = txParam.GetAttribVal(NULL, TEXT("dbtype")).GetAsInt(INVALID_VALUE);
	strConn = txParam.GetAttribVal(NULL, TEXT("conn")).GetAsString();
	txParam.GetAttribVal(NULL, TEXT("cmdlist")).GetAsStringArray(vSqlKeys);
	vSqls.clear();
	for (size_t i = 0 ; i < vSqlKeys.size(); ++i)
	{
		CString strTmp = txParam.GetElemVal(vSqlKeys[i]).GetAsString();
		vSqls.push_back(strTmp);
	}

	return TRUE;
}

BOOL ST_UISVRCONFIG::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_UISVRCONFIG);
	nProberType = txParam.GetAttribVal(NULL, TEXT("type")).GetAsInt(INVALID_VALUE);
	CString strAddr = txParam.GetAttribVal(NULL, TEXT("addrListen")).GetAsString();
	if (!strAddr.IsEmpty())
	{
		addrListen = GetAddrFromStr(strAddr);
	}

	return TRUE;
}

BOOL ST_UISVRCONFIG::ToString( CString& strOut )
{
	CTxParamString txParam(TEXT(""));
	txParam.SetElemVal(EK_UISVRCONFIG, TEXT(""));
	txParam.GoIntoKey(EK_UISVRCONFIG);
	txParam.SetAttribVal(NULL, TEXT("type"), nProberType);
	txParam.SetAttribVal(NULL, TEXT("addrListen"), Addr2String(addrListen).c_str());
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_SVRLIVEINFO::ToString( CString& strOut )
{
	CString strFmt;
	strFmt.Format(EDOC_MAINHEADERFMT, 1, embxmltype_svrInfo, TEXT(""));
	CTxParamString txParam(strFmt);
	txParam.GoIntoKey(EK_MAIN);
	txParam.SetElemVal(EK_SVRLIVEINFO, TEXT(""));
	txParam.GoIntoKey(EK_SVRLIVEINFO);
	txParam.SetAttribVal(NULL, TEXT("svrId"), nsvrId);
	txParam.SetAttribVal(NULL, TEXT("master"), nMaster);
	txParam.SetAttribVal(NULL, TEXT("active"), nActive);
	txParam.SetAttribVal(NULL, TEXT("connState"), nConnState);
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_SVRLIVEINFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_MAIN);
	int nType = txParam.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt();
	if (nType != embxmltype_svrInfo)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	txParam.GoIntoKey(EK_SVRLIVEINFO);
	nsvrId = txParam.GetAttribVal(NULL, TEXT("svrId")).GetAsInt(INVALID_VALUE);
	nMaster = txParam.GetAttribVal(NULL, TEXT("master")).GetAsInt(INVALID_VALUE);
	nActive = txParam.GetAttribVal(NULL, TEXT("active")).GetAsInt(INVALID_VALUE);
	nConnState = txParam.GetAttribVal(NULL, TEXT("connState")).GetAsInt(INVALID_VALUE);
	return TRUE;
}

BOOL ST_TASKLISTINFO::ToString( CString& strOut )
{
	CTxStrConvert val(vlist);
	strOut.Format(EDOC_TASKLISTFMT, EMBVER, embxmltype_taskList, TEXT(""), vlist.size(), val.GetAsString());
	return TRUE;
}

BOOL ST_TASKLISTINFO::FromString( const CString& strIn )
{
	vlist.clear();
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_MAIN);
	int nType = txParam.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt();
	if(nType != embxmltype_taskList)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	txParam.GoIntoKey(TEXT("list"));
	int nCount = txParam.GetAttribVal(NULL, TEXT("count")).GetAsInt();
	txParam.GetElemVal(EK_TASKLIST).GetAsStringArray(vlist);
	ASSERT(vlist.size() == nCount);
	return TRUE;
}

BOOL ST_ACTORLISTINFO::ToString( CString& strOut )
{
	CTxStrConvert val(vlist);
	strOut.Format(EDOC_ACTORLISTFMT, EMBVER, embxmltype_taskList, TEXT(""), vlist.size(), val.GetAsString());
	return TRUE;

}

BOOL ST_ACTORLISTINFO::FromString( const CString& strIn )
{
	vlist.clear();
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_MAIN);
	int nType = txParam.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt();
	if(nType != embxmltype_actorList)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	int nCount = txParam.GetAttribVal(EK_ACTORLIST, TEXT("count")).GetAsInt();
	txParam.GetElemVal(EK_ACTORLIST).GetAsStringArray(vlist);
	ASSERT(vlist.size() == nCount);
	return TRUE;
}

BOOL ST_UICLIENTCONFIG::ToString( CString& strOut )
{
	CTxParamString txParam(TEXT(""));
	txParam.SetElemVal(EK_UICLIENTCONFIG, TEXT(""));
	txParam.GoIntoKey(EK_UICLIENTCONFIG);
	txParam.SetAttribVal(NULL, TEXT("addrMain"), Addr2String(addrMain).c_str());
	txParam.SetAttribVal(NULL, TEXT("addrSlave"), Addr2String(addrSlave).c_str());
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_UICLIENTCONFIG::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_UICLIENTCONFIG);
	CString strAddr = txParam.GetAttribVal(NULL, TEXT("addrMain")).GetAsString();
	addrMain = GetAddrFromStr(strAddr);
	strAddr = txParam.GetAttribVal(NULL, TEXT("addrSlave")).GetAsString();
	addrSlave = GetAddrFromStr(strAddr);
	return TRUE;
}
BOOL ST_FCVSRESULTTASK::ToString(CString& strOut)
{
	CTxParamString txParam("");
	CTxStrConvert val;
	txParam.SetElemVal(EK_FCVSRESULTTASK, val);
	txParam.GoIntoKey(EK_FCVSRESULTTASK);
	val.SetVal(filePath);
	txParam.SetAttribVal(NULL,TEXT("FilePath"),val);
	val.SetVal(nTotalSectionCount);
	txParam.SetAttribVal(NULL,TEXT("NTotalSectionCount"),val);	
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
};
BOOL ST_FCVSRESULTTASK::FromString(const CString& strIn)
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(EK_FCVSRESULTTASK);
	filePath = txParam.GetAttribVal(NULL,TEXT("FilePath")).GetAsString("");
	nTotalSectionCount = txParam.GetAttribVal(NULL,TEXT("NTotalSectionCount")).GetAsInt(0);
	return TRUE;
}
