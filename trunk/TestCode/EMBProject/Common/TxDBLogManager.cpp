#include "StdAfx.h"
#include "TxDBLogManager.h"
#include "FGlobal.h"

#define MAXLOGQUEUESIZE 1000


CTxDBLogManager* CTxDBLogManager::m_spDBLogMgr = NULL;

CTxDBLogManager::CTxDBLogManager(void)
{
}

CTxDBLogManager::~CTxDBLogManager(void)
{
	EMB::CAutoLock lock(&m_fLock);
	MAPLOGDB::iterator itb = m_mapdbs.begin();
	MAPLOGDB::iterator ite = m_mapdbs.end();
	for (; itb != ite; itb++)
	{
		if (itb->second != 0)
		{
			CTxADODBMgr::GetADODBMgr()->CloseDB(itb->second);
		}
	}
	m_mapdbs.clear();
}

DWORD CTxDBLogManager::AddNewDBConn( DWORD dwLogKeyIn,DWORD dwDBKeyIn, CString& strDBConn )
{
	DWORD dwKey = 0;
	EMB::CAutoLock lock(&m_fLock);
	MAPLOGDB::iterator itf = m_mapdbs.find(dwLogKeyIn);
	if (itf != m_mapdbs.end())
	{
		dwKey = itf->first;
	}
	else
	{
		BOOL bOpened = CTxADODBMgr::GetADODBMgr()->OpenDB(dwDBKeyIn, strDBConn);
		if (bOpened)
		{
			dwKey = dwLogKeyIn;
			m_mapdbs[dwLogKeyIn] = dwDBKeyIn;
		}
	}

	return dwKey;

}


void CTxDBLogManager::CheckLog()
{
	VECLOGDATA vData;
	vData.reserve(MAXLOGQUEUESIZE);
	while (TxWaitObjWithQuit(m_hEventDataInQueue ,m_hQuitEvent, INFINITE) == WAIT_OBJECT_0)
	{

		//copy to cache
		m_qlock.Lock();	
		vData= m_vLogData;
		m_vLogData.clear();
		m_qlock.Unlock();
		ResetEvent(m_hEventDataInQueue);
		int nSize = vData.size();
		for (int i = 0; i < nSize; ++i)
		{
			//Write Out;
			m_fLock.Lock();
			MAPLOGDB::iterator itf = m_mapdbs.find(vData[i].dwLogKey);
			DWORD dwDBKey = 0;
			if (itf != m_mapdbs.end())
			{
				dwDBKey = itf->second;
			}
			else
			{
				//write to first;
				if (m_mapdbs.begin() != m_mapdbs.end())
				{
					dwDBKey = m_mapdbs.begin()->second;
				}
				else
				{
					ASSERT(FALSE);
				}
			}
			m_fLock.Unlock();
			if (dwDBKey)
			{
				CTxADOCommand cmd(dwDBKey);
				cmd.ExecuteSQL(vData[i].strData);
			}

		}
	}
}

CTxDBLogManager* CTxDBLogManager::GetTxDBLogMgr()
{
	if (m_spDBLogMgr == NULL)
	{
		m_spDBLogMgr = new CTxDBLogManager;
		m_spDBLogMgr->Start();
	}

	return m_spDBLogMgr;
}

void CTxDBLogManager::ReleaseTxDBLogMgr()
{
	if (m_spDBLogMgr)
	{
		delete m_spDBLogMgr;
		m_spDBLogMgr = NULL;
	}
}


