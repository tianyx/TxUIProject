// GraphManager.h: interface for the CEncoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENCODER_H__AA7D6A89_CA1B_46FB_A29D_466645476532__INCLUDED_)
#define AFX_ENCODER_H__AA7D6A89_CA1B_46FB_A29D_466645476532__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define NO_AUDIO	999 

#include "fmUtility.h"
#include <DShow.h>
#include <atlbase.h>
#include <afxtempl.h>
#include <vector>

extern const GUID CLSID_MXFParser;					//0x025BE2E4, 0x1787, 0x4DA4, 0xA5, 0x85, 0xC5, 0xB2, 0xB9, 0xEE, 0xB5, 0x7C);
extern const GUID CLSID_LXFParser;					//0x6B4FABAA, 0x47CC, 0x41A6, 0xAA, 0x3B, 0xD8, 0xF1, 0xC3, 0xB4, 0x55, 0xD7);
extern const GUID CLSID_MCMpgDmx;					//0x136DCBF5, 0x3874, 0x4B70, 0xAE, 0x3E, 0x15, 0x99, 0x7D, 0x63, 0x34, 0xF7);
extern const GUID CLSID_MainConcept_MPEG_Decoder;	//0xBC4EB321, 0x771F, 0x4E9F, 0xAF, 0x67, 0x37, 0xC6, 0x31, 0xEC, 0xA1, 0x06);
extern const GUID CLSID_MainConcept_MP4_Decoder;
extern const GUID CLSID_MainConcept_infTee;			//0x08C0DE0E, 0xd048, 0x4d36, 0x84, 0x0e, 0x2b, 0x21, 0x35, 0xf7, 0x9a, 0x21);
extern const GUID CLSID_MainConcept_ColorSpace;		//0x272D77A0, 0xA852, 0x4851, 0xAD, 0xA4, 0x90, 0x91, 0xFE, 0xAD, 0x4C, 0x86);
extern const GUID CLSID_Bluefish444_Video_Render;	//0xD79F359D, 0x8eb2, 0x4d38, 0xa1, 0x2d, 0x07, 0x72, 0x20, 0xee, 0x36, 0x99);
extern const GUID CLSID_AviFileSource;				//0xd3588ab0, 0x0781, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
extern const GUID CLSID_MatroxRiffReader;			//0x74A541E1, 0x34B0, 0x11CF, 0x82, 0xDA, 0x00, 0x20, 0xAF, 0x11, 0xC0, 0x10);
extern const GUID CLSID_FounderAVIDemultiplexer;	//0x02b3384f, 0xfacf, 0x491e, 0x99, 0x19, 0xef, 0x22, 0x02, 0xcf, 0xbc, 0xcc);
extern const GUID CLSID_Bluefish444_Audio_Render;	//0xF27A8BE3, 0x9177, 0x4F60, 0x95, 0x0A, 0x16, 0xC8, 0xCA, 0x42, 0x50, 0xE4);
extern const GUID CLSID_DecklinkVideoRender; 		//0xCEB13CC8, 0x3591, 0x45A5, 0xBA, 0x0F, 0x20, 0xE9, 0xA1, 0xD7, 0x2F, 0x76);
extern const GUID CLSID_DecklinkAudioRender; 		//0x19FA8CC3, 0x56CE, 0x46AB, 0x82, 0x5D, 0x5C, 0xE1, 0xA3, 0x9B, 0x13, 0x7A);
extern const GUID CLSID_MCCSC;						//0x272D77A0, 0xA852, 0x4851, 0xAD, 0xA4, 0x90, 0x91, 0xFE, 0xAD, 0x4C, 0x86);
extern const GUID CLSID_WaveParser;		
extern const GUID CLSID_MCL2AD;		
extern const GUID CLSID_LegacyAmFilterCategory;
extern const GUID CLSID_SystemDeviceEnum;
extern const GUID CLSID_MXFMuxer;					//0x24FF6532, 0x6BE7, 0x4151, 0xA5, 0x63, 0x4E, 0x57, 0xC1, 0x45, 0xF2, 0xB5);
extern const GUID CLSID_MP4Demux;//class DECLSPEC_UUID("E7022EDA-D953-4018-AE75-89036DEA0BB7")
extern const GUID CLSID_MP4Mux;//DECLSPEC_UUID("F5E52B44-DFC9-4c25-8CB3-5D5BB7515172")
extern const GUID CLSID_MCMpgMux;
extern const GUID CLSID_MCNetRender;
extern const GUID CLSID_TSSplitter;
extern const GUID CLSID_MCNetRender1;
extern const GUID CLSID_H264VideoEncoder;
extern const GUID CLSID_MCM2Enc;
extern const GUID CLSID_FounderTSPCRAdjust;

//---------------------------------------------Founder Net Render----------------------------------------------------------
// {dc26c15a-ce7b-48df-8818-83e74e8135de} 
extern const GUID CLSID_FounderNetRendererFilter;

// {64a178ff-3dc8-49d6-8111-952582fb31e4} 
extern const GUID CLSID_FounderNetRendererFilterProperty;
//---------------------------------------------NewMCNewRender Params-------------------------------------------------------
//extern const GUID MCNR_StreamingProtocol;
//extern const GUID MCNR_StreamingPort;
//extern const GUID MCNR_DestinationAddress;

// {6E726F48-2C00-4493-AEE8-2615EBC11188}
extern const GUID MCNR_OriginNetworkType;
// {6E726F48-2C01-4493-AEE8-2615EBC11188}
extern const GUID MCNR_OriginAddressType;
// {6E726F48-2C02-4493-AEE8-2615EBC11188}
extern const GUID MCNR_ConnectionNetworkType;
// {6E726F48-2C03-4493-AEE8-2615EBC11188}
extern const GUID MCNR_ConnectionAddressType;
// {6E726F48-2C04-4493-AEE8-2615EBC11188}
extern const GUID MCNR_StreamingProtocol;
// {6E726F48-2C05-4493-AEE8-2615EBC11188}
extern const GUID MCNR_StreamingType;
// {6E726F48-2C06-4493-AEE8-2615EBC11188}
extern const GUID MCNR_StreamingPort;
// {6E726F48-2C07-4493-AEE8-2615EBC11188}
extern const GUID MCNR_ConnectionTTL;
// {6E726F48-2C08-4493-AEE8-2615EBC11188}
extern const GUID MCNR_BandWidthValue;
// {6E726F48-2C09-4493-AEE8-2615EBC11188}
extern const GUID MCNR_OriginSessionID;
// {6E726F48-2C0A-4493-AEE8-2615EBC11188}
extern const GUID MCNR_OutgoingNetworkInterface;
// {6E726F48-2C0B-4493-AEE8-2615EBC11188}
extern const GUID MCNR_DestinationAddress;
// {6E726F48-2C0C-4493-AEE8-2615EBC11188}
extern const GUID MCNR_EmailField;
// {6E726F48-2C0D-4493-AEE8-2615EBC11188}
extern const GUID MCNR_PhoneField;
// {6E726F48-2C0E-4493-AEE8-2615EBC11188}
extern const GUID MCNR_BandWidthModifier;
// {6E726F48-2C0F-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SessionName;
// {6E726F48-2C10-4493-AEE8-2615EBC11188}
extern const GUID MCNR_OriginCreatorName;
// {6E726F48-2C11-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SessionInfo;
// {6E726F48-2C12-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SessionDescriptionURI;
// {6E726F48-2C14-4493-AEE8-2615EBC11188}
extern const GUID MCNR_OriginSessionVersion;
// {6E726F48-2C15-4493-AEE8-2615EBC11188}
extern const GUID MCNR_MaxTransferUnit;
// {6E726F48-2C16-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SapMulticastGroup;
// {6E726F48-2C18-4493-AEE8-2615EBC11188}
extern const GUID MCNR_RateControl;
// {6E726F48-2C19-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SdpDecoder;
// {6E726F48-2C20-4493-AEE8-2615EBC11188}
extern const GUID MCNR_ServerWorkMode;
// {6E726F48-2C21-4493-AEE8-2615EBC11188}
extern const GUID MCNR_NetworkPinProperties;
// {6E726F48-2C22-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SdpFileName;
// {6E726F48-2C23-4493-AEE8-2615EBC11188}
extern const GUID MCNR_InterleavingConnectionSocket;
// {6E726F48-2C24-4493-AEE8-2615EBC11188}
extern const GUID MCNR_UseSAP;
// {6E726F48-2C25-4493-AEE8-2615EBC11188}
extern const GUID MCNR_UseRTCP;
// {6E726F48-2C26-4493-AEE8-2615EBC11188}
extern const GUID MCNR_StreamBitrate;
// {6E726F48-2C27-4493-AEE8-2615EBC11188}
extern const GUID MCNR_BitrateInfoStorage;
// {6E726F48-2C28-4493-AEE8-2615EBC11188}
extern const GUID MCNR_NetworkMediaSSRC;
// {6E726F48-2C29-4493-AEE8-2615EBC11188}
extern const GUID MCNR_PublicRtpInfo;
// {6E726F48-2C30-4493-AEE8-2615EBC11188}
extern const GUID MCNR_UseLossTollerantMP3;
// {6E726F48-2C31-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SvcDependencyId;
// {6E726F48-2C33-4493-AEE8-2615EBC11188}
extern const GUID MCNR_SvcQualityId;
// {6E726F48-2C32-4493-AEE8-2615EBC11188}
extern const GUID MCNR_BurstnessValue;
// {9451777A-18CB-49ce-BEA9-21CBC6C5159B}
extern const GUID MCNR_NetworkErrorHandling;
//---------------------------------------------NWRender Params-------------------------------------------------------------
// {97A5D863-EC94-4234-8C12-554DE775EAD0}
extern const GUID ENR_payload;

// {C49B0276-2227-4d72-88A2-B5D36969F099}
extern const GUID ENR_transport;

// {31543CB0-C01C-4b58-B494-E32D0D52CBED}
extern const GUID ENR_port;
extern const GUID ENR_port_video;
extern const GUID ENR_port_audio;
extern const GUID ENR_port_bifs;

// {62E77578-3B29-4702-90AA-E2F6FBF29EDB}
extern const GUID ENR_destination;
// {5DCF9169-6B9C-40c7-BDA1-F4C9587E9F6F}
extern const GUID ENR_interface;
// {AEB4E28D-D66F-4ec4-9ADD-52F27F44B23F}
extern const GUID ENR_connection_speed;
// {A060FFF0-0FC5-41e7-AE97-21484A91F6A8}
extern const GUID ENR_ttl;
// {BB21FBC6-21B9-4b9b-AB5B-2435AE1B8C3B}
extern const GUID ENR_max_bit_rate;
// {560EB57B-F0CA-4a8e-90B4-FD6AD68B515C}
extern const GUID ENR_max_video_bit_rate;
// {AE754EB2-4D02-48dd-BA3B-C0A3EFD2E76B}
extern const GUID ENR_max_audio_bit_rate;
// {7696A0BA-C5B6-4786-91F5-8C2699C48EE8}
extern const GUID ENR_sync_point;
// {CE75DD63-F658-447e-ADC3-FA93D07D5043}
extern const GUID ENR_max_upd_size;
// {239EEB26-4D56-4b3f-B892-2288D855A353}
extern const GUID ENR_not_control_rate;

// {F406F4E5-CE8F-4a20-88B1-B7A9FC2A5FC6}
extern const GUID ENR_user_name;
// {E914C2B7-7A27-4bcd-AB66-1BC7B3D96C47}
extern const GUID ENR_session_name;
// {4271B640-E2EC-459e-8474-120AD5A227C6}
extern const GUID ENR_session_info;
// {A1741E5B-FF49-4db4-91B1-2229712D92ED}
extern const GUID ENR_description;
// {849F69DD-E669-4c78-80B2-31B66F0EF83D}
extern const GUID ENR_email;
// {4FEF6DA3-7869-40df-91BB-188438727B30}
extern const GUID ENR_phone;


/////////////////////////////////////dektec render/////////////////////////////////////
//object name: "Dektec ASI Renderer 2144000193 - 1"
//{544B4544-0203-4000-8000-012144000193}
extern const GUID DTA_2144AsiRender1;
//object name: "Dektec ASI Renderer 2144000193 - 2"
//{544B4544-0203-4000-8000-022144000193}
extern const GUID DTA_2144AsiRender2;

//object name: "Dektec ASI Renderer 2144000193 - 3"
//{544B4544-0203-4000-8000-032144000193}
extern const GUID DTA_2144AsiRender3;

//object name: "Dektec ASI Renderer 2144000193 - 4"
//{544B4544-0203-4000-8000-042144000193}
extern const GUID DTA_2144AsiRender4;

//{E436EBB5-524F-11CE-9F53-0020AF0BA770}
extern const GUID DTA_2144FileSource;

class tagFilterName
{
public:
	tagFilterName();
	tagFilterName(LPCSTR lpName);
	tagFilterName(LPCWSTR lpName);
public:
	TCHAR m_szName[256];
};

typedef tagFilterName FILTER_NAME, *LPFILTER_NAME;
typedef std::vector<IBaseFilter*> VECFILTERS;
//////////////////////////////////////////////////////////////////////////
// CAutoFilter
// 说明：对Filter 的轻量级的封装，使的对Filer的使用更方便
// 
class CAutoFilter
{
public:
	CAutoFilter();
	CAutoFilter(REFCLSID FILTER_CLSID);
	CAutoFilter(LPCSTR FILTER_NAMEA);
	CAutoFilter(LPCWSTR FILTER_NAMEW);
public:
	int				Attach(IBaseFilter* pFilter);
	IBaseFilter*	Detach();
	int				Create(REFCLSID FILTER_CLSID);
	int				Create(LPCSTR FILTER_NAMEA, REFCLSID Category = CLSID_LegacyAmFilterCategory);
	int				Create(LPCWSTR FILTER_NAMEW, REFCLSID Category = CLSID_LegacyAmFilterCategory);
	void			Disconnect();


	FILTER_NAME		Name();
	BOOL			IsInGraph();
	BOOL			IsConnected();
	int				Destroy();
	bool			IsValidate();
	template<typename TT>
	HRESULT			GetInterface(TT** ppIf);

	typedef BOOL	(*CHECKPIN)(IPin*);
	typedef CArray<void*, void*> PINLIST;

	UINT			PinsList(PINLIST& plist, CHECKPIN FunCheck = 0 , LPCGUID lpMediaType = 0);
	
	static	BOOL	IsOutAndFree(IPin* pin);
	static	BOOL	IsInAndFree(IPin* pin);
	static	BOOL	IsOutAndConnected(IPin* pin);
	static	BOOL	IsInAndConnected(IPin* pin);
	static  BOOL	CheckPinDirect(IPin* pin, PIN_DIRECTION dir);
	static	BOOL	IsPinConnected(IPin* pin);
public:
	IBaseFilter* operator->();
	CAutoFilter& operator=(IBaseFilter* pFilter);
	CAutoFilter& operator=(CAutoFilter& refAf);
	CAutoFilter& operator=(LPUNKNOWN pUnknown);
	CAutoFilter& operator=(REFCLSID FILTER_CLSID);
	CAutoFilter& operator=(LPCSTR FILTER_NAMEA);
	CAutoFilter& operator=(LPCWSTR FILTER_NAMEW);

public:
	static CAutoFilter* FromPointer(IBaseFilter*& pFilter);

public:
	void print(CString& strInfo);

private:
	IBaseFilter* m_pFilter;
};

///////////////////////////////////////////////////////////////////////////
// CGraphBase
// 说明： 对GraphBuilder的轻量级的封装，使得Filter 链路处理更方便
//
class CGraphBase
{
protected:
	IGraphBuilder* m_pFilterGraph;

public:
	CGraphBase();
	virtual ~CGraphBase();

public:
	virtual BOOL    MakeGraph();
	virtual HRESULT AddFilter(REFCLSID clsid, IBaseFilter** pFilter, LPCSTR lpName);
	virtual HRESULT	AddFilter(REFCLSID clsid, IBaseFilter** pFilter, LPCWSTR lpName);
	virtual HRESULT AddFilter(LPCTSTR lpFriendlyName,IBaseFilter** pFilter, REFCLSID Category = CLSID_LegacyAmFilterCategory);
	virtual HRESULT AddLocalFilter(REFCLSID clsid, IBaseFilter** pFilter, LPCSTR lpName);
	virtual HRESULT AddLocalFilter(REFCLSID clsid, IBaseFilter** pFilter, LPCWSTR lpName);
	virtual HRESULT AddFilterSource(LPCSTR lpFileName, IBaseFilter** pFilter);
	virtual HRESULT AddFilterSource(LPCWSTR lpFileName, IBaseFilter** pFilter);
	virtual	HRESULT AddFilterSink( REFCLSID clsid, LPCWSTR wcsFilterName, LPCWSTR wcsOutputFN, IBaseFilter** pFilter );	
	virtual HRESULT	AddFilterSink( REFCLSID clsid, LPCSTR csFilterName, LPCSTR csOutputFN, IBaseFilter** pFilter);
	virtual HRESULT AddLocalFilterSink( REFCLSID clsid, LPCSTR csFilterName ,LPCSTR csOutputFN, IBaseFilter** pFilter);
	virtual HRESULT AddLocalFilterSink( REFCLSID clsid, LPCWSTR wcsFilterName, LPCWSTR wcsOutputFN, IBaseFilter** pFilter);
	virtual HRESULT RemoveNotConnectedFilters();
	virtual int		CurrentGraphState(IGraphBuilder*& pFG);
	HRESULT			AddToROT(DWORD* pROID);
	HRESULT			RemoveFromROT(DWORD* pROID);

	virtual HRESULT ConnectFilter(IBaseFilter*& pfirst,IBaseFilter*& pNext, BOOL bIntelligent = FALSE);
	virtual	HRESULT ConnectOutPinWithFilter(IPin*& OutPin, IBaseFilter*& Filter, BOOL bIntelligent = FALSE );
	
	//{added by tianyx  17:7:2013   14:37
	//virtual HRESULT GetNextFilters(IBaseFilter* pFilterIn, PIN_DIRECTION Dir, VECFILTERS& vFilters);
	//}end added by tianyx

	virtual HRESULT FindPin(IBaseFilter*& pFilter,PIN_DIRECTION direction,IPin*& pPin,const GUID* MediaTypes[] = NULL,ULONG ctype= 0 );
	virtual HRESULT RemoveAllFilters(IGraphBuilder*& FG);
	virtual HRESULT ReleaseGraph();
	virtual HRESULT NukeDownstream(IBaseFilter*& pf);
	virtual HRESULT RenderOutputPin( IBaseFilter*& pFilter);
	virtual HRESULT	RenderVideoPin(IPin* pPin, IBaseFilter* pReceiveFilter);
	virtual HRESULT RenderAudioPin(IPin* pPin, IBaseFilter* pReceiveFilter);
	virtual HRESULT CLSIDToString(const GUID* pGUID, CString& strGUID);
	virtual HRESULT StringToCLSID(LPCTSTR lpszGUID, CLSID& rid);

			HRESULT ReleaseTimer();

public:
	BOOL			Run();
	BOOL			Pause();
	BOOL			Stop();	
	BOOL			SeekTo( REFERENCE_TIME rtPos );
	LONGLONG		GetDuration();
	BOOL			SetRate(double rate);
	double			GetRate();
	BOOL			IsRuning();

public:
	void			GetLastError(CString& strLastError);

protected:
	void __cdecl	ErrMsg(LPCTSTR lpMsg, ...);

protected:
	class _ErrImpl;
	_ErrImpl* m_pError;

	class CMapClsidToDll;

	static CMapClsidToDll* m_pMapClsidToDll;

	class CLocalDlls ;
	static CLocalDlls* m_pLocalDlls;

	static BOOL m_bAllLocalDllLoaded;

	static CCritSec m_LocalDllLock;
public:
#ifdef _DEBUG
	DWORD m_RegdwID;
	HRESULT savegrf();
#endif // _DEBUG

public:
	static void ClearLocalDlls();

};
//////////////////////////////////////////////////////////////////////////
// CIndexedFilter
// 对Filter的管理，可以通过Index访问特定的Filter，并且提供释放所有Filter功能
//
class CIndexedFilter:public CMapWordToPtr
{
private:
	CCritSec		m_Lock;
public:
	CIndexedFilter(){};
	~CIndexedFilter(){ReleaseAllFilter();}
public:
	IBaseFilter*&	operator[](WORD wIndex);
	void			ReleaseAllFilter();
	enum FILTER_INDEX{
		Video_File_Source = 0,
		Audio_File_Source,
		Video_Capture,
		Audio_Capture,
		Video_Demuxer,
		Audio_Demuxer,
		Video_Decoder,
		Audio_Decoder,
		Video_Encoder,
		Video_Encoder2,
		Audio_Encoder,
		Audio_Encoder2,
		Video_SmartTee,
		Audio_SmartTee,
		Video_Muxer,
		Audio_Muxer,
		Video_Grabber,
		Audio_Grabber,
		Video_Grabber2,
		Audio_Grabber2,
		Video_Convert,
		Video_Convert2,
		Audio_Convert,
		Audio_Convert2,
		Video_Render,
		Audio_Render,
		Video_File_Sink,
		Video_File_Sink2,
		Audio_File_Sink,
		Auido_File_Sink2,
		//{added by tianyx  17:7:2013   15:44
		DTA2144AsiRender1,
		DTA2144AsiRender2,
		DTA2144AsiRender3,
		DTA2144AsiRender4,
		InfTreeForDTA,
		DTATSFileSource,
		//}end added by tianyx
		FILTER_USER = 1024,
	};
};


#endif // !defined(AFX_ENCODER_H__AA7D6A89_CA1B_46FB_A29D_466645476532__INCLUDED_)
