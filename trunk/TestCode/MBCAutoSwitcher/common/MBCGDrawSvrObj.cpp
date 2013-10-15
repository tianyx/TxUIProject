#include "StdAfx.h"
#include "MBCGDrawSvrObj.h"
#include "FGlobal.h"

CMBCGDrawSvrObj::CMBCGDrawSvrObj(void)
{
	m_ArrLineClr = Color::SkyBlue;
}

CMBCGDrawSvrObj::~CMBCGDrawSvrObj(void)
{
}

BOOL CMBCGDrawSvrObj::Init( ST_OBJSTATEINFO& objInfo, const int nGraphState )
{
	CMBCGDrawBaseObj::Init(objInfo, nGraphState);
	

	for (size_t i = 0; i <objInfo.remoteInfo.vCHInfo.size(); ++i )
	{
		ST_MBCCHANNELINFO& chInfo = objInfo.remoteInfo.vCHInfo[i];

		m_lineOutInfo.vaddrs.push_back(chInfo.addrDest);
		
		//file Line in info
		m_lineInInfo.vaddrs.push_back(chInfo.addrCtrl);
	}

	return TRUE;
}

BOOL CMBCGDrawSvrObj::CanConnectIn( CMBCGDrawBaseObj* pObjIn )
{
	if (pObjIn->m_nObjType != MBCOBJTYPE_CLIENT)
	{
		return FALSE;
	}

	return CMBCGDrawBaseObj::CanConnectIn(pObjIn);
	
}

BOOL CMBCGDrawSvrObj::ResetDrawData()
{
	 __super::ResetDrawData();
	 if (!m_pObjInfo)
	 {
		 return FALSE;
	 }
	 ST_MBCMSG_LIVEQA& remoteInfo = m_pObjInfo->remoteInfo;
	 ST_GDRAWITEM item;
	 m_vDrawData.push_back(item.Init(TEXT("���IP:"),  Addr2String(m_pObjInfo->addrRemote).c_str()));
	 for (size_t i = 0; i < remoteInfo.vCHInfo.size(); ++i)
	 {
		 if (i == 0)
		 {
			 m_vDrawData.push_back(item.Init(TEXT("����������IP:"),  Addr2String(remoteInfo.vCHInfo[i].addrCtrl).c_str()));
			 m_vDrawData.push_back(item.Init(TEXT("Ƶ������:"),  remoteInfo.vCHInfo[i].szChannelId));
			 m_vDrawData.push_back(item.Init(TEXT("��ǰ�ಥIP:"),  Addr2String(remoteInfo.vCHInfo[i].addrDest).c_str()));
			 m_vDrawData.push_back(item.Init(TEXT("���öಥIP:"),  Addr2String(remoteInfo.vCHInfo[i].addrDestSecond).c_str()));

		 }
	 }

	 ReCalcNeedSize();

	return TRUE;
}
