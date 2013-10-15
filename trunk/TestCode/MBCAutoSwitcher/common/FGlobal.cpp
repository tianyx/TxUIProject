#include "StdAfx.h"
#include "FGlobal.h"
#include "TxFontLoader.h"

using namespace std;

HWND g_hwndLog = NULL;
HANDLE g_hconsoleHandle = NULL;
CTxFontLoader*  g_pFontLoader = NULL;

extern int 	g_nEndBitRateCheckInterval  = 1000;

extern int 	g_SOCK_INTERVAL_RECONN  = 5000;
extern int g_INTERVAL_LIVECHECK  = 5000;
extern int g_DELAY_BEFORELIVECHECK  = 2000;
extern int g_nDefMBCDrawItemCX = 200;
extern int g_nDefMBCDrawItemCY = 32;
extern int g_nDefMBCDrawObjIntervalCX = 40;
extern int g_nDefMBCDrawObjIntervalCY = 60;
extern int g_nDefMBCOneSendPackSize = 1316;
extern int g_nMBCSockOptTTL = 1;

extern int g_nScreenCX = GetSystemMetrics(SM_CXSCREEN);
extern int g_nScreenCY = GetSystemMetrics(SM_CYSCREEN);
extern int g_nScrollBarWidth = GetSystemMetrics(SM_CXHSCROLL);


#define LINE_BUFFER_SIZE 4096
void CFWriteLog( LPCTSTR format,... )
{
	va_list ap;
	va_start(ap, format);
	return CFWriteLog2(g_hwndLog, format, ap);
	
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));


#ifdef _UNICODE
	vswprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	
	va_end(ap);
	_RPTW0(0, line);
	_RPTW0(0, TEXT("\n"));
#else
	vsprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	
	va_end(ap);
	_RPT0(0, line);
	_RPT0(0, TEXT("\n"));
#endif // _UNICODE
	


	if(g_hwndLog)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}


void CFWriteLog2Wnd( HWND hwndIn, LPCTSTR format,... )
{
	va_list ap;
	va_start(ap, format);

	return CFWriteLog2(hwndIn, format, ap);

	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
#ifdef _UNICODE
	vswprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	va_end(ap);
	_RPTW0(0, TEXT("\n"));
	_RPTW0(0, line);
#else
	vsprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	va_end(ap);
	_RPT0(0, TEXT("\n"));
	_RPT0(0, line);
#endif // _UNICODE

	if(hwndIn)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}

void CFWriteLog2( HWND hwndIn, LPCTSTR format, va_list& ap )
{
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));

	int nLen = 0;
#ifdef _UNICODE
	vswprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	nLen = wcslen(line);

	_RPTW0(0, TEXT("\n"));
	_RPTW0(0, line);

#else
	vsprintf_s(line, LINE_BUFFER_SIZE-1, format, ap);
	nLen = strlen(line);

	_RPT0(0, TEXT("\n"));
	_RPT0(0, line);

#endif // _UNICODE
	va_end(ap);

#ifdef _DEBUG
	if (g_hconsoleHandle)
	{
		DWORD dwWrited = 0;
		WriteConsole(g_hconsoleHandle, TEXT("\n"), 1, &dwWrited, 0);
		WriteConsole(g_hconsoleHandle, line, nLen,&dwWrited,0);
	}
#endif // _DEBUG
	if(hwndIn)
	{
		SendMessage(hwndIn, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}

std::wstring GetAppPathW()
{
	wchar_t szbuff[256];
	GetModuleFileNameW(NULL, szbuff, 256);
	std::wstring wsPath = szbuff;
	size_t nPos = wsPath.rfind(L'\\');
	return wsPath.substr(0, nPos);
}

std::string GetAppPath()
{
	char szbuff[256];
	GetModuleFileNameA(NULL, szbuff, 256);
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



SOCKADDR_IN GetAddrFromStr( CString& strAddrIn )
{
	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	int nPos = strAddrIn.Find(TEXT(":"));
	if (nPos != -1)
	{
		addr.sin_family = AF_INET;
		addr.sin_port =htons(atoi(strAddrIn.Mid(nPos+1)));
		addr.sin_addr.S_un.S_addr = inet_addr(strAddrIn.Left(nPos));
	}
	else
	{
		ASSERT(FALSE);
	}

	return addr;
}

BOOL IsEqualAddr( const SOCKADDR_IN& addr1, const SOCKADDR_IN& addr2 )
{
	return addr1.sin_family == AF_INET  && addr1.sin_family == addr2.sin_family
		&& addr1.sin_port == addr2.sin_port
		&& addr1.sin_addr.S_un.S_addr == addr2.sin_addr.S_un.S_addr;
}

std::string Addr2String( const SOCKADDR_IN& addr1 )
{
	std::string strTmp = inet_ntoa(addr1.sin_addr);
	int nPortTmp = ntohs(addr1.sin_port);
	char buff[32];
	ZeroMemory(buff, 32);
	sprintf_s(buff, TEXT("%s:%d"), strTmp.c_str(), nPortTmp);
	strTmp = buff;
	return strTmp;
}

bool operator==( const SOCKADDR_IN& addr1, const SOCKADDR_IN& addr2 )
{
	return addr1.sin_family == AF_INET
		&& addr1.sin_family == addr2.sin_family
		&& addr1.sin_port == addr2.sin_port
		&& addr1.sin_addr.S_un.S_addr == addr2.sin_addr.S_un.S_addr;

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