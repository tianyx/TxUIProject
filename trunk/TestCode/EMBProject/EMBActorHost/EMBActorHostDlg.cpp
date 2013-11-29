
// EMBActorHostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMBActorHost.h"
#include "EMBActorHostDlg.h"
#include "..\..\Plugins\EMBActor\TaskActor.h"

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
	DDX_Control(pDX, IDC_LIST_EXECUTOR, m_lstExecutor);
}

BEGIN_MESSAGE_MAP(CEMBActorHostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CEMBActorHostDlg::OnBnClickedButtonRefresh)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_LUANCHEXEC, &CEMBActorHostDlg::OnBnClickedLuanchexec)
END_MESSAGE_MAP()


// CEMBActorHostDlg 消息处理程序

BOOL CEMBActorHostDlg::OnInitDialog()
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
	InitUI();

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

void CEMBActorHostDlg::OnBnClickedButtonRefresh()
{
	CArray<tagExecutorProcess, tagExecutorProcess> arrExecutor;
	GetExecutors(arrExecutor);

	m_lstExecutor.DeleteAllItems();
	
	for (int i = 0; i < arrExecutor.GetSize(); ++i)
	{
		tagExecutorProcess tag = arrExecutor[i];

		m_lstExecutor.InsertItem(i, tag.m_strID);
		m_lstExecutor.SetItemText(i, 0, tag.m_strName);
		m_lstExecutor.SetItemText(i, 1, tag.m_strID);
		m_lstExecutor.SetItemText(i, 2, tag.m_strState);
		// 进度
		CString strTem;
		strTem.Format("%d", tag.m_nPercent);
		m_lstExecutor.SetItemText(i, 3, strTem);
		m_lstExecutor.SetItemText(i, 4, tag.m_strTaskGuid);
	}

	UpdateData(FALSE);
}

// 初始化界面信息
void CEMBActorHostDlg::InitUI()
{
	m_lstExecutor.InsertColumn(0, "名称", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(1, "标识", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(2, "状态", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(3, "执行进度", LVCFMT_LEFT, 100);
	m_lstExecutor.InsertColumn(4, "任务标识", LVCFMT_LEFT, 100);
}

void CEMBActorHostDlg::OnClose()
{
	// 提示
	if (IDNO == MessageBox("确认退出程序", "提示", MB_YESNO))
	{
		return;
	}

	CDialog::OnClose();
}

void CEMBActorHostDlg::GetExecutors( CArray<tagExecutorProcess, tagExecutorProcess>& m_arrExecutors )
{
	// 从EMBActor.dll 查询信息, 未实现 to do
	tagExecutorProcess tag;
	tag.m_strID = "test001";
	tag.m_strName = "Executor1";
	tag.m_strState = "空闲";
	tag.m_strTaskGuid = "";
	tag.m_nPercent = 100;

	m_arrExecutors.Add(tag);
}

void CEMBActorHostDlg::OnBnClickedLuanchexec()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strfile =GetAppPath().c_str();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this, 0 );
	if (dlg.DoModal() == IDOK)
	{
		strfile = dlg.GetOFN().lpstrFile;
	}

	CFile file;
	BOOL bOPen =file.Open(strfile, CFile::modeRead, NULL);
	CString strTask;
	if (bOPen)
	{
		int nLen = file.GetLength()+1;
		char* pBuff = new char[nLen];
		ZeroMemory(pBuff, nLen);
		file.Read(pBuff, file.GetLength());
		strTask = pBuff;
		file.Close();
	}

	if (g_pIActorPlugin)
	{
		EMB::CTaskActor* pActor = dynamic_cast<EMB::CTaskActor*>(g_pIActorPlugin);
		if (pActor)
		{
			CString strFile;
			DWORD dwProcessId = 0;
			CString strRet;
			pActor->OnActorConnectorMsg(strTask, strRet);
			CString strmsg;
			strmsg.Format(TEXT("excutorlaunched id = %d, processid = %d"));
		}
	}
}
