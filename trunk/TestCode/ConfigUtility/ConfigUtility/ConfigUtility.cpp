
// ConfigUtility.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ConfigUtility.h"
#include "ConfigUtilityDlg.h"
#include "FGlobal.h"
#include "StrConvert.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConfigUtilityApp

BEGIN_MESSAGE_MAP(CConfigUtilityApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CConfigUtilityApp ����

CConfigUtilityApp::CConfigUtilityApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CConfigUtilityApp ����

CConfigUtilityApp theApp;


// CConfigUtilityApp ��ʼ��

BOOL CConfigUtilityApp::InitInstance()
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
	AfxOleInit();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("tianyx"));
	
	g_GlobalInfo.wsAppPath = GetAppPath();
	g_GlobalInfo.wsIniPath = g_GlobalInfo.wsAppPath;
	g_GlobalInfo.wsIniPath += TEXT("\\config.ini");
	wchar_t szBuff[MAX_PATH];
	GetPrivateProfileString(TEXT("Setting"), TEXT("CfgSrcPath"), TEXT(""), szBuff, MAX_PATH,  g_GlobalInfo.wsIniPath.c_str());
	g_GlobalInfo.wsCfgSrcPath = szBuff;
	RTrim(g_GlobalInfo.wsCfgSrcPath, TEXT("\\"));
	if (g_GlobalInfo.wsCfgSrcPath.size() == 0 || _waccess(g_GlobalInfo.wsCfgSrcPath.c_str(), 00) == -1)
	{
		g_GlobalInfo.wsCfgSrcPath = g_GlobalInfo.wsAppPath;
	}
	CConfigUtilityDlg dlg;
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
