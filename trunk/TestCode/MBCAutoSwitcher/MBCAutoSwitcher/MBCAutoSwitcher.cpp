
// MBCAutoSwitcher.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MBCAutoSwitcher.h"
#include "MainFrm.h"
#include "GlobalDef.h"
#include "FGlobal.h"
#include "TxFontLoader.h"
#include "TxLogManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken = NULL;

// CMBCAutoSwitcherApp

BEGIN_MESSAGE_MAP(CMBCAutoSwitcherApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMBCAutoSwitcherApp::OnAppAbout)
END_MESSAGE_MAP()


// CMBCAutoSwitcherApp 构造

CMBCAutoSwitcherApp::CMBCAutoSwitcherApp()
{

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMBCAutoSwitcherApp 对象

CMBCAutoSwitcherApp theApp;


// CMBCAutoSwitcherApp 初始化

BOOL CMBCAutoSwitcherApp::InitInstance()
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

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
#ifdef _DEBUG
MACRO_CREATEOUTPUTCONSOLE
#endif // _DEBUG


	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	if (wsaData.wVersion != MAKEWORD(2,2))
	{
		int nErr = WSAGetLastError();
		ASSERT(FALSE);
		AfxMessageBox(TEXT("系统不支持winsock2"));
		return FALSE;
	}

	CString strLogFile = 	GetAppPath().c_str();
	strLogFile += TEXT("\\log");
	CreateDirectory(strLogFile, NULL);
	strLogFile +=TEXT("\\autoswitch.log");
	GetTxLogMgr()->AddNewLogFile(SWITCHERLOGKEY, strLogFile);
	GetTxLogMgr()->Start();
	//test log
	CFWriteLog(SWITCHERLOGKEY, "!!!new log started!!!");
	CFWriteLog(SWITCHERLOGKEY, "Test message");

	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);
	g_pFontLoader = new CTxFontLoader;
	g_pFontLoader->AddFont(TEXT("宋体"), 16, TRUE);
	if (!InitGlobalConfig())
	{
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

	g_hFrame = pFrame->GetSafeHwnd();

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}


// CMBCAutoSwitcherApp 消息处理程序




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
void CMBCAutoSwitcherApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMBCAutoSwitcherApp 消息处理程序




int CMBCAutoSwitcherApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	ReleaseTxLogMgr();
	WSACleanup();
	if (g_pFontLoader)
	{
		delete g_pFontLoader;
		g_pFontLoader = NULL;
	}
	Gdiplus::GdiplusShutdown(g_gdiplusToken);

#ifdef _DEBUG
MACRO_FREEOUTPUTCONSOLE
#endif // _DEBUG
	return CWinApp::ExitInstance();
}
