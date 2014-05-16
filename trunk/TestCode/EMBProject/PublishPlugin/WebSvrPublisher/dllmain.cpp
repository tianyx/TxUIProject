// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "FGlobal.h"
#include "TxLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE WebSvrPublisherDLL = { NULL, NULL };

extern HMODULE g_hGlobalDllModule = NULL;
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WorkPluginTransFile.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(WebSvrPublisherDLL, hInstance))
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

		new CDynLinkLibrary(WebSvrPublisherDLL);

		// �׽��ֳ�ʼ��
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �������д� DllMain ���Ƴ��������Ƿ�����һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL��
// 		if (!AfxSocketInit())
// 		{
// 			return FALSE;
// 		}
		GetTxLogMgr()->AddNewLogFile(1, TEXT("WebSvrPublisher"));

		g_hGlobalDllModule = hInstance;

		MACRO_CREATEOUTPUTCONSOLE

	
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//locked why!!!?
		GetTxLogMgr()->Stop(TRUE);
		ReleaseTxLogMgr();
		TRACE0("WebSvrPublisher.DLL ������ֹ!\n");
		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(WebSvrPublisherDLL);
		
		g_hGlobalDllModule = NULL;
	}
	return 1;   // ȷ��
}
