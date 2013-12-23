/********************************************************************
	created:	2013/11/25
	created:	$2013:11:25
	filename: 	MBCRemoteObj.h
	file base:	MBCRemoteObj
	file ext:	h
	author:		tianyx
	
	purpose:	basic listen object, support one linstener, multi connections
*********************************************************************/
#pragma once
#include "mbcsocket.h"
#include "MBCTransMsg.h"
#include <map>
#include "MBCCommonDef.h"
#include "MBCBaseObj.h"


using namespace std;
typedef map<CMBCSocket*, CMBCSocket*> MAPSOCKINS;

class CMBCRemoteObj:public CMBCBaseObj
{
public:
	CMBCRemoteObj(void);
	~CMBCRemoteObj(void);

	//map for save incoming connection
	CAutoCritSec m_csSockIn;
	MAPSOCKINS m_mapSockIns;

	void RemoveSock(CMBCSocket* pSock);
	void AddSock(CMBCSocket* pSock);

	virtual HRESULT Run();
	virtual HRESULT Stop();

	virtual HRESULT NetCall_Connect(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Close(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);
	virtual HRESULT NetCall_Accept(CMBCSocket* pMBCSock, WPARAM wParam, LPARAM lParam);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

};

