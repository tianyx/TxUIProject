/********************************************************************
	created:	2013/11/22
	created:	$2013:11:22
	filename: 	AutoCritSec.h
	file base:	AutoCritSec
	file ext:	h
	author:		tianyx
	
	purpose:	自动临界区类/临界区自动锁类
*********************************************************************/
#pragma once

class CAutoCritSec {

	// make copy constructor and assignment operator inaccessible

	CAutoCritSec(const CAutoCritSec &refCritSec);
	CAutoCritSec &operator=(const CAutoCritSec &refCritSec);

	CRITICAL_SECTION m_CritSec;
public:
// 	void ResetLock()
// 	{
// 		DeleteCriticalSection(&m_CritSec);
// 		InitializeCriticalSection(&m_CritSec);
// 	}

#ifdef _DEBUG
public:
	DWORD   m_currentOwner;
	DWORD   m_lockCount;
	BOOL    m_fTrace;        // Trace this one
public:
	CAutoCritSec();
	~CAutoCritSec();
	void Lock();
	void Unlock();
#else

public:
	CAutoCritSec() {
		InitializeCriticalSection(&m_CritSec);
	};

	~CAutoCritSec() {
		DeleteCriticalSection(&m_CritSec);
	};

	void Lock() {
		EnterCriticalSection(&m_CritSec);
	};

	void Unlock() {
		LeaveCriticalSection(&m_CritSec);
	};
#endif
};


class CAutoLock {

	// make copy constructor and assignment operator inaccessible

	CAutoLock(const CAutoLock &refAutoLock);
	CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
	CAutoCritSec * m_pLock;

public:
	CAutoLock(CAutoCritSec * plock)
	{
		m_pLock = plock;
		m_pLock->Lock();
	};

	~CAutoLock() {
		m_pLock->Unlock();
	};
};