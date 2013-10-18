#include "StdAfx.h"
#include "ZQGlobal.h"
#include <algorithm>

HWND g_hwndLog = NULL;
ST_GLOBAL g_GlobalInfo;

using namespace std;
#define LINE_BUFFER_SIZE 4096
void CFWriteLog( LPCTSTR format,... )
{
	va_list ap;
	va_start(ap, format);
	
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
	vsprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	va_end(ap);
	_RPT0(0, line);
	_RPT0(0, TEXT("\n"));
	if(g_hwndLog)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}

void CFWriteLog2( LPCTSTR format, va_list& ap )
{
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
	vsprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	_RPT0(0, line);
	_RPT0(0, TEXT("\n"));
	if(g_hwndLog)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}

std::string GetAppPath()
{
	char szbuff[512];
	GetModuleFileName(NULL, szbuff, 512);
	std::string wsPath = szbuff;
	size_t nPos = wsPath.rfind('\\');
	return wsPath.substr(0, nPos);
}


