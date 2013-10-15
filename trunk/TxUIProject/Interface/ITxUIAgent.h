/********************************************************************
	created:	2012/10/23
	created:	23:10:2012   14:05
	filename: 	g:\backup\MySrc\GoogleProj\txuiproject\trunk\TxUIProject\Interface\ITxUIAgent.h
	file path:	g:\backup\MySrc\GoogleProj\txuiproject\trunk\TxUIProject\Interface
	file base:	ITxUIAgent
	file ext:	h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "ITxDrawBase.h"
#include "ITxResourceMgr.h"
using namespace namespace_TxDrawBase;
typedef LRESULT (CALLBACK *TXCALLBACKDRAWFN)(ITxDrawBase* pIDraw);

#pragma once

//window to implement
interface ITxD3DToDraw
{
	virtual HRESULT Tx3DDraw(ITxDrawBase* pIDraw) = 0;
};

interface ITxUIAgent
{
	virtual void Init(HWND hwnd, ITxD3DToDraw* pRender) =0;
	virtual ITxResourceMgr* GetResourceMgr() =0;
	virtual void Release() =0;
};

TXDRAW_EXPORTS_API ITxUIAgent* CreateTxAgent();