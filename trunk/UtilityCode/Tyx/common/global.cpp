#include "stdafx.h"
#include "global.h"
#include "BCMessageBox.h"
#include "LanManager.h"
#include "Log.h"
#include "SoundPlayer.h"
#include "DlgDebugLog.h"
#include "DlgIMContainer.h"
#include <MMSystem.h>
#include "IMUserMgr.h"
#include "ChatMgr.h"
#include "DlgNetReceiver.h"
#include "PopChatDlgDlg.h"
#include "NetCommutator.h"
#include "Export_hardwareadjust.h"
#include "Export_AV_MGR.h"


CLanManager				g_lanManager;
CFont					g_fontBase;
Log						g_log;
HD_GLOBAL_INFO			g_globalInfo;
LOGFONT					g_logfont;
CxSoundPlayer*			g_pSoundPlayer= NULL;
CDlgDebugLog*			g_pdlgDebugLog = NULL;
BOOL					g_bStopStun = FALSE;
CDlgIMContainer*		g_pDlgMainContainer = NULL;
CMiniFrameWnd*			g_pAppFrm = NULL;

CIMUserMgr*				g_pUserMgr = NULL;
CChatMgr*				g_pChatMgr = NULL;
IClientNet*				g_pClientNet = NULL;
CDlgNetReceiver*		g_pNetReceiver = NULL;
_SingleServerNode		g_SvrInfo;
HD_AVCHANNEL_INFO		g_AVChannel;
_ChannelInfo			g_MultiChannelInfo; //for multi av call
IHYMultiTunnel*	g_MultiChannel = NULL;
HWND				g_hFrameUserInfo;


// dshow声音名字
CString g_strCurWaveInDev;   // current wavein(mic) device name
CString g_strCurWaveOutDev;  // current waveout(speaker) devcie name


// dsound 序列的声音id
int     g_nCurWaveInDev;    // current mic device's index(0_based)
int     g_nCurWaveOutDev;   // current speaker device's index(0_based)


// dshouw 序列的声音
int		g_nCurDShowSoundInDev = 0;
int		g_nCurDShowSoundOutDev = 0;

CString g_strAsynMsg;
CString g_strAsynHeader;

UINT   SMInfoMessageBox( LPCTSTR szMsg,DWORD dwWaitTime , BOOL bCenterScreen)
{
	AddHDDebugLog( szMsg );
	CWnd* pParent = NULL;
	if(g_pDlgMainContainer && g_pDlgMainContainer->GetSafeHwnd() && g_pDlgMainContainer->IsWindowVisible())
	{
		pParent = g_pDlgMainContainer;
	}
	CMessageBox dlg( szMsg,MB_OK|MB_ICONINFORMATION,dwWaitTime, pParent);
	if (bCenterScreen)
	{
		dlg.CenterScreen();
	}
	return dlg.DoModal();
}
UINT	SMInfoMessageBox(LPCTSTR szMsg, DWORD dwWaitTime /*= 5000*/, CWnd* pParent)
{
	CMessageBox dlg( szMsg,MB_OK|MB_ICONINFORMATION,dwWaitTime, pParent);
	
	dlg.CenterScreen();
	return dlg.DoModal();
}


UINT    SMInfoMessageBox( UINT IDPrompt,DWORD dwWaitTime , BOOL bCenterScreen /*= FALSE*/)
{
	CString szMsg;
	g_lanManager.ReadTableString( IDPrompt,szMsg );

	return SMInfoMessageBox( szMsg,dwWaitTime, bCenterScreen );
}

UINT SMInfoMessageBox( LPCTSTR szMsg, LPCTSTR szHeader, DWORD dwWaitTime /*= 5000 */, BOOL bCenterScreen /*= FALSE*/ )
{
	AddHDDebugLog( szMsg );
	CWnd* pParent = NULL;
	if(g_pDlgMainContainer && g_pDlgMainContainer->GetSafeHwnd() && g_pDlgMainContainer->IsWindowVisible())
	{
		pParent = g_pDlgMainContainer;
	}
	CMessageBox dlg( szMsg,MB_OK|MB_ICONINFORMATION,dwWaitTime, pParent);
	if (bCenterScreen)
	{
		dlg.CenterScreen();
	}
	if (szHeader != NULL)
	{
		CString strHeader(szHeader);
		dlg.SetSpecialHeader(strHeader);
	}
	return dlg.DoModal();

}

UINT    SMQuestionMessageBox( LPCTSTR szMsg,DWORD dwWaitTime )
{
	AddHDDebugLog( szMsg );
	CWnd*  pParent= NULL;
	if(g_pDlgMainContainer && g_pDlgMainContainer->GetSafeHwnd() && g_pDlgMainContainer->IsWindowVisible())
	{
		pParent = g_pDlgMainContainer;
	}
	CMessageBox dlg( szMsg,MB_OKCANCEL|MB_ICONQUESTION,dwWaitTime, pParent);
	return dlg.DoModal();
}

UINT    SMQuestionMessageBox( UINT IDPrompt,DWORD dwWaitTime )
{
	CString szMsg;
	g_lanManager.ReadTableString( IDPrompt,szMsg );

	return SMQuestionMessageBox( szMsg,dwWaitTime );
}

UINT SMQuestionMessageBox( LPCTSTR szMsg, LPCTSTR szHeader, DWORD dwWaitTime /*= INFINITE */, CWnd* pParent /*= NULL*/)
{
	AddHDDebugLog( szMsg );
	if (!pParent)
	{
		if(g_pDlgMainContainer && g_pDlgMainContainer->GetSafeHwnd() && g_pDlgMainContainer->IsWindowVisible())
		{
			pParent = g_pDlgMainContainer;
		}
	}
	
	CMessageBox dlg( szMsg,MB_OKCANCEL|MB_ICONQUESTION,dwWaitTime, pParent);
	if (szHeader != NULL)
	{
		CString strHeader(szHeader);
		dlg.SetSpecialHeader(strHeader);
	}
	return dlg.DoModal();

}
//CDlgNotify g_dlgNotify;

void    SMNotifyBox( LPCTSTR szMsg )
{
#if defined MEETINGROOM_VERSION
	CDlgSysInfoTip *pDlg = new CDlgSysInfoTip(szMsg);
	pDlg->Create();
	if (pDlg->GetSafeHwnd() == NULL)
		delete pDlg;
#endif
}

void    SMNotifyBox( UINT IDPrompt )
{
	CString szMsg;
	g_lanManager.ReadTableString(IDPrompt, szMsg);
	SMNotifyBox( szMsg );
}

BOOL GetVersionIsHigherThanXp()
{

	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 6) 
	{
		return TRUE;
	}
	return FALSE;
}



CString GetFullFilePath(const CString& strFile )
{
	CString strTmpFileOut = AfxGetAppPath();
	//strTmpFileOut.TrimRight(TEXT("\\"));
	strTmpFileOut += strFile;
	return strTmpFileOut;
}

CString GetFullFilePathOCX( const CString& strFile )
{
	CString strTmpFileOut = AfxGetOcxPath();
	//strTmpFileOut.TrimRight(TEXT("\\"));
	strTmpFileOut += strFile;
	return strTmpFileOut;
}

CString AfxGetOcxPath()
{
	CWinApp* pApp = AfxGetApp();
	if(pApp && pApp->m_pszHelpFilePath != NULL)
	{
		return AfxSplitPathName(AfxGetApp()->m_pszHelpFilePath,0x0001|0x0002);
	}
	else
	{
		return NULL;
	}

}

CRect GetCenterRect( CRect rc1, CRect rc2 )
{
	CRect rcRet;
	int nOffX = (rc1.Width() - rc2.Width()) /2;
	rcRet.left = rc1.left + nOffX;
	rcRet.right = rcRet.left + rc2.Width();
	int nOffY = (rc1.Height() - rc2.Height())/2;
	rcRet.top = rc1.top + nOffY;
	rcRet.bottom = rcRet.top + rc2.Height();
	return rcRet;
}

CString g_strMainVideoDev;
CString g_strSecondVideoDev;
int g_nMainVideoDev;  // DEV index in system, 0_based
int g_nSecondVideoDev; // dev in dex in system, 0_based


//
// 设备配置的说明
// 在Box设备中，会带有2个设备，一个是USB、一个是1394采集卡。这2个设备的名称是固定的或者是有一定的规律的
// 以后如果设备确定了后，可以在配置文件里面写上采集卡的名称，从而可以确定那个设备是main、那个设备是Second
// 或者根据采集卡是不会随意被拔出来的，因此在系统刚启动的时候，就检查有那些视频采集设备，然后在检查那个设备
// 是模拟信号输入源，该设备就做为第二个采集设备，在整个系统启动的时候就固定下来！
//
BOOL GetMainVideo(CStringList &lstDevs)
{
	// check

	BOOL l_bFind = FALSE;
	int l_nIndex = 0;
	POSITION l_head = lstDevs.GetHeadPosition();
	while (l_head)
	{
		CString l_strCur = lstDevs.GetNext(l_head);
		g_strMainVideoDev = l_strCur;

		l_strCur.MakeUpper();
		if (l_strCur.Find("USB") >= 0)  // 因为我们配置的usb、卡是这样的名字分布
		{
			l_bFind = TRUE;
			break;
		}
		l_nIndex ++;
	}

	if (l_bFind)
	{
		g_nMainVideoDev = l_nIndex;
	}
	else
	{
		if (lstDevs.GetCount() > 0)
		{
			g_nMainVideoDev = 0;
			g_strMainVideoDev = lstDevs.GetHead();
		}
		else {
		g_nMainVideoDev = -1;
		g_strMainVideoDev.Empty();
		}
	}
	return l_bFind;
}
CString GetSoundTestFileName()
{
	CString l_SndTstName = g_globalInfo.strAppPath + MISC_NAME;
	l_SndTstName += "\\SoundTest.wav";
	return l_SndTstName;
}



int HDFuncPopQAMsg( int mcoLanId , CWnd* pParent/* = NULL*/)
{
	HDMACRO_STRING_READ_LANG(mcoLanId);
	return SMQuestionMessageBox(STRMACROTMPFMT, NULL, 300000, pParent);
}

void AddHDDebugLog( LPCSTR szMsg )
{
	if (g_pdlgDebugLog && g_pdlgDebugLog->GetSafeHwnd())
	{
		g_pdlgDebugLog->m_loglist.InsertString(-1, szMsg);
	}
}


HWND GetSpecifiedParent(CWnd* pChildIn, const CRuntimeClass* pClass)
{
	if (!pClass)
	{
		return NULL;
	}

	CWnd* pWnd = pChildIn;
	while(pWnd != NULL)
	{
		if (pWnd->IsKindOf(pClass))
		{
			break;
		}
		pWnd = pWnd->GetParent();
	}
	return pWnd? pWnd->GetSafeHwnd():NULL;
}

UINT AFX_CDECL DoAsynMsgThreadFunc( LPVOID paramIn )
{
	if (!g_strAsynMsg.IsEmpty())
	{
		SMInfoMessageBox(g_strAsynMsg, g_strAsynHeader, 10000, TRUE);
		g_strAsynMsg.Empty();
		g_strAsynHeader.Empty();
	}

	return 1;
}

void ShowAsynMsg( LPCSTR strMsgIn , LPCSTR strHeader /*= NULL*/)
{
	g_strAsynMsg = strMsgIn;
	g_strAsynHeader = strHeader;
	g_log.Print(TEXT("\n--receive net message: %s"), g_strAsynMsg);
	AfxBeginThread(DoAsynMsgThreadFunc, NULL);
	//SMInfoMessageBox(m_strMsg, m_strHeader, 10000, TRUE);
}

void ShowPopMsg( LPCSTR szMsg, LPCTSTR szTitle, HWND hInformWnd /*= NULL*/, int nPopType /*= 0*/ )
{
	CString strMsg(szMsg);
	CString strTitle(szTitle);
	CPopChatDlgDlg::CreatePopDlg(strTitle, strMsg, hInformWnd, (POP_MSG_TYPE)nPopType);
}



void ReDrawStatic( CWnd* pWndIn )
{
	if (!pWndIn || !::IsWindow(pWndIn->GetSafeHwnd()))
	{
		return;
	}
	CWnd* pSubWnd = pWndIn->GetWindow(GW_CHILD);
	while(pSubWnd != NULL)
	{
		if (pSubWnd->IsKindOf(RUNTIME_CLASS(CStatic)))
		{
			pSubWnd->Invalidate(TRUE);
		}

		pSubWnd = pSubWnd->GetWindow(GW_HWNDNEXT);
	}
}

BOOL InitVideoDevice()
{
	HANDLE l_handle = HardwareVideo_Create();
	if (!l_handle)
	{
		TRACEx(1, "Warn: FindAnalogVideoDevice failed to Create()\r\n");
		g_nMainVideoDev = -1;
		return FALSE;
	}
	CStringList l_lstDevs;
	int l_nCount = HardwareVideo_GetCapDevs(l_handle, l_lstDevs);
	
	if (l_nCount < 0) // no video dev
		g_nMainVideoDev = -1;
	else
	{
		g_nMainVideoDev = g_nMainVideoDev > l_nCount -1 ? 0 : g_nMainVideoDev; // g_nMainVideoDev is 0_based
	}

	if (l_handle)
	{
		HardwareVideo_Destroy(l_handle);
		l_handle = NULL;
	}
	TRACEx(1, "Info: cur video devices info\r\n{\r\n count = %d, index = %d\r\n", l_nCount, g_nMainVideoDev);

	return TRUE;
}

// ==========================================================================
// Func:CalcAVFormat
// Pur:  根据输入的通话类型，给出一个初始值
// Param:
// Return:
// Date: by wei on 2010/05/27 16:06
// ==========================================================================
HD_CALL_Format CalcAVFormat(_BOX_CALL_TYPE nType)
{
	HD_CALL_Format l_avFormat;
	l_avFormat.nFps = 20;
	switch (nType)
	{
	case _BOX_CALL_HD_720P:
		{
			l_avFormat.nWidth = 320;
			l_avFormat.nHeight = 240;
			l_avFormat.nKBitrate = 256; //1.2M 
		}
		break;
	case _BOX_CALL_VGA:
		{
			l_avFormat.nWidth = 640;
			l_avFormat.nHeight = 480;
			l_avFormat.nKBitrate = 512; 

		}
		break;
	case _BOX_CALL_DVD_HALF:
		{
			l_avFormat.nWidth = 720;
			l_avFormat.nHeight = 288;
			l_avFormat.nKBitrate = 512; //1.2M 

		}
		break;
	case _BOX_CALL_DVD:
		{
			l_avFormat.nWidth = 720;
			l_avFormat.nHeight = 576;
			l_avFormat.nKBitrate = 512; //1.2M 
		}
		break;
	case _BOX_CALL_CIF:
	default:
		{
			l_avFormat.nWidth = 320;
			l_avFormat.nHeight = 240;
			l_avFormat.nKBitrate = 256; //1.2M 

		}
		break;
	}

	////{{ Added by wei on 2010/05/28 10:46
	// for testing, just get it from ini file
	char l_szValue[_MAX_PATH];
#define INI_APP_HD_VIDEOFORMAT (TEXT("Video Format(only for testing)"))
#define INI_KEY_HD_VIDEOWIDTH (TEXT("Video Width"))
#define INI_KEY_HD_VIDEOHEIGHT (TEXT("Video Height"))
#define INI_KEY_HD_1V3VIDEOWIDTH (TEXT("1V3Video Width"))
#define INI_KEY_HD_1V3VIDEOHEIGHT (TEXT("1V3Video Height"))

#define INI_KEY_HD_VIDEOFPS		(TEXT("Video Fps"))  //20
#define INI_KEY_HD_VIDEOKBPS (TEXT("Video Kbps"))	//128
#define INI_KEY_HD_VIDEOKEYFRAMEINTERVAL (TEXT("Video keyframeinterval(second)"))  //3
#define INI_KEY_HD_VIDEOVBRQUALITY (TEXT("Video vbrquality"))						//26
#define INI_KEY_HD_VIDEOENCMODEL (TEXT("Video encode model(0_vbr_1_cbr)"))			//1

#define INI_KEY_HD_1V3VIDEOFPS		(TEXT("1V3Video Fps"))  //20
#define INI_KEY_HD_1V3VIDEOKBPS (TEXT("1V3Video Kbps"))	//128
#define INI_KEY_HD_1V3VIDEOKEYFRAMEINTERVAL (TEXT("1V3Video keyframeinterval(second)"))  //3
#define INI_KEY_HD_1V3VIDEOVBRQUALITY (TEXT("1V3Video vbrquality"))						//26
#define INI_KEY_HD_1V3VIDEOENCMODEL (TEXT("1V3Video encode model(0_vbr_1_cbr)"))			//1


	// width
	/*
	if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOWIDTH, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nWidth = atoi (l_szValue);
		}
		else {
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOWIDTH, "640",
				g_globalInfo.strIniFile);
		}*/
	
	/*
	// height
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOHEIGHT, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nHeight = atoi (l_szValue);
		}
		else {
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOHEIGHT, "480",
				g_globalInfo.strIniFile);
		}*/
	
	// fps
	/*
	if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOFPS, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nFps = atoi (l_szValue);
		}
		else {
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOFPS, "20",
				g_globalInfo.strIniFile);
		}
	
	// bps
	if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOKBPS, "",
		l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		l_avFormat.nKBitrate = atoi (l_szValue);
	}
	else {
		WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOKBPS, "512",
			g_globalInfo.strIniFile);
	}*/

	// keyframe interval
	if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOKEYFRAMEINTERVAL, "",
		l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		l_avFormat.nKeyframeInterval = atoi (l_szValue);
	}
	else {
		l_avFormat.nKeyframeInterval = 1;
		WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOKEYFRAMEINTERVAL, "1",
			g_globalInfo.strIniFile);
	}
	// encode model
	if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOENCMODEL, "",
		l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		l_avFormat.nModel = atoi (l_szValue);
	}
	else {
		l_avFormat.nModel = 1; // default to cbr
		WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOENCMODEL, "1",
			g_globalInfo.strIniFile);
	}

	// vbr quality
	if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOVBRQUALITY, "",
		l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		l_avFormat.nVbrQuality = atoi (l_szValue);
	}
	else {
		l_avFormat.nVbrQuality = 30;
		WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_VIDEOVBRQUALITY, "30",
			g_globalInfo.strIniFile);
	}
	//}} End added

	//{{ Added by wei on 2010/06/22 13:20
	// to force 1-to-3 to cif
	if (_BOX_CALL_CIF == nType)
	{
		l_avFormat.nWidth = 320;
		l_avFormat.nHeight = 240;
		l_avFormat.nFps = 15;

		/*
// width
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOWIDTH, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nWidth = atoi (l_szValue);
		}
		else {
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOWIDTH, "320",
				g_globalInfo.strIniFile);
		}
		// height
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOHEIGHT, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nHeight = atoi (l_szValue);
		}
		else {
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOHEIGHT, "240",
				g_globalInfo.strIniFile);
		}
*/


		// fps
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOFPS, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nFps = atoi (l_szValue);
		}
		else {
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOFPS, "20",
				g_globalInfo.strIniFile);
		}
		// bps
		/*
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOKBPS, "",
					l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
				{
					l_avFormat.nKBitrate = atoi (l_szValue);
				}
				else {
					WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOKBPS, "128",
						g_globalInfo.strIniFile);
				}*/
		

		// keyframe interval
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOKEYFRAMEINTERVAL, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nKeyframeInterval = atoi (l_szValue);
		}
		else {
			l_avFormat.nKeyframeInterval = 1;
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOKEYFRAMEINTERVAL, "3",
				g_globalInfo.strIniFile);
		}
		// encode model
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOENCMODEL, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nModel = atoi (l_szValue);
		}
		else {
			l_avFormat.nModel = 1; // default to cbr
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOENCMODEL, "1",
				g_globalInfo.strIniFile);
		}
		l_avFormat.nModel = 1;

		// vbr quality
		if (GetPrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOVBRQUALITY, "",
			l_szValue, _MAX_PATH, g_globalInfo.strIniFile) > 0)
		{
			l_avFormat.nVbrQuality = atoi (l_szValue);
		}
		else {
			l_avFormat.nVbrQuality = 30;
			WritePrivateProfileString(INI_APP_HD_VIDEOFORMAT, INI_KEY_HD_1V3VIDEOVBRQUALITY, "26",
				g_globalInfo.strIniFile);
		}

	}
	//}} End added

	return l_avFormat;
}

void InvalidateItemBk( CWnd* pWndIn )
{
	if (!pWndIn)
	{
		return;
	}

	CWnd* pPar = pWndIn->GetParent();
	if (!pPar)
	{
		return;
	}

	CRect rcWnd;
	pWndIn->GetWindowRect(rcWnd);
	pPar->ScreenToClient(rcWnd);
	pPar->InvalidateRect(&rcWnd, TRUE);
}

// ==========================================================================
// Func: GlobalSaveAudioSetting
// Pur:  write audio setting(configure) to ini file
// Param:
// Return:
// Date: by wei on 2010/06/01 10:21
// ==========================================================================
// save audio setting to ini file
int GlobalSaveAudioSetting()
{
	char l_szDevID[_MAX_PATH];
	// write wavein
	WritePrivateProfileString (INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEIN, 
		g_strCurWaveInDev, g_globalInfo.strIniFile);

	sprintf_s(l_szDevID, _MAX_PATH, "%d", g_nCurWaveInDev);
	WritePrivateProfileString (INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEINID, 
		l_szDevID, g_globalInfo.strIniFile);

	WritePrivateProfileString(INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEOUT,
		g_strCurWaveOutDev, g_globalInfo.strIniFile);

	sprintf_s(l_szDevID, _MAX_PATH, "%d", g_nCurWaveOutDev);
	WritePrivateProfileString (INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEOUTID, 
		l_szDevID, g_globalInfo.strIniFile);


	// main video dev
	WritePrivateProfileString(INI_APP_VIDEO_SETTING, INI_KEY_VIDEO_MAINDEV, g_strMainVideoDev,
		g_globalInfo.strIniFile);



	return 2;
}

// read from ini and change the audio dev name to current dev id( maybe the audio dev is changed but the name is not changed)
int GlobalInitAudioSetting()
{
	char l_szDevID[_MAX_PATH];
	// write wavein
	if (GetPrivateProfileString (INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEIN, "",
		l_szDevID, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		g_strCurWaveInDev = l_szDevID;
	}


	if(GetPrivateProfileString (INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEINID, "",
		l_szDevID, _MAX_PATH, g_globalInfo.strIniFile) >0)
	{
		g_nCurWaveInDev = atoi (l_szDevID);
	}

	if(GetPrivateProfileString(INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEOUT, "",
		l_szDevID, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		g_strCurWaveOutDev = l_szDevID;
	}

	if (GetPrivateProfileString (INI_APP_AUDIO_SETTING, INI_KEY_AUDIO_WAVEOUTID, "",
		l_szDevID, _MAX_PATH, g_globalInfo.strIniFile) > 0)
	{
		g_nCurWaveOutDev = atoi (l_szDevID);
	}


	// query current wave devices to verify whether the wave devices is changed or not
	// note: the 
	CStringList l_lstWavein, l_lstWaveout;
	int l_nIndex = -1;
	BOOL l_bFind = FALSE;
	POSITION l_head = NULL;

	// get current devs
	DSoundDshowMap(l_lstWavein, AUDIO_SETTING_TYPE_IN);
	DSoundDshowMap(l_lstWaveout, AUDIO_SETTING_TYPE_OUT);

	// do query cap dev
	l_head = l_lstWavein.GetHeadPosition();
	l_nIndex = 0;
	l_bFind = FALSE;
	while (l_head)
	{
		if (g_strCurWaveInDev == l_lstWavein.GetNext(l_head))
		{
			l_bFind = TRUE;
			g_nCurWaveInDev = l_nIndex;
			break;
		}
		l_nIndex ++;
	}
	if (!l_bFind)
	{
		if (l_lstWavein.GetCount() > 0)
		{
			g_nCurWaveInDev = 0;
			g_strCurWaveInDev = l_lstWavein.GetHead();
		}
		else
		{
			g_nCurWaveInDev = -1;
			g_strCurWaveInDev.Empty();
		}
	}

	// do query play dev
	l_bFind = FALSE;
	l_head = l_lstWaveout.GetHeadPosition();
	l_nIndex = 0;
	while (l_head)
	{
		if (g_strCurWaveOutDev == l_lstWaveout.GetNext(l_head))
		{
			l_bFind = TRUE;
			g_nCurWaveOutDev = l_nIndex;
			break;
		}
		l_nIndex ++;
	}
	if (!l_bFind)
	{
		if (l_lstWaveout.GetCount() > 0)
		{
			g_nCurWaveOutDev = 0;
			g_strCurWaveOutDev = l_lstWaveout.GetHead();
		}
		else
		{
			g_nCurWaveOutDev = -1;
			g_strCurWaveOutDev.Empty();
		}
	}



	// trace to bug file
	TRACEx(1, "GlobalInitAudioSetting\r\n{\r\n Wavein string = %s\r\n Wavein id = %d\r\n Waveout string = %s\r\n Waveout id = %d\r\n}\r\n",
		g_strCurWaveInDev, g_nCurWaveInDev, g_strCurWaveOutDev, g_nCurWaveOutDev);
 

	return 2;
}


void DSoundDshowMap(CStringList &strDevs, HD_AUDIEO_SETTING_TYPE typeIn)
{
	int l_nCount = 0;
	int l_nIndex = 0;
	CString l_strValue;
	char l_szName[260];
	HANDLE l_hAudio = NULL;
	CStringList l_lstDevs;   // from dshow

	strDevs.RemoveAll();
	l_hAudio = HardwareAudio_Create();

	// wavein
	if (AUDIO_SETTING_TYPE_IN == typeIn)
	{
		// get from dsound
		l_nIndex = 0;
		l_nCount = AUDIO_MGR_GetCapDevicesCount();
		while (l_nIndex < l_nCount)
		{
			if (AUDIO_MGR_GetCapDeviceName(l_nIndex, l_szName, 260) > 0)
				strDevs.AddTail(l_szName);
			l_nIndex ++;
		}

		// get from dshow
		HardwareAudio_GetCapDevs(l_hAudio, l_lstDevs);

		TRACEx(1, "-----AudioCap Count dev: %d, dsound = %d\r\n", l_nCount, l_lstDevs.GetCount());

	}
	else if (AUDIO_SETTING_TYPE_OUT == typeIn)
	{
		l_nIndex = 0;
		l_nCount = AUDIO_MGR_GetPlayDevicesCount();
		while (l_nIndex < l_nCount)
		{	
			if (AUDIO_MGR_GetPlayDeviceName(l_nIndex, l_szName, 26) > 0)
				strDevs.AddTail(l_szName);
			l_nIndex++;
		}

		// get from dshow
		HardwareAudio_GetPlayDevs(l_hAudio, l_lstDevs);
		TRACEx(1, "-----AudioPlay Count dev: %d, dsound = %d\r\n", l_nCount, l_lstDevs.GetCount());
	}

	// release 
	HardwareAudio_Destroy(l_hAudio);
	l_hAudio = NULL;

	//
	// map from dsound to dshow: only map name, but do not change the sequence!!
	// WARN: only change name from DSound to DShow, but do not change the sequence!!
	//
	CString l_strMatched;
	POSITION l_head = strDevs.GetHeadPosition();
	while (l_head)
	{
		CString &l_strCur = strDevs.GetNext(l_head);
		l_strMatched = DoMatch(l_strCur, l_lstDevs);
		TRACEx(1, "DSound match: [%s] to: [%s]\r\n", l_strCur, l_strMatched);
		l_strCur = l_strMatched;
	}

}


CString DoMatch(CString &strRef, CStringList &lstSrc)
{
	int l_nRefLen = strRef.GetLength();
	int l_nLen = 0;
	POSITION l_head = lstSrc.GetHeadPosition();
	while (l_head)
	{
		CString &l_strValue = lstSrc.GetNext(l_head);
		l_nLen = l_strValue.GetLength();

		TRACEx(1, "DShow Audio dev = %s\r\n", l_strValue);

		if (l_nRefLen > l_nLen)
		{
			if (strRef.Find(l_strValue) == 0) // found, return l_strValue;
			{
				return l_strValue;
			}
		}
		else  // reverse 
		{
			if (l_strValue.Find(strRef) == 0)
			{
				return l_strValue;
			}
		}
	}

	TRACEx(1, "Err: DoMatch can not find: %s\r\n", strRef);

	return "";
}


void WaveDshowMap(CStringList &strDevs, HD_AUDIEO_SETTING_TYPE typeIn)
{
	CStringList l_lstWaveDevs;
	int l_nIndex = -1;
	int l_nCout = 0;

	WAVEINCAPS l_waveinCap;
	WAVEOUTCAPS l_waveoutCap;
	MMRESULT l_hr = -1;

	CString l_strSrc;
	CString l_strMatched;

	// wavein
	if (AUDIO_SETTING_TYPE_IN == typeIn)
	{
		l_nCout = waveInGetNumDevs();
		for (l_nIndex = 0; l_nIndex < l_nCout; l_nIndex++)
		{
			ZeroMemory(&l_waveinCap, sizeof (l_waveinCap));
			l_hr = waveInGetDevCaps(l_nIndex, &l_waveinCap, sizeof (l_waveinCap));
			if (MMSYSERR_NOERROR  == l_hr)
			{
				l_strSrc = l_waveinCap.szPname;
				l_strMatched = GetMatch(l_strSrc, strDevs);
				l_lstWaveDevs.AddTail(l_strMatched);
			}
		}
	}
	else if (AUDIO_SETTING_TYPE_OUT == typeIn)
	{
		l_nCout = waveOutGetNumDevs();
		for (l_nIndex = 0; l_nIndex < l_nCout; l_nIndex++)
		{
			ZeroMemory(&l_waveoutCap, sizeof (l_waveoutCap));
			l_hr = waveOutGetDevCaps(l_nIndex, &l_waveoutCap, sizeof (l_waveoutCap));
			if (MMSYSERR_NOERROR  == l_hr)
			{
				l_strSrc = l_waveoutCap.szPname;
				l_strMatched = GetMatch(l_strSrc, strDevs);
				l_lstWaveDevs.AddTail(l_strMatched);
			}
		}
	}

	// change the input
	strDevs.RemoveAll();
	POSITION l_head = l_lstWaveDevs.GetHeadPosition();
	while (l_head)
	{
		strDevs.AddTail(l_lstWaveDevs.GetNext(l_head));
	}
}

CString GetMatch(CString &strSrc, CStringList &lstDst)
{
	POSITION l_head = lstDst.GetHeadPosition();
	int l_nIndex = 0;
	while (l_head)
	{
		CString &l_curValue = lstDst.GetNext(l_head);
		if (l_curValue.Find(strSrc) == 0) // find a match
		{
			TRACEx(1, "GetMach: src = %s, matched = %s, Index = %d\r\n", strSrc, l_curValue, l_nIndex);
			return l_curValue;

		}
		l_nIndex ++;
	}
	return "";
}

// 从DShow名称中得到其序列号
int DShowNameToDShowIndex(CString &strDshowName, HD_AUDIEO_SETTING_TYPE typeIn)
{
	int l_nCount = 0;
	int l_nIndex = 0;
	CString l_strValue;
	char l_szName[260];
	HANDLE l_hAudio = NULL;
	CStringList l_lstDevs;   // from dshow

	l_hAudio = HardwareAudio_Create();

	// wavein
	if (AUDIO_SETTING_TYPE_IN == typeIn)
	{
		// get from dshow
		HardwareAudio_GetCapDevs(l_hAudio, l_lstDevs);

		TRACEx(1, "-----AudioCap Count dev: %d, dsound = %d\r\n", l_nCount, l_lstDevs.GetCount());

	}
	else if (AUDIO_SETTING_TYPE_OUT == typeIn)
	{
		// get from dshow
		HardwareAudio_GetPlayDevs(l_hAudio, l_lstDevs);
		TRACEx(1, "-----AudioPlay Count dev: %d, dsound = %d\r\n", l_nCount, l_lstDevs.GetCount());
	}

	// release 
	HardwareAudio_Destroy(l_hAudio);
	l_hAudio = NULL;

	//
	// map from dsound to dshow: only map name, but do not change the sequence!!
	// WARN: only change name from DSound to DShow, but do not change the sequence!!
	//
	if (l_lstDevs.GetCount() < 1)
		return -1;
	else
	{
		l_nIndex = 0;
		POSITION l_head = l_lstDevs.GetHeadPosition();
		while (l_head)
		{
			CString &l_strCur = l_lstDevs.GetNext(l_head);
			if (l_strCur == strDshowName)
				return l_nIndex;
			l_nIndex++;
		}
	}

	// if do not find, just try the first one
	return 0;
}