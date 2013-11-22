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

//flag
#define MBCSOCKTYPE_UDP_MBC_SENDER 0x01
#define MBCSOCKTYPE_UDP_MBC_RECVER 0x02
#define MBCSOCKTYPE_UDP_MBC_BOTH 0x04

#define MBCSOCKTYPE_UDP_MBCTYPE 0x07 //used for internal

#define MBCSOCKTYPE_TCP_LISTENER 0x10
#define MBCSOCKTYPE_TCP_CTRL	 0x20

#define MBCSOCKTYPE_AUTORECONNECT 0x100

enum ENUMMBCSOCKSTATE
{
	MBCSOCKSTATE_OK = 0,
	MBCSOCKSTATE_UNINIT= -10,
	MBCSOCKSTATE_FALSE = -1,
	MBCSOCKSTATE_CLOSE = -2

};

struct MBCSOCKMSG
{
	WPARAM wparam;
	LPARAM lparam;
};

struct MCBSOCKADDRS
{
	SOCKADDR_IN addrLocal;
	SOCKADDR_IN addrRemote;
};

typedef std::vector<MBCSOCKMSG> VECSOCKMSG;

class CMBCSocket;

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
friend DWORD __stdcall CreateSockWndThread( void* lparam );
private:
	CMBCSocket(void);
	~CMBCSocket(void);

public:
	HRESULT Init();
	void UnInit();

	BOOL IsOpen();

	inline BOOL HasMsg();
	HRESULT PushMsg(const MBCSOCKMSG& msg);
	HRESULT CopyMsg( MBCSOCKMSG* pmsgOut, const int inBuffCount, int& nOut);

	operator SOCKET(){return m_hSock;}

	public:
	HANDLE m_hEventSockMsgArrival;
	HANDLE m_hEventQuit;
	SOCKET m_hSock;
	SOCKET m_hMBCSock; //for end obj only
	HANDLE m_hSockLoopProc; //for receive msg async
	HANDLE m_hWndThread;
	VECSOCKMSG m_vecSockMsg;

	CAutoCritSec m_lockdeque;

	HWND m_hSockWnd;
	MCBSOCKADDRS m_addrs;
	int m_nSockType;
	int m_nProtocolType;
	int m_nFavMsgType;
	int m_nCreateFlag;

	int  SOCKDOWN_RECONN_INTERVAL;

	ISockMsgCallbackInterFace* m_pCallBack;

	virtual HRESULT SockMsgCallback(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Read(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Connect(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Close(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Accept(WPARAM wParam, LPARAM lParam);
	virtual HRESULT SockCall_Write( WPARAM wParam, LPARAM lParam);

	BOOL CreateMsgWindow();
	BOOL StartMsgLoopThd();
	BOOL StopMsgLoopThd();

	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);


	CTxTimer m_timerReconnect;

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