// BaseGraph_MP4.cpp: implementation of the CBaseGraph_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_MP4.h"
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseGraph_MP4::CBaseGraph_MP4()
{

}

CBaseGraph_MP4::~CBaseGraph_MP4()
{

}

HRESULT CBaseGraph_MP4::AddFilters2Graph()
{
	
	HRESULT hr = E_FAIL;
	if(!m_pGraphBuilder)
		return hr;

// 	hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
// 	hr = AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_MP4_Demux,&m_pParserFilter,"MainConcept Mp4 DeMux");
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}

	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_HaaliSpliter,&m_pFileSrcF,"Haali Spliter Filter") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_HaaliSpliter,&m_pFileSrcF,"Haali Spliter Filter");
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

	//添加解码器
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_H264_VDecoder,&m_pVDecoder,"MainConcept H264 Video Decoder") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_H264_VDecoder,&m_pVDecoder,"MainConcept H264 Video Decoder");
	if (hr != S_OK)
	{
		return hr;
	}
	
   // hr = AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_ELMPGLayer2AudioDecoder,&m_pADecoder,"MainConcept Layer2 Audio Decoder");
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_AAC_ADecoder,&m_pADecoder,"MainConcept AAC Audio Decoder") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_AAC_ADecoder,&m_pADecoder,"MainConcept AAC Audio Decoder");
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
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
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
  
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render");
   return hr;
   
}

HRESULT CBaseGraph_MP4::ConnectFiltersInGraph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder || !m_pFileSrcF ||!m_pSampGrabberF ||!m_pVNullRenderF)
	{
		return hr;
	}
// 	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter,true);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pVDecoder,true);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pADecoder,true);
	if (hr != S_OK)
	{
// 		//尝试用AAC去解码
// 		m_pGraphBuilder->RemoveFilter(m_pADecoder);
// 		SAFE_RELEASE(m_pADecoder);
// 		hr = AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_AAC_ADecoder,&m_pADecoder,"MainConcept AAC Audio Decoder");
// 		if (hr != S_OK)
// 		{
// 			return hr;
// 		} 
// 		hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pADecoder,true);
// 		if (hr != S_OK)
// 		    return hr;
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pSampGrabberF,true);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF,true);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pADecoder,m_pAudioGrabberF,true);
	if (hr != S_OK)
	{
		return hr;
	}
    hr = ConnectFilter(m_pGraphBuilder,m_pAudioGrabberF,m_pANullRenderF,true);
	return hr;
	   
}
HRESULT CBaseGraph_MP4::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph_MP4 builder!\n");
	HRESULT hr = E_FAIL;
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
	return hr;
}

LONGLONG CBaseGraph_MP4::GetDuration()
{
    if(m_pMediaSeeking == NULL)
	{
		HRESULT hr = GetMediaSeeking(m_pGraphBuilder,&m_pMediaSeeking);
		if(hr != S_OK)
			return 0;
	}
	LONGLONG tcl = 0;
	if(S_OK == m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_FRAME))
	m_pMediaSeeking->GetDuration(&tcl);
	
	return  tcl ;  //ns -> frame
}