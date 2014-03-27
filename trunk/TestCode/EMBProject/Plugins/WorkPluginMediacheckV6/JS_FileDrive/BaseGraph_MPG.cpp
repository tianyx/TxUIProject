// BaseGraph_MPG.cpp: implementation of the CBaseGraph_MPG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_MPG.h"
#include "GuidRef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseGraph_MPG::CBaseGraph_MPG()
{

}

CBaseGraph_MPG::~CBaseGraph_MPG()
{

}
HRESULT CBaseGraph_MPG::AddFilters2Graph()
{
	
	HRESULT hr = E_FAIL;
	if(!m_pGraphBuilder)
		return hr;

	hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
	if (hr != S_OK)
	{
		return hr;
	}
	//添加解码器
	//zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_EM2VD,&m_pVDecoder,"MainConcept MPEG-2 Video Decoder") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_EM2VD,&m_pVDecoder,"MainConcept MPEG-2 Video Decoder");
//	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept MPEG-2 Video Decoder",&m_pVDecoder);
	if (hr != S_OK)
	{
		return hr;
	}

	IModuleConfig *pIModuleConfig;
	GUID*   pParamIDs=NULL;
	VARIANT Value ;
	VariantInit(&Value);			
	hr=m_pVDecoder->QueryInterface(IID_IModuleConfig,(void**)&pIModuleConfig);
	if(S_OK == hr)
	{
       	Value.vt=VT_UI4;
		Value.ulVal=3;
		pIModuleConfig->SetValue(&EM2VD_Quality,&Value);
		Value.ulVal=0;
		pIModuleConfig->SetValue(&EM2VD_HardwareAcceleration,&Value);
		Value.ulVal=3;
		pIModuleConfig->SetValue(&EM2VD_SynchronizationMode,&Value);
		pIModuleConfig->CommitChanges(&Value);
	    pIModuleConfig->Release();
	}

	VariantClear(&Value);

	
    //zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MPGDMX,&m_pParserFilter,"MainConcept MPEG Demultiplexer") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MPGDMX,&m_pParserFilter,"MainConcept MPEG Demultiplexer");
//	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept MPEG Demultiplexer",&m_pParserFilter);
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

    //增加音频检测
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber") && 
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber");
   if (hr != S_OK)
   {
	   return hr;
   } 
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render");
   if (hr != S_OK)
   {
	   return hr;
   }

   return hr;
}

HRESULT CBaseGraph_MPG::ConnectFiltersInGraph()
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
	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pSampGrabberF);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pAudioGrabberF);
	if (hr != S_OK)
	{
		return hr;
	}
    hr = ConnectFilter(m_pGraphBuilder,m_pAudioGrabberF,m_pANullRenderF);

	return hr;
	   
}
HRESULT CBaseGraph_MPG::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph_M2V builder!\n");
	HRESULT hr = E_FAIL;
	if (AddFilters2Graph() == S_OK)
	{
		hr = SetVideoGrabber();
		if (hr != S_OK)
		{
			return hr;
		}
		return ConnectFiltersInGraph();
	}
	return hr;
}
