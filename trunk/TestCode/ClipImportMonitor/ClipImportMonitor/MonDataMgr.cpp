/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	MonDataMgr.cpp
	file base:	MonDataMgr
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "MonDataMgr.h"
#include "CODEX20/Codex_ADO.h"
#include "TrCommonInfoMgr.h"
#include <algorithm>


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//param num = m_nMonTimeOutMinutes, m_nMonDays, m_vMonChannels list, m_vMonChannels list
CString g_strSqlGetDataFmt = "\
DECLARE @BeginDay datetime; \
DECLARE @EndDay datetime; \
select @BeginDay = DATEADD(minute, -%d, convert(datetime, convert(varchar(10),getdate(),101), 101)); \
select @EndDay= DATEADD(day, %d, @BeginDay); \
select t_PrepareTrans.ProgramID as ProgramID,\
t_PrepareTrans.ProgramName as ProgramName,\
t_PrepareTrans.ChannelID as ChannelID, \
CONVERT(varchar(64),t_PrepareTrans.TotalLength) as TotalLength, \
t_PrepareTrans.FirstTime as FirstTime, \n\
--P1.nClipTypeID as Type, \n\
2 as Type, \
t_PrepareTrans.Status as Status \
from t_PrepareTrans \n\
--right join  T_CLIPTYPECompare as P1 on P1.strMZTypeName = t_PrepareTrans.Type \n\
where (t_PrepareTrans.ProgramID IN \
	   (select t_BIDMapCompile.strPrimaryID from t_BIDMapCompile where t_BIDMapCompile.strPGMGuid IN \
	   (select t_PlaylistCompile.strPGMGuid from t_PlaylistCompile where t_PlaylistCompile.strPSID IN \
	   (\
	   select t_ProgramSheet.strPSID from t_ProgramSheet where t_ProgramSheet.strChnID IN \
	   (%s) AND t_ProgramSheet.nState = 2 and t_ProgramSheet.tPlayDate >=@BeginDay AND t_ProgramSheet.tPlayDate <=@EndDay \
	   ) \
	   ) \
	   ) \
	   AND  t_PrepareTrans.ProgramID NOT IN (select T_BVSID.strClipLogicID from T_BVSID) \
	   ) \
union \
select T0.strPrimaryID as ProgramID, \
T0.strClipName as ProgramName, \
T2.strChnID as ChannelID, \
T0.strDuration as TotalLength, \
DATEADD(second, (T1.nPGMSchStartTime)/1000, T1.tPGMSchPlayDate) as FirstTime, \
CONVERT(varchar(32), T0.nClipType) as Type, \
convert(int, 6666) as Status \
from \
t_BIDMapCompile as T0  right join t_PlaylistCompile as T1 on T0.strPGMGuid = T1.strPGMGuid right join t_ProgramSheet  as T2 on T1.strPSID = T2.strPSID \
where (T0.strPrimaryID NOT IN (select T_BVSID.strClipLogicID from T_BVSID) \
	   AND T0.strPrimaryID NOT IN (select t_PrepareTrans.ProgramID from t_PrepareTrans) \
	   AND  T0.strPGMGuid IN \
	   (select t_PlaylistCompile.strPGMGuid from t_PlaylistCompile where t_PlaylistCompile.strPSID IN \
	   ( \
	   select t_ProgramSheet.strPSID from t_ProgramSheet where t_ProgramSheet.strChnID IN \
	   (%s) AND t_ProgramSheet.nState = 2 and t_ProgramSheet.tPlayDate >=@BeginDay AND t_ProgramSheet.tPlayDate <=@EndDay \
	   ) \
	   ) \
	   ) \
	   order by FirstTime";

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CMonDataMgr::CMonDataMgr(void):m_lock(FALSE, TEXT("{BEB50095-6488-4a93-9785-FEA9D5A2DFC5}"))
{
	m_bInited = FALSE;
	m_hThreadGetData = NULL;
	m_hwndOb = NULL;
	m_lsData.reserve(500);
	m_vmonState.reserve(9);
	m_nMonDays = 5;
	m_nMonTimeOutMinutes = 10;
	m_nCheckInterval = 30000; //ns
	m_nOffsetSafeMinute = 5;
	m_nAlertCheckInterval = 10; //second
	m_nOffsetSafeSecond = m_nOffsetSafeMinute* 60;
	m_nMonTimeOutSecond = m_nMonTimeOutMinutes * 60;
}

CMonDataMgr::~CMonDataMgr(void)
{
	if (m_hThreadGetData)
	{
		TerminateThread(m_hThreadGetData, 0);
		m_hThreadGetData = NULL;
	}
}

BOOL CMonDataMgr::Init()
{
	m_bInited = ConnectSrv();
	if (!m_bInited)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_bInited = LoadConfig();
	if (!m_bInited)
	{
		ASSERT(FALSE);
		return FALSE;
	}

#ifndef _CIMTEST
	CString strMonCh;
	for (int i = 0; i < m_vMonChannels.size(); i++)
	{
		if (i != 0)
		{
			strMonCh+=",";
		}
		strMonCh +=  "'" +m_vMonChannels[i] +"'";
	}
	m_strSqlOnline.Format(g_strSqlGetDataFmt, m_nMonTimeOutMinutes, m_nMonDays, strMonCh, strMonCh);
	g_log.Print("sql = %s", m_strSqlOnline);
#endif

	m_hThreadGetData =  CreateThread(NULL, 0, ClockThreadProc, (void*)this, 0, NULL);
	return m_bInited;
}

BOOL CMonDataMgr::ConnectSrv()
{
	CString ConnectionStrSQLSVR;
	CResString DatabaseCfg("/etc/iip/Environment");
	CString strConnectionType = DatabaseCfg["Default"].GetAsString();
	m_dbURL.LoadFromStringForSql(DatabaseCfg["DataBase"].GetAsString());
	if ( strConnectionType.CompareNoCase( "Oracle" ) == 0 )
	{

		ConnectionStrSQLSVR = Coco::CADOConnectLE::MakeOrclConnectionString(
			CString(m_dbURL.m_strServer),
			CString(m_dbURL.m_strFile),
			CString(m_dbURL.m_strUserID),
			CString(m_dbURL.m_strUserPsw)		
			);
	}
	else //if(strConnectionType.CompareNoCase( "SqlServer" ) == 0 )
	{
		ConnectionStrSQLSVR = Coco::CADOConnectLE::MakeSqlConnectionString(
			CString(m_dbURL.m_strServer),
			CString(m_dbURL.m_strFile),
			CString(m_dbURL.m_strUserID),
			CString(m_dbURL.m_strUserPsw)	
			);
	}
	
	strConnectionType = strConnectionType.MakeUpper();
	m_ADOConnectSQL.SetConnectionString(ConnectionStrSQLSVR);
	m_ADOConnectSQL.SetDBConnName(strConnectionType);
	m_ADOConnectSQL.SetAsDefault();
	if(!m_ADOConnectSQL.OpenDBConnect(0,0))
	{
		AfxMessageBox("连接数据库失败");
		TxMACRO_WriteLog(TEXT("database connerror"), DatabaseCfg["DataBase"].GetAsString() );
		return FALSE;
	}		


	if (!LoadChannelInfo())
	{
		ASSERT(FALSE);
		return FALSE;
	}

#ifdef _CIMTEST
	BOOL bReadSql = TRUE;
	CFile file1;
	if (file1.Open(GetFullFilePath(TEXT("sqlonline.sql")),CFile::modeRead, NULL))
	{
		int nLen = file1.GetLength();
		char* szbuff = new char[nLen+1];
		file1.Read(szbuff, nLen);
		szbuff[nLen] = '\0';
		m_strSqlOnline = szbuff;
		delete szbuff;
		szbuff = NULL;
	}
	else
	{
		bReadSql = FALSE;
	}

	if (!bReadSql)
	{
		AfxMessageBox("读取查询失败");
		return FALSE;
	}
#endif


	return TRUE;
}

BOOL CMonDataMgr::LoadChannelInfo()
{
	CString strSql = TEXT("select * from T_CHANNEL");
	Coco::CADORecordsetLE rsData;
	if(rsData.ExecuteSQL(strSql) != 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	while(!rsData.IsRSEOF())
	{
		CString strChId = rsData.GetAsString(TEXT("strChnID"));
		CString strChName= rsData.GetAsString(TEXT("strChnName"));
		if (!strChId.IsEmpty() || strChName.IsEmpty())
		{
			m_mapChannels[strChId]= strChName;
		}
		rsData.RSMoveNext();
	}
	rsData.CloseRecordset();
	return TRUE;
}

BOOL CMonDataMgr::GetDataFromDB()
{
	if (!m_bInited || m_strSqlOnline.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//Get data online
	Coco::CADORecordsetLE rsData;
	if(rsData.ExecuteSQL(m_strSqlOnline) != 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	VECTMPITEM lsData;
	int nOrder = 1;
	CTime tmNow(time(NULL));
	if (m_nMonTimeOutMinutes != 0)
	{
		CTimeSpan tmTimeOutSpan(0,0,m_nMonTimeOutMinutes, 0);
		tmNow -= tmTimeOutSpan;
	}
	while(!rsData.IsRSEOF())
	{
		const VECSTRING& vDBCols =g_CoInfoMgr.GetTemplateDBColName();
		ASSERT(vDBCols.size() == (TEMPLATE_DB_COL_NUM));
		CTrMonItem item;

		CString strStateInTask = 0;
		BOOL bOutTimed = FALSE;
		//skip first order number
		for (size_t i = 1; i < vDBCols.size(); i++)
		{
			ENUM_TEMPCOL_TYPE nType = (ENUM_TEMPCOL_TYPE)i;
			if (nType == TRCOLTYPE_FIRSTIME)
			{
				CTime tm = rsData.GetAsCTime(vDBCols[i]);
				if (tm < tmNow)
				{
					bOutTimed = TRUE;
					break;
				}
				item.DirectSetValue(nType, tm.GetTime());
			}
			else if (nType == TRCOLTYPE_TOTALLENTH)
			{
				CString strLen = rsData.GetAsString(vDBCols[i]);
				if (strLen.Find(':') != -1)
				{
					//timecode mode
					item.DirectSetValue(nType, rsData.GetAsString(vDBCols[i]));
				}
				else
				{
					char buff[24];
					ZeroMemory(buff, 24);

					//frame mode
					UINT nTotalLen = rsData.GetAsInt(vDBCols[i]);
					timecode_t tcode(nTotalLen);
					tcode.GetAsString(buff);
					item.DirectSetValue(nType, buff);
				}
			}
			else
			{
				item.DirectSetValue(nType, rsData.GetAsString(vDBCols[i]));
			}
		}

		if (!bOutTimed)
		{
			item.DirectSetValue(TRCOLTYPE_INDEX, nOrder);
			//check duplicate
			BOOL bFind = FALSE;
			for (int i = 0; i < lsData.size(); i++)
			{
				if (lsData[i].IsSamePGMID(item))
				{
					bFind = TRUE;
					break;
				}
			}

			if (!bFind)
			{
				lsData.push_back(item);
				nOrder++;
			}
		}
		rsData.RSMoveNext();
	}
	rsData.CloseRecordset();


	m_lock.Lock(3000);
	m_lsData.clear();
	m_lsData = lsData;
	m_lock.Unlock();
	//getdata offline
	return TRUE;
}

void CMonDataMgr::NotifyView()
{
	::PostMessage(m_hwndOb, MSG_MONDATACHANGED, WPARAM(m_lsData.size()),0);
}

BOOL CMonDataMgr::CallRequest()
{
	if (m_bInited)
	{
		if (GetDataFromDB())
		{
			NotifyView();
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CMonDataMgr::RegisterObserverWnd( HWND hwndIn )
{
	if (m_hwndOb != NULL)
	{
		ASSERT(FALSE);
	}
	m_hwndOb = hwndIn;

	return TRUE;
}

int CMonDataMgr::GetRangeData( int nBegin, int nCount, VECTMPITEM& vOut)
{
	if (!m_bInited || m_lsData.size() <= nBegin)
	{
		return 0;
	}
	m_lock.Lock(2000);
	ASSERT(nBegin >= 0);
	int nSize = m_lsData.size();
	if (nBegin + nCount >m_lsData.size())
	{
		//ASSERT(FALSE);
		nCount = nSize - nBegin;
	}
	
	for (int i = nBegin; i< nBegin+nCount; i++)
	{
		vOut.push_back(m_lsData[i]);
	}

	m_lock.Unlock();

	return nCount;
}

bool MonStateSortFn(ST_MONSTATEPARAM& e1, ST_MONSTATEPARAM& e2)
{
	return e1.nTransState < e2.nTransState;
}
BOOL CMonDataMgr::LoadConfig()
{
	Coco::CResString cfgs("/etc/iip/Environment");
	CStringArray arrCh;
	cfgs["MonitorChannle"].GetAsStringArray(arrCh);
	if (arrCh.GetSize() <= 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	for (int i = 0; i < arrCh.GetSize(); i++)
	{
		m_vMonChannels.push_back(arrCh[i]);
	}

	m_nMonTimeOutMinutes = cfgs["MonitorTimeout"].GetAsInteger(0);
	m_nMonTimeOutSecond = m_nMonTimeOutMinutes * 60;
	m_nCheckInterval = cfgs["MonitorInterval "].GetAsInteger(30) * 1000;
	if (m_nCheckInterval <=1000)
	{
		ASSERT(FALSE);
		m_nCheckInterval = 10000;
	}

	m_nCloseDoorMinute = cfgs["MonitorCloseTime"].GetAsInteger(60);
	m_nCloseDoorSecond = m_nCloseDoorMinute* 60;
	m_nOffsetSafeMinute = cfgs["MonitorSafeRange"].GetAsInteger(5);
	m_nOffsetSafeSecond = m_nOffsetSafeMinute * 60;
	m_nMonDays = cfgs["MonitorDays"].GetAsInteger(5);
	if(arrCh.GetSize() ==0)
	{
		ASSERT(FALSE);
		AfxMessageBox("没有获取到监视频道列表");
		return FALSE;
	}

	Coco::CResString Setting("/etc/iip/MonState");
	CStringArray strArrColName;
	Coco::CXmlProp xmlColumn;

	Setting["StateList"].GetAsStringArray(strArrColName);

	int nColSize = strArrColName.GetSize();
	if (nColSize <= 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	for(int i = 0; i < nColSize; i++)
	{
		//predefined in constructor.
		//m_vTmpViewConfig.m_vDBTmpCols.push_back(strArrColName[i]);
		CString strCol = Setting[strArrColName[i]].AsString;
		if (strCol.IsEmpty())
		{
			continue;
		}
		xmlColumn = strCol;	
		ST_MONSTATEPARAM tmpInfo;
		int nPos = xmlColumn["TypePos"].GetAsInteger((int)TRANSSTATE_UNKNOW);
		ASSERT(nPos < (int)TRANSSTATE_UNKNOW);
		tmpInfo.dwColor = xmlColumn["color"].GetAsARGB(0xff000000);
		tmpInfo.dwBkColor = xmlColumn["bkColor"].GetAsARGB(0xffffffff);
		tmpInfo.strTitle = xmlColumn["title"].GetAsString("");
		tmpInfo.strDBValue = strArrColName[i];
		tmpInfo.nTransState = (ENUM_TRANSSTATE)nPos;
		m_vmonState.push_back(tmpInfo);
		m_mapMonState[strArrColName[i]] = tmpInfo;
	}

	std::sort(m_vmonState.begin(), m_vmonState.end(), MonStateSortFn);

	//////////////////////////////////////////////////////////////////////////
	//load list font setting
	//for all
	g_FontLoader.AddFont(g_logfont);

	Coco::CResString gSetting("/etc/Gui/GlobalSetting");
	
	CString strFamily = gSetting["SysFontFace"].GetAsString("宋体");
	int nHeight = gSetting["SysFontHeight"].GetAsInteger(20);
	g_FontLoader.AddFont(strFamily, nHeight);

	g_globalInfo.gSetting.dwPen = gSetting["dwClrPen"].GetAsARGB(0xff000000);
	g_globalInfo.gSetting.dwBrush = gSetting["dwClrBursh"].GetAsARGB(0xff000000);


	Coco::CResString capSetting("/etc/Gui/CaptionSetting");

	//FOR caption
	g_globalInfo.captionSetting.strFontFaceTime = capSetting["strFontFaceTime"].GetAsString("宋体");
	g_globalInfo.captionSetting.nFontHeightTime = capSetting["nFontHeightTime"].GetAsInteger(20);
	g_globalInfo.captionSetting.dwClrBursh = capSetting["dwClrBursh"].GetAsARGB(0xffffffff);
	g_globalInfo.captionSetting.dwClrBurshTime = capSetting["dwClrBurshTime"].GetAsARGB(0xff00ffff);
	g_globalInfo.captionSetting.dwClrPen = capSetting["dwClrPen"].GetAsARGB(0x0);
	g_globalInfo.captionSetting.dwClrPenTime = capSetting["dwClrPenTime"].GetAsARGB(0xa0ff00ffff);

	//////////////////////////////////////////////////////////////////////////
	//load view setting
	Coco::CResString viewSetting("/etc/Gui/ViewSetting");
	g_globalInfo.viewSetting.clrBk = viewSetting["bkColor"].GetAsARGB(g_globalInfo.viewSetting.clrBk);
	g_globalInfo.viewSetting.clrSeparateLine = viewSetting["lineColor"].GetAsARGB(g_globalInfo.viewSetting.clrSeparateLine);
	g_globalInfo.viewSetting.nItemHeight = viewSetting["ItemHeight"].GetAsInteger(64);
	g_globalInfo.viewSetting.bHeaderFitClient  = viewSetting["bHeaderFitClient"].GetAsBool(TRUE);
	g_globalInfo.viewSetting.bEnableHBar  = viewSetting["bEnableHBar"].GetAsBool(TRUE);
	g_globalInfo.viewSetting.strFontFace  = viewSetting["FontFace"].GetAsString("宋体");
	g_globalInfo.viewSetting.nFontHeight = viewSetting["FontHeight"].GetAsInteger(32);
	g_globalInfo.viewSetting.bFontBlod = viewSetting["FontBlod"].GetAsBool(true);


	//////////////////////////////////////////////////////////////////////////
	//for header setting
	Coco::CResString headerSetting("/etc/Gui/viewHeaderSetting");
	g_globalInfo.listheaderSetting.clrTextPen = headerSetting["dwClrPen"].GetAsARGB(0xff000000);
	g_globalInfo.listheaderSetting.clrTextbrush = headerSetting["dwClrBursh"].GetAsARGB(0xff000000);
	g_globalInfo.listheaderSetting.nFontHeight = headerSetting["nFontHeight"].GetAsInteger(20);
	g_globalInfo.listheaderSetting.strFontFace = headerSetting["strFontFace"].GetAsString("宋体");
	g_globalInfo.listheaderSetting.clrSeparateLine = headerSetting["lineColor"].GetAsARGB(0xff000000);

	return TRUE;
}

CString CMonDataMgr::DB2Show( const ENUM_TEMPCOL_TYPE typeIn, const CString& strDBIn )
{
	CString strRet;
	if (TRCOLTYPE_CHANNELID == typeIn)
	{
		MAPCHANNELS::iterator itf = m_mapChannels.find(strDBIn);
		if (itf != m_mapChannels.end())
		{
			strRet = itf->second;
		}
	}
	else if (typeIn == TRCOLTYPE_STATE)
	{
		MAPMONSTATES::iterator itf = m_mapMonState.find(strDBIn);
		if (itf != m_mapMonState.end())
		{
			strRet = itf->second.strTitle;
		}
	}
	return strRet;
}

COLORREF CMonDataMgr::GetStateColor( ENUM_TRANSSTATE typeIn )
{
	if (typeIn == TRANSSTATE_UNKNOW)
	{
		ASSERT(FALSE);
		return 0;
	}
	return m_vmonState[typeIn].dwColor;
}

BOOL CMonDataMgr::GetStateColor( ENUM_TRANSSTATE typeIn, DWORD& dwclrStateFontOut, DWORD& dwclrStateBkOut )
{
	if (typeIn == TRANSSTATE_UNKNOW)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	dwclrStateFontOut = m_vmonState[typeIn].dwColor;
	dwclrStateBkOut = m_vmonState[typeIn].dwBkColor;
	return TRUE;
}

CString CMonDataMgr::GetMonStateTitle( ENUM_TRANSSTATE typeIn )
{
	return m_vmonState[typeIn].strTitle;
}

ENUM_TRANSSTATE CMonDataMgr::GetMonState( CString& strDBIn )
{
	ENUM_TRANSSTATE nState = TRANSSTATE_UNKNOW;
	MAPMONSTATES::iterator itf = m_mapMonState.find(strDBIn);
	if (itf != m_mapMonState.end())
	{
		nState = itf->second.nTransState;
	}
	ASSERT(nState != TRANSSTATE_UNKNOW);
	return nState;
}

BOOL CMonDataMgr::Release()
{
	if (m_bInited)
	{
		if (m_hThreadGetData)
		{
			TerminateThread(m_hThreadGetData, 0);
			m_hThreadGetData = NULL;
		}
	}

	m_ADOConnectSQL.CloseDBConnect();
	return TRUE;
}

BOOL CMonDataMgr::CheckAlert()
{
	int bAlert = ALERTTYPE_FLASH_SAFE;
	if (!m_bInited || m_lsData.size() == 0)
	{
		return FALSE;
	}
	m_lock.Lock(3000);
	for (int i = 0; i < m_lsData.size(); i++)
	{
		ENUM_TRANSSTATE state = m_lsData[i].GetMonState();
		if ( state == TRANSSTATE_ERROR)
		{
			bAlert = ALERTTYPE_FLASH_ERROR;
			break;
		}
		else if (state == TRANSSTATE_TRANSNONETIMEOUT
			|| state == TRANSSTATE_TRANSNONETIMEUNSAFE
			|| state == TRANSSTATE_TRANSDOORCLOSED)
		{
			bAlert = ALERTTYPE_FLASH_UNSAFE;
		}
	}
	m_lock.Unlock();
	return bAlert;
}



DWORD __stdcall ClockThreadProc( LPVOID pParam )
{
	CMonDataMgr* pMgr = (CMonDataMgr*)pParam;
	while (pMgr->CallRequest())
	{
		Sleep(pMgr->m_nCheckInterval);
	} 
	
	return 0;
}
