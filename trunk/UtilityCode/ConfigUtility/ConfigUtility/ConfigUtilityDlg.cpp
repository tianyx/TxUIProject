
// ConfigUtilityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigUtility.h"
#include "ConfigUtilityDlg.h"
#include "FGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STR_KEY_PRFIX TEXT("╠")
#define STR_KEY_SPACE TEXT("  ")
#define STR_VAL_LOSSTEXT TEXT("----")

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


// CConfigUtilityDlg 对话框




CConfigUtilityDlg::CConfigUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigUtilityDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConfigUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_MODE_INTERNAL, m_rdoModeLocal);
	DDX_Control(pDX, IDC_BUTTON_STARTCOMPARE, m_btnStartCompare);
	DDX_Control(pDX, IDC_EDIT_SRCPATH, m_edtSrcPath);
	DDX_Control(pDX, IDC_EDIT_DES_PATH, m_edtDesPath);
	DDX_Control(pDX, IDC_COMBO_CLASS_ROOT, m_cmbClassRoot);
	DDX_Control(pDX, IDC_COMBO_CLASS_SUB, m_cmbClassSub);
	DDX_Control(pDX, IDC_COMBO_CLASS_SUBSUB, m_cmbClassSubSub);
	DDX_Control(pDX, IDC_LIST_LOG, m_lstLog);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lstResult);
	DDX_Control(pDX, IDC_BUTTON_SRC_BROWSE, m_btnSrcBrowse);
	DDX_Control(pDX, IDC_BUTTON_DES_BROWSE, m_btnDesBrowse);
	DDX_Control(pDX, IDC_CHECK_HIDE_SRC_LOSS, m_chkHideSrcLoss);
	DDX_Control(pDX, IDC_CHECK_HIDE_DES_LOSS, m_chkHideDesLoss);
	DDX_Control(pDX, IDC_CHECK_SHOWDIFFONLY, m_chkShowDiffOnly);
	DDX_Control(pDX, IDC_STATIC_LOG, m_stcLog);
}

BEGIN_MESSAGE_MAP(CConfigUtilityDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_STARTCOMPARE, &CConfigUtilityDlg::OnBnClickedButtonStartcompare)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_CLASS_ROOT, &CConfigUtilityDlg::OnCbnSelchangeComboClassRoot)
	ON_CBN_SELCHANGE(IDC_COMBO_CLASS_SUB, &CConfigUtilityDlg::OnCbnSelchangeComboClassSub)
	ON_BN_CLICKED(IDC_RADIO_MODE_INTERNAL, &CConfigUtilityDlg::OnBnClickedRadioModeInternal)
	ON_BN_CLICKED(IDC_RADIO_MODE_OPENFILE, &CConfigUtilityDlg::OnBnClickedRadioModeOpenfile)
	ON_BN_CLICKED(IDC_BUTTON_SRC_BROWSE, &CConfigUtilityDlg::OnBnClickedButtonSrcBrowse)
	ON_BN_CLICKED(IDC_BUTTON_DES_BROWSE, &CConfigUtilityDlg::OnBnClickedButtonDesBrowse)
	ON_BN_CLICKED(IDC_CHECK_SHOWDIFFONLY, &CConfigUtilityDlg::OnBnClickedCheckShowdiffonly)
	ON_BN_CLICKED(IDC_CHECK_HIDE_SRC_LOSS, &CConfigUtilityDlg::OnBnClickedCheckHideSrcLoss)
	ON_BN_CLICKED(IDC_CHECK_HIDE_DES_LOSS, &CConfigUtilityDlg::OnBnClickedCheckHideDesLoss)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_RESULT, &CConfigUtilityDlg::OnNMCustomdrawListResult)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CConfigUtilityDlg 消息处理程序

BOOL CConfigUtilityDlg::OnInitDialog()
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
	m_lstResult.SetExtendedStyle(m_lstResult.GetExtendedStyle()| LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	g_hwndLog = GetSafeHwnd();
	Reset();
	ChangeSrc();
	m_lstResult.InsertColumn(0,TEXT("名称"), LVCFMT_LEFT, 150);
	m_lstResult.InsertColumn(1,TEXT("源配置"), LVCFMT_LEFT, 300);
	m_lstResult.InsertColumn(2,TEXT("目标配置"), LVCFMT_LEFT, 300);

	//
// 	m_edtDesPath.DragAcceptFiles(TRUE);
// 	m_edtSrcPath.DragAcceptFiles(TRUE);
// 	DragAcceptFiles(TRUE);
	m_drapTarget.Register(this);

#ifdef _DEBUG
	m_edtSrcPath.SetWindowText(TEXT("e:\\1.clsid"));
	m_edtDesPath.SetWindowText(TEXT("e:\\2.clsid"));

#endif // _DEBUG

	Relayout();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE


}

void CConfigUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConfigUtilityDlg::OnPaint()
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
HCURSOR CConfigUtilityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CConfigUtilityDlg::OnBnClickedButtonStartcompare()
{
	// TODO: 在此添加控件通知处理程序代码
	wstring wsSrcFile;
	wstring wsDesFile;

	CString strTmp;
	if (m_rdoModeLocal.GetCheck() == BST_CHECKED)
	{
		int nSel = m_cmbClassRoot.GetCurSel();
		int nSelsub = m_cmbClassSub.GetCurSel();
		int nSelsubsub = m_cmbClassSubSub.GetCurSel();
		if (nSelsubsub <= 0)
		{
			AfxMessageBox(TEXT("请选择源文件！"));
			return;
		}

		MAPSROOT::iterator itb = m_cfgMgr.m_mapCfgRoot.begin();
		std::advance(itb, nSel-1);

		MAPSSUB::iterator itbsub = itb->second.m_data.begin();
		std::advance(itbsub, nSelsub-1);

		MAPSSUB2::iterator itbsubsub = itbsub->second.m_data.begin();
		std::advance(itbsubsub, nSelsubsub-1);
		wsSrcFile = itbsubsub->second;
	}
	else
	{
		m_edtSrcPath.GetWindowText(strTmp);
		wsSrcFile = strTmp;
	}

	m_edtDesPath.GetWindowText(strTmp);
	wsDesFile = strTmp;

	if (wsSrcFile.empty()||wsDesFile.empty())
	{
		AfxMessageBox(TEXT("please select a file!"));
		return;
	}

	if (_waccess(wsSrcFile.c_str(), 04) == -1)
	{
		CFWriteLog(TEXT("Error: can't access file: %s"), wsSrcFile.c_str());
		AfxMessageBox(TEXT("can't access source file!"));
		return;
		
	}

	if (_waccess(wsDesFile.c_str(), 04) == -1)
	{
		CFWriteLog(TEXT("Error: can't access file: %s"), wsSrcFile.c_str());
		AfxMessageBox(TEXT("cant't access destination file!"));
		return;
	}

	if (wsDesFile.compare(wsSrcFile) == 0)
	{
		AfxMessageBox(TEXT("file equal！"));
		return;
	}

	m_lstLog.ResetContent();
	m_lstResult.DeleteAllItems();
		
	int nRet = m_cfgMgr.Compare(wsSrcFile, wsDesFile);
	CString strRet = TEXT("unknow result");
	switch (nRet)
	{
	case 0:
		strRet = TEXT("files are identical.");
		break;
	case 1:
		strRet = TEXT("there are some difference between files.");
		break;
	case -1:
		strRet = TEXT("comparing failed, please check log!");
		break;
	}
	AfxMessageBox(strRet);
	if (nRet >= 0)
	{
		RefreshList();
	}
}

void CConfigUtilityDlg::Reset()
{
	m_rdoModeLocal.SetCheck(BST_CHECKED);
	//init
	m_cfgMgr.ResetData();

	m_cmbClassRoot.ResetContent();
	m_cmbClassSubSub.ResetContent();
	m_cmbClassSubSub.ResetContent();

	//insert to
	m_cmbClassRoot.InsertString(-1, TEXT("请选择"));
	MAPSROOT::iterator itb = m_cfgMgr.m_mapCfgRoot.begin();
	MAPSROOT::iterator ite = m_cfgMgr.m_mapCfgRoot.end();
	for (; itb != ite; ++itb)
	{
		m_cmbClassRoot.InsertString(-1, itb->first.c_str());
	}

	m_cmbClassSub.InsertString(-1, TEXT("请选择"));
	m_cmbClassSubSub.InsertString(-1, TEXT("请选择"));
	
	m_cmbClassRoot.SetCurSel(0);
	m_cmbClassSub.SetCurSel(0);
	m_cmbClassSubSub.SetCurSel(0);
}

void CConfigUtilityDlg::OnDestroy()
{
	g_hwndLog = NULL;
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CConfigUtilityDlg::OnCbnSelchangeComboClassRoot()
{
	// TODO: 在此添加控件通知处理程序代码
	//change sub
	m_cmbClassSub.ResetContent();
	m_cmbClassSubSub.ResetContent();
	m_cmbClassSub.InsertString(-1, TEXT("请选择"));
	m_cmbClassSubSub.InsertString(-1, TEXT("请选择"));

	m_cmbClassSub.SetCurSel(0);
	m_cmbClassSubSub.SetCurSel(0);

	int nSel = m_cmbClassRoot.GetCurSel();
	if (nSel <= 0)
	{
		return;
	}

	MAPSROOT::iterator itb = m_cfgMgr.m_mapCfgRoot.begin();
	std::advance(itb, nSel-1);
	MAPSSUB& mapSub = itb->second;
	MAPSSUB::iterator itbs = mapSub.begin();
	MAPSSUB::iterator ites = mapSub.end();
	for (; itbs != ites; ++itbs)
	{
		m_cmbClassSub.InsertString(-1, itbs->first.c_str());
	}
	
}

void CConfigUtilityDlg::OnCbnSelchangeComboClassSub()
{
	// TODO: 在此添加控件通知处理程序代码
	m_cmbClassSubSub.ResetContent();
	m_cmbClassSubSub.InsertString(-1, TEXT("请选择"));

	m_cmbClassSubSub.SetCurSel(0);

	int nSel = m_cmbClassRoot.GetCurSel();
	int nSelsub = m_cmbClassSub.GetCurSel();

	if (nSelsub <= 0)
	{
		return;
	}

	MAPSROOT::iterator itb = m_cfgMgr.m_mapCfgRoot.begin();
	std::advance(itb, nSel-1);

	MAPSSUB::iterator itbsub = itb->second.m_data.begin();
	std::advance(itbsub, nSelsub-1);

	MAPSSUB2 & mapSub = itbsub->second;
	MAPSSUB2::iterator itbss = mapSub.begin();
	MAPSSUB2::iterator itess = mapSub.end();
	for (; itbss != itess; ++itbss)
	{
		m_cmbClassSubSub.InsertString(-1, itbss->first.c_str());
	}

}

void CConfigUtilityDlg::OnBnClickedRadioModeInternal()
{
	// TODO: 在此添加控件通知处理程序代码
	ChangeSrc();
}

void CConfigUtilityDlg::OnBnClickedRadioModeOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	ChangeSrc();
}

void CConfigUtilityDlg::ChangeSrc()
{
	BOOL bLocal = m_rdoModeLocal.GetCheck() == BST_CHECKED;
	m_cmbClassRoot.EnableWindow(bLocal);
	m_cmbClassSub.EnableWindow(bLocal);
	m_cmbClassSubSub.EnableWindow(bLocal);
	m_edtSrcPath.EnableWindow(!bLocal);
	m_btnSrcBrowse.EnableWindow(!bLocal);
}



void CConfigUtilityDlg::OnBnClickedButtonSrcBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ENABLESIZING|OFN_FILEMUSTEXIST, TEXT("*.ini,*.clsid|*.ini;*.clsid||"),this);
	if (dlg.DoModal() == IDOK)
	{
		m_edtSrcPath.SetWindowText(dlg.m_ofn.lpstrFile);
	}
}

void CConfigUtilityDlg::OnBnClickedButtonDesBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_ENABLESIZING|OFN_FILEMUSTEXIST, TEXT("*.ini,*.clsid|*.ini;*.clsid||"),this);
	if (dlg.DoModal() == IDOK)
	{
		m_edtDesPath.SetWindowText(dlg.m_ofn.lpstrFile);
	}
}

void CConfigUtilityDlg::RefreshList()
{
	m_lstResult.DeleteAllItems();
	if (m_cfgMgr.m_mapValLeft.empty())
	{
		return;
	}

	ST_SHOWPARAM paramIn;
	paramIn.bShowDiffOnly = m_chkShowDiffOnly.GetCheck() == BST_CHECKED;
	paramIn.bHideLeftLoss = m_chkHideSrcLoss.GetCheck() == BST_CHECKED;
	paramIn.bHideRightLoss = m_chkHideDesLoss.GetCheck()== BST_CHECKED;

	ASSERT(m_cfgMgr.m_mapValLeft.size() == m_cfgMgr.m_mapValRight.size());
	MAPCFGSECTION::iterator itbSecLeft = m_cfgMgr.m_mapValLeft.begin();
	MAPCFGSECTION::iterator itbSecRight = m_cfgMgr.m_mapValRight.begin();

	MAPCFGSECTION::iterator iteSecLeft = m_cfgMgr.m_mapValLeft.end();
	MAPCFGSECTION::iterator iteSecRight = m_cfgMgr.m_mapValRight.end();
	for (; itbSecLeft != iteSecLeft; ++itbSecLeft, ++itbSecRight)
	{
		m_lstResult.InsertItem(m_lstResult.GetItemCount(), itbSecLeft->first.c_str());
		CString strPreFix = STR_KEY_PRFIX;
		InsertKey(itbSecLeft->second, itbSecRight->second, strPreFix, paramIn);
	}


}

void CConfigUtilityDlg::InsertKey( MAPCFGKEY& mapLeftIn, MAPCFGKEY& mapRightIn, CString strPreIn, ST_SHOWPARAM& paramIn )
{
	ASSERT(mapLeftIn.size() == mapRightIn.size());
	MAPCFGKEY::iterator itbL = mapLeftIn.begin();
	MAPCFGKEY::iterator iteL = mapLeftIn.end();
	MAPCFGKEY::iterator itbR = mapRightIn.begin();
	MAPCFGKEY::iterator iteR = mapRightIn.end();

	for (; itbL != iteL; ++itbL, ++itbR)
	{
		InsertKey(itbL->second, itbR->second, strPreIn, paramIn);
	}
}

void CConfigUtilityDlg::InsertKey( ST_CFGVALUE& valLeftIn, ST_CFGVALUE& valRightIn, CString strPreIn, ST_SHOWPARAM& paramIn )
{
	ASSERT(valLeftIn.wskey.compare(valRightIn.wskey) == 0
		&& valLeftIn.nType == valRightIn.nType);
	if (valLeftIn.nCompType == COMPTYPE_LOSS && paramIn.bHideLeftLoss)
	{
		return;
	}

	if (valRightIn.nCompType ==  COMPTYPE_LOSS && paramIn.bHideRightLoss)
	{
		return;
	}

	if (valLeftIn.nCompType == COMPTYPE_EQUAL && paramIn.bShowDiffOnly)
	{
		return;
	}

	CString strTmp = strPreIn;
	strTmp += valLeftIn.wskey.c_str();

	int nItem = m_lstResult.InsertItem(m_lstResult.GetItemCount(), strTmp);
	m_lstResult.SetItemData(nItem, (DWORD_PTR)valLeftIn.nCompType);
	if (valLeftIn.nCompType == COMPTYPE_LOSS)
	{
		strTmp = STR_VAL_LOSSTEXT;
	}
	else
	{
		strTmp = (valLeftIn.nType == CFGVALTYPE_VEC||valRightIn.nType == CFGVALTYPE_MAP)? valLeftIn.wsOrgData.c_str() : valLeftIn.wsVal.c_str();
	}
	m_lstResult.SetItemText(nItem, 1, strTmp);
	if (valRightIn.nCompType == COMPTYPE_LOSS)
	{
		strTmp = STR_VAL_LOSSTEXT;
	}
	else
	{
		strTmp = (valRightIn.nType == CFGVALTYPE_VEC||valRightIn.nType == CFGVALTYPE_MAP)? valRightIn.wsOrgData.c_str() : valRightIn.wsVal.c_str();

	}
	m_lstResult.SetItemText(nItem, 2, strTmp);

	if (valLeftIn.nType == CFGVALTYPE_MAP)
	{
		CString strPreFix =STR_KEY_SPACE + strPreIn;
		InsertKey(valLeftIn.mapVal, valRightIn.mapVal, strPreFix, paramIn);
	}

}

void CConfigUtilityDlg::OnBnClickedCheckShowdiffonly()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshList();
}

void CConfigUtilityDlg::OnBnClickedCheckHideSrcLoss()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshList();
}

void CConfigUtilityDlg::OnBnClickedCheckHideDesLoss()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshList();
}

void CConfigUtilityDlg::OnNMCustomdrawListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW  pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW >(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	switch (pNMCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;

		}break;
	case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
		}break;

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		{
			ENUMCOMAPRETYPE compType = (ENUMCOMAPRETYPE)(pNMCD->nmcd.lItemlParam);
			if (compType == COMPTYPE_DIFF)
			{
				if (pNMCD->iSubItem == 2)
				{
					pNMCD->clrTextBk = RGB(255,255, 200);
				}
// 				else if (pNMCD->iSubItem == 1)
// 				{
// 					pNMCD->clrTextBk = RGB(255,255,200);
// 				}
			}
			else if (compType == COMPTYPE_LOSS)
			{
				if (pNMCD->iSubItem == 2)
				{
					pNMCD->clrTextBk = RGB(255, 200, 255);
				}
// 				else
// 				{
// 					pNMCD->clrTextBk = RGB(255, 255, 255);
// 				}
			}
			else if (compType == COMPTYPE_EXTRA)
			{
				if (pNMCD->iSubItem == 2)
				{
					pNMCD->clrTextBk = RGB(200,255,255);
				}
			}

		}break;

	}

}

LRESULT CConfigUtilityDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_LOGWRITE)
	{
		TCHAR* pBuffer = (TCHAR*)wParam;
		if (pBuffer)
		{
			m_lstLog.AddString(pBuffer);
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CConfigUtilityDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Relayout();
}

void CConfigUtilityDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 750;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CConfigUtilityDlg::Relayout()
{
	if (!m_edtDesPath.GetSafeHwnd())
	{
		return;
	}

	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcLog(rcThis);
	rcLog.top = rcLog.bottom -190;
	rcLog.DeflateRect(10, 0,10,10);
	m_lstLog.MoveWindow(rcLog);

	CRect rcStcLog(0,0,100,20);
	rcStcLog.MoveToXY(rcLog.left, rcLog.top - rcStcLog.Height()-2);
	m_stcLog.MoveWindow(rcStcLog);

	CRect rcResult(rcThis);
	rcResult.bottom = rcStcLog.top -2;
	rcResult.top = 200;
	rcResult.DeflateRect(10,0,10,0);
	m_lstResult.MoveWindow(rcResult);

}
