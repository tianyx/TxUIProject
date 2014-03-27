// FileInfoExtract_WMV.cpp: implementation of the CFileInfoExtract_WMV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileInfoExtract_WMV.h"
#include "BaseGraph_WMV.h"
#include <Dvdmedia.h>
#include <io.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// {000000FF-0000-0010-8000-00AA00389B71}
// DEFINE_MEDIATYPE_GUID(MFVideoFormat_ABCD_Format, FCC('ABCD') );
// {32315659-0000-0010-8000-00AA00389B71}
// DEFINE_GUID(MEDIASUBTYPE_VIDEO_WMV,0x32315659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71);

CFileInfoExtract_WMV::CFileInfoExtract_WMV()
{

}

CFileInfoExtract_WMV::~CFileInfoExtract_WMV()
{

}
BOOL CFileInfoExtract_WMV::AnalyzeFile()
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
		CBaseGraph_WMV WmvBaskGrahp;
		hr = WmvBaskGrahp.LoadFile(m_strFileName.c_str(),0,0);
		hr = WmvBaskGrahp.RenderFile();//ptsBaskGrahp->MakeGraph();
		if(hr != S_OK)
		{
			OutputDebugString("”Ô∑®ºÏ≤‚£¨º”‘ÿÀÿ≤ƒ ß∞‹");
			return false;
		}
			
	    LONGLONG nMinNum = WmvBaskGrahp.GetDuration();
		
		int Streams = WmvBaskGrahp.GetSrcStreamCount();
		for( int i = 0 ; i < Streams ; i++ )
		{
			AM_MEDIA_TYPE Type;
            WmvBaskGrahp.GetSrcStreamMedaiType(i,&Type);			
			double nDuration  = NULL;
			double nFrameRate = NULL;
			if(Type.majortype == MEDIATYPE_Video ) 
			{
				//{32315659-0000-0010-8000-00AA00389B71}
	//			if (Type.subtype == MEDIASUBTYPE_VIDEO_H264)
				{
					m_VideoInfo.strVideoCodecFormat = "WMVideo";
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
							m_VideoInfo.lVideoBitRate = v2.dwBitRate;
						}
					}
					m_StreamInfo.llFileDuration = ((nMinNum*25) / 10000000); 
					m_VideoInfo.strAspectRatio = "4:3";
					m_StreamInfo.VideoInfo = m_VideoInfo;
				}
			}
			else if (Type.majortype == MEDIATYPE_Audio)
			{
				WAVEFORMATEX wfe;
				//{000000FF-0000-0010-8000-00AA00389B71}
				//if (Type.subtype == MFAudioFormat_WMAudioV8)
				{
					m_AudioInfo.strAudioCodecFormat = "WMAudio";
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
	}
	catch (...)
	{
		m_strError = "Analyze file catch error!";
		OutputDebugString("\nError Error Error Error Error Error \n");
		return false;
	}
	//CoUninitialize();
	return true;
}

void CFileInfoExtract_WMV::GenerateGUID(GUID guid,CString &strGUID)
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