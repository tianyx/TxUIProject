
// TS2ASISvrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TS2ASISvr.h"
#include "TS2ASISvrDlg.h"
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDTIMER_CHECKSRCOUTRATE 4001

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CTS2ASISvrDlg �Ի���




CTS2ASISvrDlg::CTS2ASISvrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTS2ASISvrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTS2ASISvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DEVTYPE, m_edtDevType);
	DDX_Control(pDX, IDC_EDIT_TSRATE, m_edtASIoutRateBps);
	DDX_Control(pDX, IDC_EDIT_PORT, m_edtOutPort);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_BTNSTART, m_btnStart);
	DDX_Control(pDX, IDC_BTNSTOP, m_btnStop);
	DDX_Control(pDX, IDC_CHK_AUTOADJUSTOUTPBS, m_chkASIAutoAdjustoutRate);
	DDX_Control(pDX, IDC_STATIC_ASISTATE_INPUTRATE, m_stcASIInRate);
	DDX_Control(pDX, IDC_STATIC_ASISTATE_OUTPUTRATE, m_stcASIOutRate);
	DDX_Control(pDX, IDC_LIST_TSADDR, m_lstSrcAddr);
	DDX_Control(pDX, IDC_CHK_MONSRCOUTBPS, m_chkSrcMonOutRate);
	DDX_Control(pDX, IDC_BTN_OUTBPSAPPLY, m_btnApplyASIOutbps);
}

BEGIN_MESSAGE_MAP(CTS2ASISvrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTNSTART, &CTS2ASISvrDlg::OnBnClickedBtnstart)
	ON_BN_CLICKED(IDC_BTNSTOP, &CTS2ASISvrDlg::OnBnClickedBtnstop)
	ON_BN_CLICKED(IDC_BTN_OUTBPSAPPLY, &CTS2ASISvrDlg::OnBnClickedBtnOutbpsapply)
	ON_BN_CLICKED(IDC_CHK_AUTOADJUSTOUTPBS, &CTS2ASISvrDlg::OnBnClickedChkAutoadjustoutpbs)
	ON_BN_CLICKED(IDC_CHK_MONSRCOUTBPS, &CTS2ASISvrDlg::OnBnClickedChkMonsrcoutbps)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTS2ASISvrDlg ��Ϣ�������

BOOL CTS2ASISvrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	g_hwndLog = this->GetSafeHwnd();
	m_nDevPort = 1;
	m_nDevType = 2144;
	m_nTSRatebps = 2000000;
	
	CString strTmp;
	strTmp.Format(TEXT("%d"), m_nDevType);
	m_edtDevType.SetWindowText(strTmp);
	strTmp.Format(TEXT("%d"), m_nDevPort);
	m_edtOutPort.SetWindowText(strTmp);
	strTmp.Format(TEXT("%d"), m_nTSRatebps);
	m_edtASIoutRateBps.SetWindowText(strTmp);
	m_chkASIAutoAdjustoutRate.SetCheck(BST_CHECKED);
	m_chkSrcMonOutRate.SetCheck(BST_CHECKED);

	m_lstSrcAddr.SetExtendedStyle(m_lstSrcAddr.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CRect rcLst;
	m_lstSrcAddr.GetClientRect(rcLst);
	m_lstSrcAddr.InsertColumn(0, TEXT("multicast address"), LVCFMT_LEFT, 250);
	m_lstSrcAddr.InsertColumn(1, TEXT("output bps"),  LVCFMT_LEFT,rcLst.Width() - 250);
	
	for (size_t i = 0; i < g_GlobalInfo.vtsAddr.size(); ++i)
	{
		ST_TSADDR tmpAddr = g_GlobalInfo.vtsAddr[i];
		CString strAddr;
		strAddr.Format(TEXT("udp://%s:%d"),tmpAddr.ip.c_str(), tmpAddr.nPort);
		m_lstSrcAddr.InsertItem(0, strAddr);
		m_lstSrcAddr.SetItemText(i, 1, TEXT("--"));
	}

	m_btnStop.EnableWindow(TRUE);
	m_btnApplyASIOutbps.ShowWindow(SW_HIDE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTS2ASISvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTS2ASISvrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTS2ASISvrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTS2ASISvrDlg::OnBnClickedBtnstart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#ifdef _DEBUG
	g_GlobalInfo.vtsAddr.clear();
	ST_TSADDR addr;
	addr.ip = TEXT("236.1.1.1");
	addr.nPort = 10001;
	g_GlobalInfo.vtsAddr.push_back(addr);
	addr.ip = TEXT("236.1.1.2");
	addr.nPort = 10002;
	g_GlobalInfo.vtsAddr.push_back(addr);
#endif // _DEBUG
	
	ST_DEVPARAM devParam;
	CString strTmp;
	m_edtDevType.GetWindowText(strTmp);
	int nDevType = atoi(strTmp);
	if (nDevType == 0)
	{
		AfxMessageBox(TEXT("dev type error"));
		return;
	}
	devParam.nDevType = nDevType;


	strTmp.Empty();
	m_edtOutPort.GetWindowText(strTmp);
	int nPort = atoi(strTmp);
	if (nPort <1 || nPort > 4)
	{
		AfxMessageBox(TEXT("dev port error, must 1 to 4"));
		return;
	}
	devParam.nPort = nPort;

	strTmp.Empty();
	m_edtASIoutRateBps.GetWindowText(strTmp);
	int nbps = atoi(strTmp);
	if (nbps < 10000 || nbps > 32000000)
	{
		AfxMessageBox(TEXT("ASI out bps is not valid"));
		return;
	}
	devParam.nOutTsBps = nbps;

	devParam.bAutoAdjustOutTsBps = m_chkASIAutoAdjustoutRate.GetCheck()== BST_CHECKED? 1:0;

	m_btnStart.EnableWindow(FALSE);

	HRESULT hr = m_builder.InitGraph(devParam);
	if (hr != S_OK)
	{
		if (hr == TRT_READYINITED)
		{
			CFWriteLog(TEXT("�Ѿ���ʼ��, code = %d"), hr);

		}
		else
		{
			CFWriteLog(TEXT("��ʼ��ʧ��, code = %d"), hr);
		}
	}
	else
	{
		CFWriteLog((TEXT("��ʼ���ɹ�...")));
	}

	if (!m_builder.m_gfBuilder.Run())
	{
		CFWriteLog(TEXT("����ʧ��"));
		m_builder.UnInitGraph();
		m_btnStart.EnableWindow(TRUE);

	}
	else
	{
		CFWriteLog((TEXT("Running...")));
		m_btnStart.EnableWindow(FALSE);
		m_btnStop.EnableWindow(TRUE);
		m_btnApplyASIOutbps.ShowWindow(SW_SHOW);
		SetTimer(IDTIMER_CHECKSRCOUTRATE, 1000, 0);


	}


}

LRESULT CTS2ASISvrDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	MACRO_EDITLOGOUTPUT
	return CDialog::WindowProc(message, wParam, lParam);
}

void CTS2ASISvrDlg::OnBnClickedBtnstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(IDTIMER_CHECKSRCOUTRATE);
	 HRESULT hr = m_builder.UnInitGraph();
	if (hr != S_OK)
	{
		CFWriteLog(TEXT("�ͷ�ʧ��"));
	}
	else
	{
		CFWriteLog(TEXT("�ͷųɹ�"));
		m_btnStart.EnableWindow(TRUE);
		m_btnApplyASIOutbps.ShowWindow(SW_HIDE);
	}

}

void CTS2ASISvrDlg::OnBnClickedBtnOutbpsapply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_builder.m_bInited)
	{
		return;
	}

	CString strTmp;
	m_edtASIoutRateBps.GetWindowText(strTmp);
	int nbps = atoi(strTmp);
	if (nbps < 10000 || nbps > 32000000)
	{
		AfxMessageBox(TEXT("ASI out bps is not valid"));
		return;
	}

	HRESULT hr = m_builder.SetASIAutoAdjustOutBps(nbps);
}

void CTS2ASISvrDlg::OnBnClickedChkAutoadjustoutpbs()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HRESULT hr = m_builder.SetASIAutoAdjustOutBps(m_chkASIAutoAdjustoutRate.GetCheck()==BST_CHECKED? 1:0);
	if (hr != S_OK)
	{
		CFWriteLog(TEXT("set ASI adjust Out rate err = %d"), hr);
	}
}

void CTS2ASISvrDlg::OnBnClickedChkMonsrcoutbps()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_chkASIAutoAdjustoutRate.GetCheck() == BST_CHECKED)
	{
		SetTimer(IDTIMER_CHECKSRCOUTRATE, 1000, 0);
	}
	else
	{
		KillTimer(IDTIMER_CHECKSRCOUTRATE);
	}
}

void CTS2ASISvrDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == IDTIMER_CHECKSRCOUTRATE)
	{
		VECSRCOUTBPS vout;
		m_builder.GetSrcRenderBps(vout);
		ASSERT(vout.size() == g_GlobalInfo.vtsAddr.size());
		m_lstSrcAddr.SetRedraw(FALSE);
		for (size_t i = 0; i < vout.size(); ++i)
		{

			CString strBps;
			if (vout[i] < 0)
			{
				strBps = TEXT("unknow");
			}
			else
			{
				strBps.Format(TEXT("%.3fMbps"), (double)vout[i]/(1024.0*1024.0));
			}
			m_lstSrcAddr.SetItemText(i, 1, strBps);
		}
		m_lstSrcAddr.SetRedraw(TRUE);
	}
	CDialog::OnTimer(nIDEvent);
}
