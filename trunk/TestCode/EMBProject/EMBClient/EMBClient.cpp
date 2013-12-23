
// EMBClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EMBClient.h"
#include "EMBClientDlg.h"
#include "FGlobal.h"
#include "MainDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEMBClientApp

BEGIN_MESSAGE_MAP(CEMBClientApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEMBClientApp ����

CEMBClientApp::CEMBClientApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEMBClientApp ����

CEMBClientApp theApp;


// CEMBClientApp ��ʼ��

BOOL CEMBClientApp::InitInstance()
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

	AfxEnableControlContainer();
	
	ST_UICLIENTCONFIG config;
	CString strRet;
	config.ToString(strRet);
	if (!InitGlobalConfig())
	{
		ASSERT(FALSE);
		return FALSE;
	}


	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	if (wsaData.wVersion != MAKEWORD(2,2))
	{
		int nErr = WSAGetLastError();
		ASSERT(FALSE);
		return FALSE;
	}
	MACRO_CREATEOUTPUTCONSOLE
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CEMBClientDlg dlg;
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

int CEMBClientApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	WSACleanup();
	MACRO_FREEOUTPUTCONSOLE
	return CWinAppEx::ExitInstance();
}
