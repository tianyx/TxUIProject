/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCSocket.h
	file base:	MBCSocket
	file ext:	h
	author:		tian_yx
	purpose:	socket wrap class, support tcp asynchronous connection,
				udp multicast, and listen sock,
*********************************************************************/
#pragma once
#include <vector>
#include "TxTimer.h"
#include "AutoCritSec.h"
using namespace std;

//socket message define for async sock
#define MSG_MBCNETSOCK WM_USER + 7999
#define MSG_MBCREQUESTBASEOBJ WM_USER + 8100

//timer id for sock reconnection 
#define IDTIMER_RECONN 1032

//timer id for live check
#define IDTIMER_LIVECHECK 1031

// #define IDTIMER_GRAPHCHECK 1033
// #define IDTIMER_COOLDOWN	1034

#define MAXRECVBUFF 40000
#define MAXGRAPHBUFF 40000

//flag of socket type can united using |
#define MBCSOCKTYPE_UDP_MBC_SENDER 0x01
#define MBCSOCKTYPE_UDP_MBC_RECVER 0x02
#define MBCSOCKTYPE_UDP_MBC_BOTH 0x04

#define MBCSOCKTYPE_UDP_MBCTYPE 0x07 //used for internal

#define MBCSOCKTYPE_TCP_LISTENER 0x10
#define MBCSOCKTYPE_TCP_CTRL	 0x20

#define MBCSOCKTYPE_AUTORECONNECT 0x100

//socket state
enum ENUMMBCSOCKSTATE
{
	MBCSOCKSTATE_OK = 0,
	MBCSOCKSTATE_UNINIT= -10,
	MBCSOCKSTATE_FALSE = -1,
	MBCSOCKSTATE_CLOSE = -2

};

//socket msg
struct MBCSOCKMSG
{
	WPARAM wparam;
	LPARAM lparam;
};

//socket address
struct MCBSOCKADDRS
{
	SOCKADDR_IN addrLocal;
	SOCKADDR_IN addrRemote;
};

//socket message vector
typedef std::vector<MBCSOCKMSG> VECSOCKMSG;

class CMBCSocket;

//interface for inherited class
interface ISockMsgCallbackInterFace
{
	virtual HRESULT NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT NetCall_Write(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam) = 0;
	virtual HRESULT SockStateChange(CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn) = 0;
};

DWORD __stdcall SockMsgLoopProc(void* lparam);

class CMBCSocket : public ITxTimerCallbackInterface
{
friend DWORD __stdcall SockMsgLoopProc(void* lparam);
//window loop for socket
friend DWORD __stdcall CreateSockWndThread( void* lparam );
private:
	CMBCSocket(void);
	~CMBCSocket(void);

public:

	HRESULT Init();
	void UnInit();

	BOOL IsOpen();


	HRESULT PushMsg(const MBCSOCKMSG& msg);
private:
	inline BOOL HasMsg();
	HRESULT CopyMsg( MBCSOCKMSG* pmsgOut, const int inBuffCount, int& nOut);
public:
	operator SOCKET(){return m_hSock;}

public:
	HANDLE m_hEventSockMsgArrival;	//event when sock message arrival
	HANDLE m_hEventQuit;			//event for quit
	SOCKET m_hSock;					//socket handle
	SOCKET m_hMBCSock;				//socket handle for multicast end obj only
	HANDLE m_hSockLoopProc;			//for receive msg asynchronous
	HANDLE m_hWndThread;			//for create socket window message thread
	VECSOCKMSG m_vecSockMsg;		//vector for socket

	CAutoCritSec m_lockdeque;		//lock for m_vecSockMsg

	HWND m_hSockWnd;				//window hwnd for socket
	MCBSOCKADDRS m_addrs;			//socket address
	int m_nSockType;				//socket type see socket type define
	int m_nProtocolType;			//protocol
	int m_nFavMsgType;				//support msg type
	int m_nCreateFlag;				//create type see MBCSOCKTYPE_AUTORECONNECT

	int  SOCKDOWN_RECONN_INTERVAL;	//socket reconn interval time

	ISockMsgCallbackInterFace* m_pCallBack;	//message callback

	//socket function call
	virtual HRESULT SockMsgCallback(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Read(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Connect(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Close(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Accept(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Write( WPARAM wParam, LPARAM lParam);

	//thread loop function
	BOOL CreateMsgWindow();
	BOOL StartMsgLoopThd();
	BOOL StopMsgLoopThd();

	//timer callback function
	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	//timer
	CTxTimer m_timerReconnect;

	//socket state
	ENUMMBCSOCKSTATE m_nSelfState;
	ENUMMBCSOCKSTATE GetState(){return m_nSelfState;}
private:
	void SetState(ENUMMBCSOCKSTATE stateIn);

public:
	//static CMBCSocket* CreateSocket(int nType, MCBSOCKADDRS& addrIn, int nFavMsgType, ISockMsgCallbackFace* pCallbackProc, int nFlag = 0);
	static CMBCSocket* CreateUDPSocket(MCBSOCKADDRS& addrIn, int nFavMsgType, ISockMsgCallbackInterFace* pCallbackProc, int nFlag = 0);
	static CMBCSocket* CreateTCPSocket(MCBSOCKADDRS& addrIn, int nFavMsgType, ISockMsgCallbackInterFace* pCallbackProc, int nFlag = 0);
	static CMBCSocket* AttachSock(SOCKET sockIn, int nFavMsgType, ISockMsgCallbackInterFace* pCallbackProc, int nFlag = 0);
	//do not to call delete functon,must use this.
	static BOOL ReleaseSock(CMBCSocket* pSockIn);

};


 DWORD __stdcall SockWndLoopProc(void* lparam);