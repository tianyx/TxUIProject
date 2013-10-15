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
	ST_TXTIMERDATA()
	{
		dwEnvent = 0;
		hTimer = NULL;
		lparam = NULL;
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
