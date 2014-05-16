
// EMBExternalExcutor.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EMBExternalExcutor.h"
#include "EMBExternalExcutorDlg.h"
#include "MainDef.h"
#include "LogKeyDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEMBExternalExcutorApp

BEGIN_MESSAGE_MAP(CEMBExternalExcutorApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEMBExternalExcutorApp ����

CEMBExternalExcutorApp::CEMBExternalExcutorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEMBExternalExcutorApp ����

CEMBExternalExcutorApp theApp;


// CEMBExternalExcutorApp ��ʼ��

BOOL CEMBExternalExcutorApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
#ifndef _DEBUG
	SetErrorMode(SEM_NOGPFAULTERRORBOX);
#endif

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
	if (!InitGlobalConfig())
	{
		
		return FALSE;
	}

	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKEXCUTOR, TEXT("excutor"));

	CEMBExternalExcutorDlg dlg;
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

int CEMBExternalExcutorApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	ReleaseTxLogMgr();
	MACRO_FREEOUTPUTCONSOLE
	return CWinAppEx::ExitInstance();
}
