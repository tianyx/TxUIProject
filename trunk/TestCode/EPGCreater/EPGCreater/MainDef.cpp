#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "TxParamString.h"
#include "TxADOConn.h"

// 全局信息存储变量？尽量少用全局变量
ST_GLOBAL g_GlobalInfo;

/*
*Description：初始化配置文件
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL InitGlobalConfig()
{
	//策略管理配置文件
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EPGCreater.ini");
	//日志配置文件
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\EPGCreater.log");
	g_GlobalInfo.szEpgDatafile = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szEpgDatafile += TEXT("\\taskData.dat");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		CFWriteLog("找不到配置文件EPGCreater.xml！");
		return FALSE;
	}

	char szbuff[1024];
	ZeroMemory(szbuff, sizeof(szbuff));
	GetPrivateProfileString("config", "db", "", szbuff, 1024, g_GlobalInfo.szIniPath);
	g_GlobalInfo.szDBConnString = szbuff;
	if (g_GlobalInfo.szDBConnString.IsEmpty())
	{
		CFWriteLog(TEXT("数据库连接字符串错误"));
		return FALSE;
	}

	g_GlobalInfo.nAutoCheckInterval = GetPrivateProfileInt("config", "AutoCheckInterval", 10*60, g_GlobalInfo.szIniPath);

	return TRUE;
}

BOOL LoadSectionInfo()
{
	g_GlobalInfo.vChnInfo.clear();
	CTxADORecordSet rst;
	CString strSql = TEXT("select strChnID, strChnName from T_CHANNEL");
	if (!rst.ExecuteSQL(strSql))
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("sql exc error: %s"), strSql);
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
			CFWriteLog(TEXT("sql exc error:%s"), strSqlSec);
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

BOOL GetChnInfo( LPCTSTR szChId, STCHNSECTIONINFO& out )
{
	if (szChId == NULL)
	{
		return FALSE;
	}

	BOOL bFind = FALSE;
	for (size_t i = 0; i < g_GlobalInfo.vChnInfo.size(); ++i)
	{
		if (g_GlobalInfo.vChnInfo[i].strChnId.Compare(szChId) == 0)
		{
			out = g_GlobalInfo.vChnInfo[i];
			bFind = TRUE;
			break;
		}

	}

	return bFind;
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

CTimeSpan TimeCodeToSpan( INT64 nTimeCodeIn )
{
	ASSERT(nTimeCodeIn >= 0);
	INT64 F = nTimeCodeIn;
	int ff = (F % 25);	F /=25;
	int ss = (F % 60);	F /=60;
	int mm = (F % 60);	F /=60;
	int hh = F%24; F/=24;
	INT dd = F;
	CTimeSpan spRet(dd, hh, mm,ss);
	return spRet;
}

CString GetChnNum(const CString& strChnIDIn )
{
	CString strChid;
	CString strChidOrg = strChnIDIn;
	for (int i = 0; i < strChidOrg.GetLength(); ++i)
	{
		if (strChidOrg.GetAt(i) >='0' && strChidOrg.GetAt(i) <='9')
		{
			strChid.AppendChar(strChidOrg.GetAt(i));
		}
	}
	
	return strChid;
}

#define EK_MAIN TEXT("EPGTasks")
#define EA_MAIN_TASKNUM TEXT("num")

BOOL STEPGTASKS::ToString( CString& strDataOut )
{
	CTxParamString sParam(TEXT(""));
	sParam.SetElemVal(EK_MAIN, TEXT(""));
	sParam.SetAttribVal(EK_MAIN, EA_MAIN_TASKNUM, vTasks.size());
	sParam.GoIntoKey(EK_MAIN);

	for (size_t i =0; i < vTasks.size(); ++i)
	{
		STEPGTASK& taskRef = vTasks[i];
		CString strTaskName;
		strTaskName.Format(TEXT("task%d"), i);
		sParam.SetElemVal(strTaskName, TEXT(""));
		sParam.SetAttribVal(strTaskName, TEXT("strDesc"), taskRef.strDesc);
		sParam.SetAttribVal(strTaskName, TEXT("bEnableTimer"), taskRef.bEnableTimer);
		sParam.SetAttribVal(strTaskName, TEXT("nOffsetDay"), taskRef.nOffsetDay);
		sParam.SetAttribVal(strTaskName, TEXT("tTimeToGen"), taskRef.tTimeToGen);
		sParam.SetAttribVal(strTaskName, TEXT("bMustSubmitted"), taskRef.bMustSubmitted);
		sParam.SetAttribVal(strTaskName, TEXT("nWeekLoopMode"), taskRef.nWeekLoopMode);
		sParam.SetAttribVal(strTaskName, TEXT("chnNum"), taskRef.vChnTask.size());

		sParam.GoIntoKey(strTaskName);
		for (size_t ich = 0; ich < taskRef.vChnTask.size(); ++ich)
		{
			CString strChnName;
			strChnName.Format(TEXT("chn%d"), ich);
			sParam.SetElemVal( strChnName, TEXT(""));
			sParam.SetAttribVal(strChnName, TEXT("strChId"), taskRef.vChnTask[ich].strChId);

			ST_ONECHN& oneChnRef =  taskRef.vChnTask[ich];
			sParam.SetAttribVal(strChnName, TEXT("dayNum"), oneChnRef.vWeekDays.size());
			sParam.GoIntoKey(strChnName);
			for (size_t j = 0; j < oneChnRef.vWeekDays.size(); ++j)
			{
				CString strDayName;
				strDayName.Format(TEXT("Day%d"), j);
				sParam.SetElemVal(strDayName, TEXT(""));
				sParam.SetAttribVal(strDayName, TEXT("bEnable"), oneChnRef.vWeekDays[j].bEnable);
				sParam.SetAttribVal(strDayName, TEXT("nDay"), oneChnRef.vWeekDays[j].nDay);
				sParam.SetAttribVal(strDayName, TEXT("strSecID"), oneChnRef.vWeekDays[j].strSecID);
			}
			sParam.OutofKey();

		}
		
		sParam.OutofKey();
	}
	sParam.UpdateData();
	strDataOut = sParam;
	return TRUE;
}

BOOL STEPGTASKS::FromString( LPCTSTR strDataIn )
{
	if (strDataIn == NULL)
	{
		return FALSE;
	}
	vTasks.clear();

	CTxParamString sParam(strDataIn);
	sParam.GoIntoKey(EK_MAIN);
	int nTaskSize = sParam.GetAttribVal(NULL, EA_MAIN_TASKNUM).GetAsInt();
	for (int i = 0; i < nTaskSize; ++i)
	{
		STEPGTASK task;
		CString strTaskName;
		strTaskName.Format(TEXT("task%d"), i);
		sParam.GoIntoKey(strTaskName);
		task.bEnableTimer = sParam.GetAttribVal(NULL, TEXT("bEnableTimer")).GetAsBOOL(TRUE);
		task.nOffsetDay= sParam.GetAttribVal(NULL, TEXT("nOffsetDay")).GetAsInt(3);
		task.tTimeToGen= sParam.GetAttribVal(NULL, TEXT("tTimeToGen")).GetAsInt(0);
		task.bMustSubmitted= sParam.GetAttribVal(NULL, TEXT("bMustSubmitted")).GetAsBOOL(FALSE);
		task.nWeekLoopMode= sParam.GetAttribVal(NULL, TEXT("nWeekLoopMode")).GetAsInt();

		task.strDesc = sParam.GetAttribVal(NULL, TEXT("strDesc")).GetAsString();
		int nChnSize = sParam.GetAttribVal(NULL, TEXT("chnNum")).GetAsInt();
		for (int ich = 0; ich < nChnSize; ++ich)
		{
			CString strChnName;
			strChnName.Format(TEXT("chn%d"), ich);
			sParam.GoIntoKey(strChnName);
			ST_ONECHN oneChn;
			oneChn.strChId = sParam.GetAttribVal(NULL, TEXT("strChId")).GetAsString();

			int nDaySize = sParam.GetAttribVal(NULL, TEXT("dayNum")).GetAsInt(0);
			for (int j = 0; j < nDaySize; ++j)
			{
				CString strDayName;
				strDayName.Format(TEXT("Day%d"), j);
				STWEEKDAYINFO dayInfo;
				dayInfo.nDay = sParam.GetAttribVal(strDayName, TEXT("nDay")).GetAsInt(-1);
				dayInfo.strSecID = sParam.GetAttribVal(strDayName, TEXT("strSecID")).GetAsString();
				dayInfo.bEnable = sParam.GetAttribVal(strDayName, TEXT("bEnable")).GetAsInt(0);
				oneChn.vWeekDays.push_back(dayInfo);
			}
			task.vChnTask.push_back(oneChn);
			sParam.OutofKey();
		}
		
		sParam.OutofKey();
		vTasks.push_back(task);
	}

	return TRUE;
}
