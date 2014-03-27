// FileDriver.cpp : implementation file
//

#include "stdafx.h"
//#include "FCVSServer.h"
#include "FileDriver.h"
#include "XMLOp.h"
#include <math.h>
#include "TxParamString.h"
#include "TxLogManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection g_DetectInfoCriticalSection;
CDetectResult::CDetectResult()
{
	m_strLevel = "";
	m_LowestLevel.m_strLevelName = "";
	m_som = 0;
	GetLowestLevel();
}

CDetectResult::~CDetectResult()
{
   	m_DetectResultArr.RemoveAll();
	m_ADetectResultArr.RemoveAll();
	m_DetectFileInfoArr.RemoveAll();
	m_AnlysisResultArr.RemoveAll();
}

BOOL CDetectResult::AppendFileInfo(CString strFiledValue,CString strenFiledName,CString strcnFiledName /* =  */)
{
	strFiledValue.TrimRight();
	strFiledValue.TrimLeft();
	strenFiledName.TrimLeft();
	strenFiledName.TrimRight();
	strcnFiledName.TrimRight();
	strcnFiledName.TrimLeft();
	if (strenFiledName.IsEmpty())
	{
		return FALSE;
	}
	tagDetectFileInfo FileInfo;
	FileInfo.m_strenFiledName = strenFiledName;
	FileInfo.m_strcnFiledName = strcnFiledName;
	FileInfo.m_strFiledValue = strFiledValue;

	CSingleLock sl(&g_DetectInfoCriticalSection);
	sl.Lock();
	m_DetectFileInfoArr.Add(FileInfo);
	sl.Unlock();
	return TRUE;
}

BOOL CDetectResult::CleanFileInfo()
{
	CSingleLock sl(&g_DetectInfoCriticalSection);
    sl.Lock();
	m_DetectFileInfoArr.RemoveAll();
	sl.Unlock();
	return TRUE;
}
//增加视频检测结果
BOOL CDetectResult::AppendDetectResult(LONGLONG lFrame,DETECT_TYPE DetectType)
{
	tagDetectResult DetectResult;
	DetectResult.m_DetectType = DetectType;
	DetectResult.m_lFrame = lFrame;

	CSingleLock sl(&g_DetectInfoCriticalSection);
	sl.Lock();
	bool bRe = m_DetectResultArr.Add(DetectResult);
	sl.Unlock();
	return bRe;
}
//增加音频检测结果
BOOL CDetectResult::AppendAudioDetectResult(LONGLONG lFrameStart, LONGLONG lDuration, DETECT_TYPE DetectType)
{
	tagAudioDetectResult DetectResult;
	DetectResult.m_DetectType  = DetectType;
	DetectResult.m_lFrameStart = lFrameStart;
	DetectResult.m_lDuration   = lDuration;
	
	CSingleLock sl(&g_DetectInfoCriticalSection);
	sl.Lock();
	bool bRe = m_ADetectResultArr.Add(DetectResult);
	sl.Unlock();
	return bRe;
}

BOOL CDetectResult::CleanDetectResult()
{
	CSingleLock sl(&g_DetectInfoCriticalSection);
	sl.Lock();
	m_DetectResultArr.RemoveAll();
	sl.Unlock();
	return TRUE;
}

CString CDetectResult::GetDetectTypeName(DETECT_TYPE DetectType)
{
	CString strTypeName = "";
	switch(DetectType)
	{
	case DETECT_TYPE_BLACK_SCENE:
		strTypeName = "黑场";
		break;
	case DETECT_TYPE_COLOR_SCENE:
		strTypeName = "彩场";
		break;
	case DETECT_TYPE_COLOR_STRIP:
		strTypeName = "彩条";
		break;
	case DETECT_TYPE_STATIC_FRAME:
		strTypeName = "静帧";
		break;
	case DETECT_TYPE_AUDIO_MUTE:
		strTypeName = "音量静音";
		break;
// 	case DETECT_TYPE_AUDIO_DISCONTINUITY:
// 		strTypeName = "音频中断";
// 		break;
	case DETECT_TYPE_AUDIO_HIGH:
		strTypeName = "音量过高";
		break;
	case DETECT_TYPE_AUDIO_LOW:
		strTypeName = "音量过低";
		break;
	case DETECT_TYPE_LOUDNESS_LOW:
		strTypeName = "响度过低";
		break;
	case DETECT_TYPE_LOUDNESS_HIGH:
		strTypeName = "响度过高";
         break;
	case DETECT_TYPE_MOSAIC:
		strTypeName = "马赛克";
		break;
	case DETECT_TYPE_COLOR_CAST:
		strTypeName = "偏色";
		break;
	}
	return strTypeName;
}

CString CDetectResult::GetTimeCodeString(LONGLONG lFrame)
{
	int nF,nS,nM,nH;
	nH=nM=nS=nF= 0;
	nF = lFrame % 25;
	LONGLONG Temp = lFrame / 25;
	nS = Temp % 60;
	lFrame = Temp / 60;
	nM = lFrame % 60;
	Temp = lFrame / 60;
	nH = Temp;
	CString strRet = "";
	strRet.Format("%02d:%02d:%02d:%02d",nH,nM,nS,nF);
	return strRet;
}

void CDetectResult::AnlysisResult()
{
	//先分析视频后分析音频，顺序不许颠倒
	CString strVideoLevel = AnlysisVideoResult();
	CString strAudioLevel = AnlysisAudioResult();

	m_strLevel = strVideoLevel.Compare(strAudioLevel) < 0?strVideoLevel:strAudioLevel;
}
CString CDetectResult::AnlysisVideoResult()
{
    DETECT_TYPE PreDetectType;
	LONGLONG lFrameStart   = 0;
	LONGLONG lFrameEnd     = 0;
	LONGLONG lDetectSize   = 0;
	CString  strVideoLevel = "";

	lDetectSize = m_DetectResultArr.GetSize();
	for (int i = 0 ; i < lDetectSize ; i ++)
	{
		OutputDebugString("分析结果呢，别惹我");
		tagDetectResult ResultTemp = (tagDetectResult)m_DetectResultArr.GetAt(i);
		if (i == 0)
		{
			PreDetectType = ResultTemp.m_DetectType;
			lFrameStart = ResultTemp.m_lFrame;
		}
		else
		{
			if (ResultTemp.m_lFrame != lFrameEnd + 1 || ResultTemp.m_DetectType != PreDetectType || i == lDetectSize-1)
			{
				if (i == lDetectSize-1 && ResultTemp.m_lFrame == lFrameEnd + 1 && ResultTemp.m_DetectType == PreDetectType)
				{
					lFrameEnd = ResultTemp.m_lFrame;
				}
				//------------------------------------------------------------
				//wangjun 20081112
				////修改检测结果分析算法，检测出符合正常素材的数据目前不纪录数据库
				long lDuration = lFrameEnd - lFrameStart + 1;
				BOOL bSkip = FALSE;
				if(!m_LowestLevel.m_strLevelName.IsEmpty())//如果不按等级过滤分析结果，那么分析结果全部纪录数据库
				{
					switch(PreDetectType)
					{
					case DETECT_TYPE_BLACK_SCENE:
						if(lDuration < m_LowestLevel.m_nBlackFrame)
							bSkip = TRUE;
						break;
					case DETECT_TYPE_COLOR_SCENE:
						if(lDuration < m_LowestLevel.m_nColorFrame)
							bSkip = TRUE;
						break;
					case DETECT_TYPE_COLOR_STRIP:
						if(lDuration < m_LowestLevel.m_nColorStrip)
							bSkip = TRUE;
						break;
					case DETECT_TYPE_STATIC_FRAME:
						if(lDuration < m_LowestLevel.m_nStillFrame)
							bSkip = TRUE;
						break;
					case DETECT_TYPE_COLOR_CAST:
						if(lDuration < m_LowestLevel.m_nColorCast)
							bSkip = TRUE;					
						break;
					}
				}
                //--------------------------------------------------------------
				if (!bSkip)//符合正常素材条件的数据，忽略不纪录数据库
				{
					tagAnlysisResult Anlysis;
					Anlysis.m_DetectType = PreDetectType;
					Anlysis.m_strDetectTypeName = GetDetectTypeName(PreDetectType);
					Anlysis.m_nFrameStart = lFrameStart;
					Anlysis.m_nFrameEnd = lFrameEnd;
					Anlysis.m_nDuration = lFrameEnd - lFrameStart + 1;
					Anlysis.m_strTimeCodeStart = GetTimeCodeString(lFrameStart+m_som);
					Anlysis.m_strTimeCodeEnd = GetTimeCodeString( lFrameEnd+m_som);
					Anlysis.m_strTimeCodeDuration = GetTimeCodeString(lFrameEnd - lFrameStart + 1);
				    m_AnlysisResultArr.Add(Anlysis);
				}
				lFrameStart = ResultTemp.m_lFrame;
				PreDetectType = ResultTemp.m_DetectType;
			}
		}
		lFrameEnd = ResultTemp.m_lFrame;
	}
    //数据表中添加了触发器按逻辑出入点计算技审等级
	//wangjun 20081109
    //如果没有配置技审等级或使用数据库触发器计算技审等级，那么不在程序里计算等级
	if (GetFCVSServerConfig().m_ResultLevelArr.GetSize() <= 0 || GetFCVSServerConfig().m_bUseTrigger)
	{
		return strVideoLevel;
	}
	strVideoLevel = m_LowestLevel.m_strLevelName;
	for (i = 0 ; i < m_AnlysisResultArr.GetSize() ; i ++)
	{
		tagAnlysisResult Anlysis;
		Anlysis = (tagAnlysisResult)m_AnlysisResultArr.GetAt(i);

		for (int j = 0 ; j < GetFCVSServerConfig().m_ResultLevelArr.GetSize(); j ++)
		{
			tagResultLevel Level = (tagResultLevel)GetFCVSServerConfig().m_ResultLevelArr.GetAt(j);
			
			if (Anlysis.m_DetectType == DETECT_TYPE_BLACK_SCENE && Anlysis.m_nDuration >= Level.m_nBlackFrame && strVideoLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strVideoLevel = Level.m_strLevelName;
			}
			if (Anlysis.m_DetectType == DETECT_TYPE_COLOR_SCENE && Anlysis.m_nDuration >= Level.m_nColorFrame && strVideoLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strVideoLevel = Level.m_strLevelName;
			}
			if (Anlysis.m_DetectType == DETECT_TYPE_COLOR_STRIP && Anlysis.m_nDuration >= Level.m_nColorStrip && strVideoLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strVideoLevel = Level.m_strLevelName;
			}
			if (Anlysis.m_DetectType == DETECT_TYPE_STATIC_FRAME && Anlysis.m_nDuration >= Level.m_nStillFrame && strVideoLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strVideoLevel = Level.m_strLevelName;
			}
			if (Anlysis.m_DetectType == DETECT_TYPE_COLOR_CAST && Anlysis.m_nDuration >= Level.m_nColorCast && strVideoLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strVideoLevel = Level.m_strLevelName;
			}
		}
	}

	if (strVideoLevel == m_LowestLevel.m_strLevelName && m_AnlysisResultArr.GetSize() <= 0)//设置一个最低的检测等级
	{
       	char* szpLevel = m_LowestLevel.m_strLevelName.GetBuffer(0);
	    strVideoLevel = ((*szpLevel)+1);
	}
	return strVideoLevel;
}
CString CDetectResult::AnlysisAudioResult()
{
	LONGLONG lDetectSize   = 0;
	CString  strAudioLevel = "";

	long lVideoAnlysisCount = m_AnlysisResultArr.GetSize();

	lDetectSize = m_ADetectResultArr.GetSize();
	for (int i = 0 ; i < lDetectSize ; i ++)
	{
	    tagAudioDetectResult ResultTemp = (tagAudioDetectResult)m_ADetectResultArr.GetAt(i);
		BOOL bSkip = FALSE;
		if(!m_LowestLevel.m_strLevelName.IsEmpty())//如果不按等级过滤分析结果，那么分析结果全部纪录数据库
		{
			switch(ResultTemp.m_DetectType)
			{
			case DETECT_TYPE_AUDIO_MUTE:
				if(ResultTemp.m_lDuration < m_LowestLevel.m_nMuteDur)
					bSkip = TRUE;
				break;
// 			case DETECT_TYPE_AUDIO_DISCONTINUITY:
// 				if(ResultTemp.m_lDuration < m_LowestLevel.m_nDiscontinuityDur)
// 					bSkip = TRUE;
// 				break;
			case DETECT_TYPE_AUDIO_HIGH:
				if(ResultTemp.m_lDuration < m_LowestLevel.m_nHighDur)
					bSkip = TRUE;
				break;
			case DETECT_TYPE_AUDIO_LOW:
				if(ResultTemp.m_lDuration < m_LowestLevel.m_nLowDur)
					bSkip = TRUE;
				break;
			}
		}
		//--------------------------------------------------------------
		if (!bSkip)//符合正常素材条件的数据，忽略不纪录数据库
		{
			tagAnlysisResult Anlysis;
			Anlysis.m_DetectType = ResultTemp.m_DetectType;
			Anlysis.m_strDetectTypeName = GetDetectTypeName(ResultTemp.m_DetectType);
			Anlysis.m_nFrameStart = ResultTemp.m_lFrameStart;
			Anlysis.m_nFrameEnd = ResultTemp.m_lFrameStart + ResultTemp.m_lDuration -1;
			Anlysis.m_nDuration = ResultTemp.m_lDuration;
			Anlysis.m_strTimeCodeStart = GetTimeCodeString(ResultTemp.m_lFrameStart);
			Anlysis.m_strTimeCodeEnd = GetTimeCodeString( ResultTemp.m_lFrameStart + ResultTemp.m_lDuration -1);
			Anlysis.m_strTimeCodeDuration = GetTimeCodeString(ResultTemp.m_lDuration);
			m_AnlysisResultArr.Add(Anlysis);
		}
		
	}
    //数据表中添加了触发器按逻辑出入点计算技审等级
	//wangjun 20081109
    //如果没有配置技审等级或使用数据库触发器计算技审等级，那么不在程序里计算等级
	if (GetFCVSServerConfig().m_ResultLevelArr.GetSize() <= 0 || GetFCVSServerConfig().m_bUseTrigger)
	{
		return strAudioLevel;
	}
	strAudioLevel = m_LowestLevel.m_strLevelName;
	//从音频开始
	for (i = lVideoAnlysisCount ; i < m_AnlysisResultArr.GetSize() ; i ++)
	{
		tagAnlysisResult Anlysis;
		Anlysis = (tagAnlysisResult)m_AnlysisResultArr.GetAt(i);

		for (int j = 0 ; j < GetFCVSServerConfig().m_ResultLevelArr.GetSize(); j ++)
		{
			tagResultLevel Level = (tagResultLevel)GetFCVSServerConfig().m_ResultLevelArr.GetAt(j);
			
			if (Anlysis.m_DetectType == DETECT_TYPE_AUDIO_MUTE && Anlysis.m_nDuration >= Level.m_nMuteDur && strAudioLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strAudioLevel = Level.m_strLevelName;
			}
// 			if (Anlysis.m_DetectType == DETECT_TYPE_AUDIO_DISCONTINUITY && Anlysis.m_nDuration >= Level.m_nDiscontinuityDur && strAudioLevel.Compare(Level.m_strLevelName) >= 0)
// 			{
// 				strAudioLevel = Level.m_strLevelName;
// 			}
			if (Anlysis.m_DetectType == DETECT_TYPE_AUDIO_HIGH && Anlysis.m_nDuration >= Level.m_nHighDur && strAudioLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strAudioLevel = Level.m_strLevelName;
			}
			if (Anlysis.m_DetectType == DETECT_TYPE_AUDIO_LOW && Anlysis.m_nDuration >= Level.m_nLowDur && strAudioLevel.Compare(Level.m_strLevelName) >= 0)
			{
				strAudioLevel = Level.m_strLevelName;
			}
		}
	}

	if (strAudioLevel == m_LowestLevel.m_strLevelName && m_AnlysisResultArr.GetSize() <= 0)//设置一个最低的检测等级
	{
       	char* szpLevel = m_LowestLevel.m_strLevelName.GetBuffer(0);
	    strAudioLevel = ((*szpLevel)+1);
	}

	return strAudioLevel;
}
/////////////////////////////////////////////////////////////////////////////
//为了防止多个线程同时写数据库，这里声明了一个全局临界区


CFileDriver::CFileDriver(CDriveMgr* pDriver)
: m_pFileDriver(pDriver)
{
	OutputDebugString("into CFileDriver::CFileDriver()\n");
	m_strServiceNameOp = "";
	nPos = -1;
	m_nStatus = TSK_IS_EMPTY;
	m_strTskID = "";
	nCount = 0;
	m_ThreadEvent        = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_evThreadExit       = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_evAThreadEvent     = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_evAThreadExit      = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_evFileInfoComplete = CreateEvent(NULL,TRUE,FALSE,NULL);
	::InitializeCriticalSection(&m_Lock);
	m_uReturnType = DR_NOT_RETURN;
	m_bSubmit     = FALSE;

	m_hTaskThread = NULL;
	m_hAThread    = NULL;
	
	//切片参数初始化
	m_TaskSOM = 0;
	m_TaskDuration = 0;
	isSetSectionEnd = FALSE;
	isUseSectionFuction = FALSE;
	m_TaskSectionID = "0";
}

CFileDriver::~CFileDriver()
{
	if (WaitForSingleObject(m_evThreadExit,10000L) == WAIT_TIMEOUT)
	{
		if (m_hTaskThread)
		{
			CloseHandle(m_hTaskThread);
            m_hTaskThread = NULL;
		}
	}
	if (GetFCVSServerConfig().m_bCheckAudio && WaitForSingleObject(m_evAThreadExit,10000L) == WAIT_TIMEOUT)
	{
		if(m_hAThread)
		{
			CloseHandle(m_hAThread);
			m_hAThread = NULL;
		}
	}

	CloseHandle(m_ThreadEvent);
	CloseHandle(m_evThreadExit);

	CloseHandle(m_evAThreadEvent);
	CloseHandle(m_evAThreadExit);

	CloseHandle(m_evFileInfoComplete);
	
	if (m_pAutoDetect)
	{
		m_pAutoDetect->Release();
		m_pAutoDetect = NULL;
	}

	::DeleteCriticalSection(&m_Lock);

	OutputDebugString("into CFileDriver::~CFileDriver()\n");
}


BEGIN_MESSAGE_MAP(CFileDriver, CWnd)
	//{{AFX_MSG_MAP(CFileDriver)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileDriver message handlers

void CFileDriver::SetTaskErrInfo(CString strError)
{
	m_strLastErrorInfo = strError;
}

CString CFileDriver::GetLastAnlysisResultAsXmlString()
{
	CString strRetXml = "";
	CString strErr = "";
	CTxParamString XmlOp;
	CTxStrConvert val;
	val.SetVal(strErr);
	XmlOp.SetElemVal("AnlysisResult", val);
	XmlOp.GoIntoKey("AnlysisResult");
	XmlOp.SetElemVal("TskInfo", val);
	XmlOp.GoIntoKey("TskInfo");
	val.SetVal(m_strTskID);
	XmlOp.SetAttribVal( NULL, "TskID", val);

	val.SetVal(m_strClipID);
	XmlOp.SetAttribVal(NULL, "ClipID", val);

	val.SetVal(m_strClipName);
	XmlOp.SetAttribVal(NULL, "ClipName", val);

	val.SetVal(m_strFilePath);
	XmlOp.SetAttribVal(NULL, "SrcDir", val);

	val.SetVal(m_DetectResult.m_strLevel);
	XmlOp.SetAttribVal(NULL, "Level", val);

	val.SetVal(m_strCheckKind);
	XmlOp.SetAttribVal(NULL, "CheckKind", val);

	val.SetVal(m_strFileType);
	XmlOp.SetAttribVal(NULL, "FileType", val);

	val.SetVal(m_strCheckItem);
	XmlOp.SetAttribVal(NULL, "CheckItem", val);

	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		CString strSOM;
		CString strEOM;
		strSOM.Format("%d",m_TaskSOM);
		LONGLONG EOM = m_TaskSOM+m_TaskDuration;
		strEOM.Format("%d",EOM);
		val.SetVal(strSOM);
		XmlOp.SetAttribVal(NULL, "SOM",val);
		val.SetVal(strEOM);
		XmlOp.SetAttribVal(NULL, "EOM",val);

		int temp = 0;
		temp = _ttoi(m_TaskSectionID);
		if (temp !=0)
		{
			temp = temp -1;
		}
		//适应切片合并算法
		val.SetVal(temp);
		XmlOp.SetAttribVal(NULL, "SecID",val);
		val.SetVal(m_totalSectionCount);
		XmlOp.SetAttribVal(NULL, "nTotalSectionCount",val);
	}


	val.SetVal(m_nCheckOpt);
	XmlOp.SetAttribVal(NULL, "CheckOpt",val);
	val.SetVal(m_nPRI);
	XmlOp.SetAttribVal(NULL, "PRI",val);
	XmlOp.OutofKey();
	val.SetVal("");
	XmlOp.SetElemVal("TskData", val);
	XmlOp.GoIntoKey("TskData");
	XmlOp.SetElemVal("FileInfo", val);
	XmlOp.GoIntoKey("FileInfo");

	if (!GetFCVSServerConfig().m_bUseSectionFunction)
	{
		for (int i = 0 ; i < m_DetectResult.m_DetectFileInfoArr.GetSize() ; i ++)
		{
			tagDetectFileInfo FileInfo = (tagDetectFileInfo)m_DetectResult.m_DetectFileInfoArr.GetAt(i);
			CString strNode = "";
			strNode.Format("%s%d","InfoData",i);
			XmlOp.SetElemVal(strNode, val);
			XmlOp.GoIntoKey(strNode);
			val.SetVal(FileInfo.m_strenFiledName);
			XmlOp.SetAttribVal(NULL, "ColName",val);
			val.SetVal(FileInfo.m_strFiledValue);
			XmlOp.SetAttribVal(NULL, "ColValue",val);
			val.SetVal("");
			XmlOp.OutofKey();
		}
	}
	XmlOp.OutofKey();

	val.SetVal("");
	XmlOp.SetElemVal("ResultInfo", val);
	XmlOp.GoIntoKey("ResultInfo");
	// 以下代码用时过长：测试发现 m_DetectResult.m_AnlysisResultArr 若有1000个元素，大概需要7分钟; 待优化
	for (int i = 0 ; i < m_DetectResult.m_AnlysisResultArr.GetSize(); i ++)
	{
		tagAnlysisResult ResultInfo = (tagAnlysisResult)m_DetectResult.m_AnlysisResultArr.GetAt(i);
		CString strNode = "";
		/*/
		strNode.Format("%s","ResultData");
		/*/
		strNode.Format("%s%04d","ResultData",i);
		//*/
		val.SetVal("");
		XmlOp.SetElemVal(strNode, val);
		XmlOp.GoIntoKey(strNode);

		val.SetVal(ResultInfo.m_DetectType);
		XmlOp.SetAttribVal(NULL, "DetectType", val);

		val.SetVal(ResultInfo.m_strDetectTypeName);
		XmlOp.SetAttribVal(NULL, "DetectTypeName",val);

		CString strTemp;
		strTemp.Format("%I64d",ResultInfo.m_nFrameStart);
		val.SetVal(strTemp);
		XmlOp.SetAttribVal(NULL, "FrameStart",val);

		strTemp.Format("%I64d",ResultInfo.m_nFrameEnd);
		val.SetVal(strTemp);
		XmlOp.SetAttribVal(NULL, "FrameEnd",val);

		strTemp.Format("%I64d",ResultInfo.m_nDuration);
		val.SetVal(strTemp);
		XmlOp.SetAttribVal(NULL, "FrameDuration",val);
		XmlOp.OutofKey();
		OutputDebugString("写入XML字符串");
// 		XmlOp.SetAttribute("TimeCodeStart",ResultInfo.m_strTimeCodeStart,strErr,strNode);
// 		XmlOp.SetAttribute("TimeCodeEnd",ResultInfo.m_strTimeCodeEnd,strErr,strNode);
// 		XmlOp.SetAttribute("TimeCodeDuration",ResultInfo.m_strTimeCodeDuration,strErr,strNode);
	}
	XmlOp.UpdateData();
	return XmlOp;

}

//removed by tianyx 2013-12-6 too slow..
//slow not used
/*
CString CFileDriver::GetLastAnlysisResultAsXmlStringOld()
{
	CString strRetXml = "";
	CString strErr = "";
	CXMLOp XmlOp;
	XmlOp.CleanXMLDocument();
	XmlOp.CreateRoot("AnlysisResult",strErr);
	XmlOp.CreateNode("TskInfo",strErr,"Root");
	XmlOp.SetAttribute("TskID",m_strTskID,strErr,"TskInfo");
	XmlOp.SetAttribute("ClipID",m_strClipID,strErr,"TskInfo");
	XmlOp.SetAttribute("ClipName",m_strClipName,strErr,"TskInfo");
	XmlOp.SetAttribute("SrcDir",m_strFilePath,strErr,"TskInfo");
	XmlOp.SetAttribute("Level",m_DetectResult.m_strLevel,strErr,"TskInfo");
	XmlOp.SetAttribute("CheckKind",m_strCheckKind,strErr,"TskInfo");
	XmlOp.SetAttribute("FileType",m_strFileType,strErr,"TskInfo");
	XmlOp.SetAttribute("CheckItem",m_strCheckItem,strErr,"TskInfo");
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		CString strSOM;
		CString strEOM;
		strSOM.Format("%d",m_TaskSOM);
		LONGLONG EOM = m_TaskSOM+m_TaskDuration;
		strEOM.Format("%d",EOM);
		XmlOp.SetAttribute("SOM",strSOM,strErr,"TskInfo");
		XmlOp.SetAttribute("EOM",strEOM,strErr,"TskInfo");
		XmlOp.SetAttribute("SecID",m_TaskSectionID,strErr,"TskInfo");
	}


	CString strCheckOpt = "";
	strCheckOpt.Format("%d",m_nCheckOpt);
	XmlOp.SetAttribute("CheckOpt",strCheckOpt,strErr,"TskInfo");
	CString strPRI = "";
	strPRI.Format("%d",m_nPRI);
	XmlOp.SetAttribute("PRI",strPRI,strErr,"TskInfo");
	XmlOp.CreateNode("TskData",strErr,"Root");
	XmlOp.CreateNode("FileInfo",strErr,"TskData");
 	if (!GetFCVSServerConfig().m_bUseSectionFunction)
 	{
		for (int i = 0 ; i < m_DetectResult.m_DetectFileInfoArr.GetSize() ; i ++)
		{
			tagDetectFileInfo FileInfo = (tagDetectFileInfo)m_DetectResult.m_DetectFileInfoArr.GetAt(i);
			CString strNode = "";
			strNode.Format("%s%d","InfoData",i);
			XmlOp.CreateNode(strNode,strErr,"FileInfo");
			XmlOp.SetAttribute("ColName",FileInfo.m_strenFiledName,strErr,strNode);
			XmlOp.SetAttribute("ColValue",FileInfo.m_strFiledValue,strErr,strNode);
			
		}
 	}

	XmlOp.CreateNode("ResultInfo",strErr,"TskData");

	// 以下代码用时过长：测试发现 m_DetectResult.m_AnlysisResultArr 若有1000个元素，大概需要7分钟; 待优化
	for (int i = 0 ; i < m_DetectResult.m_AnlysisResultArr.GetSize(); i ++)
	{
		tagAnlysisResult ResultInfo = (tagAnlysisResult)m_DetectResult.m_AnlysisResultArr.GetAt(i);
		CString strNode = "";
		/ * /
		strNode.Format("%s","ResultData");
		/ * /
		strNode.Format("%s%d","ResultData",i);
		// * /
		
		XmlOp.CreateNode(strNode,strErr,"ResultInfo");
		CString strTemp = "";
		strTemp.Format("%d",ResultInfo.m_DetectType);
		XmlOp.SetAttribute("DetectType",strTemp,strErr,strNode);
		XmlOp.SetAttribute("DetectTypeName",ResultInfo.m_strDetectTypeName,strErr,strNode);
		strTemp.Format("%ld",ResultInfo.m_nFrameStart);
		XmlOp.SetAttribute("FrameStart",strTemp,strErr,strNode);
		strTemp.Format("%ld",ResultInfo.m_nFrameEnd,strErr,strNode);
		XmlOp.SetAttribute("FrameEnd",strTemp,strErr,strNode);
		strTemp.Format("%ld",ResultInfo.m_nDuration);
		XmlOp.SetAttribute("FrameDuration",strTemp,strErr,strNode);
		OutputDebugString("写入XML字符串");
// 		XmlOp.SetAttribute("TimeCodeStart",ResultInfo.m_strTimeCodeStart,strErr,strNode);
// 		XmlOp.SetAttribute("TimeCodeEnd",ResultInfo.m_strTimeCodeEnd,strErr,strNode);
// 		XmlOp.SetAttribute("TimeCodeDuration",ResultInfo.m_strTimeCodeDuration,strErr,strNode);
	}
	strRetXml = XmlOp.GetXMLAsString();
	return strRetXml;
}*/



BOOL CFileDriver::AnalysisResult()
{
	TCHAR szPath[MAX_PATH];
	TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	m_DetectResult.AnlysisResult();
	if ((GetFCVSServerConfig().m_bUseSectionFunction)&& (m_TaskSectionID !="1"))
	{
		strSQLArr.RemoveAll();
	}else
	{
		SaveFileInfoToDB();
	}
	
	SaveResultToDB();//写数据库SQL语句结果在CStringArray :: strSQLArr
	if (GetFCVSServerConfig().m_bSaveXml)
	{
		CString strXml = GetLastAnlysisResultAsXmlString();
		if (strXml.IsEmpty())
		{
			return FALSE;
		}


		CString strPath = "";
		CString strFilePath = GetFCVSServerConfig().m_strXmlName;
		if(!strFilePath.IsEmpty())
		{
			_tsplitpath(m_strFilePath.GetBuffer(0),drive,dir,fname,ext);
			strPath.Format("%s%s%s",drive,dir,fname);
			int nFind = strFilePath.Find(":");
			CString strS,strE;
			if (nFind >=1)
			{
				strS = strFilePath.Left(nFind);
				strE = strFilePath.Right(strFilePath.GetLength() - nFind - 1);
				if (strS.CompareNoCase("Name") == 0)
				{
					if (GetFCVSServerConfig().m_bUseInCaptureReview && m_strFileType.CompareNoCase("MSS") == 0)
					{
						strPath += "\\std" + strE;
					}
					else
					{
						strPath += strE;
					}
				}
			}
			CString strDir = "";
			strDir.Format("%s%s",drive,dir);
			CString strTemp = "";
			strTemp.Format("%s%s",fname,strE);
			GetFCVSServerConfig().FileFind(strDir,strTemp,TRUE);
		}
		else
		{
			if (GetFCVSServerConfig().m_bUseSectionFunction)
			{
				_tsplitpath(m_strFilePath.GetBuffer(0),drive,dir,fname,ext);
				strPath.Format("%s%s%s_%s%s",drive,dir,fname,m_TaskSectionID,"_Detect.xml");
				CString strDir = "";
				strDir.Format("%s%s",drive,dir);
				CString strTemp = "";
				strTemp.Format("%s_%s%s",fname,m_TaskSectionID,"_Detect.xml");
				GetFCVSServerConfig().FileFind(strDir,strTemp,TRUE);
			}else
			{
				/*/
				::GetModuleFileName(NULL,szPath,MAX_PATH);
				_tsplitpath(szPath,drive,dir,fname,ext);
				strPath.Format("%s%s%s%s%s%s",drive,dir,"ResultXml\\",m_strClipID,m_TaskSectionID,"_Detect.xml");
				CString strDir = "";
				strDir.Format("%s%s",drive,dir);
				GetFCVSServerConfig().DirectoryFind(strDir,"ResultXml",TRUE);
				GetFCVSServerConfig().FileFind(strDir + "\\ResultXml",m_strClipID +m_TaskSectionID+ "_Detect.xml",TRUE);
			/*/
				_tsplitpath(m_strFilePath.GetBuffer(0),drive,dir,fname,ext);
				strPath.Format("%s%s%s%s",drive,dir,fname,"_Detect.xml");
// 				if (!m_strSaveXmlPath.IsEmpty() && m_strSaveXmlPath.GetAt(m_strSaveXmlPath.GetLength() - 1) != '\\')
// 				{
// 					m_strSaveXmlPath = m_strSaveXmlPath + "\\";
// 				}
// 				strPath.Format("%s%s_%s_Detect.xml", m_strSaveXmlPath, fname, m_TaskSectionID);
				//-----

				CString strDir = "";
				strDir.Format("%s%s",drive,dir);
				CString strTemp = "";
				strTemp.Format("%s%s",fname,"_Detect.xml");
				GetFCVSServerConfig().FileFind(strDir,strTemp,TRUE);
				//*/
			  }
		}
		CXMLOp XmlOp;
		XmlOp.CleanXMLDocument();
		XmlOp.LoadXMLFromString(strXml);
		CString strFirstTagName((char*)XmlOp.GetFistTagName());
		// 多尝试几次
		for (int i = 0; i < 5; ++i)
		{
			BOOL save_result = XmlOp.SaveXML(strPath);
			if (save_result)
			{
				break;
			}
			if (4 == i)
			{
				CString strLog = "";
				strLog.Format("strTaskID = %s,ConText = %s(路径:)",m_strTskID,"写技审结果XML文件失败",strPath);
//				glReportLog("Log_Task",strLog);
				GetTxLogMgr()->WriteLog(1,strLog);
			}
			Sleep(500);
		}
	}

	//下面写SQL语句文件
// 	CString strTemp;
// 	
// 	CFile mFile;
// 	
// 	mFile.Open("d:\\dd\\try.TRY",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
// 	
// 	CArchive ar(&mFile,CArchive::store);
// 	
// 	ar<<strTemp;
// 	
// 	ar.Close();
// 	
// 	mFile.Close();
	CString strSQLTemp = "";
	strSQLTemp.Format("%s%s%s.sql",drive,dir,fname);
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		strSQLTemp = "";
		strSQLTemp.Format("%s%s%s_wow%s.sql",drive,dir,fname,m_TaskSectionID);
	}
// 	std::ofstream SQLfile;
// 	SQLfile.open(strSQLTemp);
	CFile mFile;
	if(!mFile.Open(strSQLTemp.GetBuffer(strSQLTemp.GetLength()),CFile::modeCreate|CFile::modeWrite))
	{
		//CFWriteLog()
		OutputDebugString("创建文件失败");
		return FALSE;
	}
	
	//CArchive ar(&mFile,CArchive::store);
	CString enterB = '\n';
	int nSQL =  strSQLArr.GetSize();
	for(int i = 0 ; i<nSQL ; i++)
	{
		CString SQLtemp ="";
		SQLtemp = strSQLArr[i];
		mFile.Write(SQLtemp.LockBuffer(),SQLtemp.GetLength());
		SQLtemp.UnlockBuffer();
		//ar<<enterB;
		mFile.Write("\r\n",2);

	}
//	ar.Close();
	mFile.Close();



//wangjun 20081106
//下面代码可能造成，发送任务状态4丢失
//------------------------------------------------------------------------------
// 	char* pMsg = new char[strXml.GetLength() + 1];
// 	char* pId = new char[m_strTskID.GetLength() + 1];
// 	strcpy(pMsg,strXml.GetBuffer(0));
// 	strcpy(pId,m_strTskID.GetBuffer(0));
// 	::PostMessage(m_hParentWnd,WM_BROADCAST_FILE_INFO,(WPARAM)pMsg,(LPARAM)pId);
//--------------------------------------------------------------------------------
	return TRUE;
}



void CFileDriver::SetTaskStatus(int nStatus)
{
	CString strLog = "";
	CString strFiled = "";
	Lock();
	m_nStatus = nStatus;
	UnLock();
	switch(m_nStatus)
	{
	case TSK_IS_RUNING:
		SetEvent(m_ThreadEvent);
		SetEvent(m_evAThreadEvent);
		strFiled = "tStartTime";
		break;
	case TSK_IS_PAUSED:
		ResetEvent(m_ThreadEvent);
		ResetEvent(m_evAThreadEvent);
		break;
	case TSK_IS_CANNELED:
		SetEvent(m_ThreadEvent);
		SetEvent(m_evAThreadEvent);
		strFiled = "tEndTime";
		break;
	case TSK_IS_FINISHED://wangjun 修改异常结束也要写数据库结果
		{
		SetEvent(m_ThreadEvent);
		SetEvent(m_evAThreadEvent);
		//AnalysisResult();
			//需要分别计算等级
			//m_ResultLevelArr.GetAt(0)
		}
	case TSK_IS_ABORTED:
		{
			AnalysisResult();
			strFiled = "tEndTime";
		}
		break;
	}

	strLog.Format("strTaskID = %s,ConText = %s(%d)",m_strTskID,"任务状态",nStatus);
//	glReportLog("Log_Task",strLog);


	//wangjun 20081018,
	//任务管理判断任务状态的同时，还会根据Submit返回消息是否任务为最大数，来提交新任务
	//任务调度程序写数据库，开关统一控制
	BOOL bSucStatus = TRUE;

	//wangjun 这样就不会出现先析构对象，后写数据库，由于m_strTskID = ""报错的问题了
	CString strErr = "";
	CXMLOp ReXmlOp;
	ReXmlOp.CleanXMLDocument();
	ReXmlOp.CreateBroadCastRoot(strErr);
	ReXmlOp.CreateNode("Info",strErr,"Root");
	ReXmlOp.SetAttribute("TskID",m_strTskID,strErr,"Info");
	ReXmlOp.SetAttribute("BDType","TskStatus",strErr,"Info");
	ReXmlOp.CreateNode("Data",strErr,"Root");
	CString strStatus = "";
	strStatus.Format("%d",m_nStatus);
	//如果写任务状态失败,那么返回错误码
	ReXmlOp.SetAttribute("Status",strStatus,strErr,"Data");
	if (!bSucStatus)
	{
		ReXmlOp.SetAttribute("ErrInfo","-1",strErr,"Data");
	}
	else
	{
        ReXmlOp.SetAttribute("ErrInfo","1",strErr,"Data");
	}
	//SecID
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		CString strSectionID = m_TaskSectionID;
		ReXmlOp.SetAttribute("SecID",strSectionID,strErr,"Data");
	}
	//SecID
	CString strMsg = ReXmlOp.GetXMLAsString();
	char* pMsg = new char[strMsg.GetLength() + 1];
	char* pId = new char[m_strTskID.GetLength() + 1];
	strcpy(pMsg,strMsg.GetBuffer(0));
	strcpy(pId,m_strTskID.GetBuffer(0));


	OutputDebugString(strMsg);
	::PostMessage(m_hParentWnd,WM_UPDATE_TSK_STATUS,(WPARAM)pMsg,(LPARAM)pId);
}

void CFileDriver::OnUpdateProgress()
{
//	char* pId1 = new char[m_strTskID.GetLength() + 1];
//	strcpy(pId1,m_strTskID.GetBuffer(0));
//	::PostMessage(m_hParentWnd,WM_STEP_BY_FRAME,(WPARAM)nCount,(LPARAM)pId1);
	LONGLONG llDuration = 0;
	if ((m_TaskDuration != 0)&&(GetFCVSServerConfig().m_bUseSectionFunction))
	{
		llDuration = m_TaskDuration;
	}
	else
	{
		llDuration = GetFileDuration();
	}

	//对于电视台素材一般长度一般不超过24小时，LONGLONG转换为float一般不会出问题
	//长度小于100的素材，进度计算不出来
 	float fInValue = llDuration / 100;
 	nCount ++;
 	int nProcess = nCount / fInValue;
	if (nProcess >= 100)
	{
		nProcess = 100;
	}

	if (nProcess == nPos)
	{
		return;
	}
	nPos = nProcess;
	
	DWORD m_currTime = timeGetTime();
	DWORD diff = m_currTime - m_startTime;
		
	int ms = diff%1000;
	int totalSeconds = diff/1000;
	int hour = totalSeconds/3600;
	totalSeconds -= hour*3600;
	int minute = totalSeconds/60;
	totalSeconds -= minute*60;
	int second = totalSeconds;

	CString str;
	str.Format("检测用时%02d:%02d:%02d:%d", hour, minute, second, ms);
	//GetTxLogMgr()->WriteLog(1,str);
	CString strErr = "";
	CXMLOp BdXmlOp;
	BdXmlOp.CleanXMLDocument();
	BdXmlOp.CreateBroadCastRoot(strErr);
	BdXmlOp.CreateNode("Info",strErr,"Root");
	BdXmlOp.SetAttribute("TskID",m_strTskID,strErr,"Info");
	BdXmlOp.SetAttribute("BDType","Progress",strErr,"Info");

	
	CString strProcess = "";
	strProcess.Format("%d",nProcess);
	BdXmlOp.CreateNode("Data",strErr,"Root");
	
//	BdXmlOp.SetAttribute("SecID","SecID",strErr,"Data");
	//SecID
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		CString strSectionID = m_TaskSectionID;
		BdXmlOp.SetAttribute("SecID",strSectionID,strErr,"Data");
	}
	//SecID
	BdXmlOp.SetAttribute("Precent",strProcess,strErr,"Data");
	BdXmlOp.SetAttribute("UseTime",str,strErr,"Data");
	CString strMsg = BdXmlOp.GetXMLAsString();
	char* pMsg = new char[strMsg.GetLength() + 1];
	char* pId = new char[m_strTskID.GetLength() + 1];
	strcpy(pMsg,strMsg.GetBuffer(0));
	strcpy(pId,m_strTskID.GetBuffer(0));
	::PostMessage(m_hParentWnd,WM_UPDATE_TSK_STATUS,(WPARAM)pMsg,(LPARAM)pId);
	CString strLog;
	strLog.Format("myOut: PostMessage = %s",strMsg);
	OutputDebugString(strLog);

}

int CFileDriver::GetCurrentStatus()
{
	return m_nStatus;
}

CString CFileDriver::GetLastErrInfo()
{
	return m_strLastErrorInfo;
}

void CFileDriver::OnGetFileInfoFinished()
{
	CStringArray strenFiledArr,strenValueArr,strcnFiledArr,strcnValueArr;
	GetFileInfoByArr(strenFiledArr,strenValueArr,TRUE);
	GetFileInfoByArr(strcnFiledArr,strcnValueArr,FALSE);
	for (int i = 0 ; i < strenFiledArr.GetSize() ; i ++)
	{
		m_DetectResult.AppendFileInfo(strenValueArr.GetAt(i),strenFiledArr.GetAt(i),strcnFiledArr.GetAt(i));
	}
	if (!GetFCVSServerConfig().m_bBroadCast)
	{
		return;
	}
	LONGLONG FileDuration = 0;
	if ((m_TaskDuration != 0)&&(GetFCVSServerConfig().m_bUseSectionFunction))
	{
		FileDuration = m_TaskDuration;
	}
	else
	{
		FileDuration = GetFileDuration();
	}
	//long FileDuration = GetFileDuration();
	int nLen = 0;
	CString strErr;
	CXMLOp XmlBroadCast;
	XmlBroadCast.CleanXMLDocument();
	XmlBroadCast.CreateBroadCastRoot(strErr);
	XmlBroadCast.CreateNode("Info",strErr,"Root");
	XmlBroadCast.SetAttribute("TskID",m_strTskID,strErr,"Info");
	XmlBroadCast.SetAttribute("BDType","FileInfo",strErr,"Info");
	XmlBroadCast.CreateNode("Data",strErr,"Root");
	for (i = 0 ; i < strenFiledArr.GetSize() ; i ++)
	{
		CString strTemp = "";
		strTemp.Format("%d",i);
		XmlBroadCast.CreateNode("FileData"+strTemp,strErr,"Data");
		XmlBroadCast.SetAttribute("ColName",strenFiledArr.GetAt(i),strErr,"FileData"+strTemp);
		XmlBroadCast.SetAttribute("ColValue",strenValueArr.GetAt(i),strErr,"FileData"+strTemp);
	}
	//SecID
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		CString strSectionID = m_TaskSectionID;
		XmlBroadCast.SetAttribute("SecID",strSectionID,strErr,"Data");
	}
	//SecID
	CString strMsg = XmlBroadCast.GetXMLAsString();
	char* pMsg = new char[strMsg.GetLength() + 1];
	char* pId = new char[m_strTskID.GetLength() + 1];
	strcpy(pMsg,strMsg.GetBuffer(0));
	strcpy(pId,m_strTskID.GetBuffer(0));
	::PostMessage(m_hParentWnd,WM_BROADCAST_FILE_INFO,(WPARAM)pMsg,(LPARAM)pId);
}
//得到检测驱动反馈的视频检测结果
void CFileDriver::ProcessResult(DETECT_TYPE Result,long Reserved)
{
	m_DetectResult.AppendDetectResult(Reserved,Result);
// 	if (!GetFCVSServerConfig().m_bBroadCast)
// 	{
// 		return;
// 	}
// 	CString strErr = "";
// 	CXMLOp XmlBroadCast;
// 	XmlBroadCast.CleanXMLDocument();
// 	XmlBroadCast.CreateBroadCastRoot(strErr);
// 	XmlBroadCast.CreateNode("Info",strErr,"Root");
// 	XmlBroadCast.SetAttribute("TskID",m_strTskID,strErr,"Info");
// 	XmlBroadCast.SetAttribute("BDType","DetectResult",strErr,"Info");
// 	XmlBroadCast.CreateNode("Data",strErr,"Root");
// 	XmlBroadCast.CreateNode("Data",strErr,"Root");
// 	CString strErrorType = "";
// 	CString strFrameNum = "";
// 	strFrameNum.Format("%ld",Reserved);
// 	strErrorType.Format("%ld",Result);
// 	XmlBroadCast.SetAttribute("FrameNum",strFrameNum,strErr,"Data");
// 	XmlBroadCast.SetAttribute("ErrType",strErrorType,strErr,"Data");
// 	XmlBroadCast.SetAttribute("ErrInfo",m_DetectResult.GetDetectTypeName(Result),strErr,"Data");
// 	//SecID
// 	if (GetFCVSServerConfig().m_bUseSectionFunction)
// 	{
// 		CString strSectionID = m_TaskSectionID;
// 		XmlBroadCast.SetAttribute("SecID",strSectionID,strErr,"Data");
// 	}
// 	//SecID
// 	CString strMsg = XmlBroadCast.GetXMLAsString();
// 	char* pMsg = new char[strMsg.GetLength() + 1];
// 	char* pId = new char[m_strTskID.GetLength() + 1];
// 	strcpy(pMsg,strMsg.GetBuffer(0));
// 	strcpy(pId,m_strTskID.GetBuffer(0));
//	::PostMessage(m_hParentWnd,WM_BROADCAST_DETECT_RESULT,(WPARAM)pMsg,(LPARAM)pId);
}

//得到检测驱动反馈的音频检测结果
void CFileDriver::ProcessResultAudio(DETECT_TYPE Result, __int64 nStart, __int64 nDuration, __int64 qReserved)
{

	LONGLONG lStartFrame = 0;
	LONGLONG lDuration = 0;
	if(m_StreamInfo.VideoInfo.iFrameRate == 0)
		return;
	nDiv = m_StreamInfo.AudioInfo.iAudioSamplingRate/m_StreamInfo.VideoInfo.iFrameRate;
	if(nDiv == 0)
		return;
    lStartFrame   = nStart/*/nDiv*/;
	lDuration = nDuration/*/nDiv*/;

	//判断如果检测错误帧数已经大于素材总帧数，则不报告出来
	/*/
	long lFTotalFrame = GetFileDuration();
	/*/
	LONGLONG lFTotalFrame = 0;
	if ((m_TaskDuration != 0)&&(GetFCVSServerConfig().m_bUseSectionFunction))
	{
		lFTotalFrame = m_TaskDuration;
	}
	else
	{
		lFTotalFrame = GetFileDuration();
	}
	//*/
	if(lFTotalFrame < lStartFrame)
		return;

	if(lFTotalFrame < (lStartFrame + lDuration))
		lDuration = lFTotalFrame - lStartFrame;

   	m_DetectResult.AppendAudioDetectResult(lStartFrame,lDuration,Result);

	
// 	if (!GetFCVSServerConfig().m_bBroadCast)
// 	{
// 		return;
// 	}
// 	for (int i = 0; i < lDuration; i++)
// 	{
// 		CString strErr = "";
// 		CXMLOp XmlBroadCast;
// 		XmlBroadCast.CleanXMLDocument();
// 		XmlBroadCast.CreateBroadCastRoot(strErr);
// 		XmlBroadCast.CreateNode("Info",strErr,"Root");
// 		XmlBroadCast.SetAttribute("TskID",m_strTskID,strErr,"Info");
// 		XmlBroadCast.SetAttribute("BDType","DetectResult",strErr,"Info");
// 		XmlBroadCast.CreateNode("Data",strErr,"Root");
// 		CString strErrorType = "";
// 		CString strFrameNum = "";
// 		strFrameNum.Format("%ld",lStartFrame++);
// 		strErrorType.Format("%ld",Result);
// 		XmlBroadCast.SetAttribute("FrameNum",strFrameNum,strErr,"Data");
// 		XmlBroadCast.SetAttribute("ErrType",strErrorType,strErr,"Data");
// 		XmlBroadCast.SetAttribute("ErrInfo",m_DetectResult.GetDetectTypeName(Result),strErr,"Data");
// 		//SecID
// 		if (GetFCVSServerConfig().m_bUseSectionFunction)
// 		{
// 			CString strSectionID = m_TaskSectionID;
// 			XmlBroadCast.SetAttribute("SecID",strSectionID,strErr,"Data");
// 		}
// 	//SecID
// 		CString strMsg = XmlBroadCast.GetXMLAsString();
// 		char* pMsg = new char[strMsg.GetLength() + 1];
// 		char* pId = new char[m_strTskID.GetLength() + 1];
// 		strcpy(pMsg,strMsg.GetBuffer(0));
// 		strcpy(pId,m_strTskID.GetBuffer(0));
// 	 //   ::PostMessage(m_hParentWnd,WM_BROADCAST_DETECT_RESULT,(WPARAM)pMsg,(LPARAM)pId);
// 	}

}
void CFileDriver::AudioCount(__int64 count)
{
	if (nDiv !=0)
	{
		m_AudioCount = count/nDiv; 
	}

	//endSection();	
}
void CFileDriver::VideoCount(long count)
{	
	m_VideoCount = count;
//	endSection();
}
LRESULT CFileDriver::OnDecodeMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_DS_EC_COMPLETE:
		SetReturnType(DR_NOMAL_FINISHED);
		break;
	case WM_DS_EC_PAUSED:
		break;
	case WM_DS_EC_SHUTTING_DOWN:
		break;
	case WM_DS_EC_STOP:
		SetReturnType(DR_STOP);
		break;
	case WM_DS_EC_REPAINT:
		break;
	case WM_DS_EC_EC_SNDDEV_IN_ERROR:
		break;
	case WM_DS_EC_EC_SNDDEV_OUT_ERROR:
		break;
	case WM_DS_EC_STREAM_ERROR_STILLPLAYING:
		break;
	case WM_DS_EC_STREAM_ERROR_STOPPED:
		break;
	case WM_DS_EC_USERABORT:
		break;
	case WM_DS_EC_USE_CLEANUP:
		SetReturnType(DR_USER_CANCEL);
		break;
	case WM_DS_EC_USE_UNRENDER://wangjun 20081024
		{
			SetReturnType(DR_ERROR_ABORT);
			CString strLog = "";
			strLog.Format("m_strTskID = %s,ConText = %s(%s)",m_strTskID,"任务状态","连接解码链路失败!");
//			glReportLog("Log_Task",strLog);
			break;
			
		}
	case WM_IC_UNRECOG:
		SetReturnType(DR_ERROR_ABORT);
		CString strErrResult = "";
        if(wParam == 1)
			strErrResult = "技审文件不存在";
		else if(wParam == 2)
			strErrResult = "语法检测失败";
		else if(wParam == 3)
			strErrResult = "文件正在传输或被锁定，加载失败";
		else if(wParam == 4)
			strErrResult = "获取视频流失败";
		else
			strErrResult = "素材格式不可识别";
		CString strLog = "";
		strLog.Format("m_strTskID = %s,ConText = %s(%s)",m_strTskID,"任务状态",strErrResult);
//	    glReportLog("Log_Task",strLog);

		m_strLastErrorInfo = strErrResult;
		break;
	}
	return 1L;
}

LRESULT CFileDriver::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_DS_EC_COMPLETE:
		break;
	case WM_DS_EC_PAUSED:
		break;
	case WM_DS_EC_SHUTTING_DOWN:
		break;
	case WM_DS_EC_STOP:
		break;
	case WM_DS_EC_REPAINT:
		break;
	case WM_DS_EC_EC_SNDDEV_IN_ERROR:
		break;
	case WM_DS_EC_EC_SNDDEV_OUT_ERROR:
		break;
	case WM_DS_EC_STREAM_ERROR_STILLPLAYING:
		break;
	case WM_DS_EC_STREAM_ERROR_STOPPED:
		break;
	case WM_DS_EC_USERABORT:
		break;
	case WM_DS_EC_USE_CLEANUP:
		break;
	case WM_DS_EC_USE_UNRENDER://wangjun 应该结束检测进程
		break;
	case WM_IC_UNRECOG:
		break;
	}	
	
	return CWnd::DefWindowProc(message, wParam, lParam);
}
//得到一个级别，这个级别以下的检测结果不纪录数据库
tagResultLevel CDetectResult::GetLowestLevel()
{
	int iSize = GetFCVSServerConfig().m_ResultLevelArr.GetSize();
	if (iSize <= 0)
	{
		m_LowestLevel.m_strLevelName = "";
		m_LowestLevel.m_nBlackFrame = 0;
		m_LowestLevel.m_nColorFrame = 0;
		m_LowestLevel.m_nColorStrip = 0;
		m_LowestLevel.m_nStillFrame = 0;

		m_LowestLevel.m_nColorCast  = 0;
	}
	else
	{
		tagResultLevel Level = (tagResultLevel)GetFCVSServerConfig().m_ResultLevelArr.GetAt(0);
	    m_LowestLevel.m_strLevelName = Level.m_strLevelName;
		m_LowestLevel.m_nBlackFrame = Level.m_nBlackFrame;
		m_LowestLevel.m_nColorFrame = Level.m_nColorFrame;
		m_LowestLevel.m_nColorStrip = Level.m_nColorStrip;
		m_LowestLevel.m_nStillFrame = Level.m_nStillFrame;

		m_LowestLevel.m_nColorCast  = Level.m_nColorCast;

       for (int i = 1; i < iSize; i++)
       {
		   tagResultLevel Level = (tagResultLevel)GetFCVSServerConfig().m_ResultLevelArr.GetAt(i);
		   //这里为了实现配置可以无序排序，等级名称必须是Ascall码，如A,B,C或1，2，3
		   if (m_LowestLevel.m_strLevelName.Compare(Level.m_strLevelName) < 0)
		   {
			   m_LowestLevel.m_strLevelName = Level.m_strLevelName;
			   m_LowestLevel.m_nBlackFrame = Level.m_nBlackFrame;
			   m_LowestLevel.m_nColorFrame = Level.m_nColorFrame;
			   m_LowestLevel.m_nColorStrip = Level.m_nColorStrip;
		       m_LowestLevel.m_nStillFrame = Level.m_nStillFrame;

			   m_LowestLevel.m_nColorCast  = Level.m_nColorCast;
		   }
       }
	}
	m_LowestLevel = GetFCVSServerConfig().GetFilterLevel();
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{

	}
	return m_LowestLevel;
}

long CFileDriver::CheckItemForStr2L(CString strCheckItem)
{
	long lCheckItem = 0;
	if (m_strCheckItem.GetLength() == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			char T = m_strCheckItem.GetAt(i);
			switch(T)
			{
			case '0':
				break;
			case '1':
				lCheckItem += pow(2,4-i-1);
				break;
			default:
				return 0;
			}
		}
	}
	return lCheckItem;
				
}

void CFileDriver::SetRetryInfo(DWORD dwRetryTimes, DWORD dwRetryInterval)
{	
	if (m_pFileDriver)
	{
		m_pFileDriver->SetRetryTimes(dwRetryTimes, dwRetryInterval);
	}
	
}
void CFileDriver::ReportLog(UINT iType,CString userMsg)
{
	CString log = m_strTskID + " " + m_strFilePath + " ";
	CString sysMsg = "";
	switch (iType)
	{
	case TSK_IS_RUNING:
		sysMsg = "技审任务开始执行";
		break;
	case TSK_IS_PAUSED:
		sysMsg = "用户暂停技审任务";
		break;
	case TSK_IS_CANNELED:
		sysMsg = "用户取消技审任务";
		break;
	case TSK_IS_FINISHED:
		sysMsg = "技审任务正常结束";
		break;
	case TSK_IS_ABORTED:
		sysMsg = "技审任务异常结束";
		break;
	default:
		sysMsg = "";
	}
	log += sysMsg + " " + userMsg;
//	glReportLog("LOG_LOG",log);
}

void CFileDriver::SeekTo(LONGLONG som,LONGLONG duration)
{
	m_pFileDriver->seekTo(som, duration);
}
BOOL CFileDriver::isSectionEnd()
{
	//m_TaskSOM;
	//*/
	if (nCount>=m_TaskDuration)
	/*/
	if ((m_VideoCount>=m_TaskDuration) && (m_AudioCount>=m_TaskDuration))
	//*/
	{
		return TRUE;
	}
	return FALSE;
	//m_TaskDuration;
}
void CFileDriver::endSection()
{
	if (isSetSectionEnd)
	{
		return;
	}
	if (isSectionEnd())
	{
		CancelTask();
		SetReturnType(DR_NOMAL_FINISHED);
		
		SetTaskStatus(TSK_IS_FINISHED);
		
		SetEvent(m_evThreadExit);
		SetEvent(m_ThreadEvent);
		SetEvent(m_evAThreadEvent);
		isSetSectionEnd = TRUE;
	}
}

BOOL CFileDriver::SaveFileInfoToDB()
{	
	strSQLArr.RemoveAll();
	for (int i = 0 ; i < m_DetectResult.m_DetectFileInfoArr.GetSize(); i ++)
	{
		tagDetectFileInfo FileInfo = (tagDetectFileInfo)m_DetectResult.m_DetectFileInfoArr.GetAt(i);
		CString strSQL = "Insert into T_FCVSFileInfo values('";
		strSQL += m_strTskID;
		strSQL += "','";
		strSQL += FileInfo.m_strenFiledName;
		strSQL += "','";
		strSQL += FileInfo.m_strFiledValue;
		strSQL += "')";
		strSQLArr.Add(strSQL);

	}

	return TRUE;
}

BOOL CFileDriver::SaveResultToDB()
{
	LONGLONG tempSom = 0;
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		tempSom = m_TaskSOM;
	}
	for (int i = 0 ; i < m_DetectResult.m_AnlysisResultArr.GetSize(); i ++)
	{
		tagAnlysisResult Anlysis = (tagAnlysisResult)m_DetectResult.m_AnlysisResultArr.GetAt(i);
		int nDetectType = Anlysis.m_DetectType;
		CString strDetectType;
		strDetectType.Format("%d",nDetectType);
		CString strFrameStart,strFrameEnd,strFrameDuration;
		strFrameStart.Format("%d",Anlysis.m_nFrameStart+tempSom);
		strFrameEnd.Format("%d",Anlysis.m_nFrameEnd+tempSom);
		strFrameDuration.Format("%d",Anlysis.m_nDuration);
		CString strSQL = "Insert into T_FCVSClipInfo values('";
		strSQL += m_strTskID;
		strSQL += "','";
		strSQL += strDetectType;
		strSQL += "','";
		strSQL += Anlysis.m_strDetectTypeName;
		strSQL += "','";
		strSQL += strFrameStart;
		strSQL += "','";
		strSQL += strFrameEnd;
		strSQL += "','";
		strSQL += strFrameDuration;
		strSQL += "','";
		strSQL += Anlysis.m_strTimeCodeStart;
		strSQL += "','";
		strSQL += Anlysis.m_strTimeCodeEnd;
		strSQL += "','";
		strSQL += Anlysis.m_strTimeCodeDuration;
		strSQL += "')";		
		strSQLArr.Add(strSQL);
	}

//	g_bIsWriteResultToDB = 0;
	return TRUE;
}
