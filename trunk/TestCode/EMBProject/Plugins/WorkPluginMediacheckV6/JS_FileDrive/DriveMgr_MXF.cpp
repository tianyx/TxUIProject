// DriveMgr_MXF.cpp: implementation of the CDriveMgr_MXF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DriveMgr_MXF.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int LoadDict(const char* DictPath)
{
	using namespace mxflib;
	return LoadDictionary(DictPath);
}

CDriveMgr_MXF::CDriveMgr_MXF()
:CDriveMgr(NULL,&m_DecoderGraph,&m_InfoExtract)
{

	OutputDebugString("\nWWW: CDriveMgr_MXF::CDriveMgr_MXF()\n");
}
CDriveMgr_MXF::CDriveMgr_MXF(HWND ParentHwnd)
:CDriveMgr(ParentHwnd,&m_DecoderGraph,&m_InfoExtract)
{
   OutputDebugString("\nInto CDriveMgr_MXF(HWND ParentHwnd)\n");
}
CDriveMgr_MXF::~CDriveMgr_MXF()
{
    OutputDebugString("\nWWW: CDriveMgr_MXF::~CDriveMgr_MXF()\n");
}
