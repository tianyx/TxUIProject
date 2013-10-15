#include "StdAfx.h"
#include "MBCClientObj.h"
#include "MBCTransMsg.h"

CMBCClientObj::CMBCClientObj(void)
{
	m_nObjType = MBCOBJTYPE_CLIENT;
}

CMBCClientObj::~CMBCClientObj(void)
{
}

HRESULT CMBCClientObj::ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == msgtype_ACTQA)
	{
		ST_MBCMSG_ACTQA msg;
		HRESULT hr =  UnPackMBCMsg(bufferIn, nUsed, msg);
		ASSERT(hr == S_OK);
		if(msg.nActType == actType_changeMBCDest)
		{
			ASSERT(msg.nResult == S_OK);
			if (m_pMsgPostProc)
			{
				m_pMsgPostProc->OnActMsgReply(this, msg);
			}
		}
		return S_OK;
	}
	return CMBCHeartBeatObj::ProcessIncomingMsg( pMBCSock,nMsgType, bufferIn, nUsed);
}

