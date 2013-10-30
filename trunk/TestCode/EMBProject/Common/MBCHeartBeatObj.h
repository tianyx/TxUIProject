#pragma once
#include "mbcbaseobj.h"

class CMBCHeartBeatObj :
	public CMBCBaseObj
{
public:
	CMBCHeartBeatObj(void);
	virtual ~CMBCHeartBeatObj(void);

	CTxTimer m_timerLive;
	INT64 m_nLiveRequestCount;
	INT64 m_nLiveReplyCount;

	int INTERVAL_LIVECHECK;
	int DELAY_BEFORELIVECHECK;
	CAutoCritSec m_lockLastInfo;

	virtual HRESULT	GetStateInfo(ST_OBJSTATEINFO& infoOut);


	ST_MBCMSG_LIVEQA m_LastReceivedInfo;

	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed );

	virtual HRESULT NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);

	virtual HRESULT Run();
	virtual HRESULT Stop();

};
