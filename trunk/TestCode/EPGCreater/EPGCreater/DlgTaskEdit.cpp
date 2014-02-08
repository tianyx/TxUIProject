// DlgTaskEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "EPGCreater.h"
#include "DlgTaskEdit.h"
#include "StrConvert.h"

// CDlgTaskEdit 对话框

IMPLEMENT_DYNAMIC(CDlgTaskEdit, CDialog)

CDlgTaskEdit::CDlgTaskEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTaskEdit::IDD, pParent)
{
	nLastChnSel = -1;
	m_vSectionBox[0] = &m_cmbSection1;
	m_vSectionBox[1] = &m_cmbSection2;
	m_vSectionBox[2] = &m_cmbSection3;
	m_vSectionBox[3] = &m_cmbSection4;
	m_vSectionBox[4] = &m_cmbSection5;
	m_vSectionBox[5] = &m_cmbSection6;
	m_vSectionBox[6] = &m_cmbSection7;

	m_vchkWeekDay[0] = &m_chkWeekDay1;
	m_vchkWeekDay[1] = &m_chkWeekDay2;
	m_vchkWeekDay[2] = &m_chkWeekDay3;
	m_vchkWeekDay[3] = &m_chkWeekDay4;
	m_vchkWeekDay[4] = &m_chkWeekDay5;
	m_vchkWeekDay[5] = &m_chkWeekDay6;
	m_vchkWeekDay[6] = &m_chkWeekDay7;


}

CDlgTaskEdit::~CDlgTaskEdit()
{
}

void CDlgTaskEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHN, m_cmbChn);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY1, m_chkWeekDay1);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY2, m_chkWeekDay2);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY3, m_chkWeekDay3);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY4, m_chkWeekDay4);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY5, m_chkWeekDay5);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY6, m_chkWeekDay6);
	DDX_Control(pDX, IDC_CHECK_WEEKDAY7, m_chkWeekDay7);
	DDX_Control(pDX, IDC_COMBO_SECTION1, m_cmbSection1);
	DDX_Control(pDX, IDC_COMBO_SECTION2, m_cmbSection2);
	DDX_Control(pDX, IDC_COMBO_SECTION3, m_cmbSection3);
	DDX_Control(pDX, IDC_COMBO_SECTION4, m_cmbSection4);
	DDX_Control(pDX, IDC_COMBO_SECTION5, m_cmbSection5);
	DDX_Control(pDX, IDC_COMBO_SECTION6, m_cmbSection6);
	DDX_Control(pDX, IDC_COMBO_SECTION7, m_cmbSection7);
	DDX_Control(pDX, IDC_CHECK_TASKGENONTIME, m_chkTimeGen);
	DDX_Control(pDX, IDC_EDIT_DAYOFFSET, m_edtOffsetDay);
	DDX_Control(pDX, IDC_DATETIMEPICKER_GENTIME, m_dtStartTime);
	DDX_Control(pDX, IDC_CHECK_NEEDSUBMITTED, m_chkMustSubmitted);
	DDX_Control(pDX, IDC_EDIT_DESC, m_edtDesc);
	DDX_Control(pDX, IDC_TREE1, m_TreeChn);
	DDX_Control(pDX, IDC_CHECK_CHANGEWEEKLOOPMODE, m_chkWeekLoopMode);
}


BEGIN_MESSAGE_MAP(CDlgTaskEdit, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_CHN, &CDlgTaskEdit::OnCbnSelchangeComboChn)
	ON_BN_CLICKED(IDOK, &CDlgTaskEdit::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_TASKGENONTIME, &CDlgTaskEdit::OnBnClickedCheckTaskgenontime)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY1, &CDlgTaskEdit::OnBnClickedCheckWeekday)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY2, &CDlgTaskEdit::OnBnClickedCheckWeekday)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY3, &CDlgTaskEdit::OnBnClickedCheckWeekday)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY4, &CDlgTaskEdit::OnBnClickedCheckWeekday)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY5, &CDlgTaskEdit::OnBnClickedCheckWeekday)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY6, &CDlgTaskEdit::OnBnClickedCheckWeekday)
	ON_BN_CLICKED(IDC_CHECK_WEEKDAY7, &CDlgTaskEdit::OnBnClickedCheckWeekday)

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CDlgTaskEdit::OnTvnSelchangedTree1)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION1, &CDlgTaskEdit::OnCbnSelchangeComboSection)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION2, &CDlgTaskEdit::OnCbnSelchangeComboSection)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION3, &CDlgTaskEdit::OnCbnSelchangeComboSection)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION4, &CDlgTaskEdit::OnCbnSelchangeComboSection)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION5, &CDlgTaskEdit::OnCbnSelchangeComboSection)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION6, &CDlgTaskEdit::OnCbnSelchangeComboSection)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION7, &CDlgTaskEdit::OnCbnSelchangeComboSection)


	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE1, &CDlgTaskEdit::OnNMCustomdrawTree1)
END_MESSAGE_MAP()


// CDlgTaskEdit 消息处理程序

BOOL CDlgTaskEdit::OnInitDialog()
{
	CDialog::OnInitDialog();
	//
	m_TreeChn.ModifyStyle(0, TVS_FULLROWSELECT, 0);
	m_TreeChn.ModifyStyle( TVS_CHECKBOXES, 0 ); 
	m_TreeChn.ModifyStyle( 0, TVS_CHECKBOXES ); 
	SetWindowText(strDlgTitle);
	m_edtDesc.LimitText(64);
	//init chn list
	for (size_t i = 0; i < g_GlobalInfo.vChnInfo.size(); ++i)
	{
		m_cmbChn.AddString(g_GlobalInfo.vChnInfo[i].strChnName);
		HTREEITEM hItem =m_TreeChn.InsertItem(g_GlobalInfo.vChnInfo[i].strChnName);
		m_TreeChn.SetItemData(hItem, (DWORD_PTR)i);
		ST_ONECHN oneChn;
		oneChn.strChId = g_GlobalInfo.vChnInfo[i].strChnId;
		for (int i = 0; i < 7; ++i)
		{
			STWEEKDAYINFO wInfo;
			wInfo.nDay = i;
			wInfo.bEnable = 0;
			oneChn.vWeekDays.push_back(wInfo);
		}
		m_vChnCtrlData.push_back(oneChn);
	}

	for (int i = 0; i< 7; ++i)
	{
		m_vchkWeekDay[i]->SetCheck(BST_CHECKED);
	}
	m_chkTimeGen.SetCheck(BST_CHECKED);
	UpdateTaskData(FALSE);
	m_TreeChn.SelectItem(m_TreeChn.GetRootItem());
	UpdateWeedayCtrl();
	m_TreeChn.SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CDlgTaskEdit::Init( STEPGTASK* pTaskIn /*= NULL*/ )
{
	m_nEditMode = pTaskIn != NULL? 1:0;
	if (pTaskIn)
	{
		m_task = *pTaskIn;
		strDlgTitle = TEXT("编辑策略");
	}
	else
	{
		strDlgTitle = TEXT("新建策略");
	}

	return TRUE;
}

BOOL CDlgTaskEdit::UpdateTaskData( BOOL bCtrlToData /*= TRUE*/ )
{
	if (bCtrlToData)
	{
		
		m_task.vChnTask.clear();
		HTREEITEM hchild = m_TreeChn.GetRootItem();
		BOOL bChnSelOnce = FALSE;
		while (hchild)
		{
			int nIdx = (int) m_TreeChn.GetItemData(hchild);
			if(m_TreeChn.GetCheck(hchild))
			{
				bChnSelOnce = TRUE;
				BOOL bWeekSelOnce = FALSE;
				//channel checked, add to task
				//check if valid;
				ST_ONECHN& oneChnRef = m_vChnCtrlData[nIdx];
				if(oneChnRef.vWeekDays.size() !=7)
				{
					ASSERT(FALSE);
					return FALSE;
				}
				for (int i = 0; i< oneChnRef.vWeekDays.size(); ++i)
				{
					if (oneChnRef.vWeekDays[i].bEnable)
					{
						bWeekSelOnce = TRUE;
						if( oneChnRef.vWeekDays[i].strSecID.IsEmpty())
						{
							CString strMsg;
							strMsg.Format(TEXT("频道%s，星期%d，未选择节目档"), GetChnName(oneChnRef.strChId), i+1);
							AfxMessageBox(strMsg);
							return FALSE;
						}
					}
				}

				if (!bWeekSelOnce)
				{
					CString strMsg;
					strMsg.Format(TEXT("频道%s，未选择任何周天"), GetChnName(oneChnRef.strChId));

					AfxMessageBox(TEXT(strMsg));
					return FALSE;
				}

				m_task.vChnTask.push_back(oneChnRef);

			}
			hchild = m_TreeChn.GetNextItem(hchild, TVGN_NEXT);

		}

		if (!bChnSelOnce)
		{
			CString strMsg;
			strMsg.Format(TEXT("未选择任何频道"));
			AfxMessageBox(TEXT(strMsg));
			return FALSE;

		}


			m_task.bEnableTimer = m_chkTimeGen.GetCheck() == BST_CHECKED;
			CTime tmGen;
			m_dtStartTime.GetTime(tmGen);
			m_task.tTimeToGen = tmGen.GetTime();
			CString strOffDay;
			m_edtOffsetDay.GetWindowText(strOffDay);
			CTxStrConvert offDay(strOffDay);
			m_task.nOffsetDay = offDay.GetAsInt(0);
			if (m_task.nOffsetDay < 0 || m_task.nOffsetDay > 100)
			{
				AfxMessageBox(TEXT("提前日期不合法, (0-100)"));
				m_edtOffsetDay.SetFocus();
			}
			m_task.bMustSubmitted = m_chkMustSubmitted.GetCheck() == BST_CHECKED? 1:0;
			m_task.nWeekLoopMode = m_chkWeekLoopMode.GetCheck() == BST_CHECKED? 1:0;
			m_edtDesc.GetWindowText(m_task.strDesc);
		return TRUE;

	}
	else
	{


		//new mode
		if (m_nEditMode == 0)
		{

			return TRUE;
		}

		HTREEITEM hchild = m_TreeChn.GetRootItem();

		while (hchild)
		{
			int nIdx = (int) m_TreeChn.GetItemData(hchild);
			STCHNSECTIONINFO chInfo = g_GlobalInfo.vChnInfo[nIdx];
			BOOL bFindChn = FALSE;
			int nTaskIdx = -1;
			for (int i = 0; i < m_task.vChnTask.size(); ++i)
			{
				if (m_task.vChnTask[i].strChId.Compare(chInfo.strChnId) == 0)
				{
					nTaskIdx = i;
					bFindChn = TRUE;
					break;
				}
			}

			m_TreeChn.SetCheck(hchild, bFindChn);
			BOOL bcheck = m_TreeChn.GetCheck(hchild);
			if (bFindChn)
			{
				//update weekday data;
				m_vChnCtrlData[nIdx] = m_task.vChnTask[nTaskIdx];
			}

			hchild = m_TreeChn.GetNextItem(hchild, TVGN_NEXT);

		}
		
			m_chkTimeGen.SetCheck(m_task.bEnableTimer? BST_CHECKED:BST_UNCHECKED);
			OnBnClickedCheckTaskgenontime();
			m_chkMustSubmitted.SetCheck(m_task.bMustSubmitted? BST_CHECKED:BST_UNCHECKED);
			m_chkWeekLoopMode.SetCheck(m_task.nWeekLoopMode == 1? BST_CHECKED:BST_UNCHECKED);
			CTxStrConvert offday(m_task.nOffsetDay);
			m_edtOffsetDay.SetWindowText(offday.GetAsString());
			CTime tmGen(m_task.tTimeToGen);
			m_dtStartTime.SetTime(&tmGen);
			m_edtDesc.SetWindowText(m_task.strDesc);
		
	}

	return TRUE;
}

void CDlgTaskEdit::OnCbnSelchangeComboChn()
{
	int nSel = m_cmbChn.GetCurSel();
	if (nSel < 0 || nSel >= g_GlobalInfo.vChnInfo.size())
	{
		//error
		//clear
	}

	if (nSel == nLastChnSel)
	{
		return;
	}

	STCHNSECTIONINFO& chnInfo = g_GlobalInfo.vChnInfo[nSel];
	//refill section
	for (int i = 0; i < 7; ++i)
	{
		CComboBox& cmbBox = *(m_vSectionBox[i]);
		cmbBox.ResetContent();
		for (size_t j = 0; j< chnInfo.vWeekSections[i].vSections.size(); ++j)
		{
			cmbBox.AddString(chnInfo.vWeekSections[i].vSections[j].strSecName);
		}
		cmbBox.SetCurSel(0);
	}
}

void CDlgTaskEdit::OnBnClickedOk()
{
	if (!UpdateTaskData(TRUE))
	{
		return;
	}
	OnOK();
}

void CDlgTaskEdit::OnBnClickedCheckTaskgenontime()
{
	BOOL bChek = m_chkTimeGen.GetCheck() == BST_CHECKED;
	m_edtOffsetDay.EnableWindow(bChek);
	m_dtStartTime.EnableWindow(bChek);
}

void CDlgTaskEdit::OnBnClickedCheckWeekday()
{
	for (int i = 0; i <7; ++i)
	{
		m_vSectionBox[i]->EnableWindow(m_vchkWeekDay[i]->GetCheck() == BST_CHECKED);
	}
	OnCbnSelchangeComboSection();
}

void CDlgTaskEdit::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView->itemNew.hItem == pNMTreeView->itemOld.hItem)
	{
		return;
	}

	HTREEITEM hNewSel = pNMTreeView->itemNew.hItem;
	int nSel = -1;
	if (hNewSel)
	{

		nSel = (int)m_TreeChn.GetItemData(hNewSel);
	}
	
	if (nSel < 0 || nSel >= g_GlobalInfo.vChnInfo.size())
	{
		//error
		//clear
		for (int i = 0; i < 7; ++i)
		{
			CComboBox& cmbBox = *(m_vSectionBox[i]);
			cmbBox.ResetContent();
		}

		return;
	}

	if (nSel == nLastChnSel)
	{
		return;
	}

	
	UpdateWeedayCtrl();


	*pResult = 0;
}

void CDlgTaskEdit::OnCbnSelchangeComboSection()
{
	// TODO: 在此添加控件通知处理程序代码
	//save to 
	HTREEITEM hItem = m_TreeChn.GetSelectedItem();
	if (hItem == NULL)
	{
		ASSERT(FALSE);
		return ;
	}
	int nChnSel = m_TreeChn.GetItemData(hItem);
	if (nChnSel >= 0)
	{
		ST_ONECHN& oneChnRef = m_vChnCtrlData[nChnSel];
		for (int i = 0; i < 7; ++i)
		{
			int nSecSel =m_vSectionBox[i]->GetCurSel();

			STWEEKDAYINFO wk;
			wk.bEnable = m_vchkWeekDay[i]->GetCheck() == BST_CHECKED? 1:0;
			wk.nDay = i;
			if (nSecSel >= 0 && nSecSel < g_GlobalInfo.vChnInfo[nChnSel].vWeekSections[i].vSections.size())
			{
				wk.strSecID = g_GlobalInfo.vChnInfo[nChnSel].vWeekSections[i].vSections[nSecSel].strSecId;
			}
			oneChnRef.vWeekDays[i] = wk;
		}
	}
}

void CDlgTaskEdit::UpdateWeedayCtrl()
{
	HTREEITEM hItem = m_TreeChn.GetSelectedItem();
	if (hItem == NULL)
	{
		//clear all
		for (int i = 0; i < 7; ++i)
		{
			CComboBox& cmbBox = *(m_vSectionBox[i]);
			cmbBox.ResetContent();
		}
		return;
	}
	int nSel = m_TreeChn.GetItemData(hItem);


	STCHNSECTIONINFO& chnInfo = g_GlobalInfo.vChnInfo[nSel];
	m_cmbChn.SelectString(0, chnInfo.strChnName);

	//refill section
	for (int i = 0; i < 7; ++i)
	{
		CComboBox& cmbBox = *(m_vSectionBox[i]);
		cmbBox.ResetContent();
		for (size_t j = 0; j< chnInfo.vWeekSections[i].vSections.size(); ++j)
		{
			cmbBox.AddString(chnInfo.vWeekSections[i].vSections[j].strSecName);
		}
	}

	ST_ONECHN& oneChnRef = m_vChnCtrlData[nSel];
	for (int i = 0; i < 7; ++i)
	{
		m_vchkWeekDay[i]->SetCheck(oneChnRef.vWeekDays[i].bEnable== 0? BST_UNCHECKED:BST_CHECKED);
		if (oneChnRef.vWeekDays[i].strSecID.IsEmpty())
		{
			m_vSectionBox[i]->SetCurSel(-1);
		}
		else
		{
			m_vSectionBox[i]->SelectString(0, GetSectionName(oneChnRef.vWeekDays[i].strSecID, chnInfo.vWeekSections[i].vSections));
		}
		m_vSectionBox[i]->EnableWindow(oneChnRef.vWeekDays[i].bEnable== 0? FALSE:TRUE);
	}
}

void CDlgTaskEdit::OnNMCustomdrawTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	LPNMTVCUSTOMDRAW plvCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult =CDRF_NOTIFYITEMDRAW;
		return;
	}

	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{

		if(plvCD->nmcd.uItemState & CDIS_SELECTED)
		{
			plvCD->clrTextBk = RGB(53,152,255);
			plvCD->clrText = RGB(255,255,255);
			*pResult = 0;
			return;
		}
	}


}
