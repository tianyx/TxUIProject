
// MonitorTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MonitorTest.h"
#include "MonitorTestDlg.h"
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonitorTestApp

BEGIN_MESSAGE_MAP(CMonitorTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMonitorTestApp ����

CMonitorTestApp::CMonitorTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMonitorTestApp ����

CMonitorTestApp theApp;


// CMonitorTestApp ��ʼ��

BOOL CMonitorTestApp::InitInstance()
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
	WSADATA wsadata;
	int nRet = WSAStartup(MAKEWORD(2,2), &wsadata);
	ASSERT(nRet == 0);
	CWinAppEx::InitInstance();

	AfxEnableControlContainer();
#ifdef _DEBUG
MACRO_CREATEOUTPUTCONSOLE
#endif // _DEBUG
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMonitorTestDlg dlg;
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

	WSACleanup();
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CMonitorTestApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
#ifdef _DEBUG
	MACRO_FREEOUTPUTCONSOLE
#endif // _DEBUG
	return CWinAppEx::ExitInstance();
}
