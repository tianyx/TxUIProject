/********************************************************************
	created:	2013/05/22
	created:	2013:5
	filename: 	ZQGlobal.h
	file base:	ZQGlobal
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#define  MSG_LOGWRITE WM_USER + 1222
#include <string>
#include <vector>
extern HWND g_hwndLog;


struct ST_GLOBAL
{
	std::string szAppPath;
	std::string szIniPath;
	std::string szCfgSrcPath;
	std::vector<std::string> vModuleName;
	std::vector<std::string> vModuleKey;
};

extern ST_GLOBAL g_GlobalInfo;

void CFWriteLog( LPCTSTR format,...);
void CFWriteLog2( LPCTSTR format, va_list& ap );

std::string GetAppPath();

