// WorkPluginMediaCheck.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "MediaCheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CMediaCheck* g_pPluginInstane = NULL;

extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CMediaCheck;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}

