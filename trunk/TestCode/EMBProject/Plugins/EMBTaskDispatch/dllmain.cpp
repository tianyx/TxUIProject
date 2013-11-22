// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE EMBTaskDispatchDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("EMBTaskDispatch.DLL ���ڳ�ʼ��!\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(EMBTaskDispatchDLL, hInstance))
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

		new CDynLinkLibrary(EMBTaskDispatchDLL);
		MACRO_CREATEOUTPUTCONSOLE
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);
		if (wsaData.wVersion != MAKEWORD(2,2))
		{
			int nErr = WSAGetLastError();
			ASSERT(FALSE);
			AfxMessageBox(TEXT("ϵͳ��֧��winsock2"));
			return FALSE;
		}

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("EMBTaskDispatch.DLL ������ֹ!\n");
		WSACleanup();
		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(EMBTaskDispatchDLL);
	}
	return 1;   // ȷ��
}
