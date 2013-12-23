
// EmbTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTest.h"
#include "EmbTestDlg.h"
#include "TxParamString.h"
#include "EmbStructDef.h"
#include "TxADOConn.h"

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


// CEmbTestDlg 对话框




CEmbTestDlg::CEmbTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmbTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEmbTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DBSTR, m_edtdbStr);
}

BEGIN_MESSAGE_MAP(CEmbTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CEmbTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEmbTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEmbTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_TESTADO, &CEmbTestDlg::OnBnClickedBtnTestado)
	ON_BN_CLICKED(IDC_BUTTON5, &CEmbTestDlg::OnBnClickedButton5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_AUTOGENTASK, &CEmbTestDlg::OnBnClickedBtnAutogentask)
END_MESSAGE_MAP()


// CEmbTestDlg 消息处理程序

BOOL CEmbTestDlg::OnInitDialog()
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

	m_edtdbStr.SetWindowText(TEXT("Provider=SQLOLEDB; Server=.; Database=emb; uid=sa; pwd=123"));
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEmbTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEmbTestDlg::OnPaint()
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
HCURSOR CEmbTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEmbTestDlg::OnBnClickedButton1()
{
	CTxParamString str(TEXT("<edoc_main bbb=\"10\"></edoc_main>"));
	str.GoIntoKey(TEXT("edoc_main"));
	int nTick = GetTickCount();
	for (int i = 0; i < 2000 ; i++)
	{
		CTxStrConvert val;
		val.SetVal(10);
		CString strTmp;
		strTmp.Format(TEXT("keyvalue-%02d"), i);
		str.SetElemVal(strTmp, val);
		str.SetAttribVal(strTmp, TEXT("attrib1"), val);
	}

	str.UpdateData();
	CString strRerr= str;
	nTick = GetTickCount() - nTick;

	CTxStrConvert val;
	val.SetVal(10);
	str.SetAttribVal(NULL, "aaa", "1123");
	//str.SetAttribVal(NULL, "bbb", val);
	str.UpdateData();
	CString strRet = str;
	CTxParamString test(strRet);
	test.GoIntoKey("edoc_main");
	strRet = test.GetAttribVal(NULL, "aaa").GetAsString();
	strRet = test.GetAttribVal(NULL, "bbb").GetAsString();
	AfxMessageBox(strRet);


	CFile file;
	file.Open("f:\\dd.xml", CFile::modeRead, NULL);
	char szbuff[2048];
	ZeroMemory(szbuff, 2048);
	file.Read(szbuff, 2048);
	file.Close();
	CTxParamString txStr(szbuff);
	int nId = txStr.GetAttribVal(TEXT(""), TEXT("ID")).GetAsInt();
	txStr.GoToPath(TEXT(".\\SpotListInfo"));
	CString strDate = txStr.GetElemVal(TEXT("Date")).GetAsString();
	txStr.SetElemVal(TEXT("Date"), CTxStrConvert(TEXT("2013-11-11")));
	txStr.SetAttribVal(TEXT("Date"), TEXT("DD"), CTxStrConvert("testAttr"));
	txStr.UpdateData();

}

void CEmbTestDlg::OnBnClickedButton2()
{
	ST_TASKREPORT report;
	CString strREt;
	report.ToString(strREt);


	// TODO: 在此添加控件通知处理程序代码
	CString strParm;
	strParm = TEXT("\"f:\\111\\*.*\" \"f:\\222\\*.*\" /S /C /Y");
	HINSTANCE nRet = ShellExecute(NULL, 0, "xcopy", strParm, NULL, SW_HIDE);
	if ((int)nRet < 32)
	{
		ASSERT(FALSE);
	}
}

typedef int (__cdecl *GETPLUGININSTANCE)(LPVOID& pInterface);


void CEmbTestDlg::OnBnClickedButton3()
{

	// TODO: 在此添加控件通知处理程序代码
	HMODULE hModuleOut = NULL;
	CString strFile = TEXT("TestWorkString.dll");
	hModuleOut = LoadLibrary(strFile);
	EMB::ITaskWorkerCallInterface * pInterfaceOut = NULL;
	if (hModuleOut != NULL)
	{
		GETPLUGININSTANCE  pFun = (GETPLUGININSTANCE)GetProcAddress(hModuleOut, "TestWorkString");
		if (pFun)
		{
			pFun((LPVOID&)pInterfaceOut);
			if (pInterfaceOut)
			{
				CEMBWorkString str;
				pInterfaceOut->GetTaskProgress(str);
			}
		}
		else
		{
			FreeLibrary(hModuleOut);
			hModuleOut = NULL;
		}

	}
}

void CEmbTestDlg::OnBnClickedBtnTestado()
{
	CString strTaskGuid;
	GUID   guid; 
	if (S_OK != ::CoCreateGuid(&guid)) 
	{
		return;
	}

	strTaskGuid = Guid2String(guid);

	// TODO: 在此添加控件通知处理程序代码
	
	
	CTxADOCommand command;
	VECSTRINGS vStr;

	CFile file;
	CString strFileAuto = GetAppPath().c_str();
	strFileAuto += TEXT("\\autotemplate.xml");
	CString strXml;

	if(file.Open(strFileAuto, CFile::modeRead, NULL))
	{
		int nLen = file.GetLength();
		char szbuff[4096];
		ZeroMemory(szbuff, 4096);
		file.Read(szbuff, 4096);
		file.Close();
		strXml = szbuff;
	}
	

	if (strXml.IsEmpty())
	{
		return;
	}
	strXml.Replace("{TaskGuid}", strTaskGuid);
	strXml.Replace("{TaskDesFile}", strTaskGuid);

	CString strTmp;
	strTmp.Format(TEXT("insert into T_EMBTask (strTaskID, nPRI, nState,strExtendInfoXml, nRetry) values ('%s', '80','0', '%s', '3')"), strTaskGuid, strXml);
	
	CTxADORecordSet rs;
	rs.ExecuteSQL(strTmp);

	//CTxADODBMgr::ReleaseDBMgr();
}

void CEmbTestDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE);
	dlg.DoModal();
}

void CEmbTestDlg::OnTimer( UINT_PTR nIDEvent )
{
	if (nIDEvent == 1000)
	{
		OnBnClickedBtnTestado();
	}

	CDialog::OnTimer(nIDEvent);
}

void CEmbTestDlg::OnBnClickedBtnAutogentask()
{
	// TODO: 在此添加控件通知处理程序代码
	static BOOL bstRun = FALSE;
	bstRun = !bstRun;
	if (bstRun)
	{
		
		CString strConn;
		m_edtdbStr.GetWindowText(strConn);
		if(CTxADODBMgr::GetADODBMgr()->OpenDB(1, strConn))
		{
			SetTimer(1000, 10000, NULL); // 间隔为3分钟 
		}
		else
		{
			bstRun = FALSE;
		}

	}
	else
	{
		CTxADODBMgr::ReleaseDBMgr();
		KillTimer(1000);
	}
	GetDlgItem(IDC_BTN_AUTOGENTASK)->SetWindowText(bstRun? TEXT("stop"):TEXT("start"));
}
