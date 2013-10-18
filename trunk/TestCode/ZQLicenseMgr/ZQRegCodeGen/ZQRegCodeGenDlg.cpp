
// ZQRegCodeGenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ZQRegCodeGen.h"
#include "ZQRegCodeGenDlg.h"
#include "TxSystemOp.h"
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


// CZQRegCodeGenDlg 对话框




CZQRegCodeGenDlg::CZQRegCodeGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZQRegCodeGenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZQRegCodeGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtReg);
	DDX_Control(pDX, IDC_STATIC_LOG, m_stcLog);
}

BEGIN_MESSAGE_MAP(CZQRegCodeGenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CZQRegCodeGenDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CZQRegCodeGenDlg 消息处理程序

BOOL CZQRegCodeGenDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CZQRegCodeGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CZQRegCodeGenDlg::OnPaint()
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
HCURSOR CZQRegCodeGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CZQRegCodeGenDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_stcLog.SetWindowText(TEXT(""));
	ST_NAME name;
	if (InitReg(name))
	{
		m_edtReg.SetWindowText(name.key4.c_str());
		CFile file;
		char szbuff[512];
		GetModuleFileName(NULL, szbuff, 512);
		CString wsPath = szbuff;
		int nPos = wsPath.ReverseFind('\\');
		CString strFile = wsPath.Mid(0, nPos);
		strFile +=TEXT("\\applyCode.dat");
		if(file.Open(strFile, CFile::modeWrite|CFile::modeCreate, NULL))
		{
			file.Write(name.key4.c_str(), name.key4.size());
			CString strMsg;
			strMsg.Format(TEXT("申请文件成功保存在:%s"), strFile);
			m_stcLog.SetWindowText(strMsg);
		}
		
	}
	else
	{
		AfxMessageBox(TEXT("生成申请码失败！"));
	}
}
