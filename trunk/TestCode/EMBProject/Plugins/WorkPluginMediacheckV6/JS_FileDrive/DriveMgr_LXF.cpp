// DriveMgr_LXF.cpp: implementation of the CDriveMgr_LXF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_LXF.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_LXF::CDriveMgr_LXF()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("WWW: CDriveMgr_LXF::CDriveMgr_LXF()");
}

CDriveMgr_LXF::~CDriveMgr_LXF()
{
	OutputDebugString("WWW: CDriveMgr_LXF::~CDriveMgr_LXF()");
}

CDriveMgr_LXF::CDriveMgr_LXF(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
    OutputDebugString("<<<into CDriveMgr_LXF(HWND ParentHwnd)\n");
}
