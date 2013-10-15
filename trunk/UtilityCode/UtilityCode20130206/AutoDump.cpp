/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	AutoDump.cpp
	file base:	AutoDump
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#ifndef _DEBUG
#include "AutoDump.h"
#include "dbghelp.h"
#include "strsafe.h"

#pragma comment(lib, "dbghelp.lib")


LONG WINAPI MyExceptionCrashFunc(struct _EXCEPTION_POINTERS *pExceptionPointers);
LONG WINAPI MyExceptionCrashFunc(struct _EXCEPTION_POINTERS *pExceptionPointers)
{
	SetErrorMode( SEM_NOGPFAULTERRORBOX );   

	//收集信息   
	CString strBuild;   
	strBuild.Format("Build: %s %s", __DATE__, __TIME__);   
	CString strError;   
	HMODULE hModule;   
	CHAR szModuleName[MAX_PATH] = "";   
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)pExceptionPointers->ExceptionRecord->ExceptionAddress, &hModule);   
	GetModuleFileName(hModule, szModuleName, ARRAYSIZE(szModuleName));   
	strError.AppendFormat("%s %d , %d ,%d.", szModuleName,pExceptionPointers->ExceptionRecord->ExceptionCode, pExceptionPointers->ExceptionRecord->ExceptionFlags, pExceptionPointers->ExceptionRecord->ExceptionAddress);   
	CString strAppFile = szModuleName;
	int nFind = strAppFile.ReverseFind('\\');
	CString strAppPath = strAppFile.Left(nFind+1);
	strAppPath += "dump";
	CString strAppName = strAppFile.Mid(nFind+1);
	//{add by tian_yx 2011-07-22 09:36
// 	char szMyDir[MAX_PATH];   
// 	ZeroMemory(szMyDir, sizeof(szMyDir));
// 	::SHGetSpecialFolderPath(NULL,szMyDir,CSIDL_LOCAL_APPDATA,0); 
// 	strAppPath = szMyDir;
// 	strAppPath +=TEXT("\\EZworking");
// 	CreateDirectory(strAppPath, NULL);
// 	strAppPath +=TEXT("\\dump");
	CreateDirectory(strAppPath, NULL);
	//}end by tian_yx 2011-07-22 09:36

	//生成 mini crash dump   
	BOOL bMiniDumpSuccessful;   
	CHAR szFileName[512];    
	DWORD dwBufferSize = MAX_PATH;   
	HANDLE hDumpFile;   
	SYSTEMTIME stLocalTime;   
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;   
	GetLocalTime( &stLocalTime );   
	CreateDirectory( strAppPath, NULL );   
	StringCchPrintf( szFileName, MAX_PATH, "%s\\%04d%02d%02d-%02dh%02dm%02ds.dmp",    
		strAppPath,     
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,    
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,    
		GetCurrentProcessId(), GetCurrentThreadId());  

	hDumpFile = CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE,    
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);   

	MINIDUMP_USER_STREAM UserStream[2];   
	MINIDUMP_USER_STREAM_INFORMATION UserInfo;   
	UserInfo.UserStreamCount = 1;   
	UserInfo.UserStreamArray = UserStream;   
	UserStream[0].Type = CommentStreamW;   
	UserStream[0].BufferSize = strBuild.GetLength()*sizeof(WCHAR);   
	UserStream[0].Buffer = strBuild.GetBuffer();   
	UserStream[1].Type = CommentStreamW;   
	UserStream[1].BufferSize = strError.GetLength()*sizeof(WCHAR);   
	UserStream[1].Buffer = strError.GetBuffer();   

	ExpParam.ThreadId = GetCurrentThreadId();   
	ExpParam.ExceptionPointers = pExceptionPointers;   
	ExpParam.ClientPointers = TRUE;   

	MINIDUMP_TYPE MiniDumpWithDataSegs = (MINIDUMP_TYPE)(
		MiniDumpNormal    
		| MiniDumpWithHandleData    
		| MiniDumpWithUnloadedModules    
		| MiniDumpWithIndirectlyReferencedMemory    
		| MiniDumpScanMemory    
		| MiniDumpWithProcessThreadData    
		| MiniDumpWithThreadInfo);   

	bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),    
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);  

	return TRUE;
}
#endif // _DEBUG

void AutoDumpInit()
{
#ifndef _DEBUG
	SetUnhandledExceptionFilter(MyExceptionCrashFunc);
#endif // _DEBUG
}
