
#ifndef __FM_UTILITY__
#define __FM_UTILITY__

#pragma once

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if((p))(p)->Release();(p) = 0;}
#endif

#ifndef FROM_INTERFACE
#define FROM_INTERFACE(pface, TTo, TFace)	reinterpret_cast<TTo*>(reinterpret_cast<PCHAR>(pface) - (reinterpret_cast<PCHAR>(static_cast<TFace*>(reinterpret_cast<TTo*>(8)))-8))
#endif

#include <streams.h>

template <typename _TT, void (__stdcall _TT::*Init)(), void (__stdcall _TT::*Uninit)()>
class CAutoInitUninitSTD
{
public:
	typedef	_TT* T_HOST;
public:
	inline CAutoInitUninitSTD(_TT* pHost)
	{
		m_pHost = pHost;
		(m_pHost->*Init)();
	}
	~CAutoInitUninitSTD()
	{
		(m_pHost->*Uninit)();
	}

private:
	T_HOST	m_pHost;
};

//////////////////////////////////////////////////////////////////////////
// COverlappedCallObj
// 
//
template<class _C,  class _P>
class COverlappedCallObj
{
private:
	typedef int (__stdcall _C::*_F)(_P&);
	typedef struct  tagThrdParam
	{
		_P* pParam;
		_C* pHost;
		_F  pFunc;
	} THRD_PARAM,*LPTHRD_PARAM;
	_P*	m_pParam;
	_C* m_pHost;
	_F m_pFunc;
public:
	inline COverlappedCallObj(_C* pHost, _F Func) :m_pParam(0), m_pHost(pHost), m_pFunc(Func){};
	inline ~COverlappedCallObj()	{ if (m_pParam) delete m_pParam; }

	inline void Init()			{ if (m_pParam==0) m_pParam = new _P; }
	inline _P* operator->()		{ Init(); return m_pParam;}
	inline _P& operator*()		{ Init(); return *m_pParam;}

	bool operator()();

private:
	static unsigned long __stdcall __ExecThrd(void*);
};
template<class _C,  class _P>
bool COverlappedCallObj<_C,_P>::operator ()()
{
	if (m_pParam==0) return false;
	unsigned long ThreadID;
	LPTHRD_PARAM lpParam = new THRD_PARAM;
	lpParam->pParam = m_pParam;
	lpParam->pHost = m_pHost;
	lpParam->pFunc = m_pFunc;
	HANDLE __hThrd = ::CreateThread(0,0,__ExecThrd,(void*)lpParam, 0, &ThreadID);
	if( !__hThrd )
	{
		delete lpParam;
		return FALSE;
	}
	::WaitForSingleObject(__hThrd, INFINITE);
	DWORD dwExit = 0;
	::GetExitCodeThread(__hThrd, &dwExit);
	::CloseHandle(__hThrd);
	delete lpParam;
	return !!dwExit;
}

template<class _C,  class _P>
unsigned long __stdcall COverlappedCallObj<_C,_P>::__ExecThrd(void* lpParam)
{
	LPTHRD_PARAM lpImpl = (LPTHRD_PARAM)lpParam; 
	return ((lpImpl->pHost)->*(lpImpl->pFunc))(*(lpImpl->pParam));
}
  
class CCritSecEx: public CCritSec
{
public:
	static BOOL TryLock(CCritSec* pLock){return reinterpret_cast<CCritSecEx*>(pLock)->TryLock();}
public:
#ifdef DEBUG
	BOOL TryLock();
#else
	BOOL TryLock(){return ::TryEnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>((CCritSec*)(this)));}
#endif
};

class CFCPSAutoTrace
{
public:

	CFCPSAutoTrace(LPCTSTR lpMsg)
	{
		m_strMsg = lpMsg;
		CString strMsg ;
		strMsg.Format(">>>>AutoTrace Start: %s<<<<:\r\n", m_strMsg);
		OutputDebugString(strMsg);
	}
	~CFCPSAutoTrace()
	{
		CString strMsg;
		strMsg.Format(">>>>AutoTrace Stop: %s<<<<\r\n", m_strMsg);
		OutputDebugString(strMsg);
	}
private:
	CString m_strMsg;
};

#endif //__FM_UTILITY__