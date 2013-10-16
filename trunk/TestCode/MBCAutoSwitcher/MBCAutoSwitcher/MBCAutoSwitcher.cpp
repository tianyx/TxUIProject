
// MBCAutoSwitcher.cpp : ����Ӧ�ó��������Ϊ��
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


// CMBCAutoSwitcherApp ����

CMBCAutoSwitcherApp::CMBCAutoSwitcherApp()
{

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CMBCAutoSwitcherApp ����

CMBCAutoSwitcherApp theApp;


// CMBCAutoSwitcherApp ��ʼ��

BOOL CMBCAutoSwitcherApp::InitInstance()
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

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
#ifdef _DEBUG
MACRO_CREATEOUTPUTCONSOLE
#endif // _DEBUG


	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
	if (wsaData.wVersion != MAKEWORD(2,2))
	{
		int nErr = WSAGetLastError();
		ASSERT(FALSE);
		AfxMessageBox(TEXT("ϵͳ��֧��winsock2"));
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
	g_pFontLoader->AddFont(TEXT("����"), 16, TRUE);
	if (!InitGlobalConfig())
	{
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

	g_hFrame = pFrame->GetSafeHwnd();

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	return TRUE;
}


// CMBCAutoSwitcherApp ��Ϣ�������




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
void CMBCAutoSwitcherApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMBCAutoSwitcherApp ��Ϣ�������




int CMBCAutoSwitcherApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
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
