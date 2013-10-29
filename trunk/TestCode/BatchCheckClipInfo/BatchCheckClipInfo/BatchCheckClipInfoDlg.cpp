
// BatchCheckClipInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BatchCheckClipInfo.h"
#include "BatchCheckClipInfoDlg.h"
#include "FGlobal.h"
#ifdef UNICODE
#include "mediainfo.h"
#else
#include "FileInfoExtract.h"
#endif

using namespace MediaInfoLib;

#define  MSG_CHECKEND_NORMALLY WM_USER+122
#define CHECK_TYPE_RESOLUTION 0x01
#define CHECK_TYPE_VIDEOTYPE 0x02
#define CHECK_TYPE_AUDIOTYPE 0x04
#define CHECK_TYPE_RATIO	 0X08


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef UNICODE
DWORD __stdcall CheckClipProc(LPVOID paramIn)
{
	if (!paramIn)
	{
		return 0;
	}

	CBatchCheckClipInfoDlg* pDlg = (CBatchCheckClipInfoDlg*)paramIn;
	HWND hwndRecv = pDlg->GetSafeHwnd();
	VECFILES vFiles = pDlg->m_vecFiles;
	UINT nCheckFlag = pDlg->m_nCheckFlag;
	const int nExcludeWidth = pDlg->m_nExcludeWidth;
	const int nExcludeHeight = pDlg->m_nExcludeHeight;
	CString strNeedRatio;
	pDlg->m_cmbRatio.GetWindowText(strNeedRatio);
	BOOL bCanceled = FALSE;
	int nTotal = vFiles.size();
	int nErrFiles = 0;
	for (size_t i = 0; i < vFiles.size(); ++i)
	{
		if (!pDlg->m_bRuning)
		{
			bCanceled = TRUE;
			break;
		}

		int duration = 0;  //单位毫秒
		CString strProcess;
		strProcess.Format(TEXT("%d/%d,正在检查:%s"), i, nTotal, vFiles[i]);
		pDlg->m_stcProcess.SetWindowText(strProcess);

		MediaInfoLib::MediaInfo mInfo;
		MediaInfoLib::String strFile = vFiles[i];
		size_t nRet = mInfo.Open(strFile);
		if (nRet == 0)
		{
			CFWriteLog(TEXT("文件检查失败！file = %s"), vFiles[i]);
			continue;
		}

		int nWidth =0;
		int nHeight = 0;

	
		BOOL bSuc = TRUE;
		if (nCheckFlag & CHECK_TYPE_RESOLUTION)
		{
			String strWidth = mInfo.Get(MediaInfoLib::Stream_Video, 0,TEXT("Width"));
			if (strWidth.size() > 0)
			{
				nWidth = _wtoi(strWidth.c_str());
			}
			MediaInfoLib::String strHeight =mInfo.Get(MediaInfoLib::Stream_Video, 0,TEXT("Height"));
			if (strHeight.size() > 0)
			{
				nHeight = _wtoi(strHeight.c_str());
			}

			if (nExcludeHeight != nHeight || nExcludeWidth != nWidth)
			{
				bSuc = FALSE;
			}
			if (!bSuc)
			{
				nErrFiles++;
				CString strErr;
				strErr.Format(TEXT("长度错误:%dx%d"), nWidth, nHeight);
				int nIdx = pDlg->m_lstResult.InsertItem(pDlg->m_lstResult.GetItemCount(), strErr);
				pDlg->m_lstResult.SetItemText(nIdx, 1, vFiles[i]);
			}
		}

		if (nCheckFlag & CHECK_TYPE_RATIO)
		{
			String strRatio = mInfo.Get(MediaInfoLib::Stream_Video, 0,TEXT("DisplayAspectRatio/String"));
			if (strRatio.compare(strNeedRatio) !=0)
			{
				if (bSuc)
				{
					nErrFiles++;
				}
				CString strErr;
				strErr.Format(TEXT("比率错误:%s"), strRatio.c_str());
				int nIdx = pDlg->m_lstResult.InsertItem(pDlg->m_lstResult.GetItemCount(), strErr);
				pDlg->m_lstResult.SetItemText(nIdx, 1, vFiles[i]);

			}
		}


	}

	pDlg->m_stcProcess.SetWindowText(TEXT(""));
	CFWriteLog(TEXT("检查完毕！共发现%d 个错误"), nErrFiles);

	if (!bCanceled)
	{
		PostMessage(hwndRecv, MSG_CHECKEND_NORMALLY, 0,0);
	}


	return 0;
}
#else
DWORD __stdcall CheckClipProc(LPVOID paramIn)
{
	if (!paramIn)
	{
		return 0;
	}

	CBatchCheckClipInfoDlg* pDlg = (CBatchCheckClipInfoDlg*)paramIn;
	HWND hwndRecv = pDlg->GetSafeHwnd();
	VECFILES vFiles = pDlg->m_vecFiles;
	UINT nCheckFlag = pDlg->m_nCheckFlag;
	const int nExcludeWidth = pDlg->m_nExcludeWidth;
	const int nExcludeHeight = pDlg->m_nExcludeHeight;
	BOOL bCanceled = FALSE;
	int nTotal = vFiles.size();
	int nErrFiles = 0;
	for (size_t i = 0; i < vFiles.size(); ++i)
	{
		if (!pDlg->m_bRuning)
		{
			bCanceled = TRUE;
			break;
		}
		

// 		MediaInfoLib::MediaInfo mInfo;
// 		MediaInfoLib::String strFile = vFiles[i];
//		size_t nRet = mInfo.Open(strFile);
		VideoInfo* pInfoV = 0;
		AudioInfo* pInfoA = 0;
		int duration = 0;  //单位毫秒
		CString strFileToCheck = TEXT("file:");
		strFileToCheck += vFiles[i];
		CString strProcess;
		strProcess.Format(TEXT("%d/%d,正在检查:%s"), i, nTotal, vFiles[i]);
		pDlg->m_stcProcess.SetWindowText(strProcess);

		int nRet =GetFileInfo(strFileToCheck.GetBuffer(), &pInfoV, &pInfoA);
		if (nRet == 0)
		{
			CFWriteLog(TEXT("文件检查失败！file = %s"), vFiles[i]);
			continue;
		}

		int nWidth =0;
		int nHeight = 0;

		if( pInfoA)
		{
			::CoTaskMemFree(pInfoA);
		}
		if(pInfoV)
		{
			nWidth = pInfoV->m_nWidth;
			nHeight = pInfoV->m_nHeight;
			::CoTaskMemFree(pInfoV);

		}
		
		BOOL bSuc = TRUE;
		if (nCheckFlag & CHECK_TYPE_RESOLUTION)
		{
// 			CString strWidth = mInfo.Get(MediaInfoLib::Stream_General, 0,"Width");
// 			if (strWidth.size() > 0)
// 			{
// 				nWidth = atoi(strWidth.c_str());
// 			}
// 			MediaInfoLib::String strHeight =mInfo.Get(MediaInfoLib::Stream_General, 0,"Height");
// 			if (strHeight.size() > 0)
// 			{
// 				nHeight = atoi(strHeight.c_str());
// 			}

			if (nExcludeHeight != nHeight || nExcludeWidth != nWidth)
			{
				bSuc = FALSE;
			}
			if (!bSuc)
			{
				nErrFiles++;
				CString strErr;
				strErr.Format(TEXT("长度错误:%dx%d"), nWidth, nHeight);
				int nIdx = pDlg->m_lstResult.InsertItem(pDlg->m_lstResult.GetItemCount(), strErr);
				pDlg->m_lstResult.SetItemText(nIdx, 1, vFiles[i]);
			}
		}

		
	}
	
	pDlg->m_stcProcess.SetWindowText(TEXT(""));
	CFWriteLog("检查完毕！共发现%d 个错误", nErrFiles);

	if (!bCanceled)
	{
		PostMessage(hwndRecv, MSG_CHECKEND_NORMALLY, 0,0);
	}


	return 0;
}

#endif

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


// CBatchCheckClipInfoDlg 对话框




CBatchCheckClipInfoDlg::CBatchCheckClipInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchCheckClipInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThreadCheckInfo = NULL;
	m_bRuning = FALSE;
	m_nCheckFlag = 0;
}

void CBatchCheckClipInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_edtFolder);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResult);
	DDX_Control(pDX, IDC_EDIT_RESWIDTH, m_edtResWidth);
	DDX_Control(pDX, IDC_EDIT_RESHEIGHT, m_edtResHeight);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_STATIC_PROCESS, m_stcProcess);
	DDX_Control(pDX, IDC_CHECK_TIMERANGE, m_chkTimeRange);
	DDX_Control(pDX, IDC_DATETIMEPICKER_MIN, m_tmCtlMin);
	DDX_Control(pDX, IDC_DATETIMEPICKER_MAX, m_tmCtlMax);
	DDX_Control(pDX, IDC_CMB_RATIO, m_cmbRatio);
}

BEGIN_MESSAGE_MAP(CBatchCheckClipInfoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CBatchCheckClipInfoDlg::OnBnClickedBtnStart)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTNBROWSE, &CBatchCheckClipInfoDlg::OnBnClickedBtnbrowse)
	ON_BN_CLICKED(IDC_CHECK_TIMERANGE, &CBatchCheckClipInfoDlg::OnBnClickedCheckTimerange)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, &CBatchCheckClipInfoDlg::OnBnClickedBtnSavetofile)
END_MESSAGE_MAP()


// CBatchCheckClipInfoDlg 消息处理程序

BOOL CBatchCheckClipInfoDlg::OnInitDialog()
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
	g_hwndLog = GetSafeHwnd();

	m_nCheckFlag = CHECK_TYPE_RESOLUTION|CHECK_TYPE_RATIO;
	m_edtResWidth.SetWindowText(TEXT("720"));
	m_edtResHeight.SetWindowText(TEXT("576"));
	m_edtFolder.SetWindowText(TEXT("d:\\clips"));

	m_lstResult.InsertColumn(0, TEXT("错误"), LVCFMT_LEFT, 150);
	m_lstResult.InsertColumn(1, TEXT("文件"), LVCFMT_LEFT,500);
	m_edtLog.LimitText(102400);
	m_chkTimeRange.SetCheck(BST_UNCHECKED);
	m_cmbRatio.AddString(TEXT("16:9"));
	m_cmbRatio.SetCurSel(0);
	//m_cmbRatio.InsertString("4:3");
	OnBnClickedCheckTimerange();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBatchCheckClipInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBatchCheckClipInfoDlg::OnPaint()
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
HCURSOR CBatchCheckClipInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBatchCheckClipInfoDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_hThreadCheckInfo)
	{
		if (WaitForSingleObject(m_hThreadCheckInfo, 10000) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hThreadCheckInfo, 0);
			CloseHandle(m_hThreadCheckInfo);
		}
		m_bRuning = FALSE;
		m_hThreadCheckInfo = NULL;

	}
	else
	{
		if (!InitParam())
		{
			return;
		}
		m_edtLog.SetWindowText(TEXT(""));

		if (InitCheckFileList() == 0)
		{
			return;
		}
		m_lstResult.DeleteAllItems();
		m_hThreadCheckInfo = CreateThread(NULL, NULL, CheckClipProc, (LPVOID)this, 0, 0);
		m_bRuning = TRUE;
	}
	RefreshCtrl();
	
}

void CBatchCheckClipInfoDlg::OnDestroy()
{
	if (m_hThreadCheckInfo)
	{
		OnBnClickedBtnStart();
		if (m_hThreadCheckInfo)
		{
			TerminateThread(m_hThreadCheckInfo, 0);
			m_hThreadCheckInfo = NULL;
			m_bRuning = FALSE;
		}
	}
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

LRESULT CBatchCheckClipInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (MSG_CHECKEND_NORMALLY == message)
	{
		m_hThreadCheckInfo = NULL;
		m_bRuning = FALSE;
		RefreshCtrl();
		return TRUE;
	}
	MACRO_EDITLOGOUTPUT;
	return CDialog::WindowProc(message, wParam, lParam);
}

int CBatchCheckClipInfoDlg::InitCheckFileList()
{

	m_edtFolder.GetWindowText(m_strCheckFolderPath);
	CString strFolder = m_strCheckFolderPath;
	if (!PathIsDirectory(strFolder))
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("CheckPathNotDirectory!!"));
		AfxMessageBox(TEXT("文件夹路径不正确"));
		return 0;
	}
	
	CFileFind ffind;
	CString strFileFmt = m_strCheckFolderPath;
	strFileFmt.TrimRight('\\');
	strFileFmt += "\\*.mpg";
	BOOL bFind = ffind.FindFile(strFileFmt);
	if (!bFind)
	{
		AfxMessageBox(TEXT("文件获取失败"));
		return 0;
	}
	//m_vecFiles.push_back(ffind.GetFilePath());
	while(bFind)
	{
		bFind = ffind.FindNextFile();
		BOOL bSkip = FALSE;
		CTime tmWrite;
		if (m_bUseTimeRange)
		{
			if(ffind.GetLastWriteTime(tmWrite))
			{
				if (tmWrite > m_tmMax || tmWrite < m_tmMin)
				{
					bSkip = TRUE;
				}
			}
			else
			{
				CFWriteLog(TEXT("文件修改时间获取失败! file = %s"), ffind.GetFilePath() );
			}
		
		}
		if (!bSkip)
		{
			m_vecFiles.push_back(ffind.GetFilePath());
		}
	}
	ffind.Close();
	int nSize =m_vecFiles.size(); 
	CFWriteLog(TEXT("total %d files in %s"), nSize, m_strCheckFolderPath);
	if (nSize == 0)
	{
		AfxMessageBox(TEXT("没有文件待检查"));
		return 0;
	}

	return nSize;
}



void CBatchCheckClipInfoDlg::OnBnClickedBtnbrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL);
	if (dlg.DoModal() == IDOK)
	{
		CString strFile = dlg.m_ofn.lpstrFile;
		int nPosDot = strFile.ReverseFind('.');
		int nPosSplash = strFile.ReverseFind('\\');
		if (nPosDot != -1 && nPosSplash < nPosDot)
		{
			strFile = strFile.Left(nPosSplash);
		}
		m_edtFolder.SetWindowText(strFile);
	}
}

void CBatchCheckClipInfoDlg::RefreshCtrl()
{
	m_btnStart.SetWindowText(m_bRuning? TEXT("Stop"):TEXT("Start"));
}

BOOL CBatchCheckClipInfoDlg::InitParam()
{
	CString strX,strY;
	m_edtResWidth.GetWindowText(strX);
	m_edtResHeight.GetWindowText(strY);
	m_nExcludeWidth = _wtoi(strX.GetBuffer());
	m_nExcludeHeight = _wtoi(strY.GetBuffer());
	if (m_nExcludeHeight <= 0 || m_nExcludeWidth <= 0)
	{
		AfxMessageBox(TEXT("分辨率设定错误"));
		return FALSE;
	}
	m_bUseTimeRange = (m_chkTimeRange.GetCheck() == BST_CHECKED);
	if (m_bUseTimeRange)
	{
		SYSTEMTIME stmMIn;
		m_tmCtlMin.GetTime(&stmMIn);
		SYSTEMTIME stmMax;
		m_tmCtlMax.GetTime(&stmMax);
		m_tmMin = CTime(stmMIn);
		m_tmMax = CTime(stmMax);
		if (m_tmMin >= m_tmMax)
		{
			AfxMessageBox(TEXT("日期范围不对"));
			return FALSE;
		}
	}
	
	return TRUE;
}

void CBatchCheckClipInfoDlg::OnBnClickedCheckTimerange()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bUseTimeRange = m_chkTimeRange.GetCheck() == BST_CHECKED;
	m_tmCtlMin.EnableWindow(m_bUseTimeRange);
	m_tmCtlMax.EnableWindow(m_bUseTimeRange);
}

void CBatchCheckClipInfoDlg::OnBnClickedBtnSavetofile()
{
	// TODO: 在此添加控件通知处理程序代码
	int  nCount = m_lstResult.GetItemCount();
	if (nCount == 0)
	{
		AfxMessageBox(TEXT("no result"));
		return;
	}
	CString strPath = GetAppPathW().c_str();
	CString strFile;
	CTime tm(time(NULL));
	strFile.Format(TEXT("%s\\Result_%04d-%02d-%02d-%02d%02d%02d.txt"),
		strPath, tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
	CFile file;
	if (file.Open(strFile, CFile::modeCreate|CFile::modeWrite, NULL))
	{
		for (int i = 0; i < nCount; ++i)
		{
			CString strTextErr =m_lstResult.GetItemText(i, 1);
			strTextErr +=TEXT("\r\n");
			file.Write(strTextErr.GetBuffer(), strTextErr.GetLength()*2);
		}
		file.Flush();
		file.Close();
		AfxMessageBox(TEXT("write file success"));
	}
	else
	{
		AfxMessageBox(TEXT("write file err"));

	}

}
