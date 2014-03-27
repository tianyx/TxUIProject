// FileInfoExtract_TS.cpp: implementation of the CFileInfoExtract_TS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileInfoExtract_TS.h"
#include <Dvdmedia.h>
#include <io.h>
#include "BaseGraph_TS.h"
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFileInfoExtract_TS::CFileInfoExtract_TS()
{

}

CFileInfoExtract_TS::~CFileInfoExtract_TS()
{

}

BOOL CFileInfoExtract_TS::AnalyzeFile()
{
//	CoInitialize(NULL);

	try
	{
		if(_access((LPCSTR)m_strFileName.c_str(),04) != 0)
		{
			m_strError = "File not exist!";
			return false;
		}
		
		HRESULT hr = E_FAIL;
		CBaseGraph_TS *ptsBaskGrahp = new CBaseGraph_TS();
		hr = ptsBaskGrahp->LoadFile(m_strFileName.c_str(),0,0);
		hr = ptsBaskGrahp->RenderFile();//ptsBaskGrahp->MakeGraph();
		if(hr != S_OK)
		{
			OutputDebugString("语法检测，加载素材失败");
			delete ptsBaskGrahp;
		    ptsBaskGrahp = NULL;
			return false;
		}
			
	    LONGLONG nMinNum = ptsBaskGrahp->GetDuration();
		m_StreamInfo.llFileDuration = nMinNum ;

		int Streams = ptsBaskGrahp->GetSrcStreamCount();
		for( int i = 0 ; i < Streams ; i++ )
		{
			AM_MEDIA_TYPE Type;
            ptsBaskGrahp->GetSrcStreamMedaiType(i,&Type);			
			double nDuration  = NULL;
			double nFrameRate = NULL;
			if(Type.majortype == MEDIATYPE_Video ) 
			{
				//{32315659-0000-0010-8000-00AA00389B71}
	//			if (Type.subtype == MEDIASUBTYPE_VIDEO_H264)
				{
					m_VideoInfo.strVideoCodecFormat = "H264";
					//{05589F80-C356-11CE-BF01-00AA0055595A}
	//				if (Type.formattype == FORMAT_H264)
					{
						if (Type.cbFormat >= sizeof(VIDEOINFOHEADER))
						{
							VIDEOINFOHEADER v2;
							memcpy((void*)&v2,Type.pbFormat,sizeof(VIDEOINFOHEADER));
							m_VideoInfo.iVideowidth = v2.rcSource.right;
							m_VideoInfo.iVideoHeight = v2.rcSource.bottom;
							//m_VideoInfo.iFrameRate = UNITS/v2.AvgTimePerFrame;
							//zibj
							if(v2.AvgTimePerFrame!=0)
							{
								m_VideoInfo.iFrameRate = UNITS/v2.AvgTimePerFrame;
							}
							else 
								m_VideoInfo.iFrameRate = 0;
							//zibj end
						}
					}

					int gcd = GCD(m_VideoInfo.iVideowidth, m_VideoInfo.iVideoHeight);
					CString strTem;

					if (gcd > 0)
					{
						strTem.Format("%d:%d", m_VideoInfo.iVideowidth / gcd, m_VideoInfo.iVideoHeight / gcd);
					}
					else
					{
						strTem.Format("%d:%d", m_VideoInfo.iVideowidth, m_VideoInfo.iVideoHeight);
					}

					m_VideoInfo.strAspectRatio = strTem;
					m_StreamInfo.VideoInfo = m_VideoInfo;
				}
			}
			else if (Type.majortype == MEDIATYPE_Audio)
			{
				WAVEFORMATEX wfe;
				//{000000FF-0000-0010-8000-00AA00389B71}
				if (Type.subtype == MEDIASUBTYPE_AUDIO_AAC)
				{
					//由于江苏IPTV不审看AAC音频，所以去掉
					if(i == 2)
						continue;
					m_AudioInfo.strAudioCodecFormat = "AAC";
					if (Type.formattype == FORMAT_WaveFormatEx)
					{
						if (Type.cbFormat >= sizeof(WAVEFORMATEX) )
						{
							memcpy((void*)&wfe,Type.pbFormat,sizeof(WAVEFORMATEX));
							m_AudioInfo.lChannels = wfe.nChannels;
							m_AudioInfo.iAudioSamplingRate = wfe.nSamplesPerSec;
							m_AudioInfo.lAudioQuantizationBits = 16;
							
						}
					}
				}
				else if(Type.subtype == MEDIASUBTYPE_AUDIO_MP1_LAYER2)
				{
					CString strSubTypeGuid = "";
					GenerateGUID(Type.subtype,strSubTypeGuid);

					OutputDebugString("音频GUID为：" + strSubTypeGuid);
					m_AudioInfo.strAudioCodecFormat = "MPEG-LAYER2";
					if (Type.formattype == FORMAT_WaveFormatEx)
					{
						if (Type.cbFormat >= sizeof(WAVEFORMATEX) )
						{
							memcpy((void*)&wfe,Type.pbFormat,sizeof(WAVEFORMATEX));
							m_AudioInfo.lChannels = wfe.nChannels;
							m_AudioInfo.iAudioSamplingRate = wfe.nSamplesPerSec;
							m_AudioInfo.lAudioQuantizationBits = 16;
							
						}
					}
				}
				m_StreamInfo.AudioInfo = m_AudioInfo;
			}
			FreeMediaType(Type );
		}

		delete ptsBaskGrahp;
		ptsBaskGrahp = NULL;

// 		CComPtr< IMediaDet > pDet;
// 		HRESULT hr = CoCreateInstance( CLSID_MediaDet, NULL, CLSCTX_INPROC_SERVER, 
// 			IID_IMediaDet, (void**) &pDet );
// 		if(hr != S_OK)
// 		{
// 			m_strError = "Failed in CoCreateInstance";
// 			return false;
// 		}
// 		
// 		CComBSTR openBSTR(m_strFileName.c_str());
// 		hr = pDet->put_Filename(openBSTR);
// 		if(hr != S_OK)
// 		{
// 			m_strError = "Couldn't load the file";
// 			return false;
// 		}
// 		long Streams = 0;
// 		hr = pDet->get_OutputStreams( &Streams );
// 		if(hr != S_OK)
// 		{
// 			m_strError = "get_OutputStreams";
// 			return false;
// 		} 
	
	
// 		for( int i = 0 ; i < Streams ; i++ )
// 		{
// 			AM_MEDIA_TYPE Type;
// 			memset( &Type, 0, sizeof( Type ) );
// 			// Select a media stream
// 			hr = pDet->put_CurrentStream( i );
// 			if(hr != S_OK)
// 			{
// 				m_strError = "put_CurrentStream";
// 				FreeMediaType(Type );
// 				return false;
// 			}
// 			// Read the media type of the selected stream
// 			hr = pDet->get_StreamMediaType( &Type );
// 			if(hr != S_OK)
// 			{
// 				m_strError = "get_StreamMediaType";
// 				FreeMediaType(Type );
// 				return false;
// 			}
// 			
// 			double nDuration  = NULL;
// 			double nFrameRate = NULL;
// 			if(Type.majortype == MEDIATYPE_Video ) 
// 			{
// 				//hr = pDet->get_StreamLength(&nDuration);
// 				hr = pDet->get_FrameRate(&nFrameRate);
// 				//m_StreamInfo.llFileDuration = (nDuration)*(nFrameRate);
// 				//zhou liang add
// 				//////////////////////////////////////////////////////////////////////////
// 				//由于之前发现有些素材时常不准，所以需要增加从链路中得到时常
// 				CBaseGraph_TS *ptsBaskGrahp = new CBaseGraph_TS();
// 				hr = ptsBaskGrahp->LoadFile(m_strFileName.c_str(),0,0);
// 				hr = ptsBaskGrahp->MakeGraph();
// 				if(SUCCEEDED(hr))
// 				{
// 					hr = ptsBaskGrahp->AddFilters2Graph();
// 					if(SUCCEEDED(hr))
// 						hr = ptsBaskGrahp->ConnectFiltersInGraph();
// 					else
// 					{
// 						OutputDebugString("语法检测，添加FILTER失败!");
// 					}
// 				}
// 				else
// 					OutputDebugString("语法检测，加载素材失败");
// 				if(SUCCEEDED(hr))
// 				{
// 					LONGLONG nMinNum = ptsBaskGrahp->GetDuration();
// 					m_StreamInfo.llFileDuration = nMinNum * nFrameRate / 10000000; //nMinNum / (10000000 / nFrameRate)
// 				}
// 				else
// 					m_StreamInfo.llFileDuration = 0;
// 
// 				//ptsBaskGrahp->ClearUpForStaticGhaph();
// 				delete ptsBaskGrahp;
// 				ptsBaskGrahp = NULL;
// 				//////////////////////////////////////////////////////////////////////////
// 				
// 				
// 				//{32315659-0000-0010-8000-00AA00389B71}
// 	//			if (Type.subtype == MEDIASUBTYPE_VIDEO_H264)
// 				{
// 					m_VideoInfo.strVideoCodecFormat = "H264";
// 					//{05589F80-C356-11CE-BF01-00AA0055595A}
// 	//				if (Type.formattype == FORMAT_H264)
// 					{
// 						if (Type.cbFormat >= sizeof(VIDEOINFOHEADER))
// 						{
// 							VIDEOINFOHEADER v2;
// 							memcpy((void*)&v2,Type.pbFormat,sizeof(VIDEOINFOHEADER));
// 							m_VideoInfo.iVideowidth = v2.rcSource.right;
// 							m_VideoInfo.iVideoHeight = v2.rcSource.bottom;
// 							m_VideoInfo.iFrameRate   = UNITS/v2.AvgTimePerFrame;
// 						}
// 					}
// 					m_VideoInfo.strAspectRatio = "4:3";
// 					m_StreamInfo.VideoInfo = m_VideoInfo;
// 				}
// 			}
// 			else if (Type.majortype == MEDIATYPE_Audio)
// 			{
// 				WAVEFORMATEX wfe;
// 				//{000000FF-0000-0010-8000-00AA00389B71}
// 				if (Type.subtype == MEDIASUBTYPE_AUDIO_AAC)
// 				{
// 					//由于江苏IPTV不审看AAC音频，所以去掉
// 					if(i == 2)
// 						continue;
// 					m_AudioInfo.strAudioCodecFormat = "AAC";
// 					if (Type.formattype == FORMAT_WaveFormatEx)
// 					{
// 						if (Type.cbFormat >= sizeof(WAVEFORMATEX) )
// 						{
// 							memcpy((void*)&wfe,Type.pbFormat,sizeof(WAVEFORMATEX));
// 							m_AudioInfo.lChannels = wfe.nChannels;
// 							m_AudioInfo.iAudioSamplingRate = wfe.nSamplesPerSec;
// 							m_AudioInfo.lAudioQuantizationBits = 16;
// 							
// 						}
// 					}
// 				}
// 				else if(Type.subtype == MEDIASUBTYPE_AUDIO_MP1_LAYER2)
// 				{
// 					CString strSubTypeGuid = "";
// 					GenerateGUID(Type.subtype,strSubTypeGuid);
// 
// 					OutputDebugString("音频GUID为：" + strSubTypeGuid);
// 					m_AudioInfo.strAudioCodecFormat = "MPEG-LAYER2";
// 					if (Type.formattype == FORMAT_WaveFormatEx)
// 					{
// 						if (Type.cbFormat >= sizeof(WAVEFORMATEX) )
// 						{
// 							memcpy((void*)&wfe,Type.pbFormat,sizeof(WAVEFORMATEX));
// 							m_AudioInfo.lChannels = wfe.nChannels;
// 							m_AudioInfo.iAudioSamplingRate = wfe.nSamplesPerSec;
// 							m_AudioInfo.lAudioQuantizationBits = 16;
// 							
// 						}
// 					}
// 				}
// 				m_StreamInfo.AudioInfo = m_AudioInfo;
// 			}
// 			FreeMediaType(Type );
// 		}
//		pDet.Release();
	}
	catch (...)
	{
		m_strError = "Analyze file catch error!";
		return false;
	}
	//CoUninitialize();
	return true;
}

void CFileInfoExtract_TS::GenerateGUID(GUID guid,CString &strGUID)
{
	CString		strFormat =  CString("%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"); 
	
	strGUID.Format(strFormat,
		// first copy...
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
		// second copy...
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

int CFileInfoExtract_TS::GCD( int nLeft, int nRight )
{
	if (0 == nLeft || 0 == nRight || nLeft < 0 || nRight < 0)
	{
		return 0;
	}

	if (nLeft == nRight)
	{
		return nLeft;
	}

	int a = nLeft > nRight ? nLeft : nRight; // max
	int b = nLeft < nRight ? nLeft : nRight; // min
    int c = a % b; // 余数

	while(c > 0)
	{
		a = b;
		b = c;

		c = a % b; // 余数
	}

	return b;
}
