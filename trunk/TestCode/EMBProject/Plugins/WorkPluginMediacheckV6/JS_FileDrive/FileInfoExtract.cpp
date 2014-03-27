#include "StdAfx.h"
#include "FileInfoExtract.h"

CFileInfoExtract::CFileInfoExtract(void)
{
	//memset(m_strFileName,0,MAX_FILEPATH);
	memset(&m_VideoInfo,0,sizeof(m_VideoInfo));
	memset(&m_AudioInfo,0,sizeof(m_AudioInfo));
	memset(&m_StreamInfo,0,sizeof(m_StreamInfo));
	m_strFileName = "";
	m_hFile       = NULL;
	m_bAnalyzing  = 0;
}

CFileInfoExtract::~CFileInfoExtract(void)
{
	while (m_bAnalyzing)
	{
		Sleep(500);
	}
}

BOOL CFileInfoExtract::OpenFile(LPCTSTR lpFileName)
{
	if(lpFileName)
	{
		//strcpy(m_strFileName,lpFileName);
		m_strFileName = (LPSTR)lpFileName;
		return true;
	}
	return false;
}

BOOL CFileInfoExtract::Read(__int64 llOffset, long cBytes, BYTE *pBuffer)
{
	if(m_hFile)
	{
		DWORD Num = 0;
		long hWord = llOffset>>32;
		long LWord = (long)llOffset;
	    if(0xFFFFFFFF == SetFilePointer(m_hFile,LWord,&hWord,FILE_BEGIN))
			return false;
        return ReadFile(m_hFile,pBuffer,cBytes,&Num,NULL);
	}
	return false;
}
BOOL CFileInfoExtract::Analyze()
{
	m_bAnalyzing = 1;
    if(AnalyzeFile())
	{
       m_bAnalyzing = 0;
	   return true;
	}
	else
	{
		m_bAnalyzing = 0;
	    return false;
	}
}

//�麯��
//��ͬ��ý�������ļ��������в�ͬ�ķ��������������������ʵ���ļ���Ϣ����
BOOL CFileInfoExtract::AnalyzeFile()
{
	return true;
}

BOOL CFileInfoExtract::CloseFile()
{
	return 0;
}

//������Ƶ��Ϣ
JS_VideoInfo CFileInfoExtract::GetVideoInfo()
{
	return 	m_VideoInfo;
}

//������Ƶ��Ϣ
JS_AudioInfo CFileInfoExtract::GetAudioInfo()
{
	return m_AudioInfo;
}

//��������Ϣ
JS_StreamInfo CFileInfoExtract::GetStreamInfo()
{
	return m_StreamInfo;
}



CString CFileInfoExtract::GetFileInfoLastError()
{
	CString strErrors = m_strError.c_str();
    return strErrors;
}
