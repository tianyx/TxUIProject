// BaseGraph.cpp: implementation of the CBaseGraph class.
//王鋆 2008.04.29
//基类:所有的各种格式文件解码驱动的基类
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseGraph.h"
#include <atlconv.h>
#include "afxtempl.h"//zibj
//#include "ModuleConfig/ModuleConfig.h"
//#include "ModuleConfig/common_mc.h"
//#include "ModuleConfig/mpeg2dec_mc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//zibj filter本地动态加载
//EXTERN_C void __stdcall gClearLocalDlls()
//{
//	CBaseGraph::ClearLocalDlls();
//}
CBaseGraph::CMapClsidToDll* CBaseGraph::m_pMapClsidToDll = NULL;
CBaseGraph::CLocalDlls* CBaseGraph::m_pLocalDlls = NULL;
BOOL CBaseGraph::m_bAllLocalDllLoaded = FALSE;
CCritSec CBaseGraph::m_LocalDllLock;

class CLocalDll
{
public:
	CLocalDll(LPCTSTR lpDllName)
		: m_hHandle(0)
		, fnDllGetClassObject(0)
	{
		m_hHandle = ::LoadLibraryEx(lpDllName, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
		if( m_hHandle )
		{
			fnDllGetClassObject = (LPFNGETCLASSOBJECT)::GetProcAddress(m_hHandle,"DllGetClassObject");
			if( !fnDllGetClassObject )
			{
				::FreeLibrary(m_hHandle);
				m_hHandle = 0;
			}
		}
	}
	
	~CLocalDll()
	{
		if( m_hHandle )
		{
			fnDllGetClassObject = 0;
			::FreeLibrary(m_hHandle);
			m_hHandle = 0;
		}
	}
private:
	HMODULE m_hHandle;
	LPFNGETCLASSOBJECT fnDllGetClassObject;
public:
	operator bool() const {return m_hHandle && fnDllGetClassObject;}
	
	//HRESULT __stdcall DllGetClassObject(__in REFCLSID rclsid, __in REFIID riid, LPVOID FAR* ppv)
	HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR* ppv)
	{
		if( *this )
		{
			return fnDllGetClassObject(rclsid, riid, ppv);
		}
		return E_FAIL;
	}
};

template<>
AFX_INLINE UINT AFXAPI HashKey<REFCLSID>(REFCLSID key)
{
	return key.Data1 + MAKELONG(key.Data2 ,key.Data3) +
		MAKELONG(MAKEWORD(key.Data4[0], key.Data4[1]),MAKEWORD(key.Data4[2],key.Data4[3])) +
		MAKELONG(MAKEWORD(key.Data4[4], key.Data4[5]),MAKEWORD(key.Data4[6],key.Data4[7]));
}
template<>
AFX_INLINE UINT AFXAPI HashKey<CString&>(CString& key)
{
	int nLength = key.GetLength();
	
	UINT uRet = 0;
	for (int n = 0; n < nLength; n ++)
	{
		uRet += ((UINT)(key.GetAt(n)) << ((n % 4) * 8));
	}	
	return uRet;
}

class CBaseGraph::CMapClsidToDll : public CMap<CLSID, REFCLSID, CLocalDll*, CLocalDll*>
{
};
class CBaseGraph::CLocalDlls: public CMap<CString, CString&, CLocalDll*, CLocalDll*>
{
	typedef CMap<CString, CString&, CLocalDll*, CLocalDll*> _BaseT;
public:
	~CLocalDlls()
	{
		Clear();
	}
public:
	void Clear()
	{
		CString strKey;
		CLocalDll* lpDll;
		POSITION pos = _BaseT::GetStartPosition();
		while(pos)
		{
			strKey.Empty();
			lpDll = 0;
			
			_BaseT::GetNextAssoc(pos, strKey,lpDll);
			if( lpDll )
			{
				delete lpDll;
			}
		}
		
		_BaseT::RemoveAll();
	}
};
void CBaseGraph::ClearLocalDlls()
{
	CAutoLock lc(&m_LocalDllLock);
	
	if( m_pMapClsidToDll )
	{
		delete m_pMapClsidToDll;
		m_pMapClsidToDll = 0;
	}
	if( m_pLocalDlls )
	{
		delete m_pLocalDlls;
		m_pLocalDlls = 0;
	}
}
//zibj end
DWORD __stdcall CBaseGraph::EventProc(LPVOID lpvoid)
{
	TRACE("<<<Into EventProc!\n");
	CBaseGraph *T = (CBaseGraph*)lpvoid;

	HRESULT hr = S_FALSE;
	long evCode, param1, param2;
	try
	{
		while (hr = T->m_pMediaEvent->GetEvent(&evCode, &param1, &param2, INFINITE), SUCCEEDED(hr))
		{
			TRACE1("\nevCode:%ld\n",evCode);
			switch(evCode) 
			{ 
			case EC_COMPLETE:
				TRACE("message:EC_COMPLETE\n");
				//	::SendMessage(T->m_ParentHWND, WM_DS_EC_COMPLETE,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_COMPLETE,param1,param2);//chenAdd for Detect
				break;
			case EC_PAUSED:
				TRACE("message:EC_PAUSED\n");
				//	::SendMessage(T->m_ParentHWND, WM_DS_EC_PAUSED,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_PAUSED,param1,param2);
				break;
			case EC_SHUTTING_DOWN:
				TRACE("message:EC_SHUTTING_DOWN\n");
				//		::SendMessage(T->m_ParentHWND, WM_DS_EC_SHUTTING_DOWN,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_SHUTTING_DOWN,param1,param2);
				break;
			case EC_REPAINT://
				//	::SendMessage(T->m_ParentHWND, WM_DS_EC_REPAINT,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_REPAINT,param1,param2);
				break;
			case EC_SNDDEV_IN_ERROR://An audio device error has occurred on an input pin
				//		::SendMessage(T->m_ParentHWND, WM_DS_EC_EC_SNDDEV_IN_ERROR,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_EC_SNDDEV_IN_ERROR,param1,param2);
				break;
			case EC_SNDDEV_OUT_ERROR: //An audio device error has occurred on an output pin. 
				//	::SendMessage(T->m_ParentHWND, WM_DS_EC_EC_SNDDEV_OUT_ERROR,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_EC_SNDDEV_OUT_ERROR,param1,param2);
				break;
			case EC_STREAM_ERROR_STILLPLAYING:// An error has occurred in a stream. The stream is still playing 
				//   ::SendMessage(T->m_ParentHWND, WM_DS_EC_STREAM_ERROR_STILLPLAYING,param1,param2); 
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_STREAM_ERROR_STILLPLAYING,param1,param2);
				break;
			case EC_STREAM_ERROR_STOPPED:// A stream has stopped because of an error 
				//		::SendMessage(T->m_ParentHWND, WM_DS_EC_STREAM_ERROR_STOPPED,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_STREAM_ERROR_STOPPED,param1,param2);
				break;
			case EC_USERABORT://The user has terminated playback
				TRACE("message:EC_USERABORT\n");
				//	::SendMessage(T->m_ParentHWND, WM_DS_EC_USERABORT,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_USERABORT,param1,param2);
				break;
			case EC_STATE_CHANGE:
				TRACE("message:EC_STATE_CHANGE\n");
				//	::SendMessage(T->m_ParentHWND, EC_STATE_CHANGE,param1,param2);
				T->m_pMsgCallBack->OnDecodeMessage(EC_STATE_CHANGE,param1,param2);
				break;
			case EC_UNBUILT:
				TRACE("message:EC_UNBUILT\n");
				break;
			case EC_USE_CLEANUP://用户定义退出线程消息
				TRACE(">>>Leave EventProc!\n");
				if (param1)
				{
					//		::SendMessage(T->m_ParentHWND, WM_DS_EC_USE_CLEANUP,0,0);
					T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_USE_CLEANUP,param1,param2);
				}
				AfxEndThread(0);
				break;
			default:
				TRACE1("message:%d\n",evCode);
			} 
			
			hr = T->m_pMediaEvent->FreeEventParams(evCode,param1,param2);
		}
	}
	catch(...)
	{
		T->m_pMsgCallBack->OnDecodeMessage(WM_DS_EC_USERABORT,0,0);
	}

	TRACE(">>>Leave EventProc!\n");
	return 0;
}

CBaseGraph::CBaseGraph()
{
	OutputDebugString("WWW: CBaseGraph::CBaseGraph()");
	m_pGraphBuilder   = NULL;
	m_pMediaControl   = NULL;
	m_pMediaSeeking   = NULL;
	m_pMediaEvent     = NULL;
	m_pMediaEventSink = NULL;
	m_pFileSrcF       = NULL;
	m_pSampGrabberF   = NULL;
//	m_pSampGrabber    = NULL;
	m_pAudioGrabberF  = NULL;
	m_pANullRenderF   = NULL;
    m_pVNullRenderF   = NULL;
	m_pParserFilter   = NULL;
	m_pVDecoder       = NULL;
	m_pADecoder       = NULL;
	m_pColorSpaceFilter = NULL;
 	m_ParentHWND      = NULL;

	m_filePath[0]     = '\0';
    m_graphState = State_Empty;
	m_hEventTreadHandle = 0;
    m_pMsgCallBack = NULL;
	m_dwRetryTimes = 0;
	m_dwRetryInterval = 0;

    CoInitialize(NULL);	
	//zibj filter本地化动态加载
	CAutoLock lc(&m_LocalDllLock);
	
	if( m_pMapClsidToDll == NULL )
	{
		m_pMapClsidToDll = new CBaseGraph::CMapClsidToDll();
	}
	if( m_pLocalDlls == NULL )
	{
		m_pLocalDlls = new CBaseGraph::CLocalDlls();
	}
	//zibj end


}

CBaseGraph::CBaseGraph(HWND ParentHwnd)
{
	if (ParentHwnd)
	{
		m_ParentHWND = ParentHwnd;
	}
	m_pGraphBuilder   = NULL;
	m_pMediaControl   = NULL;
	m_pMediaEvent     = NULL;
	m_pMediaEventSink = NULL;
	m_pFileSrcF       = NULL;
	m_pSampGrabberF   = NULL;
//	m_pSampGrabber    = NULL;
	m_pAudioGrabberF  = NULL;
	m_pANullRenderF   = NULL;
    m_pVNullRenderF   = NULL;
	m_pParserFilter   = NULL;
	m_pVDecoder       = NULL;
	m_pADecoder       = NULL;
	m_ParentHWND      = NULL;
	m_filePath[0]     = '\0';
    m_graphState = State_Empty;
	m_hEventTreadHandle = 0;
	m_pMsgCallBack = NULL;
	m_dwRetryTimes = 0;
	m_dwRetryInterval = 0;
    CoInitialize(NULL);

	//zibj filter本地化动态加载
	CAutoLock lc(&m_LocalDllLock);
	
	if( m_pMapClsidToDll == NULL )
	{
		m_pMapClsidToDll = new CBaseGraph::CMapClsidToDll();
	}
	if( m_pLocalDlls == NULL )
	{
		m_pLocalDlls = new CBaseGraph::CLocalDlls();
	}
	//zibj end
}

CBaseGraph::~CBaseGraph()
{
   OutputDebugString("WWW: CBaseGraph::~CBaseGraph()");
  // Pause();
   //Cancel();
	CleanUp();
  // Release();//当Graph还没有建立，但是有 Filter加入的时候，stop不成功，需要这样释放内存
   CoUninitialize();
}

HRESULT CBaseGraph::LoadFile(LPCTSTR lpszPathName, DWORD dwRetryTimes, DWORD dwRetryInterval)
{
	if(!lpszPathName)
		return S_FALSE;
	//如果GraphManager 里有链路，那么清理
	if (m_pGraphBuilder)
	{
		CleanUp();
	}
	strcpy(m_filePath,lpszPathName);
	
	m_dwRetryTimes = dwRetryTimes;
	m_dwRetryInterval = dwRetryInterval;
	
	return S_OK;
	
}

// wang.zh add 
HRESULT CBaseGraph::LoadFile(IFileSourceFilter* pFileSourceFilter, const char* strFileName)
{
	USES_CONVERSION;
	HRESULT hr = E_FAIL;
	hr=pFileSourceFilter->Load(A2OLE(strFileName),NULL);
    
	DWORD dwTriedTimes = 0;
	while(FAILED(hr) && dwTriedTimes < m_dwRetryTimes)
	{
		Sleep(m_dwRetryInterval * 1000);
		dwTriedTimes ++ ;
		hr=pFileSourceFilter->Load(A2OLE(strFileName),NULL);
	}
	return hr;
}
/////////////////////////////////////////////////////////////////////////////////////////
//创建fileReader(Async)过滤器实例，并添加到pGB
//pGB:(in)
//pfileReader:(out)指定添加到pGB中的过滤器
//strFileName:(in)指定pfileReader需要加在的文件路径
///////////////////////////////////////////////////////////////////////////////////////
HRESULT CBaseGraph::AddSourceFilter2Graph(IGraphBuilder *pGB,IBaseFilter** pfileReader,const char* strFileName)
{
	HRESULT hr = E_FAIL;
	//File source filter
	if(!pGB || !strFileName)
		return hr;

	USES_CONVERSION;
	IFileSourceFilter* pFileSourceFilter=NULL;
	hr = CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)(pfileReader));
	if (FAILED(hr))
	{
		return hr;
	}
    hr=(*pfileReader)->QueryInterface(IID_IFileSourceFilter, (void**)&pFileSourceFilter);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = LoadFile(pFileSourceFilter, strFileName);
	pFileSourceFilter->Release();
	if ( FAILED( hr ) )
	{		
		return hr;
	}

    hr = pGB->AddFilter(*pfileReader, L"File Source");
	if (FAILED(hr))
	{
		(*pfileReader)->Release();
		return hr;
	}
	return hr;
}
/////////////////////////////////////////////////////////////////////////////////////////
//通过CLSID直接创建过滤器实例，并添加到pGB
//pGB:(in)
//clsid:(in)CLSID
//pFilter:(out)指定添加到pGB中的过滤器
//Name:(in)指定添加到pBG中过滤器的名称
///////////////////////////////////////////////////////////////////////////////////////
HRESULT CBaseGraph::AddFilter2GraphByClassID(IGraphBuilder *pGB,REFCLSID clsid,IBaseFilter** pFilter,const char* Name)
{
	HRESULT hr = E_FAIL;
	if(!pGB)
		return hr;

	char filtername[256];
	if(Name != NULL)
		strcpy(filtername,Name);
	else
		strcpy(filtername,"filter");

	
	hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)(pFilter));
	if (FAILED(hr))
	{
		ErrMsg(TEXT("Cannot find  %s"),Name);
		return hr;
	}

	USES_CONVERSION	;
	hr = pGB->AddFilter(*pFilter, A2W(Name));
	if (FAILED(hr))
	{
		(*pFilter)->Release();
		ErrMsg(TEXT("Cannot add  %s"),Name);
	}

	return hr;
}
/////////////////////////////////////////////////////////////////////////////////////////
//通过FriendlyNamep在系统设备中查找过滤器，并添加到pGB
//pFilter:指定添加到pGB中的友好名为FriendlyName的过滤器
///////////////////////////////////////////////////////////////////////////////////////
HRESULT CBaseGraph::AddFilter2GraphByFriendlyName(IGraphBuilder *pGB,const char* FriendlyName,IBaseFilter** pFilter)
{
	HRESULT   hr = E_FAIL;   
	if( !pGB || !FriendlyName )
		return hr;

	ICreateDevEnum   *pSysDevEnum = NULL;   
	IEnumMoniker     *pEnum       = NULL;   
	IMoniker         *pMoniker    = NULL;   
	bool              bExit = false;

	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pSysDevEnum);   
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pSysDevEnum->CreateClassEnumerator( CLSID_LegacyAmFilterCategory, &pEnum,0);   
	if(hr == S_OK)     //   S_FALSE   means   nothing   in   this   category.   
	{  
		while(S_OK == pEnum->Next(1, &pMoniker, NULL) && bExit == false)   
		{   
			IPropertyBag   *pPropBag = NULL;   
			pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&pPropBag);   

			VARIANT   var;
			VariantInit(&var);  
			char FriendlyName0[256];
			
			hr = pPropBag->Read(L"FriendlyName",&var, 0); 

			USES_CONVERSION;
			strcpy(FriendlyName0, OLE2A(var.bstrVal));
			if (SUCCEEDED(hr) && !lstrcmp(FriendlyName0,FriendlyName) )   
			{   
				hr = pMoniker->BindToObject(NULL, NULL,IID_IBaseFilter,(void**)pFilter);   
				if(SUCCEEDED(hr))   
				{  
					USES_CONVERSION;
					if (pGB)
					{
						hr = pGB->AddFilter((*pFilter), A2W(FriendlyName));
						if (FAILED(hr))
						{
							(*pFilter)->Release();
							ErrMsg(TEXT("Cannot add  %s"),FriendlyName);
						}
                        bExit = true;//添加成功
					}
				}  
			} 
			pPropBag->Release();   
			pMoniker->Release(); 
			VariantClear(&var);     
		}   
		pEnum->Release();   
	} 
	pSysDevEnum->Release();   
	if (bExit)
	{
		return S_OK;
	}
	return E_FAIL;
} 
///////////////////////////////////////////////////////////////////////////////////////////
//连接两个filter,pfirst的OutputPin与pNext的InputPin连接,目前只能连接video and  audio 
//pGB:input,指定连接filter的GraphManager
//BOOL bdirect:是否需要采用智能连接
//////////////////////////////////////////////////////////////////////////////////////////
HRESULT CBaseGraph::ConnectFilter(IGraphBuilder *pGB,IBaseFilter* pfirst,IBaseFilter* pNext,BOOL bdirect)
{
	HRESULT hr=E_FAIL;
	if(!pGB || !pfirst || !pNext)
		return hr;

	ULONG           Fetched  = 0;
	AM_MEDIA_TYPE   *pAM     = NULL;
	IPin            *pOutPin[10] ;
	IPin            *pInPin[10]  ;
	IEnumMediaTypes *pEnum   = NULL;
	BYTE            BInputType = 0x00;;
	BYTE            BOutputType= 0x00;

	for(int i=0;i<10;i++)
	{
		pOutPin[i] = NULL;
		pInPin[i]  = NULL;
	}
	hr = EnumdPins(pfirst,PINDIR_OUTPUT,&pOutPin[0],10);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = EnumdPins(pNext,PINDIR_INPUT,&pInPin[0],10);
	if (hr != S_OK)
	{
		return hr;
	}
	ULONG x = 0;
	while(pOutPin[x] != NULL)
	{
		LPWSTR WStr = NULL;
		hr = pOutPin[x]->QueryId(&WStr);
		USES_CONVERSION;   
        std::string  strPinID(W2A(WStr)); 
		size_t nErrPos = -1;
		size_t nPos = strPinID.find('~',0);
		hr = pOutPin[x]->EnumMediaTypes(&pEnum);//枚举支持的媒体类型
		if (SUCCEEDED(hr))
		{
			hr = pEnum->Reset();
			hr = pEnum->Next(1,&pAM,&Fetched);
			while (S_OK == hr)//pEnum->Next(1,&pAM,&Fetched))
			{
				if(pAM->majortype == MEDIATYPE_Stream )
					BOutputType = BOutputType | 1;
				else if(pAM->majortype == MEDIATYPE_Audio )
					BOutputType = BOutputType | 2;
				else if(pAM->majortype == MEDIATYPE_Video )
					BOutputType = BOutputType | 4;
				else if (nPos == nErrPos)
				{
					BOutputType = BOutputType | 8;
				}

				DeleteMediaType(pAM);//deletes an old allocated AM_MEDIA_TYPE structure
				hr = pEnum->Next(1,&pAM,&Fetched);
			}
			pEnum->Release();
		}
		else if (nPos == nErrPos)//处理
		{
			BOutputType = BOutputType | 8;
		}

		if (BOutputType > 0x00)
		{
			ULONG y = 0;
			while (pInPin[y] != NULL)
			{
			//	if (BOutputType == BInputType && (BOutputType|BInputType)>0x00)
				{
					if (bdirect)
					{
						hr=pGB->ConnectDirect(pOutPin[x],pInPin[y],NULL);
					} 
					else
					{
						hr=pGB->Connect(pOutPin[x],pInPin[y]);
					}
					if(FAILED(hr))
					{
						DbgOutString("Cannot find Connect the two Pin");
					}
					else
					{
						return hr;
					}
				}
				y++;
			}
		}
		x++;
	}
	return hr;

}
/////////////////////////////////////////////
//枚举过滤器pFilter中方向为direction的所有没有连接的引脚
//pFilter:(in)
//direction:(in)
//pPin:(out)指向找到的引脚首地址
////////////////////////////////////////////////
HRESULT CBaseGraph::EnumdPins( IBaseFilter* pFilter, 
						 PIN_DIRECTION direction, 
						 IPin **ppPin ,ULONG cPins)
{
    if( !pFilter )
    {
        return E_POINTER;
    }
    HRESULT hr = E_FAIL;
    IEnumPins *pEnumPins;
    hr = pFilter->EnumPins( &pEnumPins );
    if( FAILED( hr ) )
    {
        return hr;
    }
    
    IPin            *pTempPin    = NULL;
    ULONG           fetched      = 0;
	ULONG           cfetchedPins = 0;
    hr = pEnumPins->Reset( );
    while(S_OK == pEnumPins->Next( 1, &pTempPin, &fetched ) 
		  && cfetchedPins<cPins)
    {
        ASSERT( pTempPin );
		PIN_DIRECTION   dir;
        hr = pTempPin->QueryDirection( &dir );
        /*if( FAILED( hr ) || dir != direction )
        {
			pTempPin->Release();
			continue;
		}
		
		else*/
		if(dir == direction)
		{
			IPin *pCTPin = 0;
		    hr = pTempPin->ConnectedTo(&pCTPin);
			if (SUCCEEDED(hr))
			{
				pCTPin->Release();
			}
			else//没有连接
			{
				*(ppPin+cfetchedPins) = pTempPin;
				cfetchedPins++;
				hr = S_OK;
			}
			
		}
		pTempPin->Release();
    }
    pEnumPins->Release();
    return hr;
}
HRESULT CBaseGraph::GetUnconnectedPin(
									 IBaseFilter *pFilter,   // Pointer to the filter.
									 PIN_DIRECTION PinDir,   // Direction of the pin to find.
									 IPin **ppPin)           // Receives a pointer to the pin.
{
	*ppPin = 0;
	IEnumPins *pEnum = 0;
	IPin *pPin = 0;
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if (FAILED(hr))
	{
		return hr;
	}

	while (pEnum->Next(1, &pPin, NULL) == S_OK)
	{
		PIN_DIRECTION ThisPinDir;
		pPin->QueryDirection(&ThisPinDir);
		if (ThisPinDir == PinDir)
		{
			IPin *pTmp = 0;
			hr = pPin->ConnectedTo(&pTmp);
			if (SUCCEEDED(hr))  // Already connected, not the pin we want.
			{
				pTmp->Release();
			}
			else  // Unconnected, this is the pin we want.
			{
				pEnum->Release();
				*ppPin = pPin;
				return S_OK;
			}
		}
		pPin->Release();
	}
	pEnum->Release();
	// Did not find a matching pin.
	return E_FAIL;
}
//////////////////////////////////////////////////////
////创建过滤器管理组件
/////
HRESULT CBaseGraph::MakeGraph()
{
//	CAutoLock  AL(&m_RenderCritSec);
	OutputDebugString("MakeGraph()");
    // we have one already
    if(m_pGraphBuilder)
        return S_OK;
	
    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (LPVOID *)&m_pGraphBuilder);
	
    return hr;
}
////
////断开链路
//pGB[in]:过滤器管理组件
//pf [in]:这个过滤器开始向后断开的链路
////
HRESULT CBaseGraph::NukeDownstream(IGraphBuilder *pGB,IBaseFilter *pf)
{
	HRESULT hr = S_FALSE;
	if (!pf)
        return hr;

	IPin *pP=0, *pTo=0;
    ULONG u;
    IEnumPins *pins = NULL;
    PIN_INFO pininfo;

    hr = pf->EnumPins(&pins);
    pins->Reset();
    while(hr == NOERROR)
    {
        hr = pins->Next(1, &pP, &u);
        if(hr == S_OK && pP)
        {
            pP->ConnectedTo(&pTo);
            if(pTo)
            {
                hr = pTo->QueryPinInfo(&pininfo);
                if(hr == NOERROR)
                {
                    if(pininfo.dir == PINDIR_INPUT)
                    {
                        NukeDownstream(pGB,pininfo.pFilter);//递归
                        pGB->Disconnect(pTo);
                        pGB->Disconnect(pP);
                    }
                    pininfo.pFilter->Release();
                }
                pTo->Release();
            }
            pP->Release();
        }
    }
	
    if(pins)
        pins->Release();
	return true;
}
//Remove Filter from pFG
HRESULT CBaseGraph::RemoveFilterGraph(IGraphBuilder *pFG)
{
	HRESULT hr;
	// Enumerate the filters in the graph.
	IEnumFilters *pEnum = NULL;
	hr = pFG->EnumFilters(&pEnum);
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		{
			if(pFilter)//chenAdd
			{
				// Remove the filter.
				pFG->RemoveFilter(pFilter);
				// Reset the enumerator.
				pEnum->Reset();
				pFilter->Release();
			}
		}
		pEnum->Release();
	}
	return 0;
}
//设置Grabber属性
HRESULT CBaseGraph::SetVideoGrabber()
{
	HRESULT hr = S_FALSE;
	if(!m_pSampGrabberF)
	   return hr;
	ISampleGrabber      *pISampGrabber = NULL;
	hr = m_pSampGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pISampGrabber);
	if (FAILED(hr))
	{
		return hr;
	}
	// 设置接受的媒体类型
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.formattype = FORMAT_VideoInfo; 
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_YUY2;
//	mt.subtype = MEDIASUBTYPE_RGB24;
	hr = pISampGrabber->SetMediaType(&mt);
	if (FAILED(hr))
	{
		return hr;
	}
	// 设置是否自带BUFFER
	hr=pISampGrabber->SetBufferSamples(FALSE);
	if (FAILED(hr))
	{
		return hr;
	}
	hr=pISampGrabber->SetOneShot(FALSE);
	if (FAILED(hr))
	{
		return hr;
	}
	
	// 设置回调函数
    hr=pISampGrabber->SetCallback(&m_SampleGrabberCB, 1);

	pISampGrabber->Release();
	return hr;
	   
}

//设置AudioGrabber属性
HRESULT CBaseGraph::SetAudioGrabber()
{
	HRESULT hr = S_FALSE;
	if(!m_pAudioGrabberF)
		return hr;

	ISampleGrabber      *pISampGrabber = NULL;
	hr = m_pAudioGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pISampGrabber);
	if (FAILED(hr))
	{
		return hr;
	}
	// 设置接受的媒体类型
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.formattype = FORMAT_WaveFormatEx; 

	mt.majortype = MEDIATYPE_Audio;
	mt.subtype = MEDIASUBTYPE_PCM;
	hr = pISampGrabber->SetMediaType(&mt);
	if (FAILED(hr))
	{
		return hr;
	}
	// 设置是否自带BUFFER
	hr=pISampGrabber->SetBufferSamples(FALSE);
	if (FAILED(hr))
	{
		return hr;
	}
	hr=pISampGrabber->SetOneShot(FALSE);
	if (FAILED(hr))
	{
		return hr;
	}
	
	// 设置回调函数
    hr=pISampGrabber->SetCallback(&m_AudioGrabberCB, 1);
	
	pISampGrabber->Release();
	return hr;
	   
}

//创建过滤器，并且连接链路
//这是一个虚函数，不同的媒体格式解码可以重载这个函数
//

HRESULT CBaseGraph::BuildGrabberGraph()
{
	TRACE("<<<into CBaseGraph builder!\n");
   HRESULT hr = S_OK;
   if(!m_pGraphBuilder)
	   return hr;
   hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
   if (FAILED(hr))
   {
	   return hr;
   }
   // 创建Sample Grabber
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber") && 
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_SampleGrabber,&m_pSampGrabberF,"Video Grabber");
   if (FAILED(hr))
   {
	   return hr;
   }
   hr = SetVideoGrabber();
   if (FAILED(hr))
   {
	   return hr;
   }
   hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") &&
	   AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
   if (FAILED(hr))
   {
	   return hr;
   }
   hr = SetVideoGrabber();
   if (FAILED(hr))
   {
	   return hr;
   }
   hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pSampGrabberF);
   if (FAILED(hr))
   {
	   return hr;
   }
   hr = ConnectFilter(m_pGraphBuilder,m_pSampGrabberF,m_pVNullRenderF);
   return hr;
}

void CBaseGraph::Release()
{
	SAFE_RELEASE(m_pMediaControl);
	SAFE_RELEASE(m_pMediaSeeking);
	SAFE_RELEASE(m_pMediaEvent);
	SAFE_RELEASE(m_pMediaEventSink);

// 	ULONG l;
//  	l = m_pFileSrcF->AddRef();
//  	l = m_pFileSrcF->Release();
//  
//  	l = m_pVDecoder->AddRef();
//  	l = m_pVDecoder->Release();
//  
//  	l = m_pVNullRenderF->AddRef();
//  	l = m_pVNullRenderF->Release();
//  
//  // 	l = m_pParserFilter->AddRef();
//  // 	l = m_pParserFilter->Release();
//  
//  	l = m_pSampGrabberF->AddRef();
//  	l = m_pSampGrabberF->Release();
//  
//    	l = m_pAudioGrabberF->AddRef();
//    	l = m_pAudioGrabberF->Release();
 
//  	l = m_pGraphBuilder->AddRef();
//  	l = m_pGraphBuilder->Release();
    
	SAFE_RELEASE(m_pFileSrcF);
	SAFE_RELEASE(m_pVDecoder);
	SAFE_RELEASE(m_pADecoder);
	SAFE_RELEASE(m_pVNullRenderF);
	SAFE_RELEASE(m_pANullRenderF);
	SAFE_RELEASE(m_pParserFilter);
	SAFE_RELEASE(m_pSampGrabberF);
	SAFE_RELEASE(m_pAudioGrabberF);
	SAFE_RELEASE(m_pColorSpaceFilter);
	SAFE_RELEASE(m_pGraphBuilder);

	m_graphState = State_Empty;
}

//删除过滤器管理中的过滤器
HRESULT CBaseGraph::CleanUp(long lParam1,long lParam2)
{
   HRESULT hr = E_FAIL;
   if(!m_pGraphBuilder || !m_pFileSrcF)
	   return hr;

   //等待链路build完成
   while (m_graphState == State_Building)
   {
	   OutputDebugString("CBaseGraph::CleanUp");
	   Sleep(50);
   }
   //停止事件检测线程
   if(Stop() == S_OK)
   {
	      hr=NukeDownstream(m_pGraphBuilder,m_pFileSrcF);
		  if (FAILED(hr))
		  {
			  return hr;
		  }
		  hr=RemoveFilterGraph(m_pGraphBuilder);
		  if (FAILED(hr))
		  {
			  return hr;
		  }
		  if (m_pMediaEventSink)
		  {
			  hr=m_pMediaEventSink->Notify(EC_USE_CLEANUP,lParam1,lParam2);
		  }

		  Release();

		  if (m_hEventTreadHandle != 0)
		  {
			  CloseHandle(m_hEventTreadHandle);
			  m_hEventTreadHandle = 0;
		  }
		  /*
#ifdef  _DEBUG
		  IRunningObjectTable *pROT;
		  if (SUCCEEDED(GetRunningObjectTable(0, &pROT)))
		  {
			  pROT->Revoke(m_reg);
			  pROT->Release();
		  }
		  ULONG ul = m_pGraphBuilder->AddRef();
	      ul = m_pGraphBuilder->Release();
#endif
		  */
   }
   return hr;
	   
}

HRESULT CBaseGraph::Run()
{
	HRESULT hr = E_FAIL;
	if(!(m_pMediaControl))
	{
		return hr;
// 		hr = GetMediaControl(m_pGraphBuilder,&m_pMediaControl);
// 		if (hr != S_OK)
// 		{
// 			return hr;
// 		}
	}
	hr = m_pMediaControl->Run();
	if(FAILED(hr))
	{
		m_pMediaControl->Stop();
	}
	//m_pMediaControl->Release();
    return hr;
}

HRESULT CBaseGraph::Pause()
{
	HRESULT hr = E_FAIL;
	if(!(m_pMediaControl))
	{
		return hr;
// 		hr = GetMediaControl(m_pGraphBuilder,&m_pMediaControl);
// 		if (hr != S_OK)
// 		{
// 			return hr;
// 		}
	}
	hr = m_pMediaControl->Pause();
	if (SUCCEEDED(hr))
	{
	//	m_pMediaControl->Release();
	}
   return hr;
}

HRESULT CBaseGraph::Stop()
{
	HRESULT hr = E_FAIL;
	if(!(m_pMediaControl))
	{
		hr = GetMediaControl(m_pGraphBuilder,&m_pMediaControl);
		if (hr != S_OK)
		{
			return hr;
		}
	}
	hr = m_pMediaControl->Stop();
	if (SUCCEEDED(hr))
	{
	//	m_pMediaControl->Release();
	}
	
   return hr;
}

HRESULT CBaseGraph::Cancel()
{
    return CleanUp(1,0);
}
//格式化错误显示(对话框)
void CBaseGraph::ErrMsg(LPTSTR szFormat,...)
{
    static TCHAR szBuffer[2048]={0};
    const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    const int LASTCHAR = NUMCHARS - 1;
	
    // Format the input string
    va_list pArgs;
    va_start(pArgs, szFormat);
	
    // Use a bounded buffer size to prevent buffer overruns.  Limit count to
    // character size minus one to allow for a NULL terminating character.
    _vsntprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);
	
    // Ensure that the formatted string is NULL-terminated
    szBuffer[LASTCHAR] = TEXT('\0');

	OutputDebugString(szBuffer);//zibj 	
   // MessageBox(NULL,szBuffer,"JS_FileDrive" ,MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
}

HRESULT CBaseGraph::GetMediaControl(IGraphBuilder *pFG,IMediaControl **pMediaControl)
{
	HRESULT hr = E_FAIL;
	if(!pFG)
		return hr;

	hr = pFG->QueryInterface(IID_IMediaControl, (void **)pMediaControl);
	return hr;
}
HRESULT CBaseGraph::SetBuffer(void *pA)
{
    HRESULT hr = E_FAIL;
	if (pA)
	{
		hr = m_SampleGrabberCB.SetBuffer(pA);
	}
	return hr;
}
HRESULT CBaseGraph::SetAudioBuffer(void *pA)
{
    HRESULT hr = E_FAIL;
	if (pA)
	{
		hr = m_AudioGrabberCB.SetBuffer(pA);
	}
	return hr;
}
//开始解码
HRESULT CBaseGraph::RenderFile()
{
    OutputDebugString("wj: RenderFile beg!\n");
//	CAutoLock  AL(&m_RenderCritSec);
	HRESULT hr = E_FAIL;
	if(m_filePath[0] == '\0')//如果没有文件可以播放
		return hr;
	hr = MakeGraph();
	if (hr != S_OK)
	{
		return hr;
	}
	m_graphState = State_Building;
	hr = BuildGrabberGraph();
	m_graphState = State_Builded;
	if(hr != S_OK)
	{
	   //SendEvent(EC_USE_CLEANUP,0,0);
       return hr;
	}
	hr = GetMediaControl(m_pGraphBuilder,&m_pMediaControl);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = GetMediaSeeking(m_pGraphBuilder,&m_pMediaSeeking);
	if (hr != S_OK)
	{
		return hr;
	}
	//事件接口
    hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&m_pMediaEvent);
	if (hr != S_OK)
	{
		return hr;
	}
	//事件发送接口
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventSink, (void **)&m_pMediaEventSink);
	if (hr != S_OK)
	{
		return hr;
	}
	//// 关闭时钟
    IMediaFilter    *pMediaFilter = NULL;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaFilter, (void **)&pMediaFilter);
	if (hr != S_OK)
	{
		return hr;
	}
	//如果已经关闭时间戳
	hr = pMediaFilter->SetSyncSource(NULL);
	if(hr != S_OK )
		DbgOutString("不能关闭时钟!");
	SAFE_RELEASE(pMediaFilter);
#ifdef _DEBUG
	// 保存 grf
	HRESULT hr2 = E_FAIL;
	IXml2Dex* pXml2Dex;
    hr2 = CoCreateInstance(CLSID_Xml2Dex, NULL, CLSCTX_INPROC_SERVER,
		IID_IXml2Dex, (void**) &pXml2Dex);
	if (SUCCEEDED(hr2))
	{
		hr2 = pXml2Dex->WriteGrfFile(m_pGraphBuilder, L"C:\\render.grf" );
		SAFE_RELEASE(pXml2Dex);
	}
	else
	{
		MessageBox(NULL,"保存GRF失败!\n","JF_FileDrive",MB_OK);
	}
//     //注册grf
// 	IMoniker * pMoniker;
//     IRunningObjectTable *pROT;
//     if (FAILED(GetRunningObjectTable(0, &pROT))) {
//         return E_FAIL;
//     }
//     WCHAR wsz[256];
//     wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)m_pGraphBuilder, GetCurrentProcessId());
//     hr = CreateItemMoniker(L"!", wsz, &pMoniker);
//     if (SUCCEEDED(hr)) {
//         hr = pROT->Register(0, m_pGraphBuilder, pMoniker,&m_reg);
//         SAFE_RELEASE(pMoniker);
//     }
//     pROT->Release();
	
#endif
  
	//开始事件监视线程
//wangjun
//	AfxBeginThread((UINT*)EventProc,(LPVOID)this);
//	HANDLE TreadHandle;
	if (m_pMsgCallBack)
	{
		DWORD  ThreadID;
		m_hEventTreadHandle=::CreateThread(NULL,0,EventProc,this,NULL,&ThreadID);
		if (!m_hEventTreadHandle)
		{
			MessageBox(NULL,"创建线程失败!","CDriveMgr",MB_OK);
			return E_FAIL;
		}
	}
	else
	{
       OutputDebugString("wj: 无法创建事件监视线程!\n");
	}
	OutputDebugString("wj: RenderFile Complete!\n");
	return hr;
}

//设置父句柄，所有消息发给这个父句柄
BOOL CBaseGraph::SetParent(HWND ParentHwnd)
{
    if (ParentHwnd)
    {
		m_ParentHWND = ParentHwnd;
		return true;
    }
	return false;
}
///////////
//////////
HRESULT CBaseGraph::AddDMO2GraphByClassID(IGraphBuilder *pGB,
										  REFCLSID clsid,
										  REFCLSID CategoryGuid,
										  IBaseFilter** pFilter,
										  const char* Name)
{
	HRESULT hr = E_FAIL;
	if (!pGB)
	{
		return hr;
	}
	// Create the DMO Wrapper filter.
	hr = CoCreateInstance(CLSID_DMOWrapperFilter, NULL, 
		CLSCTX_INPROC_SERVER, IID_IBaseFilter, 
		reinterpret_cast<void**>(pFilter));
	
	if (SUCCEEDED(hr)) 
	{
		// Query for IDMOWrapperFilter.
		IDMOWrapperFilter *pDmoWrapper;
		hr = (*pFilter)->QueryInterface(IID_IDMOWrapperFilter, 
			reinterpret_cast<void**>(&pDmoWrapper));
		
		if (SUCCEEDED(hr)) 
		{     
			// Initialize the filter.
			hr = pDmoWrapper->Init(clsid, CategoryGuid); 
			pDmoWrapper->Release();
			
			if (SUCCEEDED(hr)) 
			{
				// Add the filter to the graph.
				USES_CONVERSION;
				hr = pGB->AddFilter((*pFilter), A2W(Name));
				if (FAILED(hr))//有问题
				{
					(*pFilter)->Release();
					ErrMsg(TEXT("Cannot add  %s"),Name);
				}
				
			}
		}
		
	}
	return hr;
}
//目前没有使用这个函数
HRESULT CBaseGraph::SendEvent(long Event,long Param1,long Param2)
{
	HRESULT hr = E_FAIL;
   if (!m_pGraphBuilder)
   {
	   return hr;
   }
   hr=m_pGraphBuilder->QueryInterface(IID_IMediaEventSink,(void**)&m_pMediaEventSink);
   if (hr == S_OK)
   {
	   hr = m_pMediaEventSink->Notify(Event,Param1,Param2);
   }
   SAFE_RELEASE(m_pMediaEventSink);

   return hr;
}

HRESULT CBaseGraph::GetMediaSeeking(IGraphBuilder *pFG,IMediaSeeking **pMediaSeeking)
{
	HRESULT hr = E_FAIL;
	if(!pFG)
		return hr;
	
	hr = pFG->QueryInterface(IID_IMediaSeeking, (void **)pMediaSeeking);
	return hr;
}

LONGLONG CBaseGraph::GetDuration()
{
    if(m_pMediaSeeking == NULL)
	{
		HRESULT hr = GetMediaSeeking(m_pGraphBuilder,&m_pMediaSeeking);
		if(hr != S_OK)
			return 0;
	}
	LONGLONG tcl = 0;
	//if(S_OK == m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_FRAME))
       m_pMediaSeeking->GetDuration(&tcl);
	
	return  tcl ;  //ns -> frame
}
HRESULT CBaseGraph::GetRate(double *dRate)
{
    if(m_pMediaSeeking == NULL)
	{
		HRESULT hr = GetMediaSeeking(m_pGraphBuilder,&m_pMediaSeeking);
		if(hr != S_OK)
			return hr;
	}
    return m_pMediaSeeking->GetRate(dRate);
}
int CBaseGraph::GetSrcStreamCount()
{
	int nStreamCount = 0;
	if (m_pParserFilter)
	{
		HRESULT hr = E_FAIL;
		IEnumPins *pEnumPins;
		hr = m_pParserFilter->EnumPins( &pEnumPins );
		if( FAILED( hr ) )
		{
			return nStreamCount;
		}
		
		IPin            *pTempPin    = NULL;
		ULONG           fetched      = 0;
		hr = pEnumPins->Reset( );
		while(S_OK == pEnumPins->Next( 1, &pTempPin, &fetched ))
		{
			ASSERT( pTempPin );
			PIN_DIRECTION   dir;
			hr = pTempPin->QueryDirection( &dir );
			if(dir == PINDIR_OUTPUT)
			{
				AM_MEDIA_TYPE amt;
				if(pTempPin->ConnectionMediaType(&amt) == S_OK)
				{
					if (amt.majortype == MEDIATYPE_Video 
						|| amt.majortype == MEDIATYPE_Audio)
					{
						nStreamCount++;
					}
				}
				FreeMediaType(amt);
			}
			pTempPin->Release();
		}
		pEnumPins->Release();
	}
	return nStreamCount;
}
//int nStream:input
//AM_MEDIA_TYPE* AM:output 
HRESULT CBaseGraph::GetSrcStreamMedaiType(int nStream,AM_MEDIA_TYPE* AM)
{
	HRESULT hr = E_FAIL;
   	if (m_pParserFilter)
	{
		int nStreamCount = 0;
		IEnumPins *pEnumPins;
		hr = m_pParserFilter->EnumPins( &pEnumPins );
		if( FAILED( hr ) )
		{
			return hr;
		}
		
		IPin   *pTempPin = NULL;
		ULONG  fetched   = 0;
		bool   bDo = true;
		hr = pEnumPins->Reset( );
		while(bDo)
		{
			if (S_OK == pEnumPins->Next( 1, &pTempPin, &fetched ))
			{
				ASSERT( pTempPin );
				PIN_DIRECTION   dir;
				hr = pTempPin->QueryDirection( &dir );
				if(dir == PINDIR_OUTPUT)
				{
					AM_MEDIA_TYPE amt;
					if(pTempPin->ConnectionMediaType(&amt) == S_OK)
					{
						if (amt.majortype == MEDIATYPE_Video 
							|| amt.majortype == MEDIATYPE_Audio)
						{
							if (nStreamCount == nStream)
							{
								hr = CopyMediaType(AM,&amt);
								bDo = false;
							}
							nStreamCount++;
						}
					}
					FreeMediaType(amt);
				}
			    pTempPin->Release();
			}
		}
		pEnumPins->Release();
	}
	return hr;
}
//得到videoGrabber连接的媒体类型
HRESULT  CBaseGraph::GetVideoMediaType(AM_MEDIA_TYPE* mt)
{
   	HRESULT hr = E_FAIL;
	if (m_graphState == State_Empty)
	{
		return hr;
	}

	if(!m_pSampGrabberF)
		return hr;
	
	ISampleGrabber  *pISampGrabber = NULL;
	hr = m_pSampGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pISampGrabber);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pISampGrabber->GetConnectedMediaType(mt);
	if (FAILED(hr))
	{
		SAFE_RELEASE(m_pSampGrabberF);
		return hr;
	}
	return hr;
}
//得到AudioGrabber连接的媒体类型
HRESULT  CBaseGraph::GetAudioMediaType(AM_MEDIA_TYPE* mt)
{
	HRESULT hr = E_FAIL;
	if (m_graphState == State_Empty)
	{
		return hr;
	}
	if(!m_pAudioGrabberF)
		return hr;
	
	ISampleGrabber  *pISampGrabber = NULL;
	hr = m_pAudioGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pISampGrabber);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = pISampGrabber->GetConnectedMediaType(mt);
	if (FAILED(hr))
	{
		SAFE_RELEASE(m_pAudioGrabberF);
		return hr;
	}
	return hr;
}


//zibj filter本地化动态加载
HRESULT CBaseGraph::AddLocalFilter2GraphByClassID(IGraphBuilder *pGB, REFCLSID clsid, IBaseFilter **pFilter, const char *Name)
{
	*pFilter = NULL;
	CAutoLock lc(&m_LocalDllLock);

	if( m_pMapClsidToDll && m_pLocalDlls )
	{
		CLocalDll* pDll = NULL;
		if( m_pMapClsidToDll->Lookup(clsid, pDll) && pDll )
		{
			IClassFactory* pFactory = NULL;
			pDll->DllGetClassObject(clsid, IID_IClassFactory, (void**)&pFactory);
			if( pFactory )
			{
				pFactory->CreateInstance(NULL, IID_IBaseFilter, (void**)pFilter);
				pFactory->Release();
			}
		}
		else 
		{
			CString strKey;
			BOOL bFind = FALSE;
			POSITION pos = m_pLocalDlls->GetStartPosition();

			while (pos)
			{
				pDll = 0;
				strKey.Empty();
				m_pLocalDlls->GetNextAssoc(pos, strKey, pDll );

				IClassFactory* pFactory = NULL;
				pDll->DllGetClassObject(clsid, IID_IClassFactory, (void**)&pFactory);
				if( pFactory )
				{
					bFind = TRUE;
					pFactory->CreateInstance(NULL, IID_IBaseFilter, (void**)pFilter);
					pFactory->Release();

					m_pMapClsidToDll->SetAt(clsid, pDll);
					break;
				}
			}

			if( !bFind && !m_bAllLocalDllLoaded )
			{
				CString strPath;
		//		char strPATH[2048];
				::GetModuleFileNameA( ::GetModuleHandle(NULL), strPath.GetBuffer(1024), 1024 );
				
// 				strPath.Format("%s",strPATH);
// 				delete strPATH;
				strPath.ReleaseBuffer();
				int nIndex = max(strPath.ReverseFind('\\'), strPath.ReverseFind('/'));
				strPath.Delete(nIndex, strPath.GetLength()-nIndex);
				strPath += "\\Data\\*.*";

				CFileFind ffind;

				BOOL bFindFile = ffind.FindFile(strPath);
				BOOL bFindNext = FALSE;

				if( bFindFile )
					while( (bFindNext = ffind.FindNextFile()) || bFindFile )
					{
						bFindFile = bFindNext;

						if( ffind.IsDots() || ffind.IsDirectory() )
						{
							continue;
						}

						if( m_pLocalDlls->Lookup(ffind.GetFileTitle(), pDll))
						{
							continue;
						}

						CLocalDll* pTryDll = new CLocalDll(ffind.GetFilePath());
						if( *pTryDll )
						{
							m_pLocalDlls->SetAt(ffind.GetFileTitle(), pTryDll);

							IClassFactory* pFactory = NULL;
							pTryDll->DllGetClassObject(clsid, IID_IClassFactory, (void**)&pFactory);
							if( pFactory )
							{
								pFactory->CreateInstance(NULL, IID_IBaseFilter, (void**)pFilter);
								pFactory->Release();

								m_pMapClsidToDll->SetAt(clsid, pTryDll);
								break;
							}
						}
						else
						{
							delete pTryDll;
						}
					}

				if( !bFindFile )
					m_bAllLocalDllLoaded = TRUE;
			}
		}
		
		USES_CONVERSION	;
		if( *pFilter )
		{
			HRESULT hr = pGB->AddFilter(*pFilter,A2W(Name));
			if (FAILED(hr))
			{
				(*pFilter)->Release();
				ErrMsg(TEXT("Cannot add  %s"),Name);
			}	
	
			return hr;
		}
		else
		{
			ErrMsg(_T("Cannot Add LocalFilter %s"),Name);
		}
	}
	return E_FAIL;
}
BOOL CBaseGraph::seekTo(LONGLONG som,LONGLONG duration)
{
	if (som == 0)
	{
		return TRUE;
	}
	DWORD dwCap = 0;
	HRESULT hr;// = m_pMediaSeeking->GetCapabilities(&dwCap);
// 	if (AM_SEEKING_CanSeekAbsolute & dwCap)
// 	{
// 	  // hr = m_pMediaSeeking->SetPositions(&som,AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning ); 
// 	}
	LONGLONG stoppoint = 0;
	LONGLONG currentSom = som;
	currentSom *= 400000L;
	LONGLONG Dur;
	m_pMediaSeeking->GetDuration(&Dur);
	hr = m_pMediaSeeking->GetStopPosition(&stoppoint);
	if(currentSom < 0 )
		currentSom = 0;

	if(currentSom >= stoppoint)
		currentSom = stoppoint;

	
	if (currentSom >= Dur - 400000L)
		currentSom = Dur - 400000L;	

	hr = m_pMediaSeeking->SetPositions(&currentSom,AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning ); 
	if (SUCCEEDED(hr))
	{
// 		Sleep(200);
// 		m_pMediaControl->Run();
		return TRUE;
	}

	AfxMessageBox("seek error");
	return FALSE;
}