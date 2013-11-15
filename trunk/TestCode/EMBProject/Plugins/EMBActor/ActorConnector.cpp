#include "StdAfx.h"
#include "ActorConnector.h"
#include "MBCTransMsg.h"
#include "FGlobal.h"
#include "TxParamString.h"
CActorConnector::CActorConnector(void)
{
	m_pITaskCommit = NULL;
}

CActorConnector::~CActorConnector(void)
{
}

HRESULT CActorConnector::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == embmsgtype_DispatchToActorMsg)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (m_pITaskCommit)
		{
			CString strRet;
			m_pITaskCommit->OnActorConnectorMsg(msgIn.strData, strRet);
			if (!strRet.IsEmpty())
			{
				msgIn.strData = strRet;
				CEMBAutoBuffer szbuff(msgIn);
				int nRetUsed = 0;
				PackMBCMsg(msgIn, szbuff, szbuff.GetSize(), nRetUsed);
				//send back to dispatch
				HRESULT hr = send(*pMBCSock, szbuff, nRetUsed, 0);
				MUSTBESOK(hr);
			}
		}
	}
	else if (nMsgType == embmsgtype_ActorReportGuid)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		CTxStrConvert val;
		val.SetVal(m_nActorId);
		msgIn.strData = val.GetAsString(); 
		CEMBAutoBuffer szbuff(msgIn);
		int nRetUsed = 0;
		PackMBCMsg(msgIn, szbuff, szbuff.GetSize(), nRetUsed);
		//send back to dispatch
		HRESULT hr = send(*pMBCSock, szbuff, nRetUsed, 0);
		MUSTBESOK(hr);
	}
	else 
	{
		return __super::ProcessIncomingMsg(pMBCSock, nMsgType, bufferIn, nUsed);
	}
	return S_OK;
}

HRESULT CActorConnector::SendtoDispatcher( CString& strInfo )
{
	ST_EMBTRANSMSG msgIn(embmsgtype_ActorToDispath);
	CTxStrConvert val;
	val.SetVal(m_nActorId);
	msgIn.strData = val.GetAsString(); 
	CEMBAutoBuffer szbuff(msgIn);
	int nRetUsed = 0;
	PackMBCMsg(msgIn, szbuff, szbuff.GetSize(), nRetUsed);
	//send back to dispatch
	HRESULT hr = send(*m_pSockBase, szbuff, nRetUsed, 0);
	MUSTBESOK(hr)
	return hr;
}
