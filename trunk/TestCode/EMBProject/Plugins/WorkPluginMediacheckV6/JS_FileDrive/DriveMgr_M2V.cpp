// DriveMgr_M2V.cpp: implementation of the CDriveMgr_M2V class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_M2V.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_M2V::CDriveMgr_M2V()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("WWW: CDriveMgr_M2V::CDriveMgr_M2V()");

}

CDriveMgr_M2V::~CDriveMgr_M2V()
{
	OutputDebugString("WWW: CDriveMgr_M2V::~CDriveMgr_M2V()");

}

CDriveMgr_M2V::CDriveMgr_M2V(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("<<<into CDriveMgr_M2V(HWND ParentHwnd)\n");

}
