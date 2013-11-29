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
	txParam.GoIntoKey(EK_FCVSTASK);

		
	clipType = txParam.GetAttribVal(NULL,TEXT("ClipType")).GetAsString();
	fileMediaType = txParam.GetAttribVal(NULL,TEXT("FileMediaType")).GetAsString();
	checkItem = txParam.GetAttribVal(NULL,TEXT("CheckItem")).GetAsInt(0x00111111);
// 	FCVSTaskCut = txParam.GetAttribVal(NULL,TEXT("FCVSTaskCut")).GetAsBOOL(FALSE);
// 	if (FCVSTaskCut)
	//{
		TaskCutSOM = txParam.GetAttribVal(NULL,TEXT("TaskCutSOM")).GetAsInt64(0);
		TaskCutEOM = txParam.GetAttribVal(NULL,TEXT("TaskCutEOM")).GetAsInt64(0);
	//}
	usedDetectLevel = txParam.GetAttribVal(NULL,TEXT("usedDetectlevel")).GetAsString();
	fileAdrType = txParam.GetAttribVal(NULL,TEXT("FileAdrType")).GetAsString();
	filePath = txParam.GetAttribVal(NULL,TEXT("FilePath")).GetAsString();
	fileName = txParam.GetAttribVal(NULL,TEXT("FileName")).GetAsString();

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
	txParam.SetAttribVal(NULL, TEXT("FileType"),val);
	val.SetVal(checkItem);
	txParam.SetAttribVal(NULL,TEXT("CheckItem"),val);
// 	val.SetVal(FCVSTaskCut);
// 	txParam.SetAttribVal(NULL,TEXT("FCVSTaskCut"),val);
	val.SetVal(TaskCutSOM);
	txParam.SetAttribVal(NULL,TEXT("TaskCutSOM"),val);
	val.SetVal(TaskCutEOM);
	txParam.SetAttribVal(NULL,TEXT("TaskCutEOM"),val);
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
	txParam.UpdateData();
	strOut = txParam;
	return TRUE;	
}
BOOL ST_FCVSCONFIGINFO::FromString(const CString& strIn)
{
	int i;
	CTxParamString txParam(strIn);
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
