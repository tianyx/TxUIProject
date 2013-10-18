#pragma once
#include "IMSerialize.h"
#include "HttpNetOp.h"

struct ST_CLOCKINDATA
{
	ST_NETOPINFO netData;
	int nState;
	int nHour;
	int nMinute;
	ST_CLOCKINDATA()
	{
		nState = 0;
		nHour = -1;
		nMinute = -1;
	}

	BOOL operator <<(CIMSerialize& ar) const
	{
		return ar << netData
			&& ar << (int)0
			&& ar << nHour
			&& ar << nMinute;
	}

	BOOL operator >> (CIMSerialize& ar)
	{
		return ar >> netData
			&& ar >> nState
			&& ar >> nHour
			&& ar >> nMinute;
	}

};


typedef std::vector<ST_CLOCKINDATA> VECCLOCKININFO;

#define MSG_AC_WRITELOG	WM_USER + 122
#define MSG_SYSTEM_TRAY	WM_USER + 123
#define MSG_SHUTDOWN WM_USER + 124

struct ST_ACSETTING
{
	BOOL bAutoRun;	//when login
	BOOL bAutoStart; //when app inited
	BOOL bAutoClockIn1; //when app inited
	BOOL bPopBubble;	//after clock in
	BOOL bAutoShutdown; // after clock in 4
	int  nPreMinute; //random minute to clock in
	BOOL bSleepPC;


	ST_ACSETTING()
	{
		bAutoRun = FALSE;
		bAutoStart = FALSE;
		bAutoClockIn1 = FALSE;
		bPopBubble = TRUE;
		bAutoShutdown = FALSE;
		nPreMinute = 0;
		bSleepPC = TRUE;
	}

	BOOL operator <<(CIMSerialize& ar) const
	{
		return ar << bAutoRun
			&& ar << bAutoStart
			&& ar << bAutoClockIn1
			&& ar << bPopBubble
			&& ar << bAutoShutdown
			&& ar << nPreMinute
			&& ar << bSleepPC;
	}

	BOOL operator >> (CIMSerialize& ar)
	{
		return ar >> bAutoRun
			&& ar >> bAutoStart
			&& ar >> bAutoClockIn1
			&& ar >> bPopBubble
			&& ar >> bAutoShutdown
			&& ar >> nPreMinute
			&& ar >> bSleepPC;
	}


};



class CACConfig
{
private:
	CACConfig(void);
	~CACConfig(void);

public:
	void Release();
	BOOL CreateStatrupShortcut();
	BOOL LoadData();
	BOOL SaveData();
public:
	ST_ACSETTING m_cfgData;
	VECCLOCKININFO m_vNetData;
private:
	static CACConfig* m_spAcCfg;
	
	CListCtrl m_pctlLog;

public:
	static CACConfig* GetACConfig();
	static int m_snRef;
	static CString GetAppPath();
};


extern HWND g_hwndLog;

void ACWriteLog(LPCTSTR lpFmt, ...);
BOOL CreateLink(LPCTSTR szPath,//快捷方式的目标应用程序名
				LPCTSTR szLink);//快捷方式的数据文件名(*.lnk)
CString GetFileName( CString& strFileIn );