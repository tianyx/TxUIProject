#include "StdAfx.h"
#include "MBCGDrawClientObj.h"
#include "MBCCommonDef.h"
#include "FGlobal.h"

CMBCGDrawClientObj::CMBCGDrawClientObj(void)
{
	m_ArrLineClr = Color::LightSalmon;

}

CMBCGDrawClientObj::~CMBCGDrawClientObj(void)
{
}

BOOL CMBCGDrawClientObj::Init( ST_OBJSTATEINFO& objInfo, const int nGraphState )
{
	CMBCGDrawBaseObj::Init(objInfo, nGraphState);

	//fill m_vLineoutInfo
	for (size_t i = 0; i <objInfo.remoteInfo.vCHInfo.size(); ++i )
	{
		ST_MBCCHANNELINFO& chInfo = objInfo.remoteInfo.vCHInfo[i];
		m_lineOutInfo.vaddrs.push_back(chInfo.addrCtrl);
	}

	return TRUE;
}

BOOL CMBCGDrawClientObj::CanConnectIn( CMBCGDrawBaseObj* pObjIn )
{
	return FALSE;
}

BOOL CMBCGDrawClientObj::ResetDrawData()
{

	 __super::ResetDrawData();
	 if (!m_pObjInfo)
	 {
		 return FALSE;
	 }
	 ST_MBCMSG_LIVEQA& remoteInfo = m_pObjInfo->remoteInfo;
	 ST_GDRAWITEM item;
	 m_vDrawData.push_back(item.Init(TEXT("监控IP:"),  Addr2String(m_pObjInfo->addrRemote).c_str()));
	for (size_t i = 0; i < remoteInfo.vCHInfo.size(); ++i)
	{
		if (i == 0)
		{
			m_vDrawData.push_back(item.Init(TEXT("目标服务器IP:"),  Addr2String(remoteInfo.vCHInfo[i].addrCtrl).c_str()));
			m_vDrawData.push_back(item.Init(TEXT("----频道列表----"), TEXT("")));
		}
		m_vDrawData.push_back(item.Init(TEXT("频道名称:"),  remoteInfo.vCHInfo[i].szChannelId));
		m_vDrawData.push_back(item.Init(TEXT("目标服务器IP:"),  Addr2String(remoteInfo.vCHInfo[i].addrCtrl).c_str()));
	}
	
	ReCalcNeedSize();

	return TRUE;
}

void CMBCGDrawClientObj::Draw( Graphics& gc )
{
	return __super::Draw(gc);
}
