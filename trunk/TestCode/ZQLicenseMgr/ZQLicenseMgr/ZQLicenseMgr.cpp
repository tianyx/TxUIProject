
// ZQLicenseMgr.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ZQLicenseMgr.h"
#include "ZQLicenseMgrDlg.h"
#include "ZQGlobal.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CZQLicenseMgrApp

BEGIN_MESSAGE_MAP(CZQLicenseMgrApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CZQLicenseMgrApp ����

CZQLicenseMgrApp::CZQLicenseMgrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CZQLicenseMgrApp ����

CZQLicenseMgrApp theApp;


// CZQLicenseMgrApp ��ʼ��

BOOL CZQLicenseMgrApp::InitInstance()
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

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	g_GlobalInfo.szAppPath = GetAppPath();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\config.ini");
	if (_access(g_GlobalInfo.szIniPath.c_str(), 04) == -1)
	{
		AfxMessageBox("�Ҳ��������ļ�config.ini��");
		return FALSE;
	}
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));
	GetPrivateProfileStringA("Config","modules","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff, g_GlobalInfo.vModuleName);
	for (size_t i = 0; i < g_GlobalInfo.vModuleName.size(); ++i)
	{
		ZeroMemory(szBuff, sizeof(szBuff));
		GetPrivateProfileStringA("Config",g_GlobalInfo.vModuleName[i].c_str(),"", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
		CString szkey = szBuff;
		szkey.TrimLeft();
		szkey.TrimRight();
		if (!szkey.IsEmpty())
		{
			g_GlobalInfo.vModuleKey.push_back(std::string(szkey));
		}
	}

	if (g_GlobalInfo.vModuleName.size() == 0 ||g_GlobalInfo.vModuleKey.size() != g_GlobalInfo.vModuleName.size())
	{
		AfxMessageBox("�����ļ�����");
		return FALSE;
	}

	CZQLicenseMgrDlg dlg;
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
