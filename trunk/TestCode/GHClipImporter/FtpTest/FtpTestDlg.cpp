
// FtpTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FtpTest.h"
#include "FtpTestDlg.h"
#include "afxinet.h"
#include "StrConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CFtpTestDlg �Ի���




CFtpTestDlg::CFtpTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFtpTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFtpTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFtpTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CFtpTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFtpTestDlg ��Ϣ�������

BOOL CFtpTestDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFtpTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFtpTestDlg::OnPaint()
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
HCURSOR CFtpTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFtpTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxOleInit();
	CString strFtp = TEXT("tianyx-notepc");
	CString strUser = TEXT("tyx");
	CString strPw = TEXT("tyx");
	CInternetSession *pInetSession= new CInternetSession (NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	pInetSession->SetOption(INTERNET_OPTION_CODEPAGE , CP_UTF8);
	DWORD dwVal = 0;
	pInetSession->QueryOption(INTERNET_OPTION_CODEPAGE, dwVal);
	CFtpConnection *pFtpConnection = NULL, *pFtpConnectionDest = NULL;
	try
	{
		pFtpConnection = pInetSession->GetFtpConnection(strFtp,strUser,strPw);
		pFtpConnection->SetOption(INTERNET_OPTION_CODEPAGE , CP_UTF8);
		BOOL bSuc = pFtpConnection->SetCurrentDirectory("tyx2/tyx");
		CFtpFileFind finder(pFtpConnection);
		string strKey = TEXT("*.txt");
		wstring wszKey = Ansi2W(strKey);
		string szKey = W2UTF8(wszKey);
		// start looping
		BOOL bWorking = finder.FindFile(szKey.c_str());
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			TRACE("%s\n", (LPCTSTR)finder.GetFileURL());
			string strFile = finder.GetFileURL();
			wstring strwFile = UTF82W(strFile);
			string strFilez = W2Ansi(strwFile);
			TRACE("%s\n", strFilez.c_str());
		}
		finder.Close();

	}
	catch (CInternetException* e)
	{   
		char szBuff[1024];
		e->GetErrorMessage(szBuff, 1000);
		ASSERT(FALSE);
	} 
}
