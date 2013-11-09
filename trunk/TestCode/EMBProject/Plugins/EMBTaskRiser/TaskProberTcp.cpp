#include "StdAfx.h"
#include "TaskProberTcp.h"
#include "EMBDefine.h"
#include "MBCTransMsg.h"
#include "FGlobal.h"
#include "EMBdocdef.h"

CTaskProberTcp::CTaskProberTcp(void)
{
}

CTaskProberTcp::~CTaskProberTcp(void)
{
}

HRESULT CTaskProberTcp::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	HRESULT hr = S_OK;


	if (nMsgType == embmsgtype_TaskIncoming)
	{
		int nLen = 0;
		int nRetUsed = 0;
		char buffer[MAXGRAPHBUFF];

		//extract msg
		ST_EMBTRANSMSG msgIn(nMsgType);
		if(UnPackMBCMsg(bufferIn, nUsed, msgIn) != 0)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}
		//send back to sender
		ST_EMBTRANSMSG msgRet(nMsgType);
		msgRet.nMsgId = msgIn.nMsgId;
		msgRet.strGuid = msgIn.strGuid;
		TransTask(msgIn, msgRet);
		hr = PackMBCMsg(msgRet, buffer, MAXGRAPHBUFF, nRetUsed);
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
	else
	{
		return E_FAIL;
	}
}

HRESULT CTaskProberTcp::TransTask( ST_EMBTRANSMSG& msgIn, ST_EMBTRANSMSG& msgRet )
{
	msgRet.nMsgState = embmsgstate_A;
	msgRet.nMsgId = msgIn.nMsgId;
	CTxParamString txTask(msgIn.strData);
	HRESULT hr = S_OK;
	//find key 
	CString strMainGuid = txTask.GetAttribVal(EK_MAIN, EA_MAIN_GUID).GetAsString();
	int nMainType = txTask.GetAttribVal(EK_MAIN, EA_MAIN_TYPE).GetAsInt();
	if (nMainType !=embmsgtype_TaskIncoming || strMainGuid.IsEmpty())
	{
		ASSERT(FALSE);
		hr = EMBERR_INVALIDARG;
	}
	else
	{
		hr =AddTask(msgIn.strData);
	}
	
	
	msgRet.strData.Format(EDOC_COMMONRETFMT, nMainType, strMainGuid, hr);
	return hr;

}

HRESULT CTaskProberTcp::Run_Prober()
{
	return CMBCRemoteObj::Run();
}

HRESULT CTaskProberTcp::Stop_Prober()
{
	return CMBCRemoteObj::Stop();
}
