#include "StdAfx.h"
#include "fmUtility.h"

#ifdef DEBUG
BOOL CCritSecEx::TryLock()
{
	DWORD us = GetCurrentThreadId();
	BOOL bLock = TryEnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>((CCritSec*)(this)));
	if (bLock && 0 == m_lockCount++) 
	{
		// we now own it for the first time.  Set owner information
		m_currentOwner = us;
	}
	return bLock;
}
#endif