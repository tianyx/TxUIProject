#include "StdAfx.h"
#include "TxTimer.h"

VOID CALLBACK TxTimerCallback(
	__in  PVOID lpParameter,
	__in  BOOLEAN TimerOrWaitFired)
{
	CTxTimer* pTimer = (CTxTimer*)lpParameter;
	if (pTimer)
	{
		pTimer->m_tData.pCallback->TxTimerCallbackProc(pTimer->m_tData.dwEnvent, pTimer->m_tData.lparam);
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
		DeleteTimerQueueTimer(m_hQueue, m_tData.hTimer, NULL);
		m_tData.hTimer = NULL;
		m_tData.dwEnvent = NULL;
	}
	
	if (m_hQueue)
	{
		DeleteTimerQueue(m_hQueue);
		m_hQueue = NULL;
	}
}
