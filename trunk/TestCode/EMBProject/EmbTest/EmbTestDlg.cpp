
// EmbTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmbTest.h"
#include "EmbTestDlg.h"
#include "TxParamString.h"
#include "EmbStructDef.h"

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


// CEmbTestDlg �Ի���




CEmbTestDlg::CEmbTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmbTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEmbTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEmbTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CEmbTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CEmbTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CEmbTestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CEmbTestDlg ��Ϣ�������

BOOL CEmbTestDlg::OnInitDialog()
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

void CEmbTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEmbTestDlg::OnPaint()
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
HCURSOR CEmbTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEmbTestDlg::OnBnClickedButton1()
{
	CTxParamString str(TEXT("<edoc_main bbb=\"10\"></edoc_main>"));
	str.GoIntoKey(TEXT("edoc_main"));
	CTxStrConvert val;
	val.SetVal(10);
	str.SetAttribVal(NULL, "aaa", val);
	//str.SetAttribVal(NULL, "bbb", val);
	str.UpdateData();
	CString strRet = str;
	CTxParamString test(strRet);
	test.GoIntoKey("edoc_main");
	strRet = test.GetAttribVal(NULL, "aaa").GetAsString();
	strRet = test.GetAttribVal(NULL, "bbb").GetAsString();
	AfxMessageBox(strRet);


	CFile file;
	file.Open("f:\\dd.xml", CFile::modeRead, NULL);
	char szbuff[2048];
	ZeroMemory(szbuff, 2048);
	file.Read(szbuff, 2048);
	file.Close();
	CTxParamString txStr(szbuff);
	int nId = txStr.GetAttribVal(TEXT(""), TEXT("ID")).GetAsInt();
	txStr.GoToPath(TEXT(".\\SpotListInfo"));
	CString strDate = txStr.GetElemVal(TEXT("Date")).GetAsString();
	txStr.SetElemVal(TEXT("Date"), CTxStrConvert(TEXT("2013-11-11")));
	txStr.SetAttribVal(TEXT("Date"), TEXT("DD"), CTxStrConvert("testAttr"));
	txStr.UpdateData();

}

void CEmbTestDlg::OnBnClickedButton2()
{
	ST_TASKREPORT report;
	CString strREt;
	report.ToString(strREt);


	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strParm;
	strParm = TEXT("\"f:\\111\\*.*\" \"f:\\222\\*.*\" /S /C /Y");
	HINSTANCE nRet = ShellExecute(NULL, 0, "xcopy", strParm, NULL, SW_HIDE);
	if ((int)nRet < 32)
	{
		ASSERT(FALSE);
	}
}

typedef int (__cdecl *GETPLUGININSTANCE)(LPVOID& pInterface);


void CEmbTestDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HMODULE hModuleOut = NULL;
	CString strFile = TEXT("TestWorkString.dll");
	hModuleOut = LoadLibrary(strFile);
	EMB::ITaskWorkerCallInterface * pInterfaceOut = NULL;
	if (hModuleOut != NULL)
	{
		GETPLUGININSTANCE  pFun = (GETPLUGININSTANCE)GetProcAddress(hModuleOut, "TestWorkString");
		if (pFun)
		{
			pFun((LPVOID&)pInterfaceOut);
			if (pInterfaceOut)
			{
				CEMBWorkString str;
				pInterfaceOut->GetTaskProgress(str);
			}
		}
		else
		{
			FreeLibrary(hModuleOut);
			hModuleOut = NULL;
		}

	}
}
