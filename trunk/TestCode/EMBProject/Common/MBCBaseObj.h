/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCBaseObj.h
	file base:	MBCBaseObj
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include <vector>
#include "MBCSocket.h"
#include "MBCCommonDef.h"
#include "TxTimer.h"



using namespace std;
typedef vector<SOCKET> VECSOCKS;

class CMBCBaseObj;
interface IMBCMsgPostProcessInterface
{
	//virtual HRESULT OnActMsgReply(CMBCBaseObj* pMBCObj,  ST_MBCMSG_ACTQA& msg) = 0;
};


class CMBCBaseObj: public ISockMsgCallbackInterFace, public ITxTimerCallbackInterface
{
public:
	CMBCBaseObj(void);
	virtual ~CMBCBaseObj(void);
	BOOL SetScokAddr(SOCKADDR_IN* pAddrRemote, SOCKADDR_IN* paddrLocal);

	ENUM_MBCSTATE m_nSelfState;
	ENUM_MBCSTATE GetState(){return m_nSelfState;};
	
	virtual HRESULT	GetStateInfo(ST_OBJSTATEINFO& infoOut);

	ENUM_MBCOBJTYPE m_nObjType;
	virtual HRESULT NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Write(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);

	virtual HRESULT SockStateChange(CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn);

	//timer callback
	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

	virtual HRESULT Run();
	virtual HRESULT Stop();
	void ChangeSelfState(ENUM_MBCSTATE stateIn);

	void SetLogWnd(HWND hwndIn = NULL){m_hLogWnd = hwndIn;}

	SOCKADDR_IN m_addrLocal;
	SOCKADDR_IN m_addrRemote; //addr for ctrl msg
	CMBCSocket* m_pSockBase;
	IMBCMsgPostProcessInterface* m_pMsgPostProc;

	HWND m_hLogWnd;

	UINT32 m_nLastStateErrTime;
	CString m_strObjName;
};


