
// TS2ASISvr.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TS2ASISvr.h"
#include "TS2ASISvrDlg.h"
#include "FGlobal.h"
#include "GlobalDef.h"
#include "io.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CTS2ASISvrApp

BEGIN_MESSAGE_MAP(CTS2ASISvrApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTS2ASISvrApp ����

CTS2ASISvrApp::CTS2ASISvrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTS2ASISvrApp ����

CTS2ASISvrApp theApp;


// CTS2ASISvrApp ��ʼ��

BOOL CTS2ASISvrApp::InitInstance()
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
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));
	GetPrivateProfileStringA("Config","tsips","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	std::vector<string> vIps;
	std::vector<string> vPorts;
	SplitteStrings(szBuff,vIps);
	GetPrivateProfileStringA("Config","tsports","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vPorts);

	if (vPorts.size() != vIps.size())
	{
		ASSERT(FALSE);
		AfxMessageBox(TEXT("TS ��ַ���ô���"));
		return FALSE;
	}

	for (size_t i = 0; i < vPorts.size(); ++i)
	{
		CString szkey = vIps[i].c_str();
		szkey.TrimLeft();
		szkey.TrimRight();
		int nPort =atoi(vPorts[i].c_str());
		ST_TSADDR tmpAddr;
		tmpAddr.ip = szkey;
		tmpAddr.nPort = nPort;
		if (!szkey.IsEmpty())
		{
			g_GlobalInfo.vtsAddr.push_back(tmpAddr);
		}
	}

	CTS2ASISvrDlg dlg;
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
