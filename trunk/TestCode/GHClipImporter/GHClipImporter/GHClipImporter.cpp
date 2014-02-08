
// GHClipImporter.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "GHClipImporterDlg.h"
#include "MainDef.h"
#include "TxLogManager.h"
#include "FGlobal.h"
#include "TxADOConn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGHClipImporterApp

BEGIN_MESSAGE_MAP(CGHClipImporterApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGHClipImporterApp 构造

CGHClipImporterApp::CGHClipImporterApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CGHClipImporterApp 对象

CGHClipImporterApp theApp;


// CGHClipImporterApp 初始化

BOOL CGHClipImporterApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	if (IsSameDirStarted())
	{
		AfxMessageBox(TEXT("同一目录下已经启动了一个程序！"));
		return FALSE;
	}

	CTxLogManager::GetTxLogMgr()->AddNewLogFile(LOGKEYGHCI, TEXT("CHClipImporter"));
	

	if (!InitGlobalConfig())
	{
		AfxMessageBox(TEXT("配置文件不正确,请检查日志"));
		return FALSE;
	}

#ifndef FORMONITOR
	if (!CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEYGHCI, g_GlobalInfo.szDBConnString))
	{
		AfxMessageBox(TEXT("数据库打开失败"));
		return FALSE;
	}
	if (!RefreshClipType())
	{
		CFWriteLog(0, TEXT("媒体类型获取失败！请检查数据库"));
		AfxMessageBox(TEXT("媒体类型获取失败！请检查数据库"));
		return FALSE;
	}

	CTxADODBMgr::GetADODBMgr()->CloseDB(DBKEYGHCI);
#endif

	CGHClipImporterDlg dlg;
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

int CGHClipImporterApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	CTxADODBMgr::ReleaseDBMgr();
	CTxLogManager::ReleaseTxLogMgr();
	return CWinAppEx::ExitInstance();
}
