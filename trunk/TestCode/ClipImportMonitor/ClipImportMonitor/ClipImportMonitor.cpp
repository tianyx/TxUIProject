/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipImportMonitor.cpp
	file base:	ClipImportMonitor
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipImportMonitor.cpp : ����Ӧ�ó��������Ϊ��
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


// CClipImportMonitorApp ����

CClipImportMonitorApp::CClipImportMonitorApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CClipImportMonitorApp ����

CClipImportMonitorApp theApp;


// CClipImportMonitorApp ��ʼ��

BOOL CClipImportMonitorApp::InitInstance()
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

	HANDLE hUpdate = CreateMutex(NULL, FALSE, TEXT("{AB0B0F5C-A88C-4948-9843-FA42BEACF247}"));
	DWORD err = GetLastError();

	if (hUpdate != NULL)
	{		
		if (err == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hUpdate);
			AfxMessageBox("�Ѿ�������һ������!");
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

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ClipImprotMonitor"));

	//////////////////////////////////////////////////////////////////////////

	CString szFilePath = AfxGetAppPath();
	
	g_globalInfo.strAppPath = szFilePath;

	//װ��������Ϣ

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

int CClipImportMonitorApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	g_monDataMgr.Release();
	CloseApplication();
	CTxImageLoader::Release();
	g_FontLoader.Release();
	GdiplusShutdown(g_gdiplusToken);
	return CWinAppEx::ExitInstance();
}

BOOL CClipImportMonitorApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		POINT pos;   
		GetCursorPos (&pos);   
		pMsg->hwnd = WindowFromPoint (pos); 
		int a  = 0;
	}
	return CWinAppEx::PreTranslateMessage(pMsg);
}
