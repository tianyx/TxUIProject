
// MultiBroadcastTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MultiBroadcastTest.h"
#include "MultiBroadcastTestDlg.h"
#include "NetCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiBroadcastTestSvrApp

BEGIN_MESSAGE_MAP(CMultiBroadcastTestSvrApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMultiBroadcastTestSvrApp ����

CMultiBroadcastTestSvrApp::CMultiBroadcastTestSvrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMultiBroadcastTestSvrApp ����

CMultiBroadcastTestSvrApp theApp;


// CMultiBroadcastTestSvrApp ��ʼ��

BOOL CMultiBroadcastTestSvrApp::InitInstance()
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

	if (CheckWinsockVersion(MAKEWORD(2,2)) != 0)
	{
		AfxMessageBox(TEXT("winsock ver 2 not found."));
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMultiBroadcastTestSvrDlg dlg;
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

int CMultiBroadcastTestSvrApp::ExitInstance()
{

	WSACleanup();
	// TODO: �ڴ����ר�ô����/����û���
	return CWinAppEx::ExitInstance();
}
