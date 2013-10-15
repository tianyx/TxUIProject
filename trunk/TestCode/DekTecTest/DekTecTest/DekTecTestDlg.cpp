
// DekTecTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DekTecTest.h"
#include "DekTecTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE m_readlock0 = CreateSemaphore(NULL, 1, 1, TEXT("m_readlock0"));
HANDLE m_outLock0 = CreateSemaphore(NULL, 0, 1, TEXT("m_outLock0"));
HANDLE m_readlock1 = CreateSemaphore(NULL, 1, 1, TEXT("m_readlock1"));
HANDLE m_outlock1 = CreateSemaphore(NULL, 0, 1, TEXT("m_outlock1"));


int g_nFifoSize = 0;
int g_nCurrReadBuffId = 0;
int g_nCurrWriteBuffId = 0;

char g_szFile[1024] = {0};

#define MAXDTABUFFLEN 0x02000000
#define WRITEBUFSIZE 0x10000
#define INITLOADSIZE 0x01000000

BOOL g_bRun = TRUE;

struct ST_DTABUFF 
{
	byte buffer[MAXDTABUFFLEN];
	int nUsed;
};


byte g_szWriteTmp[WRITEBUFSIZE];

ST_DTABUFF g_buff0;
ST_DTABUFF g_buff1;

DWORD __stdcall ReadFileProc(void* lparam);
DWORD __stdcall OutPutProc(void* lparam);

DWORD __stdcall ReadFileProc(void* lparam)
{
	DTAPI_RESULT hr = 0;
	while(g_bRun)
	{

		if (strlen(g_szFile) == 0 )
		{
			Sleep(100);
			continue;
		}
		//check if file changed.
		static CString sStrCurrFile;
		static LONGLONG snFileReadedPos = 0;
		static LONGLONG snFileLen = 0;
		if (sStrCurrFile.CompareNoCase(g_szFile) != 0)
		{
			//file changed
			sStrCurrFile = g_szFile;
			snFileReadedPos = 0;
			snFileLen = 0;
		}
		else
		{
			if (snFileLen <= snFileReadedPos)
			{
				g_szFile[0] = '\0';
				snFileReadedPos = 0;
				snFileLen = 0;
				Sleep(100);
				continue;
			}
		}



		CFile file;
		if(file.Open(sStrCurrFile, CFile::modeRead|CFile::shareDenyWrite, NULL))
		{
			while(TRUE)
			{
				HANDLE pLock =g_nCurrReadBuffId == 0? m_readlock0 : m_readlock1;
				HANDLE poutLock =g_nCurrReadBuffId == 0? m_outLock0 : m_outlock1;
				WaitForSingleObject(pLock, INFINITE);
				//clean buffer
				ST_DTABUFF& fBuffer = g_nCurrReadBuffId == 0? g_buff0:g_buff1;

				TRACE("\n read loop start, buff id =%d", g_nCurrReadBuffId);
				if (snFileReadedPos == 0)
				{
					snFileLen = file.GetLength();
				}
				else
				{
					file.Seek(snFileReadedPos, CFile::begin);
				}
				//read
				fBuffer.nUsed = file.Read(fBuffer.buffer, MAXDTABUFFLEN);
				TRACE("\n read num = %d", fBuffer.nUsed);
				snFileReadedPos += fBuffer.nUsed;
				TRACE("\n read loop end, buff id =%d",g_nCurrReadBuffId);
				LONG nPreLockCount = 0;
				ReleaseSemaphore(poutLock, 1, &nPreLockCount);
				g_nCurrReadBuffId = (++g_nCurrReadBuffId)%2;
				if (snFileReadedPos >= snFileLen)
				{
					
					TRACE("\n file read finished size = 0x%x",snFileLen);
					break;
				}

			}
			file.Close();
			
		}
		else
		{
			ASSERT(FALSE);
			sStrCurrFile = TEXT("");
			snFileLen = 0;
			snFileReadedPos = 0;
		}
		
	}
	TRACE("\n read thread quit");
	return 0;
}

DWORD __stdcall OutPutProc(void* lparam)
{
	DTAPI_RESULT hr = 0;
	int snInitLoaded = 0;

	DtOutpChannel* ptsOut = (DtOutpChannel*)lparam;
	while(g_bRun)
	{
		HANDLE pLock =g_nCurrWriteBuffId == 0? m_readlock0 : m_readlock1;
		HANDLE poutLock =g_nCurrWriteBuffId == 0? m_outLock0 : m_outlock1;
		WaitForSingleObject(poutLock, INFINITE);
		TRACE("\n --output loop start buff id =%d", g_nCurrWriteBuffId);
		ST_DTABUFF& fBuffer = g_nCurrReadBuffId == 0? g_buff0:g_buff1;
	
		if (snInitLoaded < INITLOADSIZE)
		{
			hr = ptsOut->SetTxControl(DTAPI_TXCTRL_HOLD);
			ASSERT(hr == DTAPI_OK);
		}
		int nReaded = 0;
		byte *pPos = fBuffer.buffer;
		INT64 nRemain = fBuffer.nUsed;
		int nCtrlState = 0;
		hr = ptsOut->GetTxControl(nCtrlState);
		ASSERT(hr == DTAPI_OK);

		while(nRemain > 0)
		{
			int nLoad = 0;
			hr = ptsOut->GetFifoLoad(nLoad);
			//TRACE("\nfifo load = 0x%x", nLoad);
			ASSERT(hr == DTAPI_OK);
			while(nLoad >= g_nFifoSize - 0x20000)
			{
				Sleep(1000);
				hr = ptsOut->GetFifoLoad(nLoad);
				//TRACE("\ncheck fifo load = 0x%x", nLoad);
				ASSERT(hr == DTAPI_OK);
			}
			
			int nReadOnce = nRemain < WRITEBUFSIZE? fBuffer.nUsed: WRITEBUFSIZE;
			static INT64 snTotalOutput = 0;
			snTotalOutput += nReadOnce;
			//TRACE("\ntotal output = 0x%x", snTotalOutput);
			hr = ptsOut->Write((char*)pPos, nReadOnce);
			ASSERT(hr == DTAPI_OK);
			pPos += nReadOnce;
			nRemain -= nReadOnce;
			snInitLoaded +=nReadOnce;
			hr = ptsOut->GetTxControl(nCtrlState);
			ASSERT(hr == DTAPI_OK);
			if (nCtrlState == DTAPI_TXCTRL_HOLD && snInitLoaded >= INITLOADSIZE)
			{
				hr = ptsOut->SetTxControl(DTAPI_TXCTRL_SEND);
				TRACE("\n --set ctrl to DTAPI_TXCTRL_SEND");
				ASSERT(hr == DTAPI_OK);
			}
		}

		fBuffer.nUsed = 0;
		TRACE("\n --output loop end buff id =%d", g_nCurrWriteBuffId);
		g_nCurrWriteBuffId = (++g_nCurrWriteBuffId)%2;
		LONG nPreLockCount = 0;
		ReleaseSemaphore(pLock, 1, &nPreLockCount);
		
	}

	TRACE("\n write thread quit");
	return 0;
}




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


// CDekTecTestDlg �Ի���




CDekTecTestDlg::CDekTecTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDekTecTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	

	m_hDtaThrd = NULL;
	m_hReadThrd = NULL;
	m_hDtaThrd = NULL;
}

void CDekTecTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDekTecTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDekTecTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_SELECTFILE, &CDekTecTestDlg::OnBnClickedBtnSelectfile)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDekTecTestDlg ��Ϣ�������

BOOL CDekTecTestDlg::OnInitDialog()
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

void CDekTecTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDekTecTestDlg::OnPaint()
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
HCURSOR CDekTecTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDekTecTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	DWORD dwRet =  WaitForSingleObject(m_outLock0, INFINITE);
// 	dwRet =  WaitForSingleObject(m_outLock0, INFINITE);

	int nRet =InitDtaDevice();
	ASSERT(nRet==0);
	m_hDtaThrd = CreateThread(NULL, 0, ReadFileProc, 0, 0, 0);
	m_hDtaThrd = CreateThread(NULL, 0, OutPutProc, (LPVOID)&m_tsOut,0, 0);
}

int CDekTecTestDlg::InitDtaDevice()
{
	DTAPI_RESULT hr = 0;
	if (m_dvc.AttachToType(2144) != DTAPI_OK)
	{
		ASSERT(FALSE);
		return -3;
	}

	if (m_tsOut.AttachToPort(&m_dvc, 1) != DTAPI_OK)
	{
		ASSERT(FALSE);
		return -4;
	}
	//m_tsOut.SetTsRateBps();
	int nRate = 0;
	hr =m_tsOut.GetTsRateBps(nRate);;
	hr = m_tsOut.GetFifoSize(g_nFifoSize);
	nRate = 1518191;
	hr = m_tsOut.SetTsRateBps(nRate);
	hr =m_tsOut.SetTxMode(DTAPI_TXMODE_188, 1);
	hr =m_tsOut.SetTxControl(DTAPI_TXCTRL_HOLD);

	return 0;
}

int CDekTecTestDlg::UnInitDtaDevice()
{
	return 0;
}

void CDekTecTestDlg::OnBnClickedBtnSelectfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,0, NULL, OFN_FILEMUSTEXIST);
	if(dlg.DoModal() == IDOK)
	{
		sprintf(g_szFile, dlg.m_ofn.lpstrFile);
	}
}

void CDekTecTestDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	g_bRun = FALSE;
	Sleep(1000);
	TerminateThread(m_hDtaThrd, 0);
	TerminateThread(m_hReadThrd, 0);
	m_tsOut.ClearFifo();
	CDialog::OnClose();
}
