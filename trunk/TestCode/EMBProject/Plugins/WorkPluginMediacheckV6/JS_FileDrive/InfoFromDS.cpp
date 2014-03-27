// InfoFromDS.cpp: implementation of the CInfoFromDS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoFromDS.h"
#include "GuidRef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoFromDS_3GP::CInfoFromDS_3GP()
{

}

CInfoFromDS_3GP::~CInfoFromDS_3GP()
{

}

HRESULT CInfoFromDS_3GP::AddFilters2Graph()
{
	
	HRESULT hr = E_FAIL;
	if(!m_pGraphBuilder)
		return hr;
// 	hr = AddSourceFilter2Graph(m_pGraphBuilder,&m_pFileSrcF,m_filePath);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
// 	hr =AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_MP4_Demux,&m_pParserFilter,"MC MP4 Demux Filter") &&
// 		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_MainConcept_MP4_Demux,&m_pParserFilter,"MC MP4 Demux Filter");
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_HaaliSpliter,&m_pFileSrcF,NULL) &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_HaaliSpliter,&m_pFileSrcF,NULL);
	if (hr != S_OK)
	{
		return hr;
	}

	IFileSourceFilter* pIFileSource = NULL;
    hr = m_pFileSrcF->QueryInterface(IID_IFileSourceFilter,(void**)&pIFileSource);
	if (hr != S_OK)
	{
		return hr;
	}
	USES_CONVERSION;
	hr=pIFileSource->Load(A2OLE(m_filePath),NULL);
	pIFileSource->Release();
	
	if( FAILED (hr ))
		return hr;

	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pVNullRenderF,"NULL Video Render");
	if (hr != S_OK)
	{
		return hr;
	}
	hr = AddLocalFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render") &&
		AddFilter2GraphByClassID(m_pGraphBuilder,CLSID_NullRenderer,&m_pANullRenderF,"NULL Audio Render");
    return hr;
}

HRESULT CInfoFromDS_3GP::ConnectFiltersInGraph()
{
	HRESULT hr = E_FAIL;
	if (!m_pGraphBuilder || !m_pFileSrcF)
	{
		return hr;
	}
// 	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pParserFilter,true);
// 	if (hr != S_OK)
// 	{
// 		return hr;
// 	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pANullRenderF,TRUE);
	if (hr != S_OK)
	{
		return hr;
	}
	hr = ConnectFilter(m_pGraphBuilder,m_pFileSrcF,m_pVNullRenderF,TRUE);
	return hr;
	   
}
HRESULT CInfoFromDS_3GP::BuildGrabberGraph()
{
	HRESULT hr = E_FAIL;
	if (AddFilters2Graph() == S_OK)
	{
		return ConnectFiltersInGraph();
	}
	return E_FAIL;
}
LONGLONG CInfoFromDS_3GP::GetDuration()
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
int CInfoFromDS_3GP::GetSrcStreamCount()
{
	int nStreamCount = 0;
	if (m_pFileSrcF)
	{
		HRESULT hr = E_FAIL;
		IEnumPins *pEnumPins;
		hr = m_pFileSrcF->EnumPins( &pEnumPins );
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
HRESULT CInfoFromDS_3GP::GetSrcStreamMedaiType(int nStream,AM_MEDIA_TYPE* AM)
{
	HRESULT hr = E_FAIL;
   	if (m_pFileSrcF)
	{
		int nStreamCount = 0;
		IEnumPins *pEnumPins;
		hr = m_pFileSrcF->EnumPins( &pEnumPins );
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