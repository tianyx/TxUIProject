#include "StdAfx.h"
#include "MBCStateQueryObj.h"
#include "globaldef.h"
#include "MBCTransMsg.h"
#include "FGlobal.h"

CMBCStateQueryObj::CMBCStateQueryObj(void)
{
	INTERVAL_LIVECHECK = g_GlobalInfo.nStateGetInterval;
	DELAY_BEFORELIVECHECK = g_GlobalInfo.nStateGetStartDelay;

}

CMBCStateQueryObj::~CMBCStateQueryObj(void)
{
}

HRESULT CMBCStateQueryObj::TxTimerCallbackProc( DWORD dwEvent, LPARAM lparam )
{
	if (dwEvent == IDTIMER_LIVECHECK)
	{
		//send state info request msg;
		ST_GRAPHSTATE msg;
		char buffer[MAXGRAPHBUFF];
		int nUsed = 0;
		HRESULT hr = PackMBCMsg(msg, buffer, MAXGRAPHBUFF, nUsed);
		if (nUsed > 0 && m_pSockBase->IsOpen())
		{
			hr = send(*m_pSockBase, buffer, nUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				ASSERT(FALSE);
				hr = WSAGetLastError();
			}
		}
		return hr;
	}

	return S_OK;
}

HRESULT CMBCStateQueryObj::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == msgtype_GraphState)
	{
		ST_GRAPHSTATE msg;
		HRESULT hr =  UnPackMBCMsg(bufferIn, nUsed, msg);
		if (hr == S_OK)
		{
			//CFWriteLog("receive state info");
			if (m_pGsInCallback)
			{
				m_pGsInCallback->SetGraphState(msg);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
		return hr;
	}
	return CMBCHeartBeatObj::ProcessIncomingMsg(pMBCSock, nMsgType, bufferIn, nUsed);
}

HRESULT CMBCStateQueryObj::SetGraphStateIncomingCallback( IGraphStateIncomingCallback* pCallback )
{
	m_pGsInCallback = pCallback;
	return S_OK;
}
