// MBCWinApp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MBCRemoteCtrl.h"
#include "MBCWinApp.h"
#include "FGlobal.h"


// CMBCWinApp

IMPLEMENT_DYNCREATE(CMBCWinApp, CWinApp)

CMBCWinApp::CMBCWinApp()
{
}

CMBCWinApp::~CMBCWinApp()
{
}

BOOL CMBCWinApp::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
#ifdef _DEBUG
	g_hconsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (g_hconsoleHandle == NULL)
	{
		AllocConsole();
		g_hconsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	}
#endif // _DEBUG
	return TRUE;
}

int CMBCWinApp::ExitInstance()
{
	CMBCRemoteCtrlMgr::Release();

#ifdef _DEBUG
	g_hconsoleHandle = NULL;
	//FreeConsole();
#endif // _DEBUG
	// TODO: �ڴ�ִ���������߳�����
	return CWinApp::ExitInstance();
}



BEGIN_MESSAGE_MAP(CMBCWinApp, CWinApp)
END_MESSAGE_MAP()


// CMBCWinApp ��Ϣ�������
