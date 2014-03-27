// BaseGraph_Hik.cpp: implementation of the CBaseGraph_Hik class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_Hik.h"
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseGraph_Hik::CBaseGraph_Hik()
{

}

CBaseGraph_Hik::~CBaseGraph_Hik()
{

}

HRESULT CBaseGraph_Hik::AddFilters2Graph()
{
	
	HRESULT hr = E_FAIL;
	if(!m_pGraphBuilder)
		return hr;
	try{
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikPushFileSource,&m_pFileSrcF,"Hik File Source") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikPushFileSource,&m_pFileSrcF,"Hik File Source");
	if (hr != S_OK)
	{
		return hr;
	}

	IFileSourceFilter* pFileSourceFilter=NULL;
	hr=m_pFileSrcF->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSourceFilter);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = LoadFile(pFileSourceFilter, m_filePath);
	pFileSourceFilter->Release();
	if (hr != S_OK)
	{		
		return hr;
	}
	}
	catch(...)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikNetSplitter,&m_pParserFilter,"Hik File Splitter") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikNetSplitter,&m_pParserFilter,"Hik File Splitter");
	if (hr != S_OK)
	{
		return hr;
	}
	//添加解码器
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikH264Decoder,&m_pVDecoder,"Hik H264 Decoder") && 
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikH264Decoder,&m_pVDecoder,"Hik H264 Decoder");
	if (hr != S_OK)
	{
		return hr;
	}
	   // 创建颜色空间转换

	//zibj
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_ECSC,&m_pColorSpaceFilter,"MainConcept Color Space Converter") && 
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_ECSC,&m_pColorSpaceFilter,"MainConcept Color Space Converter");
    //hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept Color Space Converter",&m_pColorSpaceFilter);
    if (hr != S_OK)
    {
 	   return hr;
    }
   // 创建Sample Grabber
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber")&& 
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

   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikG722Decoder,&m_pADecoder,"Hik G722 Decoder") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_HikG722Decoder,&m_pADecoder,"Hik G722 Decoder");
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

HRESULT CBaseGraph_Hik::ConnectFiltersInGraph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder || !m_pFileSrcF ||!m_pSampGrabberF ||!m_pVNullRenderF)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pVDecoder);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pColorSpaceFilter);
    if (hr != S_OK)
    {
		return hr;
    }
	hr = ConnectFilter(m_pGraphBuilder,m_pColorSpaceFilter,m_pSampGrabberF);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pADecoder);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pADecoder,m_pAudioGrabberF);
	if (hr != S_OK)
	{
		return hr;
	}
    hr = ConnectFilter(m_pGraphBuilder,m_pAudioGrabberF,m_pANullRenderF);
	return hr;
	   
}
HRESULT CBaseGraph_Hik::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph_Hik builder!\n");
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
