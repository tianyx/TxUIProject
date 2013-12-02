#include "StdAfx.h"
#include "MonitorGraph.h"
#include "FGlobal.h"
#include "GlobalDef.h"
#include "TxTimer.h"
#include "MBCTransMsg.h"
#include <algorithm>

void DelMBCObjProc(CMBCBaseObj* pObj)
{
	if (pObj)
	{
		delete pObj;
	}
}

CMonitorGraph::CMonitorGraph(void)
{
	m_bRunning = FALSE;
	m_nCoolDownInterval = 1000;
	m_nStatCheckInterval = g_GlobalInfo.nStateCheckInterval;
	m_nSwitchCD = g_GlobalInfo.nSwitchCD;
	m_nRelyCD = g_GlobalInfo.nRelayCD;
	m_gsHostObj.m_addrLocal = g_GlobalInfo.addrGStateHostListen;
	m_nRelay2to1Mode = g_GlobalInfo.nRelay2to1Mode;
	m_gsHostObj.SetGraphStateCallback(this);
	m_gsHostObj.Run();
}

CMonitorGraph::~CMonitorGraph(void)
{
	m_gsHostObj.Stop();
	ClearGraph();
}

HRESULT CMonitorGraph::LoadGraph()
{
	ClearGraph();
	HRESULT hr = S_OK;
	CAutoLock lock(&m_lockGraph);

	//load client
	for (size_t i = 0; i < g_GlobalInfo.vClientAddr.size(); ++i)
	{
		CMBCClientObj* pObj = new CMBCClientObj;
		pObj->m_addrRemote = g_GlobalInfo.vClientAddr[i].addr;
		pObj->m_addrLocal = g_GlobalInfo.vClientAddr[i].localBindAddr;
		pObj->m_strObjName = g_GlobalInfo.vClientAddr[i].szName.c_str();
		m_graphData.vClient.push_back(pObj);
		ST_OBJSTATEINFO info;
		info.nObjType = MBCOBJTYPE_CLIENT;
		info.pObj = (ULONG)pObj;
		m_graphState.vClientInfo.push_back(info);
	}
	for (size_t i = 0; i < g_GlobalInfo.vSvrAddr.size(); ++i)
	{
		
		CMBCSvrObj* pObj = new CMBCSvrObj;
		pObj->m_addrRemote = g_GlobalInfo.vSvrAddr[i].addr;
		pObj->m_addrLocal = g_GlobalInfo.vSvrAddr[i].localBindAddr;
		pObj->m_strObjName = g_GlobalInfo.vSvrAddr[i].szName.c_str();
		m_graphData.vSvr.push_back(pObj);
		ST_OBJSTATEINFO info;
		info.nObjType = MBCOBJTYPE_SVR;
		info.pObj = (ULONG)pObj;
		m_graphState.vSvrInfo.push_back(info);
	}


	for (size_t i = 0; i < g_GlobalInfo.vEndAddrMaster.size(); ++i)
	{
		CMBCEndObj* pObj = new CMBCEndObj(MBCOBJTYPE_ENDMASTER);
		pObj->m_addrRemote = g_GlobalInfo.vEndAddrMaster[i].addr;
		pObj->m_addrLocal = g_GlobalInfo.vEndAddrMaster[i].localBindAddr;
		pObj->m_strObjName = g_GlobalInfo.vEndAddrMaster[i].szName.c_str();
		pObj->m_strCHID = g_GlobalInfo.vEndAddrMaster[i].szChId.c_str();
		m_graphData.vEnd.push_back(pObj);
		ST_OBJSTATEINFO info;
		info.nObjType = MBCOBJTYPE_ENDMASTER;
		info.pObj = (ULONG)pObj;
		m_graphState.vEndInfo.push_back(info);
	}

	for (size_t i = 0; i < g_GlobalInfo.vEndAddrSlave.size(); ++i)
	{
		CMBCEndObj* pObj = new CMBCEndObj(MBCOBJTYPE_ENDSLAVE);
		pObj->m_addrRemote = g_GlobalInfo.vEndAddrSlave[i].addr;
		pObj->m_addrLocal = g_GlobalInfo.vEndAddrSlave[i].localBindAddr;
		pObj->m_strObjName = g_GlobalInfo.vEndAddrSlave[i].szName.c_str();
		pObj->m_strCHID = g_GlobalInfo.vEndAddrSlave[i].szChId.c_str();
		m_graphData.vEnd.push_back(pObj);
		ST_OBJSTATEINFO info;
		info.nObjType = MBCOBJTYPE_ENDSLAVE;
		info.pObj = (ULONG)pObj;
		m_graphState.vEndInfo.push_back(info);
	}

	for (size_t i = 0; i < g_GlobalInfo.vEndAddrBack.size(); ++i)
	{
		CMBCEndObj* pObj = new CMBCEndObj(MBCOBJTYPE_ENDBACK);
		pObj->m_addrRemote = g_GlobalInfo.vEndAddrBack[i].addr;
		pObj->m_addrLocal = g_GlobalInfo.vEndAddrBack[i].localBindAddr;
		pObj->m_strObjName = g_GlobalInfo.vEndAddrBack[i].szName.c_str();
		pObj->m_strCHID = g_GlobalInfo.vEndAddrBack[i].szChId.c_str();
		m_graphData.vEnd.push_back(pObj);
		ST_OBJSTATEINFO info;
		info.nObjType = MBCOBJTYPE_ENDBACK;
		info.pObj = (ULONG)pObj;
		m_graphState.vEndInfo.push_back(info);
	}

	return hr;
}

HRESULT CMonitorGraph::Run()
{
	if (m_bRunning)
	{
		Stop();
	}
	HRESULT hr = S_OK;
	for (size_t i = 0; i < m_graphData.vClient.size(); ++i)
	{
		hr = (m_graphData.vClient[i])->Run();
		if(hr != S_OK)
		{
			ASSERT(FALSE);
			break;
		}
	}

	if (hr == S_OK)
	{
		for (size_t i = 0; i < m_graphData.vSvr.size(); ++i)
		{
			hr = (m_graphData.vSvr[i])->Run();
			if(hr != S_OK)
			{
				ASSERT(FALSE);
				break;
			}
		}

	}

	if (hr == S_OK)
	{
		for (size_t i = 0; i< m_graphData.vEnd.size(); ++i)
		{
			hr = (m_graphData.vEnd[i])->Run();
			if(hr != S_OK)
			{
				ASSERT(FALSE);
				break;
			}
		}
	}
	
	if (hr != S_OK)
	{
		Stop();
	}
	else
	{
		//set timer
		m_timerActSendCoolDown.SetTimer(IDTIMER_GRAPHCHECK, m_nCoolDownInterval,  this, (LPARAM)this, WT_EXECUTEDEFAULT, 1000);
		m_timerStateCheck.SetTimer(IDTIMER_COOLDOWN, m_nStatCheckInterval, this,  (LPARAM)this, WT_EXECUTEDEFAULT, 1000);
		m_bRunning = TRUE;
	}
	return hr;
}

HRESULT CMonitorGraph::Stop()
{
	m_timerActSendCoolDown.KillTimer();
	m_timerStateCheck.KillTimer();
	HRESULT hr = S_OK;
	for (size_t i = 0; i < m_graphData.vClient.size(); ++i)
	{
		hr = (m_graphData.vClient[i])->Stop();
		ASSERT(hr == S_OK);
	}

	for (size_t i = 0; i < m_graphData.vSvr.size(); ++i)
	{
		hr = (m_graphData.vSvr[i])->Stop();
		ASSERT(hr == S_OK);

	}

	for (size_t i = 0; i< m_graphData.vEnd.size(); ++i)
	{
		hr = (m_graphData.vEnd[i])->Stop();
		ASSERT(hr == S_OK);

	}

	ClearGraph();
	m_bRunning = FALSE;

	return S_OK;
}

HRESULT CMonitorGraph::TxTimerCallbackProc( DWORD dwEvent, LPARAM lparam )
{
	if (dwEvent == IDTIMER_COOLDOWN)
	{
		CheckCDMsgs();
	}
	else if (dwEvent == IDTIMER_GRAPHCHECK)
	{
		//check graph state
		QueryGraphState();
		CheckGraphState();
	}
	return S_OK;
}

HRESULT CMonitorGraph::QueryGraphState()
{
	CAutoLock lock(&m_lockGraph);

	m_graphState.nGraphState = m_bRunning? graphState_running:graphState_stop;
	if (m_bRunning)
	{
		HRESULT hr = S_OK;
		for (size_t i = 0; i < m_graphState.vClientInfo.size(); ++i)
		{
			CMBCBaseObj* pObj = (CMBCBaseObj*)m_graphState.vClientInfo[i].pObj;
			ST_OBJSTATEINFO& objInfo= m_graphState.vClientInfo[i];
			hr = pObj->GetStateInfo(objInfo);
			ASSERT(hr == S_OK);
		}

		for (size_t i = 0; i < m_graphState.vSvrInfo.size(); ++i)
		{
			CMBCBaseObj* pObj = (CMBCBaseObj*)m_graphState.vSvrInfo[i].pObj;
			ST_OBJSTATEINFO& objInfo= m_graphState.vSvrInfo[i];
			hr = pObj->GetStateInfo(objInfo);

			ASSERT(hr == S_OK);

		}

		for (size_t i = 0; i< m_graphState.vEndInfo.size(); ++i)
		{
			CMBCBaseObj* pObj = (CMBCBaseObj*)m_graphState.vEndInfo[i].pObj;
			ST_OBJSTATEINFO& objInfo= m_graphState.vEndInfo[i];
			hr = pObj->GetStateInfo(objInfo);
			ASSERT(hr == S_OK);

		}

	}
	
	return S_OK;
}

HRESULT CMonitorGraph::CheckGraphState()
{
	CAutoLock lock(&m_lockGraph);
#ifdef _DEBUG
	//output all state
// 	for (size_t i = 0; i< m_graphState.vEndInfo.size(); ++i)
// 	{
// 		ST_OBJSTATEINFO& objInfo= m_graphState.vEndInfo[i];
// 		if (objInfo.nObjType == MBCOBJTYPE_ENDSLAVE
// 			&& objInfo.strChId.Compare("CHN02") == 0)
// 		{
// 			CFWriteLog(TEXT("endobj= %s, remoteip =%s, selfstate = %d"), objInfo.strName, Addr2String(objInfo.addrRemote).c_str(), objInfo.nSelfState);
// 		}
// 	}
// 
// 	for (size_t i = 0; i< m_graphState.vSvrInfo.size(); ++i)
// 	{
// 		ST_OBJSTATEINFO& objInfo= m_graphState.vSvrInfo[i];
// 		CFWriteLog(TEXT("svrobj= %s, remoteip =%s, selfstate = %d, remotestate = %d"), objInfo.strName,  Addr2String(objInfo.addrRemote), objInfo.nSelfState, objInfo.remoteInfo.nRemoteState);
// 
// 	}
// 
// 	for (size_t i = 0; i< m_graphState.vClientInfo.size(); ++i)
// 	{
// 		ST_OBJSTATEINFO& objInfo= m_graphState.vClientInfo[i];
// 		CFWriteLog(TEXT("svrobj= %s, remoteip =%s, selfstate = %d, remotestate = %d"), objInfo.strName,  Addr2String(objInfo.addrRemote), objInfo.nSelfState, objInfo.remoteInfo.nRemoteState);
// 
// 	}

#endif // _DEBUG
	if (!m_bRunning)
	{
		return S_OK;
	}
	//only check end
	HRESULT hr =S_OK;
	for (size_t i = 0; i< m_graphState.vEndInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vEndInfo[i];
		if (objInfo.nObjType == MBCOBJTYPE_ENDMASTER)
		{
			if (objInfo.nSelfState != MBCSTATE_OK)
			{
				//error
				CString& strCHId = objInfo.strChId;
				SOCKADDR_IN& addrDest = objInfo.addrRemote;
				if (objInfo.nSelfState ==MBCSTATE_STREAMDOWN)
				{
					CFWriteLog(SWITCHERLOGKEY, TEXT("notify:found stream down: chid = %s, addr=%s"), strCHId,  Addr2String(addrDest).c_str());
					//stream down
					//check cooldown list,  a channel only allow one live msg
					if (IsChannelActInCD(strCHId))
					{
						//do nothing
						CFWriteLog(SWITCHERLOGKEY, TEXT("warning:channel is in CD: chid = %s"), strCHId);

					}
					else
					{
						//send act change for switch or relay
						CMBCClientObj* pSlaveClentObj = NULL;
						CMBCSvrObj* pSlaveSvrObj = NULL;
						CMBCEndObj* pSlaveEndObj = NULL;
						SOCKADDR_IN addrSvrCtrl;
						if (FindFirstValidSlaveLinkPath(strCHId, pSlaveEndObj, pSlaveSvrObj, pSlaveClentObj, addrSvrCtrl))
						{
							//
							CFWriteLog(SWITCHERLOGKEY, TEXT("notify: slave link path found : chid = %s"), strCHId);
							//send switch msg
							ST_MBCACTCOOLDOWN cdmsg;
							cdmsg.nCoolDownCount = m_nSwitchCD;
							cdmsg.pObj = pSlaveClentObj;
							cdmsg.actMsg.nMsgId = actid_notsended;
							cdmsg.actMsg.nActType = actType_changeMBCDest;
							cdmsg.actMsg.addrDest = addrSvrCtrl;
							StrCpy(cdmsg.actMsg.strChId, strCHId.GetBuffer());
							cdmsg.strChId = strCHId;
							AddCDMsg(cdmsg);
							CheckCDMsgs();
						}
						else
						{
							CFWriteLog(SWITCHERLOGKEY, TEXT("warning:no available slave link path for using, start find back svr : chid = %s"), strCHId);
							CMBCEndObj* pBackEndObj = NULL;
							if (FindFirstValidBackEnd(strCHId, pBackEndObj))
							{
								CFWriteLog(SWITCHERLOGKEY, TEXT("found available back end obj, start relay from %s to %s"),  Addr2String(pBackEndObj->m_addrRemote).c_str(),  Addr2String(addrDest).c_str());
								//pBackEndObj->AddRelayAddr(addrDest);
								ST_MBCACTCOOLDOWN cdmsg;
								cdmsg.nCoolDownCount = m_nRelyCD;
								cdmsg.pObj = pBackEndObj;
								cdmsg.actMsg.nMsgId = actid_notsended;
								cdmsg.actMsg.nActType = actType_relayDest;
								cdmsg.actMsg.addrDest = addrDest;
								StrCpy(cdmsg.actMsg.strChId, strCHId.GetBuffer());
								cdmsg.strChId = strCHId;
								AddCDMsg(cdmsg);
							}
						}
					}
					
				}
				else
				{
					//monitor obj failed.
					CFWriteLog(SWITCHERLOGKEY, TEXT("!!!error:found master end obj error: chid = %s, addr=%s"), strCHId,  Addr2String(addrDest).c_str());
				}
			}
			else
			{
				//check src addr trace
				//if have more than 2 addr, notify relay end obj to stop relay
				CMBCEndObj* pMasterEndObj = (CMBCEndObj* )objInfo.pObj;
				CheckMasterRecvState(pMasterEndObj);
			}
		}
		ASSERT(hr == S_OK);
	}

	return hr;
}

BOOL CMonitorGraph::FindFirstValidSlaveLinkPath( const CString& strCHId, CMBCEndObj*& pEndObjOut, CMBCSvrObj*& pObjSvrOut, CMBCClientObj*& pObjClientOut, SOCKADDR_IN& addrSvrCtrlOut)
{
	BOOL bFind = FALSE;
	pObjClientOut = NULL;
	SOCKADDR_IN addrEndSlave;

	for (size_t i = 0; i< m_graphState.vEndInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vEndInfo[i];
		if (objInfo.nObjType == MBCOBJTYPE_ENDSLAVE
			&& objInfo.nSelfState == MBCSTATE_OK 
			&& (objInfo.strChId.Compare(strCHId) == 0))
		{
			addrEndSlave = objInfo.addrRemote;
			pEndObjOut = (CMBCEndObj*)objInfo.pObj;
			bFind = FindSvr(strCHId, addrEndSlave, pObjSvrOut, addrSvrCtrlOut);
			if (bFind)
			{
				bFind = FindClient(strCHId, addrSvrCtrlOut, pObjClientOut);
				if (bFind)
				{
					break;
				}
			}
		}
	}

	return bFind;
}

BOOL CMonitorGraph::IsChannelActInCD(CString& strCHId)
{
	CAutoLock lock(&m_lockCD);
	//check cooldown list
	MAPCOOLDOWN::iterator itf = m_lCooldownActs.find(strCHId);
	return(itf != m_lCooldownActs.end());
}

BOOL CMonitorGraph::FindSvr( const CString& strCHId, SOCKADDR_IN& AddrEnd, CMBCSvrObj*& pObjOut, SOCKADDR_IN& addrCtrlOut  )
{
	BOOL bFind = FALSE;
	//find slave svr
	for (size_t i = 0; i< m_graphState.vSvrInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vSvrInfo[i];
		if (objInfo.nSelfState == S_OK)
		{
			std::vector<ST_MBCCHANNELINFO>& vchInfo =objInfo.remoteInfo.vCHInfo;
			for (size_t k = 0; k < vchInfo.size(); k++)
			{
				if (vchInfo[k].szChannelId.Compare(strCHId) == 0
					&& IsEqualAddr(vchInfo[k].addrDest, AddrEnd))
				{
					bFind = TRUE;
					pObjOut = (CMBCSvrObj*)objInfo.pObj;
					addrCtrlOut = vchInfo[k].addrCtrl;
					break;
				}
			}
			if (bFind)
			{
				break;
			}

		}
	}

	return bFind;
}

BOOL CMonitorGraph::FindClient( const CString& strCHId, SOCKADDR_IN& AddrSvrctrl,CMBCClientObj*& pObjOut )
{
	BOOL bFind = FALSE;
	for (size_t i = 0; i< m_graphState.vClientInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vClientInfo[i];
		if (objInfo.nSelfState == MBCSTATE_OK
			&&objInfo.remoteInfo.nRemoteState == MBCSTATE_OK)
		{
			std::vector<ST_MBCCHANNELINFO>& vchInfo =objInfo.remoteInfo.vCHInfo;
			for (size_t k = 0; k < vchInfo.size(); k++)
			{
				if (vchInfo[k].szChannelId.Compare(strCHId) == 0
					&& vchInfo[k].addrCtrl == AddrSvrctrl)
				{
					bFind = TRUE;
					pObjOut = (CMBCClientObj*)objInfo.pObj;
					break;
				}
			}
			if (bFind)
			{
				break;
			}
		}
	}

	return bFind;
}

HRESULT CMonitorGraph::AddCDMsg( ST_MBCACTCOOLDOWN& msg )
{
	if (IsChannelActInCD(msg.strChId))
	{
		ASSERT(FALSE);
		return S_FALSE;
	}

	CAutoLock lock(&m_lockCD);
	m_lCooldownActs[msg.strChId] = msg;

	return S_OK;
}

HRESULT CMonitorGraph::CheckCDMsgs()
{
	CAutoLock lock(&m_lockCD);

	//check cooldown list
	MAPCOOLDOWN::iterator itb = m_lCooldownActs.begin();
	MAPCOOLDOWN::iterator ite = m_lCooldownActs.end();

	vector<CString> vDels;
	for (; itb!=ite; ++itb)
	{
		ST_MBCACTCOOLDOWN& cdmsg = itb->second;
		if (cdmsg.actMsg.nMsgId == actid_notsended)
		{
			static int snActIdBase = 0;
			snActIdBase++;
			if (cdmsg.actMsg.nActType == actType_changeMBCDest)
			{
				cdmsg.actMsg.nMsgId = snActIdBase;
				cdmsg.actMsg.nRequestSendTime = time(NULL);
				//send msg
				ASSERT(cdmsg.pObj->GetState() ==MBCSTATE_OK);
				char buffer[MAXRECVBUFF];
				int nUsed = 0;
				HRESULT hr = PackMBCMsg(cdmsg.actMsg, buffer, MAXRECVBUFF, nUsed);
				if (nUsed > 0)
				{
					CFWriteLog("\n\n\n\n\n---send act to switch chid = %s, size =%d, obj = %s, sock = %d\n\n\n\n\n", cdmsg.strChId, nUsed, cdmsg.pObj->m_strObjName, cdmsg.pObj->m_pSockBase->m_hSock);
					HRESULT hr = send(*(cdmsg.pObj->m_pSockBase), buffer, nUsed, 0);
					if(hr == SOCKET_ERROR)
					{
						ASSERT(FALSE);
						
					}
				}	
			}
			else if (cdmsg.actMsg.nActType == actType_relayDest)
			{
				for (size_t ir  = 0; ir < m_graphData.vEnd.size(); ++ir)
				{
					CMBCEndObj* pRelayObj = m_graphData.vEnd[ir];
					if (pRelayObj->m_nObjType == MBCOBJTYPE_ENDBACK)
					{
						pRelayObj->RemoveRelayAddr(cdmsg.actMsg.addrDest);
					}
				}
				cdmsg.actMsg.nMsgId = snActIdBase;
				cdmsg.actMsg.nRequestSendTime = time(NULL);
				//send msg
				ASSERT(cdmsg.pObj->GetState() ==MBCSTATE_OK
					&&cdmsg.pObj->m_nObjType == MBCOBJTYPE_ENDBACK);
				((CMBCEndObj*)cdmsg.pObj)->AddRelayAddr(cdmsg.actMsg.addrDest);

			}
			
		}

		(itb->second.nCoolDownCount)--;
		if (itb->second.nCoolDownCount <= 0)
		{
			//erase it
			vDels.push_back(itb->first);
		}
	}

	int nDelSize = vDels.size();
	if (nDelSize != 0)
	{
		for (int i = nDelSize -1; i >= 0; --i)
		{
			MAPCOOLDOWN::iterator itf = m_lCooldownActs.find(vDels[i]);
			if (itf != m_lCooldownActs.end())
			{
				m_lCooldownActs.erase(itf);
			}
		}
	}
	return TRUE;
}

HRESULT CMonitorGraph::OnActMsgReply( CMBCBaseObj* pMBCObj, ST_MBCMSG_ACTQA& msg )
{
	CAutoLock lock(&m_lockCD);
	//find msg
	MAPCOOLDOWN::iterator itf = m_lCooldownActs.find(msg.strChId);
	if(itf != m_lCooldownActs.end())
	{
		ASSERT(itf->second.actMsg.nMsgId == msg.nMsgId);
		itf->second.actMsg.nResult = msg.nResult;
		CFWriteLog(TEXT("act msg(chid = %s,  actid= %d) returned result = %d"), msg.strChId, msg.nMsgId, msg.nResult);
	}

	return S_OK;
}

BOOL CMonitorGraph::FindFirstValidBackEnd( const CString& strCHId, CMBCEndObj*& pObjEndBackOut )
{
	BOOL bFind = FALSE;
	pObjEndBackOut = NULL;

	for (size_t i = 0; i< m_graphState.vEndInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vEndInfo[i];
		if (objInfo.nObjType == MBCOBJTYPE_ENDBACK
			&& objInfo.nSelfState == MBCSTATE_OK /*&& (objInfo.remoteInfo.vCHInfo[0].szChannelId.Compare(strCHId) == 0)*/)
		{
			pObjEndBackOut = (CMBCEndObj*)objInfo.pObj;
			bFind = TRUE;
			break;
		}
	}

	return bFind;
}

BOOL CMonitorGraph::StopRelyBroadCast( SOCKADDR_IN& AddrRely )
{
	for (size_t i = 0; i< m_graphState.vEndInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vEndInfo[i];
		if (objInfo.nObjType == MBCOBJTYPE_ENDBACK)
		{
			CMBCEndObj* pObj = (CMBCEndObj*)objInfo.pObj;
			pObj->RemoveRelayAddr(AddrRely);
		}
	}

	return TRUE;

}

HRESULT CMonitorGraph::CheckMasterRecvState(CMBCEndObj* pMasterEndObj)
{
	if (IsChannelActInCD(pMasterEndObj->m_strCHID))
	{
		return S_OK;
	}
	std::vector<SOCKADDR_IN> vSrcAddrs;
	pMasterEndObj->GetRecvAddrs(vSrcAddrs);
	if (vSrcAddrs.size() >1)
	{
		//notify other relay end obj to stop if the addr is in it's relay addrs.
		std::vector<CMBCEndObj*> vSelfRelayObj;
		std::vector<SOCKADDR_IN> vSelfRelayAddrs;

		for (size_t ir  = 0; ir < m_graphData.vEnd.size(); ++ir)
		{
			CMBCEndObj* pRelayObj = m_graphData.vEnd[ir];
			if (pRelayObj->m_nObjType == MBCOBJTYPE_ENDBACK)
			{
				int nRet = 0;
				int nPos = 0;
				for (size_t ik = 0; ik < vSrcAddrs.size(); ++ik)
				{
					nRet =pRelayObj->IsLocalAddrOfRelaySock(pMasterEndObj->m_addrRemote, vSrcAddrs[ik]);
					if (nRet ==2)
					{
						break;
					}
				}

				if (nRet >= 1)
				{
					//means i relay to the addr
					vSelfRelayObj.push_back(pRelayObj);
					if (nRet == 2)
					{
						//means the src addr is mine
						vSelfRelayAddrs.push_back(vSrcAddrs[nPos]);
					}
				}
				
			}
		}

		if (vSelfRelayObj.size() == 0)
		{
			//client obj switch err, find lastest err client and change it
			if (g_GlobalInfo.nAutoJudgeMasterClient == 1)
			{
				AutoJusticeMasterClient(pMasterEndObj->m_strCHID, pMasterEndObj->m_addrRemote);
			}
		}
		else if (vSelfRelayObj.size() < vSrcAddrs.size())
		{
			//master svr may recovered. stop local back end srv relay.
			for (size_t i = 0 ; i < vSelfRelayObj.size(); ++i)
			{
				vSelfRelayObj[i]->RemoveRelayAddr(pMasterEndObj->m_addrRemote);
			}
		}
	}

	return S_OK;
}

HRESULT CMonitorGraph::GetGraphState( ST_GRAPHSTATE& outData )
{
	CAutoLock lock(&m_lockGraph);
	outData = m_graphState;
	return S_OK;
}

HRESULT CMonitorGraph::ClearGraph()
{
	CAutoLock lock(&m_lockGraph);
	//clean all
	for_each(m_graphData.vClient.begin(), m_graphData.vClient.end(), DelMBCObjProc);
	for_each(m_graphData.vEnd.begin(), m_graphData.vEnd.end(), DelMBCObjProc);
	for_each(m_graphData.vSvr.begin(), m_graphData.vSvr.end(), DelMBCObjProc);

	m_graphData.vClient.clear();
	m_graphData.vEnd.clear();
	m_graphData.vSvr.clear();
	m_graphState.nGraphState = graphState_stop;
	m_graphState.vClientInfo.clear();
	m_graphState.vEndInfo.clear();
	m_graphState.vSvrInfo.clear();
	m_graphState.nUniqueId = GenRand();

	return S_OK;
}

HRESULT CMonitorGraph::Test()
{
	for (size_t i = 0; i< m_graphState.vClientInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vClientInfo[i];
		if (objInfo.nSelfState == MBCSTATE_OK
			&&objInfo.remoteInfo.nRemoteState == MBCSTATE_OK)
		{
			std::vector<ST_MBCCHANNELINFO>& vchInfo =objInfo.remoteInfo.vCHInfo;
			for (size_t k = 0; k < vchInfo.size(); k++)
			{
				ST_MBCACTCOOLDOWN cdmsg;
				cdmsg.nCoolDownCount = m_nSwitchCD;
				cdmsg.pObj = (CMBCBaseObj* )objInfo.pObj;
				cdmsg.actMsg.nMsgId = actid_notsended;
				cdmsg.actMsg.nActType = actType_changeMBCDest;
				cdmsg.actMsg.addrDest = vchInfo[k].addrCtrl;
				StrCpy(cdmsg.actMsg.strChId, vchInfo[k].szChannelId);
				cdmsg.strChId = vchInfo[k].szChannelId;

				char buffer[MAXRECVBUFF];
				int nUsed = 0;
				HRESULT hr = PackMBCMsg(cdmsg.actMsg, buffer, MAXRECVBUFF, nUsed);
				if (nUsed > 0)
				{
					CFWriteLog("---send act to switch chid = %s, obj = %s", cdmsg.strChId, cdmsg.pObj->m_strObjName);
					send(*(cdmsg.pObj->m_pSockBase), buffer, nUsed, 0);

				}	
			}
			
		}
	}
	
	return TRUE;
}

void CMonitorGraph::AutoJusticeMasterClient( CString& strCHId, SOCKADDR_IN& addrEndIn )
{
	BOOL bFind = FALSE;
	CMBCClientObj* pObj1 = NULL;
	CMBCClientObj* pObj2 = NULL;
	//find slave svr
	for (size_t i = 0; i< m_graphState.vSvrInfo.size(); ++i)
	{
		ST_OBJSTATEINFO& objInfo= m_graphState.vSvrInfo[i];
		if (objInfo.nSelfState == S_OK)
		{
			std::vector<ST_MBCCHANNELINFO>& vchInfo =objInfo.remoteInfo.vCHInfo;
			for (size_t k = 0; k < vchInfo.size(); k++)
			{
				if (vchInfo[k].szChannelId.Compare(strCHId) == 0
					&& IsEqualAddr(vchInfo[k].addrDest, addrEndIn))
				{
					
					CMBCSvrObj* pObjOut = (CMBCSvrObj*)objInfo.pObj;
					SOCKADDR_IN addrCtrlOut = vchInfo[k].addrCtrl;
					CMBCClientObj* pClientObjOut = NULL;
					FindClient(strCHId, addrCtrlOut, pClientObjOut);
					if (pClientObjOut != NULL)
					{
						pObj1 == NULL? pObj1 = pClientObjOut: pObj2 = pClientObjOut;
						if (pObj1 && pObj2)
						{
							break;
						}
					}
					
				}
			}
			

		}
	}

	//
	CMBCClientObj* pFinalObj = NULL;
	if (pObj1 && pObj2
		&& (pObj1->m_nLastStateErrTime > 0 || pObj2->m_nLastStateErrTime > 0))
	{
		//compare last
		if (pObj1->m_nLastStateErrTime == 0)
		{
			pFinalObj = pObj2;
		}
		else if (pObj2->m_nLastStateErrTime == 0)
		{
			pFinalObj = pObj1;
		}
		else
		{
			CMBCClientObj* pFinalObj = pObj1->m_nLastStateErrTime > pObj2->m_nLastStateErrTime? pObj2:pObj1;
		}

	}

	if (pFinalObj)
	{
		//call switch

		ST_MBCACTCOOLDOWN cdmsg;
		cdmsg.nCoolDownCount = 1;
		cdmsg.pObj = pFinalObj;
		cdmsg.actMsg.nMsgId = actid_notsended;
		cdmsg.actMsg.nActType = actType_changeMBCDest;
		cdmsg.actMsg.addrDest = pFinalObj->m_addrRemote;
		StrCpy(cdmsg.actMsg.strChId, strCHId.GetBuffer());
		cdmsg.strChId = strCHId;
		AddCDMsg(cdmsg);
		CheckCDMsgs();
	}


}

HRESULT CMonitorGraph::ChangeRelay( ST_CHANGERELAY& relayIn )
{
	CAutoLock lock(&m_lockGraph);
	//check relay mode 
	if (m_nRelay2to1Mode == 0)
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("not in relay 2 to 1 mode!"));
		return S_FALSE;
	}

	CFWriteLog(TEXT("2to1OP: chid = %s, relay addr to %s"),relayIn.strChId,  Addr2String(relayIn.addrRelay).c_str());
	HRESULT hr = S_FALSE;
	//find master obj
	CMBCEndObj* pMasterEndObj = NULL;
	CMBCEndObj* pBackObj = NULL;
	vector<CMBCEndObj*> vBackObjs;
	for(size_t i = 0; i < m_graphData.vEnd.size(); ++i)
	{
		CMBCEndObj* pObj = m_graphData.vEnd[i]; 
		if (pObj->m_nObjType == MBCOBJTYPE_ENDMASTER
			 && pObj->m_strCHID.Compare(relayIn.strChId) == 0)
		{
			pMasterEndObj = m_graphData.vEnd[i];
		}

		if (pObj->m_nObjType == MBCOBJTYPE_ENDBACK)
		{
			if (pObj->GetState() == MBCSTATE_OK && pObj->m_addrRemote == relayIn.addrRelay)
			{
				pBackObj = pObj;
			}
		}
	}
	
	if (pMasterEndObj)
	{
		std::vector<SOCKADDR_IN> vSrcAddrs;
		pMasterEndObj->GetRecvAddrs(vSrcAddrs);
		if (vSrcAddrs.size() > 0)
		{
			//stop other relay
			ASSERT(vSrcAddrs.size() == 1);
			if (vSrcAddrs[0] == relayIn.addrRelay)
			{
				//not need to change
				hr = S_OK;
			}
			else
			{
				//stop old relay
								
			}
		}
	}
	else
	{
		hr = E_FAIL;
		return hr;
	}

	//start new relay 
	if (pBackObj)
	{
		//pBackObj->AddRelayAddr(pMasterEndObj->m_addrRemote);
		ST_MBCACTCOOLDOWN cdmsg;
		cdmsg.nCoolDownCount = m_nRelyCD;
		cdmsg.pObj = pBackObj;
		cdmsg.actMsg.nMsgId = actid_notsended;
		cdmsg.actMsg.nActType = actType_relayDest;
		cdmsg.actMsg.addrDest = pMasterEndObj->m_addrRemote;
		StrCpy(cdmsg.actMsg.strChId, relayIn.strChId);
		cdmsg.strChId = relayIn.strChId;
		AddCDMsg(cdmsg);
		CheckCDMsgs();
		hr = S_OK;
	}
	

	return hr;
}
