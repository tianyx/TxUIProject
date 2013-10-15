
// AotoFill.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "AotoFill.h"
#include "AotoFillDlg.h"
#include "ZQGlobal.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAotoFillApp

BEGIN_MESSAGE_MAP(CAotoFillApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAotoFillApp ����

CAotoFillApp::CAotoFillApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAotoFillApp ����

CAotoFillApp theApp;


// CAotoFillApp ��ʼ��

BOOL CAotoFillApp::InitInstance()
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
	AfxOleInit();
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
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));
	GetPrivateProfileStringA("Config","addressList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff, g_GlobalInfo.vAddress);
	if (g_GlobalInfo.vAddress.size() == 0)
	{
		AfxMessageBox("�����ļ����󣬵�ַ�б�δ�ҵ�");
		return FALSE;
	}

	g_GlobalInfo.nMinHp = GetPrivateProfileIntA(TEXT("Config"), TEXT("MinHp"),110,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nMaxHp = GetPrivateProfileIntA(TEXT("Config"), TEXT("MaxHp"),150,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nMinMp = GetPrivateProfileIntA(TEXT("Config"), TEXT("MinMp"),60,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nMaxMp = GetPrivateProfileIntA(TEXT("Config"), TEXT("MaxMp"),94,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nDetaHp = g_GlobalInfo.nMaxHp - g_GlobalInfo.nMinHp;
	g_GlobalInfo.nDetaMp = g_GlobalInfo.nMaxMp - g_GlobalInfo.nMinMp;

	g_GlobalInfo.nAgeMin = GetPrivateProfileIntA(TEXT("Config"), TEXT("AgeMin"),35,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nAgeMax= GetPrivateProfileIntA(TEXT("Config"), TEXT("AgeMax"),150,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nKeyColAddr = GetPrivateProfileIntA(TEXT("Config"), TEXT("KeyColAddr"),0,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nKeyColAge = GetPrivateProfileIntA(TEXT("Config"), TEXT("KeyColAge"),1,  g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nKeyColFill = GetPrivateProfileIntA(TEXT("Config"), TEXT("KeyColFill"),2,  g_GlobalInfo.szIniPath.c_str());


	CAotoFillDlg dlg;
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
