
// EMBServer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "EMBServer.h"
#include "EMBServerDlg.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "GdiPlusNewHeader.h"
#include "TxImageLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace Gdiplus;

// CEMBServerApp
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

BEGIN_MESSAGE_MAP(CEMBServerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEMBServerApp 构造

CEMBServerApp::CEMBServerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CEMBServerApp 对象

CEMBServerApp theApp;


// CEMBServerApp 初始化

BOOL CEMBServerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	SetErrorMode(SEM_NOGPFAULTERRORBOX);
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	HANDLE hmutex;
	if (IsAppRunning(hmutex))
	{
		AfxMessageBox(TEXT("同一目录下程序已经在运行中."));
		return FALSE;
	}

	MACRO_CREATEOUTPUTCONSOLE
#ifndef _DEBUG
		MACRO_SHOWCONSOLEWINDOW(FALSE)
#endif // _DEBUG
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	// 添加LOG日志
	GetTxLogMgr()->AddNewLogFile(LOGKEYMAIN, "EMBServer.log");

	// 初始化配置文件
	if (!InitGlobalConfig())
	{
		return FALSE;
	}

	// 加载任务管理插件
	if (!LoadPluginManager())
	{
		return FALSE;
	}

	// 启动服务
	if (!InitServer())
	{
		UnInitServer();
		return FALSE;
	}
	
	CEMBServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。

	return FALSE;
}

int CEMBServerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	UnInitServer();
 	UnLoadPluginManager();
	ReleaseTxLogMgr();
	CTxImageLoader::Release();
	GdiplusShutdown(gdiplusToken);
	MACRO_FREEOUTPUTCONSOLE
	return CWinAppEx::ExitInstance();
}
