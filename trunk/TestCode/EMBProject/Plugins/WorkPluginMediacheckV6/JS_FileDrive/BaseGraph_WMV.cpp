// BaseGraph_WMV.cpp: implementation of the CBaseGraph_WMV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_WMV.h"
#include <DMOReg.h>
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseGraph_WMV::CBaseGraph_WMV()
{
  OutputDebugString("WWW: CBaseGraph_WMV::CBaseGraph_WMV()");
}

CBaseGraph_WMV::~CBaseGraph_WMV()
{
   OutputDebugString("WWW: CBaseGraph_WMV::~CBaseGraph_WMV()");
}
HRESULT CBaseGraph_WMV::AddFilters2Graph()
{
	TRACE("<<<into CBaseGraph_WMV AddFilters2Graph!\n");
	HRESULT hr = E_FAIL;
	if(!m_pGraphBuilder)
		return hr;
	//WMVReader
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_WMVReader,&m_pFileSrcF,NULL) &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_WMVReader,&m_pFileSrcF,NULL);
	if (hr != S_OK)
	{
		return hr;
	}
	IFileSourceFilter* pIFileSource = NULL;
    hr = m_pFileSrcF->QueryInterface(IID_IFileSourceFilter,(void**)&pIFileSource);
	if (hr != S_OK)
	{
		return hr;
	}
	// wang.zh modified.
	hr = LoadFile(pIFileSource, m_filePath);
	pIFileSource->Release();
	if ( hr != S_OK )
	{
		return hr;
	}
	
	//添加视频解码器
	hr = AddDMO2GraphByClassID(m_pGraphBuilder, CLSID_WMVideoDecoderDMO, DMOCATEGORY_VIDEO_DECODER,&m_pVDecoder,"WM Video Decoder DMO");
	if (hr != S_OK)
	{
		return hr;
	}
	//添加音频解码器
	hr = AddDMO2GraphByClassID(m_pGraphBuilder, CLSID_WMAudioDecoderDMO, DMOCATEGORY_AUDIO_DECODER,&m_pADecoder,"WM Audio Decoder DMO");
	if (hr != S_OK)
	{
		return hr;
	}
	// 创建Sample Grabber
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render");
    return hr;

}
HRESULT CBaseGraph_WMV::ConnectFiltersInGraph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder || !m_pFileSrcF ||!m_pSampGrabberF ||!m_pVNullRenderF)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pADecoder,TRUE);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pVDecoder,TRUE);
	if (hr != S_OK)
	{
		return hr;
	}
	   ///跟踪Filter/////////////
// 	IBaseFilter *pFilterTracer = NULL;
// 	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"Filter Tracer",&pFilterTracer);
// 	if (hr != S_OK)
// 	{
// 	   return hr;
// 	}
// 	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,pFilterTracer,TRUE);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pSampGrabberF,TRUE);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pADecoder,m_pAudioGrabberF,TRUE);
	if (hr != S_OK)
	{
		return hr;
	}
//	hr = ConnectFilter(m_pGraphBuilder,m_pColorSpaceFilter,m_pSampGrabberF);
//	if (hr != S_OK)
//	{
//		return hr;
//	}
	hr = ConnectFilter(m_pGraphBuilder,m_pAudioGrabberF,m_pANullRenderF,TRUE);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF,TRUE);
	return hr;

}
HRESULT CBaseGraph_WMV::BuildGrabberGraph()
{
	HRESULT hr = E_FAIL;
   	TRACE("<<<into CBaseGraph_WMV builder!\n");
	if (AddFilters2Graph() == S_OK)
	{
		hr = SetVideoGrabber();
		if (hr != S_OK)
		{
			return hr;
		}
		hr = SetAudioGrabber();
		if (hr != S_OK)
		{
			return hr;
		}
		return ConnectFiltersInGraph();
	}
	return E_FAIL;
}

int CBaseGraph_WMV::GetSrcStreamCount()
{
	int nStreamCount = 0;
	if (m_pFileSrcF)
	{
		HRESULT hr = E_FAIL;
		IEnumPins *pEnumPins;
		hr = m_pFileSrcF->EnumPins( &pEnumPins );
		if( FAILED( hr ) )
		{
			return nStreamCount;
		}
		
		IPin            *pTempPin    = NULL;
		ULONG           fetched      = 0;
		hr = pEnumPins->Reset( );
		while(S_OK == pEnumPins->Next( 1, &pTempPin, &fetched ))
		{
			ASSERT( pTempPin );
			PIN_DIRECTION   dir;
			hr = pTempPin->QueryDirection( &dir );
			if(dir == PINDIR_OUTPUT)
			{
				AM_MEDIA_TYPE amt;
				if(pTempPin->ConnectionMediaType(&amt) == S_OK)
				{
					if (amt.majortype == MEDIATYPE_Video 
						|| amt.majortype == MEDIATYPE_Audio)
					{
						nStreamCount++;
					}
				}
				FreeMediaType(amt);
			}
			pTempPin->Release();
		}
		pEnumPins->Release();
	}
	return nStreamCount;
}
//int nStream:input
//AM_MEDIA_TYPE* AM:output 
HRESULT CBaseGraph_WMV::GetSrcStreamMedaiType(int nStream,AM_MEDIA_TYPE* AM)
{
	HRESULT hr = E_FAIL;
   	if (m_pFileSrcF)
	{
		int nStreamCount = 0;
		IEnumPins *pEnumPins;
		hr = m_pFileSrcF->EnumPins( &pEnumPins );
		if( FAILED( hr ) )
		{
			return hr;
		}
		
		IPin   *pTempPin = NULL;
		ULONG  fetched   = 0;
		bool   bDo = true;
		hr = pEnumPins->Reset( );
		while(bDo)
		{
			if (S_OK == pEnumPins->Next( 1, &pTempPin, &fetched ))
			{
				ASSERT( pTempPin );
				PIN_DIRECTION   dir;
				hr = pTempPin->QueryDirection( &dir );
				if(dir == PINDIR_OUTPUT)
				{
					AM_MEDIA_TYPE amt;
					if(pTempPin->ConnectionMediaType(&amt) == S_OK)
					{
						if (amt.majortype == MEDIATYPE_Video 
							|| amt.majortype == MEDIATYPE_Audio)
						{
							if (nStreamCount == nStream)
							{
								hr = CopyMediaType(AM,&amt);
								bDo = false;
							}
							nStreamCount++;
						}
					}
					FreeMediaType(amt);
				}
				pTempPin->Release();
			}
		}
		pEnumPins->Release();
	}
	return hr;
}