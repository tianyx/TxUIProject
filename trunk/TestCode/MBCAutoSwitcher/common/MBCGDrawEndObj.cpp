#include "StdAfx.h"
#include "MBCGDrawEndObj.h"
#include "FGlobal.h"

CMBCGDrawEndObj::CMBCGDrawEndObj(void)
{
}

CMBCGDrawEndObj::~CMBCGDrawEndObj(void)
{
}

BOOL CMBCGDrawEndObj::Init( ST_OBJSTATEINFO& objInfo, const int nGraphState )
{
	CMBCGDrawBaseObj::Init(objInfo, nGraphState);

	//fill line in out info
	m_lineInInfo.vaddrs.push_back(objInfo.addrRemote);

	
	if (objInfo.nObjType == MBCOBJTYPE_ENDBACK)
	{
		for (size_t i = 0; i <objInfo.remoteInfo.vCHInfo.size(); ++i )
		{
			ST_MBCCHANNELINFO& chInfo = objInfo.remoteInfo.vCHInfo[i];
			//relay addr
			m_lineOutInfo.vaddrs.push_back(chInfo.addrDest);
		}
	}

	return TRUE;
	
}

BOOL CMBCGDrawEndObj::CanConnectIn( CMBCGDrawBaseObj* pObjIn )
{

	if (pObjIn->m_nObjType ==MBCOBJTYPE_CLIENT)
	{
		return FALSE;
	}

	return CMBCGDrawBaseObj::CanConnectIn(pObjIn);
}

BOOL CMBCGDrawEndObj::ResetDrawData()
{
	__super::ResetDrawData();
	if (!m_pObjInfo)
	{
		return FALSE;
	}
	ST_MBCMSG_LIVEQA& remoteInfo = m_pObjInfo->remoteInfo;
	ST_GDRAWITEM item;
	//m_vDrawData.push_back(item.Init(TEXT("�ͻ��˼��IP:"),  Addr2String(m_pObjInfo->addrRemote).c_str()));
	m_vDrawData.push_back(item.Init(TEXT("�ಥIP:"),  Addr2String(m_pObjInfo->addrRemote).c_str()));
	
	if (m_nObjType == MBCOBJTYPE_ENDBACK)
	{
		for (size_t i = 0; i < remoteInfo.vCHInfo.size(); ++i)
		{
			if (i == 0)
			{
				
				m_vDrawData.push_back(item.Init(TEXT("ת��Ƶ��ƥ���б�:"),  m_pObjInfo->strChId));
				m_vDrawData.push_back(item.Init(TEXT("----ת���б�----:"), TEXT("")));
				
			}

				//m_vDrawData.push_back(item.Init(TEXT("Ƶ������:"),  remoteInfo.vCHInfo[i].szChannelId));
				m_vDrawData.push_back(item.Init(TEXT("ת����ַ:"),  Addr2String(remoteInfo.vCHInfo[i].addrDest).c_str()));
		}
	}

	ReCalcNeedSize();

	return TRUE;
}
