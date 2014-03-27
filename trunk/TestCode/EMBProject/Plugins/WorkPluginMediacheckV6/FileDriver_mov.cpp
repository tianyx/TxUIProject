// FileDriver_MOV.cpp: implementation of the CFileDriver_MOV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FCVSServer.h"
#include "FileDriver_MOV.h"
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
int CFileDriver_MOV::nNum = 0;
REGISTER_CLASS(CFileDriver_MOV)
CFileDriver_MOV::CFileDriver_MOV() : CFileDriver(&m_DriverMgr_MOV)
{
	m_strTskID = "";
	m_strCheckKind = "";
	m_strFileType = "";
	m_strClipID = "";
	m_strClipName = "";
	m_strFilePath = "";
	m_strCheckItem = "";
	m_nPRI = 0;

	m_pAutoDetect = AutoDetect::CreateInstance();
//	CString strConfig = (LPCSTR)GetFCVSServerConfig().GetDetectParaXml(m_strClipType);
//	m_pAutoDetect->XMLConfig(strConfig);
//	m_DriverMgr_MOV.SetCallBack((JS_CCallBack*)this);
//	m_DriverMgr_MOV.SetBuffer((void*)&m_SampleList);
//	m_DriverMgr_MOV.SetParent(m_hWnd);
}

CFileDriver_MOV::~CFileDriver_MOV()
{
	OutputDebugString("into CFileDriver_MOV::~CFileDriver_MOV()\n");
}

void CFileDriver_MOV::ShowMsg()
{
	AfxMessageBox(m_strMsg);
}

BOOL CFileDriver_MOV::InitTask(CString strXML)
{
	if (strXML.IsEmpty())
	{
		return FALSE;
	}
	CString strErr = "";
	CXMLOp XmlOpReader;
	CXMLOp ReXmlOp;
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
    CString strDriverPara   = GetFCVSServerConfig().GetXmlByClassName("CFileDriver_MOV");
	CXMLOp XmlClipTypePara;
	CXMLOp XmlDrivePara;
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
	m_DriverMgr_MOV.SetCallBack((JS_CCallBack*)this);
	m_DriverMgr_MOV.SetBuffer((void*)&m_SampleList);
	if(GetFCVSServerConfig().m_bCheckAudio)
	   m_DriverMgr_MOV.SetAudioBuffer((void*)&m_AudioSampleList);
	m_DriverMgr_MOV.SetParent(m_hWnd);
	///add end

	ReXmlOp.CleanXMLDocument();
	ReXmlOp.CreateOperationReRoot(strErr);
	MSXML2::IXMLDOMElementPtr Info = XmlOpReader.FindNodeByNodeName("TaskInfo");
	ReXmlOp.AppendNode(Info,strErr,"Root");
	ReXmlOp.CreateNode("TaskData",strErr,"Root");
	ReXmlOp.SetAttribute("TskID",m_strTskID,strErr,"TaskData");
    //wangjun 20081009添加初始化参数失败处理
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
	m_AudioSampleList.ReSet();
	nCount = 0;
	return TRUE;
}

BOOL CFileDriver_MOV::RunTask()
{
	BOOL bRet = m_DriverMgr_MOV.Run();
	if (bRet)
	{
		SetReturnType(DR_RUN);
		SetTaskStatus(TSK_IS_RUNING);
	}
	return bRet;
}

BOOL CFileDriver_MOV::PauseTask()
{
	BOOL bRet = m_DriverMgr_MOV.Pause();
	if (bRet)
	{
		SetReturnType(DR_PAUSE);
		SetTaskStatus(TSK_IS_PAUSED);
	}
	return bRet;
}

BOOL CFileDriver_MOV::CancelTask()
{
	return m_DriverMgr_MOV.Cancel();
}

CString CFileDriver_MOV::GetTskID()
{
	return m_strTskID;
}

void CFileDriver_MOV::SetParentWnd(HWND hwnd)
{
	m_hParentWnd = hwnd;
}

HRESULT CFileDriver_MOV::GetFileInfo(JS_StreamInfo p)
{
	m_StreamInfo = p;
	OnGetFileInfoFinished();

	SetEvent(m_evFileInfoComplete);
	return 1L;
}

LONGLONG CFileDriver_MOV::GetFileDuration()
{
	return m_StreamInfo.llFileDuration;
}

BOOL CFileDriver_MOV::GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML /* = TRUE */)
{
	CString strTemp;
	if (bXML)
	{
		strFileInfoFiledArr.Add("strOP");
		strFileInfoValueArr.Add(m_StreamInfo.strOP.c_str());
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
		strFileInfoFiledArr.Add("操作模式(OP)");
		strFileInfoValueArr.Add(m_StreamInfo.strOP.c_str());
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

void CFileDriver_MOV::Submit()
{
	//m_pAutoDetect->Initialize();//wj 20081008如果m_pAutoDetect只执行一条素材那么不用调用这个函数，不然就又读取了默认参数配置
	m_pAutoDetect->SetCallback(this);

	
	CWinThread* pThread = NULL;
	pThread = AfxBeginThread(TskCheckThread,this);
	if (pThread)
	{
		m_hTaskThread = pThread->m_hThread;
	}
    
	if (GetFCVSServerConfig().m_bCheckAudio)
	{
		pThread = AfxBeginThread(TskAudioCheckThread,this);
		if (pThread)
		{
			m_hAThread = pThread->m_hThread;
		}
	}

	m_bSubmit = TRUE;
	SetTaskStatus(TSK_IS_RUNING);
	LPCTSTR m_str = m_strFilePath;
	m_DriverMgr_MOV.OpenFile(m_str);
	m_DriverMgr_MOV.StartInfoCheckThread();

	while(WaitForSingleObject(m_evFileInfoComplete,50) == WAIT_TIMEOUT)
	{
		if (m_uReturnType == DR_ERROR_ABORT )
		{
			return ;
		}
	}

	int nBlockAlign = m_StreamInfo.AudioInfo.lChannels*(m_StreamInfo.AudioInfo.lAudioQuantizationBits/8);
	m_pAutoDetect->SetAudioType(m_StreamInfo.AudioInfo.lChannels,m_StreamInfo.AudioInfo.iAudioSamplingRate,m_StreamInfo.AudioInfo.lAudioQuantizationBits,nBlockAlign);
	
	m_DriverMgr_MOV.StartDecodeThread();
	m_DriverMgr_MOV.Run();
}

void CFileDriver_MOV::SetServiceName(CString strServiceNameOp)
{
	m_strServiceNameOp = strServiceNameOp;
}

UINT CFileDriver_MOV::TskCheckThread(LPVOID pParam)
{
	MyMediaSample* pMediaSample = NULL;
	CFileDriver_MOV* pParent = (CFileDriver_MOV*)pParam;
	pParent->m_startTime = timeGetTime();
	//while (TRUE)
	{
		while (WaitForSingleObject(pParent->m_ThreadEvent,INFINITE) == WAIT_OBJECT_0)
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
		OutputDebugString("myOut123: Detect Finished");
		//  [12/14/2008 wang.zh] add ,是否进行文件长度判断。
		if (GetFCVSServerConfig().m_bCheckDuration) 
		{	
			//如果素材不是解码帧数与文件长度不符，则认为是异常结束
		    if (pParent->nCount == pParent->m_StreamInfo.llFileDuration)
			{
				pParent->SetTaskStatus(TSK_IS_FINISHED);
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
		}

		ResetEvent(pParent->m_ThreadEvent);

	}
	if (pParent->m_uReturnType == DR_USER_CANCEL)
	{
		pParent->SetTaskStatus(TSK_IS_CANNELED);
	}
	if (pParent->m_uReturnType == DR_ERROR_ABORT)
	{
		pParent->SetTaskStatus(TSK_IS_ABORTED);
	}

	SetEvent(pParent->m_evThreadExit);

	return 0;
}

UINT CFileDriver_MOV::TskAudioCheckThread(LPVOID pParam)
{
	MyMediaSample* pMediaSample = NULL;
	CFileDriver_MOV* pParent = (CFileDriver_MOV*)pParam;
	
	while (WaitForSingleObject(pParent->m_evAThreadEvent,INFINITE) == WAIT_OBJECT_0)
	//while (true)
	{
		pMediaSample = (pParent->m_AudioSampleList).GetFullObject();
		if (pMediaSample)
		{
			pParent->m_pAutoDetect->DetectAudio(pMediaSample->GetData(),pMediaSample->GetLen());
			pParent->m_AudioSampleList.GetFullObjectComplete();
		}
		else
		{
			if (pParent->m_uReturnType > DR_NOT_RETURN && pParent->m_uReturnType != DR_RUN)
			{
				pParent->m_pAutoDetect->DetectAudio(NULL,NULL);
				break;
			}
			Sleep(5);
		}
	}
	
	if (pParent->m_uReturnType == DR_NOMAL_FINISHED)
	{
		ResetEvent(pParent->m_evAThreadEvent);
	}
	
	SetEvent(pParent->m_evAThreadExit);
	return 0;
}