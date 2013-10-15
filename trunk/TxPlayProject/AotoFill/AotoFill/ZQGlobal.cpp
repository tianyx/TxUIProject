#include "StdAfx.h"
#include "ZQGlobal.h"
#include <algorithm>
#include "objbase.h"

#pragma comment(lib,"ole32.lib")


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




BOOL SplitteStrings( const char* szIn , vector<string>& vOut)
{
	string szData = szIn;
	if (szData.size() == 0)
	{
		return FALSE;
	}

	size_t nBegin = 0;
	for (size_t i = 0; i < szData.size();++i)
	{
		if (szData[i] == ',' || szData[i] == '£¬')
		{
			if (i > nBegin)
			{
				vOut.push_back(szData.substr(nBegin, i - nBegin));
			}
			nBegin = i+1;
		}		
	}

	if (nBegin < szData.size())
	{
		vOut.push_back(szData.substr(nBegin, szData.size() - nBegin));
	}
	return TRUE;
}


int GenRand()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);

	if (guid == GUID_NULL)
	{
		srand(time(NULL));
		return rand();
	}

	srand(guid.Data1 + guid.Data2 +guid.Data3+
		guid.Data4[0]+ guid.Data4[1]+ guid.Data4[2]+guid.Data4[3] +
		guid.Data4[4]+ guid.Data4[5]+ guid.Data4[6]+ guid.Data4[7]);
	return rand();
}