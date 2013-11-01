// EMBTaskDispatch.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "TaskDispatchMgr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace EMB;
extern EMB::CTaskDispatchMgr* g_pPluginInstane = NULL;

extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new CTaskDispatchMgr;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}

