/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCClientObj.h
	file base:	MBCClientObj
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include "MBCHeartBeatObj.h"
class CMBCClientObj :
	public CMBCHeartBeatObj
{
public:
	CMBCClientObj(void);
	virtual ~CMBCClientObj(void);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

};
