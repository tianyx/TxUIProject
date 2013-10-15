/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TrTemplateItem.cpp
	file base:	TrTemplateItem
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TrTemplateItem.h"
#include "Global.h"
#include "TimeCode.h"
#include "TrCommonInfoMgr.h"
#include "MonDataMgr.h"


bool IsEqualString(const CString& e1, const CString& e2)
{
	return e1.Compare(e2) == 0;
}
//////////////////////////////////////////////////////////////////////////
CTrMonItem::CTrMonItem(void)
{
	m_vData.reserve(TEMPLATE_DB_COL_NUM);
	m_vData.resize(TEMPLATE_DB_COL_NUM);
	m_vData[TRCOLTYPE_INDEX] = TEXT("--");
	m_vData[TRCOLTYPE_PGMNAME] = TEXT("--");
	m_vData[TRCOLTYPE_CHANNELID] =TEXT("--");
	m_vData[TRCOLTYPE_TOTALLENTH] =TEXT("0");
	m_vData[TRCOLTYPE_FIRSTIME] =TEXT("0");
	m_vData[TRCOLTYPE_TYPE] =TEXT("0");

}

CTrMonItem::~CTrMonItem(void)
{
}

bool CTrMonItem::operator==( const CTrMonItem& other ) const
{
	return std::equal(m_vData.begin(), m_vData.end(), other.m_vData.begin(), IsEqualString);
}

CString CTrMonItem::GetShownStrValue( const ENUM_TEMPCOL_TYPE typeIn ) const
{
	CString strRet;
	if (typeIn == TRCOLTYPE_FIRSTIME)
	{
		//time code convert it;
		CTimeCode tmcode(atoi(m_vData[typeIn]));
		strRet = tmcode.ConvertToString();
	}
	else if (typeIn == TRCOLTYPE_TYPE)
	{
		strRet = g_CoInfoMgr.DB2Show(typeIn, m_vData[typeIn]);
	}
	else
	{
		strRet = m_vData[typeIn];
	}
	return strRet;
}

BOOL CTrMonItem::GetShownValue( VECSHOWNSTRING& vValOut) const
{
	CString strRet(TEXT(""));

	for (size_t i = 0; i < m_vData.size() ; i++)
	{
		if (i == TRCOLTYPE_TYPE)
		{
			strRet = GetShownStrValue((ENUM_TEMPCOL_TYPE)i);
			if (strRet.IsEmpty())
			{
				ASSERT(FALSE);
				return FALSE;
			}
		}
		else
		{
			strRet = m_vData[i];
		}
		vValOut.push_back(strRet);
	}

	if (TEMPLATE_DB_COL_NUM != vValOut.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CTrMonItem::SetShownValue( const ENUM_TEMPCOL_TYPE typeIn, const CString& strIn )
{
	CString strRet;
	BOOL bConverted = FALSE;
	if (typeIn == TRCOLTYPE_FIRSTIME)
	{
		//time code convert it;
		CString strTm(strIn);
		CTimeCode tmcode(strTm);

		strRet.Format(TEXT("%d"),tmcode.GetTotalFrames());
		bConverted = TRUE;

	}
	else if (typeIn == TRCOLTYPE_TYPE)
	{
		strRet = g_CoInfoMgr.Show2DB(typeIn, strIn);
		bConverted = TRUE;
	}
	else
	{
		strRet = strIn;
		
	}

	if (bConverted && strRet.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	else
	{
		m_vData[typeIn] = strRet;
		return TRUE;
	}
}

BOOL CTrMonItem::SetShownValue( const VECSHOWNSTRING& vValIn)
{
	size_t i = 0;
	CString sTemp;

	while (TEMPLATE_DB_COL_NUM != i)
	{
		if (i == TRCOLTYPE_TYPE)
		{
			if (vValIn[i].IsEmpty())
			{
				ASSERT(FALSE);
				return FALSE;
			}
			sTemp = g_CoInfoMgr.Show2DB((ENUM_TEMPCOL_TYPE)i, vValIn[i]);
		}
		else if (i == TRCOLTYPE_FIRSTIME)
		{
			if (vValIn[i].IsEmpty())
			{
				ASSERT(FALSE);
				return FALSE;
			}
			CString str = vValIn[i];
			CTimeCode tmCode(str);
			sTemp.Format(TEXT("%d"),tmCode.GetTotalFrames());
		}
		else
		{
			sTemp = vValIn[i];
		}

		m_vData[i] = sTemp;
		i++;
	}

	return TRUE;
}

BOOL CTrMonItem::DirectSetValue( const ENUM_TEMPCOL_TYPE typeIn, const CString& strIn )
{
	m_vData[typeIn] = strIn;
	return TRUE;
}

BOOL CTrMonItem::DirectSetValue( const VECSTRING& vValIn)
{
	if (TEMPLATE_DB_COL_NUM != vValIn.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	for (size_t i = 0; i != TEMPLATE_DB_COL_NUM; i++)
	{
		m_vData[i] = vValIn[i];
	}
	return TRUE;
}

BOOL CTrMonItem::DirectSetValue( const ENUM_TEMPCOL_TYPE typeIn, const int nValIn )
{
	if (typeIn == TRCOLTYPE_INDEX
		||typeIn == TRCOLTYPE_TYPE
		||typeIn == TRCOLTYPE_TOTALLENTH
		||typeIn == TRCOLTYPE_FIRSTIME
		||typeIn == TRCOLTYPE_STATE)
	{
		CString strVal;
		strVal.Format(TEXT("%d"), nValIn);
		m_vData[typeIn] = strVal;
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
}

BOOL CTrMonItem::DirectSetValue( const ENUM_TEMPCOL_TYPE typeIn, const time_t nValIn )
{
	if (typeIn == TRCOLTYPE_FIRSTIME)
	{
		CString strVal;
		strVal.Format(TEXT("%d"), nValIn);
		m_vData[typeIn] = strVal;
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
}

CString CTrMonItem::DirectGetValue( const ENUM_TEMPCOL_TYPE typeIn ) const
{
	return m_vData[typeIn];
}

BOOL CTrMonItem::DirectGetValue(VECSTRING& vValOut) const
{
	for (size_t i = 0; i != TEMPLATE_DB_COL_NUM; i++)
	{
		vValOut.push_back(m_vData[i]);
	}
	return TRUE;
}

int CTrMonItem::DirectGetIntValue( const ENUM_TEMPCOL_TYPE typeIn ) const
{
	if (typeIn == TRCOLTYPE_TYPE
		|| typeIn == TRCOLTYPE_FIRSTIME
		|| typeIn == TRCOLTYPE_TOTALLENTH)
	{
		return atoi(m_vData[typeIn]);
	}
	else
	{
		return 0;
	}
}

CString CTrMonItem::GetValue( const ENUM_MONTYPE typeIn )
{ 
	switch (typeIn)
	{
	case MONTYPE_CHNAME:
		{
			return g_monDataMgr.DB2Show(TRCOLTYPE_CHANNELID, m_vData[TRCOLTYPE_CHANNELID]);
		}break;
	case MONTYPE_STATE:
		{
			int nState = atoi(m_vData[TRCOLTYPE_STATE]);
			if (nState == DBNOTRANSVALUE)
			{
				//not in trans table, use custom state
				ENUM_TRANSSTATE nTransState = TRANSSTATE_TRANSNONETIMESAFE;
#ifdef _CIMTEST
				CTimeSpan tmSpan(atoi(m_vData[TRCOLTYPE_FIRSTIME])  -  CTime(2012,10,20, 15,0,0).GetTime());

#else
				CTimeSpan tmSpan(atoi(m_vData[TRCOLTYPE_FIRSTIME])  -  time(NULL));
#endif // _CIMTEST
				LONGLONG nDeta = tmSpan.GetTotalSeconds();
				if ( nDeta <= 0)
				{
					nTransState = TRANSSTATE_TRANSNONETIMEOUT;
				}
				else if (nDeta < g_monDataMgr.m_nOffsetSafeSecond)
				{
					if (nDeta < g_monDataMgr.m_nCloseDoorSecond)
					{
						nTransState = TRANSSTATE_TRANSDOORCLOSED;
					}
					else
					{
						nTransState = TRANSSTATE_TRANSNONETIMEUNSAFE;
					}
				}
				 return g_monDataMgr.GetMonStateTitle(nTransState);
			}
			else
			{

				//use db state
				return g_monDataMgr.DB2Show(TRCOLTYPE_STATE, m_vData[TRCOLTYPE_STATE]);
			}
			//
		}break;
	case MONTYPE_TIMEREMAIN:
		{
#ifdef _CIMTEST
			CTimeSpan tmSpan(atoi(m_vData[TRCOLTYPE_FIRSTIME])  -  CTime(2012,10,20, 15,0,0).GetTime());
#else
			CTimeSpan tmSpan(atoi(m_vData[TRCOLTYPE_FIRSTIME])  - time(NULL));
#endif // _DEBUG
			//CTimeSpan tmSpan( atoi(m_vData[TRCOLTYPE_FIRSTIME])  - time(NULL));
			CString strFmt;
			strFmt.Format(TEXT("%02d:%02d:%02d"), (int)tmSpan.GetTotalHours(), tmSpan.GetMinutes(),tmSpan.GetSeconds());
			return strFmt;
		}break;
	case MONTYPE_CLIPNAME:
		{
			return m_vData[TRCOLTYPE_PGMNAME];
		}break;
	case MONTYPE_PGMCODE:
		{
			return m_vData[TRCOLTYPE_PGMID];
		}break;
	case MONTYPE_INDEX:
		{
			return m_vData[TRCOLTYPE_INDEX];
		}break;
	case MONTYPE_TOTALLENGTH:
		{
			return m_vData[TRCOLTYPE_TOTALLENTH];
		}break;

	case MONTYPE_FIRSTTIME:
		{
			CTime tm(atoi(m_vData[TRCOLTYPE_FIRSTIME]));
			CString strFmt = tm.Format(TEXT("%Y-%m-%d %H:%M:%S"));
			return strFmt;
		}break;

	default:
		{
			ASSERT(FALSE);
		}
	}

	return TEXT("");
}

int CTrMonItem::GetIntValue( const ENUM_MONTYPE typeIn )
{
	if (typeIn == MONTYPE_STATE)
	{
		return atoi(m_vData[TRCOLTYPE_STATE]);
	}

	return 0;
}

COLORREF CTrMonItem::GetMonColor()
{
	return g_monDataMgr.GetStateColor(GetMonState());
}

BOOL CTrMonItem::GetMonColor( DWORD& dwClrFont, DWORD& dwClrBk )
{
	return g_monDataMgr.GetStateColor(GetMonState(), dwClrFont, dwClrBk);
}

ENUM_TRANSSTATE CTrMonItem::GetMonState()
{
	ENUM_TRANSSTATE nTransState = TRANSSTATE_UNKNOW;

	int nState = atoi(m_vData[TRCOLTYPE_STATE]);

	if (nState == DBNOTRANSVALUE)
	{
		//not in trans table, use custom state
		nTransState = TRANSSTATE_TRANSNONETIMESAFE;
#ifdef _CIMTEST
		CTimeSpan tmSpan(atoi(m_vData[TRCOLTYPE_FIRSTIME]) - CTime(2012,10,20, 15,0,0).GetTime() );
#else
		CTimeSpan tmSpan( atoi(m_vData[TRCOLTYPE_FIRSTIME]) - time(NULL) );
#endif
		LONGLONG nDeta = tmSpan.GetTotalSeconds();
		if ( nDeta <= 0)
		{
			nTransState = TRANSSTATE_TRANSNONETIMEOUT;
		}
		else if (nDeta <= g_monDataMgr.m_nOffsetSafeSecond)
		{
			if (nDeta <= g_monDataMgr.m_nCloseDoorSecond)
			{
				nTransState = TRANSSTATE_TRANSDOORCLOSED;
			}
			else
			{
				nTransState = TRANSSTATE_TRANSNONETIMEUNSAFE;
			}
		}
	}
	else
	{
		//use db state
		nTransState = g_monDataMgr.GetMonState(m_vData[TRCOLTYPE_STATE]);
	}

	return nTransState;
}

BOOL CTrMonItem::IsSamePGMID( CTrMonItem& other )
{
	return (m_vData[TRCOLTYPE_PGMID].Compare(other.m_vData[TRCOLTYPE_PGMID])== 0);

}
