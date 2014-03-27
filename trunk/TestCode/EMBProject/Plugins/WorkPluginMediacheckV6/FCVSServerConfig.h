// FCVSServerConfig.h: interface for the CFCVSServerConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FCVSSERVERCONFIG_H__39ED384C_867B_42B4_AB1C_FA107580D74D__INCLUDED_)
#define AFX_FCVSSERVERCONFIG_H__39ED384C_867B_42B4_AB1C_FA107580D74D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include <AFXTEMPL.H>
struct tagServiceConfig 
{
// 	CString strServiceName;
// 	CString strIP;
// 	CString strPort;
// 	//NetServiceType nServiceType;
// 	int nProtocol;
// 	BOOL bServer;
// 	UINT uStatus;
};

struct tagDetectPara 
{
	CString CLIP_WIDTH;
	CString CLIP_HEIGHT;
	CString TOP_SAFE_SECTION;
	CString BOTTOM_SAFE_SECTION;
	CString LEFT_SAFE_SECTION;
	CString RIGHT_SAFE_SECTION;
	CString BLACK_SCENE_THRESHOLD;
	CString COLOR_SCENE_PERCENTAGE;
	CString COLOR_STRIP_PERCENTAGE;
	CString STATIC_FRAME_PERCENTAGE;
	CString LINE_COUNTS_FOR_STRIP_DETECT;
	CString AMBIT_DEEMED_TO_SAME_PIXEL;
	CString UNDULATE_AMBIT_OF_Y;
	CString UNDULATE_AMBIT_OF_U;
	CString UNDULATE_AMBIT_OF_V;
	CString AUDIO_MUTE_THRESHOLD;
	CString	AUDIO_MUTE_PERIOD;
	CString	AUDIO_HIGH_THRESHOLD;
	CString	AUDIO_HIGH_PERIOD;
	CString	AUDIO_LOW_THRESHOLD;
	CString	AUDIO_LOW_PERIOD;
	CString	AUDIO_DISCONTINUITY_THRESHOLD;
    CString AUDIO_DISCONTINUITY_PERIOD;
	CString LOUDNESS_LOW_THRESHOLD;
	CString LOUDNESS_HIGH_THRESHOLD;
};

struct tagFileDriverPara 
{
	CString m_strClassName;
	CString m_strDriverName;
	CString m_strFileType;
	BOOL    m_bIsDefault;
	CString m_strDetectClipW;//����زĿ�
	CString m_strDetectClipH;//����زĸ�
};

struct tagResultLevel 
{
	CString m_strLevelName;
	int m_nBlackFrame;
	int m_nColorFrame;
	int m_nColorStrip;
	int m_nStillFrame;
	int m_nMuteDur;
	int m_nDiscontinuityDur;
	int m_nHighDur;
	int m_nLowDur;
	int m_nPlosiveDur;

	bool m_nLoudnessLow; //���
	bool m_nLoudnessHigh;
	int  m_nColorCast; //ƫɫ
	tagResultLevel()
	{
		m_nColorCast = 0;
	}
	int m_nMax;
	void CalcMax();
};
typedef CMap<CString,LPCSTR,tagDetectPara,tagDetectPara> MYTESTMAP;
class CFCVSServerConfig  
{
public:
	CFCVSServerConfig();
	virtual ~CFCVSServerConfig();
	
public:
	BOOL    InitConfig(CString XMLMsg);
	CString GetCurrentLocalTime();
	CString GetCurrentLocalTime2();
	CString GetExecutePath();
	BOOL    DirectoryFind(CString strPath,CString strDirctoryName,BOOL bNotFindCreate = TRUE);
	BOOL    FileFind(CString strPath,CString strFileName,BOOL bFindDelete = TRUE);
    //wj �޸�Ϊ��ȡ���ñ���[CanDetectedClipType]����
	//=============================================
	void    ReadDetectPara();
	CString GetDetectParaXml(CString strClipType);
	//=============================================
	void    ReadResultAnalysisPara();
	tagResultLevel   GetFilterLevel();
	//chenAdd for Detect
	//========================================================
	void    ReadFileDriverPara();
	CString GetClassNameByFileType(CString strFileType = "");
	CString GetFileTypeByClassName(CString strClassName);
	CString GetXmlByClassName(CString strClassName);
	//========================================================
	//wangjun add
	//�����ⲿϵͳ�ύ�������ز�������Ҫת��Ϊ�Զ�����ϵͳ����ʶ��ļ���ز�����/
	//ת����ϵ���������ñ�[CanDetectedClipTypeMap]������
	//===========================================================
	void ReadClipTypeMapPara();
	//===========================================================
	bool ChangeDetectPara(CString DetectName,tagDetectPara DetectParam);
public:
	CString      m_strEnvSetting;
	CString      m_strServerName;
	BOOL         m_RecordResult;
	CArray<tagServiceConfig,tagServiceConfig> m_ServiceConfig;
	//	tagDetectPara m_DetectPara;
	//chenAdd for Detect
	//==============================================================
	CArray<tagFileDriverPara,tagFileDriverPara> m_FileDriverParaArr;
	CMap<CString,LPCSTR,tagDetectPara,tagDetectPara> m_ClipTypeParaMap;
	CMap<CString,LPCSTR,CString,LPCSTR> m_CTMapMap;//[CanDetectedClipTypeMap]
	CStringArray  m_DectClipTypeArr;
	int           m_nTaskMaxCount;
	BOOL          m_bBroadCast;
	CString       m_strBroadCastServiceName;
	BOOL          m_bSaveXml;
	BOOL          m_bCheckAudio;
	
	BOOL          m_bUseInCaptureReview;
	BOOL		  m_bUseLocalHost;
	
	BOOL		  m_bCheckDuration;
	DWORD		  m_dwRetryTimes;
	DWORD		  m_dwRetryInterval;
	
	CString       m_strXmlName;
	CArray<tagResultLevel,tagResultLevel> m_ResultLevelArr;
	tagResultLevel       m_FilterLevel;
	bool                 m_bUseTrigger;
	//==============================================================
public:
	static CFCVSServerConfig* m_pConfig;
	
public: 
	BOOL		m_bUseSectionFunction ;
	
};

CFCVSServerConfig& GetFCVSServerConfig();

#endif // !defined(AFX_FCVSSERVERCONFIG_H__39ED384C_867B_42B4_AB1C_FA107580D74D__INCLUDED_)
