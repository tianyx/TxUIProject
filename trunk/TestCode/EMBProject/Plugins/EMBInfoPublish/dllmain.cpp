/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	dllmain.cpp
	author:		tianyx
	
	purpose:	���� DLL �ĳ�ʼ�����̡�
*********************************************************************/

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "FGlobal.h"
#include "EMBInfoPublishMgr.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HMODULE g_hGlobalDllModule = NULL; 

//EMBInfoPublish DLL ģ��״̬��¼����
static AFX_EXTENSION_MODULE EMBInfoPublishDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("EMBInfoPublish.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(EMBInfoPublishDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(EMBInfoPublishDLL);
		MACRO_CREATEOUTPUTCONSOLE
		
		g_hGlobalDllModule = hInstance;

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("EMBInfoPublish.DLL ������ֹ!\n");
		g_hGlobalDllModule = NULL;
		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(EMBInfoPublishDLL);
	}
	return 1;   // ȷ��
}
