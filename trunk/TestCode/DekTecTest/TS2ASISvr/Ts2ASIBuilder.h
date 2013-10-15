#pragma once

#include "GraphManager.h"
#include "GlobalDef.h"
#include <map>
#include <vector>
#include "..\ZQASIRenderFilter\IASIDevPreConfig.h"

 EXTERN_C const GUID FAR CLSID_ZQTS2ASIRender;		
 EXTERN_C const GUID FAR CLSID_MainConceptMPGPushDemultiplex;	
 EXTERN_C const GUID FAR CLSID_MainConceptMPGMultiplex;		
 EXTERN_C const GUID FAR CLSID_MainConceptNetSource;	

 typedef std::vector<int> VECSRCOUTBPS;

 enum ENUM_TS2ASIERR
 {
	 TERR_GRAPH = -1000,
	 TERR_EXIST,
	 TERR_NOSOURCE,
	 TERR_NOINTERFACE,
	 TERR_NOTINIT,
	 TERR_OK = 0,
	 TRT_READYINITED

 };

enum ENUM_IDMAPFILTER
{
	IDF_DSFILEWRITER = 8,
	IDF_TS2ASIRENDER = 9,
	IDF_MULTIPLEXBASE = 10,
	IDF_NETSOURCEIDBASE = 100,
	IDF_NETSOURCEIDBASEMAX= 499,
	IDF_DEMULTIPLEXBASE = 500,
	IDF_DEMULTIPLEXBASEMAX = 1000,

};

struct ST_PARAM_FORDEMUX
{
	int nRequiredBitrate;
	ST_PARAM_FORDEMUX()
	{
		nRequiredBitrate = 0;//2*1024*1024;
	}
};

enum TSENUM_OUTTOTYPE
{
	OUTTO_ASI = 0,
	OUTTO_FILE
};

class CTs2ASIBuilder
{
public:
	CTs2ASIBuilder(void);
	~CTs2ASIBuilder(void);

	CGraphBase m_gfBuilder;
	CIndexedFilter m_Filters;

	int m_nOutToType;
	CString m_strOutFile;

	BOOL m_bInited;

	ST_PARAM_FORDEMUX m_demuxParam; //for demutiplexer
	int CreateFilter(REFCLSID clsidIn, int nmapIdIn, LPCTSTR szNameIn);

	int InitGraph(ST_DEVPARAM& devParam);
	int UnInitGraph();

	int GetSrcRenderBps(VECSRCOUTBPS& vOut);
	int SetASIAutoAdjustOutBps(int bEnable);
	int SetASIOutBps(int nBpsIn);
};
