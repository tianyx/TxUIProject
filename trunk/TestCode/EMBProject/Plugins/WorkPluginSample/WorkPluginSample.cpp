// WorkPluginSample.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "worksample.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CWorkSample* g_pPluginInstane = NULL;

extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CWorkSample;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}
