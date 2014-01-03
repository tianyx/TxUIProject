
// EMBServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMBServer.h"
#include "EMBServerDlg.h"
#include "EmbStructDef.h"
#include "MainDef.h"
#include "TxParamString.h"
#include "Resource.h"
#include "ADOCtrl.h"
#include "IEMBBaseInterface.h"
#include "TxAutoComPtr.h"
#include "GDIDrawFunc.h"
#include "TxImageLoader.h"
using namespace EMB;
using namespace Gdiplus;

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


// CEMBServerDlg 对话框




CEMBServerDlg::CEMBServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMBServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bRunning = FALSE;
}

void CEMBServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Actor, m_actorList);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
}

BEGIN_MESSAGE_MAP(CEMBServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CEMBServerDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTNTEST, &CEMBServerDlg::OnBnClickedBtntest)
	ON_BN_CLICKED(IDC_BUTTON_XML, &CEMBServerDlg::OnBnClickedButtonXml)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CEMBServerDlg 消息处理程序

BOOL CEMBServerDlg::OnInitDialog()
{
	m_btnStart.LoadBitmap(TEXT("btnstart.png"), TXBMP_STRETCH_NONE);
	m_btnStart.SetForceRedrawParentBk(TRUE, this);
	m_pbmpBack = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("back.bmp"));
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

	AfxOleInit();       //为调用ADO数据库

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEMBServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMBServerDlg::OnPaint()
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
HCURSOR CEMBServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
*Description：手动启动服务按钮
*Input Param：
*Return Param：返回成功或失败
*History：
*/
void CEMBServerDlg::OnBnClickedBtnStart()
{
	/*CADOCtrl dbCtrl;
	dbCtrl.SetODBCDatabase("Provider=SQLOLEDB; Server=ZHOU-LIANG; Database=Dbas_henyang; uid=sa; pwd=123;");
	if(!dbCtrl.OpenDB())
	{
		return ;
	}*/
	// TODO: 在此添加控件通知处理程序代码
	if (m_bRunning)
	{
		RunServer(FALSE);
		m_bRunning = !m_bRunning;

	}
	else
	{
		m_bRunning = RunServer(TRUE);
	}
	GetDlgItem(IDC_BTN_START)->SetWindowText(m_bRunning? TEXT("Stop"):TEXT("Start"));
}

/*
*Description：手动测试添加一条任务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
void CEMBServerDlg::OnBnClickedBtntest()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strfile;
	UpdateData();
	GetDlgItemText(IDC_EDIT_TESTXML, strfile);

	if (strfile.IsEmpty())
	{
		MessageBox("请选择测试xml文件");
		return;
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
		file.Close();
		strTask = pBuff;
		delete[] pBuff;
		EMB::IPluginTaskCommit* pIcall = dynamic_cast<EMB::IPluginTaskCommit* >(g_GlobalInfo.vPlugins[2].pIface);
		if (pIcall)
		{
			CString strRet;
			pIcall->SubmitTask(strTask, strRet);
		}
	}


}

void CEMBServerDlg::InitUI()
{
	CString strfile =GetAppPath().c_str();
	strfile += TEXT("\\testTask.xml");

	SetDlgItemText(IDC_EDIT_TESTXML, strfile);
	UpdateData(FALSE);

	// 初始化actor列表
	m_actorList.InsertColumn(0, "Actor标识", LVCFMT_LEFT, 100);
	m_actorList.InsertColumn(1, "状态", LVCFMT_LEFT, 100);
	m_actorList.InsertColumn(2, "计算机名", LVCFMT_LEFT, 100);

	// 启动timer
	this->SetTimer(1001, 3000, NULL); // 间隔3秒
}

void CEMBServerDlg::OnBnClickedButtonXml()
{
	// 选择 XML File
	CFileDialog xmlFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Xml File (*.xml)|*.xml"), (CWnd *)this, 0);

	if (IDOK == xmlFileDlg.DoModal() )
	{
		CString xmlFile = xmlFileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_TESTXML, xmlFile);
		UpdateData(FALSE);
	}
}

// 刷新Actor列表
void CEMBServerDlg::RefreshActorList()
{
	m_actorList.DeleteAllItems();

	if (!m_bRunning) // 未运行时
	{
		return;
	}

	if (NULL == g_pTaskDispatchMgr)
	{
		return;
	}

	CTxAutoComPtr<IServerUI> ptrServerUI;

	if (S_OK == g_pTaskDispatchMgr->QueryInterface(GuidEMBServer_IUI, (LPVOID&)(*&ptrServerUI)))
	{
		vector<CString> vInfor;
		ptrServerUI->GetActors(vInfor);

		CString strTem;

		for (int i = 0; i < vInfor.size(); ++i)
		{
			ST_ACTORSTATE st;
			st.FromString(vInfor[i]);

			strTem.Format("%d", i);
			m_actorList.InsertItem(i, strTem);

			// actorid
			strTem.Format("%d", st.actorId);
			m_actorList.SetItemText(i, 0, strTem);
			// 连接状态
			m_actorList.SetItemText(i, 1, "运行");
			// 计算机名
			m_actorList.SetItemText(i, 2, st.strPcName);
		}
	}
	
}

void CEMBServerDlg::OnTimer( UINT_PTR nIDEvent )
{
	if (1001 == nIDEvent)
	{
		RefreshActorList();
	}

	CDialog::OnTimer(nIDEvent);
}

void CEMBServerDlg::OnClose()
{
	// 提示
	if (IDNO == MessageBox("确认退出程序?", "提示", MB_YESNO))
	{
		return;
	}

	CDialog::OnClose();
}

BOOL CEMBServerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pbmpBack)
	{
		TRACE(TEXT("\n dlg back redraw"));
		CRect rcThis;
		GetClientRect(rcThis);
		CRect rcEdge(5,5,5,5);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpBack, rcThis, NULL, TXBMP_STRETCH_MID_LRTB, NULL, NULL);
	}
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CEMBServerDlg::GetParentBack( CDC* pDc )
{
	OnEraseBkgnd(pDc);
}
