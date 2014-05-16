#include "StdAfx.h"
#include "GlobalDef.h"
#include "FGlobal.h"
#include "io.h"
#include "AutoLuaState.h"
#include "TxParamString.h"
#include <algorithm>
#include "TxADOConn.h"
#include "AutoTaskFunc.h"
ST_GLOBAL g_GlobalInfo;
CAutoCritSec g_csStg;

HWND g_hFrame = NULL;
CAutoTaskFunc* g_pAutoTaskFunc = NULL;
using namespace std;

#define MAXSTGFILELENGTH 1024*1024

#define STGDOC_CFG_BEGIN	TEXT("--[[**stgConfigPosBegin**--")
#define STGDOC_CFG_END		TEXT("--**stgConfignPosEnd**]]--")
#define STGDOC_LUA_BEGIN	TEXT("--**stgLuaPosBegin**--")
#define STGDOC_LUA_END		TEXT("--**stgLuaPosEnd**--")

BOOL ST_STGPARAM::ToString( CString& strOut )
{
	CTxParamString txParam(TEXT(""));
	txParam.SetElemVal(TEXT("stgconfig"), TEXT(""));
	txParam.GoIntoKey(TEXT("stgconfig"));
	txParam.SetAttribVal(NULL, TEXT("bEnable"), bEnable);
	txParam.SetAttribVal(NULL, TEXT("strName"), strName);
	txParam.SetAttribVal(NULL, TEXT("nStgType"), nStgType);

	int nIdx = 0;
	if (mapKt.size() > 0)
	{
		txParam.SetElemVal(TEXT("KtInfo"), TEXT(""));
		txParam.GoIntoKey(TEXT("KtInfo"));
		txParam.SetAttribVal(NULL, TEXT("count"), mapKt.size());
		MAPKEYTEMPLATE::iterator itb = mapKt.begin();
		MAPKEYTEMPLATE::iterator ite = mapKt.end();
		CString strIdx;
		strIdx.Format(TEXT("Id%d"), nIdx++);
		txParam.SetElemVal(strIdx, TEXT(""));
		txParam.SetAttribVal(strIdx, TEXT("key"), itb->first);
		txParam.SetAttribVal(strIdx, TEXT("val"), itb->second);
	}

	txParam.UpdateData();
	strOut = txParam;
	return TRUE;
}

BOOL ST_STGPARAM::FromString( CString& strIn )
{
	CTxParamString txParam(strIn);
	txParam.GoIntoKey(TEXT("stgconfig"));

	bEnable = txParam.GetAttribVal(NULL,  TEXT("bEnable")).GetAsBOOL(FALSE);
	strName = txParam.GetAttribVal(NULL, TEXT("strName")).GetAsString();
	nStgType = txParam.GetAttribVal(NULL, TEXT("nStgType")).GetAsInt(-1);
	nStgPriority = txParam.GetAttribVal(NULL, TEXT("nStgPriority")).GetAsInt(0);
	txParam.GoIntoKey(TEXT("KtInfo"));
	int nCount = txParam.GetAttribVal(NULL, TEXT("count")).GetAsInt();
	if (nCount > 0)
	{
		for (int i = 0; i < nCount; ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("Id%d"),  i);
			int nKey =txParam.GetAttribVal(strIdx, TEXT("key")).GetAsInt(-9999);
			CString strVal = txParam.GetAttribVal(strIdx, TEXT("val")).GetAsString();
			if (!strVal.IsEmpty())
			{
				mapKt[nKey] = strVal;
			}
		}
	}
	return TRUE;
}


BOOL InitGlobalConfig()
{
	g_GlobalInfo.szAppPath = GetAppPath();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EmbTaskOwl.xml");
	if (_access(g_GlobalInfo.szIniPath.c_str(), 04) == -1)
	{
		CFWriteLog(0, "找不到配置文件EmbTaskOwlConfig.xml");
		return FALSE;
	}

	CFile xmlfile;
	CString strParam;
	if (xmlfile.Open(g_GlobalInfo.szIniPath.c_str(), CFile::modeRead, NULL))
	{
		int nFileLen = xmlfile.GetLength();
		char* pBuff = new char[nFileLen +1];
		pBuff[nFileLen] = '\0';
		int nRead = xmlfile.Read(pBuff, nFileLen);
		strParam = pBuff;
		delete[] pBuff;
		pBuff = NULL;
		xmlfile.Close();
	}
	else
	{
		ASSERT(FALSE);
		AfxMessageBox("read config file err");
		return FALSE;
	}

	g_GlobalInfo.FromString(strParam);

	return TRUE;
}

BOOL LoadStgConfig()
{

	g_GlobalInfo.stgConfig.vStgs.clear();

	CString strStgPath = g_GlobalInfo.szAppPath.c_str();
	strStgPath += TEXT("\\EmbStrategy");

	vector<CString> vStgFiles;
	GetFilesInFolder(strStgPath, vStgFiles, TEXT("stg"), FALSE);
	
	for (size_t i = 0; i < vStgFiles.size(); ++i)
	{
		ST_STGPARAM stg;
		if(AnalyseStgFile(vStgFiles[i], stg))
		{
			g_GlobalInfo.stgConfig.vStgs.push_back(stg);
			CFWriteLog(0, TEXT("strategy [%s] loaded..."), stg.strName);
		}
		else
		{
			//found invalid template
			g_GlobalInfo.bIsAllOk = FALSE;
		}
	}

	SortStgByPriority();
	CFWriteLog(0, TEXT("total [%d] strategy files  loaded..."), g_GlobalInfo.stgConfig.vStgs.size());

	return g_GlobalInfo.stgConfig.vStgs.size() > 0;

}

BOOL AnalyseStgFile( CString& strFileIn, ST_STGPARAM& stgOut )
{
	stgOut.bValid = FALSE;
	BOOL bRet = FALSE;
	CFile file;
	bRet = file.Open(strFileIn, CFile::modeRead, NULL);
	if (!bRet)
	{
		CFWriteLog(0, TEXT("stgfile open failed, %s"), strFileIn);
		return bRet;
	}

	UINT64 nFileLen = file.GetLength();
	if (nFileLen > MAXSTGFILELENGTH)
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("stg file too large > 1m, %s"), strFileIn);
		file.Close();
		return bRet;
	}

	char* pBuff  = new char[nFileLen +2];
	ZeroMemory(pBuff, nFileLen +2);
	file.Read(pBuff, nFileLen);
	
	string szData = pBuff;

	delete pBuff;
	pBuff = NULL;
	file.Close();
	
	wstring wszBuff = UTF82W(szData);
	CString strStgData = W2Ansi(wszBuff).c_str();

	int nPosBegin = strStgData.Find(STGDOC_CFG_BEGIN);
	int nPosEnd = strStgData.Find(STGDOC_CFG_END);

	CString strCfg;
	BOOL bFoundCfg = FALSE;
	if (nPosBegin >= 0 && nPosEnd >nPosBegin)
	{
		//stg config valid
		int nPosCfg0 = strStgData.Find(TEXT("<"), nPosBegin);
		if (nPosCfg0 > nPosBegin && nPosCfg0 < nPosEnd)
		{
			//get stg
			strCfg = strStgData.Mid(nPosCfg0, nPosEnd - nPosCfg0);
			bFoundCfg = TRUE;
		}
		else
		{
			CFWriteLog(0, TEXT("stg config section not found, %s"), strFileIn);
		}
	}
	else
	{
		CFWriteLog(0, TEXT("stg config section not found, %s"), strFileIn);
	}

	if (!bFoundCfg)
	{
		return FALSE;
	}

	//check stg func
	nPosBegin = strStgData.Find(STGDOC_LUA_BEGIN);
	nPosEnd = strStgData.Find(STGDOC_LUA_END);

	CString strLua;
	BOOL bFoundLua = FALSE;
	if (nPosBegin >= 0 && nPosEnd >nPosBegin)
	{
		//stg config valid
		int nPosLua0 = nPosBegin;
		if (nPosLua0 >= nPosBegin && nPosLua0 < nPosEnd)
		{
			//get stg
			strLua = strStgData.Mid(nPosLua0, nPosEnd - nPosLua0);
			bFoundLua = TRUE;
		}
		else
		{
			CFWriteLog(0, TEXT("stg config section not found, %s"), strFileIn);
		}
	}
	else
	{
		CFWriteLog(0, TEXT("stg config section not found, %s"), strFileIn);
	}

	if (!bFoundCfg || !bFoundLua)
	{
		return bRet;
	}

	stgOut.FromString(strCfg);
	stgOut.strStgFile = strFileIn;
	stgOut.strLua = strLua;
	
	if (stgOut.nStgType == stgtype_unknow)
	{
		CFWriteLog(0, TEXT("strategy type incorrect, %s"), strFileIn);
		return FALSE;
	}

	bRet = TestLuaValid(strLua) && CheckStgValMappingValid(stgOut);
	stgOut.bValid = bRet;
	return bRet;
}

BOOL TestLuaValid( CString& strLuaIn)
{
	if (strLuaIn.Find(LUASTGFUNCNAMEFORMAT) == -1)
	{
		CFWriteLog(0, TEXT("stragegy function %s not found!"), LUASTGFUNCNAMEFORMAT);
		return FALSE;
	}

	CAutoLuaState aLState;
	luaL_openlibs(aLState);

	//load lua func file
	int nRet = luaL_loadstring(aLState, strLuaIn);
	if (nRet != LUA_OK)
	{
		CFWriteLog(TEXT("lua stragegy check failed, lua return:%s"), lua_tostring(aLState, -1));
		return FALSE;
	}

	return TRUE;
}

bool BigerStgPri(const ST_STGPARAM& par1, const ST_STGPARAM& par2)
{
	return par1.nStgPriority > par2.nStgPriority;
}

BOOL SortStgByPriority()
{
	if (g_GlobalInfo.stgConfig.vStgs.size() < 2)
	{
		return FALSE;
	}
	std::sort(g_GlobalInfo.stgConfig.vStgs.begin(), g_GlobalInfo.stgConfig.vStgs.end(), BigerStgPri);
	return TRUE;
}

BOOL LoadEmbTemplate()
{
	g_GlobalInfo.mapTaskTemplates.clear();
	CString strStgPath = g_GlobalInfo.szAppPath.c_str();
	strStgPath += TEXT("\\EmbTemplate");

	vector<CString> vtmpFiles;
	GetFilesInFolder(strStgPath, vtmpFiles, TEXT("xml"), FALSE);

	for (size_t i = 0; i < vtmpFiles.size(); ++i)
	{
		ST_EMBTEMPLATEINFO infoOut;
		if (AnalyseEmbTemplateFile(vtmpFiles[i], infoOut))
		{
			//get template key;
			int nBegin = strStgPath.GetLength();
			int nCount = vtmpFiles[i].GetLength() - nBegin-1;
			CString strKey = vtmpFiles[i].Mid(strStgPath.GetLength() +1, nCount-4);
			infoOut.strKey = strKey;
			g_GlobalInfo.mapTaskTemplates[strKey] = infoOut;
			CFWriteLog(0, TEXT("template [%s] loaded..."), strKey);
		}
		else
		{
			//found invalid template
			g_GlobalInfo.bIsAllOk = FALSE;
		}
	}

	CFWriteLog(0, TEXT("total %d template file loaded..."), g_GlobalInfo.mapTaskTemplates.size());
	return g_GlobalInfo.mapTaskTemplates.size() > 0;
}

BOOL AnalyseEmbTemplateFile( CString& strFileIn, ST_EMBTEMPLATEINFO& tOut )
{
	BOOL bRet = FALSE;
	CFile file;
	if (!file.Open(strFileIn, CFile::modeRead, NULL))
	{
		CFWriteLog(0, TEXT("template open error!, %s"), strFileIn);
		return FALSE;
	}

	UINT64 nFileLen = file.GetLength();
	if (nFileLen > MAXSTGFILELENGTH*2)
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("template file too large > 2m, %s"),strFileIn);
		file.Close();
		return bRet;
	}


	char* pBuff  = new char[nFileLen +2];
	ZeroMemory(pBuff, nFileLen +2);
	file.Read(pBuff, nFileLen);

	tOut.strContent = pBuff;
	tOut.strFile = strFileIn;
	delete pBuff;
	pBuff = NULL;
	file.Close();

	int nPos = tOut.strContent.Find(TEXT("<taskSequence>"));
	if (nPos > 0)
	{
		int nPos2 = tOut.strContent.Find(TEXT("</taskSequence>"));
		if (nPos2 > 0)
		{
			tOut.strTaskSequence = tOut.strContent.Mid(nPos+14, nPos2 - (nPos+14));
		}
	}

	bRet = TRUE;

	return bRet;
}

BOOL CheckStgValMappingValid( ST_STGPARAM& stgIn )
{
	if (stgIn.mapKt.size() == 0)
	{
		return TRUE;
	}

	BOOL bLost = FALSE;
	MAPKEYTEMPLATE::iterator itb = stgIn.mapKt.begin();
	MAPKEYTEMPLATE::iterator ite = stgIn.mapKt.end();
	for (; itb != ite; ++itb)
	{
		MAPTPLCONTENT::iterator itf = g_GlobalInfo.mapTaskTemplates.find(itb->second);
		if (itf == g_GlobalInfo.mapTaskTemplates.end())
		{
			CFWriteLog(0, TEXT("strategy mapping lost, <%d, %s>"), itb->first, itb->second);
			bLost = TRUE;
		}
	}

	return !bLost;
}





CString GenClipIDFromDB()
{
	CString strRet;
	CTxADOCommand cmd(DBKEY_PLAYLIST);
	_RecordsetPtr ptr = cmd.ExecuteStoredProc(TEXT("sp_GetNewDUBID"));
	if (ptr)
	{
		strRet = ptr->GetCollect("NewDubID");
	}

	if (strRet.IsEmpty())
	{
		CFWriteLog(TEXT("获取NewDubID失败"));
	}
	else
	{
		//replace pos 3 with global ID;
		strRet.SetAt(3, g_GlobalInfo.chClipMachineID);
	}

	return strRet;

}

CString GenClipId2()
{
	srand(time(NULL));
	static UINT64 nSeed =rand();
	nSeed++;
	CString strRet;
	strRet.Format(TEXT("%08d"), nSeed%100000000);
	return strRet;
}

CString FtpList2String( vector<ST_FTPSITEINFO>& vFtps, BOOL bSrc)
{
	CTxParamString txParam(TEXT(""));
	CString strKeyList = bSrc? TEXT("srcFtpList"):TEXT("desFtpList");
	txParam.SetElemVal(strKeyList, TEXT(""));
	txParam.GoIntoKey(strKeyList);
	txParam.SetAttribVal(NULL, TEXT("count"), vFtps.size());
	size_t i;
	for (i = 0; i < vFtps.size(); ++i)
	{
		ST_FTPSITEINFO& sitRef = vFtps[i];
		CString strNum;
		strNum.Format(TEXT("site%d"), i);
		txParam.SetElemVal(strNum, TEXT(""));
		txParam.SetAttribVal(strNum, TEXT("ftpName"), sitRef.strFtpName);
		txParam.SetAttribVal(strNum, TEXT("strFtpIp"), sitRef.strFtpIp);
		txParam.SetAttribVal(strNum, TEXT("nFtpPort"), sitRef.nFtpPort);
		txParam.SetAttribVal(strNum, TEXT("strUser"), sitRef.strUser);
		txParam.SetAttribVal(strNum, TEXT("strPw"), sitRef.strPw);
		txParam.SetAttribVal(strNum, TEXT("nPassive"), sitRef.nPassive);
		txParam.SetAttribVal(strNum, TEXT("strUncDir"), sitRef.strUncDir);

		txParam.SetAttribVal(strNum, TEXT("strDBConn"), sitRef.strDBConn);
		txParam.SetAttribVal(strNum, TEXT("nLocation"), sitRef.nLocation);
		txParam.SetAttribVal(strNum, TEXT("strStoreName"), sitRef.strStoreName);

	}

	txParam.UpdateData();
	return txParam;

}

CString TransDBInfo2String(CString strClipLogicId, CString strClipId, ST_DB_BVSIDINFO& dbBvsInfo, ST_DB_LOCATIONINFO& dbLocationInfo )
{
	CTxParamString txParam(TEXT(""));
	//dbinfo
	txParam.SetElemVal(TEXT("dbinfo"), TEXT(""));
	txParam.GoIntoKey(TEXT("dbinfo"));
	txParam.SetAttribVal(NULL, TEXT("strClipLogicID"), strClipLogicId);
	txParam.SetAttribVal(NULL, TEXT("strClipID"), strClipId);

	//t_location info
	txParam.SetElemVal(TEXT("tloc"), TEXT(""));
	txParam.GoIntoKey(TEXT("tloc"));
	txParam.SetAttribVal(NULL, TEXT("strMediaType"), dbLocationInfo.strMediaType);
	txParam.SetAttribVal(NULL, TEXT("nBitReate"), dbLocationInfo.nBitRate);
	txParam.SetAttribVal(NULL, TEXT("strFileSize"), dbLocationInfo.strFileSize);
	txParam.SetAttribVal(NULL, TEXT("nAfdType"), dbLocationInfo.nAfdType);
	txParam.OutofKey();

	//t_bvsid info
	txParam.SetElemVal(TEXT("tbvsid"), TEXT(""));
	txParam.GoIntoKey(TEXT("tbvsid"));
	txParam.SetAttribVal(NULL, TEXT("strClipName"), dbBvsInfo.strClipName);
	txParam.SetAttribVal(NULL, TEXT("strTapeID"), dbBvsInfo.strTapeID);
	txParam.SetAttribVal(NULL, TEXT("nTapeType"), dbBvsInfo.nTapeType);
	txParam.SetAttribVal(NULL, TEXT("strSOM"), dbBvsInfo.strSOM);
	txParam.SetAttribVal(NULL, TEXT("strEOM"), dbBvsInfo.strEOM);
	txParam.SetAttribVal(NULL, TEXT("strDuration"), dbBvsInfo.strDuration);
	txParam.SetAttribVal(NULL, TEXT("strLSOM"), dbBvsInfo.strLSOM);
	txParam.SetAttribVal(NULL, TEXT("strLEOM"), dbBvsInfo.strLEOM);
	txParam.SetAttribVal(NULL, TEXT("strLDuration"), dbBvsInfo.strLDuration);
	txParam.SetAttribVal(NULL, TEXT("strTypeSOM"), dbBvsInfo.strTypeSOM);
	txParam.SetAttribVal(NULL, TEXT("nClipType"), dbBvsInfo.nClipType);
	txParam.SetAttribVal(NULL, TEXT("strChID"), dbBvsInfo.strChID);
	txParam.SetAttribVal(NULL, TEXT("tPlanAirTime"), dbBvsInfo.tPlanAirTime);
	txParam.SetAttribVal(NULL, TEXT("tPlanLastAirTime"), dbBvsInfo.tPlanLastAirTime);
	txParam.SetAttribVal(NULL, TEXT("nBitRate"), dbBvsInfo.nBitRate);
	txParam.UpdateData();
	return txParam;
}

BOOL ST_GLOBAL::FromString( CString& strXml )
{

	CTxParamString txParam(strXml);
	//dbinfo
	txParam.GoIntoKey(TEXT("owlconfig"));
	nAutoLoopInterval = txParam.GetElemVal(TEXT("nAutoLoopInterval")).GetAsInt(60);
	nTimelineClose = txParam.GetElemVal(TEXT("nTimelineClose")).GetAsInt(60);
	nTimeLineOffline = txParam.GetElemVal(TEXT("nTimeLineOffline")).GetAsInt(24*60);
	nMaxTaskCoutOnceCheck = txParam.GetElemVal(TEXT("nMaxTaskCoutOnceCheck")).GetAsInt(100);

	strEmbTaskDBConn = txParam.GetElemVal(TEXT("strEmbTaskDBConn")).GetAsString();
	strPlayListCheckDBConn = txParam.GetElemVal(TEXT("strPlayListCheckDBConn")).GetAsString();
	strPlayListCheckDBConn2 = txParam.GetElemVal(TEXT("strPlayListCheckDBConn2")).GetAsString();
	strPlaylistDBName = txParam.GetElemVal(TEXT("strPlaylistDBName")).GetAsString();
	nIdleHourStart = txParam.GetElemVal(TEXT("nIdleHourStart")).GetAsInt(0);
	nIdleHourNum = txParam.GetElemVal(TEXT("nIdleHourNum")).GetAsInt(-1);
	nIdleTimelineClose= txParam.GetElemVal(TEXT("nIdleTimelineClose")).GetAsInt(0);
	nIdleTimeLineOffline= txParam.GetElemVal(TEXT("nIdleTimeLineOffline")).GetAsInt(24*60);
	chClipMachineID= txParam.GetElemVal(TEXT("chClipMachineID")).GetAsInt(TEXT('x'));
	nDesFtpMaxsize= txParam.GetElemVal(TEXT("nDesFtpMaxsize")).GetAsInt(10);
	nClipKeepDays= txParam.GetElemVal(TEXT("nClipKeepDays")).GetAsInt(5);
	nAfdInfo= txParam.GetElemVal(TEXT("nAfdInfo")).GetAsInt(0);
	nDefBitrate= txParam.GetElemVal(TEXT("nDefBitrate")).GetAsInt(8);
	nHDBitRate= txParam.GetElemVal(TEXT("nHDBitRate")).GetAsInt(8);
	strDefMediaType= txParam.GetElemVal(TEXT("strDefMediaType")).GetAsString(TEXT("OMN"));
	strDefChnID= txParam.GetElemVal(TEXT("strDefChnID")).GetAsString(TEXT("CH00"));
	nDefClipSourceId =txParam.GetElemVal(TEXT("nDefClipSourceId")).GetAsInt(0);
	nDefClipType =txParam.GetElemVal(TEXT("nDefClipType")).GetAsInt(0);

	nEmergencyPriority= txParam.GetElemVal(TEXT("nEmergencyPriority")).GetAsInt(1);
	nEmbtaskRetryCount= txParam.GetElemVal(TEXT("nEmbtaskRetryCount")).GetAsInt(1);
	bShowManualAlert= txParam.GetElemVal(TEXT("bShowManualAlert")).GetAsInt(1);

	mapFtpSites.clear();
	int nCount = txParam.GetAttribVal(TEXT("mapFtpSites"), TEXT("count")).GetAsInt();
	if (nCount > 0)
	{
		txParam.GoIntoKey(TEXT("mapFtpSites"));
		for (int i = 0; i< nCount; ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.GoIntoKey(strIdx);
			int nFtpSrcCount = txParam.GetAttribVal(NULL, TEXT("count")).GetAsInt();
			int nKey =txParam.GetAttribVal(NULL, TEXT("key")).GetAsInt(-1);
			CString strGpName = txParam.GetAttribVal(NULL, TEXT("GroupName")).GetAsString();

			VECFTPSITES vSitSrc;
			for (int iftp = 0; iftp < nFtpSrcCount; ++iftp)
			{
				ST_FTPSITEINFO sitTmp;
				CString strNum;
				strNum.Format(TEXT("site%d"), iftp);
				sitTmp.strFtpName =txParam.GetAttribVal(strNum, TEXT("strFtpName")).GetAsString();
				sitTmp.strFtpIp = txParam.GetAttribVal(strNum, TEXT("strFtpIp")).GetAsString();
				sitTmp.nFtpPort = txParam.GetAttribVal(strNum, TEXT("nFtpPort")).GetAsInt();
				sitTmp.strUser = txParam.GetAttribVal(strNum, TEXT("strUser")).GetAsString();
				sitTmp.strPw = txParam.GetAttribVal(strNum, TEXT("strPw")).GetAsString();
				sitTmp.nPassive = txParam.GetAttribVal(strNum, TEXT("nPassive")).GetAsInt();
				sitTmp.strUncDir = txParam.GetAttribVal(strNum, TEXT("strUncDir")).GetAsString();

				sitTmp.strDBConn = txParam.GetAttribVal(strNum, TEXT("strDBConn")).GetAsString();
				sitTmp.strStoreName = txParam.GetAttribVal(strNum, TEXT("strStoreName")).GetAsString();
				sitTmp.nLocation = txParam.GetAttribVal(strNum, TEXT("nLocation")).GetAsInt();
				vSitSrc.push_back(sitTmp);

			}
			if (nKey != -1)
			{
				mapFtpSites[nKey] = vSitSrc;
				mapGroupIdName[nKey] = strGpName;
			}
			txParam.OutofKey(); //out strIdx
		}
		txParam.OutofKey(); //out mapFtpSites
	}

	//
	nCount = txParam.GetAttribVal(TEXT("mapExternalCHId"), TEXT("count")).GetAsInt();
	if (nCount > 0)
	{
		txParam.GoIntoKey(TEXT("mapExternalCHId"));
		for (int i = 0; i< nCount; ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.GoIntoKey(strIdx);
			CString strKey =txParam.GetAttribVal(NULL, TEXT("key")).GetAsString(); 
			CString strVal =txParam.GetAttribVal(NULL, TEXT("val")).GetAsString(); 
			if (!strKey.IsEmpty() && !strVal.IsEmpty())
			{
				mapExternalCHId[strKey] = strVal;
			}
			txParam.OutofKey();// out strIdx;
		}
		txParam.OutofKey();
	}

	//extcliptype
	nCount = txParam.GetAttribVal(TEXT("mapExternalClipType"), TEXT("count")).GetAsInt();
	if (nCount > 0)
	{
		txParam.GoIntoKey(TEXT("mapExternalClipType"));
		for (int i = 0; i< nCount; ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.GoIntoKey(strIdx);
			CString strKey =txParam.GetAttribVal(NULL, TEXT("key")).GetAsString(); 
			int nVal =txParam.GetAttribVal(NULL, TEXT("val")).GetAsInt(-1); 
			if (!strKey.IsEmpty() && nVal >= 0)
			{
				mapExternalClipType[strKey] = nVal;
			}
			txParam.OutofKey();// out strIdx;
		}
		txParam.OutofKey();//out mapExternalClipId
	}

	//mapExternalClipSrcId;
	nCount = txParam.GetAttribVal(TEXT("mapExternalClipSrcId"), TEXT("count")).GetAsInt();
	if (nCount > 0)
	{
		txParam.GoIntoKey(TEXT("mapExternalClipSrcId"));
		for (int i = 0; i< nCount; ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.GoIntoKey(strIdx);
			CString strKey =txParam.GetAttribVal(NULL, TEXT("key")).GetAsString(); 
			int nVal =txParam.GetAttribVal(NULL, TEXT("val")).GetAsInt(-1); 
			if (!strKey.IsEmpty() && nVal >= 0)
			{
				mapExternalClipSrcId[strKey] = nVal;
			}
			txParam.OutofKey();// out strIdx;
		}
		txParam.OutofKey();//out mapExternalClipSrcId
	}

	//////////////////////////////////////////////////////////////////////////
	//list info
	//auto list
	{
		ST_LISTCONFINFO& lsConRef = listInfoAutoTask;
		txParam.GoIntoKey(TEXT("listInfoAutoTask"));
		lsConRef.clrText = txParam.GetElemVal(TEXT("clrText")).GetAsUInt(RGB(0,0,0));
		lsConRef.clrTextBk = txParam.GetElemVal(TEXT("clrTextBk")).GetAsUInt(RGB(255,255,255));
		lsConRef.clrWarnText = txParam.GetElemVal(TEXT("clrWarnText")).GetAsUInt(RGB(0,200,200));
		lsConRef.clrWarnTextBk = txParam.GetElemVal(TEXT("clrWarnTextBk")).GetAsUInt(RGB(255,255,255));
		lsConRef.clrErrText = txParam.GetElemVal(TEXT("clrErrText")).GetAsUInt(RGB(200,0,0));
		lsConRef.clrErrTextBk = txParam.GetElemVal(TEXT("clrErrTextBk")).GetAsUInt(RGB(255,255,255));

		int nCount = txParam.GetAttribVal(TEXT("headerinfo"), TEXT("count")).GetAsInt();
		if (nCount > 0)
		{
			txParam.GoIntoKey(TEXT("headerinfo"));
			for(int i = 0; i < nCount; ++i)
			{
				ST_LISTHEADERITEM hInfoRef;
				CString strIdx;
				strIdx.Format(TEXT("col%d"), i);
				hInfoRef.nId = txParam.GetAttribVal(strIdx, TEXT("nId")).GetAsInt(-1);
				hInfoRef.nWidth = txParam.GetAttribVal(strIdx, TEXT("nWidth")).GetAsInt(-1);
				hInfoRef.strName = txParam.GetAttribVal(strIdx, TEXT("strName")).GetAsString();
				lsConRef.vListHeader.push_back(hInfoRef);
			}
			txParam.OutofKey(); //out headerinfo
		}
		txParam.OutofKey(); // out listInfoAutoTask
	}

	//manual list
	{
		ST_LISTCONFINFO& lsConRef = listInfoManualTask;
		txParam.GoIntoKey(TEXT("listInfoManualTask"));
		lsConRef.clrText = txParam.GetElemVal(TEXT("clrText")).GetAsUInt(RGB(0,0,0));
		lsConRef.clrTextBk = txParam.GetElemVal(TEXT("clrTextBk")).GetAsUInt(RGB(255,255,255));
		lsConRef.clrWarnText = txParam.GetElemVal(TEXT("clrWarnText")).GetAsUInt(RGB(0,200,200));
		lsConRef.clrWarnTextBk = txParam.GetElemVal(TEXT("clrWarnTextBk")).GetAsUInt(RGB(255,255,255));
		lsConRef.clrErrText = txParam.GetElemVal(TEXT("clrErrText")).GetAsUInt(RGB(200,0,0));
		lsConRef.clrErrTextBk = txParam.GetElemVal(TEXT("clrErrTextBk")).GetAsUInt(RGB(255,255,255));

		int nCount = txParam.GetAttribVal(TEXT("headerinfo"), TEXT("count")).GetAsInt();
		if (nCount > 0)
		{
			txParam.GoIntoKey(TEXT("headerinfo"));
			for(int i = 0; i < nCount; ++i)
			{
				ST_LISTHEADERITEM hInfoRef;
				CString strIdx;
				strIdx.Format(TEXT("col%d"), i);
				hInfoRef.nId = txParam.GetAttribVal(strIdx, TEXT("nId")).GetAsInt(-1);
				hInfoRef.nWidth = txParam.GetAttribVal(strIdx, TEXT("nWidth")).GetAsInt(-1);
				hInfoRef.strName = txParam.GetAttribVal(strIdx, TEXT("strName")).GetAsString();
				lsConRef.vListHeader.push_back(hInfoRef);
			}
			txParam.OutofKey(); //out headerinfo
		}
		txParam.OutofKey(); // out listInfoManualTask
	}

	//auto list
	{
		ST_LISTCONFINFO& lsConRef = listInfoPlaylistTask;
		txParam.GoIntoKey(TEXT("listInfoPlaylistTask"));
		lsConRef.clrText = txParam.GetElemVal(TEXT("clrText")).GetAsUInt(RGB(0,0,0));
		lsConRef.clrTextBk = txParam.GetElemVal(TEXT("clrTextBk")).GetAsUInt(RGB(255,255,255));
		lsConRef.clrWarnText = txParam.GetElemVal(TEXT("clrWarnText")).GetAsUInt(RGB(0,200,200));
		lsConRef.clrWarnTextBk = txParam.GetElemVal(TEXT("clrWarnTextBk")).GetAsUInt(RGB(255,255,255));
		lsConRef.clrErrText = txParam.GetElemVal(TEXT("clrErrText")).GetAsUInt(RGB(200,0,0));
		lsConRef.clrErrTextBk = txParam.GetElemVal(TEXT("clrErrTextBk")).GetAsUInt(RGB(255,255,255));

		int nCount = txParam.GetAttribVal(TEXT("headerinfo"), TEXT("count")).GetAsInt();
		if (nCount > 0)
		{
			txParam.GoIntoKey(TEXT("headerinfo"));
			for(int i = 0; i < nCount; ++i)
			{
				ST_LISTHEADERITEM hInfoRef;
				CString strIdx;
				strIdx.Format(TEXT("col%d"), i);
				hInfoRef.nId = txParam.GetAttribVal(strIdx, TEXT("nId")).GetAsInt(-1);
				hInfoRef.nWidth = txParam.GetAttribVal(strIdx, TEXT("nWidth")).GetAsInt(-1);
				hInfoRef.strName = txParam.GetAttribVal(strIdx, TEXT("strName")).GetAsString();
				lsConRef.vListHeader.push_back(hInfoRef);
			}
			txParam.OutofKey(); //out headerinfo
		}
		txParam.OutofKey(); // out listInfoPlaylistTask
	}


	return TRUE;
}

BOOL ST_GLOBAL::ToString( CString& strOut )
{
	CTxParamString txParam(TEXT(""));
	txParam.SetElemVal(TEXT("owlconfig"), TEXT(""));
	txParam.GoIntoKey(TEXT("owlconfig"));
	txParam.SetElemVal(TEXT("nAutoLoopInterval"), nAutoLoopInterval);
	txParam.SetElemVal(TEXT("nTimelineClose"), nTimelineClose);
	txParam.SetElemVal(TEXT("nTimeLineOffline"), nTimeLineOffline);
	txParam.SetElemVal(TEXT("nMaxTaskCoutOnceCheck"), nMaxTaskCoutOnceCheck);
	txParam.SetElemVal(TEXT("strEmbTaskDBConn"), strEmbTaskDBConn);
	txParam.SetElemVal(TEXT("strPlayListCheckDBConn"), strPlayListCheckDBConn);
	txParam.SetElemVal(TEXT("strPlayListCheckDBConn2"), strPlayListCheckDBConn2);
	txParam.SetElemVal(TEXT("strPlaylistDBName"), strPlaylistDBName);
	txParam.SetElemVal(TEXT("nIdleHourStart"), nIdleHourStart);
	txParam.SetElemVal(TEXT("nIdleHourNum"), nIdleHourNum);
	txParam.SetElemVal(TEXT("nIdleTimelineClose"), nIdleTimelineClose);
	txParam.SetElemVal(TEXT("nIdleTimeLineOffline"), nIdleTimeLineOffline);
	txParam.SetElemVal(TEXT("chClipMachineID"), chClipMachineID);
	txParam.SetElemVal(TEXT("nDesFtpMaxsize"), nDesFtpMaxsize);
	txParam.SetElemVal(TEXT("nClipKeepDays"), nClipKeepDays);
	txParam.SetElemVal(TEXT("nAfdInfo"), nAfdInfo);
	txParam.SetElemVal(TEXT("nDefBitrate"), nDefBitrate);
	txParam.SetElemVal(TEXT("nHDBitRate"), nHDBitRate);
	txParam.SetElemVal(TEXT("strDefMediaType"), strDefMediaType);
	txParam.SetElemVal(TEXT("strDefChnID"), strDefChnID);
	txParam.SetElemVal(TEXT("nDefClipType"), nDefClipType);
	txParam.SetElemVal(TEXT("nDefClipSourceId"), nDefClipSourceId);

	txParam.SetElemVal(TEXT("nEmergencyPriority"), nEmergencyPriority);
	txParam.SetElemVal(TEXT("nEmbtaskRetryCount"), nEmbtaskRetryCount);

	txParam.SetElemVal(TEXT("bShowManualAlert"), bShowManualAlert);

	if (mapFtpSites.size() > 0)
	{
		txParam.SetElemVal(TEXT("mapFtpSites"), TEXT(""));
		txParam.GoIntoKey(TEXT("mapFtpSites"));
		txParam.SetAttribVal(NULL, TEXT("count"), mapFtpSites.size());
		MAPGPFTPSITES::iterator itb = mapFtpSites.begin();
		MAPGPFTPSITES::iterator ite = mapFtpSites.end();

		for (size_t i = 0; itb != ite; ++itb, ++i)
		{
			VECFTPSITES& vsiteRef = itb->second;
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.SetElemVal(strIdx, TEXT(""));
			txParam.GoIntoKey(strIdx);
			txParam.SetAttribVal(NULL, TEXT("key"), itb->first);
			MAPGROUPIDNAME::iterator itf = mapGroupIdName.find(itb->first);
			CString strGPName;
			if (itf != mapGroupIdName.end())
			{
				strGPName = itf->second;
			}
			txParam.SetAttribVal(NULL, TEXT("GroupName"), strGPName);
			txParam.SetAttribVal(NULL, TEXT("count"), vsiteRef.size());
			for (int iftp = 0; iftp < vsiteRef.size(); ++iftp)
			{
				ST_FTPSITEINFO& ftpRef = vsiteRef[iftp];
				CString strNum;
				strNum.Format(TEXT("site%d"), iftp);
				txParam.SetElemVal(strNum, TEXT(""));
				txParam.GoIntoKey(strNum);
				txParam.SetAttribVal(NULL, TEXT("strFtpName"), ftpRef.strFtpName);
				txParam.SetAttribVal(NULL, TEXT("strFtpIp"), ftpRef.strFtpIp);
				txParam.SetAttribVal(NULL, TEXT("nFtpPort"), ftpRef.nFtpPort);
				txParam.SetAttribVal(NULL, TEXT("strUser"), ftpRef.strUser);
				txParam.SetAttribVal(NULL, TEXT("strPw"), ftpRef.strPw);
				txParam.SetAttribVal(NULL, TEXT("nPassive"), ftpRef.nPassive);
				txParam.SetAttribVal(NULL, TEXT("strUncDir"), ftpRef.strUncDir);
				txParam.SetAttribVal(NULL, TEXT("strDBConn"), ftpRef.strDBConn);
				txParam.SetAttribVal(NULL, TEXT("strStoreName"), ftpRef.strStoreName);
				txParam.SetAttribVal(NULL, TEXT("nLocation"), ftpRef.nLocation);
				txParam.OutofKey();//out strNum;
			}

			txParam.OutofKey(); //out strIdx

		}
		txParam.OutofKey(); //out mapFtpSites
	}

	if (mapExternalCHId.size() > 0)
	{
		txParam.SetElemVal(TEXT("mapExternalCHId"), TEXT(""));
		txParam.GoIntoKey(TEXT("mapExternalCHId"));
		txParam.SetAttribVal(NULL, TEXT("count"), mapExternalCHId.size());
		MAPEXTERNALCHID::iterator itb = mapExternalCHId.begin();
		MAPEXTERNALCHID::iterator ite = mapExternalCHId.end();
		for (size_t i = 0; itb != ite; ++itb, ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.SetElemVal(strIdx, TEXT(""));
			txParam.SetAttribVal(strIdx, TEXT("key"), itb->first);
			txParam.SetAttribVal(strIdx, TEXT("val"), itb->second);
		}
		txParam.OutofKey();//out mapExternalCHId
	}

	if (mapExternalClipType.size() > 0)
	{
		txParam.SetElemVal(TEXT("mapExternalClipType"), TEXT(""));
		txParam.GoIntoKey(TEXT("mapExternalClipType"));
		txParam.SetAttribVal(NULL, TEXT("count"), mapExternalClipType.size());
		MAPEXTERNALCLIPTYPE::iterator itb = mapExternalClipType.begin();
		MAPEXTERNALCLIPTYPE::iterator ite = mapExternalClipType.end();
		for (size_t i = 0; itb != ite; ++itb, ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.SetElemVal(strIdx, TEXT(""));
			txParam.SetAttribVal(strIdx, TEXT("key"), itb->first);
			txParam.SetAttribVal(strIdx, TEXT("val"), itb->second);
		}
		txParam.OutofKey();//out mapExternalClipType
	}

	if (mapExternalClipSrcId.size() > 0)
	{
		txParam.SetElemVal(TEXT("mapExternalClipSrcId"), TEXT(""));
		txParam.GoIntoKey(TEXT("mapExternalClipSrcId"));
		txParam.SetAttribVal(NULL, TEXT("count"), mapExternalClipSrcId.size());
		MAPEXTERNALCLIPSOURCEID::iterator itb = mapExternalClipSrcId.begin();
		MAPEXTERNALCLIPSOURCEID::iterator ite = mapExternalClipSrcId.end();
		for (size_t i = 0; itb != ite; ++itb, ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("key%d"), i);
			txParam.SetElemVal(strIdx, TEXT(""));
			txParam.SetAttribVal(strIdx, TEXT("key"), itb->first);
			txParam.SetAttribVal(strIdx, TEXT("val"), itb->second);
		}
		txParam.OutofKey();//out mapExternalClipType
	}



	//////////////////////////////////////////////////////////////////////////
	//set list info
	//auto list
	{
		ST_LISTCONFINFO& lsConfRef = listInfoAutoTask;
		txParam.SetElemVal(TEXT("listInfoAutoTask"), TEXT(""));
		txParam.GoIntoKey(TEXT("listInfoAutoTask"));
		txParam.SetElemVal(TEXT("clrText"), lsConfRef.clrText);
		txParam.SetElemVal(TEXT("clrTextBk"), lsConfRef.clrTextBk);
		txParam.SetElemVal(TEXT("clrWarnText"), lsConfRef.clrWarnText);
		txParam.SetElemVal(TEXT("clrWarnTextBk"), lsConfRef.clrWarnTextBk);
		txParam.SetElemVal(TEXT("clrErrText"), lsConfRef.clrErrText);
		txParam.SetElemVal(TEXT("clrErrTextBk"), lsConfRef.clrErrTextBk);
		int nHeaderCount = lsConfRef.vListHeader.size();
		if (nHeaderCount > 0)
		{
			txParam.SetElemVal(TEXT("headerinfo"), TEXT(""));
			txParam.GoIntoKey(TEXT("headerinfo"));

			txParam.SetAttribVal(NULL, TEXT("count"), nHeaderCount);
			for (int  i= 0;  i< nHeaderCount ; ++i)
			{
				ST_LISTHEADERITEM& hInfoRef = listInfoAutoTask.vListHeader[i];
				CString strIdx;
				strIdx.Format(TEXT("col%d"), i);
				txParam.SetElemVal(strIdx, TEXT(""));
				txParam.SetAttribVal(strIdx, TEXT("nId"), hInfoRef.nId);
				txParam.SetAttribVal(strIdx, TEXT("nWidth"), hInfoRef.nWidth);
				txParam.SetAttribVal(strIdx, TEXT("strName"), hInfoRef.strName);
			}
			txParam.OutofKey();//out headerinfo
		}

		txParam.OutofKey();//out listconf
	}
	

	//manual list
	{
		ST_LISTCONFINFO& lsConfRef = listInfoManualTask;
		txParam.SetElemVal(TEXT("listInfoManualTask"), TEXT(""));
		txParam.GoIntoKey(TEXT("listInfoManualTask"));
		txParam.SetElemVal(TEXT("clrText"), lsConfRef.clrText);
		txParam.SetElemVal(TEXT("clrTextBk"), lsConfRef.clrTextBk);
		txParam.SetElemVal(TEXT("clrWarnText"), lsConfRef.clrWarnText);
		txParam.SetElemVal(TEXT("clrWarnTextBk"), lsConfRef.clrWarnTextBk);
		txParam.SetElemVal(TEXT("clrErrText"), lsConfRef.clrErrText);
		txParam.SetElemVal(TEXT("clrErrTextBk"), lsConfRef.clrErrTextBk);
		int nHeaderCount = lsConfRef.vListHeader.size();
		if (nHeaderCount > 0)
		{
			txParam.SetElemVal(TEXT("headerinfo"), TEXT(""));
			txParam.GoIntoKey(TEXT("headerinfo"));

			txParam.SetAttribVal(NULL, TEXT("count"), nHeaderCount);
			for (int  i= 0;  i< nHeaderCount ; ++i)
			{
				ST_LISTHEADERITEM& hInfoRef = listInfoManualTask.vListHeader[i];
				CString strIdx;
				strIdx.Format(TEXT("col%d"), i);
				txParam.SetElemVal(strIdx, TEXT(""));
				txParam.SetAttribVal(strIdx, TEXT("nId"), hInfoRef.nId);
				txParam.SetAttribVal(strIdx, TEXT("nWidth"), hInfoRef.nWidth);
				txParam.SetAttribVal(strIdx, TEXT("strName"), hInfoRef.strName);
			}
			txParam.OutofKey();//out headerinfo
		}

		txParam.OutofKey();//out listconf
	}


	//play list
	{
		ST_LISTCONFINFO& lsConfRef = listInfoPlaylistTask;
		txParam.SetElemVal(TEXT("listInfoPlaylistTask"), TEXT(""));
		txParam.GoIntoKey(TEXT("listInfoPlaylistTask"));
		txParam.SetElemVal(TEXT("clrText"), lsConfRef.clrText);
		txParam.SetElemVal(TEXT("clrTextBk"), lsConfRef.clrTextBk);
		txParam.SetElemVal(TEXT("clrWarnText"), lsConfRef.clrWarnText);
		txParam.SetElemVal(TEXT("clrWarnTextBk"), lsConfRef.clrWarnTextBk);
		txParam.SetElemVal(TEXT("clrErrText"), lsConfRef.clrErrText);
		txParam.SetElemVal(TEXT("clrErrTextBk"), lsConfRef.clrErrTextBk);
		int nHeaderCount = lsConfRef.vListHeader.size();
		if (nHeaderCount > 0)
		{
			txParam.SetElemVal(TEXT("headerinfo"), TEXT(""));
			txParam.GoIntoKey(TEXT("headerinfo"));

			txParam.SetAttribVal(NULL, TEXT("count"), nHeaderCount);
			for (int  i= 0;  i< nHeaderCount ; ++i)
			{
				ST_LISTHEADERITEM& hInfoRef = listInfoPlaylistTask.vListHeader[i];
				CString strIdx;
				strIdx.Format(TEXT("col%d"), i);
				txParam.SetElemVal(strIdx, TEXT(""));
				txParam.SetAttribVal(strIdx, TEXT("nId"), hInfoRef.nId);
				txParam.SetAttribVal(strIdx, TEXT("nWidth"), hInfoRef.nWidth);
				txParam.SetAttribVal(strIdx, TEXT("strName"), hInfoRef.strName);
			}
			txParam.OutofKey();//out headerinfo
		}
		txParam.OutofKey();//out listconf
	}


	txParam.UpdateData();
	strOut = txParam;

	return TRUE;
}


BOOL LoadSectionInfo()
{
	g_GlobalInfo.vChnInfo.clear();
	CTxADORecordSet rst(DBKEY_PLAYLIST);
	CString strSql = TEXT("select strChnID, strChnName from T_CHANNEL");
	if (!rst.ExecuteSQL(strSql))
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("sql exc error: %s"), strSql);
		return FALSE;
	}

	while(!rst.IsEOF())
	{
		STCHNSECTIONINFO chnInfo;

		chnInfo.strChnId = rst.GetVal(TEXT("strChnID")).GetAsString();
		chnInfo.strChnName = rst.GetVal(TEXT("strChnName")).GetAsString();
		g_GlobalInfo.vChnInfo.push_back(chnInfo);
		rst.MoveNext();
	}

	for (size_t i = 0; i < g_GlobalInfo.vChnInfo.size(); ++i)
	{
		STCHNSECTIONINFO& chnInfoRef = g_GlobalInfo.vChnInfo[i];
		CString strSqlSec;
		strSqlSec.Format(TEXT("select * from t_SectionProperty where strChnID = '%s' order by nSectionIndex"), chnInfoRef.strChnId);
		if (!rst.ExecuteSQL(strSqlSec))
		{
			ASSERT(FALSE);
			CFWriteLog(0, TEXT("sql exc error:%s"), strSqlSec);
		}
		else
		{
			while(!rst.IsEOF())
			{
				STSECTION secInfo;
				secInfo.strSecId = rst.GetVal(TEXT("strSectionGuid")).GetAsString();
				secInfo.strSecName = rst.GetVal(TEXT("strSectionName")).GetAsString();
				int nWeekDay = rst.GetVal(TEXT("nWeekDay")).GetAsInt(-1);
				if (!secInfo.strSecId.IsEmpty() &&  nWeekDay >= 0 && nWeekDay < 7)
				{
					chnInfoRef.vWeekSections[nWeekDay].vSections.push_back(secInfo);
				}
				rst.MoveNext();
			}
		}

	}

	return TRUE;

}

CString GetSectionName( LPCTSTR szSecId, VECSECTIONS& vSecIn )
{
	if (szSecId == NULL)
	{
		return TEXT("");
	}

	for (size_t i = 0; i < vSecIn.size(); ++i)
	{
		if (vSecIn[i].strSecId.Compare(szSecId) == 0)
		{
			return vSecIn[i].strSecName;
		}
	}

	return TEXT("");
}

CString GetChnName( LPCTSTR szChId )
{
	if (szChId == NULL)
	{
		return TEXT("");
	}

	BOOL bFind = FALSE;
	for (size_t i = 0; i < g_GlobalInfo.vChnInfo.size(); ++i)
	{
		if (g_GlobalInfo.vChnInfo[i].strChnId.Compare(szChId) == 0)
		{
			return g_GlobalInfo.vChnInfo[i].strChnName;
		}

	}

	return TEXT("");
}

CString GetFtpListColText(const int nColTypeIn, int nGroupId, ST_FTPSITEINFO& ftpIn )
{
	CString strRet;
	if (nColTypeIn == colftp_group)
	{
		return FtpGroupID2Str(nGroupId);
	}
	else if (colftp_ip == nColTypeIn)
	{
		return ftpIn.strFtpIp;
	}
	else if (colftp_port == nColTypeIn)
	{
		return CTxStrConvert(ftpIn.nFtpPort).GetAsString();
	}
	else if (colftp_user == nColTypeIn)
	{
		return ftpIn.strUser;
	}
	else if (colftp_pw == nColTypeIn)
	{
		return ftpIn.strPw;
	}
	else if (colftp_name == nColTypeIn)
	{
		return ftpIn.strFtpName;
	}
	else if (colftp_storename == nColTypeIn)
	{
		return ftpIn.strStoreName;
	}
	else if (colftp_mode == nColTypeIn)
	{
		return ftpIn.nPassive == 0? TEXT("postive"):TEXT("passive");
	}

	return strRet;
}

CString FtpGroupID2Str( int nGroupId )
{
	MAPGROUPIDNAME::iterator itf = g_GlobalInfo.mapGroupIdName.find(nGroupId);
	if (itf != g_GlobalInfo.mapGroupIdName.end())
	{
		return itf->second;
	}

	return TEXT("");
}

CString ExternalChId2LocalChid( CString& strExternalChid )
{
	if (strExternalChid.IsEmpty())
	{
		return g_GlobalInfo.strDefChnID;
	}

	MAPEXTERNALCHID::iterator itf = g_GlobalInfo.mapExternalCHId.find(strExternalChid);
	if (itf != g_GlobalInfo.mapExternalCHId.end())
	{
		return itf->second;
	}
	return g_GlobalInfo.strDefChnID;
}

BOOL IsIdleHour()
{
	if (g_GlobalInfo.nIdleHourNum = -1 || g_GlobalInfo.nIdleHourStart > 23
		||g_GlobalInfo.nIdleHourStart < 0)
	{
		return FALSE;
	}

	CTime tmNow(time(NULL));
	int nHour = tmNow.GetHour();
	if (g_GlobalInfo.nIdleHourStart + g_GlobalInfo.nIdleHourNum <= 24)
	{
		return nHour >= g_GlobalInfo.nIdleHourStart && nHour < (g_GlobalInfo.nIdleHourStart + g_GlobalInfo.nIdleHourNum);
	}
	else
	{
		return nHour >= g_GlobalInfo.nIdleHourStart || nHour < (g_GlobalInfo.nIdleHourStart + g_GlobalInfo.nIdleHourNum -24);
	}
}

int ExternalClipType2Local( CString& szExternalClipType )
{
	if (szExternalClipType.IsEmpty())
	{
		return g_GlobalInfo.nDefClipType;
	}
	MAPEXTERNALCLIPTYPE::iterator itf = g_GlobalInfo.mapExternalClipType.find(szExternalClipType);
	if (itf != g_GlobalInfo.mapExternalClipType.end())
	{
		return itf->second;
	}
	else
	{
		return g_GlobalInfo.nDefClipType;
	}
}

int ExternalClipSourceId2Local( CString& szExternalClipSrcId )
{
	if (szExternalClipSrcId.IsEmpty())
	{
		return g_GlobalInfo.nDefClipSourceId;
	}
	MAPEXTERNALCLIPSOURCEID::iterator itf = g_GlobalInfo.mapExternalClipSrcId.find(szExternalClipSrcId);
	if (itf != g_GlobalInfo.mapExternalClipSrcId.end())
	{
		return itf->second;
	}
	else
	{
		return  g_GlobalInfo.nDefClipSourceId;
	}

}

int GetListColPos( VECLSTHEADERS& vHeaders, int nColId )
{
	int nRet = -1;
	for (int i = 0; i < vHeaders.size(); i++)
	{
		if (nColId == vHeaders[i].nId)
		{
			nRet = i;
			break;
		}
	}

	return nRet;
}
