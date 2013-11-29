#include "StdAfx.h"
#include "MBCBaseObj.h"
#include "FGlobal.h"
#include "MBCTransMsg.h"


// DWORD __stdcall SockWndLoopProc( void* lparam )
// {
// 	CMBCBaseObj* pbaseObj = (CMBCBaseObj*)lparam;
// 	ASSERT(pbaseObj);
// 
// 	MSG          msg;
// 	BOOL bRet;
// 	while( (bRet = GetMessage( &msg, pbaseObj->m_hwndSockMsgRecver, 0, 0 )) != 0)
// 	{ 
// 		if (bRet == -1)
// 		{
// 			// handle the error and possibly exit
// 		}
// 		else
// 		{
// 			
// 			TranslateMessage (&msg);
// 			DispatchMessage (&msg);
// 
// 		}
// 	}
// 	return msg.wParam;
// 
// }


CMBCBaseObj::CMBCBaseObj(void)
{
	m_nObjType = MBCOBJTYPE_BASE;
	m_nSelfState = MBCSTATE_STOP;
	m_hLogWnd = NULL;
	m_pSockBase = NULL;
	m_addrLocal.sin_family = AF_INET;
	m_addrLocal.sin_port =  htons(0);
	m_addrLocal.sin_addr.S_un.S_addr =htonl(INADDR_ANY);
	m_addrRemote.sin_family = AF_INET;
	m_addrRemote.sin_port =  htons(0);
	m_addrRemote.sin_addr.S_un.S_addr =htonl(INADDR_ANY);
	m_pMsgPostProc = NULL;
	m_nLastStateErrTime = 0;
}

CMBCBaseObj::~CMBCBaseObj(void)
{
}


HRESULT CMBCBaseObj::NetCall_Read(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	if (nErr != S_OK)
	{
		ASSERT(FALSE);
		return nErr;
	}
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;

	char szbuff[MAXRECVBUFF];
	ZeroMemory(szbuff, MAXRECVBUFF);
	DWORD dwFlag = 0;
	int nHeadRead = recv(socRecv, szbuff, 5, 0);
	BOOL bSafeEnd = TRUE;
	while(nHeadRead == 5)
	{
		char* pBuff = szbuff;
		if (*pBuff == MBCCHECKSUM)
		{
			pBuff++;
			int nMsgLen = *((int*)pBuff);
			ASSERT(nMsgLen > 5 && nMsgLen < MAXINT);
			char* pRealUsedBuffer = szbuff;
			char* pLargeBuff = NULL;
			if (nMsgLen > MAXRECVBUFF)
			{
				//buffer not enough!
				pLargeBuff = new char[nMsgLen];
				pRealUsedBuffer = pLargeBuff;
				memcpy(pLargeBuff,  szbuff, 5);
				pBuff = pLargeBuff;
				pBuff ++;
			}
			pBuff += 4;
			int nMsgRead = recv(socRecv, pBuff, nMsgLen-5, 0);
			while(nMsgRead < nMsgLen -5)
			{
				int nTmpRead =recv(socRecv, pBuff, nMsgLen-5 - nMsgRead, 0);
				if (nTmpRead == SOCKET_ERROR)
				{
					break;
				}
				else
				{
					nMsgRead += nTmpRead;
				}
			}

			if (nMsgRead == nMsgLen-5)
			{
				int nMsgType =  GetMBCMsgType(pRealUsedBuffer, nMsgLen);
				if (nMsgType != msgtype_NONE)
				{
					ProcessIncomingMsg(pMBCSock, nMsgType, pRealUsedBuffer, nMsgLen);
					//CFWriteLog2Wnd(m_hLogWnd, "msg ReadData, sock = %d, len = %d", (int)wParam, nMsgRead+5);
				}
			}
			else
			{
				ASSERT(FALSE);
				bSafeEnd = FALSE;
			}

			if (pLargeBuff)
			{
				delete[] pLargeBuff;
				pLargeBuff = NULL;
			}
			if (!bSafeEnd)
			{
				break;
			}

		}
		else
		{
			ASSERT(FALSE);
			bSafeEnd = FALSE;
			break;
		}
		nHeadRead = recv(socRecv, szbuff, 5, 0);
	}

	if (!bSafeEnd)
	{
		//clean all recvdata
		ASSERT(FALSE);
		int nRead = recv(socRecv, szbuff, MAXRECVBUFF, 0);
		while(nRead != SOCKET_ERROR)
		{
			nRead = recv(socRecv, szbuff, MAXRECVBUFF, 0);
		}
		//CFWriteLog2Wnd(m_hLogWnd, "msg ReadData error, data in sock buffer will clear!!! sock = %d", (int)wParam);
		return E_FAIL;
	}
	
	return S_OK;
}


HRESULT CMBCBaseObj::Run()
{
	return S_OK;
}

HRESULT CMBCBaseObj::Stop()
{
	if (m_pSockBase)
	{
		CMBCSocket::ReleaseSock(m_pSockBase);
		m_pSockBase = NULL;
	}
	m_nSelfState = MBCSTATE_STOP;
	return S_OK;
}

HRESULT CMBCBaseObj::TxTimerCallbackProc( DWORD dwEvent, LPARAM lparam )
{
	return S_OK;
}

HRESULT CMBCBaseObj::ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{

	return S_OK;

}


HRESULT CMBCBaseObj::NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	//i am client
	ASSERT(socRecv == *pMBCSock);
	if (nErr ==S_OK)
	{	
		//CFWriteLog("==connected, soc = %d, ip =%s", socRecv, Addr2String(pMBCSock->m_addrs.addrRemote).c_str());
		ChangeSelfState(MBCSTATE_OK);
	}
	return S_OK;
}

HRESULT CMBCBaseObj::NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	if (socRecv ==  *m_pSockBase)
	{
		ChangeSelfState(MBCSTATE_NETERR);
	}
	TRACE("\n Sock Closed");

	return S_OK;
}

HRESULT CMBCBaseObj::NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	//not process
	ASSERT(FALSE);

	return S_OK;
}

HRESULT CMBCBaseObj::NetCall_Write(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam )
{
	int nErr =WSAGETSELECTERROR(lParam);
	int nEvent = WSAGETSELECTEVENT(lParam);
	SOCKET socRecv = (SOCKET)wParam;
	//not process
	ASSERT(FALSE);
	return S_OK;
}

HRESULT CMBCBaseObj::SockStateChange(CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn )
{
	if (socStateIn != MBCSOCKSTATE_OK)
	{
		ChangeSelfState(MBCSTATE_NETERR);
	}
	else if (socStateIn == MBCSOCKSTATE_UNINIT)
	{
		ChangeSelfState(MBCSTATE_STOP);
	}
	else if (socStateIn == MBCSOCKSTATE_OK)
	{
		if(m_nSelfState == MBCSTATE_NETERR)
		{
			ChangeSelfState(MBCSTATE_OK);
		}
	}

	return S_OK;
}

HRESULT CMBCBaseObj::GetStateInfo( ST_OBJSTATEINFO& infoOut )
{
	return E_NOTIMPL;
}

void CMBCBaseObj::ChangeSelfState( ENUM_MBCSTATE stateIn )
{
	if (stateIn == MBCSTATE_NETERR && m_nSelfState == MBCSTATE_OK)
	{
		//new err 
		m_nLastStateErrTime = GetTickCount();
	}
	
	m_nSelfState = stateIn;
	
}

BOOL CMBCBaseObj::SetScokAddr( SOCKADDR_IN* pAddrRemote, SOCKADDR_IN* paddrLocal )
{
	if (pAddrRemote)
	{
		m_addrRemote = *pAddrRemote;
	}
	if (paddrLocal)
	{
		m_addrLocal = *paddrLocal;
	}

	return TRUE;
}
