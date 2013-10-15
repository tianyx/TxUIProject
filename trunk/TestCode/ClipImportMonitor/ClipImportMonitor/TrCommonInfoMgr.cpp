/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TrCommonInfoMgr.cpp
	file base:	TrCommonInfoMgr
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "trCommonInfoMgr.h"
#include <algorithm>
//#include "ccmxt\MxtDef.h"
#include "COCOLIB20\Coco_Text_DB.h"
//////////////////////////////////////////////////////////////////////////
bool LstHeaderSortFn(ST_TMP_COLINFO& e1, ST_TMP_COLINFO& e2)
{
	return e1.nColPos < e2.nColPos;
}

void Vector2StringList( const VECSTRING& vDataIn, CStringList& vDataOut )
{
	vDataOut.RemoveAll();
	for (size_t i = 0; i < vDataIn.size(); i++)
	{
		vDataOut.AddTail(vDataIn[i]);
	}
}

CString ToTimeCode( CString& strIn )
{
	CString strRet;
	return strRet;
}


//////////////////////////////////////////////////////////////////////////


CTrCommonInfoMgr g_CoInfoMgr;
CTrCommonInfoMgr::CTrCommonInfoMgr(void)
{
	//init header name in db
	m_vTmpViewConfig.m_vDBTmpCols.reserve(TEMPLATE_DB_COL_NUM);
	m_vTmpViewConfig.m_vDBTmpCols.resize(TEMPLATE_DB_COL_NUM);
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_INDEX]=("Id");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_PGMID]=("ProgramID");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_PGMNAME]=("ProgramName");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_CHANNELID]=("ChannelID");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_TOTALLENTH]=("TotalLength");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_FIRSTIME]=("FirstTime");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_TYPE]=("Type");
	m_vTmpViewConfig.m_vDBTmpCols[TRCOLTYPE_STATE]=("Status");


}

CTrCommonInfoMgr::~CTrCommonInfoMgr(void)
{
}

BOOL CTrCommonInfoMgr::IsCanTemplateEdit()
{
	return m_bCanEditTemplate;
}

BOOL CTrCommonInfoMgr::InitMgr()
{
	
	//device type
/*
	CArrayDeviceType& deviceTypeArray = *(GetEditConfig()->GetVideoDevCfgPackage().GetDeviceTypeInfo());
	m_arrSrcDev.Append(deviceTypeArray);
	if (m_arrSrcDev.GetSize() == 0)
	{
		ASSERT(FALSE);
	}
	for (int i = 0; i < m_arrSrcDev.GetSize(); i++)
	{
		m_vcmbDevice.push_back(m_arrSrcDev[i].strDeviceTypeName);
		if (m_arrSrcDev[i].strDeviceTypeID.Compare(TEXT("LVSRC_DEC_ADV")) != 0)
		{
			m_vcmbDeviceNoAd.push_back(m_arrSrcDev[i].strDeviceTypeName);
		}
		m_vDevType.push_back(m_arrSrcDev[i].strDeviceTypeID);
	}
*/


	//play style


/*
	//replay
	for(int i = 0; i < 2; i++)
		m_vcmbStrReplay.push_back(g_strFirstBroadcast[i]);

	//---下面2个信息待改。。
	//have/not have
	for(int i = 0; i < LOGOMODE_TYPES; i++)
		m_vcmbStrHaveNot.push_back(g_strLogoMode[i]);
	//yes no
	for (int i = 0; i< DUBWHENPLAYING_TYPES; i++)
	{
		m_vcmbStrYesNo.push_back(g_strDubWhenPlaying[i]);
	}*/

	//---
	LoadTemplateViewConfig();


	return TRUE;
}

BOOL CTrCommonInfoMgr::LoadTemplateViewConfig()
{
	//template header
	CResString Setting("/etc/gui/ColSetting");
	CStringArray strArrColName;
	Coco::CXmlProp xmlColumn;

	Setting["Columns"].GetAsStringArray(strArrColName);

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
		ST_TMP_COLINFO tmpColInfo;
		tmpColInfo.nPosInType = (ENUM_TEMPCOL_TYPE)xmlColumn["TypePos"].GetAsInteger(-1);
		tmpColInfo.nColPos = xmlColumn["ColPos"].GetAsInteger(-1);
		tmpColInfo.nColWidth = xmlColumn["Width"].GetAsInteger(150);
		tmpColInfo.nMinWidth = xmlColumn["MinWidth"].GetAsInteger(150);
		tmpColInfo.strTitle = xmlColumn["title"].GetAsString();
		tmpColInfo.bWidthFixed = xmlColumn["WidthFixed"].GetAsBool();
		tmpColInfo.nEditMode = xmlColumn["EditMode"].GetAsInteger(0);
		m_vTmpViewConfig.m_vTmpCols.push_back(tmpColInfo);
	}
	std::sort(m_vTmpViewConfig.m_vTmpCols.begin(), m_vTmpViewConfig.m_vTmpCols.end(), LstHeaderSortFn);

	//color setting
	// 下面读取列表的行高和颜色配置信息
	m_vTmpViewConfig.m_RowDefSetting.m_TextColor				= Setting["TextColor"].GetAsColor(g_globalInfo.viewSetting.clrText);
	m_vTmpViewConfig.m_RowDefSetting.m_TextColorHighlight	= Setting["TextColorHightlight"].GetAsColor(g_globalInfo.viewSetting.clrTextHilight);

	m_vTmpViewConfig.m_RowDefSetting.m_TextBKColor			= Setting["TextBKColor"].GetAsColor(g_globalInfo.viewSetting.clrTextBk);
	m_vTmpViewConfig.m_RowDefSetting.m_TextBKColorHighlight	= Setting["TextBKColorHightlight"].GetAsColor(g_globalInfo.viewSetting.clrTextBkHilight);

	m_vTmpViewConfig.m_RowDefSetting.m_BKColor				= Setting["BKColor"].GetAsColor(g_globalInfo.viewSetting.clrBk);
	m_vTmpViewConfig.m_RowDefSetting.m_BKColorHighlight		= Setting["BKColorHightlight"].GetAsColor(g_globalInfo.viewSetting.clrBkHight);
	m_vTmpViewConfig.m_RowDefSetting.m_ActiveColor			= Setting["ActiveColor"].GetAsColor(RGB(255,255,255));

	m_vTmpViewConfig.m_RowDefSetting.m_TextHitHighLight = Setting["HitHighLightColor"].GetAsColor(RGB(255,255,255));
	m_vTmpViewConfig.m_RowDefSetting.m_TextBKHitHighLight = Setting["BKHitHighLightColor"].GetAsColor(RGB(0,0,0));

	/////////////////////////////////////////////////////
	//
	// 字体信息
	m_vTmpViewConfig.m_nRowHeight	= Setting["RowHeight"].GetAsInteger(20);
	m_vTmpViewConfig.m_nFontSize		= Setting["FontSize"].GetAsInteger(15);
	m_vTmpViewConfig.m_strFontName	= Setting["FontName"].GetAsString("宋体");
	return TRUE;

}



const VECSTRING* CTrCommonInfoMgr::GetComboListByColType( const ENUM_TEMPCOL_TYPE typeIn)
{
	VECSTRING* pOut = NULL;
/*
	switch(typeIn)
	{
	case TRCOLTYPE_DEVICETYPEID:
		{
			pOut = &m_vcmbDeviceNoAd;
		}break;
	case TRCOLTYPE_PLAYLOGO:
		{
			pOut = &m_vcmbStrHaveNot;
		}break;
	case TRCOLTYPE_PLAYTITLE:
		{	
			pOut = &m_vcmbStrHaveNot;
		}break;
	case TRCOLTYPE_REPLAY:
		{
			pOut = &m_vcmbStrReplay;
		}break;
	case TRCOLTYPE_PLAYSTYLE:
		{
			pOut = &m_vcmbStrPlayStyle;
		}break;
	}
*/

	return pOut;
}

CString CTrCommonInfoMgr::DB2Show( const ENUM_TEMPCOL_TYPE typeIn, const CString& strDBIn )
{
	ASSERT(FALSE);
	CString strRet;	
	
	return strRet;
}

CString CTrCommonInfoMgr::Show2DB( const ENUM_TEMPCOL_TYPE typeIn, const CString& strShowIn )
{
	ASSERT(FALSE);
	CString strRet;
/*
	switch(typeIn)
	{
	case TRCOLTYPE_DEVICETYPEID:
		{
			ASSERT(m_vDevType.size() > 0);
			int nDev = 0;
			for (int i = 0; i < m_vDevType.size(); i++)
			{
				if (m_vcmbDevice[i].Compare(strShowIn)==0)
				{
					nDev =i;
					break;
				}
			}
			strRet = m_vcmbDevice.size()> 0? m_vDevType[nDev]: TEXT("");
		}break;
	case TRCOLTYPE_PLAYLOGO:
	case TRCOLTYPE_PLAYTITLE:
		{
			int nPos = 0;
			for (int i = 0; i < m_vcmbStrHaveNot.size(); i++)
			{
				if (m_vcmbStrHaveNot[i].Compare(strShowIn)==0)
				{
					nPos =i;
					break;
				}
			}

			char buffer[32];
			strRet = itoa(nPos, buffer, 32);
		}break;
	case TRCOLTYPE_REPLAY:
		{
			int nPos = 0;
			for (int i = 0; i < m_vcmbStrYesNo.size(); i++)
			{
				if (m_vcmbStrReplay[i].Compare(strShowIn)==0)
				{
					nPos =i;
					break;
				}
			}
			char buffer[32];
			strRet = itoa(nPos, buffer, 32);
		}break;
	case TRCOLTYPE_PLAYSTYLE:
		{
			int nPos = 0;
			for (int i = 0; i < m_vcmbStrPlayStyle.size(); i++)
			{
				if (m_vcmbStrPlayStyle[i].Compare(strShowIn)==0)
				{
					nPos =i;
					break;
				}
			}
			char buffer[32];
			strRet = itoa(nPos, buffer, 32);
		}break;
	}*/

	return strRet;
}

BOOL CheckStrValid( LPCTSTR szStrIn, BOOL bEnableSpace /*= TRUE */)
{
	static char g_szInValidCharList[] =" [];'\\/,.?{}:\"|<>?*";
	if (szStrIn == NULL)
	{
		ASSERT(FALSE);
		return TRUE;
	}
	const TCHAR* pCh = szStrIn;
	while (*pCh != '\0')
	{
		int nTmpLen = strlen(g_szInValidCharList);
		char* pTmpChar = g_szInValidCharList;
		if(bEnableSpace)
		{
			pTmpChar++;
			nTmpLen--;
		}
		for (int i=0; i < nTmpLen; i++)
		{
			if (*pCh == *pTmpChar)
			{
				return FALSE;
			}
			pTmpChar++;
		}
		pCh++;
	}

	return TRUE;
}

CString GetFileName(const CString& strFileIn )
{
	CString strExt = strFileIn;
	int nPos = strFileIn.ReverseFind('\\');
	if(nPos > 0)
	{
		strExt = strFileIn.Mid(nPos+1);
	}
	return strExt;
}
