#include "StdAfx.h"
#include "TxGlobal.h"
#include "TxDxAgent.h"
#include "TxWndDrawClockMgr.h"
#include "TxDxAgentMgr.h"

 TXDRAW_EXPORTS_API BOOL TxGlobalInit()
{
	return TRUE;
}

 TXDRAW_EXPORTS_API BOOL TxGlobalUnInit()
{
	CTxWndDrawClockMgr::Release();
	CTxDxAgentMgr::Release();
	return TRUE;
}
