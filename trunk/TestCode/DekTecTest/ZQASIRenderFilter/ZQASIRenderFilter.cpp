/********************************************************************
	created:	2013/07/23
	created:	2013:7
	filename: 	ZQASIRenderFilter.cpp
	file base:	ZQASIRenderFilter
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// ZQASIRenderFilter.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ZQASIRenderFilter.h"
#include "crtdbg.h"
#include "time.h"

#define MINOUTBPS 10000
#define PADDINGOUTPUTBIT + 100000
#define DEBUG_NULLDEV_TEST

#define CALCTSRATEDOWNMAX 2000
#define DbgLog(_x_) DbgLogInfo _x_
//////////////////////////////////////////////////////////////////////////
//ddshow def
#include <streams.h>     // DirectShow (includes windows.h)
#include <initguid.h>    // declares DEFINE_GUID to declare an EXTERN_C const.


// The CLSID used by the minimal, in-place, null filter
// DO NOT REUSE IT.  Run uuidgen.exe to create a new one.
// {C66A9EE1-4025-48a5-8380-C3ACB42697CB}
DEFINE_GUID(CLSID_ZQASIRenderFilter,
			 0xc66a9ee1, 0x4025, 0x48a5, 0x83, 0x80, 0xc3, 0xac, 0xb4, 0x26, 0x97, 0xcb);

//////////////////////////////////////////////////////////////////////////
DWORD __stdcall ReadFileProc(void* lparam);
DWORD __stdcall OutPutProc(void* lparam);
DWORD __stdcall CalInputbitRatePrc(void* lparam);
//////////////////////////////////////////////////////////////////////////

extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);

BOOL APIENTRY DllMain(HANDLE hModule, 
					  DWORD  dwReason, 
					  LPVOID lpReserved)
{
	return DllEntryPoint((HINSTANCE)(hModule), dwReason, lpReserved);
}



////////////////////////////////////////////////////////////////////////
//
// Exported entry points for registration and unregistration 
// (in this case they only call through to default implementations).
//
////////////////////////////////////////////////////////////////////////

//
// DllRegisterSever
//
// Handle the registration of this filter
//
STDAPI DllRegisterServer()
{
	return AMovieDllRegisterServer2( TRUE );

} // DllRegisterServer


//
// DllUnregisterServer
//
STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2( FALSE );

} // DllUnregisterServer




// setup data - allows the self-registration to work.

const AMOVIESETUP_MEDIATYPE sudPinTypes =
{
	&MEDIATYPE_Stream        // clsMajorType
	, &MEDIASUBTYPE_MPEG2_TRANSPORT  // clsMinorType
};

const AMOVIESETUP_PIN psudPins[] =
{ L"Input"            // strName
, FALSE               // bRendered
, FALSE               // bOutput
, FALSE               // bZero
, FALSE               // bMany
, &CLSID_NULL         // clsConnectsToFilter
, NULL                 // strConnectsToPin
, 1                   // nTypes
, &sudPinTypes        // lpTypes

};

const AMOVIESETUP_FILTER sudZQASIRenderFilter =
{ 
	&CLSID_ZQASIRenderFilter           // clsID
	, L"ZQ TS->ASI Output"            // strName
	, MERIT_DO_NOT_USE                // dwMerit
	, 1                               // nPins
	, psudPins                        // lpPin
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance
// function when it is asked to create a CLSID_VideoRenderer object

CFactoryTemplate g_Templates[] = {
	{ L"ZQ TS->ASI Output"
	, &CLSID_ZQASIRenderFilter
	, CZQASIRenderFilter::CreateInstance
	, NULL
	, &sudZQASIRenderFilter },
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

//////////////////////////////////////////////////////////////////////////
DWORD __stdcall CalInputbitRatePrc(void* lparam)
{
	CZQASIRenderFilter* pFilter = (CZQASIRenderFilter*) lparam;
	ASSERT(pFilter);
	while(TRUE)
	{
		pFilter->m_nCalRateByte = 0;
		int nSec = GetTickCount();
		Sleep(1000);
		nSec = GetTickCount()-nSec;

		pFilter->m_nInTsbps = pFilter->m_nCalRateByte*CLOCKS_PER_SEC /nSec * 8; //add padding bit
		_RPT1(0, "\n real input tsRate = %.3f Mbps", (double)pFilter->m_nInTsbps/1048576.0);
	}
}

DWORD __stdcall ReadFileProc(void* lparam)
{
	CZQASIRenderFilter* pFilter = (CZQASIRenderFilter*) lparam;
	ASSERT(pFilter);
	ST_DTABUFF& buffExchange = pFilter->m_buffExchange;
	HANDLE& m_readlock0 = pFilter->m_readlock0;
	HANDLE& m_readlock1 = pFilter->m_readlock1;
	HANDLE& m_outLock0 = pFilter->m_outLock0;
	HANDLE& m_outlock1 = pFilter->m_outlock1;
	ST_DTABUFF& g_buff0 = pFilter->m_buff0;
	ST_DTABUFF& g_buff1 = pFilter->m_buff1;

	static int g_nCurrReadBuffId = 0;
	DTAPI_RESULT hr = 0;
	unsigned int nCheckTime = 0;
	while(TRUE)
	{

		if (buffExchange.nUsed == 0 || (nCheckTime < 10 && buffExchange.nUsed < DTAWAITBUFFMIN))
		{
			nCheckTime++;
			Sleep(10);
			continue;
		}
		nCheckTime = 0;
		ASSERT(buffExchange.nUsed != 0);
		HANDLE pLock =g_nCurrReadBuffId == 0? m_readlock0 : m_readlock1;
		HANDLE poutLock =g_nCurrReadBuffId == 0? m_outLock0 : m_outlock1;
		WaitForSingleObject(pLock, INFINITE);

		//write buffer
		ST_DTABUFF& fBuffer = g_nCurrReadBuffId == 0? g_buff0:g_buff1;
		memcpy(fBuffer.buffer, buffExchange.buffer, buffExchange.nUsed);
		fBuffer.nUsed = buffExchange.nUsed;
		buffExchange.nUsed = 0;
		ReleaseSemaphore(poutLock, 1, NULL);
		g_nCurrReadBuffId = (++g_nCurrReadBuffId)%2;
	}
	return 0;
}

DWORD __stdcall OutPutProc(void* lparam)
{
	CZQASIRenderFilter* pFilter = (CZQASIRenderFilter*) lparam;
	ASSERT(pFilter);
	HANDLE& m_readlock0 = pFilter->m_readlock0;
	HANDLE& m_readlock1 = pFilter->m_readlock1;
	HANDLE& m_outLock0 = pFilter->m_outLock0;
	HANDLE& m_outlock1 = pFilter->m_outlock1;
	ST_DTABUFF& g_buff0 = pFilter->m_buff0;
	ST_DTABUFF& g_buff1 = pFilter->m_buff1;
	int g_nFifoSize = pFilter->g_nFifoSize;
	if (g_nFifoSize == 0)
	{
		ASSERT(FALSE);
		g_nFifoSize = 3000000;
	}
	static int g_nCurrWriteBuffId = 0;

	DTAPI_RESULT hr = 0;
	int snInitLoaded = 0;

	DtOutpChannel* ptsOut = &pFilter->m_tsOut;
	while(TRUE)
	{
		HANDLE pLock =g_nCurrWriteBuffId == 0? m_readlock0 : m_readlock1;
		HANDLE poutLock =g_nCurrWriteBuffId == 0? m_outLock0 : m_outlock1;
		WaitForSingleObject(poutLock, INFINITE);
		//_RPT1(0,"\n --output loop start buff id =%d", g_nCurrWriteBuffId);
		ST_DTABUFF& fBuffer = g_nCurrWriteBuffId == 0? g_buff0:g_buff1;

#ifndef DEBUG_NULLDEV_TEST
		if (snInitLoaded < INITLOADSIZE)
		{
			hr = ptsOut->SetTxControl(DTAPI_TXCTRL_HOLD);
			ASSERT(hr == DTAPI_OK);
		}
		int nReaded = 0;
		byte *pPos = fBuffer.buffer;
		INT64 nRemain = fBuffer.nUsed;
		int nCtrlState = 0;
		hr = ptsOut->GetTxControl(nCtrlState);
		ASSERT(hr == DTAPI_OK);

		while(nRemain > 0)
		{
			int nLoad = 0;
			hr = ptsOut->GetFifoLoad(nLoad);
			//TRACE("\nfifo load = 0x%x", nLoad);
			ASSERT(hr == DTAPI_OK);
			while(nLoad >= g_nFifoSize - 0x20000)
			{					
				Sleep(1000);
				hr = ptsOut->GetFifoLoad(nLoad);
				//TRACE("\ncheck fifo load = 0x%x", nLoad);
				ASSERT(hr == DTAPI_OK);
			}

			int nReadOnce = nRemain < WRITEBUFSIZE? nRemain: WRITEBUFSIZE;
			static INT64 snTotalOutput = 0;
			snTotalOutput += nReadOnce;
			//TRACE("\ntotal output = 0x%x", snTotalOutput);
			hr = ptsOut->Write((char*)pPos, nReadOnce);
			ASSERT(hr == DTAPI_OK);
			pPos += nReadOnce;
			nRemain -= nReadOnce;
			snInitLoaded +=nReadOnce;
			hr = ptsOut->GetTxControl(nCtrlState);
			ASSERT(hr == DTAPI_OK);
			if (nCtrlState == DTAPI_TXCTRL_HOLD && snInitLoaded >= INITLOADSIZE)
			{
				hr = ptsOut->SetTxControl(DTAPI_TXCTRL_SEND);
				_RPT0(0,"\n --initloaded, set ctrl to DTAPI_TXCTRL_SEND");
				ASSERT(hr == DTAPI_OK);
			}
		}
#endif

		fBuffer.nUsed = 0;
		//_RPT1(0,"\n --output loop end buff id =%d", g_nCurrWriteBuffId);
		g_nCurrWriteBuffId = (++g_nCurrWriteBuffId)%2;
		LONG nPreLockCount = 0;
		ReleaseSemaphore(pLock, 1, &nPreLockCount);

	}

	_RPT0(0,"\n write thread quit");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CUnknown * WINAPI CZQASIRenderFilter::CreateInstance( LPUNKNOWN punk, HRESULT *phr )
{
	ASSERT(phr);
	CZQASIRenderFilter *pNewObject = new CZQASIRenderFilter(punk, phr);
	if (pNewObject == NULL) {
		if (phr)
			*phr = E_OUTOFMEMORY;
	}

	return pNewObject;
}

STDMETHODIMP CZQASIRenderFilter::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	CheckPointer(ppv,E_POINTER);
	if (riid == IID_IASIDevPreConfig) {
		return GetInterface((IASIDevPreConfig*) this, ppv);
	} 
	return CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
}

STDMETHODIMP CZQASIRenderFilter::init_DevPreConfig( ST_DEVPARAM *paramIn /* */ )
{
	if (!paramIn)
	{
		return E_FAIL;
	}
	if (m_DevParam.bInputBlockMode != DEVVALUENOTUSED)
	{
		m_DevParam.bInputBlockMode = (paramIn->bInputBlockMode!= DEVVALUENOTUSED)? paramIn->bInputBlockMode:m_DevParam.bInputBlockMode;
		m_DevParam.bAutoAdjustOutTsBps = (paramIn->bAutoAdjustOutTsBps != DEVVALUENOTUSED)? paramIn->bAutoAdjustOutTsBps:m_DevParam.bAutoAdjustOutTsBps;
		m_DevParam.nDevType = (paramIn->nDevType != DEVVALUENOTUSED)? paramIn->nDevType:m_DevParam.nDevType;
		m_DevParam.nFifoSize = (paramIn->nFifoSize != DEVVALUENOTUSED)? paramIn->nFifoSize:m_DevParam.nFifoSize;
		m_DevParam.nOutTsBps = (paramIn->nOutTsBps != DEVVALUENOTUSED)? paramIn->nOutTsBps:m_DevParam.nOutTsBps;
		m_DevParam.nPort = (paramIn->nPort != DEVVALUENOTUSED)? paramIn->nPort:m_DevParam.nPort;
		m_DevParam.nTxMode = (paramIn->nTxMode != DEVVALUENOTUSED)? paramIn->nTxMode:m_DevParam.nTxMode;
		m_DevParam.pExInfo = (paramIn->pExInfo != NULL)? paramIn->pExInfo:m_DevParam.pExInfo;
	}
	return S_OK;
}

STDMETHODIMP CZQASIRenderFilter::get_DevPreConfig( ST_DEVPARAM* paramOut  )
{
	if (!paramOut)
	{
		return E_FAIL;
	}
	*paramOut = m_DevParam;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////


CZQASIRenderFilter::CZQASIRenderFilter( LPUNKNOWN pUnk, HRESULT *phr )
:CBaseFilter(NAME("CZQASIRenderFilter"), pUnk, &m_Lock, CLSID_ZQASIRenderFilter)
{
	m_pPin = new CZQASIRenderInputPin(this, GetOwner(), this,
		&m_Lock,
		&m_ReceiveLock,
		phr);
	if (m_pPin == NULL) {
		if (phr)
			*phr = E_OUTOFMEMORY;
		return;
	}

	m_bDevInited = FALSE;
	m_hReadThrd = NULL;
	m_hDtaThrd = NULL;

	m_readlock0 = CreateSemaphore(NULL, 1, 1, NULL);
	m_outLock0 = CreateSemaphore(NULL, 1, 1, NULL);
	m_readlock1 = CreateSemaphore(NULL, 1, 1, NULL);
	m_outlock1 = CreateSemaphore(NULL, 1, 1, NULL);
	m_hfexcLock = CreateSemaphore(NULL, 1, 1, NULL);

	g_nFifoSize = 0;

	m_nInTsbps = 0;

	m_DevParam.nDevType = 2144;
	m_DevParam.nPort = 1;
	m_DevParam.nFifoSize = 0;
	m_DevParam.nOutTsBps = 3000000;
	m_DevParam.pExInfo = 0;
	m_DevParam.nTxMode = DTAPI_TXMODE_188;
	m_DevParam.bAutoAdjustOutTsBps = 1;
	m_DevParam.bInputBlockMode = FALSE;
}

CBasePin * CZQASIRenderFilter::GetPin( int n )
{
	if (n == 0) {
		return m_pPin;
	} else {
		return NULL;
	}
}

int CZQASIRenderFilter::GetPinCount()
{
	return 1;
}

STDMETHODIMP CZQASIRenderFilter::Stop()
{
	if (m_hDtaThrd)
	{
		CloseRecvLoop();
	}
	return CBaseFilter::Stop();	
}

STDMETHODIMP CZQASIRenderFilter::Run( REFERENCE_TIME tStart )
{
	// Clear the global 'write error' flag that would be set
	// if we had encountered a problem writing the previous dump file.
	// (eg. running out of disk space).
	//
		// Since we are restarting the graph, a new file will be created.
	m_hCalBitrateThrd =  CreateThread(NULL, 0, CalInputbitRatePrc, (LPVOID)this,0, 0);
	if (!m_bDevInited)
	{
		if(InitDtaDevice() != S_OK)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}
	}

	if (!m_DevParam.bInputBlockMode && m_nInTsbps == 0)
	{
		ReCalDevOutRate();
		return S_OK;
	}

	if (m_hDtaThrd)
	{
		CloseRecvLoop();
	}
	if(StartRecvLoop() != S_OK)
	{
		return E_FAIL;
	}
	return CBaseFilter::Run(tStart);
}

STDMETHODIMP CZQASIRenderFilter::Pause()
{

	return CBaseFilter::Pause();
}

CZQASIRenderFilter::~CZQASIRenderFilter()
{
	if (m_hCalBitrateThrd)
	{
		TerminateThread(m_hCalBitrateThrd, 0);
		m_hCalBitrateThrd = NULL;
	}
	delete m_pPin;
	m_pPin = NULL;

	
}

int CZQASIRenderFilter::InitDtaDevice()
{
#ifdef DEBUG_NULLDEV_TEST
	return 0;
#endif // DEBUG_NULLDEV_TEST
	DTAPI_RESULT hr = 0;
	if (m_dvc.AttachToType(m_DevParam.nDevType) != DTAPI_OK)
	{
		ASSERT(FALSE);
		return -3;
	}

	if (m_tsOut.AttachToPort(&m_dvc, m_DevParam.nPort) != DTAPI_OK)
	{
		ASSERT(FALSE);
		return -4;
	}
	//m_tsOut.SetTsRateBps();
	int nRate = 0;
	hr =m_tsOut.GetTsRateBps(nRate);;
	hr = m_tsOut.GetFifoSize(g_nFifoSize);
	hr = m_tsOut.GetFifoSizeMax(g_nFifoMaxSize);
	m_DevParam.nFifoSize = g_nFifoMaxSize < m_DevParam.nFifoSize? g_nFifoMaxSize:m_DevParam.nFifoSize;
	if (m_DevParam.nFifoSize > 0)
	{
		hr = m_tsOut.SetFifoSize(m_DevParam.nFifoSize);
	}
	hr = m_tsOut.GetFifoSize(g_nFifoSize);
	nRate = m_DevParam.nOutTsBps;
	hr = m_tsOut.SetTsRateBps(nRate);
	hr =m_tsOut.SetTxMode(m_DevParam.nTxMode, 1);
	hr =m_tsOut.SetTxControl(DTAPI_TXCTRL_HOLD);
	m_bDevInited = TRUE;
	return 0;
}

int CZQASIRenderFilter::UnInitDtaDevice()
{
	return 0;
}

int CZQASIRenderFilter::CloseRecvLoop()
{

	if (m_hReadThrd)
	{
		TerminateThread(m_hReadThrd, 0);
		CloseHandle(m_hReadThrd);
		m_hReadThrd = NULL;
	}

	if (m_hDtaThrd)
	{
		TerminateThread(m_hDtaThrd, 0);
		CloseHandle(m_hDtaThrd);
		m_hDtaThrd = NULL;
	}

	ReleaseSemaphore(m_readlock0, 1, NULL);
	ReleaseSemaphore(m_readlock1, 1, NULL);
	ReleaseSemaphore(m_outLock0, 1, NULL);
	ReleaseSemaphore(m_outlock1, 1, NULL);
	ReleaseSemaphore(m_hfexcLock, 1, NULL);

	//clean buff
	m_buff0.nUsed = 0;
	m_buff1.nUsed = 0;
	m_buffExchange.nUsed = 0;
	
	//dev set to hold
	m_tsOut.ClearFifo();
#ifndef DEBUG_NULLDEV_TEST
	m_tsOut.SetTxControl(DTAPI_TXCTRL_HOLD);
#endif

	return S_OK;
}

int CZQASIRenderFilter::StartRecvLoop()
{
	//preset semaphore
	WaitForSingleObject(m_outLock0, INFINITE);
	WaitForSingleObject(m_outlock1, INFINITE);

	m_hReadThrd = CreateThread(NULL, 0, ReadFileProc, (LPVOID)this, 0, 0);
	m_hDtaThrd = CreateThread(NULL, 0, OutPutProc, (LPVOID)this,0, 0);
	if (m_hDtaThrd == NULL || m_hReadThrd == NULL)
	{
		ASSERT(FALSE);
		CloseRecvLoop();
		return -1;
	}
	return S_OK;
}

int CZQASIRenderFilter::ReCalDevOutRate()
{
	if (m_DevParam.bInputBlockMode)
	{
		return -1;
	}

	m_bResetDevOutBps = TRUE;
	return S_OK;

}

HRESULT CZQASIRenderFilter::Receive( IMediaSample *pSample )
{
	CheckPointer(pSample,E_POINTER);
	CAutoLock lock(&m_ReceiveLock);

	PBYTE pbData;
	HRESULT hr = pSample->GetPointer(&pbData);
	if (FAILED(hr)) {
		return hr;
	}
	int nDataLen = pSample->GetActualDataLength();

	//for calc input bitrate thread 
	m_nCalRateByte += nDataLen;

	if ((m_DevParam.bInputBlockMode== 0 && m_DevParam.bAutoAdjustOutTsBps) && m_bResetDevOutBps)
	{
		if (m_nInTsbps > g_nFifoSize || m_nInTsbps < MINOUTBPS)
		{
			//too big or too small
		}
		else
		{
			m_bResetDevOutBps = FALSE;
			m_nOutTsbps = m_nInTsbps +PADDINGOUTPUTBIT;
// 			//reset loop
// 			if (m_hDtaThrd)
// 			{
// 				CloseRecvLoop();
// 			}

#ifndef DEBUG_NULLDEV_TEST
			hr =m_tsOut.SetTxControl(DTAPI_TXCTRL_HOLD);
			if (hr== S_OK)
			{
				hr = m_tsOut.SetTsRateBps(m_nOutTsbps);
				ASSERT(hr==S_OK);
			}
#endif
			//ASSERT(StartRecvLoop()== S_OK);
		}
		return S_OK;
	}
	
	//receive data
	DWORD dwRet = WaitForSingleObject(m_hfexcLock, (m_DevParam.bInputBlockMode == 1)? INFINITE: 10);
	static int nPackLostTime = 0;
	if (dwRet == WAIT_OBJECT_0)
	{
		nPackLostTime = 0;
		// copy to exchange
		if (m_buffExchange.nUsed + nDataLen <MAXDTABUFFLEN)
		{
			byte* pPos = m_buffExchange.buffer;
			pPos += m_buffExchange.nUsed;
			memcpy(pPos, pbData, nDataLen);
			m_buffExchange.nUsed += nDataLen;
			//check
		}
		else
		{
			_RPT0(0, "\n zq pack lost !!!");
			nPackLostTime++;
			if (nPackLostTime == 10)
			{
				//the output rate must change bigger!!!!
				if (!m_DevParam.bInputBlockMode || m_DevParam.nOutTsBps == 0)
				{
					nPackLostTime = 0;
					ReCalDevOutRate();
				}
			}
			//ASSERT(FALSE);
		}
		
		ReleaseSemaphore(m_hfexcLock, 1, NULL);
	}
	else
	{
		//dispose it
		ASSERT(FALSE);
	}

	return S_OK;

// 	REFERENCE_TIME tStart, tStop;
// 	pSample->GetTime(&tStart, &tStop);
// 
// 	DbgLog((LOG_TRACE, 1, TEXT("tStart(%s), tStop(%s), Diff(%d ms), Bytes(%d)"),
// 		(LPCTSTR) CDisp(tStart),
// 		(LPCTSTR) CDisp(tStop),
// 		(LONG)((tStart - m_tLast) / 10000),
// 		pSample->GetActualDataLength()));
// 
// 	m_tLast = tStart;

	// Copy the data to the file
}

STDMETHODIMP CZQASIRenderFilter::get_value( const GUID *pParamID, /* [in] */ VARIANT *pValue )
{
	if (pParamID == NULL || pValue == NULL)
	{
		return E_INVALIDARG;
	}

	if (IsEqualGUID(*pParamID, CLSID_ASIOutputBitrate))
	{
		pValue->vt = VT_INT;
		pValue->intVal = m_nOutTsbps;
		return S_OK;
	}
	else if (IsEqualGUID(*pParamID, CLSID_ASIInputBitrate))
	{
		pValue->vt = VT_INT;
		pValue->intVal = m_nInTsbps;
		return S_OK;
	}

	return E_INVALIDARG;
}

STDMETHODIMP CZQASIRenderFilter::set_value( const GUID *pParamID, /* [in] */ const VARIANT *pValue )
{

	if (pParamID == NULL || pValue == NULL)
	{
		return E_INVALIDARG;
	}

	if (IsEqualGUID(*pParamID, CLSID_ASIOutputBitrate))
	{
		if (pValue->vt != VT_INT || m_nInTsbps < MINOUTBPS)
		{
			return E_INVALIDARG;
		}
		
		HRESULT hr =S_OK;
		if (m_bDevInited)
		{
			hr = m_tsOut.SetTsRateBps(pValue->intVal);
			if (hr = DTAPI_OK)
			{
				m_DevParam.nOutTsBps = pValue->intVal;
				m_nOutTsbps = pValue->intVal;
			}

		}
		return hr;

	}

	if (IsEqualGUID(*pParamID, CLSID_AutoAdjustOutputBitrate))
	{
		if (m_DevParam.bAutoAdjustOutTsBps != pValue->intVal)
		{
			if (m_DevParam.bAutoAdjustOutTsBps)
			{
				if (m_DevParam.bInputBlockMode)
				{
					return E_FAIL;
				}
				ReCalDevOutRate();
				return S_OK;
			}
		}
	}
	return E_INVALIDARG;

}



//////////////////////////////////////////////////////////////////////////
CZQASIRenderInputPin::CZQASIRenderInputPin( CZQASIRenderFilter *pObj, LPUNKNOWN pUnk, CBaseFilter *pFilter, CCritSec *pLock, CCritSec *pReceiveLock, HRESULT *phr )
:CRenderedInputPin(NAME("CZQASIRenderInputPin"),
				  pFilter,                   // Filter
				  pLock,                     // Locking
				  phr,                       // Return code
				  L"Input"),                 // Pin name
				  m_pReceiveLock(pReceiveLock),
				  m_pZQASIRender(pObj),
				  m_tLast(0)
{
	m_nTotalByte = 0;
}

STDMETHODIMP CZQASIRenderInputPin::Receive( IMediaSample *pSample )
{
	return m_pZQASIRender->Receive(pSample);

	CheckPointer(pSample,E_POINTER);

	CAutoLock lock(m_pReceiveLock);
	PBYTE pbData;

// 	Has the filter been stopped yet?
// 	if (m_pDump->m_hFile == INVALID_HANDLE_VALUE) {
// 		return NOERROR;
// 	}

	REFERENCE_TIME tStart, tStop;
	pSample->GetTime(&tStart, &tStop);

	DbgLog((LOG_TRACE, 1, TEXT("tStart(%s), tStop(%s), Diff(%d ms), Bytes(%d)"),
		(LPCTSTR) CDisp(tStart),
		(LPCTSTR) CDisp(tStop),
		(LONG)((tStart - m_tLast) / 10000),
		pSample->GetActualDataLength()));

	m_tLast = tStart;

	// Copy the data to the file

	HRESULT hr = pSample->GetPointer(&pbData);
	if (FAILED(hr)) {
		return hr;
	}

	int nDataLen = pSample->GetActualDataLength();
	m_nTotalByte += nDataLen;

	//return m_pDump->Write(pbData, pSample->GetActualDataLength());
	return hr;
}

HRESULT CZQASIRenderInputPin::CheckMediaType( const CMediaType * pmt)
{
	CheckPointer(pmt,E_POINTER);

	if (!(pmt->majortype == MEDIATYPE_Stream
				&& (pmt->subtype == MEDIASUBTYPE_MPEG2_TRANSPORT
				|| pmt->subtype == MEDIATYPE_NULL))) {
		return E_INVALIDARG;
	}

	return S_OK;
}

// We don't hold up source threads on Receive
STDMETHODIMP CZQASIRenderInputPin::ReceiveCanBlock()
{
	return m_pZQASIRender->m_DevParam.bInputBlockMode;
}

HRESULT CZQASIRenderInputPin::BreakConnect()
{
	return CRenderedInputPin::BreakConnect();
}

STDMETHODIMP CZQASIRenderInputPin::EndOfStream( void )
{
	_RPT1(0, "stream ended total = %d", m_nTotalByte);
	return CRenderedInputPin::EndOfStream();
}

HRESULT CZQASIRenderInputPin::CompleteConnect( IPin *pReceivePin )
{
	HRESULT hr = CBaseInputPin::CompleteConnect(pReceivePin);
	if (FAILED(hr)) {
		return hr;
	}
	return S_OK;
}

HRESULT CZQASIRenderInputPin::CheckConnect( IPin * pPin )
{
	return CBaseInputPin::CheckConnect(pPin);
}

HRESULT CZQASIRenderInputPin::GetMediaType( int iPosition,CMediaType *pMediaType )
{
	ASSERT(iPosition == 0 || iPosition == 1);

	if(iPosition == 0)
	{
		CheckPointer(pMediaType,E_POINTER);

		pMediaType->SetType(&MEDIATYPE_Stream);
		pMediaType->SetSubtype(&MEDIASUBTYPE_MPEG2_TRANSPORT);
		return S_OK;
	}

	return VFW_S_NO_MORE_ITEMS;
}

