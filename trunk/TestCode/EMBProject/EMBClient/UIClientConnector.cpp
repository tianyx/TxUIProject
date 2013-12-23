#include "StdAfx.h"
#include "UIClientConnector.h"
#include "EMBDefine.h"
#include "MBCTransMsg.h"
#include "EMBMessageDef.h"
#include "FGlobal.h"
#include "EmbStructDef.h"

CUIClientConnector::CUIClientConnector(void)
{
	m_nMaster = INVALID_VALUE;
	m_connState = embConnState_error;
}

CUIClientConnector::~CUIClientConnector(void)
{
}

HRESULT CUIClientConnector::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == embmsgtype_UIServerToUIClientMsg)
	{
		HRESULT hr = S_OK;
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		
		ST_EMBTRANSMSG msgOut(embmsgtype_UIClientToUIServerMsg);

		if (m_pIProcess)
		{
			hr = m_pIProcess->OnMessage(m_nMaster, msgIn.strData, msgOut.strData);
		}
		 
		if (!msgOut.strData.IsEmpty())
		{
			CEMBAutoBuffer szbuff(msgOut);
			int nRetUsed = 0;
			PackMBCMsg(msgOut, szbuff, szbuff.GetSize(), nRetUsed);
			//send back to dispatch
			hr = send(*pMBCSock, szbuff, nRetUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				hr = WSAGetLastError();
				CFWriteLog(0, TEXT("send back to uisvr error = %d!"), hr);
				ASSERT(FALSE);
			}
		}
		return hr;
	}
	else
	{
		return __super::ProcessIncomingMsg(pMBCSock, nMsgType, bufferIn, nUsed);
	}

}

HRESULT CUIClientConnector::SendtoUISvr( CString& strInfo )
{
	if (strInfo.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}

	ST_EMBTRANSMSG msgIn(embmsgtype_UIClientToUIServerMsg);
	msgIn.strData = strInfo; 
	CEMBAutoBuffer szbuff(msgIn);
	int nRetUsed = 0;
	PackMBCMsg(msgIn, szbuff, szbuff.GetSize(), nRetUsed);
	//send back to dispatch
	HRESULT hr = send(*m_pSockBase, szbuff, nRetUsed, 0);
	if (hr == SOCKET_ERROR)
	{
		hr = WSAGetLastError();
		ASSERT(FALSE);
	}

	return hr;
}

HRESULT CUIClientConnector::Run()
{
	return CMBCHeartBeatObj::Run();
}

HRESULT CUIClientConnector::Stop()
{
	return CMBCHeartBeatObj::Stop();
}

HRESULT CUIClientConnector::SockStateChange( CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn )
{
	int tmpState = socStateIn == MBCSOCKSTATE_OK? embConnState_ok:embConnState_error;
	if (tmpState != m_connState)
	{
		m_connState = tmpState;
		//inform upper.
		if (m_pIProcess)
		{
			m_pIProcess->OnConnStateChange(m_nMaster, tmpState);
		}
	}
	return S_OK;
}

BOOL CUIClientConnector::SetParam(int nMasterId, SOCKADDR_IN& addrConn )
{
	m_nMaster = nMasterId;
	SetScokAddr(&addrConn, NULL);
	return TRUE;
}
