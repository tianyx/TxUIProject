#pragma once
#include <vector>
/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MonitorGraph.h
	file base:	MonitorGraph
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#include <map>
#include <list>
#include "MBCClientObj.h"
#include "MBCSvrObj.h"
#include "MBCEndObj.h"
#include "FGlobal.h"
#include "MBCGraphStateSvrObj.h"
using namespace std;
struct ST_MBCACTPACK
{
	CMBCBaseObj* pDestObj;
	ST_MBCMSG_ACTQA msg;
	ST_MBCACTPACK()
	{
		pDestObj = NULL;
	}

	bool operator ==(ST_MBCACTPACK& other)
	{
		return other.pDestObj == pDestObj && IsEqualAddr(msg.addrDest, other.msg.addrDest)
			&& msg.nMsgId == other.msg.nMsgId;
	}

};

struct ST_MBCACTCOOLDOWN
{
	int nCoolDownCount;
	CString strChId;
	CMBCBaseObj* pObj;
	ST_MBCMSG_ACTQA actMsg;

	bool operator == (const ST_MBCACTCOOLDOWN& other)
	{
		return strChId.Compare(other.strChId) == 0 && pObj == other.pObj 
			&& actMsg.nMsgId == other.actMsg.nMsgId && actMsg.addrDest == other.actMsg.addrDest;
	}

};

typedef map<CString, ST_MBCACTCOOLDOWN> MAPCOOLDOWN;


struct ST_GRAPHDATA
{
	vector<CMBCClientObj*> vClient;
	vector<CMBCSvrObj*> vSvr;
	vector<CMBCEndObj*> vEnd;
};





class CMonitorGraph:public ITxTimerCallbackInterface, public IMBCMsgPostProcessInterface, public IGraphStateCallbackInterface
{
public:
	CMonitorGraph(void);
	~CMonitorGraph(void);

	HRESULT LoadGraph();
	HRESULT ClearGraph();
	HRESULT Run();
	HRESULT Stop();

	virtual HRESULT GetGraphState(ST_GRAPHSTATE& outData);

private:
	BOOL IsChannelActInCD(CString& strCHId);
	HRESULT QueryGraphState();
	HRESULT CheckGraphState();
	HRESULT CheckMasterRecvState(CMBCEndObj* pMasterEndObj);

	//find slave client through link  slave end----> slave svr ---> slave client
	BOOL FindFirstValidSlaveLinkPath(const CString& strCHId, CMBCEndObj*& pEndObjOut, CMBCSvrObj*& pObjSvrOut, CMBCClientObj*& pObjClientOut, SOCKADDR_IN& addrSvrCtrlOut);
	BOOL FindSvr(const CString& strCHId, SOCKADDR_IN& AddrEnd, CMBCSvrObj*& pObjOut, SOCKADDR_IN& addrCtrlOut );
	BOOL FindClient(const CString& strCHId, SOCKADDR_IN& AddrCtrl, CMBCClientObj*& pObjOut);

	BOOL FindFirstValidBackEnd(const CString& strCHId, CMBCEndObj*& pObjOut);
	BOOL StopRelyBroadCast(SOCKADDR_IN& AddrRely);
	BOOL SendActRequest(CMBCBaseObj* pObj);

	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	virtual HRESULT OnActMsgReply(CMBCBaseObj* pMBCObj,  ST_MBCMSG_ACTQA& msg);

	void AutoJusticeMasterClient(CString& strChId, SOCKADDR_IN& addrEnd);

	HRESULT AddCDMsg(ST_MBCACTCOOLDOWN& msg);
	HRESULT CheckCDMsgs();
	CAutoCritSec m_lockGraph; //graph state lock
	CAutoCritSec m_lockCD; //colldown lock

	ST_GRAPHDATA m_graphData;
	ST_GRAPHSTATE m_graphState;

	CTxTimer m_timerStateCheck;
	CTxTimer m_timerActSendCoolDown;

	BOOL m_bRunning;
	
	int m_nStatCheckInterval;
	int m_nCoolDownInterval;

	MAPCOOLDOWN m_lCooldownActs;

	//conifg
	int m_nSwitchCD;
	int m_nRelyCD;

	//for remote query graph state 
	CMBCGraphStateSvrObj m_gsHostObj;

public:
	HRESULT Test();

};
