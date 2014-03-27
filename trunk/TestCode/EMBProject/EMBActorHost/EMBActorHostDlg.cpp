
// EMBActorHostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EMBActorHost.h"
#include "EMBActorHostDlg.h"

#include "GDIDrawFunc.h"
#include "TxImageLoader.h"


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


// CEMBActorHostDlg �Ի���




CEMBActorHostDlg::CEMBActorHostDlg(CWnd* pParent /*=NULL*/)
: CDialog(CEMBActorHostDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMBActorHostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEMBActorHostDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CLOSE()

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CEMBActorHostDlg ��Ϣ�������

BOOL CEMBActorHostDlg::OnInitDialog()
{
	m_pbmpBack = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("back.bmp"));
	m_pbmpLogo2 = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("logo2.png"));
	m_pbmpSublogo = CTxImageLoader::GetTxImageLoader().LoadBitmap(TEXT("sublogoactor.png"));

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
	m_dlgtest.Create(IDD_DLG_TEST, this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEMBActorHostDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEMBActorHostDlg::OnPaint()
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
HCURSOR CEMBActorHostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CEMBActorHostDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void CEMBActorHostDlg::OnClose()
{
	// ��ʾ
	if (IDNO == MessageBox("ȷ���˳�����?", "��ʾ", MB_YESNO))
	{
		return;
	}

	CDialog::OnClose();
}
BOOL CEMBActorHostDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_pbmpBack)
	{
		//TRACE(TEXT("\n dlg back redraw"));
		CRect rcThis;
		GetClientRect(rcThis);
		CRect rcEdge(5,5,5,5);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpBack, rcThis, NULL, TXBMP_STRETCH_MID_LRTB, NULL, NULL);
		CRect rclogo2(0,0, m_pbmpLogo2->GetWidth(), m_pbmpLogo2->GetHeight());
		rclogo2.MoveToXY(0, (rcThis.Height() - rclogo2.Height())/2);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpLogo2, rclogo2, NULL, TXBMP_STRETCH_NONE, NULL, NULL);
		CRect rcsublogo(0,0,100,40);
		rcsublogo.MoveToXY(rclogo2.left +rclogo2.Width() - 100, rclogo2.bottom - 25);
		GPDrawStretchImage(pDC->GetSafeHdc(), m_pbmpSublogo, rcsublogo, NULL, TXBMP_STRETCH_NONE, NULL, NULL);
		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

BOOL CEMBActorHostDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE ||  pMsg->wParam == VK_F1
			||pMsg ->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_F11 && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
			&& ((GetKeyState(VK_LSHIFT) & 0x8000) != 0))
		{
			static BOOL  bShowDbg = TRUE;
			bShowDbg = !bShowDbg;
			//show debug
			HWND hwndCnol =GetConsoleWindow();
			if (hwndCnol)
			{
				::ShowWindow(hwndCnol, bShowDbg? SW_SHOW:SW_HIDE);

			}
			m_dlgtest.ShowWindow(bShowDbg? SW_SHOW:SW_HIDE);

		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
