#include "StdAfx.h"
#include "MBCSvrObj.h"
#include "MBCTransMsg.h"

CMBCSvrObj::CMBCSvrObj()
{
	m_nObjType = MBCOBJTYPE_SVR;
}

CMBCSvrObj::~CMBCSvrObj(void)
{
}

HRESULT CMBCSvrObj::ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	if (nMsgType == msgtype_ACTQA)
	{
		ST_MBCMSG_ACTQA msg;
		HRESULT hr = UnPackMBCMsg(bufferIn, nUsed, msg);
		ASSERT(hr == S_OK);
		if(msg.nActType == actType_changeMBCDest)
		{
			ASSERT(msg.nResult == S_OK);
		}
		return S_OK;
	}
	
	return CMBCHeartBeatObj::ProcessIncomingMsg(pMBCSock,nMsgType, bufferIn, nUsed);
}
