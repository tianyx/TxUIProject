
// AotoFill.cpp : 定义应用程序的类行为。
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


// CAotoFillApp 构造

CAotoFillApp::CAotoFillApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAotoFillApp 对象

CAotoFillApp theApp;


// CAotoFillApp 初始化

BOOL CAotoFillApp::InitInstance()
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
	AfxOleInit();
	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	g_GlobalInfo.szAppPath = GetAppPath();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\config.ini");
	if (_access(g_GlobalInfo.szIniPath.c_str(), 04) == -1)
	{
		AfxMessageBox("找不到配置文件config.ini！");
		return FALSE;
	}
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));
	GetPrivateProfileStringA("Config","addressList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff, g_GlobalInfo.vAddress);
	if (g_GlobalInfo.vAddress.size() == 0)
	{
		AfxMessageBox("配置文件错误，地址列表未找到");
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
