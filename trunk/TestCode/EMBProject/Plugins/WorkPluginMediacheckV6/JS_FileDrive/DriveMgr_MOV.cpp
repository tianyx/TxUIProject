// DriveMgr_MOV.cpp: implementation of the CDriveMgr_MXF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_MOV.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CDriveMgr_MOV::CDriveMgr_MOV()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("\nWWW: CDriveMgr_MOV::CDriveMgr_MOV()\n");
}

CDriveMgr_MOV::CDriveMgr_MOV(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
   OutputDebugString("\nInto CDriveMgr_MOV(HWND ParentHwnd)\n");
}
CDriveMgr_MOV::~CDriveMgr_MOV()
{
    OutputDebugString("\nWWW: CDriveMgr_MOV::~CDriveMgr_MOV()\n");
}
