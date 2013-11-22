/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:01
	filename: 	TxTimer.h
	author:		tianyx
	
	purpose:	timer not use wnd
*********************************************************************/
#pragma once
#include <map>
using namespace std;



interface ITxTimerCallbackInterface
{
	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam) = 0;
};

struct ST_TXTIMERDATA
{
	DWORD dwEnvent;
	HANDLE hTimer;
	LPARAM lparam;
	ITxTimerCallbackInterface* pCallback;
	HANDLE hEventGo;
	UINT nflag;
	ST_TXTIMERDATA()
	{
		dwEnvent = 0;
		hTimer = NULL;
		lparam = NULL;
		hEventGo =NULL;
		nflag = 0;
	}
};


typedef map<DWORD, ST_TXTIMERDATA> MAPTXTIMERS;


class CTxTimer
{
public:
	CTxTimer(void);
	~CTxTimer(void);

	HANDLE m_hQueue;
	ST_TXTIMERDATA m_tData;

	BOOL SetTimer(DWORD dwTimerId, DWORD dwInterval, ITxTimerCallbackInterface* pCallback, LPARAM lparam, int nflag = WT_EXECUTEDEFAULT, int nstartbeforeNow = 0);
	void KillTimer();
};
