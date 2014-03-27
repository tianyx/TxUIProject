// BaseGraph_AVI.cpp: implementation of the CBaseGraph_AVI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_AVI.h"
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// {74A541E1-34B0-11CF-82DA-0020AF11C010} 
// DEFINE_GUID(CLSID_MatroxRiffReader, 
// 			0x74A541E1, 0x34B0, 0x11CF, 0x82, 0xDA, 0x00, 0x20, 0xAF, 0x11, 0xC0, 0x10);

//DEFINE_GUID (CLSID_FounderAVIDemultiplexer,  0x02b3384f, 0xfacf, 0x491e, 0x99, 0x19, 0xef, 0x22, 0x02, 0xcf, 0xbc, 0xcc);

CBaseGraph_AVI::CBaseGraph_AVI()
{

}

CBaseGraph_AVI::~CBaseGraph_AVI()
{

}

HRESULT CBaseGraph_AVI::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph_AVI builder!\n");
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

HRESULT CBaseGraph_AVI::AddFilters2Graph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder)
	{
		return hr;
	}
	hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder, CLSID_FounderAVIDemultiplexer, &m_pParserFilter, "Founder AVI Demux") &&
		AddFilter2GraphByClassID(m_pGraphBuilder, CLSID_FounderAVIDemultiplexer, &m_pParserFilter, "Founder AVI Demux");
	if (hr != S_OK)
	{
		return hr;
	}
// 	IFileSourceFilter* pIFileSource = NULL;
// 	hr = m_pFileSrcF->QueryInterface(IID_IFileSourceFilter,(void**)&pIFileSource);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}

// 	USES_CONVERSION;
// 	hr=pIFileSource->Load(A2W(m_filePath),NULL);
//     if(hr != S_OK)
// 	{
// 		pIFileSource->Release();
// 		return hr;
// 	}
// 	pIFileSource->Release();

	//zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_EM2VD,&m_pVDecoder,"MainConcept MPEG-2 Video Decoder") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_EM2VD,&m_pVDecoder,"MainConcept MPEG-2 Video Decoder");
	//hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept MPEG-2 Video Decoder",&m_pVDecoder);
	if (hr != S_OK)
	{
		return hr;
	}

	IModuleConfig *pIModuleConfig;
	GUID*   pParamIDs=NULL;
	VARIANT Value ;
	VariantInit(&Value);			
	hr=m_pVDecoder->QueryInterface(IID_IModuleConfig,(void**)&pIModuleConfig);
	Value.vt=VT_UI4;
	Value.ulVal=3;
	pIModuleConfig->SetValue(&EM2VD_Quality,&Value);
	Value.ulVal=0;
	pIModuleConfig->SetValue(&EM2VD_HardwareAcceleration,&Value);
	Value.ulVal=3;
	pIModuleConfig->SetValue(&EM2VD_SynchronizationMode,&Value);
	pIModuleConfig->CommitChanges(&Value);
	pIModuleConfig->Release();
	VariantClear(&Value);

   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber");
   if (hr != S_OK)
   {
	   return hr;
   }
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
   return hr;
}

HRESULT CBaseGraph_AVI::ConnectFiltersInGraph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder || !m_pFileSrcF ||!m_pSampGrabberF ||!m_pVNullRenderF ||! m_pParserFilter)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter (m_pGraphBuilder, m_pParserFilter, m_pVDecoder,TRUE);
	   if (hr != S_OK)//Mpeg2解码连接不成功
	   {
		   m_pVDecoder->Release();//前面的接口
		   m_pGraphBuilder->RemoveFilter(m_pVDecoder);
		   
		   //zibj
		   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_DVVideoCodec,&m_pVDecoder,"DV Video Decoder") &&
			   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_DVVideoCodec,&m_pVDecoder,"DV Video Decoder");
		   // hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"DV Video Decoder",&m_pVDecoder);
		   if (hr != S_OK)
		   {
			   return hr;
		   }
		   hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pVDecoder,TRUE);//重新连接
		   if (hr != S_OK)
		   {
			   return hr;
		   }
	   }
	   hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pSampGrabberF);
	   if (hr != S_OK)
	   {
		   return hr;
	   }
	   hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF);
	   return hr;
}