#pragma once

#include "IMPreDef.h"

//////////////NOTIFY MESSAGE ID//////////////////////////////////////////////

#define UM_AUDIOCAP_ENERGY_MSG      WM_USER+112		//声音采集音量大小回调消息
#define UM_AUDIOPLAY_ENERGY_MSG     WM_USER+113		//声音播放音量大小回调消息

#define UM_LANGUAGE_CHANDGED		WM_USER + 601
#define MSG_SYSTEM_TRAY				WM_USER + 602

#define MSG_ANI_INSERT				WM_USER + 603	//insert emotion

#define MSG_CHAT_RECV_DATA			WM_USER + 604	//recv chat data
//////////////////////////for transparent draw////////////////////////////////////////////////
#define  TRANS_PARENT_CLR			RGB(0,188,242)
//////////////////////////////////////////////////////////////////////////

#define APP_PROCESS_NAME			TEXT("EZworking.exe")
#define APP_MAIN_WND_NAME			TEXT("EZworking")
#define MAIN_FRAME_MUTEX			"{B468F42B-2FA6-4903-8ED6-7272E96BF237}"

//////////////////////////////////////////////////////////////////////////
#define INI_APP_KEY					TEXT("EZworking")

//////////////////////////////////////////////////////////////////////////
#define INI_APP_AUDIO_SETTING (TEXT("wave dev setting"))
#define INI_KEY_AUDIO_WAVEIN  (TEXT("wavein dev name"))
#define INI_KEY_AUDIO_WAVEOUT (TEXT("waveout dev name"))

#define INI_KEY_AUDIO_WAVEINID  (TEXT("wavein dev ID"))
#define INI_KEY_AUDIO_WAVEOUTID (TEXT("waveout dev ID"))


#define INI_APP_VIDEO_SETTING (TEXT("video dev setting"))
#define INI_KEY_VIDEO_MAINDEV  (TEXT("main dev name"))


#define CONTAINER_CAPTION_HEIGHT 24
#define CONTAINER_BOTTOM_BTN_OFFSET 5

#define IM_FONTSIZE_STATIC 100
#define IM_POINT_FONT_SIZE 100

#define IM_BORDER_L_X 2
#define IM_BORDER_L_Y 2

#define IM_BORDER_R_X 3
#define IM_BORDER_R_Y 3


#define PHOTO_NORMAL_WIDTH 40
#define PHOTO_NORMAL_HEIGHT 40
#define PHOTO_SMALL_WIDTH 20
#define PHOTO_SMALL_HEIGHT 20


#define IM_LOGIN_STEP_SIGNINED		0x1
#define IM_LOGIN_STEP_ALLUSERLIST	0x2
#define IM_LOGIN_STEP_ONLINELIST	0x4
#define IM_LOGIN_STEP_SELF_INFO		0x8
#define IM_LOGIN_STEP_ACTIVEGRP		0x10
#define IM_LOGIN_STEP_DEPARTLIST	0x20
#define IM_LOGIN_STEP_SUCCESS		0x3F

#define AVMODE_NONE				0
#define AVMODE_VIDEO_SINGLE		1
#define AVMODE_VIDEO_MULTI		2
#define AVMODE_AUDIO_SINGLE		3
#define AVMODE_AUDIO_MULTI		4
#define AVMODE_AVMSG			5

class CLanManager;
class CFont;
class Log;

class Gdiplus::Image;
class CxSoundPlayer;
class CDlgIMContainer;
class CDlgDebugLog;
class CIMUserMgr;
class CChatMgr;

class IClientNet;
class CDlgNetReceiver;
struct _ChannelInfo;

struct _SingleServerNode;
struct HD_AVCHANNEL_INFO;
class IHYTunnel;
class IHYMultiTunnel;

extern CLanManager				g_lanManager;
extern CFont					g_fontBase;
extern CDlgDebugLog*			g_pdlgDebugLog;

extern Log						g_log;
extern LOGFONT					g_logfont;
extern CDlgIMContainer*			g_pDlgMainContainer;
extern CMiniFrameWnd*			g_pAppFrm;

extern CIMUserMgr*				g_pUserMgr;
extern CChatMgr*				g_pChatMgr;

extern IClientNet*				g_pClientNet;
extern CDlgNetReceiver*			g_pNetReceiver;
extern _SingleServerNode		g_SvrInfo;

extern CxSoundPlayer*			g_pSoundPlayer;
extern BOOL						g_bStopStun;
extern 	_ChannelInfo            g_MultiChannelInfo; //for multi av call

extern HWND						g_hFrameUserInfo;

struct HD_AVCHANNEL_INFO
{
	IHYTunnel* pNetTunnel;
	BOOL bAudioStarted;
	BOOL bVideoStarted;
};

extern HD_AVCHANNEL_INFO		g_AVChannel;
extern IHYMultiTunnel*			g_MultiChannel;


struct HD_NET_STATE_INFO 
{
	HD_NET_STATE_INFO()
	{
		bValid = FALSE;
		fPacklost = 0; //0-100
		fkBitrate = 0;
		dwSndPacks = 0;
		dwRcvPacks = 0;
		bReset = TRUE;
	}

	BOOL  bValid;   //the return value of VIDEO_RMGR_GetQoS() 
	float fPacklost;
	float fkBitrate;
	DWORD dwSndPacks;
	DWORD dwRcvPacks;
	BOOL bReset;
};

struct HD_GLOBAL_INFO 
{
	IMUserInfo userInfo;

	CString strPw;
	BOOL bMsgOnShow;
	BOOL bLogined; 
	BOOL bRecvNetMsg; //if receive net msg
	BOOL bVerHigerXp;
	BOOL bSvrInited;
	BOOL bForceQuit;
	BOOL bDelaySignIn;
	CString strAppPath;
	CString strSysAppLocalPath;
	CString strSysDocRecvPath;
	CString strImgPath;
	CString strIniFile;
	CString strUserDataPath;
	CString strUserImgPath;
	CString strCCAppPath;
	CString strUserDocRecvPath;
	CString strRecTmpPath;

	int nSoundVol;
	int nMicVol;

	CSize nScreenSize; //in pixel
	CSize nMonitorSize; //in mm

	HD_NET_STATE_INFO netInfo;

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
		CString strEZWorking;
		CString strDepartTitleFmt;
		CString strDepartWithChildTitleFmt;
		CString strDepartInformTitleFmt;
		CString strDepartInformWithChildTitleFmt;
		CString strDepartUserBkTitleFmt;
		CString strDepartUserBkWithChildTitleFmt;
		CString strAVConnecting;
		CString strAVConnected;
		CString strDate;
		CString strTime;
		CString strChatMsgwav;
		CString strChatMsgwavAV;
			
	}commonStr;

	struct SYSTRAY_INFO
	{
		// CDlgIMContainer 对话框
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

	struct AV_STATE_INFO 
	{
		int nAVMode;  //AVMODE_NONE for none,
		HWND hOwner; //container that use the video
		HWND hLocalVideoOwner;
		USERID nCaller;
		BOOL m_bLocalVideoInited;
		HWND hLocalHolder;
		BOOL bVideoSended; //for multi av call
		AV_STATE_INFO()
		{
			nAVMode = AVMODE_NONE;
			hOwner = NULL;
			nCaller = 0;
			m_bLocalVideoInited = FALSE;
			hLocalHolder = NULL;
			bVideoSended = FALSE;
			hLocalVideoOwner = 0;
		}
	}avStateInfo;

	HD_GLOBAL_INFO()
	{
		bMsgOnShow = FALSE;
		bSvrInited = FALSE;
		bLogined = 0;
		bVerHigerXp = FALSE;
		bRecvNetMsg = TRUE;
		bForceQuit = FALSE;
		nMicVol = 70;
		nSoundVol = 70;
		nScreenSize.cx = 0;
		nScreenSize.cy = 0;
		nMonitorSize.cx = 0;
		nMonitorSize.cy = 0;
		bDelaySignIn = FALSE;

	}

};


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

struct IMACTIONSDATA 
{
	int nActionMode;
	int nRespond;
	int nActionId;
	DWORD dwRiserUserId;
	DWORD dwDesUserId;
	DWORD dwTriggerUserId;

	union ACTIONDATA
	{
		struct STRUCT_ACT_TEXT
		{
			TCHAR szTextFile[MAX_IM_TEXT_BUFFER];
		}dataText;

		struct STRUCT_ACT_LINK 
		{
			TCHAR szLink[MAX_IM_TEXT_BUFFER];
		}dataLink;

		struct STRUCT_ACT_FILE
		{
			TCHAR szFile[MAX_IM_TEXT_BUFFER];
			int nState; //0 =cancel, 1= ok, -1 = no act
		}dataFile;

		struct STRUCT_ACT_VIDEO
		{
			int nState;
		}dataVideo;

		struct STRUCT_ACT_AUDIO
		{
			int nState;
		}dataAudio;
	}data;

	IMACTIONSDATA()
	{
		nActionMode = ACTION_TYPE_ERR;
		nRespond = RESPOND_TYPE_INIT;
		dwRiserUserId = 0;
		dwDesUserId = 0;
		nActionId = -1;
		dwTriggerUserId = -1;
	}
};


//////////////////////////////////////////////////////////////////////////
//respond
extern HD_GLOBAL_INFO g_globalInfo;

#define MAX_CHAR_BUFFER 256
#define STRING_END '\0'

////{{ Added by wei on 2010/05/24 15:37
extern CString g_strMainVideoDev;
extern CString g_strSecondVideoDev;
extern int g_nMainVideoDev;  // DEV index in system, 0_based
extern int g_nSecondVideoDev; // dev in dex in system, 0_based

extern CString g_strCurWaveInDev;   // current wavein(mic) device name
extern CString g_strCurWaveOutDev;  // current waveout(speaker) devcie name
extern int     g_nCurWaveInDev;    // current mic device's index(0_based)
extern int     g_nCurWaveOutDev;   // current speaker device's index(0_based)

extern int		g_nCurDShowSoundInDev;
extern int		g_nCurDShowSoundOutDev;

// from system's current video devices, to get the main video dev's friendly name
BOOL GetMainVideo(CStringList &lstDevs);
// from system's current video devices, to get the second video dev's friendly name
BOOL GetSecondVideo(CStringList &lstDevs);

	// to get system's video device info when in App::Init, to classify which is the second
//and which one is the main device
BOOL InitVideoDevice();

// save audio setting to ini file
int GlobalSaveAudioSetting();

// read from ini and change the audio dev name to current dev id( maybe the audio dev is changed but the name is not changed)
int GlobalInitAudioSetting();

enum HD_AUDIEO_SETTING_TYPE
{
	AUDIO_SETTING_TYPE_IN,
	AUDIO_SETTING_TYPE_OUT
};

//
// 系统中音频有关变量说明
//
/*
g_nCurWaveInDev -- DSound序列号
g_nCurWaveOutDev -- DSound 序列号
g_strCurWaveInDev -- DShow的语音输入设备名称;
g_strCurWaveOutDev-- DShow的语音输出设备名称;
*/
//
// DSoundDshowMap() -- 序列号保持DSound的，但里面的friendlyname是dshow的
// 这样在进行语音通话的时候，就只需要取这个序列号就可以
// 语音通话是dsound 序列；
// 录制是dshow序列
//
void DSoundDshowMap(CStringList &strDevs, HD_AUDIEO_SETTING_TYPE typeIn);
void WaveDshowMap(CStringList &strDevs, HD_AUDIEO_SETTING_TYPE typeIn);
CString DoMatch(CString &strRef, CStringList &lstSrc);
CString GetMatch(CString &strSrc, CStringList &lstDst);

//
// 将DShow名字的音频设备转换为DShow的序列号
//
int DShowNameToDShowIndex(CString &strDshowName, HD_AUDIEO_SETTING_TYPE typeIn);


CString GetSoundTestFileName();
#define		MISC_NAME "Misc"

#pragma pack(push, r201001, 1)


#pragma pack(pop, r201001)




CString AfxGetAppPath();
CString AfxGetOcxPath();
CString AfxSplitPathName(LPCTSTR lpszPath, int nSplit);
//strfile formate = TEXT("subfolder\\filename.bmp");
CString GetFullFilePath(const CString& strFile);
//get the filepath based on .ocx directory.
CString GetFullFilePathOCX(const CString& strFile);

UINT    SMInfoMessageBox( LPCTSTR szMsg,DWORD dwWaitTime = 5000 , BOOL bCenterScreen = FALSE);
UINT    SMInfoMessageBox( LPCTSTR szMsg, LPCTSTR szHeader, DWORD dwWaitTime = 5000 , BOOL bCenterScreen = FALSE);
UINT    SMInfoMessageBox( UINT IDPrompt, DWORD dwWaitTime = 5000 , BOOL bCenterScreen = FALSE);
UINT	SMInfoMessageBox(LPCTSTR szMsg, DWORD dwWaitTime, CWnd* pParent);

UINT    SMQuestionMessageBox( LPCTSTR szMsg,DWORD dwWaitTime = INFINITE );
UINT    SMQuestionMessageBox( LPCTSTR szMsg, LPCTSTR szHeader, DWORD dwWaitTime = INFINITE, CWnd* pParent = NULL);
UINT    SMQuestionMessageBox( UINT IDPrompt,DWORD dwWaitTime = INFINITE );

void    SMNotifyBox( LPCTSTR szMsg );
void    SMNotifyBox( UINT IDPrompt );
void	SMGetResultMsg(WORD result, CHAR *szErrorMsg);
BOOL	GetVersionIsHigherThanXp();

//move rc2 to center of rc1 
CRect	GetCenterRect(CRect rc1, CRect rc2);

void	AddHDDebugLog(LPCSTR szMsg);

#ifdef USED_IN_OCX_TYPE 
#define IM_MACRO_GET_OCX_FILE_PATH(x) GetFullFilePathOCX(x)
#else 
#define IM_MACRO_GET_OCX_FILE_PATH(x) GetFullFilePath(x)
#endif 


int HDFuncPopQAMsg(int mcoLanId, CWnd* pParent = NULL);
//type 0 for net;
CString HDGetErrMsg(int nErrcode, int nUserId, LPCSTR szName, CString& strHeaderOut, int nType = 0);

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
void ReDrawStatic(CWnd* pWndIn);
void InvalidateItemBk(CWnd* pWndIn);

void ShowAsynMsg( LPCSTR strMsgIn , LPCSTR strHeader = NULL);

void ShowPopMsg(LPCSTR szMsg, LPCTSTR szTitle, HWND hInformWnd = NULL, int nPopType = 0);

//////////////////////////////////////////////////////////////////////////
typedef enum {
	_BOX_CALL_CIF = 1, 
	_BOX_CALL_VGA, 
	_BOX_CALL_DVD_HALF,
	_BOX_CALL_DVD,
	_BOX_CALL_HD_720P,  // 720P : default
	_BOX_CALL_HD_1080I,  // 1080I
	_BOX_CALL_HD_1080P,  // 1080P
	_BOX_CALL_CC		 //MVM

} _BOX_CALL_TYPE;
typedef struct HD_CALL_Format{
	int nWidth;
	int nHeight;
	int nModel; //0--var; 1--cbr
	int nFps;
	int nKBitrate; 
	int nKeyframeInterval;  // unit: second
	int nVbrQuality;  // var quality, only valid under VBR model

	HD_CALL_Format()
	{
		nWidth = 320;
		nHeight = 240;
		nModel = 1;
		nFps = 20;
		nKBitrate = 200; 
		nKeyframeInterval = 1;
		nVbrQuality = 30;
	}
	HD_CALL_Format& operator = (HD_CALL_Format &Ref)
	{
		if (this == &Ref)
			return *this;
		nWidth = Ref.nWidth;
		nHeight = Ref.nHeight;
		nModel = Ref.nModel;
		nFps = Ref.nFps;
		nKBitrate = Ref.nKBitrate;

		nVbrQuality = Ref.nVbrQuality;
		nKeyframeInterval = Ref.nKeyframeInterval;
		return *this;
	}

}HD_CALL_Format;

HD_CALL_Format CalcAVFormat(_BOX_CALL_TYPE nType);
BOOL GetVersionIsHigherThanXp();
