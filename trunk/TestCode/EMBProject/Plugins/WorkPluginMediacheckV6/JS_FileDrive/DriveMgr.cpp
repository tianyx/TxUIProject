// DriveMgr.cpp: implementation of the CDrivemgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr.h"
#include "TxLogManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDriveMgr::CDriveMgr()
{
	m_ParentHwnd   = NULL;
    m_pBasefInfoEt = NULL;
	m_pBaseGraph   = NULL;
	m_lpCallFunAdr = NULL;  
	m_strFilePath  = ""; 
	m_ThreadHandle[0] = 0;
	m_ThreadHandle[1] = 0;

	m_dwRetryTimes = 0;
	m_dwInterVal = 0;

   CoInitialize(NULL);
}

CDriveMgr::CDriveMgr(HWND ParentHwnd = NULL,CBaseGraph *pbasegraph = NULL,CFileInfoExtract *pfileIE =NULL)
{
	OutputDebugString("WWW: CDriveMgr::CDriveMgr()");
	CoInitialize(NULL);
	InitializeCriticalSection(&m_CritSect);
/*	if(ParentHwnd || pbasegraph || pfileIE)
	{
       NOTE("Error:failed to create CDriveMgr!");
	   return ;
	}
	*/
    m_ParentHwnd   = ParentHwnd;
    m_pBasefInfoEt = pfileIE;
	m_pBaseGraph   = pbasegraph;
	m_lpCallFunAdr = NULL;  
	m_strFilePath  = ""; 
	m_ThreadHandle[0] = 0;
	m_ThreadHandle[1] = 0;

	m_dwRetryTimes = 0;
	m_dwInterVal = 0;

}

CDriveMgr::~CDriveMgr()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_ThreadHandle[i] != 0)
		{
			EXECUTE_ASSERT(WAIT_OBJECT_0 == ::WaitForSingleObject(m_ThreadHandle[i], 10000));
			CloseHandle(m_ThreadHandle[i]);
			m_ThreadHandle[i] = 0;
		}
	}

	::DeleteCriticalSection(&m_CritSect);
	OutputDebugString("WWW: CDriveMgr::~CDriveMgr()");
	CoUninitialize();
}

HANDLE  CDriveMgr::StartInfoCheckThread()
{
	if (m_ThreadHandle[0] != 0)
	{
		CloseHandle(m_ThreadHandle[0]);
		m_ThreadHandle[0] = 0;
	}
    m_ThreadHandle[0] = CreateThread(InfoCheckThread,&m_ThreadID[0]);
	return m_ThreadHandle[0];
}

HANDLE CDriveMgr::StartDecodeThread()
{
// 	m_ThreadHandle[1] = CreateThread(DecoderThread,&m_ThreadID[1]);
//     return m_ThreadHandle[1];

	HRESULT hr = S_FALSE;
//	CDriveMgr *T = (CDriveMgr*)lpParameter;
	hr = m_pBaseGraph->RenderFile();
	if (FAILED(hr))
	{
		if (m_ParentHwnd != NULL)
		{
		::SendMessage(m_ParentHwnd,WM_DS_EC_USE_UNRENDER,0,0);
		}		
		m_pBaseGraph->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_USE_UNRENDER,0,0);
	}
	return 0;	
	
}
//���ûص��������ļ���Ϣ�������Ժ�֪ͨ���÷����Զ�ȡ�ļ���Ϣ��
HRESULT CDriveMgr::SetCallBack(JS_CCallBack *lpCallFun )
{
	if(lpCallFun)
	{
		//m_lpCallFunAdr = lpCallFun;
		m_pBaseGraph->m_pMsgCallBack = m_lpCallFunAdr = lpCallFun;//chenAdd for Detect
		return 1;
	}
    return 0;
}
//���ý��뻺��
//pA[in]: �����ץ����ÿ֡���������Ķ���ָ��
HRESULT  CDriveMgr::SetBuffer(void *pA)
{
    return m_pBaseGraph->SetBuffer(pA);
	
}
HRESULT  CDriveMgr::SetAudioBuffer(void *pA)
{
    return m_pBaseGraph->SetAudioBuffer(pA);
	
}
//����һ�����߳�
//lpStartAddress[in]:�̺߳�����ַ
//ThreadID      [in]:�������
HANDLE CDriveMgr::CreateThread(LPTHREAD_START_ROUTINE lpStartAddress , LPDWORD ThreadID)
{
	HANDLE TreadHandle;
	TreadHandle=::CreateThread(NULL,0,lpStartAddress,this,NULL,ThreadID);
	 if (!TreadHandle)
	 {
		 MessageBox(NULL,"�����߳�ʧ��!","CDriveMgr",MB_OK);
		 return TreadHandle;
	 }
     return TreadHandle;
}

//�����ļ���Ϣ����߳�
DWORD __stdcall CDriveMgr::InfoCheckThread(LPVOID lpParameter)
{
	TRACE("<<<Into InfoCheckThread!\n");
	CDriveMgr *T = (CDriveMgr*)lpParameter;
	//�����ٽ�������ֹ����߳�ͬʱ��һ��m_pBasefInfoEtʵ����ͬһ�ֲ��������Ĵ���
	EnterCriticalSection(&T->m_CritSect);
	

    if(T->m_pBasefInfoEt->Analyze())
	{
	    T->m_StreamInfo=T->m_pBasefInfoEt->GetStreamInfo();
	    T->m_lpCallFunAdr->GetFileInfo(T->m_StreamInfo);
	}
	else
	{
		int nErrorResult = 0;
		CString strError = T->m_pBasefInfoEt->GetFileInfoLastError();
		if(strError == "File not exist!")
			nErrorResult = 1;
		else if(strError == "Analyze file catch error!")
			nErrorResult = 2;
		else if(strError == "Couldn't load the file")
			nErrorResult = 3;
		else if(strError == "get_OutputStreams")
			nErrorResult = 4;
		//::SendMessage(T->m_ParentHwnd,WM_IC_UNRECOG,0,0);
		OutputDebugString(strError);
		GetTxLogMgr()->WriteLog(1,strError);
		T->m_pBaseGraph->m_pMsgCallBack->OnDecodeMessage(WM_IC_UNRECOG,nErrorResult,0);
	}

	LeaveCriticalSection(&T->m_CritSect);
	TRACE(">>>Leave InfoCheckThread!\n");
	return 0;
}

//���������߳�
DWORD _stdcall CDriveMgr::DecoderThread(LPVOID lpParameter)
{
    TRACE("<<<Into DecoderThread!\n");
	HRESULT hr = E_FAIL;
	CDriveMgr *T = (CDriveMgr*)lpParameter;
	hr = T->m_pBaseGraph->RenderFile();
	if (FAILED(hr))
	{
		if (T->m_ParentHwnd != NULL)
		{
		::SendMessage(T->m_ParentHwnd,WM_DS_EC_USE_UNRENDER,0,0);
		}		
		T->m_lpCallFunAdr->OnDecodeMessage(WM_DS_EC_USE_UNRENDER,0,0);
	}
	TRACE(">>>Leave DecoderThread!\n");
	return 0;
		
}
//��ʼ��������
BOOL CDriveMgr::Run(void)
{
	if (!m_pBaseGraph)
	{
		return false;
	}
	HRESULT hr = S_FALSE;
	hr = m_pBaseGraph->Run();
	if(FAILED(hr))
		return false;
	return true;
}
//��ͣ��������
BOOL CDriveMgr::Pause(void)
{
	if (!m_pBaseGraph)
	{
		return false;
	}
	HRESULT hr = S_FALSE;
	hr = m_pBaseGraph->Pause();
	if(FAILED(hr))
		return false;
	return true;
}
//ֹͣ��������
BOOL CDriveMgr::Stop(void)
{
	if (!m_pBaseGraph)
	{
		return false;
	}
	HRESULT hr = S_FALSE;
	hr = m_pBaseGraph->Stop();
	if(FAILED(hr))
		return false;
	return true;
}
//ȡ����������
BOOL CDriveMgr::Cancel(void)
{
	if (!m_pBaseGraph)
	{
		return false;
	}
	HRESULT hr = S_FALSE;
	hr = m_pBaseGraph->Cancel();
	if(FAILED(hr))
		return false;
	return true;
}
//���ļ�
HRESULT CDriveMgr::OpenFile(LPCTSTR lpszPathName)
{
	HRESULT hr = S_FALSE;
	LPCTSTR ttt= lpszPathName;
    if (!lpszPathName)
    {
		return hr;
    }
	m_strFilePath = (LPSTR)lpszPathName;
//	CString strTemp = "";
//	strTemp.Format("WWW: CDriveMgr::OpenFile(LPCTSTR lpszPathName) %s",lpszPathName);
//	OutputDebugString(strTemp);
	hr=m_pBaseGraph->LoadFile(lpszPathName, m_dwRetryTimes, m_dwInterVal);
	if(FAILED(hr))
	{
		GetTxLogMgr()->WriteLog(1,"���ļ�ʧ��");
		return hr;
	}
	hr=m_pBasefInfoEt->OpenFile(lpszPathName);
	OutputDebugString("WWW: CDriveMgr::OpenFile(LPCTSTR lpszPathName) return");
	return hr;

}
//���� 
BOOL  CDriveMgr::SetParent(HWND ParentHwnd)
{
    if(ParentHwnd)
	{
		m_ParentHwnd = ParentHwnd;
		if(m_pBaseGraph->SetParent(m_ParentHwnd))
		   return true;
	}
	return false;
}
void CDriveMgr::SetRetryTimes(DWORD dwTimes,  DWORD dwInterVal)
{
	m_dwRetryTimes = dwTimes;
	m_dwInterVal = dwInterVal;
}
HRESULT CDriveMgr::GetVideoMediaType(AM_MEDIA_TYPE* mt)
{
	HRESULT hr = E_FAIL;
	if (m_pBaseGraph)
	{
		hr = m_pBaseGraph->GetVideoMediaType(mt);
	}
	return hr;
}
HRESULT CDriveMgr::GetAudioMediaType(AM_MEDIA_TYPE* mt)
{
   	HRESULT hr = E_FAIL;
	if (m_pBaseGraph)
	{
		hr = m_pBaseGraph->GetAudioMediaType(mt);
	}
	return hr;
}
void CDriveMgr::seekTo(LONGLONG som,LONGLONG duration)
{
	m_pBaseGraph->seekTo(som,duration);
}