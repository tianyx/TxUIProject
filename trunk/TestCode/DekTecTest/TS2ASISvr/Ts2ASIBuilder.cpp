#include "StdAfx.h"
#include "Ts2ASIBuilder.h"
#include "GraphManager.h"
#include "moduleconfig.h"
#include "demuxpush_mp2_mc.h"
#include "net_source_mc.h"
#include "net_client_defines.h "
#include "mux_mp2_mc.h"
#include <string>
#include "FGlobal.h"


#define  _OUTGRAPHDBUG

using namespace std;
#define MUSTBESOK(hr) if(hr != S_OK){ASSERT(FALSE);return hr;}
#define MUSTNOTNULL(ptr) if(ptr == NULL){ASSERT(FALSE);return E_FAIL;}


//C66A9EE1-4025-48A5-8380-C3ACB42697CB
// {C7668960-1270-447f-8FB7-BB52DF687028}
static const GUID CLSID_ZQTS2ASIRender = 
{ 0xC66A9EE1, 0x4025, 0x48A5, {0x83, 0x80, 0xC3, 0xAC, 0xB4, 0x26, 0x97, 0xCB}};

// DEFINE_GUID(CLSID_ZQTS2ASIRender, 
// 			0xC66A9EE1, 0x4025, 0x48A5, 0x83, 0x80, 0xC3, 0xAC, 0xB4, 0x26, 0x97, 0xCB);
//{668EE184-FD2D-4C72-8E79-439A35B438DE}
// DEFINE_GUID(CLSID_MainConceptMPGPushDemultiplex, 
// 			0x668EE184, 0xFD2D, 0x4C72, 0x8E, 0x79, 0x43, 0x9A, 0x35, 0xB4, 0x38, 0xDE);
static const GUID CLSID_MainConceptMPGPushDemultiplex = 
{0x668EE184, 0xFD2D, 0x4C72, {0x8E, 0x79, 0x43, 0x9A, 0x35, 0xB4, 0x38, 0xDE}};

//{CFD28198-115C-4606-BD64-27A7E0051D2A}
// DEFINE_GUID(CLSID_MainConceptMPGMultiplex, 
// 			0xCFD28198, 0x115C, 0x4606, 0xBD, 0x64, 0x27, 0xA7, 0xE0, 0x05, 0x1D, 0x2A);
static const GUID CLSID_MainConceptMPGMultiplex =
{0xCFD28198, 0x115C, 0x4606, {0xBD, 0x64, 0x27, 0xA7, 0xE0, 0x05, 0x1D, 0x2A}} ;

//DD01EDDB-8942-4632-A3FB-4E30A59FF8F2
static const GUID CLSID_MainConceptMPGMultiplexDemo =
{0xDD01EDDB, 0x8942, 0x4632, {0xA3, 0xFB, 0x4E, 0x30, 0xA5, 0x9F, 0xF8, 0xF2}} ;


//{A5B3DFAF-A6EF-4A33-B506-3D0184CF1E1F}
// DEFINE_GUID(CLSID_MainConceptNetSource, 
// 			0xA5B3DFAF, 0xA6EF, 0x4A33, 0xB5, 0x06, 0x3D, 0x01, 0x84, 0xCF, 0x1E,0x1F);
// 
static const GUID CLSID_MainConceptNetSource =
{0xA5B3DFAF, 0xA6EF, 0x4A33,{ 0xB5, 0x06, 0x3D, 0x01, 0x84, 0xCF, 0x1E,0x1F}} ;

//{8596E5F0-0DA5-11D0-BD21-00A0C911CE86}
static const GUID CLSID_IDSFileWriter =
{0x8596E5F0, 0x0DA5, 0x11D0, {0xBD, 0x21, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86}};


//interface for set asi render
static const GUID IID_IASIDevPreConfig =
{0xa12665e7, 0xa873, 0x4b18, {0x81, 0x8e, 0xcf, 0xbd, 0xed, 0xaa, 0xa7, 0xf9}};


// DEFINE_GUID(CLSID_, 
// 			0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x);


CTs2ASIBuilder::CTs2ASIBuilder(void)
{
	m_bInited = FALSE;
	m_nOutToType = OUTTO_FILE;
	CTime tm(time(NULL));
	m_strOutFile.Format(TEXT("f:\\vout\\%02d-%02d-%02d.ts"), tm.GetHour(),tm.GetMinute(), tm.GetSecond());
}

CTs2ASIBuilder::~CTs2ASIBuilder(void)
{
}

int CTs2ASIBuilder::CreateFilter(REFCLSID clsidIn, int nmapIdIn, LPCTSTR szNameIn)
{
	if (m_Filters[nmapIdIn] != NULL)
	{
		return TERR_EXIST;
	}
	if (FAILED(m_gfBuilder.AddLocalFilter(clsidIn,&m_Filters[nmapIdIn],szNameIn)) &&
		FAILED(m_gfBuilder.AddFilter(clsidIn,&m_Filters[nmapIdIn],szNameIn)))
	{
		return E_FAIL;
	}
	return S_OK;

}

int CTs2ASIBuilder::InitGraph(ST_DEVPARAM& devParam)
{
	if (m_bInited)
	{
		return TRT_READYINITED;
	}
	if (!m_gfBuilder.MakeGraph())
	{
		return TERR_GRAPH;
	}

	//build filter
	std::vector<ST_TSADDR>& vtsAddr = g_GlobalInfo.vtsAddr;
	if (vtsAddr.size() == 0)
	{
		return TERR_NOSOURCE;
	}
	

	CComVariant	var;

	HRESULT hr = S_OK;
	//---multiplexer
	hr = CreateFilter(CLSID_MainConceptMPGMultiplexDemo, IDF_MULTIPLEXBASE, TEXT("multiplexer"));
	MUSTBESOK(hr);

	//set param
	IModuleConfig* pMcConfigMux = NULL;
	IBaseFilter* pFilterMux = m_Filters[IDF_MULTIPLEXBASE];
	hr = pFilterMux->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigMux);
	if (FAILED(hr) || pMcConfigMux == NULL)
	{
		return TERR_NOINTERFACE;
	}


	//MCMux_OutputStreamType
	var.ClearToZero();
	var.vt = VT_I4;
	var.intVal = 2; 
	hr = pMcConfigMux->SetValue(&MCMux_OutputStreamType, &var);
	MUSTBESOK(hr);

	//MCMux_AutoCreatePins
	var.ClearToZero();
	var.vt = VT_I4;
	var.intVal = 0; 
	//hr = pMcConfigMux->SetValue(&MCMux_AutoCreatePins, &var);
	MUSTBESOK(hr);
	//MCMux_PaddingControl //自动填包，禁掉后可调节速度MCMux_RequiredBitrate
	var.ClearToZero();
	var.vt = VT_I4;
	var.intVal = 0; 
	//hr = pMcConfigMux->SetValue(&MCMux_PaddingControl, &var);
	MUSTBESOK(hr);

	//MCMux_RequiredBitrate , only available when MCMux_PaddingControl == false, 0 for auto detect
	if (m_demuxParam.nRequiredBitrate != 0)
	{
		var.ClearToZero();
		var.vt = VT_I4;
		var.intVal = m_demuxParam.nRequiredBitrate; 
		hr = pMcConfigMux->SetValue(&MCMux_RequiredBitrate, &var);
		MUSTBESOK(hr);
	}


	//MCMux_TS_Outpack_Size 累积多少包后发送 default = 5
	//MCMux_OptPacking --maybe useful
	
	//---net source render and push demultiplexer
	for (size_t i = 0; i < vtsAddr.size(); ++i)
	{
		ST_TSADDR tsAddrTmp = vtsAddr[i];
		
		CString strName;
		strName.Format(TEXT("net src %d"), i);
		hr = CreateFilter(CLSID_MainConceptNetSource, IDF_NETSOURCEIDBASE+i, strName);
		MUSTBESOK(hr);


		strName.Format(TEXT("push demult %d"), i);
		hr = CreateFilter(CLSID_MainConceptMPGPushDemultiplex, IDF_DEMULTIPLEXBASE+i, strName);
		MUSTBESOK(hr);

		//set param for src 
		IModuleConfig* pMcConfigSrc = NULL;
		IBaseFilter* pFilterSrc = m_Filters[IDF_NETSOURCEIDBASE+i];
		hr = pFilterSrc->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigSrc);
		if (FAILED(hr) || pMcConfigSrc == NULL)
		{
			return TERR_NOINTERFACE;
		}
		// set param....
		//MCNS_EngineInitMode
		var.ClearToZero();
		var.vt = VT_UINT;
		var.intVal = EngineSyncInit; //同步操作
		hr =pMcConfigSrc->SetValue(&MCNS_EngineInitMode, &var);
		MUSTBESOK(hr);


		var.ClearToZero();
		var.vt = VT_UINT;
		var.uintVal = EBufferingModeLowDelay; //缓冲模式
 		hr = pMcConfigSrc->SetValue(&MCNS_BufferingMode, &var);
		MUSTBESOK(hr);

		

		//MCNS_TransportType
		var.ClearToZero();
		var.vt = VT_UINT;
		var.uintVal = MainConcept::NetworkStreaming::Mp2tAvpUdpMulticastMode;
 		hr = pMcConfigSrc->SetValue(&MCNS_TransportType, &var);
		MUSTBESOK(hr);

		//MCNS_EnableAutoQualitySwithing
		var.ClearToZero();
		var.vt = VT_BOOL;
		var.boolVal = FALSE;
//  		hr = pMcConfigSrc->SetValue(&MCNS_EnableAutoQualitySwithing, &var);
		MUSTBESOK(hr);

		//MCNS_URI address of udp mc
		var.ClearToZero();
		var.vt = VT_BSTR;
		CString strUrl;
		strUrl.Format(TEXT("udp://%s:%d"), tsAddrTmp.ip.c_str(), tsAddrTmp.nPort);
		var.bstrVal = _bstr_t(strUrl);
		hr = pMcConfigSrc->SetValue(&MCNS_URI, &var);

		var.ClearToZero();
		var.vt = VT_UINT;
		var.intVal = 1;
		hr=pMcConfigSrc->CommitChanges(&var);
		CComVariant vout;
		vout.ClearToZero();
		vout.vt = VT_BSTR;
		hr = pMcConfigSrc->GetValue(&MCNS_URI, &vout);
		MUSTBESOK(hr);

		//MCNS_EngineReady must check before connect pin
		var.ClearToZero();
		var.vt = VT_BOOL;
		var.uintVal = 0;
		hr = pMcConfigSrc->GetValue(&MCNS_EngineReady, &var);
		MUSTBESOK(hr);

		//MCNS_IsLiveUdpDataAvailable if udp data is available

		SAFE_RELEASE(pMcConfigSrc);

		//////////////////////////////////////////////////////////////////////////

		IModuleConfig* pMcConfigDemux = NULL;
		IBaseFilter* pFilterDemux = m_Filters[IDF_DEMULTIPLEXBASE+i];
		hr = pFilterDemux->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigDemux);
		if (FAILED(hr) || pMcConfigDemux == NULL)
		{
			return TERR_NOINTERFACE;
		}
		// set param....
		var.ClearToZero();
		var.vt = VT_BOOL;
		var.boolVal = FALSE;

		SAFE_RELEASE(pMcConfigDemux);

		//connect src -- demux filters
		hr =m_gfBuilder.ConnectFilter(pFilterSrc,pFilterDemux);
		MUSTBESOK(hr);

		//connect demux --mux
		//MCMux_GetNextProgram
		//MCMux_CreateProgram
		//MCMux_GetNextStream  pins
		//MCMux_CreateStream
		//MCMux_MoveStreamTo
		int nPinCountToCreate = 2;
		CComVariant valUkPgm;
		valUkPgm.ChangeType(VT_UNKNOWN);
		valUkPgm.punkVal = NULL;

		if (i == 0)
		{
			nPinCountToCreate = 1;
			//not create program , only create one pin
			hr = pMcConfigMux->GetValue(&MCMux_GetNextProgram, &valUkPgm);
			MUSTBESOK(hr);
		}
		else
		{
			//create new program
			hr = pMcConfigMux->SetValue(&MCMux_CreateProgram, &valUkPgm);
			MUSTBESOK(hr);

// 			hr = pMcConfigMux->GetValue(&MCMux_GetNextProgram, &valUkPgm);
// 			MUSTSURE(hr);
		}
		
		IModuleConfig* pMcConfigProgram = NULL;
		hr = valUkPgm.punkVal->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigProgram);
		if (FAILED(hr) || pMcConfigProgram == NULL)
		{
			return TERR_NOINTERFACE;
		}

		for (int iPs = 0; iPs < nPinCountToCreate; ++iPs)
		{
			CComVariant valUkStream;
			valUkStream.ClearToZero();
			valUkStream = valUkPgm;
			hr = pMcConfigProgram->SetValue(&MCMux_CreateStream, &valUkStream);
			MUSTBESOK(hr);

// 			//move it to program
// 			hr = pMcConfigMux->SetValue(&MCMux_MoveStreamTo, &valUkPgm);
// 			MUSTSURE(hr);


// 			IModuleConfig* pMcConfigStream = NULL;
// 			hr = valUkStream.punkVal->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigStream);
// 			if (FAILED(hr) || pMcConfigProgram == NULL)
// 			{
// 				return TERR_NOINTERFACE;
// 			}

			//SAFE_RELEASE(pMcConfigStream);
		}
		SAFE_RELEASE(pMcConfigProgram);

	}
	SAFE_RELEASE(pMcConfigMux);

	//now connect demux--  mux
	for (size_t i = 0; i < vtsAddr.size(); ++i)
	{
		//connect src -- demux filters
		hr =m_gfBuilder.ConnectFilter(m_Filters[IDF_DEMULTIPLEXBASE + i],pFilterMux);
		MUSTBESOK(hr);
	}



	IBaseFilter* pFilterOut = NULL;
	if (m_nOutToType == OUTTO_FILE)
	{
		hr = CreateFilter(CLSID_IDSFileWriter, IDF_DSFILEWRITER, TEXT("ds file writer"));
		MUSTBESOK(hr);
		pFilterOut = m_Filters[IDF_DSFILEWRITER];
		IFileSinkFilter2* pISink = NULL;
		hr = pFilterOut->QueryInterface(IID_IFileSinkFilter2, (void **)&pISink);
		if (FAILED(hr) || pISink == NULL)
		{
			return TERR_NOINTERFACE;
		}
		CComVariant vOutFName;
		vOutFName.vt = VT_BSTR;
		vOutFName.bstrVal = _bstr_t(m_strOutFile);
		hr =pISink->SetFileName(vOutFName.bstrVal, NULL);
		SAFE_RELEASE(pISink);
		MUSTBESOK(hr);
		
	}
	else if (m_nOutToType == OUTTO_ASI)
	{
		//---asi render
		hr = CreateFilter(CLSID_ZQTS2ASIRender, IDF_TS2ASIRENDER, TEXT("ts->asi render"));
		MUSTBESOK(hr);
		pFilterOut = m_Filters[IDF_TS2ASIRENDER];
		//query setting interface;
		IASIDevPreConfig* pASIConfig = NULL;
		hr = pFilterOut->QueryInterface(IID_IASIDevPreConfig, (void **)&pASIConfig);
		if (FAILED(hr) || pASIConfig == NULL)
		{
			return TERR_NOINTERFACE;
		}

		//set param
		hr = pASIConfig->init_DevPreConfig(&devParam);
		MUSTBESOK(hr);
		SAFE_RELEASE(pASIConfig);
	}
	MUSTNOTNULL(pFilterOut);
	
	hr =m_gfBuilder.ConnectFilter(pFilterMux, pFilterOut);
	MUSTBESOK(hr);
	
#ifdef _OUTGRAPHDBUG
	m_gfBuilder.savegrf();
#endif // _OUTGRAPHDBUG
	m_bInited = TRUE;
	return S_OK;
}

int CTs2ASIBuilder::UnInitGraph()
{
	if (!m_bInited)
	{
		return S_OK;
	}
	HRESULT hr = m_gfBuilder.Stop();
	ASSERT(hr);
	hr =m_gfBuilder.ReleaseGraph();
	if (hr == S_OK)
	{
		m_bInited = FALSE;
	}
	return hr;
}

int CTs2ASIBuilder::GetSrcRenderBps( VECSRCOUTBPS& vOut )
{
	//MCNS_NetQualityInfoStorage;
	HRESULT hr = S_OK;
	POSITION  pos = m_Filters.GetStartPosition();
	WORD key;
	void* vTmp;
	while(pos)
	{
		int nTmpRate = -1;
		m_Filters.GetNextAssoc(pos, key, vTmp);
		if (key < IDF_NETSOURCEIDBASE 
			||key > IDF_NETSOURCEIDBASEMAX)
		{
			continue;
		}
		IModuleConfig* pMcConfigSrc = NULL;
		IBaseFilter* pFilterSrc = m_Filters[key];
		hr = pFilterSrc->QueryInterface(IID_IModuleConfig, (void **)&pMcConfigSrc);
		if (SUCCEEDED(hr) && pMcConfigSrc != NULL)
		{
			byte szBuff[10240];
			ZeroMemory(szBuff, sizeof(szBuff));
			_tagNetQualityInfoStorage* pQsInfo = (_tagNetQualityInfoStorage*)szBuff;
			pQsInfo->_DataSize = sizeof(szBuff);
			CComVariant var;
			var.ClearToZero();
			var.vt = VT_UINT_PTR;
			var.puintVal = NULL; //同步操作
			hr =pMcConfigSrc->GetValue(&MCNS_NetQualityInfoStorage, &var);
			pQsInfo =(_tagNetQualityInfoStorage*)var.puintVal;
			if (hr == S_OK && pQsInfo->_pNetQualityInfoArray)
			{
				nTmpRate = (pQsInfo->_pNetQualityInfoArray)[0]._RecvRate;
			}
		SAFE_RELEASE(pMcConfigSrc);
		}
		vOut.push_back(nTmpRate);
	}

	return hr;
}

int CTs2ASIBuilder::SetASIAutoAdjustOutBps( int bEnable )
{
	if (!m_bInited)
	{
		return TERR_NOTINIT;
	}

	HRESULT hr = S_OK;
	IBaseFilter* pFilterOut = m_Filters[IDF_TS2ASIRENDER];
	if (pFilterOut)
	{
		hr = CreateFilter(CLSID_ZQTS2ASIRender, IDF_TS2ASIRENDER, TEXT("ts->asi render"));
		MUSTBESOK(hr);
		pFilterOut = m_Filters[IDF_TS2ASIRENDER];
		//query setting interface;
		IASIDevPreConfig* pASIConfig = NULL;
		hr = pFilterOut->QueryInterface(IID_IASIDevPreConfig, (void **)&pASIConfig);
		if (FAILED(hr) || pASIConfig == NULL)
		{
			return TERR_NOINTERFACE;
		}

		//set param
		CComVariant var;
		var.vt = VT_INT;
		var.intVal = bEnable;
		hr = pASIConfig->set_value(&CLSID_AutoAdjustOutputBitrate, &var);
		SAFE_RELEASE(pASIConfig);
	}

	return hr;
}

int CTs2ASIBuilder::SetASIOutBps( int nBpsIn )
{
	if (!m_bInited)
	{
		return TERR_NOTINIT;
	}


	HRESULT hr = S_OK;
	IBaseFilter* pFilterOut = m_Filters[IDF_TS2ASIRENDER];
	if (pFilterOut)
	{
		hr = CreateFilter(CLSID_ZQTS2ASIRender, IDF_TS2ASIRENDER, TEXT("ts->asi render"));
		MUSTBESOK(hr);
		pFilterOut = m_Filters[IDF_TS2ASIRENDER];
		//query setting interface;
		IASIDevPreConfig* pASIConfig = NULL;
		hr = pFilterOut->QueryInterface(IID_IASIDevPreConfig, (void **)&pASIConfig);
		if (FAILED(hr) || pASIConfig == NULL)
		{
			return TERR_NOINTERFACE;
		}

		//set param
		CComVariant var;
		var.vt = VT_INT;
		var.intVal = nBpsIn;
		hr = pASIConfig->set_value(&CLSID_ASIOutputBitrate, &var);
		SAFE_RELEASE(pASIConfig);
	}

	return hr;
}

