#include "StdAfx.h"
#include "TxLogManager.h"
#include "FGlobal.h"

CTxLogManager* g_pLogMgr = NULL;

#define MAXLOGQUEUESIZE 1000
CTxLogManager::CTxLogManager(void)
{
	m_hLoopThread = NULL;
	m_hEventDataInQueue = CreateEvent(NULL, TRUE, FALSE,  NULL);
	m_bRunning = FALSE;
	m_vLogData.reserve(MAXLOGQUEUESIZE);
}

CTxLogManager::~CTxLogManager(void)
{
	Stop();
	//clear logs
	m_fLock.Lock();
	MAPFILELOGS::iterator itb = m_mapFileLogs.begin();
	MAPFILELOGS::iterator ite = m_mapFileLogs.end();
	for (; itb != ite; itb++)
	{
		if (itb->second.log != NULL)
		{
			delete itb->second.log;
		}
	}
	m_mapFileLogs.clear();
	m_fLock.Unlock();

}

void CTxLogManager::Start()
{
	m_bRunning = TRUE;
	m_hLoopThread = CreateThread(NULL, NULL, LogLoopProc, (LPVOID)this, 0,0);
}

void CTxLogManager::CheckLog()
{
	VECLOGDATA vData;
	vData.reserve(MAXLOGQUEUESIZE);
	while (m_bRunning)
	{
		if(WaitForSingleObject(m_hEventDataInQueue, 500)== WAIT_OBJECT_0)
		{
			//copy to cache
			m_qlock.Lock();	
			vData= m_vLogData;
			m_qlock.Unlock();
			ResetEvent(m_hEventDataInQueue);
			int nSize = vData.size();
			for (int i = 0; i < nSize; ++i)
			{
				//Write Out;
				m_fLock.Lock();
				MAPFILELOGS::iterator itf = m_mapFileLogs.find(vData[i].dwLogKey);
				Log* pLogfile = NULL;
				if (itf != m_mapFileLogs.end())
				{
					pLogfile = itf->second.log;
				}
				else
				{
					//write to first;
					ASSERT(FALSE);
					if (m_mapFileLogs.begin() != m_mapFileLogs.end())
					{
						pLogfile = m_mapFileLogs.begin()->second.log;
					}
					else
					{
						ASSERT(FALSE);
					}
				}
				m_fLock.Unlock();
				if (pLogfile)
				{
					pLogfile->Print(TEXT("%s") , vData[i].strData);
				}

			}

		}
	}
}

void CTxLogManager::Stop()
{
	if (m_hLoopThread)
	{
		m_bRunning = FALSE;
		WaitForSingleObject(m_hEventDataInQueue, 1000);
		TerminateThread(m_hEventDataInQueue, 0);
		m_hEventDataInQueue = NULL;
	}

}

DWORD CTxLogManager::WriteLog( DWORD dwLogKey, CString strDataIn )
{
	ST_LOGDATA data;
	data.dwLogKey = dwLogKey;
	data.strData = strDataIn;
	while (m_vLogData.size() >= MAXLOGQUEUESIZE)
	{
		ASSERT(FALSE);
		Sleep(10);
	}
	m_qlock.Lock();
	m_vLogData.push_back(data);
	m_qlock.Unlock();
	SetEvent(m_hEventDataInQueue);

	return 0;
}

DWORD CTxLogManager::AddNewLogFile( DWORD dwLogKey, CString strFileIn , BOOL bAddDateToName /*= TRUE*/)
{
	
	DWORD dwKey = 0;
	m_fLock.Lock();
	MAPFILELOGS::iterator itf = m_mapFileLogs.find(dwLogKey);
	if (itf != m_mapFileLogs.end())
	{
		dwLogKey = itf->first;
	}
	else
	{
		CString strFile = strFileIn;
		if (strFileIn.Find(':') == -1)
		{
			//it's a key create file name use this key
			strFile = GetAppPath().c_str();
			strFile += TEXT("\\log");
			CreateDirectory(strFile, NULL);
			strFile +=TEXT("\\");
			strFile += strFileIn;
			strFileIn +=TEXT(".log");
		}
		ST_LOGFILEDATA data;
		data.dwLogKey = dwLogKey;
		data.log = new Log;
		data.log->m_bAutoAddTimeToFileName = bAddDateToName;
		data.log->SetMode(2);

		data.log->SetFile(strFile, true);
		data.strName = strFile;
		m_mapFileLogs[dwLogKey] = data;
	}

	m_fLock.Unlock();
	return dwKey;

}

CTxLogManager* GetTxLogMgr()
{
	if (g_pLogMgr == NULL)
	{
		g_pLogMgr = new CTxLogManager();
	}

	return g_pLogMgr;

}

void ReleaseTxLogMgr()
{
	if (g_pLogMgr)
	{
		delete g_pLogMgr;
		g_pLogMgr = NULL;
	}
}

DWORD __stdcall LogLoopProc( LPVOID lparam )
{
	CTxLogManager* pLogMgr = (CTxLogManager*)lparam;
	ASSERT(pLogMgr);
	pLogMgr->CheckLog();

	return 0;
}
