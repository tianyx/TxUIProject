// SampleGrabberCB.h: interface for the CSampleGrabberCB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLEGRABBERCB_H__A1A857D9_A09A_416F_AD92_79D02DA01D30__INCLUDED_)
#define AFX_SAMPLEGRABBERCB_H__A1A857D9_A09A_416F_AD92_79D02DA01D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "MediaSample.h"

typedef CQueue2<MyMediaSample> CSList;//½âÂë»º´æ

class _JS_Fix CSampleGrabberCB  :	public ISampleGrabberCB
{
public:
	CSampleGrabberCB();
	virtual ~CSampleGrabberCB();	

    // IUnknown methods
    STDMETHODIMP_(ULONG) AddRef() { return 1; }
    STDMETHODIMP_(ULONG) Release() { return 2; }
    STDMETHOD(QueryInterface)(REFIID iid, void** ppv);
	
    // ISampleGrabberCB methods
    STDMETHOD(SampleCB)(double SampleTime, IMediaSample *pSample);
    STDMETHOD(BufferCB)(double SampleTime, BYTE *pBuffer, long nBufferLen);
    HBITMAP YV122BMP(BYTE* pBuffer,DWORD Width,DWORD Height);
	BYTE* YUY22BMP(BYTE* pBuffer,DWORD Width,DWORD Height);
	void ConverToRGB24(BYTE Y,BYTE U,BYTE V,BYTE* R,BYTE* G,BYTE* B);
	BOOL SaveAsBmp(LPBYTE pDIB,CString strPath,BITMAPINFOHEADER *BIH);
	HRESULT SetBuffer(void *pA);

private:
	CCritSec  m_CritSec;
	CSList*   m_pSampleList;
	ULONG     m_GraberSampleCounts; 
};

#endif // !defined(AFX_SAMPLEGRABBERCB_H__A1A857D9_A09A_416F_AD92_79D02DA01D30__INCLUDED_)
