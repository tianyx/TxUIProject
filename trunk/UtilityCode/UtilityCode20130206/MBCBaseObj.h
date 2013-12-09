/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCBaseObj.h
	file base:	MBCBaseObj
	file ext:	h
	author:		tian_yx
	purpose:	socket connection wrap class
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
	

	//*Description: set socket address, if pass null, will use the default address.
	BOOL SetScokAddr(SOCKADDR_IN* pAddrRemote, SOCKADDR_IN* paddrLocal);

	//*Description: get obj state
	ENUM_MBCSTATE GetState(){return m_nSelfState;};
	
	//*Description:get detail info of obj
	virtual HRESULT	GetStateInfo(ST_OBJSTATEINFO& infoOut);

	//*Description: the interface for ISockMsgCallbackInterFace
	//*Input Param: pMBCSock: the socket object; wParam and lparam see below, (ref: WSASelect() on msdn)
	//int nErr =WSAGETSELECTERROR(lParam);
	//int nEvent = WSAGETSELECTEVENT(lParam);
	//SOCKET socRecv = (SOCKET)wParam;
	//*Return Param: result
	//*History:
	virtual HRESULT NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Write(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	
	//*Description: callback for CMBCSocket inform parent that the socket state was changed.
	virtual HRESULT SockStateChange(CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn);

	//*Description: for ITxTimerCallbackInterface
	//*Input Param: dwEvent: the timer id; lparam: lparam
	//*Return Param: 
	//*History: 
	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	
	//*Description: process the message
	//*Input Param: nMsgType: user defined msgtype, see ST_TXMSGBASE.
	//*Input Param: bufferIn: the data transfered from socket.
	//*Input Param: nUsed: data len of bufferIn.
	//*Return Param: 
	//*History: 
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

	virtual HRESULT Run();
	virtual HRESULT Stop();

	void ChangeSelfState(ENUM_MBCSTATE stateIn);

	
	//*Description: set hwnd for passing log string.
	//*Input Param: hwndIn: handle of log window
	//*Return Param: 
	//*History: 
	void SetLogWnd(HWND hwndIn = NULL){m_hLogWnd = hwndIn;}

	//object state
	ENUM_MBCSTATE m_nSelfState;

	//obj type
	ENUM_MBCOBJTYPE m_nObjType;

	SOCKADDR_IN m_addrLocal;
	SOCKADDR_IN m_addrRemote; //addr for ctrl msg

	//socket object
	CMBCSocket* m_pSockBase;

	//callback interface when msg processed, not used..
	IMBCMsgPostProcessInterface* m_pMsgPostProc;

	//log wnd
	HWND m_hLogWnd;

	//time at which the object state change to error.
	UINT32 m_nLastStateErrTime;
	
	//self object name
	CString m_strObjName;
};


