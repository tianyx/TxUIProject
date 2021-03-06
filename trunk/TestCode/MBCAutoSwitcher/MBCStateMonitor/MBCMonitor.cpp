
// MBCMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MBCMonitor.h"
#include "MainFrm.h"
#include "GlobalDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"

#include "GdiPlusNewHeader.h"
#include "TxFontLoader.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken = NULL;

// CMBCMonitorApp

BEGIN_MESSAGE_MAP(CMBCMonitorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMBCMonitorApp::OnAppAbout)
END_MESSAGE_MAP()


// CMBCMonitorApp 构造

CMBCMonitorApp::CMBCMonitorApp()
{

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CMBCMonitorApp 对象

CMBCMonitorApp theApp;


// CMBCMonitorApp 初始化

BOOL CMBCMonitorApp::InitInstance()
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

	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);

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
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	if (wsaData.wVersion != MAKEWORD(2,2))
	{
		ASSERT(FALSE);
		AfxMessageBox(TEXT("系统不支持winsock2"));
		return FALSE;
	}

	GetTxLogMgr()->AddNewLogFile(LOGKEYMAIN, TEXT("MonitorLog"), TRUE);

	if (!InitGlobalConfig())
	{
		return FALSE;
	}

	g_pFontLoader = new CTxFontLoader;

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






	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}


// CMBCMonitorApp 消息处理程序




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
void CMBCMonitorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMBCMonitorApp 消息处理程序




int CMBCMonitorApp::ExitInstance()
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

	return CWinApp::ExitInstance();
}
