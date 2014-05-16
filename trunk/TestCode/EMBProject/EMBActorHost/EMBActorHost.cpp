
// EMBActorHost.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EMBActorHost.h"
#include "EMBActorHostDlg.h"
#include "MainDef.h"
#include "GdiPlusNewHeader.h"
#include "TxImageLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
// CEMBActorHostApp

BEGIN_MESSAGE_MAP(CEMBActorHostApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEMBActorHostApp ����

CEMBActorHostApp::CEMBActorHostApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEMBActorHostApp ����

CEMBActorHostApp theApp;


// CEMBActorHostApp ��ʼ��

BOOL CEMBActorHostApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	SetErrorMode(SEM_NOGPFAULTERRORBOX);
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
#ifndef _DEBUG
	HANDLE hmutex;
	if (IsAppRunning(hmutex, TRUE))
	{
		AfxMessageBox(TEXT("�����Ѿ���������."));
		return FALSE;
	}
#endif
	


	MACRO_CREATEOUTPUTCONSOLE
#ifndef _DEBUG
		MACRO_SHOWCONSOLEWINDOW(FALSE)
#endif // _DEBUG
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// ����EMBActor.dll
	if (!InitGlobalConfig())
	{
		return FALSE;
	}
	
	CEMBActorHostDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CEMBActorHostApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	UnloadActorHost();
	MACRO_FREEOUTPUTCONSOLE

	CTxImageLoader::Release();
	GdiplusShutdown(gdiplusToken);

	return CWinAppEx::ExitInstance();
}
