// DlgShutdown.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoClockIn.h"
#include "DlgShutdown.h"
#include "ACConfig.h"


CDlgShutdown* CDlgShutdown::m_spDlg = NULL;
HWND CDlgShutdown::m_hwndNotifier = NULL;
// CDlgShutdown 对话框
#define ID_TIMER_SHUTDOWN 1101
IMPLEMENT_DYNAMIC(CDlgShutdown, CDialog)

CDlgShutdown::CDlgShutdown(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShutdown::IDD, pParent)
{
	m_nCountDown = 30;
}

CDlgShutdown::~CDlgShutdown()
{
}

void CDlgShutdown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_stcInfo);
}


BEGIN_MESSAGE_MAP(CDlgShutdown, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CDlgShutdown::OnBnClickedCancel)
	ON_BN_CLICKED(IDDONOW, &CDlgShutdown::OnBnClickedDonow)
END_MESSAGE_MAP()


// CDlgShutdown 消息处理程序

void CDlgShutdown::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (ID_TIMER_SHUTDOWN == nIDEvent)
	{
		m_nCountDown--;
		if (m_nCountDown <= 0)
		{
			OnBnClickedDonow();
			return;
		}
		else
		{
			
			CString strMsg;
			if (bSleep)
			{
				strMsg.Format(TEXT("Sleep remain : %d"), m_nCountDown);

			}
			else
			{
				strMsg.Format(TEXT("Shutdown remain : %d"), m_nCountDown);
			}

			m_stcInfo.SetWindowText(strMsg);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CDlgShutdown::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = -32;
	m_font.CreateFontIndirect(&lf);

	m_stcInfo.SetFont(&m_font);

	CenterWindow();
	SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
	SetTimer(ID_TIMER_SHUTDOWN, 1000, NULL);
	CString strMsg;
	strMsg.Format(TEXT("Shutdown remain : %d"), m_nCountDown);
	m_stcInfo.SetWindowText(strMsg);

	CACConfig* m_pCfg = CACConfig::GetACConfig();
	bSleep = m_pCfg->m_cfgData.bSleepPC;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgShutdown::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_spDlg == this)
	{
		m_spDlg = NULL;
	}

	CDialog::PostNcDestroy();
	delete this;
}

void CDlgShutdown::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgShutdown::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();
	//CDialog::OnCancel();
}

CDlgShutdown* CDlgShutdown::CreateShutdownDlg()
{
	if (m_spDlg)
	{
		if (m_spDlg->GetSafeHwnd())
		{
			m_spDlg->DestroyWindow();
		}
		else
		{
			delete m_spDlg;
		}
	}
	
	m_spDlg = new CDlgShutdown;

	return m_spDlg;
}

void CDlgShutdown::OnBnClickedDonow()
{
	// TODO: 在此添加控件通知处理程序代码

	KillTimer(ID_TIMER_SHUTDOWN);
	if (bSleep)
	{
		ACWriteLog(TEXT("send shutdown msg..."));
		::SendMessage(m_hwndNotifier,MSG_SHUTDOWN, 0,0);
		OnCancel();
		return;
	}
	CString strParm =TEXT("/s /t 1");
	ShellExecute(NULL, NULL, TEXT("shutdown"), strParm, NULL, SW_HIDE);
}
