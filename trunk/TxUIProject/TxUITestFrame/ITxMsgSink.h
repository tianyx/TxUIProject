/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   16:50
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\ITxMsgSink.h
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	ITxMsgSink
	file ext:	h
	author:		tian_yx
	
	purpose:	process non window common msg
*********************************************************************/

#pragma once
#include "TxPreDef.h"

typedef int (*TXMSGCALLBACK)(UINT nMsg, WPARAM wparam, LPARAM lparam);

interface ITxMsgSink
{
	virtual int RegisterMsg(const UINT nMsg,  const IDTXOBJ nObjId) = 0;
	virtual int UnRegisterMsg(const UINT nMsg, const IDTXOBJ nObjId) = 0;
};