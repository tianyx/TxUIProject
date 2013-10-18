
// AutoClockInDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoClockIn.h"
#include "AutoClockInDlg.h"
#include "afxdialogex.h"
#include "HttpNetOp.h"
#include "DlgShutDown.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "ACConfig.h"

#define ID_TIMER_AUTOCLOCKIN 914

#define STR_ACRET_KEYWORD_BEGIN TEXT("<span id=\"lblTitle\">")
#define STR_ACRET_KEYWORD_END TEXT("</span>")

NOTIFYICONDATA g_trayData;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAutoClockInDlg 对话框




CAutoClockInDlg::CAutoClockInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoClockInDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bAutoMode = FALSE;
}

void CAutoClockInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_STC_N1, m_stcInfo[0]);
	DDX_Control(pDX, IDC_STC_N2, m_stcInfo[1]);
	DDX_Control(pDX, IDC_STC_N3, m_stcInfo[2]);
	DDX_Control(pDX, IDC_STC_N4, m_stcInfo[3]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_N1, m_ctlTimeClockIn[0]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_N2, m_ctlTimeClockIn[1]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_N3, m_ctlTimeClockIn[2]);
	DDX_Control(pDX, IDC_DATETIMEPICKER_N4, m_ctlTimeClockIn[3]);
	DDX_Control(pDX, IDC_EDIT_SVR, m_edtSvr);
	DDX_Control(pDX, IDC_CHECK_MODIFY, m_chkModify);
	DDX_Control(pDX, IDC_CHECK_SHOWLOG, m_chkShowLog);
	DDX_Control(pDX, IDC_EDIT_USER, m_edtUser);
	DDX_Control(pDX, IDC_EDIT_PW, m_edtPw);
	DDX_Control(pDX, IDC_STC_LASTTIP, m_edtLastTip);
	DDX_Control(pDX, IDC_CHECK_AUTORUN, m_chkAutorun);
	DDX_Control(pDX, IDC_CHECK_AUTOSTART, m_chkAutoStart);
	DDX_Control(pDX, IDC_CHECK_AUTOCLOCKIN1, m_chkAutoClock1);
	DDX_Control(pDX, IDC_CHECK_POPMSG, m_chkPopBubble);
	DDX_Control(pDX, IDC_CHECK_AUTOSHUTDOWN, m_chkShutdown);
	DDX_Control(pDX, IDC_EDIT_PRE_MINUTE, m_edtPreMinute);
	DDX_Control(pDX, IDC_BTN_AUTOSTART, m_btnAutoStart);
	DDX_Control(pDX, IDC_BTN_TRY1, m_btnTry[0]);
	DDX_Control(pDX, IDC_BTN_TRY2, m_btnTry[1]);
	DDX_Control(pDX, IDC_BTN_TRY3, m_btnTry[2]);
	DDX_Control(pDX, IDC_BTN_TRY4, m_btnTry[3]);
	DDX_Control(pDX, IDC_CHECK_AUTOSHUTDOWN2, m_chkSleep);
}

BEGIN_MESSAGE_MAP(CAutoClockInDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TEST, &CAutoClockInDlg::OnBnClickedMfcbtnTest)
	ON_BN_CLICKED(IDC_BTN_AUTOSTART, &CAutoClockInDlg::OnBnClickedBtnAutostart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_MODIFY, &CAutoClockInDlg::OnBnClickedCheckModify)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CAutoClockInDlg::OnBnClickedBtnExit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_SHOWLOG, &CAutoClockInDlg::OnBnClickedCheckShowlog)
	ON_BN_CLICKED(IDC_BTN_TRY1, &CAutoClockInDlg::OnBnClickedBtnTry1)
	ON_BN_CLICKED(IDC_BTN_TRY2, &CAutoClockInDlg::OnBnClickedBtnTry2)
	ON_BN_CLICKED(IDC_BTN_TRY3, &CAutoClockInDlg::OnBnClickedBtnTry3)
	ON_BN_CLICKED(IDC_BTN_TRY4, &CAutoClockInDlg::OnBnClickedBtnTry4)
	ON_BN_CLICKED(IDC_CHECK_AUTOSHUTDOWN, &CAutoClockInDlg::OnBnClickedCheckAutoshutdown)
END_MESSAGE_MAP()


// CAutoClockInDlg 消息处理程序

BOOL CAutoClockInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowPos(NULL, 0,0, 568, 394, SWP_NOMOVE);
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

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = -32;
	m_btnFont.CreateFontIndirect(&lf);

	m_btnAutoStart.SetFont(&m_btnFont);

	// TODO: 在此添加额外的初始化代码
	g_hwndLog = GetSafeHwnd();
	m_pCfg = CACConfig::GetACConfig();
	//
	ZeroMemory(&g_trayData, sizeof(g_trayData));
	g_trayData.cbSize = sizeof(g_trayData);
	g_trayData.hWnd = GetSafeHwnd();
	g_trayData.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CString strTrayTip = TEXT("Auto ClockIn");
	wsprintf(g_trayData.szInfo, TEXT("%s running now..."), strTrayTip);
	wsprintf(g_trayData.szTip, TEXT("%s"), strTrayTip);
	wsprintf(g_trayData.szInfoTitle, TEXT("%s"), strTrayTip);
	g_trayData.uID =IDR_MAINFRAME;
	g_trayData.uCallbackMessage = MSG_SYSTEM_TRAY;
	g_trayData.dwInfoFlags = NIIF_INFO;
	g_trayData.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO;
	g_trayData.uVersion = NOTIFYICON_VERSION;
	BOOL bRet =Shell_NotifyIcon(NIM_ADD, &g_trayData);

	//config
	BOOL bLoad = FALSE;
	bLoad =m_pCfg->LoadData();
	//
	m_chkModify.SetCheck(bLoad? BST_UNCHECKED:BST_CHECKED);
	LoadConfig();
	UpdateCtrlState();

	if (m_pCfg->m_cfgData.bAutoClockIn1)
	{
		//TryClock1 first;
		BOOL bRet =TryClock(0, TRUE);
	}

	if (bLoad && m_pCfg->m_cfgData.bAutoStart)
	{
		OnBnClickedBtnAutostart();
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAutoClockInDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_CLOSE || nID == SC_MINIMIZE)
		{
			ShowWindow(SW_HIDE);
			CString strTitle = TEXT("Auto ClockIn");
			CString strTip = TEXT("Auto ClockIn is been hidden.");
			ShowBubbleTip(strTitle, strTip);
			return;
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoClockInDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAutoClockInDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAutoClockInDlg::OnBnClickedMfcbtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	ShutDown();
	return;
	CheckClockInResult(0, m_pCfg->m_vNetData[0].netData.strFile);
	return;
	CString strTitle = TEXT("test");
	CString strTip = TEXT("test tip");
	ShowBubbleTip(strTitle, strTip);
	ShutDown();
	return;

	CHttpNetOp netOp;
	ST_NETOPINFO netData;
	netData.strUrl = TEXT("/_layouts/Founder/Check/Check.aspx?Type=1");
	netData.strUser = TEXT("tianyx");
	netData.strPw = TEXT("Founder@2011!");
	netData.strSvr = TEXT("itsoft.hold.founder.com");
	netData.strFile = TEXT("E:\\netRet.txt");
	netOp.Init(netData);
	netOp.DoGet();
}

void CAutoClockInDlg::OnBnClickedBtnAutostart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bAutoMode)
	{
		KillTimer(ID_TIMER_AUTOCLOCKIN);
		m_bAutoMode = FALSE;
	}
	else
	{
		SetTimer(ID_TIMER_AUTOCLOCKIN, 60000, NULL);
		m_bAutoMode = TRUE;
	}

	m_btnAutoStart.SetWindowText(m_bAutoMode? TEXT("Running"):TEXT("Stoped"));
}


void CAutoClockInDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_TIMER_AUTOCLOCKIN)
	{
		for(int i = 0; i < 4; i++)
		{
			TryClock(i);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

LRESULT CAutoClockInDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_AC_WRITELOG)
	{
		CString* pstrMsg = (CString*)wParam;
		m_lstLog.AddString(*pstrMsg);
		m_edtLastTip.SetWindowText(*pstrMsg);
// 		int nCount = m_lstLog.GetCount();
// 		if (nCount > 0)
// 			m_lstLog.SetCurSel(nCount - 1);
		SCROLLINFO scInfo;
		m_lstLog.GetScrollInfo(SB_VERT, &scInfo, SIF_POS);
		m_lstLog.SetScrollPos(SB_VERT, scInfo.nMax);


	}

	if (message == MSG_SHUTDOWN)
	{
		ACWriteLog(TEXT("received shutdown msg..."));
		CString strParm =TEXT("/h /f");
		ShellExecute(NULL, NULL, TEXT("shutdown"), strParm, NULL, SW_HIDE);
		return S_OK;
	}
	
	if (message == MSG_SYSTEM_TRAY)
	{
		if (lParam == WM_LBUTTONDBLCLK)
		{
			ShowWindow(SW_SHOWNORMAL);
			SetForegroundWindow();
			CenterWindow();
		}
		else if(lParam == WM_RBUTTONUP)
		{
			CMenu menu;
			menu.LoadMenu(IDR_MENUTRAY);
			CPoint pt;
			GetCursorPos(&pt);
			SetForegroundWindow();
			UINT nRetId = menu.GetSubMenu(0)->TrackPopupMenuEx( TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_RETURNCMD, pt.x, pt.y, this, NULL);
			if(nRetId == ID_TRAYPOP_EXIT)
			{
				OnBnClickedBtnExit();
			}
			else if (nRetId == ID_TRAYPOP_ABOUT)
			{
				CAboutDlg dlg;
				dlg.DoModal();
			}

		}
		
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CAutoClockInDlg::UpdateCtrlState()
{
	BOOL bEditMode = m_chkModify.GetCheck() == BST_CHECKED? TRUE:FALSE;
	
	if (bEditMode)
	{
		if (m_bAutoMode)
		{
			OnBnClickedBtnAutostart();
		}
	}
	m_btnAutoStart.EnableWindow(bEditMode? FALSE:TRUE);

	m_chkModify.SetWindowText(bEditMode? TEXT("save config"):TEXT("edit config"));
	for (int i = 0; i < 4; i++)
	{
		m_ctlTimeClockIn[i].EnableWindow(bEditMode);
		//m_stcInfo[i].EnableWindow(bEditMode);
		m_btnTry[i].EnableWindow(!bEditMode);
	}
	
	m_edtUser.EnableWindow(bEditMode);
	m_edtPw.EnableWindow(bEditMode);
	m_edtSvr.EnableWindow(bEditMode);
	m_edtPreMinute.EnableWindow(bEditMode);
	m_chkAutorun.EnableWindow(bEditMode);
	m_chkAutoStart.EnableWindow(bEditMode);
	m_chkPopBubble.EnableWindow(bEditMode);
	m_chkSleep.EnableWindow(bEditMode && m_pCfg->m_cfgData.bAutoShutdown);
	m_chkShutdown.EnableWindow(bEditMode);
	m_chkAutoClock1.EnableWindow(bEditMode);
	if (bEditMode)
	{
		m_edtLastTip.SetWindowText(TEXT("please save config first"));
	}
}

void CAutoClockInDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	Shell_NotifyIcon(NIM_DELETE, &g_trayData);
	m_pCfg->Release();
	CDialogEx::PostNcDestroy();
}

void CAutoClockInDlg::LoadConfig()
{
	m_edtSvr.SetWindowText(m_pCfg->m_vNetData[0].netData.strSvr);
	m_edtUser.SetWindowText(m_pCfg->m_vNetData[0].netData.strUser);
	m_edtPw.SetWindowText(m_pCfg->m_vNetData[0].netData.strPw);
	CString strPreMinute;
	strPreMinute.Format(TEXT("%d"), m_pCfg->m_cfgData.nPreMinute);
	m_edtPreMinute.SetWindowText(strPreMinute);
	for (int i = 0; i < 4; i++)
	{
		CTime tm(2012, 1,1, m_pCfg->m_vNetData[i].nHour, m_pCfg->m_vNetData[i].nMinute, 0);
		m_ctlTimeClockIn[i].SetTime(&tm);
	}

	m_chkAutorun.SetCheck(m_pCfg->m_cfgData.bAutoRun?BST_CHECKED:BST_UNCHECKED);
	m_chkAutoStart.SetCheck(m_pCfg->m_cfgData.bAutoStart?BST_CHECKED:BST_UNCHECKED);
	m_chkAutoClock1.SetCheck(m_pCfg->m_cfgData.bAutoClockIn1?BST_CHECKED:BST_UNCHECKED);
	m_chkPopBubble.SetCheck(m_pCfg->m_cfgData.bPopBubble?BST_CHECKED:BST_UNCHECKED);
	m_chkShutdown.SetCheck(m_pCfg->m_cfgData.bAutoShutdown?BST_CHECKED:BST_UNCHECKED);
	m_chkSleep.SetCheck(m_pCfg->m_cfgData.bSleepPC?BST_CHECKED:BST_UNCHECKED);
	if (m_pCfg->m_cfgData.nPreMinute <= 0)
	{
		m_nRandomMinute = 0;
	}
	else
	{
		srand(time(NULL));
		m_nRandomMinute = rand()% m_pCfg->m_cfgData.nPreMinute;

	}
}

void CAutoClockInDlg::OnBnClickedCheckModify()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bEditMode = m_chkModify.GetCheck() == BST_CHECKED;
	if (bEditMode)
	{
		//save value;
		if (SaveConfig())
		{
			m_chkModify.SetCheck(BST_UNCHECKED);
		}
		else
		{
			AfxMessageBox(TEXT("save failed, please check data"));
			
		}
	}
	else
	{
		m_chkModify.SetCheck(BST_CHECKED);
	}
	UpdateCtrlState();

}

BOOL CAutoClockInDlg::SaveConfig()
{

	CString strSvr, strUser, strPw;
	m_edtSvr.GetWindowText(strSvr);
	m_edtUser.GetWindowText(strUser);
	m_edtPw.GetWindowText(strPw);
	if (strSvr.IsEmpty())
	{
		return FALSE;
	}
	if (strUser.IsEmpty())
	{
		return FALSE;
	}

	VECCLOCKININFO vNetData;
	CWinApp* pApp = AfxGetApp();
	CString strAppPath = pApp->m_pszHelpFilePath;
	CString strPath = GetFileName(strAppPath);


	ST_CLOCKINDATA data;
	for (int i = 0; i < 4; i++)
	{
		data.netData.strUrl.Format(TEXT("/_layouts/Founder/Check/Check.aspx?Type=%d"), i+1) ;
		data.netData.strUser = strUser;
		data.netData.strPw = strPw;
		data.netData.strSvr = strSvr;
		data.netData.strFile.Format( TEXT("%sClockInLog%d.txt"), strPath, i+1);
		CTime tm;
		m_ctlTimeClockIn[i].GetTime(tm);
		data.nHour = tm.GetHour();
		data.nMinute = tm.GetMinute();
		vNetData.push_back(data);
	}

	m_pCfg->m_vNetData = vNetData;


	ST_ACSETTING  cfgData;
	cfgData.bAutoRun = m_chkAutorun.GetCheck() == BST_CHECKED;
	cfgData.bAutoClockIn1 = m_chkAutoClock1.GetCheck() == BST_CHECKED;
	cfgData.bAutoShutdown = m_chkShutdown.GetCheck() == BST_CHECKED;
	cfgData.bAutoStart = m_chkAutoStart.GetCheck() == BST_CHECKED;
	cfgData.bPopBubble = m_chkPopBubble.GetCheck() == BST_CHECKED;
	cfgData.bSleepPC = m_chkSleep.GetCheck() == BST_CHECKED;
	CString strPreMinute;
	m_edtPreMinute.GetWindowText(strPreMinute);
	int nPreMin = _wtoi(strPreMinute);
	cfgData.nPreMinute = nPreMin < 0 || nPreMin > 30? 0:nPreMin;
	m_pCfg->m_cfgData = cfgData;

	return m_pCfg->SaveData();
}

BOOL CAutoClockInDlg::TryClock1()
{
	BOOL bRet = FALSE;
	CTime tm(time(NULL));
	const int nHour =tm.GetHour();
	const int nMinute = tm.GetMinute();
	ST_CLOCKINDATA& data = m_pCfg->m_vNetData[0]; 
		if (data.nState <= 0
			&& (nHour < data.nHour
					||(nHour == data.nHour && data.nMinute > nMinute)))
		{
			//clock in
			CHttpNetOp op;
			op.Init(data.netData);
			bRet = op.DoGet();
			if (bRet)
			{
				bRet = CheckClockInResult(0, data.netData.strFile);
			}
			m_pCfg->m_vNetData[0].nState = bRet? 1:-1;
		}
		else
		{
			ACWriteLog(TEXT("clockIn[1] is time out"));
			m_stcInfo[0].SetWindowText(TEXT("time out"));
			m_pCfg->m_vNetData[0].nState = -1;
		}

	return bRet;
}

BOOL CAutoClockInDlg::CheckClockInResult(int nClockNum, CString& strFileIn)
{
	CStdioFile file;
	CString strRet;
	BOOL bRet = FALSE;
	BOOL bOpen = file.Open(strFileIn, CFile::modeRead, NULL);
	CTime tm(time(NULL));
	if (bOpen)
	{
		CString strBuffer;
		CString strKey1 = STR_ACRET_KEYWORD_BEGIN;
		CString strKey2 =STR_ACRET_KEYWORD_END;
		int nLen = file.GetLength();
		if (nLen > 1024*1024)
		{
			ACWriteLog(TEXT("returned file is too big.."));
			file.Close();
			return FALSE;
		}

		char* pszBuff = new char[nLen +1];
		ZeroMemory(pszBuff, nLen+1);
		file.Read(pszBuff, nLen);
		strBuffer = pszBuff;
		delete[] pszBuff;
		pszBuff = NULL;
		int nPos1 = strBuffer.Find(strKey1);
		int nPos2 = -1;
		if (nPos1 != -1)
		{
			int nPos2 =strBuffer.Find(strKey2);
			strRet = strBuffer.Mid(nPos1 + strKey1.GetLength(),nPos2- nPos1- strKey1.GetLength());
			ACWriteLog(TEXT("server returned:%s"), strRet);
			if (strRet.Find(TEXT("成功")) != -1)
			{
				bRet = TRUE;
				ACWriteLog(TEXT("ClockIn[%d] successfully at %02d:%02d"), nClockNum, tm.GetHour(), tm.GetMinute());
			}
		}
		file.Close();

		if (!bRet)
		{
			ACWriteLog(TEXT("ClockIn[%d] failed at %02d:%02d"), nClockNum, tm.GetHour(), tm.GetMinute());
		}
	}
	else
	{
		ACWriteLog(TEXT("ClockIn[%d] result file open failed at %02d:%02d"), nClockNum, tm.GetHour(), tm.GetMinute());
	}

	return bRet;
}



BOOL CAutoClockInDlg::ShowBubbleTip(CString& strTitle, CString& strTipIn, DWORD dwInfoFlag)
{
	if (m_pCfg->m_cfgData.bPopBubble)
	{
		NOTIFYICONDATA tempdata = g_trayData;
		tempdata.uFlags = NIF_INFO;
		swprintf_s(tempdata.szInfo,TEXT("%s"),   strTipIn);
		swprintf_s(tempdata.szInfoTitle, TEXT("%s"),  strTitle);
		tempdata.dwInfoFlags = dwInfoFlag;
		tempdata.uTimeout = 5000;
		Shell_NotifyIcon(NIM_MODIFY, &tempdata);
	}

	return TRUE;
}

BOOL CAutoClockInDlg::ShutDown()
{
	//ShellExecute(NULL, NULL, TEXT("shutdown"), TEXT("/s /t 30"), NULL, SW_HIDE);
	CDlgShutdown* pDlg = CDlgShutdown::CreateShutdownDlg();
	pDlg->Create(CDlgShutdown::IDD, GetDesktopWindow());
	pDlg->m_hwndNotifier = this->GetSafeHwnd();
	return TRUE;
}

void CAutoClockInDlg::OnBnClickedBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT bRet = AfxMessageBox(TEXT("Exit Auto ClockIn?"), MB_OKCANCEL);
	if(bRet == IDOK)
	{
		PostMessage(WM_CLOSE);
	}
}

HBRUSH CAutoClockInDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (m_pCfg)
		{
			for (int i = 0; i < 4; i++)
			{
				if (pWnd->GetSafeHwnd() == m_stcInfo[i].GetSafeHwnd())
				{
					COLORREF clr = GetStcColor(i);
					pDC->SetTextColor(clr);
				}
			}
		}
	}

	if (nCtlColor == CTLCOLOR_BTN)
	{
		if (pWnd->GetSafeHwnd() == m_btnAutoStart.GetSafeHwnd())
		{
			pDC->SetBkColor(m_bAutoMode? RGB(100, 100,200):RGB(200, 100, 100));
		}
	}
	
	// TODO:  在此更改 DC 的任何属性
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

COLORREF CAutoClockInDlg::GetStcColor( int nClockNum )
{
	COLORREF clr = GetSysColor(COLOR_BTNTEXT);
	if (m_pCfg->m_vNetData[nClockNum].nState ==1)
	{
		clr = RGB(0,255,0);
	}
	else if (m_pCfg->m_vNetData[nClockNum].nState == -1)
	{
		clr = RGB(255,0,0);
	}
	else if (m_pCfg->m_vNetData[nClockNum].nState == -2)
	{
		//time out
		clr = RGB(0,255,0);
	}
	return clr;
}

BOOL CAutoClockInDlg::TryClock( int nNum, BOOL bNow /*= FALSE*/)
{
	BOOL bRet = FALSE;
	CTime tm(time(NULL));
	const int nHour =tm.GetHour();
	const int nMinute = tm.GetMinute();
	static int s_nDay = -1;
	int nDay = tm.GetDay();
	if (s_nDay != nDay)
	{
		//reset state
		s_nDay = nDay;
		for (int i = 0;i < 4; i++)
		{
			m_pCfg->m_vNetData[i].nState = 0;
		}
		if (nHour < 9 && nHour >6)
		{
			ACWriteLog(TEXT("day changed, start clock1..."));
			TryClock(0, TRUE);
		}
	}

	VECCLOCKININFO& m_vData = m_pCfg->m_vNetData;
	CTimeSpan span1(0, m_vData[nNum].nHour, m_vData[nNum].nMinute, 0);
	CTimeSpan preSpan(0,0, m_nRandomMinute, 0);
	if (!bNow)
	{
		if (nNum == 0)
		{
			span1 = span1 - preSpan;
		}
		else
		{
			span1 = span1 + preSpan;
		}
	}

	CTimeSpan tmRemain(0, nHour, nMinute, 0);
	tmRemain = span1 - tmRemain;

	//
	if ((nNum == 0 && nHour >= 9)
				||((nNum ==1 || nNum == 2) && nHour >= 13))
	{
		//time out
		if (m_vData[nNum].nState == 0)
		{
			m_stcInfo[nNum].SetWindowText(TEXT("out of range"));
		}
		//ACWriteLog(TEXT("clockin[%d] is time out!"), nNum);
		m_vData[nNum].nState = m_vData[nNum].nState== -1? -1:-2;
		return bRet;
	}

	if (bNow && ((nNum == 3 && nHour < 18 )
		||((nNum ==1 || nNum == 2) && (nHour < 11 || (nHour == 11 && nMinute <30)))))
	{
		m_stcInfo[nNum].SetWindowText(TEXT("not in range"));
		return bRet;
	}

	if (m_vData[nNum].nState == 0)
	{
		CString strMsg;
		strMsg.Format(TEXT("remain %02d:%02d"), tmRemain.GetHours(), tmRemain.GetMinutes());
		m_stcInfo[nNum].SetWindowText(strMsg);
	}

	

	if(bNow || ((abs((int)tmRemain.GetTotalMinutes()) <= 1) && m_pCfg->m_vNetData[nNum].nState == 0))
	{
		//clock in
		CHttpNetOp op;
		op.Init(m_vData[nNum].netData);
		try
		{
			bRet = op.DoGet();
		}
		catch (...)
		{
			bRet = FALSE;
		}
		if (bRet)
		{
			bRet = CheckClockInResult(nNum, m_vData[nNum].netData.strFile);
		}
		m_vData[nNum].nState = bRet? 1:-1;
		m_stcInfo[nNum].SetWindowText(bRet? TEXT("successfully"):TEXT("failed"));
		CString strTitle = TEXT("clock result");
		CString strTip;
		if (!bRet)
		{
			strTip.Format(TEXT("clockIn[%d] failed!, check log for detail."), nNum);
		}
		else
		{
			strTip.Format(TEXT("clockIn[%d] successfully!"), nNum);
		}
		ShowBubbleTip(strTitle, strTip);
		if (nNum == 3 && m_pCfg->m_cfgData.bAutoShutdown)
		{
			ShutDown();
		}
	}	

	return bRet;
}

void CAutoClockInDlg::OnBnClickedCheckShowlog()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL bShowLog = m_chkShowLog.GetCheck() == BST_CHECKED? TRUE:FALSE;

	SetWindowPos(NULL, 0,0, 568, bShowLog? 568:394, SWP_NOMOVE);
}

void CAutoClockInDlg::OnBnClickedBtnTry1()
{
	// TODO: 在此添加控件通知处理程序代码
	TryClock(0, TRUE);
}

void CAutoClockInDlg::OnBnClickedBtnTry2()
{
	// TODO: 在此添加控件通知处理程序代码
	TryClock(1, TRUE);
}

void CAutoClockInDlg::OnBnClickedBtnTry3()
{
	// TODO: 在此添加控件通知处理程序代码
	TryClock(2, TRUE);
}

void CAutoClockInDlg::OnBnClickedBtnTry4()
{
	// TODO: 在此添加控件通知处理程序代码
	TryClock(3, TRUE);
}

void CAutoClockInDlg::OnBnClickedCheckAutoshutdown()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCheck = m_chkShutdown.GetCheck();
	m_chkSleep.EnableWindow(nCheck == BST_CHECKED);
}
