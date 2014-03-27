
// EMBServer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EMBServer.h"
#include "EMBServerDlg.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "GdiPlusNewHeader.h"
#include "TxImageLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

// CEMBServerApp
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

BEGIN_MESSAGE_MAP(CEMBServerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEMBServerApp ����

CEMBServerApp::CEMBServerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEMBServerApp ����

CEMBServerApp theApp;


// CEMBServerApp ��ʼ��

BOOL CEMBServerApp::InitInstance()
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
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	MACRO_CREATEOUTPUTCONSOLE
#ifndef _DEBUG
		MACRO_SHOWCONSOLEWINDOW(FALSE)
#endif // _DEBUG
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	// ���LOG��־
	GetTxLogMgr()->AddNewLogFile(LOGKEYMAIN, "EMBServer.log");

	// ��ʼ�������ļ�
	if (!InitGlobalConfig())
	{
		return FALSE;
	}

	// �������������
	if (!LoadPluginManager())
	{
		return FALSE;
	}

	// ��������
	if (!InitServer())
	{
		UnInitServer();
		return FALSE;
	}
	
	CEMBServerDlg dlg;
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

int CEMBServerApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	UnInitServer();
 	UnLoadPluginManager();
	ReleaseTxLogMgr();
	CTxImageLoader::Release();
	GdiplusShutdown(gdiplusToken);
	MACRO_FREEOUTPUTCONSOLE
	return CWinAppEx::ExitInstance();
}
