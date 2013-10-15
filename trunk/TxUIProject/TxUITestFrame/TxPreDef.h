/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   10:43
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxPreDef.h
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxPreDef
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include <map>
#include <vector>
#include <list>

#define FLOATISEZERO(x) (x< 0.00000001 && x > -0.00000001)
//////////////////////////////////////////////////////////////////////////
#define TS_VISABLE		0x00000001
#define TS_ENABLED		0x00000002
#define TS_UNCHECKED	0x00000004
#define TS_ACTIVED		0x00000008
#define TS_MOUSEOVER	0x00000010
#define TS_MOUSEPRESSED 0x00000020
#define TS_SELECTED		0x00000040
#define TS_SETCAPTURED	0x00000100
#define	TS_INIT_STATE	(TS_VISABLE|TS_ENABLED|TS_UNCHECKED)
//////////////////////////////////////////////////////////////////////////

#define TXMACRO_GETCONTAINER(pContainer) \
	CWnd* pWndCont = CWnd::FromHandle(m_hWnd); \
	if (pWndCont && pWndCont->IsKindOf(RUNTIME_CLASS(CTxWndContainer))) \
		pContainer = (CTxWndContainer*)pWndCont; \

//////////////////////////////////////////////////////////////////////////

typedef  int IDTXOBJ;
class CTxObjectBase;
typedef std::map<IDTXOBJ,  CTxObjectBase *> MAPTXOBJ;
typedef std::vector<IDTXOBJ> VTXOBJIDS;
typedef std::list<IDTXOBJ> LSTXOBJIDS;
typedef std::list<IDTXOBJ> LSTXOBJIDS;