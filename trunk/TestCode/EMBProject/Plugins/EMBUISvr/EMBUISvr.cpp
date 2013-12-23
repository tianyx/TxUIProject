// EMBUISvr.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"

#include "EMBPluginBase.h"
#include "uiserver.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma 
extern EMB::CUIServer* g_pPluginInstane = NULL;

extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CUIServer;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}