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

/*
*Description：进程间通讯接收消息函数
*Input Param：
*      pMBCSock ：EMB任务XML描述
*      nMsgType ：消息类型 ， 请参考EMBDefine.h头文件定义
*      bufferIn ：XML内容字符串信息指针，此BUFFER需要通过UnPackMBCMsg解析后得到
*      nUsed    ：XML字符串内容有效长度
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskProberTcp::ProcessIncomingMsg( CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed )
{
	HRESULT hr = S_OK;


	if (nMsgType == embmsgtype_TaskIncoming)
	{
		int nLen = 0;
		int nRetUsed = 0;

		//解析数据
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
		CEMBAutoBuffer buffer(msgIn);
		PackMBCMsg(msgRet, buffer, buffer.GetSize(), nRetUsed);
		//send info back
		if (nRetUsed > 0)
		{
			hr = send(*pMBCSock, buffer, nRetUsed, 0);
			if(hr == SOCKET_ERROR)
			{
				ASSERT(FALSE);
				hr = WSAGetLastError();
				CFWriteLog(TEXT("prober send back to soc %d error %d"), pMBCSock->m_hSock, hr);

			}
		}
		return hr;

	}
	else
	{
		return E_FAIL;
	}
}

/*
*Description：分解消息并将任务信息（XML方式）添加到任务列表中
*Input Param：
*      msgIn ：
*      msgRet ：
*Return Param：返回成功或失败
*History：
*/
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

/*
*Description：启动接收任务功能
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskProberTcp::Run_Prober()
{
	return CMBCRemoteObj::Run();
}

/*
*Description：停止接收任务功能
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskProberTcp::Stop_Prober()
{
	return CMBCRemoteObj::Stop();
}
