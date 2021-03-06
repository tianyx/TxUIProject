
// EmbTaskOwl.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "EmbTaskOwl.h"
#include "MainFrm.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "GlobalDef.h"
#include "TxFontLoader.h"
#include "TxADOConn.h"


Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken = NULL;

#pragma comment(lib, "gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMainFrame* g_pMainFrame = NULL;

// CEmbTaskOwlApp

BEGIN_MESSAGE_MAP(CEmbTaskOwlApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CEmbTaskOwlApp::OnAppAbout)
END_MESSAGE_MAP()


// CEmbTaskOwlApp 构造

CEmbTaskOwlApp::CEmbTaskOwlApp()
{

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CEmbTaskOwlApp 对象

CEmbTaskOwlApp theApp;


// CEmbTaskOwlApp 初始化

BOOL CEmbTaskOwlApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
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
	//SetRegistryKey(_T("embtaskowl"));
	HANDLE hmutex;
	if (IsAppRunning(hmutex, FALSE))
	{
		AfxMessageBox(TEXT("程序已经在运行中."));
		return FALSE;
	}

	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
	
	g_pFontLoader = new CTxFontLoader;

	MACRO_CREATEOUTPUTCONSOLE;
#ifndef _DEBUG
	MACRO_SHOWCONSOLEWINDOW(FALSE);
#endif
	GetTxLogMgr()->AddNewLogFile(1, TEXT("EmbTaskOwl"));
	if (!InitGlobalConfig())
	{
		return FALSE;
	}

	BOOL bDbOpen = CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_EMB, g_GlobalInfo.strEmbTaskDBConn);
	if (!bDbOpen)
	{
		AfxMessageBox("EMB数据库打开失败！");
		return FALSE;
	}

	 bDbOpen = CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_PLAYLIST, g_GlobalInfo.strPlayListCheckDBConn);
	if (!bDbOpen)
	{
		AfxMessageBox("数据库1打开失败！");
		return FALSE;
	}

	 bDbOpen = CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_PLAYLIST2, g_GlobalInfo.strPlayListCheckDBConn2);
	if (!bDbOpen)
	{
		AfxMessageBox("数据库2打开失败！");
		return FALSE;
	}

	if (!LoadSectionInfo())
	{
		AfxMessageBox(TEXT("读取频道信息失败！"));
		return FALSE;
	}

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	g_pMainFrame = pFrame;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}


// CEmbTaskOwlApp 消息处理程序




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CEmbTaskOwlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEmbTaskOwlApp 消息处理程序




int CEmbTaskOwlApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	ReleaseTxLogMgr();
	CTxADODBMgr::ReleaseDBMgr();
	MACRO_FREEOUTPUTCONSOLE;
	if (g_pFontLoader)
	{
		g_pFontLoader->Release();
		delete g_pFontLoader;
		g_pFontLoader = NULL;
	}
	
	Gdiplus::GdiplusShutdown(g_gdiplusToken);
	return CWinApp::ExitInstance();
}
