#pragma once
#include <vector>
#include <string>
////log output macro//////////////////////////////////////////////////////////////////////
#define  MSG_LOGWRITE WM_USER + 1222

#define MACRO_EDITLOGOUTPUT \
if (message == MSG_LOGWRITE)\
{\
	if (wParam != NULL)\
	{\
		TCHAR* szMsg = (TCHAR*) wParam;\
		m_edtLog.SetSel(-1,-1);\
		CString strMsg = TEXT("\r\n");\
		strMsg += szMsg;\
		m_edtLog.ReplaceSel(strMsg);\
	}\
}\

//////////////////////////////////////////////////////////////////////////
#include <string>
extern HWND g_hwndLog;

#ifndef NOTUSEFGGLOBAL
struct ST_GLOBAL
{
	std::wstring wsAppPath;
	std::wstring wsIniPath;
	std::wstring wsCfgSrcPath;

};
extern ST_GLOBAL g_GlobalInfo;
#endif // USEFGGLOBAL

void CFWriteLog( LPCTSTR format,...);
void CFWriteLog2( LPCTSTR format, va_list& ap );

BOOL SplitteStrings( const char* szIn , std::vector<std::string>& vOut);

std::string GetAppPath();
std::wstring GetAppPathW();

int GenRand();