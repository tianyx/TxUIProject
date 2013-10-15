#pragma once
#include <afxmt.h>

namespace marrco
{
	class CLogTool
	{
	public:		

	public:
		CLogTool( );
		~CLogTool();

	public:
		HRESULT		Log( CString strLogType, CString strLog );
		
		HRESULT		StartLog( CString strFileName, int nMaxSize = 100  );
		HRESULT		StopLog( );

	public:
		static UINT	LogThread(LPVOID lpVoid);

	protected:
		HRESULT		SetLogFileName( CString strFileName );

		CStringArray	m_strLogArray;
		CString			m_strLogFileName;	
		int				m_nMaxSize;

		CCriticalSection m_lock;

		CStdioFile		m_logFile;
		BOOL			m_bWorking;
		CEvent			m_QuitEvent;
	};

	

};