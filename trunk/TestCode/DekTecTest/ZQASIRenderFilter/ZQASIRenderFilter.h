/********************************************************************
	created:	2013/07/23
	created:	2013:7
	filename: 	ZQASIRenderFilter.h
	file base:	ZQASIRenderFilter
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ZQASIRENDERFILTER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ZQASIRENDERFILTER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once

#ifdef ZQASIRENDERFILTER_EXPORTS
#define ZQASIRENDERFILTER_API __declspec(dllexport)
#else
#define ZQASIRENDERFILTER_API __declspec(dllimport)
#endif

#include <streams.h>
#include <initguid.h>
#include "IASIDevpreConfig.h"
#include "dtapi.h"

#define MAXDTABUFFLEN 0x02000000
#define DTAWAITBUFFMIN 0x0100000
#define WRITEBUFSIZE 0x10000
#define INITLOADSIZE 0x01000000

struct ST_DTABUFF 
{
	byte buffer[MAXDTABUFFLEN];
	int nUsed;
	ST_DTABUFF()
	{
		nUsed = 0;
	}
};



class CZQASIRenderInputPin;

class CZQASIRenderFilter: public CBaseFilter//, public IASIDevPreConfig
{


public:
	DECLARE_IUNKNOWN

	// TODO: 在此添加您的方法。
	// Constructor
	CZQASIRenderFilter(
		LPUNKNOWN pUnk,
		HRESULT *phr);

	virtual ~CZQASIRenderFilter();
	// Pin enumeration
	CBasePin * GetPin(int n);
	int GetPinCount();

	// Open and close the file as necessary
	STDMETHODIMP Run(REFERENCE_TIME tStart);
	STDMETHODIMP Pause();
	STDMETHODIMP Stop();

	HRESULT Receive(IMediaSample *pSample);
public:
	CZQASIRenderInputPin *m_pPin;          // A simple rendered input pin
	CCritSec m_Lock;                // Main renderer critical section
	CCritSec m_ReceiveLock;         // Sublock for received samples

public:
	static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
	// 	// Overriden to say what interfaces we support where
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	//////////////////////////////////////////////////////////////////////////
	// interface for preset dev
	STDMETHODIMP init_DevPreConfig (
		ST_DEVPARAM *paramIn      //
		);

	STDMETHODIMP get_DevPreConfig (
		ST_DEVPARAM* paramOut      // 
		);

	STDMETHODIMP get_value (
		const GUID *pParamID,
		/* [in] */ VARIANT *pValue 
		);

	STDMETHODIMP set_value (
		const GUID *pParamID,
		/* [in] */ const VARIANT *pValue 
		);

	ST_DEVPARAM m_DevParam;

	//////////////////////////////////////////////////////////////////////////
public:
	//device
	BOOL m_bDevInited;
	HANDLE m_hDtaThrd;
	HANDLE m_hReadThrd;
	HANDLE m_hCalBitrateThrd;

	HANDLE m_readlock0;
	HANDLE m_outLock0;
	HANDLE m_readlock1;
	HANDLE m_outlock1;

	int StartRecvLoop();
	int CloseRecvLoop();

	DtDevice m_dvc;
	DtOutpChannel m_tsOut;
	int g_nFifoSize;
	int g_nFifoMaxSize;

	HANDLE m_hfexcLock; //exchange buffer lock
	ST_DTABUFF m_buffExchange;

	ST_DTABUFF m_buff0;
	ST_DTABUFF m_buff1;

	int InitDtaDevice();
	int UnInitDtaDevice();
	
	INT64 m_nInTsbps; //bps
	int m_nOutTsbps;
	INT64 m_nCalRateByte;
	int m_nCalRateDuration;
	BOOL m_bResetDevOutBps;
	int ReCalDevOutRate();
	
	
//////////////////////////////////////////////////////////////////////////
};



//  Pin object

class CZQASIRenderInputPin : public CRenderedInputPin
{
	CZQASIRenderFilter    * const m_pZQASIRender;  // Main renderer object
	CCritSec * const m_pReceiveLock;    // Sample critical section
	REFERENCE_TIME m_tLast;             // Last sample receive time

	INT64 m_nTotalByte;

public:

	CZQASIRenderInputPin(CZQASIRenderFilter *pObj,
		LPUNKNOWN pUnk,
		CBaseFilter *pFilter,
		CCritSec *pLock,
		CCritSec *pReceiveLock,
		HRESULT *phr);

	// Do something with this media sample
	STDMETHODIMP Receive(IMediaSample *pSample);
	STDMETHODIMP EndOfStream(void);
	STDMETHODIMP ReceiveCanBlock();

	//////////////////////////////////////////////////////////////////////////
	//must overrided  when pin connecting.
	// returns the preferred formats for a pin
	virtual HRESULT GetMediaType(int iPosition,CMediaType *pMediaType);

	// Check if the pin can support this specific proposed type and format
	HRESULT CheckMediaType(const CMediaType * pmt);
	// check that the connection is ok before verifying it
	// can be overridden eg to check what interfaces will be supported.
	virtual HRESULT CheckConnect(IPin * pPin);
	// Set and release resources required for a connection
	virtual HRESULT CompleteConnect(IPin *pReceivePin);
	//////////////////////////////////////////////////////////////////////////
	// Break connection
	HRESULT BreakConnect();

// 	// Track NewSegment
// 	STDMETHODIMP NewSegment(REFERENCE_TIME tStart,
// 		REFERENCE_TIME tStop,
// 		double dRate);
};



// ZQASIRENDERFILTER_API int fnZQASIRenderFilter(void);
