#include "StdAfx.h"
#include "UISvrProber.h"
#include "EMBMessageDef.h"
#include "FGlobal.h"
#include "EmbStructDef.h"

CUISvrProber::CUISvrProber(void)
{
	m_pIProcessor = NULL;
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
	HRESULT hr = CMBCRemoteObj::Run();
	CFWriteLog(0, TEXT("uisvr Run_Prober return %x"), hr);
	return hr;
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

	EMB::CAutoLock lock(&m_csSockIn);
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
	if (nMsgType == msgtype_LIVEQA)
	{
		//extract msg
		//CFWriteLog("recv live Q");
		ST_TXMSG_LIVEQA msgRet;
		UnPackMBCMsg(bufferIn, nUsed, msgRet);
		//get live info
		msgRet.nMsgState = msgState_A;
		int nRetUsed = 0;
		char buffer[MIDBUFFSIZE];
		PackMBCMsg(msgRet, buffer, MIDBUFFSIZE, nRetUsed);
		//send info back
		if (nRetUsed > 0)
		{
			hr = send(*pMBCSock, buffer, nRetUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				hr = WSAGetLastError();
				ASSERT(FALSE);

			}
		}

	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
}

HRESULT CUISvrProber::OnSockConnected( CMBCSocket* pMBCSock )
{
	//send svr info to client
	if (!m_pIProcessor)
	{
		return EMBERR_NOTIMPL;
	}
	ST_SVRLIVEINFO livInfo;
	CString strReq, strRet;
	livInfo.ToString(strReq);

	m_pIProcessor->UIMessageCallback(strReq, strRet);
	if (!strRet.IsEmpty())
	{
		SendToUI(strRet);
	}

	return S_OK;
}
