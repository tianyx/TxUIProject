#include "StdAfx.h"
#include "ActorConnector.h"
#include "MBCTransMsg.h"
#include "FGlobal.h"
#include "TxParamString.h"
#include "EMBMessageDef.h"
CActorConnector::CActorConnector(void)
{
	m_pITaskCommit = NULL;
}

CActorConnector::~CActorConnector(void)
{
}

HRESULT CActorConnector::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	// Actor处理
	if (nMsgType == embmsgtype_DispatchToActorMsg)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (m_pITaskCommit)
		{
			CString strRet;
			// 解析消息类型
			m_pITaskCommit->OnActorConnectorMsg(msgIn.strData, strRet);
			if (!strRet.IsEmpty())
			{
				ST_EMBTRANSMSG msgOut(embmsgtype_ActorToDispatchMsg);
				msgOut.strGuid = msgIn.strGuid;
				msgOut.strData = strRet;
				CEMBAutoBuffer szbuff(msgOut);
				int nRetUsed = 0;
				PackMBCMsg(msgOut, szbuff, szbuff.GetSize(), nRetUsed);
				//send back to dispatch
				HRESULT hr = send(*pMBCSock, szbuff, nRetUsed, 0);
				if (hr == SOCKET_ERROR)
				{
					hr = WSAGetLastError();
					ASSERT(FALSE);
				}
				else
				{
					hr = S_OK;
				}
				
			}
		}
	}
	else if (nMsgType == embmsgtype_ActorReportGuid)
	{
		CFWriteLog(0, TEXT("dispatch request actorid"));
		ST_EMBTRANSMSG msgIn(nMsgType);
		CTxStrConvert val;
		val.SetVal(m_nActorId);
		msgIn.strData = val.GetAsString(); 
		CEMBAutoBuffer szbuff(msgIn);
		int nRetUsed = 0;
		PackMBCMsg(msgIn, szbuff, szbuff.GetSize(), nRetUsed);
		//send back to dispatch
		HRESULT hr = send(*pMBCSock, szbuff, nRetUsed, 0);
		if(hr == SOCKET_ERROR)
		{
			hr = WSAGetLastError();
			ASSERT(FALSE);
		}
	}
	else 
	{
		return __super::ProcessIncomingMsg(pMBCSock, nMsgType, bufferIn, nUsed);
	}
	return S_OK;
}

HRESULT CActorConnector::SendtoDispatcher( CString& strInfo )
{
	ST_EMBTRANSMSG msgIn(embmsgtype_ActorToDispatchMsg);
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
