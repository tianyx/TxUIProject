#include "StdAfx.h"
#include "TxWndDrawClockMgr.h"
#include "ITxDrawBase.h"

CMutex g_mutexDrawClock(0, TEXT("{CFE58A18-9B6F-4ed6-BDA4-92C07E53DADF}"));

HANDLE CTxWndDrawClockMgr::m_hThread = NULL;
CTxWndDrawClockMgr* CTxWndDrawClockMgr::m_pMgr = NULL;
CTxWndDrawClockMgr::CTxWndDrawClockMgr(void)
{
	m_hThread = NULL;
}

CTxWndDrawClockMgr::~CTxWndDrawClockMgr(void)
{
	if (m_hThread != NULL)
	{
		TerminateThread(m_hThread, 0);
		m_hThread = NULL;
	}
}

CTxWndDrawClockMgr* CTxWndDrawClockMgr::GetTxDrawClockMgr()
{
	if (m_pMgr == NULL)
	{
		m_pMgr = new CTxWndDrawClockMgr;
	}

	return m_pMgr;
}

BOOL CTxWndDrawClockMgr::RegisterDrawClock( ST_DRAWCLOCKDATA& data )
{
	g_mutexDrawClock.Lock(1000);
	MAPCLOCLKDATA::iterator itf = m_mapClockData.find(data.hwnd);
	if (itf == m_mapClockData.end())
	{
		data.nCurrentTick = GetTickCount();
		m_mapClockData[data.hwnd] = data;
		if (m_mapClockData.size() ==1)
		{
			if (m_hThread != NULL)
			{
				ASSERT(FALSE);
				CloseHandle(m_hThread);
			}
			m_hThread = CreateThread(NULL, 0, ClockThreadProc, (void*)(&m_mapClockData), 0, NULL);
			ASSERT(m_hThread != NULL);
		}
	}
		
	g_mutexDrawClock.Unlock();
	return TRUE;
}

BOOL CTxWndDrawClockMgr::UnRegisterDrawClock( ST_DRAWCLOCKDATA& data )
{
	g_mutexDrawClock.Lock(1000);
	m_mapClockData.erase(data.hwnd);
	if (m_mapClockData.size() == 0)
	{
		if (m_hThread != NULL)
		{
			TerminateThread(m_hThread, 0);
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
	}

	g_mutexDrawClock.Unlock();
	
	return TRUE;
}

void CTxWndDrawClockMgr::CheckClock( UINT nClockIn )
{
	g_mutexDrawClock.Lock(1000);
	
	MAPCLOCLKDATA::iterator itb = m_mapClockData.begin();
	MAPCLOCLKDATA::iterator ite = m_mapClockData.end();

	HWND hInvalidWnd = FALSE;
	for (; itb != ite; itb++)
	{
		if (itb->second.nCurrentTick < nClockIn)
		{
			//send draw msg
			if (::IsWindow(itb->first))
			{
				::PostMessage(itb->first, MSG_TXDRAWCLOCK, itb->second.wparam, itb->second.lparam);
				itb->second.nCurrentTick = nClockIn + itb->second.nTickInternal;
			}
			else
			{
				//error;
				hInvalidWnd = itb->first;
			}
		}
	}

	g_mutexDrawClock.Unlock();

	if (hInvalidWnd != NULL)
	{
		ST_DRAWCLOCKDATA data;
		data.hwnd = hInvalidWnd;
		UnRegisterDrawClock(data);
	}
}

void CTxWndDrawClockMgr::Release()
{
	if (m_pMgr)
	{
		delete m_pMgr;
		m_pMgr = NULL;
	}
}


DWORD __stdcall ClockThreadProc( LPVOID pParam )
{
	static INT64 g_nClockRef = 0;

	while(TRUE)
	{
		g_nClockRef = GetTickCount64();
		CTxWndDrawClockMgr* pMgr = CTxWndDrawClockMgr::GetTxDrawClockMgr();
		if (pMgr)
		{
			pMgr->CheckClock(g_nClockRef);
		}
		Sleep(33);
	}
}
