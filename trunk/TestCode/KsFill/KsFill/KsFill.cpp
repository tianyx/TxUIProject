
// KsFill.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "KsFill.h"
#include "KsFillDlg.h"
#include "GlobalDef.h"
#include "FGlobal.h"
#include "io.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKsFillApp

BEGIN_MESSAGE_MAP(CKsFillApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKsFillApp ����

CKsFillApp::CKsFillApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CKsFillApp ����

CKsFillApp theApp;


// CKsFillApp ��ʼ��

BOOL CKsFillApp::InitInstance()
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
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	g_GlobalInfo.szAppPath = GetAppPath();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\config.ini");
	if (_access(g_GlobalInfo.szIniPath.c_str(), 04) == -1)
	{
		AfxMessageBox("�Ҳ��������ļ�config.ini��");
		return FALSE;
	}
// 	char szBuff[512];
// 	ZeroMemory(szBuff, sizeof(szBuff));
// 	GetPrivateProfileStringA("Config","addressList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
// 	SplitteStrings(szBuff, g_GlobalInfo.vAddress);
// 	if (g_GlobalInfo.vAddress.size() == 0)
// 	{
// 		AfxMessageBox("�����ļ����󣬵�ַ�б�δ�ҵ�");
// 		return FALSE;
// 	}



	CKsFillDlg dlg;
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
