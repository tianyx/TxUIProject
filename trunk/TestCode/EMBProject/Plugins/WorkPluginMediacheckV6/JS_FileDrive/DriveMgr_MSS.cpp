// DriveMgr_PNCL.cpp: implementation of the CDriveMgr_PNCL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_MSS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_MSS::CDriveMgr_MSS()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

}
CDriveMgr_MSS::CDriveMgr_MSS(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	
}

CDriveMgr_MSS::~CDriveMgr_MSS()
{

}
