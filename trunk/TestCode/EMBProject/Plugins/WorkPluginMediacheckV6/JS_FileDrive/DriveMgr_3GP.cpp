// DriveMgr_3GP.cpp: implementation of the CDriveMgr_3GP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_3GP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_3GP::CDriveMgr_3GP()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

	OutputDebugString("\nWWW: CDriveMgr_3GP::CDriveMgr_3GP()\n");
}
CDriveMgr_3GP::CDriveMgr_3GP(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	OutputDebugString("\nInto CDriveMgr_3GP(HWND ParentHwnd)\n");
}
CDriveMgr_3GP::~CDriveMgr_3GP()
{
    OutputDebugString("\nWWW: CDriveMgr_3GP::~CDriveMgr_3GP()\n");
}