
// EMBActorHostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMBActorHost.h"
#include "EMBActorHostDlg.h"

#include "GDIDrawFunc.h"
#include "TxImageLoader.h"


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


// CEMBActorHostDlg 对话框




CEMBActorHostDlg::CEMBActorHostDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEMBActorHostDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMBActorHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEMBActorHostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CLOSE()

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CEMBActorHostDlg 消息处理程序

BOOL CEMBActorHostDlg::OnInitDialog()
{
	m_pbmpBack = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("back.bmp"));
	m_pbmpLogo2 = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("logo2.png"));
	m_pbmpSublogo = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("sublogoactor.png"));

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
	m_dlgtest.Create(IDD_DLG_TEST, this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEMBActorHostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMBActorHostDlg::OnPaint()
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
HCURSOR CEMBActorHostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEMBActorHostDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CEMBActorHostDlg::OnClose()
{
	// 提示
	if (IDNO == MessageBox("确认退出程序?", "提示", MB_YESNO))
	{
		return;
	}

	CDialog::OnClose();
}
BOOL CEMBActorHostDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_pbmpBack)
	{
		//TRACE(TEXT("\n dlg back redraw"));
		CRect rcThis;
		GetClientRect(rcThis);
		CRect rcEdge(5,5,5,5);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpBack, rcThis, NULL, TXBMP_STRETCH_MID_LRTB, NULL, NULL);
		CRect rclogo2(0,0, m_pbmpLogo2->GetWidth(), m_pbmpLogo2->GetHeight());
		rclogo2.MoveToXY(0, (rcThis.Height() - rclogo2.Height())/2);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpLogo2, rclogo2, NULL, TXBMP_STRETCH_NONE, NULL, NULL);
		CRect rcsublogo(0,0,100,40);
		rcsublogo.MoveToXY(rclogo2.left +rclogo2.Width() - 100, rclogo2.bottom - 25);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpSublogo, rcsublogo, NULL, TXBMP_STRETCH_NONE, NULL, NULL);
		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

BOOL CEMBActorHostDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE ||  pMsg->wParam == VK_F1
			||pMsg ->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_F11 && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
			&& ((GetKeyState(VK_LSHIFT) & 0x8000) != 0))
		{
			static BOOL  bShowDbg = TRUE;
			bShowDbg = !bShowDbg;
			//show debug
			HWND hwndCnol =GetConsoleWindow();
			if (hwndCnol)
			{
				::ShowWindow(hwndCnol, bShowDbg? SW_SHOW:SW_HIDE);

			}
			m_dlgtest.ShowWindow(bShowDbg? SW_SHOW:SW_HIDE);

		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
