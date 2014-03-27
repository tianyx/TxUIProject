// FileDriver_Omn.cpp: implementation of the CFileDriver_Omn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FCVSServer.h"
#include "FileDriver_Omn.h"
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
int CFileDriver_Omn::nNum = 0;
REGISTER_CLASS(CFileDriver_Omn)
CFileDriver_Omn::CFileDriver_Omn() : CFileDriver(&m_DriverMgr_MXF)
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
//	m_DriverMgr_MXF.SetCallBack((JS_CCallBack*)this);
//	m_DriverMgr_MXF.SetBuffer((void*)&m_SampleList);
//	m_DriverMgr_MXF.SetParent(m_hWnd);
}

CFileDriver_Omn::~CFileDriver_Omn()
{
	OutputDebugString("into CFileDriver_Omn::~CFileDriver_Omn()\n");
}

void CFileDriver_Omn::ShowMsg()
{
	AfxMessageBox(m_strMsg);
}

BOOL CFileDriver_Omn::InitTask(CString strXML)
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
	XmlOpReader.GetAttribute("SaveXmlPath", m_strSaveXmlPath, strErr, "TaskData");
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
    CString strDriverPara   = GetFCVSServerConfig().GetXmlByClassName("CFileDriver_Omn");
	CXMLOp XmlClipTypePara;
	CXMLOp XmlDrivePara;
	if (strClipTypePara.IsEmpty() || strDriverPara.IsEmpty())
	{
		bInitSuccess = FALSE;
		AfxMessageBox("�޷��ҵ��������������޷���������!");
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
	m_DriverMgr_MXF.SetCallBack((JS_CCallBack*)this);
	m_DriverMgr_MXF.SetBuffer((void*)&m_SampleList);
	if(GetFCVSServerConfig().m_bCheckAudio)
	   m_DriverMgr_MXF.SetAudioBuffer((void*)&m_AudioSampleList);
	m_DriverMgr_MXF.SetParent(m_hWnd);
	///add end

	ReXmlOp.CleanXMLDocument();
	ReXmlOp.CreateOperationReRoot(strErr);
	MSXML2::IXMLDOMElementPtr Info = XmlOpReader.FindNodeByNodeName("TaskInfo");
	ReXmlOp.AppendNode(Info,strErr,"Root");
	ReXmlOp.CreateNode("TaskData",strErr,"Root");
	ReXmlOp.SetAttribute("TskID",m_strTskID,strErr,"TaskData");
    //wangjun 20081009��ӳ�ʼ������ʧ�ܴ���
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

BOOL CFileDriver_Omn::RunTask()
{
	BOOL bRet = m_DriverMgr_MXF.Run();
	if (bRet)
	{
		SetReturnType(DR_RUN);
		SetTaskStatus(TSK_IS_RUNING);
	}
	return bRet;
}

BOOL CFileDriver_Omn::PauseTask()
{
	BOOL bRet = m_DriverMgr_MXF.Pause();
	if (bRet)
	{
		SetReturnType(DR_PAUSE);
		SetTaskStatus(TSK_IS_PAUSED);
	}
	return bRet;
}

BOOL CFileDriver_Omn::CancelTask()
{
	return m_DriverMgr_MXF.Cancel();
}

CString CFileDriver_Omn::GetTskID()
{
	return m_strTskID;
}

void CFileDriver_Omn::SetParentWnd(HWND hwnd)
{
	m_hParentWnd = hwnd;
}

HRESULT CFileDriver_Omn::GetFileInfo(JS_StreamInfo p)
{
	m_StreamInfo = p;
	OnGetFileInfoFinished();

	SetEvent(m_evFileInfoComplete);
	return 1L;
}

LONGLONG CFileDriver_Omn::GetFileDuration()
{
	return m_StreamInfo.llFileDuration;
}

BOOL CFileDriver_Omn::GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML /* = TRUE */)
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
		//{added by tianyx  13:6:2013   15:09
		strFileInfoFiledArr.Add("AFD");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strAFDInfo.c_str());
		//}end added by tianyx

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
		strFileInfoFiledArr.Add("����ģʽ(OP)");
		strFileInfoValueArr.Add(m_StreamInfo.strOP.c_str());
		strTemp.Format("%ld",m_StreamInfo.llFileDuration);
		strFileInfoFiledArr.Add("�ļ�����(֡)");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("ͼ�����");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strVideoCodecFormat.c_str());
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iVideoHeight);
		strFileInfoFiledArr.Add("ͼ���");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iVideowidth);
		strFileInfoFiledArr.Add("ͼ���");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("�߿��");
		strTemp = m_StreamInfo.VideoInfo.strAspectRatio.c_str();
		strTemp.Replace("/",":");
		strFileInfoValueArr.Add(strTemp);
		//{added by tianyx  13:6:2013   15:09
		strFileInfoFiledArr.Add("AFD��Ϣ");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strAFDInfo.c_str());
		//}end added by tianyx

		strFileInfoFiledArr.Add("֡��");
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iFrameRate);
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.VideoInfo.lVideoBitRate);
		strFileInfoFiledArr.Add("������");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%d",m_StreamInfo.VideoInfo.iMaxGOP);
		strFileInfoFiledArr.Add("���GOP");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.VideoInfo.lvbvBufferSiae);
		strFileInfoFiledArr.Add("VBV����");
		strFileInfoValueArr.Add(strTemp);
		strFileInfoFiledArr.Add("��ɫ�ռ�");
		strFileInfoValueArr.Add(m_StreamInfo.VideoInfo.strColorSpace.c_str());

		strFileInfoFiledArr.Add("��Ƶ����");
		strFileInfoValueArr.Add(m_StreamInfo.AudioInfo.strAudioCodecFormat.c_str());
		strTemp.Format("%d",m_StreamInfo.AudioInfo.iAudioSamplingRate);
		strFileInfoFiledArr.Add("��Ƶ������");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.AudioInfo.lAudioQuantizationBits);
		strFileInfoFiledArr.Add("��Ƶ����");
		strFileInfoValueArr.Add(strTemp);
		strTemp.Format("%ld",m_StreamInfo.AudioInfo.lChannels);
		strFileInfoFiledArr.Add("ͨ��");
		strFileInfoValueArr.Add(strTemp);	
	}
	return TRUE;
}

void CFileDriver_Omn::Submit()
{
	//m_pAutoDetect->Initialize();//wj 20081008���m_pAutoDetectִֻ��һ���ز���ô���õ��������������Ȼ���ֶ�ȡ��Ĭ�ϲ�������
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
	m_DriverMgr_MXF.OpenFile(m_str);
	m_DriverMgr_MXF.StartInfoCheckThread();
	
	while(WaitForSingleObject(m_evFileInfoComplete,50) == WAIT_TIMEOUT)
	{
		if (m_uReturnType == DR_ERROR_ABORT )
		{
			return ;
		}
	}

	int nBlockAlign = m_StreamInfo.AudioInfo.lChannels*(m_StreamInfo.AudioInfo.lAudioQuantizationBits/8);
	m_pAutoDetect->SetAudioType(m_StreamInfo.AudioInfo.lChannels,m_StreamInfo.AudioInfo.iAudioSamplingRate,m_StreamInfo.AudioInfo.lAudioQuantizationBits,nBlockAlign);
	
	m_DriverMgr_MXF.StartDecodeThread();
	m_DriverMgr_MXF.Run();
}

void CFileDriver_Omn::SetServiceName(CString strServiceNameOp)
{
	m_strServiceNameOp = strServiceNameOp;
}

UINT CFileDriver_Omn::TskCheckThread(LPVOID pParam)
{
	MyMediaSample* pMediaSample = NULL;
	CFileDriver_Omn* pParent = (CFileDriver_Omn*)pParam;
	pParent->m_startTime = timeGetTime();
	//while (TRUE)
	{
		long iRetry = 0;
		while (WaitForSingleObject(pParent->m_ThreadEvent,INFINITE) == WAIT_OBJECT_0)
		{
			pMediaSample = (pParent->m_SampleList).GetFullObject();
			if (pMediaSample)
			{
				pParent->m_pAutoDetect->Detect(pMediaSample->GetData(),pMediaSample->GetLen());
				pParent->m_SampleList.GetFullObjectComplete();
				pParent->OnUpdateProgress();
				iRetry = 0;
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
				Sleep(200);
				OutputDebugString("��ȡ����Ϊ��!");
				iRetry++;
				if((iRetry > 5000) &&(!GetFCVSServerConfig().m_bUseSectionFunction))
				{
					pParent->m_DriverMgr_MXF.Cancel();
					OutputDebugString("�˳�ѭ��!");
					pParent->m_uReturnType = DR_USER_CANCEL;
					break;
				}
			}
		}
	}

	OutputDebugString("End1");
	if (pParent->m_uReturnType == DR_NOMAL_FINISHED)
	{
		OutputDebugString("myOut123: Detect Finished");
		//  [12/14/2008 wang.zh] add ,�Ƿ�����ļ������жϡ�
		if (GetFCVSServerConfig().m_bCheckDuration) 
		{	
			//����زĲ��ǽ���֡�����ļ����Ȳ���������Ϊ���쳣����
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
				strLog.Format("strTaskID = %s,ConText = %s(%s)",pParent->m_strTskID,"����״̬","���֡������ȫ");
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

UINT CFileDriver_Omn::TskAudioCheckThread(LPVOID pParam)
{
	MyMediaSample* pMediaSample = NULL;
	CFileDriver_Omn* pParent = (CFileDriver_Omn*)pParam;
	
	while (WaitForSingleObject(pParent->m_evAThreadEvent,INFINITE) == WAIT_OBJECT_0)
	//while (true)
	{
		long iRetry = 0;

		pMediaSample = (pParent->m_AudioSampleList).GetFullObject();
		if (pMediaSample)
		{
			pParent->m_pAutoDetect->DetectAudio(pMediaSample->GetData(),pMediaSample->GetLen());
			pParent->m_AudioSampleList.GetFullObjectComplete();

			iRetry = 0;
		}
		else
		{
			if (pParent->m_uReturnType > DR_NOT_RETURN && pParent->m_uReturnType != DR_RUN)
			{
				pParent->m_pAutoDetect->DetectAudio(NULL,NULL);
				break;
			}
			Sleep(5);

			iRetry++;
			if(iRetry > 5000)
			{
				OutputDebugString("�˳�ѭ��!");
				break;
			} 
		}
	}
	
	if (pParent->m_uReturnType == DR_NOMAL_FINISHED)
	{
		ResetEvent(pParent->m_evAThreadEvent);
	}
	
	SetEvent(pParent->m_evAThreadExit);
	return 0;
}