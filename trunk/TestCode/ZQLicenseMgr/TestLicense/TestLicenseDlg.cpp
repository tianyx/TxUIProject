
// TestLicenseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestLicense.h"
#include "TestLicenseDlg.h"
#include "TxSystemOp.h"
#include "ZQProtectDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTestLicenseDlg 对话框




CTestLicenseDlg::CTestLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestLicenseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtRet);
}

BEGIN_MESSAGE_MAP(CTestLicenseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestLicenseDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_TESTLC, &CTestLicenseDlg::OnBnClickedBtnTestlc)
	ON_BN_CLICKED(IDC_BTN_GETLC, &CTestLicenseDlg::OnBnClickedBtnGetlc)
END_MESSAGE_MAP()


// CTestLicenseDlg 消息处理程序

BOOL CTestLicenseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OnBnClickedBtnTestlc();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestLicenseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestLicenseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestLicenseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestLicenseDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ST_NAME name;
	name.key1 = "2010-01-03";
	name.key2 = "chbroadcast";
	name.key3 = GenRegCode();
	InitZQ(name);
	ST_NAME name2;
	name2.key4 = name.key4;
	UnInitZQ(name2);
	int a = 0;
	SYSTEMTIME tmForLc;
	GetSystemTime(&tmForLc);
	int nRandForLc = tmForLc.wSecond*tmForLc.wMinute*tmForLc.wHour;

}


void CTestLicenseDlg::OnBnClickedBtnTestlc()
{
	CheckLc();
	// TODO: 在此添加控件通知处理程序代码
	
}


void CTestLicenseDlg::OnBnClickedBtnGetlc()
{
	m_edtRet.SetWindowText("");
	// TODO: 在此添加控件通知处理程序代码
	typedef int (__cdecl *GetZQ)(char*, const int);
	GetZQ  zqFun;
	int nRetCode = -200;
	char szbuff[256];
	memset(szbuff, 0, sizeof(szbuff));
	GetModuleFileName(NULL, szbuff, sizeof(szbuff) -1);
	CString strAppPath = szbuff;
	int nPos = strAppPath.ReverseFind('\\');
	CString szFile = strAppPath.Mid(0, nPos);
	szFile += TEXT("\\ZQLpt.dll");
	HMODULE hLCModule = LoadLibrary(szFile);
	if (hLCModule != NULL)
	{
		zqFun = (GetZQ)GetProcAddress(hLCModule, "GetZQApp");
		if (zqFun)
		{
			char szRetbuff[1024];
			ZeroMemory(szRetbuff, 1024);
			nRetCode = zqFun(szRetbuff, 1024);
			CString strMsg;
			strMsg.Format("RetCode =%d", nRetCode);
			strMsg +="\r\n输出信息:\r\n";
			strMsg +=szRetbuff;
			m_edtRet.SetWindowText(strMsg);
		}
		FreeLibrary(hLCModule);
		hLCModule = NULL;
	}

	if (nRetCode < 0)
	{
		CString strLCMsgRet;
		strLCMsgRet.Format("授权信息验证错误，code = %d", nRetCode);
		AfxMessageBox(strLCMsgRet);
	}

}

MACRODEFDELAYNOTIFYPROC(10000)

BOOL CTestLicenseDlg::CheckLc()
{
	m_edtRet.SetWindowText("");
	MACROZQLCCHECK(TRUE, MACROERRNORET);
	CString strMsg;
	strMsg.Format("RetCode =%d", nRetCode);
	m_edtRet.SetWindowText(strMsg);
	return TRUE;
}

LRESULT CTestLicenseDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_LICENSETOBEEXPIRE)
	{
		AfxMessageBox(TEXT("授权已到期，请及时续期！"));
		return TRUE;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
