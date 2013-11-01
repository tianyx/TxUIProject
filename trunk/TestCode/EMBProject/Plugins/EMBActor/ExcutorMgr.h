#pragma once
#include "fglobal.h"
#include <map>
using namespace std;

struct ST_EXCUTORINFO
{
	TXGUID excutorId;
	HWND hwnd;
	DWORD hProcessId;
};
typedef map<TXGUID, ST_EXCUTORINFO> MAPEXCUTORS;

class CExcutorMgr
{
private:
	CExcutorMgr(void);
	virtual ~CExcutorMgr(void);

public:
	BOOL SetExcutorPath(LPCTSTR strPathIn);
	GUID StartNewExcutor();
	BOOL StopExcutor(const GUID& guidIn);
	HRESULT SendMessageToExcutor(TXGUID& guid, WPARAM wParam, LPARAM lParam);
public:
	HRESULT OnExcutorMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	HWND m_hMessageWnd;
private:
	int m_nMinExcutorNum;
	int m_nMaxExcutorNum;
	MAPEXCUTORS m_mapExcutors;
	static CExcutorMgr* m_spExcMgr;

public:
	static CExcutorMgr* GetExcutorMgr();
	static BOOL Release();
};
