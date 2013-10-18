// DlgShutdown.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoClockIn.h"
#include "DlgShutdown.h"
#include "ACConfig.h"


CDlgShutdown* CDlgShutdown::m_spDlg = NULL;
HWND CDlgShutdown::m_hwndNotifier = NULL;
// CDlgShutdown �Ի���
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


// CDlgShutdown ��Ϣ�������

void CDlgShutdown::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgShutdown::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_spDlg == this)
	{
		m_spDlg = NULL;
	}

	CDialog::PostNcDestroy();
	delete this;
}

void CDlgShutdown::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CDlgShutdown::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
