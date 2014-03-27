// FileDriver_M2V.cpp: implementation of the CFileDriver_M2V class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FCVSServer.h"
#include "FileDriver_M2V.h"
#include "RuntimeClassFactory.h"
#include "XMLOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CFileDriver_M2V::m_nNum = 0;
REGISTER_CLASS(CFileDriver_M2V)
CFileDriver_M2V::CFileDriver_M2V() : CFileDriver(&m_DriverMgr_M2V)
{
	OutputDebugString("into CFileDriver_M2V::CFileDriver_M2V()\n");
	m_strTskID = "";
	m_strCheckKind = "";
	m_strFileType = "";
	m_strClipID = "";
	m_strClipName = "";
	m_strFilePath = "";
	m_strCheckItem = "";
	m_nPRI = 0;

	m_pAutoDetect = AutoDetect::CreateInstance();
//	m_pAutoDetect->XMLConfig((LPCSTR)GetFCVSServerConfig().GetDetectParaXml());
//	m_DriverMgr_M2V.SetCallBack((JS_CCallBack*)this);
//	m_DriverMgr_M2V.SetBuffer((void*)&m_SampleList);
//	m_DriverMgr_M2V.SetParent(m_hWnd);
//	OutputDebugString("Leave CFileDriver_M2V::CFileDriver_M2V()\n");
}

CFileDriver_M2V::~CFileDriver_M2V()
{
	OutputDebugString("into CFileDriver_M2V::~CFileDriver_M2V()\n");
	
}

void CFileDriver_M2V::ShowMsg()
{
	AfxMessageBox(m_strMsg);
}

BOOL CFileDriver_M2V::InitTask(CString strXML)
{
	OutputDebugString("into CFileDriver_M2V::InitTask(CString strXML)\n");
	if (strXML.IsEmpty())
	{
		return FALSE;
	}
	CString strErr = "";
	CXMLOp XmlOpReader,ReXmlOp;
	XmlOpReader.LoadXMLFromString(strXML);
	XmlOpReader.GetAttribute("CheckKind",m_strCheckKind,strErr,"TaskData");
	XmlOpReader.GetAttribute("FileType",m_strFileType,strErr,"TaskData");
	XmlOpReader.GetAttribute("TskID",m_strTskID,strErr,"TaskData");
	XmlOpReader.GetAttribute("ClipID",m_strClipID,strErr,"TaskData");
	XmlOpReader.GetAttribute("ClipName",m_strClipName,strErr,"TaskData");
	XmlOpReader.GetAttribute("SrcDir",m_strFilePath,strErr,"TaskData");
	XmlOpReader.GetAttribute("CheckItem",m_strCheckItem,strErr,"TaskData");
	CString strPRI = "";
	XmlOpReader.GetAttribute("PRI",strPRI,strErr,"TaskData");
	m_nPRI = atoi(strPRI);
	CString strCheckOpt = "";
	XmlOpReader.GetAttribute("CheckOpt",strCheckOpt,strErr,"TaskData");
	m_nCheckOpt = atoi(strCheckOpt);

	
	//wangjun add for AutoDectect Dyn Init
	BOOL bInitSuccess = TRUE;
	XmlOpReader.GetAttribute("ClipType",m_strClipType,strErr,"TaskData");
	CString strClipTypePara = (LPCSTR)GetFCVSServerConfig().GetDetectParaXml(m_strClipType);
    CString strDriverPara   = GetFCVSServerConfig().GetXmlByClassName("CFileDriver_M2V");
	CXMLOp XmlClipTypePara,XmlDrivePara;
	if (strClipTypePara.IsEmpty() || strDriverPara.IsEmpty())
	{
		bInitSuccess = FALSE;
		AfxMessageBox("无法找到检测参数，程序无法正常运行!");
		//return FALSE;
	}
	else
	{
       	XmlClipTypePara.LoadXMLFromString(strClipTypePara);
		XmlDrivePara.LoadXMLFromString(strDriverPara);
		CString strWidth,strHeight;
		XmlDrivePara.GetAttribute("Class",strWidth,strErr,"Data");
		XmlDrivePara.GetAttribute("DetectedClip_Width",strWidth,strErr,"Data");
		XmlDrivePara.GetAttribute("DetectedClip_Height",strHeight,strErr,"Data");
		
		XmlClipTypePara.SetAttribute("CLIP_WIDTH",strWidth,strErr,"Data");
		XmlClipTypePara.SetAttribute("CLIP_HEIGHT",strHeight,strErr,"Data");
		
		CString strConfig = XmlClipTypePara.GetXMLAsString();
		if (strConfig.IsEmpty())
		{
			bInitSuccess = FALSE;
		}
		else
		{
			m_pAutoDetect->XMLConfig(strConfig);
			//wangjun 20081114
			long lCheckItem = CheckItemForStr2L(m_strCheckItem);
			if (lCheckItem != 0)
			{
				m_pAutoDetect->SetDetectType(lCheckItem);
			}
		}
	}
    
	m_DriverMgr_M2V.SetCallBack((JS_CCallBack*)this);
	m_DriverMgr_M2V.SetBuffer((void*)&m_SampleList);
	m_DriverMgr_M2V.SetParent(m_hWnd);
	///add end

	ReXmlOp.CleanXMLDocument();
	ReXmlOp.CreateOperationReRoot(strErr);
	MSXML2::IXMLDOMElementPtr Info = XmlOpReader.FindNodeByNodeName("TaskInfo");
	ReXmlOp.AppendNode(Info,strErr,"Root");
	ReXmlOp.CreateNode("TaskData",strErr,"Root");
	ReXmlOp.SetAttribute("TskID",m_strTskID,strErr,"TaskData");
//	if (m_strTskID.IsEmpty() || m_strFilePath.IsEmpty())
	if (!bInitSuccess || m_strTskID.IsEmpty() || m_strFilePath.IsEmpty())
	{
		CString strReturn = "";
		strReturn.Format("%d",OPRET_TSK_SUBMIT_FAILE);
		ReXmlOp.SetAttribute("Result",strReturn,strErr,"TaskData");
		ReXmlOp.SetAttribute("ErrInfo",GetOPReturnInfo(OPRET_TSK_SUBMIT_FAILE),strErr,"TaskData");
	}
	else
	{
		CString strReturn = "";
		strReturn.Format("%d",OPRET_TSK_SUBMIT_SUCCESS);
		ReXmlOp.SetAttribute("Result",strReturn,strErr,"TaskData");
		ReXmlOp.SetAttribute("ErrInfo",GetOPReturnInfo(OPRET_TSK_SUBMIT_SUCCESS),strErr,"TaskData");
	}
	//SecID
	if (GetFCVSServerConfig().m_bUseSectionFunction)
	{
		CString strSectionID = m_TaskSectionID;
		ReXmlOp.SetAttribute("SecID",strSectionID,strErr,"TaskData");
	}
	//SecID
	CString strMsg = ReXmlOp.GetXMLAsString();
	char* pMsg = new char[strMsg.GetLength() + 1];
	char* pId = new char[m_strTskID.GetLength() + 1];
	strcpy(pMsg,strMsg.GetBuffer(0));
	strcpy(pId,m_strTskID.GetBuffer(0));
	::PostMessage(m_hParentWnd,WM_TSK_OPRET_MSG,(WPARAM)pMsg,(LPARAM)pId);
	m_SampleList.ReSet();
	nCount = 0;
	
	OutputDebugString("leave CFileDriver_M2V::InitTask(CString strXML)\n");
	return TRUE;
}

BOOL CFileDriver_M2V::RunTask()
{
	BOOL bRet = m_DriverMgr_M2V.Run();
	if (bRet)
	{
		SetReturnType(DR_RUN);
		SetTaskStatus(TSK_IS_RUNING);
	}
	return bRet;
	
	return 1;
}

BOOL CFileDriver_M2V::PauseTask()
{
	BOOL bRet = m_DriverMgr_M2V.Pause();
	if (bRet)
	{
		SetReturnType(DR_PAUSE);
		SetTaskStatus(TSK_IS_PAUSED);
	}
	return bRet;
	
	return 1;
}


BOOL CFileDriver_M2V::CancelTask()
{
	return m_DriverMgr_M2V.Cancel();
}

CString CFileDriver_M2V::GetTskID()
{
	return m_strTskID;
}

void CFileDriver_M2V::SetParentWnd(HWND hwnd)
{
	m_hParentWnd = hwnd;
}

LONGLONG CFileDriver_M2V::GetFileDuration()
{
	return m_StreamInfo.llFileDuration;
}

HRESULT CFileDriver_M2V::GetFileInfo(JS_StreamInfo p)
{
 	m_StreamInfo = p;
	OnGetFileInfoFinished();
	SetEvent(m_evFileInfoComplete);

	return 1L;
}

BOOL CFileDriver_M2V::GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML /* = TRUE */)
{
	CString strTemp;
	if (bXML)
	{
// 		strFileInfoFiledArr.Add("strOP");
// 		strFileInfoValueArr.Add(m_StreamInfo.strOP.c_str());
		strTemp.Format("%ld",m_StreamInfo.llFileDuration);
		strFileInfoFiledArr.Add("FileDuration");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("V_Codec");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strVideoCodecFormat.c_str());
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iVideoHeight);
		strFileInfoFiledArr.Add("V_Height");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iVideowidth);
		strFileInfoFiledArr.Add("V_Weight");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("V_HW");
		strTemp = m_StreamInfo.VideoInfo.strAspectRatio.c_str();
		strTemp.Replace("/",":");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("V_FrameRate");
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iFrameRate);
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.VideoInfo.lVideoBitRate);
		strFileInfoFiledArr.Add("V_BitRate");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iMaxGOP);
		strFileInfoFiledArr.Add("V_MaxGOP");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.VideoInfo.lvbvBufferSiae);
		strFileInfoFiledArr.Add("V_VBVCache");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("V_ColorSpace");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strColorSpace.c_str());

		strFileInfoFiledArr.Add("A_Codec");
		strFileInfoValueArr.Add(m_StreamInfo.AudioInfo.strAudioCodecFormat.c_str());
		strTemp.Format("%d",m_StreamInfo.AudioInfo.iAudioSamplingRate);
		strFileInfoFiledArr.Add("A_SamplingRate");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.AudioInfo.lAudioQuantizationBits);
		strFileInfoFiledArr.Add("A_QuantizationBits");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.AudioInfo.lChannels);
		strFileInfoFiledArr.Add("A_Channels");
		strFileInfoValueArr.Add(strTemp);
	}
	else
	{
// 		strFileInfoFiledArr.Add("操作模式(OP)");
// 		strFileInfoValueArr.Add(m_StreamInfo.strOP.c_str());
		strTemp.Format("%ld",m_StreamInfo.llFileDuration);
		strFileInfoFiledArr.Add("文件长度(帧)");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("图像编码");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strVideoCodecFormat.c_str());
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iVideoHeight);
		strFileInfoFiledArr.Add("图像高");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iVideowidth);
		strFileInfoFiledArr.Add("图像宽");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("高宽比");
		strTemp = m_StreamInfo.VideoInfo.strAspectRatio.c_str();
		strTemp.Replace("/",":");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("帧率");
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iFrameRate);
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.VideoInfo.lVideoBitRate);
		strFileInfoFiledArr.Add("比特率");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iMaxGOP);
		strFileInfoFiledArr.Add("最大GOP");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.VideoInfo.lvbvBufferSiae);
		strFileInfoFiledArr.Add("VBV缓存");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("颜色空间");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strColorSpace.c_str());

		strFileInfoFiledArr.Add("音频编码");
		strFileInfoValueArr.Add(m_StreamInfo.AudioInfo.strAudioCodecFormat.c_str());
		strTemp.Format("%d",m_StreamInfo.AudioInfo.iAudioSamplingRate);
		strFileInfoFiledArr.Add("音频采样率");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.AudioInfo.lAudioQuantizationBits);
		strFileInfoFiledArr.Add("音频量化");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.AudioInfo.lChannels);
		strFileInfoFiledArr.Add("通道");
		strFileInfoValueArr.Add(strTemp);	
	}
	
	return TRUE;
}

void CFileDriver_M2V::SetServiceName(CString strServiceNameOp)
{
	m_strServiceNameOp = strServiceNameOp;
}

void CFileDriver_M2V::Submit()
{
  	//m_pAutoDetect->Initialize();//wj 20081008如果m_pAutoDetect只执行一条素材那么不用调用这个函数，不然就又读取了默认参数配置
	m_pAutoDetect->SetCallback(this);
	CWinThread* pThread = NULL;
	pThread = AfxBeginThread(TskCheckThread,this);
	if (pThread)
	{
		m_hTaskThread = pThread->m_hThread;
	}
	m_bSubmit = TRUE;
	SetTaskStatus(TSK_IS_RUNING);
	LPCTSTR m_str = m_strFilePath;
	m_DriverMgr_M2V.OpenFile(m_str);
	m_DriverMgr_M2V.StartInfoCheckThread();

	while(WaitForSingleObject(m_evFileInfoComplete,50) == WAIT_TIMEOUT)
	{
		if (m_uReturnType == DR_ERROR_ABORT )
		{
			return ;
		}
	}

	m_DriverMgr_M2V.StartDecodeThread();
	m_DriverMgr_M2V.Run();
}

UINT CFileDriver_M2V::TskCheckThread(LPVOID pParam)
{
	MyMediaSample* pMediaSample = NULL;
	CFileDriver_M2V* pParent = (CFileDriver_M2V*)pParam;
	pParent->m_startTime = timeGetTime();
	while (TRUE)
	{
		if (WaitForSingleObject(pParent->m_ThreadEvent,INFINITE) == WAIT_OBJECT_0)
		{
			pMediaSample = (pParent->m_SampleList).GetFullObject();
			if (pMediaSample)
			{
				pParent->m_pAutoDetect->Detect(pMediaSample->GetData(),pMediaSample->GetLen());
				pParent->m_SampleList.GetFullObjectComplete();
				pParent->OnUpdateProgress();
				if((GetFCVSServerConfig().m_bUseSectionFunction)&& (pParent->isSectionEnd()))
				{
					pParent->CancelTask();
					pParent->m_uReturnType = DR_NOMAL_FINISHED;
					break;
				}
			}
			else
			{
				if (pParent->m_uReturnType > DR_NOT_RETURN && pParent->m_uReturnType != DR_RUN)
				{
					break;
				}
				Sleep(5);
			}
		}
	}
	if (pParent->m_uReturnType == DR_NOMAL_FINISHED)
	{
		//pParent->SetTaskStatus(TSK_IS_FINISHED);
		//wangjun 20081019
		//如果素材不是解码帧数与文件长度不符，则认为是异常结束
		
		//  [12/14/2008 wang.zh] add ,是否进行文件长度判断。
		if (GetFCVSServerConfig().m_bCheckDuration) 
		{	
			//wangjun 20081019		
			//如果素材不是解码帧数与文件长度不符，则认为是异常结束
			if (pParent->nCount == pParent->m_StreamInfo.llFileDuration)
			{
				pParent->SetTaskStatus(TSK_IS_FINISHED);
				//pParent->ReportLog(TSK_IS_FINISHED); //wang.zh add
			}
			else if((GetFCVSServerConfig().m_bUseSectionFunction)&& (pParent->nCount >= pParent->m_TaskDuration))
			{
				pParent->SetTaskStatus(TSK_IS_FINISHED);			
			}
			else
			{
				pParent->SetTaskStatus(TSK_IS_ABORTED);
				CString strLog = "";
				strLog.Format("strTaskID = %s,ConText = %s(%s)",pParent->m_strTskID,"任务状态","检测帧数不完全");
//	            glReportLog("Log_Task",strLog);
			}	
		}
		else
		{
			pParent->SetTaskStatus(TSK_IS_FINISHED);
			//pParent->ReportLog(TSK_IS_FINISHED); // wang.zh add
		}
		//return 0;
	}
	if (pParent->m_uReturnType == DR_USER_CANCEL)
	{
		pParent->SetTaskStatus(TSK_IS_CANNELED);
		//return 0;
	}
	if (pParent->m_uReturnType == DR_ERROR_ABORT)
	{
		pParent->SetTaskStatus(TSK_IS_ABORTED);
		//return 0;
	}

	SetEvent(pParent->m_evThreadExit);
	return 0;
}