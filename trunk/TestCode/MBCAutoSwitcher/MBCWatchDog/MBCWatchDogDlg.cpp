
// MBCWatchDogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MBCWatchDog.h"
#include "MBCWatchDogDlg.h"
#include "GlobalDef.h"
#include "io.h"
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDTIMER_STATECHECK 1331
#define IDTIMER_CDCOUNTING 1334
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


// CMBCWatchDogDlg 对话框




CMBCWatchDogDlg::CMBCWatchDogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMBCWatchDogDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nBackActCD = 10;
	m_bMasterDown= SWITCHSTATE_UNKNOW;
	m_bBackDown = SWITCHSTATE_UNKNOW;
}

void CMBCWatchDogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_EDIT2, m_edtMasterAddr);
	DDX_Control(pDX, IDC_EDIT3, m_edtBackPath);
	DDX_Control(pDX, IDC_EDIT4, m_edtBackAddr);
	DDX_Control(pDX, IDC_STATIC_MASTERSTATE, m_stcMasterState);
	DDX_Control(pDX, IDC_STATIC_BACKSTATE, m_stcBackState);
}

BEGIN_MESSAGE_MAP(CMBCWatchDogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CMBCWatchDogDlg::OnBnClickedBtnStart)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_QUIT, &CMBCWatchDogDlg::OnBnClickedBtnQuit)
END_MESSAGE_MAP()


// CMBCWatchDogDlg 消息处理程序

BOOL CMBCWatchDogDlg::OnInitDialog()
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
	m_edtMasterAddr.SetWindowText(Addr2String(g_GlobalInfo.addrMasterRemote).c_str());
	m_edtBackAddr.SetWindowText(Addr2String(g_GlobalInfo.addrBackRemote).c_str());
	m_edtBackPath.SetWindowText(g_GlobalInfo.szLocalWatchAppPath.c_str());
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMBCWatchDogDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMBCWatchDogDlg::OnPaint()
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
HCURSOR CMBCWatchDogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMBCWatchDogDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMasterStateCounting = 0;
	m_nBackStateCounting = 0;
	m_ObjMaster.m_addrLocal = g_GlobalInfo.addrMasterLocal;
	m_ObjMaster.m_addrRemote = g_GlobalInfo.addrMasterRemote;
	m_ObjMaster.SetGraphStateIncomingCallback(this);
	m_ObjMaster.Run();

	m_ObjBack.m_addrLocal = g_GlobalInfo.addrBackLocal;
	m_ObjBack.m_addrRemote = g_GlobalInfo.addrBackRemote;
	m_ObjBack.SetGraphStateIncomingCallback(this);
	m_ObjBack.Run();
	SetTimer(IDTIMER_CDCOUNTING, 1000, 0);
	SetTimer(IDTIMER_STATECHECK, g_GlobalInfo.nStateGetInterval, 0);
	m_btnStart.EnableWindow(FALSE);
}

HRESULT CMBCWatchDogDlg::OnRecvGraphState( CMBCWatchStateQueryObj* pObj, ST_GRAPHSTATE& gstateIn )
{
	if (pObj == &m_ObjMaster)
	{
		m_nMasterStateCounting++;
	}
	else if (pObj == &m_ObjBack)
	{
		m_nBackStateCounting++;
	}

	return TRUE;
}

void CMBCWatchDogDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//check state
	if (nIDEvent == IDTIMER_STATECHECK)
	{
		static int snTimeCount = 0;
		++snTimeCount;
		if (snTimeCount	* 1000  > g_GlobalInfo.nAutoSwitchCriticalCount)
		{
			snTimeCount = 0;
			CheckState();
		}
		
		return;
	}
	else if (nIDEvent == IDTIMER_CDCOUNTING)
	{
		if (m_nBackActCD > 0)
		{
			m_nBackActCD--;
		}
	}
	__super::OnTimer(nIDEvent);
}

void CMBCWatchDogDlg::CheckState()
{
	BOOL bDoSwitch = FALSE;
	if (m_nMasterStateCounting == 0)
	{
		m_bMasterDown = SWITCHSTATE_DOWN;
		if (m_bBackDown != SWITCHSTATE_OK)
		{
			bDoSwitch = TRUE;
			
		}
	}
	else
	{
		m_bMasterDown = SWITCHSTATE_OK;
	}
	if ( m_nBackStateCounting == 0  )
	{
		m_bBackDown = SWITCHSTATE_DOWN;
		if (m_bMasterDown !=SWITCHSTATE_OK)
		{
			bDoSwitch = TRUE;
		}
	}
	else
	{
		m_bBackDown = SWITCHSTATE_OK;
	}

	//reset counting
	m_nBackStateCounting = 0;
	m_nMasterStateCounting= 0;
	
	if (bDoSwitch && m_nBackActCD == 0)
	{
		if (_access(g_GlobalInfo.szLocalWatchAppPath.c_str(), 0) != -1)
		{
			ShellExecute(NULL, 0, g_GlobalInfo.szLocalWatchAppPath.c_str(), "-autorun", "", SW_SHOW);
		}
		//putcd
		m_nBackActCD = g_GlobalInfo.nRestartLocalAppCD/1000;
	}
	RefreshCtrl();
}

void CMBCWatchDogDlg::RefreshCtrl()
{
	CString strTmp;
	switch (m_bMasterDown)
	{
	case SWITCHSTATE_UNKNOW:
		strTmp = "未知";
		break;
	case SWITCHSTATE_OK:
		strTmp = "正常";
		break;
	case SWITCHSTATE_DOWN:
		strTmp = "断开";
		break;
	}
	
	m_stcMasterState.SetWindowText(strTmp);

	strTmp = "";
	switch (m_bBackDown)
	{
	case SWITCHSTATE_UNKNOW:
		strTmp = "未知";
		break;
	case SWITCHSTATE_OK:
		strTmp = "正常";
		break;
	case SWITCHSTATE_DOWN:
		strTmp = "断开";
		break;
	}

	m_stcBackState.SetWindowText(strTmp);

}

void CMBCWatchDogDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (AfxMessageBox("确定退出?", MB_OKCANCEL) != IDOK)
	{
		return;
	}

	KillTimer(IDTIMER_CDCOUNTING);
	KillTimer(IDTIMER_STATECHECK);
	m_ObjBack.Stop();
	m_ObjMaster.Stop();

	__super::OnClose();
}




void CMBCWatchDogDlg::OnBnClickedBtnQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_CLOSE,0,0);
}
