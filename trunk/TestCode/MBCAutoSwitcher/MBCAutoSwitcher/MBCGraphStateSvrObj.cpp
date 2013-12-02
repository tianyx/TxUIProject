#include "StdAfx.h"
#include "MBCGraphStateSvrObj.h"
#include "FGlobal.h"

CMBCGraphStateSvrObj::CMBCGraphStateSvrObj(void)
{
	m_pIGSCallback = NULL;
	m_nObjType = MBCOBJTYPE_GRAPHHOST;
	m_addrLocal.sin_family = AF_INET;
	m_addrLocal.sin_port = htons(0);
	m_addrLocal.sin_addr.S_un.S_addr = inet_addr( INADDR_ANY );
}

CMBCGraphStateSvrObj::~CMBCGraphStateSvrObj(void)
{
}

HRESULT CMBCGraphStateSvrObj::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{

	HRESULT hr = S_OK;
	int nLen = 0;


	int nRetUsed = 0;
	char buffer[MAXGRAPHBUFF];


	if (nMsgType == msgtype_GraphState)
	{
		//extract msg

		ST_GRAPHSTATE msgIn;
		if (m_pIGSCallback)
		{
			m_pIGSCallback->GetGraphState(msgIn);
		}
		hr = PackMBCMsg(msgIn, buffer, MAXGRAPHBUFF, nRetUsed);
	}
	else if (nMsgType == msgtype_ChangeRelay)
	{
		ST_CHANGERELAY msgIn;
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (m_pIGSCallback)
		{
			m_pIGSCallback->ChangeRelay(msgIn);
		}
	}
	

	//send info back
	if (nRetUsed > 0)
	{
		hr = send(*pMBCSock, buffer, nRetUsed, 0);
		CFWriteLog(TEXT("=send back to soc %d"), pMBCSock->m_hSock);
		if(hr == SOCKET_ERROR)
		{
			ASSERT(FALSE);
			hr = WSAGetLastError();
		}
	}

	return hr;
}

void CMBCGraphStateSvrObj::SetGraphStateCallback( IGraphStateCallbackInterface* pICall )
{
	m_pIGSCallback = pICall;
}
