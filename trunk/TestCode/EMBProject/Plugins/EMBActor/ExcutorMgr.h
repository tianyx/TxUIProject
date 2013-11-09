/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	ExcutorMgr.h
	author:		tianyx
	
	purpose:	excutor manager
*********************************************************************/
#pragma once
#include "fglobal.h"
#include "EmbStructDef.h"
#include <map>
using namespace std;


typedef map<TXGUID, ST_EXCUTORINFO> MAPEXCUTORS;

class CExcutorMgr
{
private:
	CExcutorMgr(void);
	virtual ~CExcutorMgr(void);

public:
	BOOL Init(LPCTSTR strExcPathIn);
	HRESULT Run();
	HRESULT Stop();
	GUID StartNewExcutor();
	BOOL StopExcutor(const GUID& guidIn);
	HRESULT SendMessageToExcutor(TXGUID& guid, WPARAM wParam, LPARAM lParam);
public:
	HRESULT OnExcutorMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	HWND m_hMessageWnd;
	HANDLE m_hLoopThread;

private:
	CString m_strExcPath;
	int m_nMinExcutorId;
	int m_nMaxExcutorId;
	MAPEXCUTORS m_mapExcutors;
	static CExcutorMgr* m_spExcMgr;

public:
	static CExcutorMgr* GetExcutorMgr();
	static BOOL Release();
};
