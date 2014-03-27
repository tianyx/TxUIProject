// AudioGrabberCB.cpp: implementation of the CAudioGrabberCB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AudioGrabberCB.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAudioGrabberCB::CAudioGrabberCB()
{
  m_pSampleList = NULL;
}

CAudioGrabberCB::~CAudioGrabberCB()
{

}
// Support querying for ISampleGrabberCB interface
HRESULT CAudioGrabberCB::QueryInterface(REFIID iid, void**ppv)
{
	if (NULL == ppv) return E_POINTER;
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
		return S_OK;
	}
	if (iid == __uuidof(ISampleGrabberCB))
	{
		*ppv = static_cast<ISampleGrabberCB*>(this);
		return S_OK;
	}
	return E_NOINTERFACE;
}
STDMETHODIMP CAudioGrabberCB::BufferCB(double SampleTime, BYTE *pBuffer, long nBufferLen)
{
	HRESULT hr = S_OK;
	MyMediaSample *MS = NULL;
	if (!m_pSampleList)
	{
		return S_OK;
	}
	MS = m_pSampleList->GetEmptyObject();
	if(MS)
	{
		hr = MS->SetData(pBuffer,nBufferLen);
		if (SUCCEEDED(hr))
		{
			MS->SetTime(SampleTime);
			m_pSampleList->GetEmptyObjectComplete();
		}
	}
	MS = NULL;
	return hr;
}
STDMETHODIMP CAudioGrabberCB::SampleCB(double SampleTime, IMediaSample *pSample)
 { 
	HRESULT hr = S_OK;
	return hr;
 }

HRESULT CAudioGrabberCB::SetBuffer(void *pA)
{
//	if(!IsBadReadPtr(pA,sizeof(CSList)))
	if(!pA)
	{
		return E_FAIL;
	}
	m_pSampleList =(CSList*)pA;
	return S_OK;
}