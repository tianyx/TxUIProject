// WorkPluginMergeDBWriter.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "MergeDBWriter.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CMergeDBWriter* g_pPluginInstane = NULL;

extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CMergeDBWriter;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}