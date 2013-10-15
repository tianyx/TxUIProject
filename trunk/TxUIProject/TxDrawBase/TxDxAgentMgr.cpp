#include "StdAfx.h"
#include "TxDxAgentMgr.h"

CTxDxAgentMgr* CTxDxAgentMgr::m_pAgentMgr = NULL;
CMutex CTxDxAgentMgr::m_mutex(FALSE, TEXT("{5F511751-E82D-43b6-9EEF-87955741FB00}"));
CTxDxAgentMgr::CTxDxAgentMgr(void)
{
	m_pLastHitted = NULL;
}

CTxDxAgentMgr::~CTxDxAgentMgr(void)
{
}

CTxDxAgent* CTxDxAgentMgr::FindAgent( HWND hwnd )
{
	CTxDxAgent* pRet = NULL;
	if (m_pLastHitted && m_pLastHitted->GetHwnd() == hwnd)
	{
		pRet = m_pLastHitted;
	}
	else
	{
		m_mutex.Lock(1000);
		MAPAGENT::iterator itf = m_mapAgents.find(hwnd);
		if (itf != m_mapAgents.end())
		{
			m_pLastHitted = itf->second;
			pRet = m_pLastHitted;
		}
		m_mutex.Unlock();

	}

	return pRet;
}

BOOL CTxDxAgentMgr::RegisterAgent( CTxDxAgent& agentIn )
{
	m_mutex.Lock(1000);
	m_mutex.Lock(1000);
	MAPAGENT::iterator itf = m_mapAgents.find(agentIn.GetHwnd());
	if (itf != m_mapAgents.end())
	{
		ASSERT(FALSE);
	}
	else
	{
		m_mapAgents[agentIn.GetHwnd()] = &agentIn;
	}
	m_mutex.Unlock();
	return TRUE;
}

CTxDxAgentMgr* CTxDxAgentMgr::GetTxDxAgentMgr()
{
	if (m_pAgentMgr == NULL)
	{
		m_pAgentMgr = new CTxDxAgentMgr;
	}

	return m_pAgentMgr;
}

BOOL CTxDxAgentMgr::UnRegisterAgent( CTxDxAgent& agentIn )
{
	m_mutex.Lock(1000);
	if (m_pLastHitted = &agentIn)
	{
		m_pLastHitted = NULL;
	}

	m_mapAgents.erase(agentIn.GetHwnd());
	m_mutex.Unlock();
	return TRUE;
}

void CTxDxAgentMgr::Release()
{
	if (m_pAgentMgr)
	{
		delete m_pAgentMgr;
		m_pAgentMgr = NULL;
	}

}
