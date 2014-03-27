// DriveMgr_WMV.cpp: implementation of the CDriveMgr_WMV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_WMV.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDriveMgr_WMV::CDriveMgr_WMV()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

	OutputDebugString("WWW: CDriveMgr_WMV::CDriveMgr_WMV()");
}
CDriveMgr_WMV::CDriveMgr_WMV(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	
}
CDriveMgr_WMV::~CDriveMgr_WMV()
{
    OutputDebugString("WWW: CDriveMgr_WMV::~CDriveMgr_WMV()");
}
