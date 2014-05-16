// test

// EmbTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EmbTest.h"
#include "EmbTestDlg.h"
#include "TxLogManager.h"
#include "FGlobal.h"
#include "DlgTestTask.h"
#include "MainDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEmbTestApp

BEGIN_MESSAGE_MAP(CEmbTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEmbTestApp ����

CEmbTestApp::CEmbTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ��aasdfasfadsfafa
}


// Ψһ��һ�� CEmbTestApp ����

CEmbTestApp theApp;


// CEmbTestApp ��ʼ��

BOOL CEmbTestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();
	MACRO_CREATEOUTPUTCONSOLE
	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	AllocConsole();
	g_hconsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetTxLogMgr()->AddNewLogFile(1, TEXT("tesetlog"));

	InitGlobalConfig();
	CDlgTestTask dlg;
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

int CEmbTestApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	ReleaseTxLogMgr();
	return CWinAppEx::ExitInstance();
}
