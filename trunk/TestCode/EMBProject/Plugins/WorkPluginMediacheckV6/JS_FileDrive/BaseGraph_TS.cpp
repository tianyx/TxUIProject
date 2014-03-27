// BaseGraph_TS.cpp: implementation of the CBaseGraph_TS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_TS.h"
// #include "CLSID/l2adec_guid.h"
#include "../Mainconcept/include/ModuleConfig/mpeg2demux_mc.h"
#include "../Mainconcept/include/ModuleConfig/ModuleConfig.h"
//#pragma comment(lib,"../Mainconcept/lib/dec_ddp.lib")
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//{DD567EF4-E509-4795-9206-27FD33130AA6}
//DEFINE_GUID(CLSID_MainConcept_H264_VDecoder,0xDD567EF4, 0xE509, 0x4795, 0x92, 0x06, 0x27, 0xFD, 0x33, 0x13, 0x0A, 0xA6);
//DEFINE_GUID(CLSID_MainConcept_H264_VDecoder,0x96B9D0ED, 0x8D13, 0x4171, 0xA9, 0x83, 0xB8, 0x4D, 0x88, 0xD6, 0x27, 0xBE);
//{19987CEE-DEE8-49DC-98EC-F21380AA9E68}
//DEFINE_GUID(CLSID_MainConcept_AAC_ADecoder,0x19987CEE, 0xDEE8, 0x49DC, 0x98, 0xEC, 0xF2, 0x13, 0x80, 0xAA, 0x9E, 0x68);

CBaseGraph_TS::CBaseGraph_TS()
{

} 

CBaseGraph_TS::~CBaseGraph_TS()
{

}
HRESULT CBaseGraph_TS::AddFilters2Graph()
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
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_H264_VDecoder,&m_pVDecoder,"MainConcept H264 Video Decoder") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_H264_VDecoder,&m_pVDecoder,"MainConcept H264 Video Decoder");
	if (hr != S_OK)
	{
		return hr;
	}
	
    hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_ELMPGLayer2AudioDecoder,&m_pADecoder,"MainConcept Layer2 Audio Decoder") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_ELMPGLayer2AudioDecoder,&m_pADecoder,"MainConcept Layer2 Audio Decoder");
	//hr = AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_AAC_ADecoder,&m_pADecoder,"MainConcept AAC Audio Decoder");
	if (hr != S_OK)
	{
		return hr;
	}
// 	IModuleConfig *pIModuleConfig;
// 	GUID*   pParamIDs=NULL;
// 	VARIANT Value ;
// 	VariantInit(&Value);			
// 	hr=m_pVDecoder->QueryInterface(IID_IModuleConfig,(void**)&pIModuleConfig);
// 	if(S_OK == hr)
// 	{
//        	Value.vt=VT_UI4;
// 		Value.ulVal=3;
// 		pIModuleConfig->SetValue(&EM2VD_Quality,&Value);
// 		Value.ulVal=0;
// 		pIModuleConfig->SetValue(&EM2VD_HardwareAcceleration,&Value);
// 		Value.ulVal=3;
// 		pIModuleConfig->SetValue(&EM2VD_SynchronizationMode,&Value);
// 		pIModuleConfig->CommitChanges(&Value);
// 	    pIModuleConfig->Release();
// 	}
// 
// 	VariantClear(&Value);

	
    //zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MPGDMX,&m_pParserFilter,"MainConcept MPEG Demultiplexer") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MPGDMX,&m_pParserFilter,"MainConcept MPEG Demultiplexer");
//	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept MPEG Demultiplexer",&m_pParserFilter);
	if (hr != S_OK)
	{
		return hr;
	}

	ConfigDemultiplexer();

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
  
   hr =AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render");
   return hr;
   
}

HRESULT CBaseGraph_TS::ConnectFiltersInGraph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder || !m_pFileSrcF ||!m_pSampGrabberF ||!m_pVNullRenderF)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter,true);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pVDecoder,true);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pADecoder,true);
	if (hr != S_OK)
	{
		//尝试用AAC去解码
		m_pADecoder->Release();
		hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_AAC_ADecoder,&m_pADecoder,"MainConcept AAC Audio Decoder") &&
			AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_AAC_ADecoder,&m_pADecoder,"MainConcept AAC Audio Decoder");
		if (hr != S_OK)
		{
			return hr;
		} 
		hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pADecoder,true);
		if (hr != S_OK)
		{
			//尝试用杜比解AC-3
			m_pADecoder->Release();
			hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConceptDolbyAC3Decoder,&m_pADecoder,"MainConcept AC-3 Audio Decoder") &&
				AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConceptDolbyAC3Decoder,&m_pADecoder,"MainConcept AC-3 Audio Decoder");
			if (hr != S_OK)
			{
				return hr;
			} 

			hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pADecoder,true);
			if (hr != S_OK)
			{
				return hr;
			} 
			else
			{
				IModuleConfig *pMcConfigMux=NULL;
				hr = m_pADecoder->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigMux); 
				if (SUCCEEDED(hr))
				{
					VARIANT vartemp,vartemp2,vartemp3,v;
					//vartemp.ClearToZero();
					vartemp.vt = VT_BSTR;
					BSTR bstrTemp = ::SysAllocString(L"DDPP-1002-FCDB-FA4A-F90F");
					vartemp.bstrVal = bstrTemp;
					vartemp2.vt = VT_I4;
					vartemp2.lVal =0;						
					hr = pMcConfigMux->SetValue(&DDPAD_SERIAL_NUMBER,&vartemp);
					hr = pMcConfigMux->SetValue(&DDPAD_AC3DEC_LFE_Channel,&vartemp2);
					vartemp2.lVal = 1;
					hr = pMcConfigMux->SetValue(&DDPAD_AC3DEC_Channels_Config,&vartemp2);
					hr = pMcConfigMux->CommitChanges(&v);
					hr = pMcConfigMux->GetValue(&DDPAD_AC3DEC_Number_Channels,&vartemp3);
					TRACE("\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%d",vartemp3.lVal);
				}
				SAFE_RELEASE(pMcConfigMux);
			}
		}
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
HRESULT CBaseGraph_TS::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph_TS builder!\n");
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

void CBaseGraph_TS::ConfigDemultiplexer()
{
	HRESULT hr = S_OK;
	IModuleConfig * pConfig = NULL;
	m_pParserFilter->QueryInterface(IID_IModuleConfig, (void**)&pConfig);

	CComVariant  var;				
	var = 1;
	hr = pConfig->SetValue(&EMPGDMX_INIT_MODE,&var);

	var = 1;
	hr = pConfig->CommitChanges(&var);
	pConfig->Release();
	if (FAILED(hr)) 
	{
		if (var.vt != VT_EMPTY)
		{
			ASSERT(var.vt = VT_BSTR);
			VariantClear(&var);
		}		
	}
}
LONGLONG CBaseGraph_TS::GetDuration()
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
// HRESULT CBaseGraph_TS::ClearUpForStaticGhaph()
// {
// 	HRESULT hr = E_FAIL;
// 	if(!m_pGraphBuilder || !m_pFileSrcF)
// 	   return hr;
// 
//     hr=NukeDownstream(m_pGraphBuilder,m_pFileSrcF);
//     if (FAILED(hr))
// 	{
// 		return hr;
// 	}
//     hr=RemoveFilterGraph(m_pGraphBuilder);
// 	if (FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	return hr;
// }
