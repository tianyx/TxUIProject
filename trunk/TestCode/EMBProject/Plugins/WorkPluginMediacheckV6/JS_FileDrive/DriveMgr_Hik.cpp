// DriveMgr_Hik.cpp: implementation of the CDriveMgr_Hik class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_Hik.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDriveMgr_Hik::CDriveMgr_Hik()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{
	
}

CDriveMgr_Hik::CDriveMgr_Hik(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
	
}

CDriveMgr_Hik::~CDriveMgr_Hik()
{

}
