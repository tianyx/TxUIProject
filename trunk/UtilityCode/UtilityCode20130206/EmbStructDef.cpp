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
	strExcPath = sParm.GetAttribVal(NULL, TEXT("strExcPath")).GetAsString();

	return TRUE;
}

BOOL ST_EDOCMAINHEADER::ToString( CString& strOut )
{
	strOut.Format(EDOC_TASKHEADERFMT, nVer, nType, strGuid);
	return TRUE;
}

BOOL ST_EDOCMAINHEADER::FromString( const CString& strIn )
{
	CTxParamString sParm(strIn);
	sParm.GoIntoKey(EK_MAIN);
	nVer = sParm.GetAttribVal(NULL, EA_MAIN_VER).GetAsInt(-1);
	nType = sParm.GetAttribVal(NULL, EA_MAIN_TYPE).GetAsInt(-1);
	strGuid = sParm.GetAttribVal(NULL, EA_MAIN_GUID).GetAsString();
	return TRUE;
}

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
	return TRUE;
}

BOOL ST_TASKRUNSTATE::ToString( CString& strOut )
{
	CTxParamString sParm(EDOC_ST_TASKRUNSTATE_STRUCT);
	sParm.GoIntoKey(EK_TASKBASIC);

	return TRUE;
}

BOOL ST_TASKRUNSTATE::FromString( const CString& strIn )
{
	return TRUE;
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

	return TRUE;
}

BOOL ST_TASKUPDATE::ToString( CString& strOut )
{
	CString strParam;
	strParam.Format(EDOC_TASKHEADERFMT, embxmltype_taskupdate, TEXT(""));
	CTxParamString txParam(strParam);
	CTxStrConvert val;
	txParam.SetElemVal(EK_TASKUPDATE, val);
	txParam.GoIntoKey(EK_TASKUPDATE);
	val.SetVal(Guid2String(guid));
	txParam.SetAttribVal(NULL, TEXT("guid"), val);
	val.SetVal(nUpdateType);
	txParam.SetAttribVal(NULL, TEXT("nUpdateType"), val);

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

	txParam.UpdateData();
	strOut = txParam;
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
	txParam.GoIntoKey(EK_TASKUPDATE);
	if (nUpdateType == embtaskupdatetype_finish)
	{
		//to be add...
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
	val.SetVal(nDiscUsage);
	txParam.SetAttribVal(NULL, TEXT("nDiscUsage"), val);
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
	nDiscUsage= txParam.GetAttribVal(NULL, TEXT("nDiscUsage")).GetAsInt(INVALID_ID);
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
	strOut.Format(EDOC_MAINHEADERFMT, ver, nType, guid);

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
	strOut.Format(EDOC_WORKMD5FMT, strFileToCheck, strFileToWriteResult);
	return TRUE;
}

BOOL ST_MD5TASKINFO::FromString( const CString& strIn )
{
	CTxParamString txParam(strIn);
	strFileToCheck = txParam.GetAttribVal(EK_WORKMD5, EA_MD5DES).GetAsString();
	strFileToWriteResult = txParam.GetAttribVal(EK_WORKMD5, EA_MD5WRITETO).GetAsString();
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
