// MBCWinApp.cpp : 实现文件
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
	// TODO: 在此执行任意逐线程初始化
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
	// TODO: 在此执行任意逐线程清理
	return CWinApp::ExitInstance();
}



BEGIN_MESSAGE_MAP(CMBCWinApp, CWinApp)
END_MESSAGE_MAP()


// CMBCWinApp 消息处理程序
