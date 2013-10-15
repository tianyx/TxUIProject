
#include "..\stdafx.h"
#include "LogTool.h"
#include <atlconv.h>
#include <locale.h>


namespace marrco
{
	CLogTool::CLogTool(  )
	{
		
	}
	
	CLogTool::~CLogTool()
	{

	}

	HRESULT	CLogTool::StartLog( CString strFileName , int nMaxSize )
	{
		m_nMaxSize = nMaxSize;
		m_bWorking = TRUE;
		SetLogFileName(strFileName);
		AfxBeginThread( CLogTool::LogThread, this );
		return S_OK;

	}
	HRESULT	CLogTool::Log( CString strLogType, CString strLog )
	{
		{
			CSingleLock lock( &m_lock, TRUE );
			CTime tm = CTime::GetCurrentTime();
			CString strTm = tm.Format(_T("%Y-%m-%d %H:%M:%S> ") );
			m_strLogArray.Add( strLogType + _T("`") + strTm + strLog );
		}
		//
		return S_OK;		
	}
		
	HRESULT	CLogTool::SetLogFileName( CString strFileName )
	{
		CSingleLock lock( &m_lock, TRUE );
		if ( strFileName != m_strLogFileName )
		{
			m_strLogFileName = strFileName;
		}

		return S_OK;
		
	}

	HRESULT	CLogTool::StopLog( )
	{
		{
			CSingleLock lock(&m_lock, TRUE );
			m_bWorking = FALSE;
		}

		WaitForSingleObject( m_QuitEvent, 5000 );
		return S_OK;
	}

	UINT CLogTool::LogThread(LPVOID lpVoid)
	{
		USES_CONVERSION;
		setlocale( LC_CTYPE, ("chs"));

		CLogTool *pLogTool = (CLogTool*)lpVoid;
		CTime tmLastWrite = CTime::GetCurrentTime();

		while ( TRUE )
		{
			CTime tmCur = CTime::GetCurrentTime();
			CTimeSpan tmSpan = tmCur - tmLastWrite;
			

			CStringArray strTempArray;
			//
			{
				CSingleLock lock( &pLogTool->m_lock, TRUE );
				if (  !pLogTool->m_bWorking || pLogTool->m_strLogArray.GetSize() >= pLogTool->m_nMaxSize || ( pLogTool->m_strLogArray.GetSize() > 0 && tmSpan.GetTotalSeconds() > 60)  )
				{
					strTempArray.Append( pLogTool->m_strLogArray );
					pLogTool->m_strLogArray.RemoveAll();
				}


			}
			//
			if ( strTempArray.GetSize() > 0 )
			{
				try
				{
					if ( !pLogTool->m_logFile.Open( pLogTool->m_strLogFileName, CFile::modeWrite) )
					{
						pLogTool->m_logFile.Open( pLogTool->m_strLogFileName, CFile::modeCreate | CFile::modeWrite );						
					}

					if ( pLogTool->m_logFile.m_hFile!= INVALID_HANDLE_VALUE )
					{
						pLogTool->m_logFile.SeekToEnd();
					}

					for ( int i = 0; i < strTempArray.GetSize(); i++)
					{
						if ( pLogTool->m_logFile.m_hFile != INVALID_HANDLE_VALUE)
						{
							CStringTool st;
							CString strLog ;
							CString strType;
							st.SplitString( _T("`"), strTempArray[i], strType, strLog );

							// CStdioFile 的 bug , unicode 下不支持中文
							// 需要设置 setlocale( LC_CTYPE, ("chs"));

							pLogTool->m_logFile.WriteString( strLog );
							pLogTool->m_logFile.WriteString( _T("\n") );
						}

					}
					pLogTool->m_logFile.Flush();
					pLogTool->m_logFile.Close();
					tmLastWrite = tmCur;
				}
				catch(...)
				{

				}
			}

			//
			if ( ! pLogTool->m_bWorking )
			{
			//	pLogTool->m_logFile.Close();
				break;
			}

			Sleep( 100 );

		};

		pLogTool->m_QuitEvent.SetEvent();

		return 0L;
	}

	

};