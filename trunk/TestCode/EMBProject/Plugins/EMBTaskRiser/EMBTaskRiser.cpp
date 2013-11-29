// EMBTaskRiser.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "TaskRiserMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CTaskRiserMgr* g_pPluginInstane = NULL;  //ȫ�ֱ�������һ���Ƿ�淶��

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
		g_pPluginInstane = new EMB::CTaskRiserMgr;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}
