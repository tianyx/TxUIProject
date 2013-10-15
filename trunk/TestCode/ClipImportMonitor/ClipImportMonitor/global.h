/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	global.h
	file base:	global
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include "PreDef.h"
#include "GdiPlusNewHeader.h"

//////////////NOTIFY MESSAGE ID//////////////////////////////////////////////
#define UM_LANGUAGE_CHANDGED		WM_USER + 601
#define MSG_SYSTEM_TRAY				WM_USER + 602

#define MSG_ANI_INSERT				WM_USER + 603	//insert emotion

//
#define MSG_MONDATACHANGED			WM_USER + 7001

#define MSG_TXHEADER_WIDTHCHANGED	WM_USER + 8001

//////////////////////////for transparent draw////////////////////////////////////////////////
#define  TRANS_PARENT_CLR			RGB(0,188,242)
//////////////////////////////////////////////////////////////////////////

#define APP_PROCESS_NAME			TEXT("ClipImportMonitor.exe")
#define APP_MAIN_WND_NAME			TEXT("ClipImportMonitor")
#define MAIN_FRAME_MUTEX			"{B468F42B-2FA6-4903-8ED6-7272E96BF237}"

//////////////////////////////////////////////////////////////////////////
#define INI_APP_KEY					TEXT("ClipImportMonitor")

//////////////////////////////////////////////////////////////////////////

#define PHOTO_NORMAL_WIDTH 40
#define PHOTO_NORMAL_HEIGHT 40
#define PHOTO_SMALL_WIDTH 20
#define PHOTO_SMALL_HEIGHT 20


typedef enum
{
	DRIVE	= 0x0001,
	PATH	= 0x0002,
	FNAME	= 0x0004,
	EXT		= 0x0008,
}SPLIT;

class CLanManager;
class CFont;
class Log;

class Gdiplus::Image;
class CxSoundPlayer;
class CMonDataMgr;
class CTxFontLoader;
struct HD_GLOBAL_INFO;

extern CLanManager				g_lanManager;
extern CFont					g_fontBase;
// extern CDlgDebugLog*			g_pdlgDebugLog;

extern Log						g_log;
extern LOGFONT					g_logfont;

extern CMonDataMgr		g_monDataMgr; 
extern CTxFontLoader  g_FontLoader;
extern HD_GLOBAL_INFO g_globalInfo;

#define MAX_IM_TEXT_BUFFER 1024

#define IM_TMP_DIR TEXT("\\tmp")

#define IM_OK 1
#define IM_ERR 0


//////////old///////////////////////////////////////////////////////////////
#define ACTION_TYPE_ERR  -1
#define ACTION_TYPE_TEXT 1
#define ACTION_TYPE_LINK 2
#define ACTION_TYPE_FILE 3
#define ACTION_TYPE_VIDEO 4
#define ACTION_TYPE_AUDIO 5

#define RESPOND_TYPE_INIT		-1
#define RESPOND_TYPE_CANCEL 0
#define RESPOND_TYPE_OK		1
#define RESPOND_TYPE_BUSY	2


//////////////////////////////////////////////////////////////////////////
//respond

#define MAX_CHAR_BUFFER 256
#define STRING_END '\0'



struct HD_GLOBAL_INFO 
{
	CString strDBString;
	BOOL bVerHigerXp;
	BOOL bSvrInited;
	CString strAppPath;

	CSize nScreenSize; //in pixel
	CSize nMonitorSize; //in mm

	struct ST_GLOBAL_SETTING
	{
		DWORD dwPen;
		DWORD dwBrush;
		ST_GLOBAL_SETTING()
		{
			dwPen = 0xff000000;
			dwBrush = 0xff000000;
		}
	}gSetting;

	struct ST_CACHESETTING
	{
		CSize szCacheMinSize;
		CSize szCacheSize;
		ST_CACHESETTING()
		{
			szCacheMinSize.cx = GetSystemMetrics(SM_CXSCREEN);
			szCacheMinSize.cy = GetSystemMetrics(SM_CYSCREEN);
			szCacheSize.cx = szCacheMinSize.cx*2;
			szCacheSize.cy = szCacheMinSize.cy*2;

		}
	}cacheSetting;

	struct ST_CAPTIONSETTING
	{
		DWORD dwClrBursh;
		DWORD dwClrPen;
		DWORD dwClrBurshTime;
		DWORD dwClrPenTime;
		CString strFontFaceTime;
		int nFontHeightTime;
		ST_CAPTIONSETTING()
		{
			dwClrPen = 0x00000000;
			dwClrBursh =0xffffffff;
			dwClrBurshTime = 0xff00ffff;
			dwClrPenTime =   0xa0ff00ff;
			nFontHeightTime = 32;
		}
		
	}captionSetting;
	
	struct DATAMONSETTING 
	{
		int nRequestInterval;
		DATAMONSETTING()
		{
			nRequestInterval = 30000;
		}
	}monSetting;

	struct LISTHEADERSETTING
	{
		DWORD clrTextPen;
		DWORD clrTextbrush;
		DWORD nFontHeight;
		CString strFontFace;
		DWORD clrSeparateLine;
		LISTHEADERSETTING()
		{
			clrTextPen = Gdiplus::Color::MakeARGB(0, 255, 255, 255);
			clrTextbrush = Gdiplus::Color::MakeARGB(255, 55, 101, 161);
			clrSeparateLine =  Gdiplus::Color::MakeARGB(255, 255, 255, 255);
			nFontHeight = 20;

		}
	}listheaderSetting;

	struct VIEWSETTING
	{
		DWORD clrBk;
		DWORD clrBkHight;
		DWORD clrText;
		DWORD clrTextBk;
		DWORD clrTextHilight;
		DWORD clrTextBkHilight;
		DWORD clrSeparateLine;
		int nItemHeight;
		BOOL bHeaderFitClient;
		BOOL bEnableHBar;
		CString strFontFace;
		int nFontHeight;
		BOOL bFontBlod;
		VIEWSETTING()
		{
			clrBk = Gdiplus::Color::MakeARGB(255, 0, 0, 0);
			clrBkHight = Gdiplus::Color::MakeARGB(255, 100, 100, 100);
			clrText = Gdiplus::Color::MakeARGB(255, 255, 255, 255);
			clrTextBk = Gdiplus::Color::MakeARGB(255, 0, 0, 0);
			clrTextHilight = Gdiplus::Color::MakeARGB(255, 0, 255, 255);
			clrTextBkHilight = Gdiplus::Color::MakeARGB(255, 100, 100, 100);
			clrSeparateLine =  Gdiplus::Color::MakeARGB(255, 100, 151, 206);
			nFontHeight = 24;
			bFontBlod = TRUE;

		}

	}viewSetting;

	struct LOCAL_SETTING 
	{
		DWORD bkClr;
		DWORD fontClr;
		DWORD grayClr;
		BOOL  bEnableMsgTwinkle;
		int nLocalVideoWidth;
		int nLocalVideoHeight;
		int nRemoteVideoWidth;
		int nRemoteVideoHeight;

		//font style
		CString m_strFontFace;
		LONG m_lFontStyle;
		COLORREF m_clrFontColor;
		LONG m_lFontHeight;

		//item style
		COLORREF m_clrItemBk1;
		COLORREF m_clrItemBk2;

		LOCAL_SETTING()
		{
			bkClr = RGB( /*244,243,249*/222,224,236 /*223,224,242*/);
			fontClr= RGB(75,75,75);
			grayClr = ::GetSysColor(COLOR_GRAYTEXT);
			bEnableMsgTwinkle = TRUE;

			nLocalVideoWidth = 320;
			nLocalVideoHeight =240;

			nRemoteVideoWidth =240;
			nRemoteVideoHeight =240;

			m_clrItemBk1 = RGB(241,232,200);
			m_clrItemBk2 = RGB(186,225,242);//RGB(230,238, 240);

		}
	}localSetting;

	struct COMMON_STR 
	{
		CString strDepartNodeRequesting;

	}commonStr;

	struct SYSTRAY_INFO
	{
		// CDlgIMContainer ¶Ô»°¿ò
		NOTIFYICONDATA  trayData;
		HICON hOriginIcon; //need not del
		HICON hCustomIcon; //must del
		HICON hEmptyIcon; //need not del
		SYSTRAY_INFO()
		{
			ZeroMemory(&trayData,sizeof(trayData));
			hOriginIcon = NULL;
			hCustomIcon = NULL;
			hEmptyIcon = NULL;
		}

	}sysTrayInfo;

	struct ST_TXDRAWPARAM
	{
		CRect rcStretchEdge;

		ST_TXDRAWPARAM()
		{
			rcStretchEdge = CRect(8,8,8,8);
		}
	}txDrawParam;

	//scroll bar default height;
	struct ST_SCROLLBARPARAM
	{
		int nMinViewPixPerScrollPix;
		int nMinMidLen;
		UINT nScrollBarWidth;
		ST_SCROLLBARPARAM()
		{
			nMinViewPixPerScrollPix = 10;
			nMinMidLen = 20;
			nScrollBarWidth = 16;
		}

	}scrollbarParam;

	HD_GLOBAL_INFO()
	{
		bSvrInited = FALSE;
		bVerHigerXp = FALSE;
		nScreenSize.cx = 0;
		nScreenSize.cy = 0;
		nMonitorSize.cx = 0;
		nMonitorSize.cy = 0;

	}

};

//////////////////////////////////////////////////////////////////////////
void TxUTModifyState(UINT& nStateInOut, UINT stateRem, UINT stateAdd);
BOOL TxUTHasState(UINT nStateOrg, UINT stateToCheck);
//////////////////////////////////////////////////////////////////////////



CString AfxGetAppPath();
CString AfxGetOcxPath();
CString AfxSplitPathName(LPCTSTR lpszPath, int nSplit);
//strfile formate = TEXT("subfolder\\filename.bmp");
CString GetFullFilePath(const CString& strFile);
//get the filepath based on .ocx directory.
CString GetFullFilePathOCX(const CString& strFile);

BOOL	GetVersionIsHigherThanXp();

//move rc2 to center of rc1 
CRect	GetCenterRect(CRect rc1, CRect rc2);

void	AddHDDebugLog(LPCSTR szMsg);

#ifdef USED_IN_OCX_TYPE 
#define IM_MACRO_GET_OCX_FILE_PATH(x) GetFullFilePathOCX(x)
#else 
#define IM_MACRO_GET_OCX_FILE_PATH(x) GetFullFilePath(x)
#endif 


////////////////////////////language macro//////////////////////////////////////////////
#define LANGKEY_MSG TEXT("String")

#define HDMACRO_READ_LANG(mcoKeyStr, mcoLanId) \
	CString strLanTxt; \
	g_lanManager.ReadString(mcoKeyStr, mcoLanId, strLanTxt); \


#define HDMACRO_INFO_MSG(mcoLanId)	\
	CString strLanTxt; \
	g_lanManager.ReadString("String", mcoLanId, strLanTxt); \
	SMInfoMessageBox(strLanTxt, 5000, TRUE)	\

//must include dlghider.h
#define HDMACRO_INFO_ASYNMSG(mcoLanId)	\
	CString strLanTxt; \
	g_lanManager.ReadString("String", mcoLanId, strLanTxt); \
	ShowAsynMsg(strLanTxt)	\


#define HDMACRO_STRING_READ_LANG(mcoLanId) \
	CString STRMACROTMPFMT; \
	g_lanManager.ReadString("String", mcoLanId, STRMACROTMPFMT); \

#define HDMACRO_STRING_READ_LANG2(strOut, mcoLanId) \
	g_lanManager.ReadString("String", mcoLanId, strOut); \

#define HDMACRO_QAINFO_MSG(mcoLanId) \
	 HDFuncPopQAMsg(mcoLanId); \

#define HDMACRO_QAINFO_MSG2(mcoLanId, pParent) \
	HDFuncPopQAMsg(mcoLanId, pParent); \

#define HDMACRO_INFO_MSG2(mcoLanId)	\
	CString strLanTxt; \
	g_lanManager.ReadString("String", mcoLanId, strLanTxt); \
	SMInfoMessageBox(strLanTxt, 10000, this)	\


#define HDMACRO_INFO_MSG_POP(mcoLanId, hInform) \
	CString strLanTxt; \
	g_lanManager.ReadString("String", mcoLanId, strLanTxt); \
	ShowPopMsg(strLanTxt, "", hInform, 0); \

#define HDMACRO_INFO_MSG_AUTOHIDEPOP(mcoLanId, hInform) \
	CString strLanTxt; \
	g_lanManager.ReadString("String", mcoLanId, strLanTxt); \
	ShowPopMsg(strLanTxt, "", hInform, 2); \


//////////////////////////////////////////////////////////////////////////


HWND GetSpecifiedParent(CWnd* pChildIn, const CRuntimeClass* pClass);

//////////////////////////////////////////////////////////////////////////

BOOL GetVersionIsHigherThanXp();
