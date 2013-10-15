
// MulticastEndTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MulticastEndTest.h"
#include "MulticastEndTestDlg.h"
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMulticastEndTestApp

BEGIN_MESSAGE_MAP(CMulticastEndTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMulticastEndTestApp ����

CMulticastEndTestApp::CMulticastEndTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMulticastEndTestApp ����

CMulticastEndTestApp theApp;


// CMulticastEndTestApp ��ʼ��

BOOL CMulticastEndTestApp::InitInstance()
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
	WSADATA wsadata;
	int nRet = WSAStartup(MAKEWORD(2,2), &wsadata);
	ASSERT(nRet == 0);
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	MACRO_CREATEOUTPUTCONSOLE;
	CMulticastEndTestDlg dlg;
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

int CMulticastEndTestApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	MACRO_FREEOUTPUTCONSOLE;
	WSACleanup();
	return CWinAppEx::ExitInstance();
}
