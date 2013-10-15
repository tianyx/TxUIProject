/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	MonDataMgr.h
	file base:	MonDataMgr
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include <Codex20\Codex_ADO.H>
#include <COCOLIB20\Coco_String_CString.h >
#include <vector>
#include <map>
#include "preDef.h"
#include "TrTemplateItem.h"
#include "TxFontLoader.h"

#define DBNOTRANSVALUE 6666
#define STRDBNOTRANSVALUE "6666"


struct ST_MONRULE
{
	CStringArray m_vChannels;
	int	m_nDaysForMon; //days after today,include today
};





struct ST_MONSTATEPARAM
{
	CString strDBValue;
	ENUM_TRANSSTATE nTransState;
	DWORD dwColor;
	DWORD dwBkColor;
	CString strTitle;
	ST_MONSTATEPARAM()
	{
		dwColor = 0xff000000;
		dwBkColor = 0xffffffff;
		nTransState = TRANSSTATE_UNKNOW;
	}
};

typedef std::vector<ST_MONSTATEPARAM> VECMONSTATES;
typedef std::map<CString, ST_MONSTATEPARAM> MAPMONSTATES;

DWORD __stdcall ClockThreadProc(LPVOID pParam);

class CMonDataMgr
{
public:
	CMonDataMgr(void);
	~CMonDataMgr(void);

	BOOL Init();
	BOOL Release();
	BOOL IsInited(){return m_bInited;}
	BOOL GetDataFromDB();
	BOOL RegisterObserverWnd(HWND hwndIn);
	void NotifyView();
	//range[0, n]
	int GetRangeData(int nBegin, int nCount, VECTMPITEM& vOut);
	int GetDataCount(){return m_lsData.size();}
	//
	BOOL CheckAlert();

	//
	CString DB2Show(const ENUM_TEMPCOL_TYPE typeIn, const CString& strDBIn);

	ENUM_TRANSSTATE GetMonState(CString& strDBIn);
	CString GetMonStateTitle(ENUM_TRANSSTATE typeIn);


	COLORREF GetStateColor(ENUM_TRANSSTATE typeIn);
	BOOL GetStateColor(ENUM_TRANSSTATE typeIn, DWORD& dwclrStateFontOut, DWORD& dwclrStateBkOut);

	friend DWORD __stdcall ClockThreadProc(LPVOID pParam);
protected:
	BOOL CallRequest();
private:
	BOOL ConnectSrv();
	BOOL LoadConfig();
	BOOL LoadChannelInfo();
 
	//
	MAPCHANNELS m_mapChannels;
	//state
	VECMONSTATES m_vmonState;
	MAPMONSTATES m_mapMonState;
	//
	VECTMPITEM		m_lsData;			//list data
// 	LSTTMPITEM	  m_lsDataOnline;		//data is ready in t_preparetrans table
// 	LSTTMPITEM	  m_lsDataOffline;		//data not in t_preparetrans table
// 	LSTTMPITEM	m_lsAllData;

//	MAPTMPLSTITEM  m_mapData;
	//monitor param
	VECSTRING m_vMonChannels;
	int m_nMonDays;
	int m_nMonTimeOutMinutes;
	int m_nCheckInterval;
	int m_nOffsetSafeMinute;
	int m_nCloseDoorMinute;

	//value for cache
public:
	int m_nOffsetSafeSecond;
	int m_nMonTimeOutSecond;
	int m_nAlertCheckInterval;
	int m_nCloseDoorSecond;
	//app info
private:
	BOOL m_bInited;
	Coco::CADOConnectLE   m_ADOConnectSQL;
	Coco::tagURL m_dbURL;
	CString m_strSqlOnline;
	//CString m_strSqlOffline;
	HANDLE m_hThreadGetData;
	CMutex m_lock;

	HWND m_hwndOb;

};
