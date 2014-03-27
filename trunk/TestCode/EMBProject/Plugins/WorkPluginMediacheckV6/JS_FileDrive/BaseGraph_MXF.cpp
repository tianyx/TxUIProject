// MXFGraph.cpp: implementation of the CMXFGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_MXF.h"
#include <objbase.h>
#include <initguid.h>
#include "GuidRef.h"
// #include "ModuleConfig/ModuleConfig.h"
// //#include "ModuleConfig/common_mc.h"
// #include "ModuleConfig/mpeg2dec_mc.h"
//DEFINE_GUID(IID_IModuleConfig, 0x486F726E, 0x4D43, 0x49b9, 0x8A, 0x0C, 0xC2, 0x2A, 0x2B, 0x05, 0x24, 0xE8);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseGraph_MXF::CBaseGraph_MXF()
{
    OutputDebugString("WWW: CBaseGraph_MXF::CBaseGraph_MXF()");
}

CBaseGraph_MXF::~CBaseGraph_MXF()
{
    OutputDebugString("WWW: CBaseGraph_MXF::~CBaseGraph_MXF()");
}
HRESULT CBaseGraph_MXF::BuildGrabberGraph()
{
	HRESULT hr = S_OK;
 //	if(!m_pGraphBuilder)
 //		return hr;
	OutputDebugString("\n add m_pFileSrcF\n");
	hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
	if (hr != S_OK)
	{
		return hr;
	}
	//添加解码器
	OutputDebugString("\n MainConcept MPEG-2 Video Decoder\n");

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
	if (hr == S_OK)
	{
		Value.vt=VT_UI4;
		Value.ulVal=3;
		pIModuleConfig->SetValue(&EM2VD_Quality,&Value);
		Value.ulVal=0;
		pIModuleConfig->SetValue(&EM2VD_HardwareAcceleration,&Value);
		Value.ulVal=1;
		pIModuleConfig->SetValue(&EM2VD_SynchronizationMode,&Value);
		Value.ulVal=0;
		pIModuleConfig->SetValue(&EM2VD_FormatVideoInfo,&Value);
		// 	Value.ulVal=4;
		// 	pIModuleConfig->SetValue(&EM2VD_Deinterlace,&Value);
		// 	Value.ulVal=0;
		// 	pIModuleConfig->SetValue(&EM2VD_DeinterlaceCondition,&Value);
		// 	Value.ulVal=0;
		// 	pIModuleConfig->SetValue(&EM2VD_FieldsReordering,&Value);
		// 	Value.ulVal=0;
		// 	pIModuleConfig->SetValue(&EM2VD_FieldsReorderingCondition,&Value);
		
		pIModuleConfig->CommitChanges(&Value);
	    pIModuleConfig->Release();
	}
	VariantClear(&Value);
	OutputDebugString("\n Mainconcept MXF Demultiplexor\n");
	//zibj
	//zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConceptMXFDemuxer,&m_pParserFilter,"Founder MXF Demultiplexor") &&
	     AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConceptMXFDemuxer,&m_pParserFilter,"Founder MXF Demultiplexor") ;
	//hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_FounderMXFDemultiplexer,&m_pParserFilter,"Founder MXF Demultiplexor") &&
	//   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_FounderMXFDemultiplexer,&m_pParserFilter,"Founder MXF Demultiplexor") ;
//	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"Founder MXF Demultiplexor",&m_pParserFilter);
//hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MOG Solutions MXF Splitter filter",&m_pParserFilter);
//hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"Snell & Wilcox MXF Parser",&m_pParserFilter);
//hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept MPEG Demultiplexor",&m_pParserFilter);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter);
	OutputDebugString("\n连接m_pFileSrcF--------------m_pParserFilter结束\n");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pVDecoder,TRUE);
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
   // 创建颜色空间转换
//    hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept Color Space Converter",&m_pColorSpaceFilter);
//    if (hr != S_OK)
//    {
// 	   return hr;
//    }
//    hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pColorSpaceFilter);
//    if (hr != S_OK)
//    {
// 	   return hr;
//    }
   
   // 创建Sample Grabber
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber")&&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber");
  // hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"SampleGrabber Example",&m_pSampGrabberF);
   if (hr != S_OK)
   {
	   return hr;
   }
   hr = SetVideoGrabber();
   if (hr != S_OK)
   {
	   return hr;
   }
   hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pSampGrabberF);
   if (hr != S_OK)
   {
	   return hr;
   }
   
   ///跟踪Filter/////////////
   // IBaseFilter *pFilterTracer = NULL;
   // hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"Filter Tracer",&pFilterTracer);
   // if (hr != S_OK)
   // {
   //	 return hr;
   // }
   // hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,pFilterTracer);
   
   /////////////////
   
   // hr = AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_VideoRenderer,&m_pVNullRenderF,"NULL Video Render");
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") && 
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
   if (hr != S_OK)
   {
	   return hr;
   }
   hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF);
   if (hr != S_OK)
   {
	   return hr;
   }

  //增加音频检测
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber") && 
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber");
   if (hr != S_OK)
   {
	   return hr;
   }
   hr = SetAudioGrabber();
   if (hr != S_OK)
   {
	   return hr;
   }
   hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pAudioGrabberF);
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
   hr = ConnectFilter(m_pGraphBuilder,m_pAudioGrabberF,m_pANullRenderF);
   return hr;

}