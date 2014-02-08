#include "StdAfx.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "Rpc.h"
#include "StrConvert.h"
#pragma comment(lib, "Rpcrt4.lib")


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
	CFWriteLog2(g_hwndLog, format, ap);
	return;
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

void CFWriteLog( DWORD dwLogKey, LPCTSTR format,... )
{
	va_list ap;
	va_start(ap, format);

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
	
	CTxLogManager::GetTxLogMgr()->WriteLog(dwLogKey, line);
#ifndef NOCONSOLEOUTPUT
	if (g_hconsoleHandle)
	{
		DWORD dwWrited = 0;
		WriteConsole(g_hconsoleHandle, TEXT("\n"), 1, &dwWrited, 0);
		WriteConsole(g_hconsoleHandle, line, nLen,&dwWrited,0);
	}
#endif // _DEBUG
	if(g_hwndLog)
	{
		SendMessage(g_hwndLog, MSG_LOGWRITE, (WPARAM)line, 0);
	}

}


void CFWriteLog2Wnd( HWND hwndIn, LPCTSTR format,... )
{
	va_list ap;
	va_start(ap, format);

	CFWriteLog2(hwndIn, format, ap);
	return;

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

#ifndef NOCONSOLEOUTPUT
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

CString Guid2String(const GUID& guidIn )
{
	RPC_CSTR pStr = NULL;
#ifdef VC6DEFINE
	UuidToString((GUID*)&guidIn, &pStr);
#else
	UuidToString(&guidIn, &pStr);

#endif
	CString strTmp;
	if (pStr)
	{
		strTmp = (char*)pStr;
		RpcStringFree(&pStr);
	}
	return strTmp;
}
GUID String2Guid( CString& strIn )
{
	if (strIn.IsEmpty())
	{
		return GUID_NULL;
	}
	GUID guid = GUID_NULL;
	if(UuidFromString((RPC_CSTR)strIn.LockBuffer(), &guid)!= RPC_S_OK)
	{
		ASSERT(FALSE);
	}
	strIn.UnlockBuffer();
	return guid;
}

DWORD TxWaitObjWithQuit( HANDLE hWait, HANDLE hQuit, DWORD dwTimeOut /*= INFINITE*/ )
{
	HANDLE handls[2];
	handls[0] = hWait;
	handls[1] = hQuit;
	return WaitForMultipleObjects(2, handls, FALSE, dwTimeOut);
}

GUID TxGenGuid()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);
	return guid;
}


void GetFilesInFolder(CString& strFolder, vector<CString>& dataOut, LPCTSTR szExtenList, BOOL bSearchInSubFolder /*= TRUE*/)
{
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(strFolder);
	strWildcard.TrimRight(TEXT("\\"));
	strWildcard +=TEXT("\\*.*");
	
	vector<string> vExtens;
	SplitteStrings(szExtenList, vExtens);

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;
		// if it's a directory, recursively search it
		if (finder.IsDirectory() && bSearchInSubFolder)
		{
			CString strPath = finder.GetFilePath();
			GetFilesInFolder(strPath, dataOut, szExtenList, TRUE);
		}
		else
		{
			//save to vec
			//check if match the extens
			if (vExtens.size() == 0)
			{
				dataOut.push_back(finder.GetFilePath());
			}
			else
			{
				CString strFileTmp = finder.GetFilePath();
				CString strExten = GetFileExten(strFileTmp);
				BOOL bMatched = FALSE;
				for (size_t i = 0; i< vExtens.size(); ++i)
				{
					if (strExten.CompareNoCase(vExtens[i].c_str()) == 0)
					{
						bMatched = TRUE;
						break;
					}
				}
				if (bMatched)
				{
					dataOut.push_back(strFileTmp);
				}

			}
		}
	}

	finder.Close();

}

CString GetFileExten( CString& strFile )
{
	CString strRet;
	int nPos = strFile.ReverseFind(_T('.'));
	if (!(nPos < 0))
	{
		 strRet = strFile.Mid(nPos+1);
	}

	return strRet;
}

HINSTANCE GetSelfModuleHandle()
{
	MEMORY_BASIC_INFORMATION mbi;

	return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) 
		? (HMODULE) mbi.AllocationBase : NULL);
}


