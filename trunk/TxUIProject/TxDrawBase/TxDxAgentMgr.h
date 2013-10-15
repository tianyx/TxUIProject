#pragma once

#include "TxDxAgent.h"
#include <map>
typedef std::map<HWND, CTxDxAgent*> MAPAGENT;


class CTxDxAgentMgr
{
private:
	CTxDxAgentMgr(void);
	~CTxDxAgentMgr(void);

private:
	MAPAGENT m_mapAgents;
	CTxDxAgent* m_pLastHitted;

public:
	CTxDxAgent* FindAgent(HWND hwnd);
public:
	BOOL RegisterAgent(CTxDxAgent& agentIn);
	BOOL UnRegisterAgent(CTxDxAgent& agentIn);

private:


private:
	static CTxDxAgentMgr* m_pAgentMgr;
	static CMutex m_mutex;
public:
	static CTxDxAgentMgr* GetTxDxAgentMgr();
	static void Release();
};
