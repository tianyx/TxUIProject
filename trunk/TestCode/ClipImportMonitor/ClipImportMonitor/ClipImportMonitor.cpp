/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipImportMonitor.cpp
	file base:	ClipImportMonitor
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipImportMonitor.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ClipImportMonitor.h"
#include "ClipImportMonitorDlg.h"
#include "AutoDump.h"
#include "Log.h"
#include "MonDataMgr.h"
#include "COCO20/Coco_APPx.h"
#include "TxImageLoader.h"
#include "TrCommonInfoMgr.h"
#include "TxFontLoader.h"
CMonDataMgr		g_monDataMgr; 
CTxFontLoader  g_FontLoader;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR g_gdiplusToken = NULL;

#pragma comment(lib, "gdiplus.lib")


// CClipImportMonitorApp

BEGIN_MESSAGE_MAP(CClipImportMonitorApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClipImportMonitorApp 构造

CClipImportMonitorApp::CClipImportMonitorApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CClipImportMonitorApp 对象

CClipImportMonitorApp theApp;


// CClipImportMonitorApp 初始化

BOOL CClipImportMonitorApp::InitInstance()
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

	HANDLE hUpdate = CreateMutex(NULL, FALSE, TEXT("{AB0B0F5C-A88C-4948-9843-FA42BEACF247}"));
	DWORD err = GetLastError();

	if (hUpdate != NULL)
	{		
		if (err == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hUpdate);
			AfxMessageBox("已经运行了一个程序!");
			return FALSE;
		}
	}


	AutoDumpInit();
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);

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
	SetRegistryKey(_T("ClipImprotMonitor"));

	//////////////////////////////////////////////////////////////////////////

	CString szFilePath = AfxGetAppPath();
	
	g_globalInfo.strAppPath = szFilePath;

	//装载语言信息

	CString strLogFile = g_globalInfo.strAppPath;
	strLogFile +=TEXT("log");
	CreateDirectory(strLogFile, NULL);
	strLogFile += TEXT("\\ClipImportMonitor.log");
	g_log.SetFile(strLogFile);



	if (::SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof (g_logfont), &g_logfont, 0))
	{
		g_logfont.lfHeight = -12;
		//		VERIFY(g_fontBase.CreateFontIndirect(&logfont));
		//		logfont.lfHeight = -30;
		//		VERIFY(g_fontBig.CreateFontIndirect(&logfont));

	}
	else
	{
		memset(&g_logfont, 0, sizeof(g_logfont));
		g_logfont.lfHeight = -12;
		g_logfont.lfCharSet = DEFAULT_CHARSET;
		g_logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		g_logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		g_logfont.lfQuality = DEFAULT_QUALITY;
		g_logfont.lfPitchAndFamily = DEFAULT_PITCH;
		StrCpy(g_logfont.lfFaceName, TEXT("system"));
		//VERIFY(g_fontBase.CreateFontIndirect(&g_logfont));
		//VERIFY(g_fontBig.CreateFontIndirect(&g_logfont));

	}

	CString strfontLanTxt= TEXT("Arial");
	StrCpy(g_logfont.lfFaceName, strfontLanTxt);
	g_fontBase.CreateFontIndirect(&g_logfont);

	HDC hDeskDc = GetWindowDC(::GetDesktopWindow());
	ASSERT(hDeskDc);
	g_globalInfo.nMonitorSize.cx = GetDeviceCaps(hDeskDc, HORZSIZE);
	g_globalInfo.nMonitorSize.cy = GetDeviceCaps(hDeskDc, VERTSIZE);
	g_globalInfo.nScreenSize.cx = GetDeviceCaps(hDeskDc, HORZRES);
	g_globalInfo.nScreenSize.cy = GetDeviceCaps(hDeskDc, VERTRES);
	//
	//////////////////////////////////////////////////////////////////////////
	//init coco;
	StartApplication();
	g_CoInfoMgr.InitMgr();
 	BOOL bInit = g_monDataMgr.Init();
 	if (!bInit)
 	{
		ASSERT(FALSE);
 		return FALSE;
 	}

	CClipImportMonitorDlg dlg;
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

int CClipImportMonitorApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	g_monDataMgr.Release();
	CloseApplication();
	CTxImageLoader::Release();
	g_FontLoader.Release();
	GdiplusShutdown(g_gdiplusToken);
	return CWinAppEx::ExitInstance();
}

BOOL CClipImportMonitorApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		POINT pos;   
		GetCursorPos (&pos);   
		pMsg->hwnd = WindowFromPoint (pos); 
		int a  = 0;
	}
	return CWinAppEx::PreTranslateMessage(pMsg);
}
