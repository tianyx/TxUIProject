// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	AutoDetect*	pDetect = AutoDetect::CreateInstance(YUV_FORMAT_YUY2);

#ifdef _DEBUG
	CFile file;
	char buffer[MAX_PATH+1];
	GetModuleFileName(GetModuleHandle(NULL),buffer,MAX_PATH);
	PathRemoveFileSpec( buffer);
//	GetCurrentDirectory(MAX_PATH,buffer);
	_tcscat(buffer,"\\DebugLog.txt");
	file.Open(buffer,CFile::modeCreate | CFile::modeWrite,NULL);
	CDumpContext dc(&file);
//	CDumpContext dc;
	pDetect->AssertValid();
	dc<<"pDetect1 Initialized:\r\n";
	dc<<"________________________________________________\r\n";
	pDetect->Dump(dc);
	dc<<"________________________________________________\r\n";

#endif
CString strXML1 = 
	"<AutoDetectPara>	\
		<Info OPType=\"Set\"/>\
		<Data \
		YUV_FORMAT=\"1\" \
		CLIP_WIDTH=\"100\" \
		CLIP_HEIGHT=\"100\" \
		TOP_SECURE_SECTION=\"100.0\"\
		BOTTOM_SECURE_SECTION=\"100.0\"\
		LEFT_SECURE_SECTION=\"100.0\" \
		RIGHT_SECURE_SECTION=\"100.0\" \
		BLACK_SCENE_THRESHOLD=\"100\"\
		COLOR_SCENE_PERCENTAGE=\"100.0\" \
		COLOR_STRIP_PERCENTAGE=\"100.0\" \
		STATIC_FRAME_PERCENTAGE=\"100.0\" \
		LINE_COUNTS_FOR_STRIP_DETECT=\"100\"\
		AMBIT_DEEMED_TO_SAME_PIXEL=\"100\" \
		UNDULATE_AMBIT_OF_Y=\"100\" \
		UNDULATE_AMBIT_OF_U=\"100\" \
		UNDULATE_AMBIT_OF_V=\"100\"/>\
	</AutoDetectPara>";
	pDetect->XMLConfig(strXML1);
#ifdef _DEBUG
	pDetect->AssertValid();
	dc<<"pDetect1 XML Configuration:\r\n";
	dc<<"________________________________________________\r\n";
	pDetect->Dump(dc);
	dc<<"________________________________________________\r\n";
#endif
	AutoDetect* pDetect2 = AutoDetect::CreateInstance(YUV_FORMAT_YV12);

#ifdef _DEBUG
	pDetect2->AssertValid();
	dc<<"pDetect2 Initialized:\r\n";
	dc<<"________________________________________________\r\n";
	pDetect2->Dump(dc);
	dc<<"________________________________________________\r\n";
#endif
	CString strXML2 = 
	"<AutoDetectPara>	\
						  <Info OPType=\"Set\"/>\
						  <Data \
						  YUV_FORMAT=\"2\" \
						  CLIP_WIDTH=\"2\" \
						  CLIP_HEIGHT=\"2\" \
						  TOP_SECURE_SECTION=\"2.0\"\
						  BOTTOM_SECURE_SECTION=\"2.0\"\
						  LEFT_SECURE_SECTION=\"2.0\" \
						  RIGHT_SECURE_SECTION=\"2.0\" \
						  BLACK_SCENE_THRESHOLD=\"2\"\
						  COLOR_SCENE_PERCENTAGE=\"2.0\" \
						  COLOR_STRIP_PERCENTAGE=\"2.0\" \
						  STATIC_FRAME_PERCENTAGE=\"2.0\" \
						  LINE_COUNTS_FOR_STRIP_DETECT=\"2\"\
						  AMBIT_DEEMED_TO_SAME_PIXEL=\"2\" \
						  UNDULATE_AMBIT_OF_Y=\"2\" \
						  UNDULATE_AMBIT_OF_U=\"2\" \
						  UNDULATE_AMBIT_OF_V=\"2\"/>\
						  </AutoDetectPara>";
	pDetect2->XMLConfig(strXML2);
#ifdef _DEBUG
	pDetect2->AssertValid();
	dc<<"pDetect2 XML Configuration:\r\n";
	dc<<"________________________________________________\r\n";
	pDetect2->Dump(dc);
	dc<<"________________________________________________\r\n";
#endif

#ifdef _DEBUG
	dc<<"Check for two instances at same time.\r\n";
	pDetect->AssertValid();
	dc<<"________________________________________________\r\n";
	pDetect->Dump(dc);
	dc<<"________________________________________________\r\n";
	pDetect2->AssertValid();
	dc<<"________________________________________________\r\n";
	pDetect2->Dump(dc);
	dc<<"________________________________________________\r\n";
#endif
	pDetect->DefaultConfig();
#ifdef _DEBUG
	dc<<"Check for two instances at same time after pDetect1 DefaultConfig.\r\n";
	pDetect->AssertValid();
	dc<<"________________________________________________\r\n";
	pDetect->Dump(dc);
	dc<<"________________________________________________\r\n";
	pDetect2->AssertValid();
	dc<<"________________________________________________\r\n";
	pDetect2->Dump(dc);
	dc<<"________________________________________________\r\n";
#endif
	pDetect2->DefaultConfig();
#ifdef _DEBUG
	dc<<"Check for two instances at same time after both instances DefaultConfig.\r\n";
	pDetect->AssertValid();
	dc<<"________________________________________________\r\n";
	pDetect->Dump(dc);
	dc<<"________________________________________________\r\n";
	pDetect2->AssertValid();
	dc<<"________________________________________________\r\n";
	pDetect2->Dump(dc);
	dc<<"________________________________________________\r\n";
#endif
	pDetect->Release();
	pDetect2->Release();
#ifdef _DEBUG
	char ret[MAX_PATH];
	_tcscpy(ret,"配置检测成功.\r\n");
	_tcscat(ret,"请到 ");_tcscat(ret,buffer);_tcscat(ret," 查看详细.");
	MessageBox(ret,"提示");
#endif
	PostQuitMessage(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CTestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	AutoDetect::DestroyDefaultOptionMap();
}
