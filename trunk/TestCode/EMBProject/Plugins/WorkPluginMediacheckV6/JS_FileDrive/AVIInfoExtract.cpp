// AVIInfoExtract.cpp: implementation of the CAVIInfoExtract class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AVIInfoExtract.h"
#include "SequenceHeader.h"
#include <vfw.h>
#include "File64.h"

#define MB_CHANGE_NUM 1000000
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAVIInfoExtract::CAVIInfoExtract()
{

}

CAVIInfoExtract::~CAVIInfoExtract()
{

}

BOOL CAVIInfoExtract::AnalyzeFile()
{
	if (m_strFileName != "")
	{
		//得到AVI文件大小
		LONGLONG fileSize = 0;
		CFile64 file;
		
		if(file.Open(m_strFileName.c_str(),CFile64::shareDenyNone,NULL) != NULL)
		{
			fileSize = file.GetLength();
			file.Close();
		}
		
		int nVideoNum = 0;
		int nAudioNum = 0;
		PAVIFILE pAviFile = NULL;
		AVIFILEINFO AviFileInfo;
		AVIFileInit();
		if(S_OK == AVIFileOpen(&pAviFile,m_strFileName.c_str(),OF_READ|OF_SHARE_DENY_READ,NULL))
		{
			AVIFileInfo(pAviFile,&AviFileInfo,sizeof(AVIFILEINFO));
			m_StreamInfo.llFileDuration = AviFileInfo.dwLength;
			m_StreamInfo.strOP = "";
		    PAVISTREAM pAviStream;
		    AVISTREAMINFO AviStreamInfo;
		    int nStreams = AviFileInfo.dwStreams;
		    for (int i = 0 ; i < nStreams ; i ++)
			{
			   if(S_OK == AVIFileGetStream(pAviFile,&pAviStream,NULL,i))
			   {
				   AVIStreamInfo(pAviStream,&AviStreamInfo,sizeof(AVISTREAMINFO));
				   if (AviStreamInfo.fccType == streamtypeVIDEO)
				   {
					   nVideoNum ++;
					   if (nVideoNum > 1)
					   {
						   continue;
					   }
					   m_StreamInfo.llFileDuration = AviStreamInfo.dwLength;
					   m_VideoInfo.iFrameRate = AviStreamInfo.dwRate / AviStreamInfo.dwScale;
					   m_VideoInfo.iMaxGOP = AviStreamInfo.dwSampleSize;
					   m_VideoInfo.iVideoHeight = AviStreamInfo.rcFrame.bottom - AviStreamInfo.rcFrame.top;
					   m_VideoInfo.iVideowidth = AviStreamInfo.rcFrame.right - AviStreamInfo.rcFrame.left;
					   m_VideoInfo.lvbvBufferSiae = AviStreamInfo.dwSuggestedBufferSize;

					   //zhou change 2009.4.16 begin
					   double nTmpTime = (float)AviStreamInfo.dwLength/m_VideoInfo.iFrameRate;
					   m_VideoInfo.lVideoBitRate =(long)((fileSize * 8)/nTmpTime);//AviStreamInfo.dwScale;
					   long nMBRate    = m_VideoInfo.lVideoBitRate / MB_CHANGE_NUM;
					   long nAdjustNum = m_VideoInfo.lVideoBitRate % MB_CHANGE_NUM;
					   if(nAdjustNum > 500000)
						   m_VideoInfo.lVideoBitRate = (nMBRate + 1) * MB_CHANGE_NUM;
					   else
						   m_VideoInfo.lVideoBitRate = nMBRate * MB_CHANGE_NUM;
					   //zhou 2009.4.16 end

					   m_VideoInfo.strAspectRatio = "";
					   m_VideoInfo.strColorSpace = "";
					   DWORD dwHandler = AviStreamInfo.fccHandler;
					   char k = (char)dwHandler;
					   dwHandler = dwHandler>>8;
					   char k1 = (char)dwHandler;
					   dwHandler = dwHandler>>8;
					   char k2 = (char)dwHandler;
					   dwHandler = dwHandler>>8;
					   char k3 = (char)dwHandler;
					   CString strTemp = "";
					   strTemp.Format("%c%c%c%c",k,k1,k2,k3);
					   m_VideoInfo.strVideoCodecFormat = strTemp;
				   }
				   if (AviStreamInfo.fccType == streamtypeAUDIO)
				   {
					   nAudioNum ++;
					   if (nAudioNum > 0)
					   {
						   continue;
					   }
					   WAVEFORMAT*   pSrcWaveInfo   =   NULL; 
					   LONG lStreamSize = 0;
					   AVIStreamFormatSize(pAviStream,0,&lStreamSize);         
					   pSrcWaveInfo   =   (WAVEFORMAT*)new   char[lStreamSize];         
					   AVIStreamReadFormat(pAviStream,0,pSrcWaveInfo,&lStreamSize);  
					   if (pSrcWaveInfo->wFormatTag == WAVE_FORMAT_PCM)
					   {
							m_AudioInfo.strAudioCodecFormat = "PCMAudio";
					   }
					   m_AudioInfo.iAudioSamplingRate = pSrcWaveInfo->nSamplesPerSec;
					   m_AudioInfo.lAudioQuantizationBits = (pSrcWaveInfo->nBlockAlign / pSrcWaveInfo->nChannels) * 8;
					   m_AudioInfo.lChannels = pSrcWaveInfo->nChannels;
				   }
			   }
			   AVIStreamRelease(pAviStream);
			} 
			AVIFileRelease(pAviFile);
			AVIFileExit();
			if(m_VideoInfo.iVideowidth == 720 && m_VideoInfo.iVideoHeight == 576)
			{
				m_VideoInfo.strAspectRatio = "4:3";
			}
			
			m_StreamInfo.VideoInfo = m_VideoInfo;
			m_StreamInfo.AudioInfo = m_AudioInfo;
		}
		else
		{
			return FALSE;
		}
	}
	return TRUE;
}

