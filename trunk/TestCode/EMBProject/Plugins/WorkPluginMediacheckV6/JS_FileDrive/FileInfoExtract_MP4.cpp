// FileInfoExtract_MP4.cpp: implementation of the CFileInfoExtract_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include <DVDMedia.h>
#include "FileInfoExtract_MP4.h"
#include "InfoFromDS.h"
#include "GuidRef.h"
#include "FileInfoExtract_MOV.h"

//#pragma comment(lib,"..\\lib\\Mudem.lib")
#pragma comment(lib,"Mudem.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInfoExtract_MP4::CFileInfoExtract_MP4()
{

}

CFileInfoExtract_MP4::~CFileInfoExtract_MP4()
{

}
BOOL CFileInfoExtract_MP4::AnalyzeFile()
{
	try
	{
		if(_access((LPCSTR)m_strFileName.c_str(),04) != 0)
		{
			m_strError = "File not exist!";
			return false;
		}

		CString strFileName = "file:";
		strFileName += m_strFileName.c_str();
		
		VideoInfo* pVideoInfo = 0;
		AudioInfo* pAudioInfo = 0;
		if( 0 < GetFileInfo((LPSTR)(LPCSTR)strFileName, &pVideoInfo, &pAudioInfo))
		{
			if( pVideoInfo )
			{
				m_VideoInfo.strVideoCodecFormat = pVideoInfo->m_szCodec;
				m_VideoInfo.iVideowidth =  pVideoInfo->m_nWidth;
				m_VideoInfo.iVideoHeight = pVideoInfo->m_nHeight;
				m_VideoInfo.iFrameRate   = pVideoInfo->m_nFrameRate;
				m_VideoInfo.lVideoBitRate = pVideoInfo->m_nBitrate;
				
				CString str;
				str.Format("%d:%d", pVideoInfo->m_nAspectX, pVideoInfo->m_nAspectY);
				m_VideoInfo.strAspectRatio = str;
				
				m_StreamInfo.llFileDuration = pVideoInfo->m_llDur;
				m_StreamInfo.VideoInfo = m_VideoInfo;
				
				::CoTaskMemFree(pVideoInfo);
			}
			
			if( pAudioInfo )
			{
				m_AudioInfo.strAudioCodecFormat = pAudioInfo->m_szCodec;
				m_AudioInfo.lChannels = pAudioInfo->m_nChannels;
				m_AudioInfo.iAudioSamplingRate = pAudioInfo->m_nSamplingRate;
				m_AudioInfo.lAudioQuantizationBits = pAudioInfo->m_nQuantizationBits;
				m_StreamInfo.AudioInfo = m_AudioInfo;
				
				::CoTaskMemFree(pAudioInfo);
			}
		}
		
// 		HRESULT hr = E_FAIL;
// 		CInfoFromDS_3GP GP3BaskGrahp;
// 		hr = GP3BaskGrahp.LoadFile(m_strFileName.c_str(),0,0);
// 		hr = GP3BaskGrahp.RenderFile();
// 		if(hr != S_OK)
// 		{
// 			OutputDebugString("”Ô∑®ºÏ≤‚£¨º”‘ÿÀÿ≤ƒ ß∞‹");
// 			return false;
// 		}
// 		
// 		LONGLONG nMinNum = GP3BaskGrahp.GetDuration();
// 		
// 		int Streams = GP3BaskGrahp.GetSrcStreamCount();
// 		for( int i = 0 ; i < Streams ; i++ )
// 		{
// 			AM_MEDIA_TYPE Type;
// 			GP3BaskGrahp.GetSrcStreamMedaiType(i,&Type);			
// 			double nDuration  = NULL;
// 			double nFrameRate = NULL;
// 			if(Type.majortype == MEDIATYPE_Video ) 
// 			{
// 				if (Type.subtype == MEDIASUBTYPE_VIDEO_H264)
// 				{
// 					m_VideoInfo.strVideoCodecFormat = "H264";
// 				}
// 				else if (Type.subtype == MEDIASUBTYPE_VIDEO_s263 
// 					|| Type.subtype == MEDIASUBTYPE_VIDEO_MC_s263 )
// 				{
//                     m_VideoInfo.strVideoCodecFormat = "H263";
// 				}
// 				if (Type.formattype == FORMAT_MPEG2_VIDEO)
// 				{
// 					if (Type.cbFormat >= sizeof(MPEG2VIDEOINFO))
// 					{
//                          MPEG2VIDEOINFO m2i;
// 						 memcpy((void*)&m2i,Type.pbFormat,sizeof(MPEG2VIDEOINFO));
// 						 m_VideoInfo.iVideowidth = m2i.hdr.rcSource.right;
// 						 m_VideoInfo.iVideoHeight = m2i.hdr.rcSource.bottom;
// 						 double t = UNITS;
// 						 t/=m2i.hdr.AvgTimePerFrame;
// 						 m_VideoInfo.iFrameRate   = int(t+0.5);
// 						m_VideoInfo.lVideoBitRate = m2i.hdr.dwBitRate;
// 					}
// 				}
// 				else if (Type.formattype == FORMAT_VideoInfo)
// 				{
// 					if (Type.cbFormat >= sizeof(VIDEOINFOHEADER))
// 					{
// 						VIDEOINFOHEADER v;
// 						memcpy((void*)&v,Type.pbFormat,sizeof(VIDEOINFOHEADER));
// 						m_VideoInfo.iVideowidth = v.rcSource.right;
// 						m_VideoInfo.iVideoHeight = v.rcSource.bottom;
// 						double t = UNITS;
// 						t /= v.AvgTimePerFrame;
// 						m_VideoInfo.iFrameRate   = int(t+0.5);
// 						m_VideoInfo.lVideoBitRate = v.dwBitRate;
// 					}
// 				}
// 				else if (Type.formattype == FORMAT_VideoInfo2)
// 				{
// 					if (Type.cbFormat >= sizeof(VIDEOINFOHEADER2))
// 					{
// 						VIDEOINFOHEADER2 v2;
// 						memcpy((void*)&v2,Type.pbFormat,sizeof(VIDEOINFOHEADER2));
// 						m_VideoInfo.iVideowidth = v2.rcSource.right;
// 						m_VideoInfo.iVideoHeight = v2.rcSource.bottom;
// 						double t = UNITS;
// 						t /= v2.AvgTimePerFrame;
// 						m_VideoInfo.iFrameRate   = int(t+0.5);
// 						m_VideoInfo.lVideoBitRate = v2.dwBitRate;
// 					}
// 				}
// 				m_StreamInfo.llFileDuration = nMinNum;
// 				if (m_VideoInfo.iVideoHeight == 240)
// 				{
// 					m_VideoInfo.strAspectRatio = "4:3";
// 				}
// 				else if (m_VideoInfo.iVideoHeight == 144)
// 				{
// 					m_VideoInfo.strAspectRatio = "16:9";
// 				}
// 				m_StreamInfo.VideoInfo = m_VideoInfo;
// 			}
// 			else if (Type.majortype == MEDIATYPE_Audio)
// 			{
// 				WAVEFORMATEX wfe;
// 				//{000000FF-0000-0010-8000-00AA00389B71}
// 				if (Type.subtype == MEDIASUBTYPE_AUDIO_AAC)
// 				{
// 					m_AudioInfo.strAudioCodecFormat = "AAC";
// 					
// 				}
// 				else if (Type.subtype == MEDIASUBTYPE_VIDEO_samr 
// 					|| Type.subtype == MEDIASUBTYPE_VIDEO_MC_samr)
// 				{
// 					m_AudioInfo.strAudioCodecFormat = "AMR";
// 				}
// 				if (Type.formattype == FORMAT_WaveFormatEx)
// 				{
// 					if (Type.cbFormat >= sizeof(WAVEFORMATEX) )
// 					{
// 						memcpy((void*)&wfe,Type.pbFormat,sizeof(WAVEFORMATEX));
// 						m_AudioInfo.lChannels = wfe.nChannels;
// 						m_AudioInfo.iAudioSamplingRate = wfe.nSamplesPerSec;
// 						m_AudioInfo.lAudioQuantizationBits = 16;
// 						
// 					}
// 				}
// 				m_StreamInfo.AudioInfo = m_AudioInfo;
// 			}
// 			FreeMediaType(Type );
// 		}
	}
	catch (...)
	{
		m_strError = "Analyze file catch error!";
		OutputDebugString("\nError Error Error Error Error Error \n");
		return false;
	}
	return true;
}