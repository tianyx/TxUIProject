#include "stdafx.h"
#include "MiniDump.h"
#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")


CMiniDump::CMiniDump()
{
	::SetUnhandledExceptionFilter(TopLevelFilter);
}

CMiniDump::~CMiniDump()
{
}

LONG WINAPI CMiniDump::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	LONG ret = EXCEPTION_CONTINUE_SEARCH;
	
	char lpFileName[MAX_PATH];
	::GetModuleFileName(NULL,lpFileName,MAX_PATH);
	CString strPathName(lpFileName);
	strPathName = strPathName.Left(strPathName.ReverseFind('\\'));

	SYSTEMTIME systime;
	GetLocalTime(&systime);

	char szFile[MAX_PATH] = {0};
	sprintf(szFile, "%s\\%4d.%02d.%02d_%02d.%02d.%02d.dmp", strPathName, systime.wYear,
			systime.wMonth, systime.wDay, systime.wHour, systime.wMinute,
			systime.wSecond);
	

	HANDLE hFile = ::CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;
		
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;
		// write the dump
		DWORD minitype = MiniDumpNormal |		 
						MiniDumpWithDataSegs | 
						MiniDumpWithFullMemory |
						MiniDumpWithHandleData;
		BOOL bOK = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(),
			hFile, (MINIDUMP_TYPE)minitype, &ExInfo, NULL, NULL );
		ret = EXCEPTION_EXECUTE_HANDLER;
		::CloseHandle(hFile);
	}
	return ret;
}
