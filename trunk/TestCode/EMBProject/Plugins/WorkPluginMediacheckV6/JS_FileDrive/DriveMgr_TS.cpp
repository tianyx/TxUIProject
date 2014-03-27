// DriveMgr_TS.cpp: implementation of the CDriveMgr_TS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_TS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_TS::CDriveMgr_TS()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

	OutputDebugString("\nWWW: CDriveMgr_TS::CDriveMgr_TS()\n");
}
CDriveMgr_TS::CDriveMgr_TS(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("\nInto CDriveMgr_TS(HWND ParentHwnd)\n");
}
CDriveMgr_TS::~CDriveMgr_TS()
{
    OutputDebugString("\nWWW: CDriveMgr_TS::~CDriveMgr_TS()\n");
}
