// BaseGraph_LXF.cpp: implementation of the CBaseGraph_LXF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph_LXF.h"
#include "GuidRef.h"
// #include "ModuleConfig/ModuleConfig.h"
// //#include "ModuleConfig/common_mc.h"
// #include "ModuleConfig/mpeg2dec_mc.h"

// DEFINE_GUID(IID_IModuleConfig, 0x486F726E, 0x4D43, 0x49b9, 0x8A, 0x0C, 0xC2, 0x2A, 0x2B, 0x05, 0x24, 0xE8);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseGraph_LXF::CBaseGraph_LXF()
{
  OutputDebugString("WWW: CBaseGraph_MXF::CBaseGraph_LXF()");
}

CBaseGraph_LXF::~CBaseGraph_LXF()
{
  OutputDebugString("WWW: CBaseGraph_MXF::~CBaseGraph_LXF()");
}

HRESULT CBaseGraph_LXF::ConnectFiltersInGraph()
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
// 	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pColorSpaceFilter);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
	hr = ConnectFilter(m_pGraphBuilder,m_pVDecoder,m_pSampGrabberF);
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
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF);
	return hr;

}

HRESULT CBaseGraph_LXF::AddFilters2Graph()
{
	
   	TRACE("<<<into CBaseGraph AddFilters2Graph!\n");
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
	Value.ulVal=1;   //原来是3,zhouliang 修改为1
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
	VariantClear(&Value);

	//zibj 
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_LXF_MPEG2,&m_pParserFilter,"Leitch Mpeg2Parser") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_LXF_MPEG2,&m_pParserFilter,"Leitch Mpeg2Parser");
//	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"Leitch Mpeg2Parser",&m_pParserFilter);
	if (hr != S_OK)
	{
		return hr;
	}
	// 创建颜色空间转换
// 	hr = AddFilter2GraphByFriendlyName(m_pGraphBuilder,"MainConcept Color Space Converter",&m_pColorSpaceFilter);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
	// 创建Sample Grabber
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber") && 
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber")&&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pAudioGrabberF,"Audio Grabber");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render")&&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
	return hr;


}
HRESULT CBaseGraph_LXF::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph_LXF builder!\n");
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