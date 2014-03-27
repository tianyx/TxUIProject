#include "StdAfx.h"
#include "AutoCritSec.h"

using namespace EMB;

#define DbgLog(_x_) 0
#ifdef _DEBUG
CAutoCritSec::CAutoCritSec()
{
	InitializeCriticalSection(&m_CritSec);
	m_currentOwner = m_lockCount = 0;
	m_fTrace = FALSE;
}

CAutoCritSec::~CAutoCritSec()
{
	DeleteCriticalSection(&m_CritSec);
}



void CAutoCritSec::Lock()
{
	UINT tracelevel=3;
	DWORD us = GetCurrentThreadId();
	DWORD currentOwner = m_currentOwner;
	if (currentOwner && (currentOwner != us)) {
		// already owned, but not by us
		if (m_fTrace) {
			DbgLog((LOG_LOCKING, 2, TEXT("Thread %d about to wait for lock %x owned by %d"),
				GetCurrentThreadId(), &m_CritSec, currentOwner));
			tracelevel=2;
			// if we saw the message about waiting for the critical
			// section we ensure we see the message when we get the
			// critical section
		}
	}
	EnterCriticalSection(&m_CritSec);
	if (0 == m_lockCount++) {
		// we now own it for the first time.  Set owner information
		m_currentOwner = us;

		if (m_fTrace) {
			DbgLog((LOG_LOCKING, tracelevel, TEXT("Thread %d now owns lock %x"), m_currentOwner, &m_CritSec));
		}
	}
}

void CAutoCritSec::Unlock() {
	if (0 == --m_lockCount) {
		// about to be unowned
		if (m_fTrace) {
			DbgLog((LOG_LOCKING, 3, TEXT("Thread %d releasing lock %x"), m_currentOwner, &m_CritSec));
		}

		m_currentOwner = 0;
	}
	LeaveCriticalSection(&m_CritSec);
}
#endif // _DEBUG