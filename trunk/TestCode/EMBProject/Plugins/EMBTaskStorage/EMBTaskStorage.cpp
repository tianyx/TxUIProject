// EMBTaskStorage.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "EMBPluginBase.h"
#include "storagemgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
extern EMB::CStorageMgr* g_pPluginInstane = NULL;


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
		g_pPluginInstane = new EMB::CStorageMgr;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBase, pInterface);
	return 0;
}
