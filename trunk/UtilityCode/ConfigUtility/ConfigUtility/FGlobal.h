#pragma once

#define  MSG_LOGWRITE WM_USER + 1222
#include <string>
extern HWND g_hwndLog;

struct ST_GLOBAL
{
	std::wstring wsAppPath;
	std::wstring wsIniPath;
	std::wstring wsCfgSrcPath;

};

extern ST_GLOBAL g_GlobalInfo;

void CFWriteLog( LPCTSTR format,...);
void CFWriteLog2( LPCTSTR format, va_list& ap );

std::wstring GetAppPath();