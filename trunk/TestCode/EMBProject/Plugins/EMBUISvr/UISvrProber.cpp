#include "StdAfx.h"
#include "UISvrProber.h"
#include "EMBMessageDef.h"
#include "FGlobal.h"

CUISvrProber::CUISvrProber(void)
{
}

CUISvrProber::~CUISvrProber(void)
{
}

BOOL CUISvrProber::SetMsgProcessor( IUISvrMsgCallbackInterface* pProcessor )
{
	ASSERT(m_pIProcessor == NULL);
	m_pIProcessor = pProcessor;
	return TRUE;
}

HRESULT CUISvrProber::Run_Prober()
{
	return CMBCRemoteObj::Run();
}

HRESULT CUISvrProber::Stop_Prober()
{
	return CMBCRemoteObj::Stop();
}

HRESULT CUISvrProber::SendToUI( CString& taskIn )
{
	ST_EMBTRANSMSG msgRet(embmsgtype_UIServerToUIClientMsg);
	msgRet.strData = taskIn;
	CEMBAutoBuffer buffer(msgRet);
	int nRetUsed = 0;
	PackMBCMsg(msgRet, buffer, buffer.GetSize(), nRetUsed);

	CAutoLock lock(&m_csSockIn);
	MAPSOCKINS::iterator itb = m_mapSockIns.begin();
	MAPSOCKINS::iterator ite = m_mapSockIns.end();
	for (; itb != ite; ++itb)
	{
		CMBCSocket* pMBCSock = itb->first;
		
		//send info back
		if (nRetUsed > 0)
		{
			HRESULT hr = send(*pMBCSock, buffer, nRetUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				ASSERT(FALSE);
				hr = WSAGetLastError();
				CFWriteLog(TEXT("sendto UIClient soc %d error %d"), pMBCSock->m_hSock, hr);
			}
		}
	}
	return S_OK;
}

HRESULT CUISvrProber::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (!m_pIProcessor)
	{
		return E_FAIL;
	}

	HRESULT hr = S_OK;
	if (nMsgType == embmsgtype_UIClientToUIServerMsg)
	{
		int nLen = 0;
		int nRetUsed = 0;

		//½âÎöÊý¾Ý
		ST_EMBTRANSMSG msgIn(nMsgType);
		if(UnPackMBCMsg(bufferIn, nUsed, msgIn) != 0)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}

		//send back to sender
		ST_EMBTRANSMSG msgRet(embmsgtype_UIServerToUIClientMsg);
		msgRet.nMsgId = msgIn.nMsgId;
		msgRet.strGuid = msgIn.strGuid;
		
		m_pIProcessor->UIMessageCallback(msgIn.strData, msgRet.strData);
		CEMBAutoBuffer buffer(msgRet);
		PackMBCMsg(msgRet, buffer, buffer.GetSize(), nRetUsed);
		//send info back
		if (nRetUsed > 0)
		{
			hr = send(*pMBCSock, buffer, nRetUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				ASSERT(FALSE);
				hr = WSAGetLastError();
				CFWriteLog(TEXT("send back to UIClient soc %d error %d"), pMBCSock->m_hSock, hr);
			}
		}
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}
