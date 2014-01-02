
// EMBClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EMBClient.h"
#include "EMBClientDlg.h"
#include "MainDef.h"
#include "StrConvert.h"
#include "EMBCommonFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDTIMER_GETSVRSTATE 1301
#define IDTIMER_GETALL		1302

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


// CEMBClientDlg 对话框




CEMBClientDlg::CEMBClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMBClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMBClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_edtLog);
	DDX_Control(pDX, IDC_EDIT_ACTORID, m_edtActorId);
	DDX_Control(pDX, IDC_EDIT_TASKGUID, m_edtTaskId);
}

BEGIN_MESSAGE_MAP(CEMBClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_GETSVRINFO, &CEMBClientDlg::OnBnClickedBtnGetsvrinfo)
	ON_BN_CLICKED(IDC_BTN_GETACTORLIST, &CEMBClientDlg::OnBnClickedBtnGetactorlist)
	ON_BN_CLICKED(IDC_BTN_GETTASKLIST, &CEMBClientDlg::OnBnClickedBtnGettasklist)
	ON_BN_CLICKED(IDC_BTN_GETACTORINFO, &CEMBClientDlg::OnBnClickedBtnGetactorinfo)
	ON_BN_CLICKED(IDC_BTN_GETTASKINFO, &CEMBClientDlg::OnBnClickedBtnGettaskinfo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CEMBClientDlg 消息处理程序

BOOL CEMBClientDlg::OnInitDialog()
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
	m_connMaster.SetScokAddr(&g_GlobalInfo.uiclientCfg.addrMain, NULL);
	m_connMaster.SetMsgProcessor(this);
	m_connSlave.SetScokAddr(&g_GlobalInfo.uiclientCfg.addrSlave, NULL);
	m_connSlave.SetMsgProcessor(this);

	HRESULT hr = m_connMaster.Run();
	if (hr != S_OK)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//hr = m_connSlave.Run();
	if (hr != S_OK)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	g_hwndLog = this->GetSafeHwnd();
	//SetTimer(IDTIMER_GETSVRSTATE, 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEMBClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMBClientDlg::OnPaint()
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
HCURSOR CEMBClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CEMBClientDlg::OnMessage(int nMaster, CString& strMsg, CString& strRet )
{
	HRESULT hr = S_OK;
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(strMsg, mainInfo);
	if (mainInfo.nType == embxmltype_svrInfo)
	{
		ST_SVRLIVEINFO livInfo;
		livInfo.FromString(strMsg);
		if (livInfo.nMaster == embSvrType_master)
		{
			m_svrInfoMaster = livInfo;
			m_svrInfoMaster.nConnState =embConnState_ok;
		}
		else if (livInfo.nMaster == embSvrType_slave)
		{
			m_svrInfoSlave = livInfo;
			m_svrInfoSlave.nConnState = embConnState_ok;
		}
	}
	else
	{

	}
	CFWriteLog(strMsg);

	return S_OK;
}

HRESULT CEMBClientDlg::OnConnStateChange( int nMaster, int nStateIn )
{
	if (nMaster == embSvrType_master)
	{
		CAutoLock lcok(&m_csLive);
		m_svrInfoMaster.nConnState = nStateIn;
	}
	else if (nMaster == embSvrType_slave)
	{
		CAutoLock lock(&m_csLive);
		m_svrInfoSlave.nConnState = nStateIn;
	}
	//refresh state ui

	return S_OK;
}


void CEMBClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDTIMER_GETSVRSTATE)
	{
		ST_SVRLIVEINFO info;
		CString strReq;
		info.ToString(strReq);
		if (m_connMaster.GetState() == MBCSTATE_OK)
		{
			m_connMaster.SendtoUISvr(strReq);
		}
		if (m_connSlave.GetState() == MBCSTATE_OK)
		{
			m_connSlave.SendtoUISvr(strReq);
		}
	}
	__super::OnTimer(nIDEvent);
}


void CEMBClientDlg::OnBnClickedBtnGetsvrinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	ST_EMBXMLMAININFO mainInfo;
	mainInfo.nType = embxmltype_svrInfo;
	CString strReq;
	mainInfo.ToString(strReq);
	CFWriteLog(TEXT(strReq));
	m_connMaster.SendtoUISvr(strReq);
	m_connSlave.SendtoUISvr(strReq);
}

LRESULT CEMBClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	MACRO_EDITLOGOUTPUT
	return __super::WindowProc(message, wParam, lParam);
}

CUIClientConnector* CEMBClientDlg::GetActiveConn()
{
	if (m_svrInfoMaster.nActive == 1 && m_svrInfoMaster.nConnState == embConnState_ok)
	{
		return &m_connMaster;
	}

	if (m_svrInfoSlave.nActive == 1 && m_svrInfoSlave.nConnState == embConnState_ok)
	{
		return &m_connSlave;
	}

	return NULL;
}

HRESULT CEMBClientDlg::SendToUISvr( CString& strMsg, BOOL bSendAll /*= FALSE*/ )
{
	CFWriteLog(strMsg);
	HRESULT hr = S_FALSE;
	if (bSendAll)
	{
		HRESULT hr1 =m_connMaster.SendtoUISvr(strMsg);
		HRESULT hr2 =m_connSlave.SendtoUISvr(strMsg);
		hr = (hr1==S_OK && hr2 == S_OK)? S_OK:E_FAIL;
	}
	else
	{
		CUIClientConnector* pConn = GetActiveConn();
		if (pConn)
		{
			hr = pConn->SendtoUISvr(strMsg);
		}
	}

	return hr;
}

void CEMBClientDlg::OnBnClickedBtnGetactorlist()
{
	// TODO: 在此添加控件通知处理程序代码
	ST_EMBXMLMAININFO mainInfo;
	mainInfo.nType = embxmltype_actorList;
	mainInfo.ver = 1;
	CString strReq;
	mainInfo.ToString(strReq);
	SendToUISvr(strReq);
}

void CEMBClientDlg::OnBnClickedBtnGettasklist()
{
	// TODO: 在此添加控件通知处理程序代码
	ST_EMBXMLMAININFO mainInfo;
	mainInfo.nType = embxmltype_taskList;
	mainInfo.ver = 1;
	CString strReq;
	mainInfo.ToString(strReq);
	SendToUISvr(strReq);
}

void CEMBClientDlg::OnBnClickedBtnGetactorinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strId;
	m_edtActorId.GetWindowText(strId);
	if (strId.IsEmpty())
	{
		return;
	}
	CTxStrConvert val(strId);
	ST_ACTORSTATE info;
	info.actorId = val.GetAsInt(-1);
	CString strReq;
	info.ToString(strReq);
	SendToUISvr(strReq);

}

void CEMBClientDlg::OnBnClickedBtnGettaskinfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strId;
	m_edtTaskId.GetWindowText(strId);
	if (strId.IsEmpty())
	{
		return;
	}
	CTxStrConvert val(strId);
	ST_TASKRUNSTATE info;
	info.guid = String2Guid(strId);
	CString strReq;
	info.ToString(strReq,TRUE);
	SendToUISvr(strReq);
}

void CEMBClientDlg::OnDestroy()
{
	m_connMaster.Stop();
	m_connSlave.Stop();
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
