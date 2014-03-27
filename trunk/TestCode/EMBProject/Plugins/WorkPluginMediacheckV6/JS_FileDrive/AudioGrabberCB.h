// AudioGrabberCB.h: interface for the CAudioGrabberCB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOGRABBERCB_H__40620E40_7AC3_49D6_9AB5_DC1D1809AFA8__INCLUDED_)
#define AFX_AUDIOGRABBERCB_H__40620E40_7AC3_49D6_9AB5_DC1D1809AFA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JS_FileDrive.h"
#include "MediaSample.h"

typedef CQueue2<MyMediaSample> CSList;//Êý¾Ý»º´æ

class CAudioGrabberCB : public ISampleGrabberCB  
{
public:
	CAudioGrabberCB();
	virtual ~CAudioGrabberCB();

    // IUnknown methods
    STDMETHODIMP_(ULONG) AddRef() { return 1; }
    STDMETHODIMP_(ULONG) Release() { return 2; }
    STDMETHOD(QueryInterface)(REFIID iid, void** ppv);
	
    // ISampleGrabberCB methods
    STDMETHOD(SampleCB)(double SampleTime, IMediaSample *pSample);
    STDMETHOD(BufferCB)(double SampleTime, BYTE *pBuffer, long nBufferLen);

	HRESULT SetBuffer(void *pA);
	
private:
	//CCritSec  m_CritSec;
	CSList*   m_pSampleList;
};

#endif // !defined(AFX_AUDIOGRABBERCB_H__40620E40_7AC3_49D6_9AB5_DC1D1809AFA8__INCLUDED_)
