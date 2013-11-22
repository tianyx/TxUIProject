// WorkPluginMD5.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"

#include "EMBPluginBase.h"
#include "WorkMD5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CWorkMD5* g_pPluginInstane = NULL;

extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CWorkMD5;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}
