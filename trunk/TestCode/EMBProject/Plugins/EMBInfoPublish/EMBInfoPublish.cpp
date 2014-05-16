// EMBTaskRiser.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "EMBInfoPublishMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CEMBInfoPublishMgr* g_pPluginInstane = NULL;  //ȫ�ֱ�������һ���Ƿ�淶��

/*
* Description���ⲿ��ȡ�������ָ��
* Input Param��
*		pInterface�����ղ��ʵ��ָ��
* Return Param���ɹ�����0
* History��
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
