// MOVGraph.cpp: implementation of the CMOVGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_MOV.h"
#include <objbase.h>
#include "GuidRef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseGraph_MOV::CBaseGraph_MOV()
{
    OutputDebugString("WWW: CBaseGraph_MOV::CBaseGraph_MOV()");
}

CBaseGraph_MOV::~CBaseGraph_MOV()
{
    OutputDebugString("WWW: CBaseGraph_MOV::~CBaseGraph_MOV()");
}
HRESULT CBaseGraph_MOV::BuildGrabberGraph()
{
	HRESULT hr = S_OK;

	OutputDebugString("\n add m_pFileSrcF\n");
	
	hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
	if (hr != S_OK)
	{
		return hr;
	}
	//��ӽ�����
	OutputDebugString("\n MainConcept MPEG-2 Video Decoder\n");

	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_EM2VD,&m_pVDecoder,"MainConcept MPEG-2 Video Decoder") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_EM2VD,&m_pVDecoder,"MainConcept MPEG-2 Video Decoder");

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
	OutputDebugString("\n Founder MXF Demultiplexor\n");
	//zibj
	//zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_LAV_DEMUX,&m_pParserFilter,"Founder MXF Demultiplexor") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_LAV_DEMUX,&m_pParserFilter,"Founder MXF Demultiplexor") ;

	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter);
	OutputDebugString("\n����m_pFileSrcF--------------m_pParserFilter����\n");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pVDecoder,TRUE);
   if (hr != S_OK)//Mpeg2�������Ӳ��ɹ�
   {
	   m_pVDecoder->Release();//ǰ��Ľӿ�
	   m_pGraphBuilder->RemoveFilter(m_pVDecoder);
	   //zibj
	   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_DVVideoCodec,&m_pVDecoder,"DV Video Decoder") &&
			   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_DVVideoCodec,&m_pVDecoder,"DV Video Decoder");
	  
	   if (hr != S_OK)
	   {
		  return hr;
	   }
	   hr = ConnectFilter(m_pGraphBuilder,m_pParserFilter,m_pVDecoder,TRUE);//��������
	   if (hr != S_OK)
	   {
		   return hr;
	   }
   }
   
   // ����Sample Grabber
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber")&&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber");

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

  //������Ƶ���
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