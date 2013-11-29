#include "StdAfx.h"
#include "TxTimer.h"

VOID CALLBACK TxTimerCallback(
	__in  PVOID lpParameter,
	__in  BOOLEAN TimerOrWaitFired)
{
	CTxTimer* pTimer = (CTxTimer*)lpParameter;
	if (pTimer)
	{
		ResetEvent(pTimer->m_tData.hEventGo);
		pTimer->m_tData.pCallback->TxTimerCallbackProc(pTimer->m_tData.dwEnvent, pTimer->m_tData.lparam);
		SetEvent(pTimer->m_tData.hEventGo);
		if ((pTimer->m_tData.nflag & WT_EXECUTEONLYONCE) != 0)
		{
			pTimer->KillTimer();
		}
		
		

	}
}


CTxTimer::CTxTimer(void)
{
	m_hQueue = NULL;
}

CTxTimer::~CTxTimer(void)
{
}

BOOL CTxTimer::SetTimer( DWORD dwTimerId, DWORD dwInterval, ITxTimerCallbackInterface* pCallback, LPARAM lparam, int nflag /*= WT_EXECUTEDEFAULT*/, int nstartbeforeNow /*= 0*/)
{
	KillTimer();
	m_hQueue = CreateTimerQueue();
	ASSERT(m_hQueue != NULL);

	ST_TXTIMERDATA& tData = m_tData;
	tData.dwEnvent = dwTimerId;
	tData.pCallback = pCallback;
	tData.lparam = lparam;
	tData.hEventGo = CreateEvent(NULL,TRUE,TRUE,NULL);
	tData.nflag = nflag;
	BOOL bRet = CreateTimerQueueTimer(&tData.hTimer, m_hQueue, TxTimerCallback, (PVOID)this, nstartbeforeNow, dwInterval, nflag);
	if (!bRet)
	{
		ASSERT(FALSE);
		DWORD dwRet = GetLastError();
	}
	return bRet;
}

void CTxTimer::KillTimer()
{
	if (m_tData.hTimer)
	{
		WaitForSingleObject(m_tData.hEventGo, INFINITE);
		DeleteTimerQueueTimer(m_hQueue, m_tData.hTimer, NULL);
		m_tData.hTimer = NULL;
		m_tData.dwEnvent = NULL;
		CloseHandle(m_tData.hEventGo);
		m_tData.hEventGo = NULL;
	}
	
	if (m_hQueue)
	{
		DeleteTimerQueue(m_hQueue);
		m_hQueue = NULL;
	}
}
