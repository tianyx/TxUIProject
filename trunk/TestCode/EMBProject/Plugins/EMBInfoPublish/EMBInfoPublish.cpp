// EMBTaskRiser.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "EMBInfoPublishMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CEMBInfoPublishMgr* g_pPluginInstane = NULL;  //全局变量名称一样是否规范？

/*
* Description：外部获取插件连接指针
* Input Param：
*		pInterface：接收插件实例指针
* Return Param：成功返回0
* History：
*/
extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CEMBInfoPublishMgr;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}
