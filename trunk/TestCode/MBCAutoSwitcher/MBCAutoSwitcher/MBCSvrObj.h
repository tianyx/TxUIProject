/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCSvrObj.h
	file base:	MBCSvrObj
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include "MBCHeartBeatObj.h"
class CMBCSvrObj :public CMBCHeartBeatObj
{
public:
	CMBCSvrObj();
	virtual ~CMBCSvrObj(void);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock,int nMsgType, char* bufferIn, int nUsed);
};
