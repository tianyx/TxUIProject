// FileInfoExtract_Hik.cpp: implementation of the CFileInfoExtract_Hik class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <io.h>
#include "FileInfoExtract_Hik.h"
#include "BaseGraph_Hik.h"
#include "GuidRef.h"
#include <Dvdmedia.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInfoExtract_Hik::CFileInfoExtract_Hik()
{
	
}

CFileInfoExtract_Hik::~CFileInfoExtract_Hik()
{
}
BOOL CFileInfoExtract_Hik::AnalyzeFile()
{
	    CoInitialize(NULL);
		if(_access((LPCSTR)m_strFileName.c_str(),04) != 0)
		{
			m_strError = "File not exist!";
			CoUninitialize();
			return false;
		}

		CComPtr< IMediaDet > pDet;
		HRESULT hr = CoCreateInstance( CLSID_MediaDet, NULL, CLSCTX_INPROC_SERVER, 
			IID_IMediaDet, (void**) &pDet );
		if(hr != S_OK)
		{
			m_strError = "Failed in CoCreateInstance";
			CoUninitialize();
			return false;
		}
		IBaseFilter* pSourceFilter = NULL;
		IFileSourceFilter* pFileSourceFilter=NULL;
		try{
		hr = CoCreateInstance(CLSID_HikPushFileSource, NULL, CLSCTX_INPROC_SERVER,
			IID_IBaseFilter, (void**)(&pSourceFilter));
		if (FAILED(hr))
		{
			m_strError = "Cannot Create source Filter to MediaDet";
			pDet.Release();
			CoUninitialize();
			return false;
		}

		hr=pSourceFilter->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSourceFilter);
		if(FAILED(hr))
		{
			m_strError = "Cannot Find IFileSourceFilter";
			pSourceFilter->Release();
			pDet.Release();
			CoUninitialize();
			return false;
		}
		USES_CONVERSION;
		hr = pFileSourceFilter->Load(A2OLE(m_strFileName.c_str()),NULL);
	    pFileSourceFilter->Release();
		if(FAILED(hr))
		{
			m_strError = "Cannot LoadFile";
			pSourceFilter->Release();
			pDet.Release();
			CoUninitialize();
			return false;
		}
		}
		catch(...)
		{
			return false;
		}
		IUnknown* pUnKnown = NULL;
		hr = pSourceFilter->QueryInterface(IID_IUnknown,(void**)&pUnKnown);
		if (FAILED(hr))
		{
			m_strError = "Cannot Find IUnknown interface from SourceFilter";
			pSourceFilter->Release();
			pDet.Release();
			CoUninitialize();
			return false;
		}
		hr = pDet->put_Filter(pUnKnown);
		if (FAILED(hr))
		{
			m_strError = "Cannot put filter to Mediadet";
			pSourceFilter->Release();
			pDet.Release();
			CoUninitialize();
			return false;
		}

// 		CComBSTR openBSTR(m_strFileName.c_str());
// 		hr = pDet->put_Filename(openBSTR);
// 		if(hr != S_OK)
// 		{
// 			m_strError = "Couldn't load the file";
// 			return false;
// 		}
		long Streams = 0;
		hr = pDet->get_OutputStreams( &Streams );
		if(hr != S_OK)
		{
			m_strError = "get_OutputStreams";
			pSourceFilter->Release();
			pDet.Release();
			CoUninitialize();
			return false;
		}
		for( int i = 0 ; i < Streams ; i++ )
		{
			AM_MEDIA_TYPE Type;
			memset( &Type, 0, sizeof( Type ) );
			// Select a media stream
			hr = pDet->put_CurrentStream( i );
			if(hr != S_OK)
			{
				m_strError = "put_CurrentStream";
				pSourceFilter->Release();
			    pDet.Release();
				CoUninitialize();
				return false;
			}
			// Read the media type of the selected stream
			hr = pDet->get_StreamMediaType( &Type );
			if(hr != S_OK)
			{
				m_strError = "get_StreamMediaType";
				pSourceFilter->Release();
			    pDet.Release();
				CoUninitialize();
				return false;
			}

			double nDuration  = NULL;
			double nFrameRate = NULL;
			if(Type.majortype == MEDIATYPE_Video ) 
			{
				hr = pDet->get_StreamLength(&nDuration);
				hr = pDet->get_FrameRate(&nFrameRate);
				m_StreamInfo.llFileDuration = (nDuration)*(nFrameRate);
                m_StreamInfo.VideoInfo.iFrameRate = nFrameRate;
				if (Type.formattype == FORMAT_VideoInfo &&
					Type.cbFormat   >= sizeof(VIDEOINFOHEADER))
				{
					VIDEOINFOHEADER* pvf = (VIDEOINFOHEADER*)Type.pbFormat;
					if (pvf)
					{
						m_StreamInfo.VideoInfo.iVideoHeight = pvf->bmiHeader.biHeight;
						m_StreamInfo.VideoInfo.iVideowidth  = pvf->bmiHeader.biWidth;
						m_StreamInfo.VideoInfo.lVideoBitRate = pvf->dwBitRate;
					}
				}
				else if (Type.formattype == FORMAT_VideoInfo2 &&
					Type.cbFormat   >= sizeof(VIDEOINFOHEADER2))
				{
					VIDEOINFOHEADER2* pvf2 = (VIDEOINFOHEADER2*)Type.pbFormat;
					if (pvf2)
					{
						m_StreamInfo.VideoInfo.iVideoHeight = pvf2->bmiHeader.biHeight;
						m_StreamInfo.VideoInfo.iVideowidth  = pvf2->bmiHeader.biWidth;
						m_StreamInfo.VideoInfo.lVideoBitRate = pvf2->dwBitRate;
					}
				}
			}
			else if(Type.majortype == MEDIATYPE_Audio && 
				   Type.formattype == FORMAT_WaveFormatEx) 
			{
				if (Type.cbFormat == sizeof(WAVEFORMATEX))
				{
					WAVEFORMATEX*  wfe = (WAVEFORMATEX*)Type.pbFormat;
					if (wfe)
					{
                        m_StreamInfo.AudioInfo.iAudioSamplingRate = wfe->nSamplesPerSec;
						m_StreamInfo.AudioInfo.lAudioQuantizationBits = wfe->wBitsPerSample;
						m_StreamInfo.AudioInfo.lChannels = wfe->nChannels;
						//m_StreamInfo.AudioInfo.strAudioCodecFormat = wfe->wFormatTag;
					}
				}
			}
			FreeMediaType(Type );
		}
	pSourceFilter->Release();
	pDet.Release();
	CoUninitialize();
	return true;
}