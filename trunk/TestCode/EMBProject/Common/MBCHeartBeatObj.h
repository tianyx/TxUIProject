/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:05
	filename: 	MBCHeartBeatObj.h
	author:		tianyx
	
	purpose:  heartbeat object class for tcp connection. support auto reconnection
*********************************************************************/
#pragma once
#include "mbcbaseobj.h"

class CMBCHeartBeatObj :
	public CMBCBaseObj
{
public:
	CMBCHeartBeatObj(void);
	virtual ~CMBCHeartBeatObj(void);


	//see CMBCBaseObj
	virtual HRESULT	GetStateInfo(ST_OBJSTATEINFO& infoOut);

	//see CMBCBaseObj
	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	//see CMBCBaseObj
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed );

	//interface for ISockMsgCallbackInterFace see CMBCBaseObj
	virtual HRESULT NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);

	virtual HRESULT Run();
	virtual HRESULT Stop();

protected:
	//fill the live info of this
	virtual HRESULT FillLIvePack(ST_TXMSG_LIVEQA& msg){return S_OK;}

	//called when live msg incoming
	virtual HRESULT OnLiveMsgIn(ST_TXMSG_LIVEQA& msg){return S_OK;}


	//live msg 
	ST_TXMSG_LIVEQA m_LastReceivedInfo;

	//lock for live info.
	CAutoCritSec m_lockLastInfo;


	//timer for send and check live info
	CTxTimer m_timerLive;

	//the live pack request count
	INT64 m_nLiveRequestCount;

	//the live pack reply count
	INT64 m_nLiveReplyCount;

	//live pack send interval
	int INTERVAL_LIVECHECK;

	//the first live check delay time when run
	int DELAY_BEFORELIVECHECK;

};
