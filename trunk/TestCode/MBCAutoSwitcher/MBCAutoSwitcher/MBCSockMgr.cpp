#include "StdAfx.h"
#include "MBCSockMgr.h"
#include <algorithm>

CMBCSockMgr::CMBCSockMgr(void)
{
}

CMBCSockMgr::~CMBCSockMgr(void)
{
}

HRESULT CMBCSockMgr::RegisterSock( SOCKET hsock, CMBCBaseObj* pBaseObj )
{
	MAPSOCKMBCOBJS::iterator itf = m_mapSockToObj.find(hsock);
	if (itf != m_mapSockToObj.end())
	{
		if(pBaseObj != itf->second)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}
	}
	else
	{
		m_mapSockToObj[hsock] = pBaseObj;
	}

	//put to map2
	MAPMBCOBJSOCKS::iterator itf2 = m_mapObjToSocks.find(pBaseObj);
	if (itf2 != m_mapObjToSocks.end())
	{
		itf2->second.push_back(hsock);
	}
	else
	{
		VECSOCKS vTmp;
		vTmp.push_back(hsock);
		m_mapObjToSocks[pBaseObj] = vTmp;
	}

	return S_OK;
}

HRESULT CMBCSockMgr::UnRegisterSock( SOCKET hsock )
{
	MAPSOCKMBCOBJS::iterator itf = m_mapSockToObj.find(hsock);
	if (itf == m_mapSockToObj.end())
	{
		return E_INVALIDARG;
	}

	MAPMBCOBJSOCKS::iterator itf2 = m_mapObjToSocks.find(itf->second);
	if (itf2 != m_mapObjToSocks.end())
	{
		VECSOCKS::iterator itfsoc = std::find(itf2->second.begin(), itf2->second.end(), hsock);
		if (itfsoc != itf2->second.end())
		{
			itf2->second.erase(itfsoc);
		}
	}

	m_mapSockToObj.erase(itf);

	return S_OK;
}

HRESULT CMBCSockMgr::UnRegisterSock( CMBCBaseObj* pBaseObj )
{
	return S_OK;
}
