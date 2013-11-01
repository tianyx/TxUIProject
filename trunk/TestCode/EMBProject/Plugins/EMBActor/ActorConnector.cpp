#include "StdAfx.h"
#include "ActorConnector.h"
#include "MBCTransMsg.h"

CActorConnector::CActorConnector(void)
{
	m_pITaskCommit = NULL;
}

CActorConnector::~CActorConnector(void)
{
}

HRESULT CActorConnector::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == embmsgtype_ReportActorInfo
		||nMsgType == embmsgtype_ReportGuid
		||nMsgType == embmsgtype_TaskDispath)
	{
		ST_EMBTRANSMSG msgIn(nMsgType);
		UnPackMBCMsg(bufferIn, nUsed, msgIn);
		if (m_pITaskCommit)
		{
			m_pITaskCommit->OnActorConnectorMsg(msgIn);
		}

	}
	else 
	{
		return __super::ProcessIncomingMsg(pMBCSock, nMsgType, bufferIn, nUsed);
	}
	return S_OK;
}
