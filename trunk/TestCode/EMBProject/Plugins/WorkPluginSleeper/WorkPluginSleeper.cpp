// WorkPluginSleeper.cpp : ���� DLL �ĳ�ʼ�����̡�
//
#include "stdafx.h"
#include "EMBPluginBase.h"
#include "worksleeper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CWorkSleeper* g_pPluginInstane = NULL;
//��Ϊִ�в��������ʵ�ִ˽ӿ�
extern "C" int EMBPLUGIN_API GetPluginInstance( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CWorkSleeper;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}
