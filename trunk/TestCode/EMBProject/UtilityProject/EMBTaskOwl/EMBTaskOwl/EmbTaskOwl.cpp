
// EmbTaskOwl.cpp : ����Ӧ�ó��������Ϊ��
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


// CEmbTaskOwlApp ����

CEmbTaskOwlApp::CEmbTaskOwlApp()
{

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CEmbTaskOwlApp ����

CEmbTaskOwlApp theApp;


// CEmbTaskOwlApp ��ʼ��

BOOL CEmbTaskOwlApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	
	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
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
	//SetRegistryKey(_T("embtaskowl"));
	HANDLE hmutex;
	if (IsAppRunning(hmutex, FALSE))
	{
		AfxMessageBox(TEXT("�����Ѿ���������."));
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
		AfxMessageBox("EMB���ݿ��ʧ�ܣ�");
		return FALSE;
	}

	 bDbOpen = CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_PLAYLIST, g_GlobalInfo.strPlayListCheckDBConn);
	if (!bDbOpen)
	{
		AfxMessageBox("���ݿ�1��ʧ�ܣ�");
		return FALSE;
	}

	 bDbOpen = CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEY_PLAYLIST2, g_GlobalInfo.strPlayListCheckDBConn2);
	if (!bDbOpen)
	{
		AfxMessageBox("���ݿ�2��ʧ�ܣ�");
		return FALSE;
	}

	if (!LoadSectionInfo())
	{
		AfxMessageBox(TEXT("��ȡƵ����Ϣʧ�ܣ�"));
		return FALSE;
	}

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// ���������ؿ�ܼ�����Դ
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	g_pMainFrame = pFrame;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}


// CEmbTaskOwlApp ��Ϣ�������




// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CEmbTaskOwlApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEmbTaskOwlApp ��Ϣ�������




int CEmbTaskOwlApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
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
