
// EmbTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTest.h"
#include "EmbTestDlg.h"
#include "TxParamString.h"
#include "EmbStructDef.h"
#include "TxADOConn.h"
#include "MainDef.h"

#include "Pdh.h"
#pragma comment(lib, "Pdh.lib")
#pragma comment(lib, "NetMessageTranslate.lib")
#include "..\NetMessageTranslate\NetMsgTransHeader.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
LRESULT CALLBACK TestObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		CFWriteLog(0, TEXT("wm_destory wnd "));

	}
	// Window msg
	return DefWindowProc (hwnd, message, wParam, lParam);
}




DWORD __stdcall TestMsgLoopThread( void* lparam )
{
	CEmbTestDlg* pExcObj = (CEmbTestDlg*)lparam;
	HINSTANCE hInstance = GetSelfModuleHandle();
	CFWriteLog(0, TEXT("start RegisterClassEx"));
	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = TestObjectWndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = NULL;
	wndclassex.hCursor       = NULL;
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);;
	wndclassex.lpszMenuName  = NULL;
	CString strClsName = Guid2String(TxGenGuid());
	wndclassex.lpszClassName = strClsName;
	wndclassex.hIconSm       = wndclassex.hIcon;
	CFWriteLog(0, TEXT("call RegisterClassEx"));

	if ( 0 == RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		if (hr != 0x00000582)
		{
			CString strMsg;
			strMsg.Format(TEXT("reg class error %d"), hr);
			OutputDebugString(strMsg);
			CFWriteLog(0, strMsg);
			ASSERT(FALSE);
			return 0;
		}
		
	}

	HWND& hwnd = pExcObj->m_hwndExcMsg;
	ASSERT(hwnd == NULL);
	CFWriteLog(0, TEXT("start CreateWindowEx"));

	hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		wndclassex.lpszClassName, 
		TEXT (""),
		WS_OVERLAPPEDWINDOW,
		0, 
		0, 
		50, 
		50, 
		NULL, 
		NULL, 
		hInstance,0);

	CFWriteLog(0, TEXT("end CreateWindowEx"));

	if (hwnd == NULL)
	{
		HRESULT hr = GetLastError();
		CString strMsg;
		strMsg.Format(TEXT("wndcreate error %d"), hr);
		CFWriteLog(0, strMsg);
		OutputDebugString(strMsg);
		return 0;
	}

	CFWriteLog(0, TEXT("start show wnd"));

	ShowWindow(hwnd, SW_HIDE);
	ASSERT(::IsWindow(hwnd));
	//::SetWindowLong(hwnd,  GWL_USERDATA, (LONG)pMgr);
	
	//CFWriteLog(TEXT("SockmsgWnd = %x, bind sock %d"), hwnd, this->m_hSock);

	//ShowWindow (hwnd, SW_HIDE);
	UpdateWindow (hwnd);
	SetEvent(pExcObj->m_hEventMsgWndCreated);
	CFWriteLog(0, TEXT("enter wnd loop"));
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(IsWindow(hwnd))
	{
		DestroyWindow(hwnd);
	}
	BOOL bSuc =  UnregisterClass(strClsName, hInstance);
	if(!bSuc)
	{
		HRESULT hr = GetLastError();
		CString strMsg;
		strMsg.Format(TEXT("wndcreate error %d"), hr);
		ASSERT(FALSE);
		CFWriteLog(0, strMsg);
	}
	CFWriteLog(0, TEXT("UnregisterClass"));
	return 0;
}

DWORD __stdcall TestMsgLooptest( void* lparam )
{
	CEmbTestDlg* pExcObj = (CEmbTestDlg*)lparam;
	HANDLE m_hThreadMsgLoop = NULL;
	pExcObj->m_bRunit = TRUE;
	UINT nCount = 0;
	while (pExcObj->m_bRunit)
	{
		nCount++;
		CFWriteLog(TEXT("----<%d>"), nCount);
		m_hThreadMsgLoop = CreateThread(NULL, 0, TestMsgLoopThread, (LPVOID)lparam, 0, 0);
		CFWriteLog(0, TEXT("test thread created"));
		WaitForSingleObject(pExcObj->m_hEventMsgWndCreated, INFINITE);
		::PostMessage(pExcObj->m_hwndExcMsg, WM_QUIT, 0,0);
		WaitForSingleObject(m_hThreadMsgLoop, INFINITE);
		CFWriteLog(0, TEXT("test thread quit"));
		m_hThreadMsgLoop = NULL;
		pExcObj->m_hwndExcMsg = NULL;
		ResetEvent(pExcObj->m_hEventMsgWndCreated);
	}

	return 0;
}

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
	m_hwndExcMsg = NULL;
	m_hEventMsgWndCreated = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bRunit = FALSE;
}

void CEmbTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DBSTR, m_edtdbStr);
	DDX_Control(pDX, IDC_EDIT1, m_edtXmltrans);
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
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_AUTOGENTASK, &CEmbTestDlg::OnBnClickedBtnAutogentask)
	ON_BN_CLICKED(IDC_BTN_PERFORMCOUNTER, &CEmbTestDlg::OnBnClickedBtnPerformcounter)
	ON_BN_CLICKED(IDC_BUTTON4, &CEmbTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BTN_PERFORMCOUNTER2, &CEmbTestDlg::OnBnClickedBtnPerformcounter2)
	ON_BN_CLICKED(IDC_BUTTON5, &CEmbTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CEmbTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CEmbTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CEmbTestDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CEmbTestDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BTNTIMETOSTR, &CEmbTestDlg::OnBnClickedBtntimetostr)

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

	m_edtdbStr.SetWindowText(g_GlobalInfo.szDBConn);
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

CString GenClipId()
{
	srand(time(NULL));
	static UINT64 nSeed =rand();
	nSeed++;
	CString strRet;
	strRet.Format(TEXT("%08d"), nSeed%100000000);
	return strRet;
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
		char szbuff[9096];
		ZeroMemory(szbuff, 9096);
		file.Read(szbuff, 9096);
		file.Close();
		strXml = szbuff;
	}
	

	if (strXml.IsEmpty())
	{
		return;
	}

	CString strClipId = GenClipId();
	strXml.Replace("{TaskGuid}", strTaskGuid);
	strXml.Replace("{TaskDesFile}", strClipId);
	strXml.Replace("{TaskClipID}", strClipId);

	CString strTmp;
	strTmp.Format(TEXT("insert into T_EMBTask (strTaskID, nPRI, nState,strExtendInfoXml, nRetry) values ('%s', '80','0', '%s', '3')"), strTaskGuid, strXml);
	
	CTxADORecordSet rs;
	rs.ExecuteSQL(strTmp);

	//CTxADODBMgr::ReleaseDBMgr();
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
// 	static BOOL bstRun = FALSE;
// 	bstRun = !bstRun;
// 	if (bstRun)
// 	{
		
		CString strConn;
		m_edtdbStr.GetWindowText(strConn);
		if(CTxADODBMgr::GetADODBMgr()->OpenDB(1, strConn))
		{
			//SetTimer(1000, 10000, NULL); // 间隔为3分钟 
			OnBnClickedBtnTestado();
			CTxADODBMgr::ReleaseDBMgr();
		}
		
// 	}
// 	else
// 	{
// 		CTxADODBMgr::ReleaseDBMgr();
// 		KillTimer(1000);
// 	}
// 	GetDlgItem(IDC_BTN_AUTOGENTASK)->SetWindowText(bstRun? TEXT("stop"):TEXT("start"));
}

void CEmbTestDlg::OnBnClickedBtnPerformcounter()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD nBuffSize = 0;
	PdhEnumObjects(NULL, NULL, NULL,  &nBuffSize, PERF_DETAIL_WIZARD, TRUE);
	if (nBuffSize == 0)
	{
		return;
	}
	TCHAR* pBuff = new TCHAR[nBuffSize];
	PdhEnumObjects(NULL, NULL, pBuff, &nBuffSize, PERF_DETAIL_WIZARD, TRUE);
	TCHAR* pIdx = pBuff;
	TCHAR* pSubStr =pBuff;
	int nCount = 0;
	while(nCount < nBuffSize)
	{
		if (*pIdx == TEXT('\0'))
		{
			TRACE("\n");
			TRACE(pSubStr);
			pSubStr = pIdx +1;
			if (*pSubStr == TEXT('\0'))
			{
				break;
			}
		}

		++nCount;
		++pIdx;
	}


}

void CEmbTestDlg::OnBnClickedButton4()
{
	
	// TODO: 在此添加控件通知处理程序代码
	char szOut[1024];
	int nUsed = 0;
	char* pOUt = szOut;
	ExpFunPackEMBTcpMsg(embmsgtype_UIClientToUIServerMsg, "134455555", "xmltest12344", pOUt, 1024, nUsed);

	int nmsgTypeOut = 0;
	char szXmlOut[1024];
	char szGuidOut[128];
	char* pxmlOUt = szXmlOut;
	char* pGuidOut = szGuidOut;
	ExpFunUnPackEMBTcpMsg(szOut, nUsed, nmsgTypeOut, pGuidOut, pxmlOUt, 1024);
	int a = 0;

}

void CEmbTestDlg::OnBnClickedBtnPerformcounter2()
{
	// TODO: 在此添加控件通知处理程序代码

	CFileDialog xmlFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Xml File (*.xml)|*.xml"), (CWnd *)this, 0);

	if (IDOK == xmlFileDlg.DoModal() )
	{
		CString xmlFile = xmlFileDlg.GetPathName();
		CString strTaskGuid;
		GUID   guid; 
		if (S_OK != ::CoCreateGuid(&guid)) 
		{
			return;
		}

		strTaskGuid = Guid2String(guid);


		CFile file;
		CString strXml;

		if(file.Open(xmlFile, CFile::modeRead, NULL))
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
		CString strConn;
		m_edtdbStr.GetWindowText(strConn);

		if(CTxADODBMgr::GetADODBMgr()->OpenDB(2, strConn))
		{
			CString strTmp;
			strTmp.Format(TEXT("insert into T_EMBTask (strTaskID, nPRI, nState,strExtendInfoXml, nRetry) values ('%s', '80','0', '%s', '3')"), strTaskGuid, strXml);

			CTxADORecordSet rs;
			rs.ExecuteSQL(strTmp);
			CTxADODBMgr::ReleaseDBMgr();
		}
	}
}


void CEmbTestDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	ST_FCVSRESULTTASK rstask;
	rstask.bRegisterToDB = TRUE;
	rstask.bRuntimeUpdate = TRUE;
	rstask.bWirteResultXml = TRUE;
	rstask.filePath = TEXT("c:\\111.mpg");
	rstask.nTotalSectionCount = 1;
	rstask.strDBConn = TEXT("");
	CString strRs;
	rstask.ToString(strRs);

	ST_TRANSFILEINFO transInfo;
	ST_FTPSITEINFO ftpInfo;
	transInfo.vSitSrc.push_back(ftpInfo);
	transInfo.vSitSrc.push_back(ftpInfo);
	transInfo.vSitDes.push_back(ftpInfo);
	transInfo.vSitDes.push_back(ftpInfo);
	CString strTmp;
	transInfo.ToString(strTmp);

	CString strFcvsfile;
	strFcvsfile = GetAppPath().c_str();
	strFcvsfile += TEXT("\\task3.txt");
	CFile file;
	BOOL bOpen = file.Open(strFcvsfile, CFile::modeRead, NULL);
	char szBuff[2048];
	ZeroMemory(szBuff, sizeof(szBuff));
	file.Read(szBuff, 2048);

	ST_FCVSTASKINFO fcvins;
	fcvins.FromString(szBuff);

	ST_SLEEPTASKINFO info;
	info.nReCallType = embrecalltype_spfcvs;
	info.nRetOnFinish = EMBERR_TIMEOUT;
	info.nSleepSec = 30;
	info.strExtInfo =szBuff;
	info.ToString(strTmp);
	return;
}

void CEmbTestDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strXmlTrans;
	m_edtXmltrans.GetWindowText(strXmlTrans);
	CString strOut = CTxParamString::EscapeString(strXmlTrans);
	m_edtXmltrans.SetWindowText(strOut);
}

void CEmbTestDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strXmlTrans;
	m_edtXmltrans.GetWindowText(strXmlTrans);
	CString strOut = CTxParamString::UnEscapeString(strXmlTrans);
	m_edtXmltrans.SetWindowText(strOut);
}

void CEmbTestDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	static HANDLE m_hThreadtet = NULL;
	if (!m_hThreadtet)
	{
		m_hThreadtet = CreateThread(NULL, 0, TestMsgLooptest, (LPVOID)this, 0, 0);

	}
	else
	{
		m_bRunit = FALSE;
		WaitForSingleObject(m_hThreadtet, INFINITE);
		m_hThreadtet = NULL;
		return ;
	}



}

void CEmbTestDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	int* p = 0;
	*p = 1;
}

void CEmbTestDlg::OnBnClickedBtntimetostr()
{
	CString strTime(TEXT("2014-12-22 12:22:23"));
	INT64 tmTest = Str2Time(strTime);
	CString strMsg;
	strMsg.Format(TEXT("org = %s, out = %s"), strTime, CTime(tmTest).Format(TEXT("%Y-%m-%d %H:%M:%S")));
	AfxMessageBox(strMsg);
}
