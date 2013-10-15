// Log.cpp: implementation of the Log class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Log.h"
#include <TCHAR.H>
#include "io.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int Log::ToDebug   =  1;
const int Log::ToFile    =  2;
const int Log::ToConsole =  4;

const static int LINE_BUFFER_SIZE = 1024;

Log::Log(int mode, int level, LPCTSTR filename, bool append)
{
	m_lastLogTime = 0;
    hlogfile = INVALID_HANDLE_VALUE;
    m_todebug = false;
    m_toconsole = false;
    m_tofile = false;
	InitializeCriticalSection( &m_criLock );
	
	SetLevel( level );
    SetMode(mode);
	if( mode&ToFile && filename )
	  SetFile(filename, append);
}

void Log::SetMode(int mode) {
    
    if (mode & ToDebug)
        m_todebug = true;
    else
        m_todebug = false;

    if (mode & ToFile)  {
		m_tofile = true;
	} else {
		CloseFile();
        m_tofile = false;
    }
    
#ifdef _WIN32_WCE
	m_toconsole = false;
#else
    if (mode & ToConsole) {
        if (!m_toconsole)
            AllocConsole();		
        m_toconsole = true;
    } else {
        m_toconsole = false;
    }
#endif
}

void Log::SetLevel(int level) {
    m_level = level;
}

int Log::GetLevel() {
	return m_level;
}

void Log::SetFile(LPCTSTR filename, bool append) 
{
	if( !filename )
		return;

    CloseFile();
	
    m_tofile  = true;
    
    // If filename is NULL or invalid we should throw an exception here
	bool bOverSized = FALSE;
	if (access(filename, 04) != -1)
	{
		//file exist, check size;
		HANDLE tmpfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (tmpfile)
		{
			DWORD lfSize = GetFileSize(tmpfile, NULL);
			if (lfSize > 1024*1024)
			{
				bOverSized = true;
			}
			CloseHandle(tmpfile);
		}

	}
    if (!append || bOverSized)
	{
		// Build the backup filename
		TCHAR backupfilename[MAX_PATH];
		lstrcpy(backupfilename, filename );
		lstrcat(backupfilename, _TEXT(".bak"));
 		// Attempt the move and replace any existing backup
		// Note that failure is silent - where would we log a message to? ;)
 		DeleteFile(backupfilename);
 		MoveFile(filename, backupfilename);
	}

    hlogfile = CreateFile(
        filename,  GENERIC_WRITE, FILE_SHARE_READ, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL  );
    
    if (hlogfile == INVALID_HANDLE_VALUE) {
        // We should throw an exception here
        m_todebug = true;
        m_tofile = false;
        Print(0, _T("Error opening log file %s\n"), filename);
		return;
    }
    if (append) {
        SetFilePointer( hlogfile, 0, NULL, FILE_END );
    } else {
        SetEndOfFile( hlogfile );
    }

	if (hlogfile)
	{
		//print log header
		this->ReallyPrintLine("\r\n-------------------------------------------------------------------------------------------------------\r\n");
		this->ReallyPrintLine("--------------------------------!!!<<new log started>>!!!----------------------------------------------\r\n");
		this->ReallyPrintLine("-------------------------------------------------------------------------------------------------------\r\n");


	}
}

// if a log file is open, close it now.
void Log::CloseFile() {
    if (hlogfile != INVALID_HANDLE_VALUE) {
        CloseHandle(hlogfile);
        hlogfile = INVALID_HANDLE_VALUE;
    }
}

inline void Log::ReallyPrintLine(LPCTSTR line) 
{
	EnterCriticalSection( &m_criLock );
	
    if (m_todebug) OutputDebugString(line);
    if (m_toconsole) {
        DWORD byteswritten;
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), (VOID*)line, lstrlen(line)*sizeof(TCHAR), &byteswritten, NULL); 
    };

    if (m_tofile && (hlogfile != INVALID_HANDLE_VALUE)) {
        DWORD byteswritten;
        WriteFile(hlogfile, (VOID*)line, lstrlen(line)*sizeof(TCHAR), &byteswritten, NULL); 
    }

	LeaveCriticalSection( &m_criLock );
}

void Log::ReallyPrint(LPCTSTR format, va_list ap) 
{

	SYSTEMTIME current;
	GetLocalTime(&current);
	if (memcmp(&m_lastLogT, &current, sizeof(SYSTEMTIME)) != 0)
	{
		m_lastLogT = current;
		char time_str[50] = {0};
		char date_str[50] = {0};

		int nRet = GetDateFormat(LOCALE_USER_DEFAULT, NULL, &current, "ddd yyyy-MM-dd",  date_str, sizeof(date_str));
		nRet = GetTimeFormat(LOCALE_USER_DEFAULT,NULL, &current,"hh:mm:ss",time_str,sizeof(time_str));
		
		char time_buf[50];
		sprintf(time_buf, "%s %s\r\n",date_str, time_str);		
		ReallyPrintLine(time_buf);
	}


	// Prepare the complete log message
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
	_vsnprintf(line, sizeof(line) - 2 * sizeof(TCHAR), format, ap);
	line[LINE_BUFFER_SIZE-2] = (TCHAR)'\0';
#if (!defined(_UNICODE) && !defined(_MBCS))
	int len = strlen(line);
	if (len > 0 && len <= sizeof(line) - 2 * sizeof(TCHAR) && line[len-1] == (TCHAR)'\n') {
		// Replace trailing '\n' with MS-DOS style end-of-line.
		line[len-1] = (TCHAR)'\r';
		line[len] =   (TCHAR)'\n';
		line[len+1] = (TCHAR)'\0';
	}
#endif
	
	ReallyPrintLine(line);
	ReallyPrintLine(TEXT("\n"));
}

Log::~Log()
{
    CloseFile();
	DeleteCriticalSection( &m_criLock );	
}
