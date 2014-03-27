// DriveMgr_AVI.cpp: implementation of the CDriveMgr_AVI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_AVI.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_AVI::CDriveMgr_AVI()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

}

CDriveMgr_AVI::CDriveMgr_AVI(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	
}

CDriveMgr_AVI::~CDriveMgr_AVI()
{

}
