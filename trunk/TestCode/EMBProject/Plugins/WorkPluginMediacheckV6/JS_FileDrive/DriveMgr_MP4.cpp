// DriveMgr_MP4.cpp: implementation of the CDriveMgr_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_MP4.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDriveMgr_MP4::CDriveMgr_MP4()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{
	
	OutputDebugString("\nWWW: CDriveMgr_MP4::CDriveMgr_MP4()\n");
}

CDriveMgr_MP4::CDriveMgr_MP4(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("\nInto CDriveMgr_MP4(HWND ParentHwnd)\n");
}
CDriveMgr_MP4::~CDriveMgr_MP4()
{
    OutputDebugString("\nWWW: CDriveMgr_MP4::~CDriveMgr_MP4()\n");
}
