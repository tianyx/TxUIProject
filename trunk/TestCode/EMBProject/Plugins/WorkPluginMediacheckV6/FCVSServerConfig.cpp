// FCVSServerConfig.cpp: implementation of the CFCVSServerConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MediaCheck.h"
#include "EmbStructDef.h"
//#include "FCVSServer.h"
#include "FCVSServerConfig.h"
//#include "COCOLIB20/Coco_COM.h"
#include ".\JS_FileDrive\DriveMgr_MXF.h"
#include "XMLOp.h"
#include <FSTREAM>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void tagResultLevel::CalcMax()
{
	m_nMax = 0;
	m_nMax<m_nBlackFrame  ? m_nMax = m_nBlackFrame  :m_nMax = m_nMax;
	m_nMax<m_nColorCast   ? m_nMax = m_nColorCast   :m_nMax = m_nMax;
	m_nMax<m_nColorFrame  ? m_nMax = m_nColorFrame  :m_nMax = m_nMax;
	m_nMax<m_nColorStrip  ? m_nMax = m_nColorStrip  :m_nMax = m_nMax;
	m_nMax<m_nHighDur     ? m_nMax = m_nHighDur     :m_nMax = m_nMax;
	m_nMax<m_nLowDur      ? m_nMax = m_nLowDur      :m_nMax = m_nMax;
	m_nMax<m_nMuteDur     ? m_nMax = m_nMuteDur     :m_nMax = m_nMax;
	m_nMax<m_nStillFrame  ? m_nMax = m_nStillFrame  :m_nMax = m_nMax;
}

CFCVSServerConfig::CFCVSServerConfig()
{
    m_strEnvSetting = "/etc/iip/Environment";
	m_pConfig = this;
	m_RecordResult = FALSE;
	m_strBroadCastServiceName = "";
	m_FilterLevel.m_strLevelName = "";
	m_bUseTrigger = FALSE;
	m_bCheckAudio = FALSE;
	m_bUseInCaptureReview = FALSE;
}

CFCVSServerConfig::~CFCVSServerConfig()
{
	m_ServiceConfig.RemoveAll();
	m_FileDriverParaArr.RemoveAll();
	m_ClipTypeParaMap.RemoveAll();
	m_CTMapMap.RemoveAll();
	m_DectClipTypeArr.RemoveAll();
	m_ResultLevelArr.RemoveAll();
	ASSERT(m_pConfig == this);
	m_pConfig = 0;
}

CFCVSServerConfig* CFCVSServerConfig::m_pConfig =0;

CFCVSServerConfig& GetFCVSServerConfig()
{
	ASSERT(CFCVSServerConfig::m_pConfig);
	return *CFCVSServerConfig::m_pConfig;
}

BOOL CFCVSServerConfig::InitConfig(CString XMLMsg)
{
	int i ;
	LoadDict(".\\dict.xml");
	ST_FCVSCONFIGINFO configGet;
	configGet.FromString(XMLMsg);	
	
	m_bCheckAudio = configGet.bCheckAudio;
	m_bSaveXml = configGet.bSaveXML;
	m_strServerName = configGet.serverName;
	m_bUseSectionFunction = configGet.bUseSection;
	m_nTaskMaxCount = 1; //只能执行一个任务
	for (i =0 ;i < configGet.fileDriverCount;i++)
	{
		tagFileDriverPara Driver;
		Driver.m_bIsDefault = configGet.fileDriver[i].isDefault;
		Driver.m_strClassName = configGet.fileDriver[i].classString;
		Driver.m_strDriverName = configGet.fileDriver[i].driverName;
		Driver.m_strDetectClipH.Format("%d",configGet.fileDriver[i].DetectedCLIP_HEIGHT);
		Driver.m_strDetectClipW.Format("%d",configGet.fileDriver[i].DetectedCLIP_WIDTH);
		Driver.m_strFileType = configGet.fileDriver[i].fileType;
		m_FileDriverParaArr.Add(Driver);
	}
	for (i=0;i < configGet.detectLevelCount;i++)
	{
		tagResultLevel Level;
		Level.m_nBlackFrame = configGet.detectLevel[i].blackFrame;
		Level.m_strLevelName = configGet.detectLevel[i].levelName;
		Level.m_nColorFrame = configGet.detectLevel[i].colorFrame;
		Level.m_nColorStrip = configGet.detectLevel[i].colorSripe;
		Level.m_nStillFrame = configGet.detectLevel[i].staticFrame;
		Level.m_nMuteDur = configGet.detectLevel[i].muteDur;
		Level.m_nLowDur = configGet.detectLevel[i].LowDur;
		Level.m_nHighDur = configGet.detectLevel[i].HighDur;
		Level.CalcMax();//计算最大值， 用来切片添加多审长度
		m_ResultLevelArr.Add(Level);
	}
	for (i=0; i < configGet.detectThresholodCount ; i++)
	{
		tagDetectPara detectThr;
		//video
		detectThr.TOP_SAFE_SECTION.Format("%lf",configGet.detectThresholod[i].TOP_SAFE_SECTION);
		detectThr.BOTTOM_SAFE_SECTION.Format("%lf",configGet.detectThresholod[i].BOTTOM_SAFE_SECTION);
		detectThr.LEFT_SAFE_SECTION.Format("%lf",configGet.detectThresholod[i].LEFT_SAFE_SECTION);
		detectThr.RIGHT_SAFE_SECTION.Format("%lf",configGet.detectThresholod[i].RIGHT_SAFE_SECTION);
		detectThr.BLACK_SCENE_THRESHOLD.Format("%lf",configGet.detectThresholod[i].BLACK_SCENE_THRESHOLD);
		detectThr.COLOR_SCENE_PERCENTAGE.Format("%lf",configGet.detectThresholod[i].COLOR_SCENE_PERCENTAGE);
		detectThr.COLOR_STRIP_PERCENTAGE.Format("%lf",configGet.detectThresholod[i].COLOR_STRIP_PERCENTAGE);
		detectThr.STATIC_FRAME_PERCENTAGE.Format("%lf",configGet.detectThresholod[i].STATIC_FRAME_PERCENTAGE);
		detectThr.LINE_COUNTS_FOR_STRIP_DETECT.Format("%lf",configGet.detectThresholod[i].LINE_COUNTS_FOR_STRIP_DETECT);
		detectThr.AMBIT_DEEMED_TO_SAME_PIXEL.Format("%lf",configGet.detectThresholod[i].AMBIT_DEEMED_TO_SAME_PIXEL);
		detectThr.UNDULATE_AMBIT_OF_U.Format("%lf",configGet.detectThresholod[i].UNDULATE_AMBIT_OF_U);
		detectThr.UNDULATE_AMBIT_OF_V.Format("%lf",configGet.detectThresholod[i].UNDULATE_AMBIT_OF_V);
		detectThr.UNDULATE_AMBIT_OF_Y.Format("%lf",configGet.detectThresholod[i].UNDULATE_AMBIT_OF_Y);
		//audio
		detectThr.AUDIO_LOW_THRESHOLD.Format("%lf",configGet.detectThresholod[i].AUDIO_Low_THRESHOLD);
		detectThr.AUDIO_LOW_PERIOD.Format("%lf",configGet.detectThresholod[i].AUDIO_Low_PERIOD);
		detectThr.AUDIO_HIGH_THRESHOLD.Format("%lf",configGet.detectThresholod[i].AUDIO_High_THRESHOLD);
		detectThr.AUDIO_HIGH_PERIOD.Format("%lf",configGet.detectThresholod[i].AUDIO_High_PERIOD);
		detectThr.AUDIO_MUTE_THRESHOLD.Format("%lf",configGet.detectThresholod[i].AUDIO_Mute_THRESHOLD);
		detectThr.AUDIO_MUTE_PERIOD.Format("%lf",configGet.detectThresholod[i].AUDIO_Mute_PERIOD);
		CString indexstring;
		indexstring = TEXT("电影");
		m_ClipTypeParaMap.SetAt(indexstring,detectThr);
	}


	return TRUE;
// 	CString strLocal;
// //	strLocal = GetNetMsg()->GetLocalAddress();
// 
// 	CResString EnvSetting(m_strEnvSetting);
// // 	m_dbURL			= EnvSetting["Database"].GetAsString("DB:\\\\sa:@dbassvr\\pdc");
// // 	m_strDBType     = EnvSetting["DataBaseType"].GetAsString("SQLSERVER");
// 	m_strServerName = EnvSetting["ServerName"].GetAsString("");
// 	m_RecordResult  = EnvSetting["bRecordResult"].GetAsBool(FALSE);
// 	m_nTaskMaxCount = EnvSetting["TaskMaxCount"].GetAsInteger(1);//chenAdd for Detect
// 	//section
// 	m_bUseSectionFunction = EnvSetting["bUseSectionFunction"].GetAsBool(FALSE);	
// 	if (m_bUseSectionFunction)
// 	{
// 		m_bSaveXml = TRUE;
// 		m_strXmlName = "";
// 	}
// 	else
// 	{
// 		m_bSaveXml      = EnvSetting["bSaveXml"].GetAsBool(FALSE);
// 		m_strXmlName    = EnvSetting["XmlName"].GetAsString("");
// //		m_bSaveDB       = EnvSetting["bSaveDB"].GetAsBool(TRUE);
// 	}
// 	//section
// 	m_bBroadCast    = EnvSetting["bBroadCast"].GetAsBool(FALSE);//chenAdd for Detect
// 	//m_bSaveDB       = EnvSetting["bSaveDB"].GetAsBool(TRUE);
// 	m_bUseLocalHost = EnvSetting["bLocalHost"].GetAsBool(FALSE); //LOCALHOST
// 	m_bCheckAudio   = EnvSetting["bCheckAudio"].GetAsBool(TRUE);
// 	m_bUseInCaptureReview = EnvSetting["bUseInCaptureReview"].GetAsBool(FALSE);
// 
//    // m_bUseSectionFunction = EnvSetting["bUseSectionFunction"].GetAsBool(FALSE);
// 	if (m_bUseLocalHost)
// 	{
// 		strLocal = "127.0.0.1";
// 	}
// 
// 	CResString ServiceSetting("/etc/iip/Service");
// 	CString strPath = _T("/etc/iip/");
// 	CStringArray ServiceListArr;
// 	ServiceSetting["ServiceList"].GetAsStringArray(ServiceListArr);
// 	CString strTemp = "";
// 	for (int i = 0 ; i < ServiceListArr.GetSize() ; i ++)
// 	{
// 		strTemp = strPath + ServiceListArr.GetAt(i);
// 		CResString TempRes(strTemp);
// 		tagServiceConfig ServiceConfig;
// 		ServiceConfig.strServiceName = TempRes["ServiceName"].GetAsString("");
// 		ServiceConfig.strIP = TempRes["IP"].GetAsString(strLocal);
// 		if (ServiceConfig.strIP.CompareNoCase(strLocal) != 0)
// 		{
// 			ServiceConfig.strIP = strLocal;
// 		}
// 	    ServiceConfig.strPort = TempRes["Port"].GetAsString("");
// 		strTemp = TempRes["ServiceType"].GetAsString("");
// 		if (strTemp.CompareNoCase("BroadCast") == 0)
// 		{
// 			ServiceConfig.nServiceType = NET_SERVICE_BROADCAST;
// 		}
// 		else
// 		{
// 			ServiceConfig.nServiceType = NET_SERVICE_OP;
// 		}
// 		strTemp = TempRes["Protocol"].GetAsString("");
// 		if (strTemp.CompareNoCase("UDP") == 0)
// 		{
// 			ServiceConfig.nProtocol = SOCK_DGRAM;
// 		}
// 		else
// 		{
// 			ServiceConfig.nProtocol = SOCK_STREAM;
// 		}
// 		ServiceConfig.bServer = TempRes["Server"].GetAsBool(TRUE);
// 		ServiceConfig.uStatus = EVT_STOPSUCCESS;
// 		m_ServiceConfig.Add(ServiceConfig);
// 	}
// 	ReadDetectPara();
// 	ReadClipTypeMapPara();//wangjun 20081008
// 	ReadFileDriverPara();//chenAdd for Detect
// 	ReadResultAnalysisPara();
}

//chenAdd for Detect
//====================================================
void CFCVSServerConfig::ReadFileDriverPara()
{
// 	CResString FileDriverSetting("/etc/iip/FileDriver");
// 
// 	m_bCheckDuration = FileDriverSetting["CheckDuration"].GetAsBool(TRUE); // wang.zh add
// 	m_dwRetryTimes = FileDriverSetting["RetryTimes"].GetAsInteger(3);
// 	m_dwRetryInterval = FileDriverSetting["RetryInterval"].GetAsInteger(5);
// 
// 	CStringArray DriverArr;
// 	DriverArr.RemoveAll();
// 	FileDriverSetting["FileDriverList"].GetAsStringArray(DriverArr);
// 	tagFileDriverPara Driver;
// 	for (int i = 0 ; i < DriverArr.GetSize() ; i ++)
// 	{
// 		CResString DriverSetting("/etc/iip/" + DriverArr.GetAt(i));
// 		CString strClassName = DriverSetting["Class"].GetAsString("");
// 		CString strFileType = DriverSetting["FileType"].GetAsString("");
// 		strClassName.TrimLeft();
// 		strClassName.TrimRight();
// 		strFileType.TrimRight();
// 		strFileType.TrimLeft();
// 		if (strClassName.IsEmpty() || strFileType.IsEmpty())
// 		{
// 			continue;
// 		}
// 		Driver.m_strClassName  = strClassName;
// 		Driver.m_strFileType   = strFileType;
// 		Driver.m_strDriverName = DriverSetting["DriverName"].GetAsString("");
// 		Driver.m_bIsDefault    = DriverSetting["IsDefault"].GetAsBool(FALSE);
//         //wangjun
// 		CString strDetectClipW = DriverSetting["DetectedClip_Width"].GetAsString("");
// 		CString strDetectClipH = DriverSetting["DetectedClip_Height"].GetAsString("");
// 		strDetectClipW.TrimLeft();
// 		strDetectClipW.TrimRight();
// 		strDetectClipH.TrimLeft();
// 		strDetectClipH.TrimRight();
// 
// 		Driver.m_strDetectClipW = strDetectClipW;
// 		Driver.m_strDetectClipH = strDetectClipH;
// 		m_FileDriverParaArr.Add(Driver);
// 	}
}

CString CFCVSServerConfig::GetClassNameByFileType(CString strFileType)
{
	strFileType.TrimRight();
	strFileType.TrimLeft();
	for (int i = 0; i < m_FileDriverParaArr.GetSize() ; i++)
	{
		tagFileDriverPara Driver = (tagFileDriverPara)m_FileDriverParaArr.GetAt(i);
		if (strFileType.IsEmpty() && Driver.m_bIsDefault)
		{
			return Driver.m_strClassName;
		}
		if (strFileType.CompareNoCase(Driver.m_strFileType) == 0)
		{
			return Driver.m_strClassName;
		}
	}
	return "";
}

CString CFCVSServerConfig::GetFileTypeByClassName(CString strClassName)
{
	strClassName.TrimLeft();
	strClassName.TrimRight();
	if (strClassName.IsEmpty())
	{
		return "";
	}
	for (int i = 0; i < m_FileDriverParaArr.GetSize() ; i++)
	{
		tagFileDriverPara Driver = (tagFileDriverPara)m_FileDriverParaArr.GetAt(i);
		if (strClassName.CompareNoCase(Driver.m_strClassName) == 0)
		{
			return Driver.m_strFileType;
		}
	}
	return "";
}
//wangjun 20081007
CString CFCVSServerConfig::GetXmlByClassName(CString strClassName)
{
	CString strXml   = "";
	CString strErr   = "";
	CString strMsgID = "";
	CXMLOp XmlPara;
	XmlPara.CleanXMLDocument();

	//strMsgID = GenerateGUID();
	strMsgID = "88888";
	strClassName.TrimLeft();
	strClassName.TrimRight();
	if (strClassName.IsEmpty())
	{
		return "";
	}
	for (int i = 0; i < m_FileDriverParaArr.GetSize() ; i++)
	{
		tagFileDriverPara Driver = (tagFileDriverPara)m_FileDriverParaArr.GetAt(i);
		if (strClassName.CompareNoCase(Driver.m_strClassName) == 0)
		{
			XmlPara.CreateRoot("DriverPara",strErr);

			XmlPara.CreateNode("Info",strErr,"Root");
			XmlPara.SetAttribute("MsgID",strMsgID,strErr,"Info");
			XmlPara.SetAttribute("OpType","ParaOp",strErr,"Info");
			XmlPara.SetAttribute("Op","Set",strErr,"Info");

			XmlPara.CreateNode("Data",strErr,"Root");
			XmlPara.SetAttribute("Class",Driver.m_strClassName,strErr,"Data");
			XmlPara.SetAttribute("DriverName",Driver.m_strDriverName,strErr,"Data");
			XmlPara.SetAttribute("FileType",Driver.m_strFileType,strErr,"Data");
			CString strIsDefault = "0";
			if (Driver.m_bIsDefault)
			{
				strIsDefault = "1";
			}
			XmlPara.SetAttribute("IsDefault",strIsDefault,strErr,"Data");
			XmlPara.SetAttribute("DetectedClip_Width",Driver.m_strDetectClipW,strErr,"Data");
			XmlPara.SetAttribute("DetectedClip_Height",Driver.m_strDetectClipH,strErr,"Data");

			strXml = XmlPara.GetXMLAsString();
			return strXml;
		}
	}
	return "";
}
//==================================================================

void CFCVSServerConfig::ReadResultAnalysisPara()
{
// 	CResString FilterLevelParaSetting("/etc/iip/FilterDBRecorder");
// 	CString strFilterLevel ;
// 	if (m_bUseSectionFunction)
// 	{
// 		strFilterLevel = "Z";
// 	}
// 	else
// 	{
// 		strFilterLevel = FilterLevelParaSetting["FilterLevelName"].GetAsString("");
// 	}
// 	CResString UseTriggerSetting("/etc/iip/UseLevelTrigger");
// 	m_bUseTrigger = UseTriggerSetting["UseTrigger"].GetAsBool("false");
// 
// 	CResString AnlysisParaSetting("/etc/iip/ResultAnlysis");
// 	CStringArray LevelArr;
// 	LevelArr.RemoveAll();
// 	AnlysisParaSetting["Level"].GetAsStringArray(LevelArr);
// 	for (int i = 0 ; i < LevelArr.GetSize() ; i ++)
// 	{
// 		CResString AnlysisSetting("/etc/iip/" + LevelArr.GetAt(i));
// 		tagResultLevel Level;
// 		Level.m_strLevelName = AnlysisSetting["LevelName"].GetAsString("");
// 		Level.m_nBlackFrame = AnlysisSetting["BlackFrame"].GetAsInteger(0);
// 		Level.m_nColorFrame = AnlysisSetting["ColorFrame"].GetAsInteger(0);
// 		Level.m_nColorStrip = AnlysisSetting["ColorStrip"].GetAsInteger(0);
// 		Level.m_nStillFrame = AnlysisSetting["StillFrame"].GetAsInteger(0);
// 		Level.m_nMuteDur    = AnlysisSetting["MuteDur"].GetAsInteger(0);
// 		Level.m_nDiscontinuityDur    = AnlysisSetting["DiscontinuityDur"].GetAsInteger(0);
// 		Level.m_nHighDur    = AnlysisSetting["HighDur"].GetAsInteger(0);
// 		Level.m_nLowDur     = AnlysisSetting["LowDur"].GetAsInteger(0);
// 		Level.m_nPlosiveDur = AnlysisSetting["PlosiveDur"].GetAsInteger(0);
// 
// 		Level.m_nLoudnessLow  = AnlysisSetting["LoudnessLow"].GetAsBool(FALSE);
// 		Level.m_nLoudnessHigh = AnlysisSetting["LoudnessHigh"].GetAsBool(FALSE);
// 		Level.m_nColorCast    = AnlysisSetting["ColorCast"].GetAsInteger(0);
// 		m_ResultLevelArr.Add(Level);
// 		if(Level.m_strLevelName == strFilterLevel)
// 		{
//            m_FilterLevel = Level;
// 		}
// 	}
}
tagResultLevel CFCVSServerConfig::GetFilterLevel()
{
	return m_FilterLevel;
}
void CFCVSServerConfig::ReadDetectPara()
{
// 	CResString ClipTypeSetting("/etc/iip/CanDetectedClipType");
// 	CStringArray ClipTypeArr;
// 	ClipTypeArr.RemoveAll();
// 	ClipTypeSetting["ClipTypeList"].GetAsStringArray(ClipTypeArr);
// 	tagDetectPara DetectPara;
// 	for (int i = 0 ; i < ClipTypeArr.GetSize() ; i ++)
// 	{
// 		CString strSegment = "/etc/iip/"+ClipTypeArr.GetAt(i);
// 		CResString DetectParaSetting(strSegment);
// 
// 		DetectPara.CLIP_WIDTH = DetectParaSetting["CLIP_WIDTH"].GetAsString("720");
// 		DetectPara.CLIP_HEIGHT = DetectParaSetting["CLIP_HEIGHT"].GetAsString("576");
// 		DetectPara.TOP_SAFE_SECTION = DetectParaSetting["TOP_SAFE_SECTION"].GetAsString("0.100000");
// 		DetectPara.BOTTOM_SAFE_SECTION = DetectParaSetting["BOTTOM_SAFE_SECTION"].GetAsString("0.100000");
// 		DetectPara.LEFT_SAFE_SECTION = DetectParaSetting["LEFT_SAFE_SECTION"].GetAsString("0.100000");
// 		DetectPara.RIGHT_SAFE_SECTION = DetectParaSetting["RIGHT_SAFE_SECTION"].GetAsString("0.100000");
// 		DetectPara.BLACK_SCENE_THRESHOLD = DetectParaSetting["BLACK_SCENE_THRESHOLD"].GetAsString("60");
// 		DetectPara.COLOR_SCENE_PERCENTAGE = DetectParaSetting["COLOR_SCENE_PERCENTAGE"].GetAsString("0.985000");
// 		DetectPara.COLOR_STRIP_PERCENTAGE = DetectParaSetting["COLOR_STRIP_PERCENTAGE"].GetAsString("0.900000");
// 		DetectPara.STATIC_FRAME_PERCENTAGE = DetectParaSetting["STATIC_FRAME_PERCENTAGE"].GetAsString("0.980000");
// 		DetectPara.LINE_COUNTS_FOR_STRIP_DETECT = DetectParaSetting["LINE_COUNTS_FOR_STRIP_DETECT"].GetAsString("100");
// 		DetectPara.AMBIT_DEEMED_TO_SAME_PIXEL = DetectParaSetting["AMBIT_DEEMED_TO_SAME_PIXEL"].GetAsString("1");
// 		DetectPara.UNDULATE_AMBIT_OF_Y = DetectParaSetting["UNDULATE_AMBIT_OF_Y"].GetAsString("3");
// 		DetectPara.UNDULATE_AMBIT_OF_U = DetectParaSetting["UNDULATE_AMBIT_OF_U"].GetAsString("3");
// 	    DetectPara.UNDULATE_AMBIT_OF_V = DetectParaSetting["UNDULATE_AMBIT_OF_V"].GetAsString("3");
// 		DetectPara.AUDIO_MUTE_THRESHOLD         =DetectParaSetting["AUDIO_MUTE_THRESHOLD"].GetAsString("3");
// 		DetectPara.AUDIO_MUTE_PERIOD            =DetectParaSetting["AUDIO_MUTE_PERIOD"].GetAsString("3");
// 		DetectPara.AUDIO_HIGH_THRESHOLD   =DetectParaSetting["AUDIO_HIGH_THRESHOLD"].GetAsString("3");
// 		DetectPara.AUDIO_HIGH_PERIOD      =DetectParaSetting["AUDIO_HIGH_PERIOD"].GetAsString("3");
// 		DetectPara.AUDIO_LOW_THRESHOLD   =DetectParaSetting["AUDIO_LOW_THRESHOLD"].GetAsString("3");
// 		DetectPara.AUDIO_LOW_PERIOD      =DetectParaSetting["AUDIO_LOW_PERIOD"].GetAsString("3");
// 		DetectPara.AUDIO_DISCONTINUITY_THRESHOLD=DetectParaSetting["AUDIO_DISCONTINUITY_THRESHOLD"].GetAsString("3");
//         DetectPara.AUDIO_DISCONTINUITY_PERIOD   =DetectParaSetting["AUDIO_DISCONTINUITY_PERIOD"].GetAsString("3");
// 
// 		DetectPara.LOUDNESS_LOW_THRESHOLD = DetectParaSetting["LOUDNESS_LOW_THRESHOLD"].GetAsString("-60");
// 		DetectPara.LOUDNESS_HIGH_THRESHOLD = DetectParaSetting["LOUDNESS_HIGH_THRESHOLD"].GetAsString("0");
// 
// 		//m_ClipTypeParaArr.Add()
// 		CString strIndex = ClipTypeArr.GetAt(i);
// 		m_ClipTypeParaMap.SetAt(strIndex,DetectPara);
// 	}
}
void CFCVSServerConfig::ReadClipTypeMapPara()
{
// 	CResString ClipTypeMapSetting("/etc/iip/CanDetectedClipTypeMap");
// 	CStringArray ClipTypeMapArr;
//     
// 	CResString ClipTypeSetting("/etc/iip/CanDetectedClipType");
// 	//CStringArray ClipTypeArr;
// 	//ClipTypeArr.RemoveAll();
// 	m_DectClipTypeArr.RemoveAll();
// 	ClipTypeSetting["ClipTypeList"].GetAsStringArray(m_DectClipTypeArr);
//     
// 	if (m_DectClipTypeArr.GetSize() < 0)
// 	{
// 		AfxMessageBox("您没有配置FCVSServer.CLSID文件中[CanDetectedClipType]部分下的ClipTypeList字段,程序无法正常工作!");
// 	    return ;
// 	}
// 	for (int i = 0; i < m_DectClipTypeArr.GetSize(); i++)
// 	{
// 		ClipTypeMapArr.RemoveAll();
// 		CString strClipTypeName = m_DectClipTypeArr.GetAt(i);
//         ClipTypeMapSetting[strClipTypeName].GetAsStringArray(ClipTypeMapArr);
// 		if (ClipTypeMapArr.GetSize() > 0)
// 		{
// 			for (int j = 0; j<ClipTypeMapArr.GetSize(); j++)
// 			{
// 				m_CTMapMap.SetAt(ClipTypeMapArr.GetAt(j),m_DectClipTypeArr.GetAt(i));
// 			}
// 		}
// 
// 	}

}
CString CFCVSServerConfig::GetDetectParaXml(CString strClipType)
{
	CString strCanDetectClipType;
    tagDetectPara DetectPara;
	CString strMsgID =""/* GenerateGUID()*/;
	CString strXml = "";
	CString strErr = "";
	CXMLOp XmlPara;
	XmlPara.CleanXMLDocument();

	BOOL bRet = m_CTMapMap.Lookup(strClipType,strCanDetectClipType);
// 	if (!bRet)
// 	{
// 		OutputDebugString("\n没有根据素材类型找到相应的技审检测类型,请确定在配置中已经映射了素材类型:");
// 		OutputDebugString(strClipType);
// 		OutputDebugString("\n");
//         strCanDetectClipType = m_DectClipTypeArr.GetAt(0);
// 	}
	if(/*!strCanDetectClipType.IsEmpty()*/TRUE)
	{
		bRet = m_ClipTypeParaMap.Lookup(strClipType,DetectPara);
		
		if (bRet)
		{
			XmlPara.CreateRoot("AutoDetectPara",strErr);
			XmlPara.CreateNode("Info",strErr,"Root");
			XmlPara.SetAttribute("MsgID",strMsgID,strErr,"Info");
			XmlPara.SetAttribute("OpType","ParaOp",strErr,"Info");
			XmlPara.SetAttribute("Op","Set",strErr,"Info");
			XmlPara.CreateNode("Data",strErr,"Root");
// 			XmlPara.SetAttribute("CLIP_WIDTH",DetectPara.CLIP_WIDTH,strErr,"Data");
// 			XmlPara.SetAttribute("CLIP_HEIGHT",DetectPara.CLIP_HEIGHT,strErr,"Data");
// 		    XmlPara.SetAttribute("TOP_SAFE_SECTION",DetectPara.TOP_SAFE_SECTION,strErr,"Data");
// 			XmlPara.SetAttribute("BOTTOM_SAFE_SECTION",DetectPara.BOTTOM_SAFE_SECTION,strErr,"Data");
// 			XmlPara.SetAttribute("LEFT_SAFE_SECTION",DetectPara.LEFT_SAFE_SECTION,strErr,"Data");
// 			XmlPara.SetAttribute("RIGHT_SAFE_SECTION",DetectPara.RIGHT_SAFE_SECTION,strErr,"Data");
 		    XmlPara.SetAttribute("TOP_SECURE_SECTION",DetectPara.TOP_SAFE_SECTION,strErr,"Data");
 			XmlPara.SetAttribute("BOTTOM_SECURE_SECTION",DetectPara.BOTTOM_SAFE_SECTION,strErr,"Data");
 			XmlPara.SetAttribute("LEFT_SECURE_SECTION",DetectPara.LEFT_SAFE_SECTION,strErr,"Data");
 			XmlPara.SetAttribute("RIGHT_SECURE_SECTION",DetectPara.RIGHT_SAFE_SECTION,strErr,"Data");
			XmlPara.SetAttribute("BLACK_SCENE_THRESHOLD",DetectPara.BLACK_SCENE_THRESHOLD,strErr,"Data");
			XmlPara.SetAttribute("COLOR_SCENE_PERCENTAGE",DetectPara.COLOR_SCENE_PERCENTAGE,strErr,"Data");
			XmlPara.SetAttribute("COLOR_STRIP_PERCENTAGE",DetectPara.COLOR_STRIP_PERCENTAGE,strErr,"Data");
			XmlPara.SetAttribute("STATIC_FRAME_PERCENTAGE",DetectPara.STATIC_FRAME_PERCENTAGE,strErr,"Data");
			XmlPara.SetAttribute("LINE_COUNTS_FOR_STRIP_DETECT",DetectPara.LINE_COUNTS_FOR_STRIP_DETECT,strErr,"Data");
			XmlPara.SetAttribute("AMBIT_DEEMED_TO_SAME_PIXEL",DetectPara.AMBIT_DEEMED_TO_SAME_PIXEL,strErr,"Data");
			XmlPara.SetAttribute("UNDULATE_AMBIT_OF_Y",DetectPara.UNDULATE_AMBIT_OF_Y,strErr,"Data");
			XmlPara.SetAttribute("UNDULATE_AMBIT_OF_U",DetectPara.UNDULATE_AMBIT_OF_U,strErr,"Data");
			XmlPara.SetAttribute("UNDULATE_AMBIT_OF_V",DetectPara.UNDULATE_AMBIT_OF_V,strErr,"Data");

			XmlPara.SetAttribute("AUDIO_MUTE_THRESHOLD",DetectPara.AUDIO_MUTE_THRESHOLD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_MUTE_PERIOD",DetectPara.AUDIO_MUTE_PERIOD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_HIGH_THRESHOLD",DetectPara.AUDIO_HIGH_THRESHOLD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_HIGH_PERIOD",DetectPara.AUDIO_HIGH_PERIOD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_LOW_THRESHOLD",DetectPara.AUDIO_LOW_THRESHOLD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_LOW_PERIOD",DetectPara.AUDIO_LOW_PERIOD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_DISCONTINUITY_THRESHOLD",DetectPara.AUDIO_DISCONTINUITY_THRESHOLD,strErr,"Data");
			XmlPara.SetAttribute("AUDIO_DISCONTINUITY_PERIOD",DetectPara.AUDIO_DISCONTINUITY_PERIOD,strErr,"Data");

			XmlPara.SetAttribute("LOUDNESS_LOW_THRESHOLD",DetectPara.LOUDNESS_LOW_THRESHOLD,strErr,"Data");
			XmlPara.SetAttribute("LOUDNESS_HIGH_THRESHOLD",DetectPara.LOUDNESS_HIGH_THRESHOLD,strErr,"Data");

			strXml = XmlPara.GetXMLAsString();
		}
	}
	return strXml;
}

CString CFCVSServerConfig::GetCurrentLocalTime()
{
	CString strTime;
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);
	strTime.Format("CurrentTime = %d-%d-%d %d:%d:%d:%d",systemtime.wYear,systemtime.wMonth,systemtime.wDay,systemtime.wHour,systemtime.wMinute,systemtime.wSecond,systemtime.wMilliseconds);
    return strTime;
}

CString CFCVSServerConfig::GetCurrentLocalTime2()
{
	CString strTime;
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);
	strTime.Format("%d-%d-%d %d:%d:%d",systemtime.wYear,systemtime.wMonth,systemtime.wDay,systemtime.wHour,systemtime.wMinute,systemtime.wSecond);
    return strTime;
}

CString CFCVSServerConfig::GetExecutePath()
{
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(g_hFCVS,szPath,MAX_PATH);
	TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	_tsplitpath(szPath,drive,dir,fname,ext);
	strcpy(szPath,drive);
	strcat(szPath,dir);
	CString strPathTemp = "";
	strPathTemp.Format("%s",szPath);
	return strPathTemp;
}

BOOL CFCVSServerConfig::DirectoryFind(CString strPath,CString strDirctoryName,BOOL bNotFindCreate)
{
	CFileFind finder;         
	CString strWildcard(strPath);     
	strWildcard += _T("\\*.*");       
	BOOL bWorking = finder.FindFile(strWildcard);  
	BOOL bFind = FALSE;
	while(bWorking)     
	{     
        bWorking = finder.FindNextFile();     
	    if(finder.IsDots())     
		  continue;        
		if(finder.IsDirectory())     
		{
			CString str = finder.GetFileName();
			if (str.CompareNoCase(strDirctoryName) == 0)
			{
				bFind = TRUE;
				break;
			}
		}     
	 }     
   finder.Close();
   if (bFind)
   {
	   return TRUE;
   }
   else
   {
	   if(::CreateDirectory(strPath + "\\" + strDirctoryName,NULL))
		   return TRUE;
	   else
		   return FALSE;
   }
   return FALSE;
}

BOOL CFCVSServerConfig::FileFind(CString strPath,CString strFileName,BOOL bFindDelete /* = TRUE */)
{
	CFileFind finder;         
	CString strWildcard(strPath);     
	strWildcard += _T("\\*.*");       
	BOOL bWorking = finder.FindFile(strWildcard);  
	BOOL bFind = FALSE;
	while(bWorking)     
	{     
        bWorking = finder.FindNextFile();     
	    if(finder.IsDots())     
		  continue;        
		if(finder.IsDirectory())     
		{     
		   continue;
		}     
		CString str = finder.GetFileName();
		if (str.CompareNoCase(strFileName) == 0)
		{
			bFind = TRUE;
		}
	 }     
   finder.Close();
   if (bFind)
   {
	   if (bFindDelete)
	   {
		   if(::DeleteFile(strPath + "\\" + strFileName))
		   {
			   return FALSE;
		   }
		   else
		   {
			   return TRUE;
		   }
	   }
   }
   else
   {
	   return TRUE;
   }
   return FALSE;
}

bool CFCVSServerConfig::ChangeDetectPara(CString DetectName,tagDetectPara DetectParam)
{
 	bool bSet = false;
// 	CString strDecName = "";
// 	CResString ClipTypeSetting("/etc/iip/CanDetectedClipType");
// 	CStringArray ClipTypeArr;
// 	ClipTypeArr.RemoveAll();
// 	ClipTypeSetting["ClipTypeList"].GetAsStringArray(ClipTypeArr);
// 	for (int i = 0 ; i < ClipTypeArr.GetSize() ; i ++)
// 	{
// 		strDecName = ClipTypeArr.GetAt(i);
// 		if(strDecName.CompareNoCase(DetectName) != 0)
// 			continue;
// 
// 		CString strSegment = "/etc/iip/"+strDecName;
// 		CResString DetectParaSetting(strSegment);
// 		
// 		DetectParaSetting["TOP_SAFE_SECTION"             ].SetAsString(DetectParam.TOP_SAFE_SECTION);
// 		DetectParaSetting["BOTTOM_SAFE_SECTION"          ].SetAsString(DetectParam.BOTTOM_SAFE_SECTION);
// 		DetectParaSetting["LEFT_SAFE_SECTION"            ].SetAsString(DetectParam.LEFT_SAFE_SECTION);
// 		DetectParaSetting["RIGHT_SAFE_SECTION"           ].SetAsString(DetectParam.RIGHT_SAFE_SECTION);
// 		DetectParaSetting["BLACK_SCENE_THRESHOLD"        ].SetAsString(DetectParam.BLACK_SCENE_THRESHOLD);
// 		DetectParaSetting["COLOR_SCENE_PERCENTAGE"       ].SetAsString(DetectParam.COLOR_SCENE_PERCENTAGE);
// 		DetectParaSetting["COLOR_STRIP_PERCENTAGE"       ].SetAsString(DetectParam.COLOR_STRIP_PERCENTAGE);
// 		DetectParaSetting["STATIC_FRAME_PERCENTAGE"      ].SetAsString(DetectParam.STATIC_FRAME_PERCENTAGE);
// 		DetectParaSetting["LINE_COUNTS_FOR_STRIP_DETECT" ].SetAsString(DetectParam.LINE_COUNTS_FOR_STRIP_DETECT);
// 		DetectParaSetting["AMBIT_DEEMED_TO_SAME_PIXEL"   ].SetAsString(DetectParam.AMBIT_DEEMED_TO_SAME_PIXEL);
// 		DetectParaSetting["UNDULATE_AMBIT_OF_Y"          ].SetAsString(DetectParam.UNDULATE_AMBIT_OF_Y);
// 		DetectParaSetting["UNDULATE_AMBIT_OF_U"          ].SetAsString(DetectParam.UNDULATE_AMBIT_OF_U);
// 		DetectParaSetting["UNDULATE_AMBIT_OF_V"          ].SetAsString(DetectParam.UNDULATE_AMBIT_OF_V);
// 		DetectParaSetting["AUDIO_MUTE_THRESHOLD"         ].SetAsString(DetectParam.AUDIO_MUTE_THRESHOLD);
// 		DetectParaSetting["AUDIO_MUTE_PERIOD"            ].SetAsString(DetectParam.AUDIO_MUTE_PERIOD);
// 		DetectParaSetting["AUDIO_HIGH_THRESHOLD"         ].SetAsString(DetectParam.AUDIO_HIGH_THRESHOLD);
// 		DetectParaSetting["AUDIO_HIGH_PERIOD"            ].SetAsString(DetectParam.AUDIO_HIGH_PERIOD);
// 		DetectParaSetting["AUDIO_LOW_THRESHOLD"          ].SetAsString(DetectParam.AUDIO_LOW_THRESHOLD);
// 		DetectParaSetting["AUDIO_LOW_PERIOD"             ].SetAsString(DetectParam.AUDIO_LOW_PERIOD);
// 		DetectParaSetting["AUDIO_DISCONTINUITY_THRESHOLD"].SetAsString(DetectParam.AUDIO_DISCONTINUITY_THRESHOLD);
//         DetectParaSetting["AUDIO_DISCONTINUITY_PERIOD"   ].SetAsString(DetectParam.AUDIO_DISCONTINUITY_PERIOD);
// 
// 		DetectParaSetting["LOUDNESS_LOW_THRESHOLD"       ].SetAsString(DetectParam.LOUDNESS_LOW_THRESHOLD);
// 		DetectParaSetting["LOUDNESS_HIGH_THRESHOLD"      ].SetAsString(DetectParam.LOUDNESS_HIGH_THRESHOLD);
// 		
// 		bSet = true;
// 		break;
// 	}

    return bSet;
}
