
// ZQLicenseMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZQLicenseMgr.h"
#include "ZQLicenseMgrDlg.h"
#include "ZQGlobal.h"
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


// CZQLicenseMgrDlg �Ի���




CZQLicenseMgrDlg::CZQLicenseMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZQLicenseMgrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZQLicenseMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HOSTCODE, m_edtPCCode);
	DDX_Control(pDX, IDC_COMBO_MODULE, m_cmbModule);
	DDX_Control(pDX, IDC_STATIC_MODULE, m_stcModule);
	DDX_Control(pDX, IDC_CHK_EXPIRE_DATE, m_chkExpireDate);
	DDX_Control(pDX, IDC_EXPIRE_DATE, m_tmCtlExpireDate);
	DDX_Control(pDX, IDC_EDIT_CODERESULT, m_edtCodeResult);
	DDX_Control(pDX, IDC_CHK_TOFILE, m_chkGenToFile);
	DDX_Control(pDX, IDC_CHK_TOFILE2, m_chkToFile2);
}

BEGIN_MESSAGE_MAP(CZQLicenseMgrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_GEN, &CZQLicenseMgrDlg::OnBnClickedBtnGen)
	ON_CBN_SELCHANGE(IDC_COMBO_MODULE, &CZQLicenseMgrDlg::OnCbnSelchangeComboModule)
	ON_BN_CLICKED(IDC_BTN_RESET, &CZQLicenseMgrDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_CHK_EXPIRE_DATE, &CZQLicenseMgrDlg::OnBnClickedChkExpireDate)
	ON_BN_CLICKED(IDC_REGFROMFILE, &CZQLicenseMgrDlg::OnBnClickedRegfromfile)
END_MESSAGE_MAP()


// CZQLicenseMgrDlg ��Ϣ�������

BOOL CZQLicenseMgrDlg::OnInitDialog()
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
	for (size_t i = 0; i < g_GlobalInfo.vModuleName.size(); i++)
	{
		m_cmbModule.InsertString(i, g_GlobalInfo.vModuleName[i].c_str());
	}

	m_cmbModule.SetCurSel(0);
	m_stcModule.SetWindowText(g_GlobalInfo.vModuleKey[0].c_str());
	CTime tmNow(time(NULL));
	tmNow += CTimeSpan(1, 0,0,0);
	CTime tm1, tm2;
	m_tmCtlExpireDate.GetRange(&tm1, &tm2);
	m_tmCtlExpireDate.SetRange(&tmNow, NULL);
	tmNow += CTimeSpan(365, 0,0,0);
	m_tmCtlExpireDate.SetTime(&tmNow);
	OnBnClickedChkExpireDate();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CZQLicenseMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CZQLicenseMgrDlg::OnPaint()
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
HCURSOR CZQLicenseMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CZQLicenseMgrDlg::OnBnClickedBtnGen()
{
	BOOL bToFile = m_chkGenToFile.GetCheck() == BST_CHECKED;
	BOOL bToFile2 = m_chkToFile2.GetCheck() == BST_CHECKED;
	m_edtCodeResult.SetWindowText("");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strRegKey;
	m_edtPCCode.GetWindowText(strRegKey);
	ST_NAME name;
	name.key4 = strRegKey;
	if (!(UninitReg(name)))
	{
		ASSERT(FALSE);
		AfxMessageBox("��������Ϣ����ȷ��");
		return ;
	}

	int nSel = m_cmbModule.GetCurSel();
	if (nSel < 0 || nSel > g_GlobalInfo.vModuleKey.size())
	{
		AfxMessageBox("ģ����Ϣδѡ��");
		return ;
	}

	name.key2 = g_GlobalInfo.vModuleKey[nSel];

	CTime tm(time(NULL));
	if (m_chkExpireDate.GetCheck() == BST_CHECKED)
	{
		CTime tm2;
		m_tmCtlExpireDate.GetTime(tm2);
		CTimeSpan tmspan = tm2 -tm;
		if(tm2  < tm || tmspan.GetTotalHours() < 23)
		{
			AfxMessageBox("����ʱ�䲻��ȷ,���ٴ���1�죡");
			return ;
		}
		tm = tm2;
	}
	else
	{
		CTimeSpan tmspan(365, 0, 0, 0);
		tm +=tmspan;
	}
	CTimeSpan tmspanDelay(30, 0, 0, 0);
	tm += tmspanDelay;
	name.key3 = tm.Format("%Y-%m-%d");
	name.key4.clear();
	if (!InitZQ(name))
	{
		AfxMessageBox("ע����Ϣ����ʧ�ܣ�");
		return ;
	}
	else
	{
		m_edtCodeResult.SetWindowText(name.key4.c_str());
		if (bToFile)
		{
			CString strFile = g_GlobalInfo.szAppPath.c_str();
			strFile += TEXT("\\zqLicense.reg");
			CFile file;
			if (file.Open(strFile, CFile::modeCreate|CFile::modeWrite))
			{
				try
				{
					CString strBuff = "Windows Registry Editor Version 5.00\r\n[HKEY_LOCAL_MACHINE\\SOFTWARE\\zqvideo]\r\n";
					strBuff +="\"Licensekey0\"=\"";
					strBuff +=name.key4.c_str();
					strBuff += "\"";
					file.Write(strBuff.GetBuffer(), strBuff.GetLength());
					file.Flush();
					file.Close();
				}
				catch (CFileException* e)
				{
					AfxMessageBox("�ļ���д�쳣��");
				}
			}
			else
			{
				AfxMessageBox("ע��regע���ļ�д��ʧ�ܣ�");
			}
		}
		if (bToFile2)
		{
			CString strFile = g_GlobalInfo.szAppPath.c_str();
			strFile += TEXT("\\License.dat");
			CFile file;
			if (file.Open(strFile, CFile::modeCreate|CFile::modeWrite))
			{
				try
				{
					CString strBuff =name.key4.c_str();
					file.Write(strBuff.GetBuffer(), strBuff.GetLength());
					file.Flush();
					file.Close();
				}
				catch (CFileException* e)
				{
					AfxMessageBox("�ļ���д�쳣��");
				}
			}
			else
			{
				AfxMessageBox("License.dat�ļ�д��ʧ�ܣ�");
			}
		}

		AfxMessageBox("����ע����Ϣ�ɹ�!");
	}
}

BOOL CZQLicenseMgrDlg::LoadConfig()
{
	 return TRUE;
}

void CZQLicenseMgrDlg::OnCbnSelchangeComboModule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel =m_cmbModule.GetCurSel();
	if (nSel >= 0 && nSel < g_GlobalInfo.vModuleKey.size())
	{
		m_stcModule.SetWindowText(g_GlobalInfo.vModuleKey[nSel].c_str());
	}
}

void CZQLicenseMgrDlg::OnBnClickedBtnReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edtPCCode.SetWindowText("");
	m_edtCodeResult.SetWindowText("");
	
}


void CZQLicenseMgrDlg::OnBnClickedChkExpireDate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_tmCtlExpireDate.EnableWindow(m_chkExpireDate.GetCheck() == BST_CHECKED);
}

void CZQLicenseMgrDlg::OnBnClickedRegfromfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST,  TEXT("�����ļ�(*.dat)|*.dat||"), this);
	if(dlg.DoModal() == IDOK)
	{
		CString strFile = dlg.m_ofn.lpstrFile;
		CFile file;
		if (file.Open(strFile, CFile::modeRead))
		{
			char* szBuffer[1024];
			ZeroMemory(szBuffer, sizeof(szBuffer));
			int nLen = file.Read(szBuffer, 1024);
			if(nLen < 1024)
			{
				m_edtPCCode.SetWindowText((LPCTSTR)szBuffer);
			}
			
		}
	}
}
