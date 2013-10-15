/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCSockMgr.h
	file base:	MBCSockMgr
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include <map>
#include "MBCBaseObj.h"

using namespace std;

typedef map<SOCKET, CMBCBaseObj*> MAPSOCKMBCOBJS;
typedef vector<SOCKET> VECSOCKS;
typedef map<CMBCBaseObj*, VECSOCKS> MAPMBCOBJSOCKS;

class CMBCSockMgr
{
public:
	CMBCSockMgr(void);
	~CMBCSockMgr(void);

public:
MAPSOCKMBCOBJS m_mapSockToObj;
MAPMBCOBJSOCKS m_mapObjToSocks;

	HRESULT RegisterSock(SOCKET hsock, CMBCBaseObj* pBaseObj);
	HRESULT UnRegisterSock(SOCKET hsock);
	HRESULT UnRegisterSock(CMBCBaseObj* pBaseObj);

};
