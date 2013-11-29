/********************************************************************
	created:	2013/11/25
	created:	$2013:11:25
	filename: 	MiniDump.h
	file base:	MiniDump
	file ext:	h
	author:		
	
	purpose:	for create mini dump file when app crashed
*********************************************************************/
#if !defined (_DUBINTERFACE_MINIDUMP_H)
#define  _DUBINTERFACE_MINIDUMP_H

#include <stdio.h>
#include <time.h>
#include <windows.h>

class CMiniDump
{
public:
	CMiniDump();
	~CMiniDump();
	void SetDumpPath(const TCHAR* fullpathname);
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);
	
};

#endif //!defined (_DUBINTERFACE_MINIDUMP_H)