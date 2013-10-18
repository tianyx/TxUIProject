#include "StdAfx.h"
#include "FGlobal.h"

HWND g_hwndLog = NULL;
ST_GLOBAL g_GlobalInfo;

#define LINE_BUFFER_SIZE 4096
void CFWriteLog( LPCTSTR format,... )
{
	va_list ap;
	va_start(ap, format);
	
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
	vswprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	va_end(ap);
	_RPTW0(0, line);
	_RPTW0(0, TEXT("\n"));
	if(g_hwndLog)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}

void CFWriteLog2( LPCTSTR format, va_list& ap )
{
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
	vswprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	_RPTW0(0, line);
	_RPTW0(0, TEXT("\n"));
	if(g_hwndLog)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}

std::wstring GetAppPath()
{
	wchar_t szbuff[256];
	GetModuleFileName(NULL, szbuff, 256);
	std::wstring wsPath = szbuff;
	size_t nPos = wsPath.rfind(L'\\');
	return wsPath.substr(0, nPos);
}

