// DriveMgr_MPG.cpp: implementation of the CDriveMgr_MPG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_MPG.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_MPG::CDriveMgr_MPG()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

}
CDriveMgr_MPG::CDriveMgr_MPG(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{

}
CDriveMgr_MPG::~CDriveMgr_MPG()
{

}
