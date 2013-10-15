#pragma once

#include <map>

struct ST_DRAWCLOCKDATA
{
	HWND hwnd;
	UINT  nTickInternal;
	INT64 nCurrentTick;
	LPARAM lparam;
	WPARAM wparam;
	ST_DRAWCLOCKDATA()
	{
		hwnd = NULL;
		nTickInternal = 33;
		nTickInternal = 0;
		lparam = 0;
		wparam = 0;
	}
};


typedef std::map<HWND, ST_DRAWCLOCKDATA> MAPCLOCLKDATA;

class CTxWndDrawClockMgr
{
private:
	CTxWndDrawClockMgr(void);
	~CTxWndDrawClockMgr(void);
public:
	BOOL RegisterDrawClock(ST_DRAWCLOCKDATA& data);
	BOOL UnRegisterDrawClock(ST_DRAWCLOCKDATA& data);
	

	void CheckClock(UINT nClockIn);
private:
	MAPCLOCLKDATA m_mapClockData;
	
private:
	static HANDLE m_hThread;
public:
	static CTxWndDrawClockMgr* m_pMgr;
	static CTxWndDrawClockMgr* GetTxDrawClockMgr();
	static void Release();

};

DWORD __stdcall ClockThreadProc(LPVOID pParam);
