#include "StdAfx.h"
#include "TxPerformMon.h"
#include <map>
#include "Pdh.h"
#pragma comment(lib, "Pdh.lib")
using namespace std;
typedef map<CString, INT64> MAPNETVALUE;
//////////////////////////////////////////////////////////////////////////
DWORD __stdcall PerfLoopProc(LPVOID lparam)
{
	CTxPerformMon* pObj = (CTxPerformMon*)lparam;
	if (pObj)
	{
		pObj->PerfRunLoop();
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

CTxPerformMon::CTxPerformMon(void)
{
	m_hThreadPerf = NULL;
	m_hEnentQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
	MEMORYSTATUSEX mStatus={0}; 
	mStatus.dwLength = sizeof(mStatus);    
	GlobalMemoryStatusEx(&mStatus);
	m_MemPhysicalSize = mStatus.ullTotalPhys / 1024 / 1024;
	if (m_MemPhysicalSize == 0)
	{
		ASSERT(FALSE);
		m_MemPhysicalSize = 1;
	}
}

CTxPerformMon::~CTxPerformMon(void)
{
}

void CTxPerformMon::GetAllObjName( VECOBJNAME& vObjOut )
{
	DWORD nBuffSize = 0;
	PdhEnumObjects(NULL, NULL, NULL,  &nBuffSize, PERF_DETAIL_WIZARD, TRUE);
	if (nBuffSize == 0)
	{
		return;
	}
	TCHAR* pBuff = new TCHAR[nBuffSize];
	PdhEnumObjects(NULL, NULL, pBuff, &nBuffSize, PERF_DETAIL_WIZARD, TRUE);
	TCHAR* pIdx = pBuff;
	TCHAR* pSubStr =pBuff;
	int nCount = 0;
	while(nCount < nBuffSize)
	{
		if (*pIdx == TEXT('\0'))
		{
			vObjOut.push_back(pSubStr);
			TRACE("\n");
			TRACE(pSubStr);
			pSubStr = pIdx +1;
			if (*pSubStr == TEXT('\0'))
			{
				break;
			}
		}

		++nCount;
		++pIdx;
	}
}

void CTxPerformMon::GetAllSubItem( LPCTSTR strObjIn, VECSUBITEMNAME& vItemsOut )
{
	PDH_STATUS  pdhStatus               = ERROR_SUCCESS;
	LPTSTR      szCounterListBuffer     = NULL;
	DWORD       dwCounterListSize       = 0;
	LPTSTR      szInstanceListBuffer    = NULL;
	DWORD       dwInstanceListSize      = 0;
	LPTSTR      szThisInstance          = NULL;


	// Determine the required buffer size for the data. 
	pdhStatus = PdhEnumObjectItems (
		NULL,                   // real time source
		NULL,                   // local machine
		TEXT("Process"),        // object to enumerate
		szCounterListBuffer,    // pass NULL and 0
		&dwCounterListSize,     // to get length required
		szInstanceListBuffer,   // buffer size 
		&dwInstanceListSize,    // 
		PERF_DETAIL_WIZARD,     // counter detail level
		0); 


	// Allocate the buffers and try the call again.
	szCounterListBuffer = (LPTSTR)malloc (
		(dwCounterListSize * sizeof (TCHAR)));
	szInstanceListBuffer = (LPTSTR)malloc (
		(dwInstanceListSize * sizeof (TCHAR)));

	if ((szCounterListBuffer != NULL) &&
		(szInstanceListBuffer != NULL)) 
	{
		pdhStatus = PdhEnumObjectItems (
			NULL,                 // real time source
			NULL,                 // local machine
			strObjIn,      // object to enumerate
			szCounterListBuffer,  // buffer to receive counter list
			&dwCounterListSize, 
			szInstanceListBuffer, // buffer to receive instance list 
			&dwInstanceListSize,    
			PERF_DETAIL_WIZARD,   // counter detail level
			0);

		if (pdhStatus == ERROR_SUCCESS) 
		{

			// Walk the instance list. The list can contain one
			// or more null-terminated strings. The last string 
			// is followed by a second null-terminator.
			for (szThisInstance = szInstanceListBuffer;
				*szThisInstance != 0;
				szThisInstance += lstrlen(szThisInstance) + 1) 
			{
				TRACE ("\n  %s", szThisInstance);
				vItemsOut.push_back(szThisInstance);
			}
		}
		else 
		{
			TRACE("\nPdhEnumObjectItems failed with %ld.", pdhStatus);
		}
	} 
	else 
	{
		TRACE ("\nUnable to allocate buffers");
		pdhStatus = ERROR_OUTOFMEMORY;
	}

	if (szCounterListBuffer != NULL) 
		free (szCounterListBuffer);

	if (szInstanceListBuffer != NULL) 
		free (szInstanceListBuffer);
}

BOOL CTxPerformMon::Run()
{
	Stop();
	ResetEvent(m_hEnentQuit);
	m_hThreadPerf = CreateThread(NULL, 0, PerfLoopProc, (LPVOID)this, 0, 0);
	ASSERT(m_hThreadPerf != NULL);
	return TRUE;
}

BOOL CTxPerformMon::Stop()
{
	if (m_hThreadPerf)
	{
		SetEvent(m_hEnentQuit);
		WaitForSingleObject(m_hThreadPerf, INFINITE);
		m_hThreadPerf = NULL;
	}

	return TRUE;
}

BOOL CTxPerformMon::PerfRunLoop()
{
	HQUERY hquery = NULL;
	HCOUNTER hctProcess = NULL;
	HCOUNTER hctMem		= NULL;
	HCOUNTER hctDisk	= NULL;
	HCOUNTER hctNetBandWidth = NULL;
	HCOUNTER hctNetIO	= NULL;
	PDH_STATUS status =PdhOpenQuery(NULL, 0,  &hquery);
	if (status != ERROR_SUCCESS)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	status = PdhAddCounter(hquery, PFCNAME_ProcessorIdle, 0,  &hctProcess);
	ASSERT(status  == ERROR_SUCCESS);
	status = PdhAddCounter(hquery, PFCNAME_MemoryAvailableMBytes, 0, &hctMem);
	ASSERT(status  == ERROR_SUCCESS);
	status = PdhAddCounter(hquery, PFCNAME_DiskIdle, 0, &hctDisk);
	ASSERT(status  == ERROR_SUCCESS);
	status = PdhAddCounter(hquery, PFCNAME_NetworkBandwidth, 0, &hctNetBandWidth);
	ASSERT(status  == ERROR_SUCCESS);
	status = PdhAddCounter(hquery, PFCNAME_NetworkRealIO, 0, &hctNetIO);
	ASSERT(status  == ERROR_SUCCESS);

	if (hctProcess == NULL || hctMem == NULL || hctDisk == NULL || hctNetIO == NULL
		|| hctNetBandWidth == NULL)
	{
		ASSERT(FALSE);
		PdhCloseQuery(hquery);
		return FALSE;
	}

	status = PdhCollectQueryData(hquery);
	ASSERT(status  == ERROR_SUCCESS);
	while(TRUE)
	{
		if (WaitForSingleObject(m_hEnentQuit, 1000) ==WAIT_OBJECT_0)
		{
			break;
		}

		status = PdhCollectQueryData(hquery);
		ASSERT(status  == ERROR_SUCCESS);

		//getvalue
		DWORD crType = 0;
		PDH_FMT_COUNTERVALUE   fmtValue;

		CAutoLock lock(&m_csRes);
		//processor
		status = PdhGetFormattedCounterValue(hctProcess, PDH_FMT_LONG, &crType, &fmtValue);
		if (status == ERROR_SUCCESS)
		{
			m_nProcessorUsage = 100 - fmtValue.longValue;
		}
		else
		{
			ASSERT(FALSE);
			m_nProcessorUsage = -1;
		}
		TRACE("\nprocessor usage:%d", m_nProcessorUsage);


		//mem
		status = PdhGetFormattedCounterValue(hctMem, PDH_FMT_LONG, &crType, &fmtValue);
		if (status == ERROR_SUCCESS)
		{
			m_nMemUsage = (m_MemPhysicalSize -fmtValue.longValue)*100/m_MemPhysicalSize;
		}
		else
		{
			ASSERT(FALSE);
			m_nMemUsage = -1;
		}
		TRACE("\nmem usage:%d", m_nMemUsage);


		//disk
		status = PdhGetFormattedCounterValue(hctDisk, PDH_FMT_LONG, &crType, &fmtValue);
		if (status == ERROR_SUCCESS)
		{
			m_nDiskUsage = 100 -fmtValue.longValue;
		}
		else
		{
			ASSERT(FALSE);
			m_nDiskUsage = -1;
		}
		TRACE("\ndisk usage:%d", m_nDiskUsage);

		//net bandwidth
		MAPNETVALUE mapBands;
		DWORD dwBuffsize = 0;
		DWORD dwCount = 0;
		PDH_FMT_COUNTERVALUE_ITEM *pItems = 0;
		status = PdhGetFormattedCounterArray(hctNetBandWidth, PDH_FMT_LARGE, &dwBuffsize, &dwCount, pItems);
		if (dwBuffsize > 0)
		{
			pItems =(PDH_FMT_COUNTERVALUE_ITEM *) new byte[dwBuffsize];
			status = PdhGetFormattedCounterArray(hctNetBandWidth, PDH_FMT_LARGE, &dwBuffsize, &dwCount, pItems);
			if (status == ERROR_SUCCESS)
			{
				for (DWORD i = 0; i < dwCount; i++)
				{
					TRACE("\nnet band:%s = %I64d", pItems[i].szName, pItems[i].FmtValue.largeValue);
					INT64 nBandWidthKbps = pItems[i].FmtValue.largeValue/1024;
					if (nBandWidthKbps > 0)
					{
						mapBands[pItems[i].szName] = nBandWidthKbps;
					}
				}

			}
			if (pItems)
			{
				delete pItems;
				pItems = NULL;
			}
		}

		//net byte/sec
		dwBuffsize = 0;
		dwCount = 0;
		pItems = NULL;
		status = PdhGetFormattedCounterArray(hctNetIO, PDH_FMT_LARGE, &dwBuffsize, &dwCount, pItems);
		if (dwBuffsize > 0)
		{
			pItems =(PDH_FMT_COUNTERVALUE_ITEM *) new byte[dwBuffsize];
			status = PdhGetFormattedCounterArray(hctNetIO, PDH_FMT_LARGE, &dwBuffsize, &dwCount, pItems);
			if (status == ERROR_SUCCESS)
			{
				//find max io bps
				int nMaxNetIo = -1;
				for (DWORD i = 0; i < dwCount; i++)
				{
					INT64 nCurrKbps = pItems[i].FmtValue.largeValue/1024*8;
					TRACE("\nnet io:%s = %I64dk", pItems[i].szName, nCurrKbps);
					MAPNETVALUE::iterator itf = mapBands.find(pItems[i].szName);
					if (itf != mapBands.end())
					{
						INT64 nNetIoTmp = nCurrKbps*100/itf->second;
						if (nNetIoTmp > nMaxNetIo)
						{
							nMaxNetIo = nNetIoTmp;
						}

					}
				}
				m_nNetUsage = nMaxNetIo;
				TRACE("\n netusage = %d", m_nNetUsage);

			}
			if (pItems)
			{
				delete pItems;
				pItems = NULL;
			}
		}

	}

	PdhCloseQuery(hquery);
	
	return TRUE;


}

BOOL CTxPerformMon::GetUsage( ENUMRESMONTYPE typeIn )
{
	CAutoLock lock(&m_csRes);
	if (typeIn == restype_Processor)
	{
		return m_nProcessorUsage;
	}
	else if (typeIn == restype_PhysicalDisk)
	{
		return m_nDiskUsage;
	}
	else if (typeIn == restype_Memory)
	{
		return m_nMemUsage;
	}
	else if (typeIn == restype_Network)
	{
		return m_nNetUsage;
	}
	else
	{
		return -1;
	}
}
