// DlgManualTask.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgManualTask.h"
#include "TxADOConn.h"
#include "DlgManualSubmitEmbTask.h"
#include "DlgTaskQuery.h"


// CDlgManualTask 对话框


IMPLEMENT_DYNAMIC(CDlgManualTask, CDialog)

CDlgManualTask::CDlgManualTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgManualTask::IDD, pParent)
{
	m_nColPosTaskid = -1;
}

CDlgManualTask::~CDlgManualTask()
{
}

void CDlgManualTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PGMCODE, m_chkPgmCode);
	DDX_Control(pDX, IDC_EDIT_PGMCODE, m_edtPgmCode);
	DDX_Control(pDX, IDC_CHECK_CLIPNAME, m_chkClipName);
	DDX_Control(pDX, IDC_EDIT_CLIPNAME, m_edtClipName);
	DDX_Control(pDX, IDC_CHECK_TIMETOPLAYRANGE, m_chkTimeRemain);
	DDX_Control(pDX, IDC_CHECK_PLAYLISTRANGE, m_chkPgmListRange);
	DDX_Control(pDX, IDC_CHECK_CHID, m_chkChid);
	DDX_Control(pDX, IDC_CHECK_EMERGNCY, m_chkEmergency);
	DDX_Control(pDX, IDC_CHECK_EMBSTATE, m_chkEmbTaskState);
	DDX_Control(pDX, IDC_EDIT_PLAYTIMEMIN, m_edtPlayTimeMin);
	DDX_Control(pDX, IDC_EDIT_PLAYTIMEMAX, m_edtPlayTimeMax);
	DDX_Control(pDX, IDC_EDIT_PLAYLISTMAX, m_edtPlayListMax);
	DDX_Control(pDX, IDC_EDIT_PLAYLISTMIN, m_edtPlayListMin);
	DDX_Control(pDX, IDC_EDIT_CHID, m_edtChId);
	DDX_Control(pDX, IDC_EDIT_EMERGNCY, m_edtEmergency);
	DDX_Control(pDX, IDC_COMBO1, m_cmbEmbTaskState);
	DDX_Control(pDX, IDC_CHECK_SHOWADVSEARCH, m_chkShowAdv);
	DDX_Control(pDX, IDC_GROUP_SEARCH, m_grpSearch);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
}


BEGIN_MESSAGE_MAP(CDlgManualTask, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgManualTask::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_CHECK_SHOWADVSEARCH, &CDlgManualTask::OnBnClickedCheckShowadvsearch)
	ON_BN_CLICKED(IDC_CHECK_EMERGNCY, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_BN_CLICKED(IDC_CHECK_CHID, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_BN_CLICKED(IDC_CHECK_CLIPNAME, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_BN_CLICKED(IDC_CHECK_PGMCODE, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_BN_CLICKED(IDC_CHECK_PLAYLISTRANGE, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_BN_CLICKED(IDC_CHECK_TIMETOPLAYRANGE, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_BN_CLICKED(IDC_CHECK_EMBSTATE, &CDlgManualTask::OnBnClickedCheckOpt)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TASK, &CDlgManualTask::OnNMRClickListTask)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TASK, &CDlgManualTask::OnNMCustomdrawListTask)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK, &CDlgManualTask::OnNMDblclkListTask)
END_MESSAGE_MAP()


// CDlgManualTask 消息处理程序

LRESULT CDlgManualTask::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (MSG_TASKINFOUPDATE == message)
	{
		if (wParam)
		{
			VECINTS& vItems =*((VECINTS*)wParam);
			for (int i = 0; i< vItems.size(); ++i)
			{
				int nPos = vItems[i];
				RefreshListItem(nPos);
			}
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgManualTask::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN 
		&& (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_F1 || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE )
	{
		CWnd* pWnd = GetFocus();
		if ( !(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit))||pWnd->IsKindOf(RUNTIME_CLASS(CEdit)))) )
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgManualTask::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	DoSearch();
}

void CDlgManualTask::OnBnClickedCheckShowadvsearch()
{
	// TODO: 在此添加控件通知处理程序代码
	Relayout();
}

void CDlgManualTask::OnBnClickedCheckOpt()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshCtrls();
}


void CDlgManualTask::RefreshCtrls()
{
	BOOL bChecked = m_chkTimeRemain.GetCheck() == BST_CHECKED;
	m_edtPlayTimeMin.EnableWindow(bChecked);
	m_edtPlayTimeMax.EnableWindow(bChecked);

	BOOL bAdvSearch = m_chkShowAdv.GetCheck() == BST_CHECKED;
	if (bAdvSearch)
	{

		bChecked = m_chkPgmListRange.GetCheck() == BST_CHECKED;
		m_edtPlayListMin.EnableWindow(bChecked);
		m_edtPlayListMax.EnableWindow(bChecked);

		bChecked = m_chkPgmCode.GetCheck() == BST_CHECKED;
		m_edtPgmCode.EnableWindow(bChecked);

		bChecked = m_chkChid.GetCheck() == BST_CHECKED;
		m_edtChId.EnableWindow(bChecked);

		bChecked = m_chkClipName.GetCheck() == BST_CHECKED;
		m_edtClipName.EnableWindow(bChecked);
	
		bChecked = m_chkEmergency.GetCheck() == BST_CHECKED;
		m_edtEmergency.EnableWindow(bChecked);

		bChecked = m_chkEmbTaskState.GetCheck() == BST_CHECKED;
		m_cmbEmbTaskState.EnableWindow(bChecked);


	}
}

BOOL CDlgManualTask::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_lstTask.SetExtendedStyle( (m_lstTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));

	// TODO:  在此添加额外的初始化
	m_chkTimeRemain.SetCheck(BST_CHECKED);
	m_edtPlayTimeMax.SetWindowText(CTxStrConvert(g_GlobalInfo.nTimeLineOffline).GetAsString());
	m_edtPlayTimeMin.SetWindowText(CTxStrConvert(g_GlobalInfo.nTimelineClose).GetAsString());

	m_chkEmbTaskState.SetCheck(BST_CHECKED);

	m_cmbEmbTaskState.AddString(TEXT("未提交"));
	m_cmbEmbTaskState.AddString(TEXT("已提交"));
	m_cmbEmbTaskState.SetCurSel(0);
	InitListHeader();
	RefreshCtrls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgManualTask::DoSearch()
{

	m_vSearchData.clear();

	CString strSql;
	strSql = TEXT("select * from T_IncomingTaskPool ");
	CString strOpt;
	if (CollectSearchOpt(strOpt))
	{
		strSql += strOpt;
	}
	
	strSql += TEXT(" order by SubmitDateTime");
	CTxADORecordSet rst(DBKEY_EMB);
	if (!rst.ExecuteSQL(strSql))
	{
		CFWriteLog(0,"search sql failed! %s", strSql);
		AfxMessageBox(TEXT("查询失败，请查看日志"));
	}

	while(!rst.IsEOF())
	{
		ST_INPOOLTASKDATA tmpData;
		tmpData.strPgmCode = rst.GetVal(TEXT("PGMCode")).GetAsString();
		tmpData.strClipName = rst.GetVal(TEXT("PGMName")).GetAsString();
		tmpData.nClipType = ExternalClipType2Local(rst.GetVal(TEXT("nClipType")).GetAsString());
		tmpData.nClipSrctype = ExternalClipSourceId2Local(rst.GetVal(TEXT("SourceID")).GetAsString());
		tmpData.strChID =  ExternalChId2LocalChid(rst.GetVal(TEXT("ChnID")).GetAsString());
		tmpData.nDuration = rst.GetVal(TEXT("nDuration")).GetAsInt64();
		tmpData.nPriority = rst.GetVal(TEXT("nPRI")).GetAsInt();
		tmpData.tmSubmit = rst.GetVal(TEXT("SubmitDateTime")).GetAsInt64();
		tmpData.tmPlayTime = rst.GetVal(TEXT("PlayDateTime")).GetAsInt64();
		tmpData.strTaskXml = rst.GetVal(TEXT("SourceStrXML")).GetAsString();
		tmpData.nVersion = rst.GetVal(TEXT("Version")).GetAsInt();
		tmpData.strTaskId = rst.GetVal(TEXT("TaskID")).GetAsString();
		tmpData.nErrcode = rst.GetVal(TEXT("nErrcode")).GetAsInt();

		m_vSearchData.push_back(tmpData);
		rst.MoveNext();
	}
	RefreshTaskList();
	return TRUE;	
}

BOOL CDlgManualTask::CollectSearchOpt(CString& strRetOut)
{
	CString strRet = TEXT(" where ");
	BOOL bSecondOpt  = FALSE;
	CString strTmp;
	if (m_chkTimeRemain.GetCheck() == BST_CHECKED)
	{
		CString strNum;
		m_edtPlayTimeMin.GetWindowText(strNum);
		int nHourMin = atoi(strNum);
		m_edtPlayTimeMax.GetWindowText(strNum);
		int nHourMax = atoi(strNum);
		INT64 tmMin =(time(NULL)+ nHourMin*3600);
		INT64 tmMax =(time(NULL)+ nHourMax*3600);
		strTmp.Format(TEXT("PlayDateTime > '%s' and PlayDateTime < '%s'"), Time2Str(tmMin), Time2Str(tmMax));
		strRet += strTmp;
		bSecondOpt = TRUE;
	}

	if (m_chkShowAdv.GetCheck() == BST_CHECKED)
	{
		if (m_chkPgmCode.GetCheck() == BST_CHECKED)
		{
			if (bSecondOpt)
			{
				strRet += TEXT(" and ");
			}

			CString strText;
			m_edtPgmCode.GetWindowText(strText);
			strTmp.Format(TEXT(" PGMCode like '%%%s%%' "), strText);
			strRet += strTmp;
			bSecondOpt = TRUE;
		}
		
		if (m_chkChid.GetCheck() == BST_CHECKED)
		{
			if (bSecondOpt)
			{
				strRet += TEXT(" and ");
			}

			CString strText;
			m_edtChId.GetWindowText(strText);
			strTmp.Format(TEXT(" ChnID = '%s' "), strText);
			strRet += strTmp;
			bSecondOpt = TRUE;
		}

		if (m_chkClipName.GetCheck() == BST_CHECKED)
		{
			if (bSecondOpt)
			{
				strRet += TEXT(" and ");
			}

			CString strText;
			m_edtClipName.GetWindowText(strText);
			strTmp.Format(TEXT(" PGMName like '%%%s%%' "), strText);
			strRet += strTmp;
			bSecondOpt = TRUE;
		}


		if (m_chkEmergency.GetCheck() == BST_CHECKED)
		{
			if (bSecondOpt)
			{
				strRet += TEXT(" and ");
			}

			CString strNum;
			m_edtEmergency.GetWindowText(strNum);
			int nEmer = atoi(strNum);
			strTmp.Format(TEXT(" nPRI > %d "), nEmer);
			strRet += strTmp;
			bSecondOpt = TRUE;
		}

		if (m_chkEmbTaskState.GetCheck() == BST_CHECKED)
		{
			if (bSecondOpt)
			{
				strRet += TEXT(" and ");
			}

			
			int nSubmitted = m_cmbEmbTaskState.GetCurSel();
			strTmp.Format(TEXT(" TaskID %s null "), nSubmitted == 0? TEXT("is"):TEXT("is not"));
			strRet += strTmp;
			bSecondOpt = TRUE;
		}

	}

	if (bSecondOpt)
	{
		strRetOut = strRet;
	}

	return bSecondOpt;

}

BOOL CDlgManualTask::RefreshTaskList()
{
	m_lstTask.DeleteAllItems();

	for (size_t i = 0; i< m_vSearchData.size(); ++i)
	{
		CString strIdx;
		strIdx.Format(TEXT("%d"), i);
		m_lstTask.InsertItem(i, strIdx);
		for (int j = 1; j < m_vListCols.size(); ++j)
		{
			CString strText = GetTaskColText(m_vListCols[j].nId, m_vSearchData[i]);
			m_lstTask.SetItemText(i, j, strText);
		}
	}
	
	return TRUE;
}

void CDlgManualTask::InitListHeader()
{
	if (g_GlobalInfo.listInfoManualTask.vListHeader.size() == 0)
	{
		ST_LISTHEADERITEM item;
		item.nId = mtaskcol_no;
		item.nWidth = 100;
		item.strName = TEXT("序号");
		m_vListCols.push_back(item);


		item.nId = mtaskcol_pgmcode;
		item.nWidth = 100;
		item.strName = TEXT("节目代码");
		m_vListCols.push_back(item);


		item.nId = mtaskcol_embtaskid;
		item.nWidth = 100;
		item.strName = TEXT("任务ID");
		m_vListCols.push_back(item);


		item.nId = mtaskcol_errcode;
		item.nWidth = 100;
		item.strName = TEXT("错误代码");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_version;
		item.nWidth = 100;
		item.strName = TEXT("版本");
		m_vListCols.push_back(item);



		item.nId = mtaskcol_clipname;
		item.nWidth = 100;
		item.strName = TEXT("节目名称");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_sourcetype;
		item.nWidth = 100;
		item.strName = TEXT("来源");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_submittime;
		item.nWidth = 100;
		item.strName = TEXT("提交时间");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_playtime;
		item.nWidth = 100;
		item.strName = TEXT("素材播出时间");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_chid;
		item.nWidth = 100;
		item.strName = TEXT("频道ID");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_cliptype;
		item.nWidth = 100;
		item.strName = TEXT("素材类型");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_duration;
		item.nWidth = 100;
		item.strName = TEXT("长度");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_npri;
		item.nWidth = 100;
		item.strName = TEXT("紧急度");
		m_vListCols.push_back(item);

		g_GlobalInfo.listInfoManualTask.vListHeader = m_vListCols;
	}
	else
	{
		m_vListCols = g_GlobalInfo.listInfoManualTask.vListHeader;
	}
	


	for (size_t i = 0; i < m_vListCols.size(); ++i)
	{
		ST_LISTHEADERITEM& itmRef = m_vListCols[i];
		m_lstTask.InsertColumn(i, itmRef.strName, LVCFMT_LEFT, itmRef.nWidth);
		if (itmRef.nId == mtaskcol_embtaskid)
		{
			m_nColPosTaskid = i;
		}
	}

}

void CDlgManualTask::OnNMRClickListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	int nCount = m_lstTask.GetSelectedCount();
	if (nCount == 0)
	{
		return;
	}
	
	CMenu popmenu;
	popmenu.LoadMenu(IDR_MENU_TASKLISTPOP);
	CPoint pt;
	GetCursorPos(&pt);
	DWORD dwID = popmenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RETURNCMD, pt.x, pt.y, this);
	if (dwID == ID_MENU_LISTPOP_SUBMIT)
	{
		POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
		VECINTS vSels;
		while(pos)
		{
			int nItem = m_lstTask.GetNextSelectedItem(pos);
			if (m_vSearchData[nItem].strTaskId.IsEmpty())
			{
				vSels.push_back(nItem);
			}
		}

		CDlgManualSubmitEmbTask dlg;
		dlg.m_pvOrgTasks = &m_vSearchData;
		dlg.m_vSubmitIds = vSels;
		dlg.m_hwndNotify = GetSafeHwnd();
		dlg.DoModal();
	}
}

void CDlgManualTask::Relayout()
{
	if (!m_btnSearch.GetSafeHwnd())
	{
		return;
	}
	CRect rcThis;
	GetClientRect(rcThis);
	BOOL bShowAdv = m_chkShowAdv.GetCheck() == BST_CHECKED;

	m_chkClipName.ShowWindow(bShowAdv);
	m_chkPgmCode.ShowWindow(bShowAdv);
	m_chkChid.ShowWindow(bShowAdv);
// 	m_chkEmbTaskState.ShowWindow(bShowAdv);
	m_chkEmergency.ShowWindow(bShowAdv);

	m_edtClipName.ShowWindow(bShowAdv);
	m_edtPgmCode.ShowWindow(bShowAdv);
	m_edtChId.ShowWindow(bShowAdv);
	m_edtEmergency.ShowWindow(bShowAdv);
// 	m_cmbEmbTaskState.ShowWindow(bShowAdv);

	CRect rcAdv(rcThis.right - 120, 0, rcThis.right, 20);
	m_chkShowAdv.MoveWindow(rcAdv);

	CRect rcGroup(rcThis);
	rcGroup.bottom = bShowAdv? 230:100;
	m_grpSearch.MoveWindow(rcGroup);

	CRect rcList(rcThis);
	rcList.top = rcGroup.bottom +5;
	m_lstTask.MoveWindow(rcList);
}



void CDlgManualTask::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	Relayout();
	// TODO: 在此处添加消息处理程序代码
}

void CDlgManualTask::RefreshListItem( int nPos )
{
	if (nPos >= 0 && nPos < m_vSearchData.size())
	{
		ST_INPOOLTASKDATA& orgtaskRef = m_vSearchData[nPos];
		for (int j = 1; j < m_vListCols.size(); ++j)
		{
			CString strText = GetTaskColText(m_vListCols[j].nId, orgtaskRef);
			m_lstTask.SetItemText(nPos, j, strText);
		}
	}
}

void CDlgManualTask::OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LPNMLVCUSTOMDRAW plvCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult =CDRF_NOTIFYITEMDRAW;
		return;
	}
// 	else if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
// 	{
// 		*pResult =CDRF_NOTIFYSUBITEMDRAW;
// 		return;
// 
// 	}

	//if (pNMCD->dwDrawStage == (CDDS_ITEMPREPAINT|CDDS_SUBITEM))
	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		int nId = pNMCD->dwItemSpec;
		int nCol =plvCD->iSubItem;

		if( m_lstTask.GetItemState(nId, LVIS_SELECTED) & LVIS_SELECTED)
		{
			plvCD->clrTextBk = RGB(53,152,255);
			plvCD->clrText = RGB(255,255,255);
			*pResult = 0;
		}
		else
		{
			plvCD->clrTextBk = g_GlobalInfo.listInfoManualTask.clrTextBk;
			plvCD->clrText = g_GlobalInfo.listInfoManualTask.clrText;


		}

		if (m_nColPosTaskid >= 0 && nCol == 0)
		{
			CString strEmbTaskID = m_lstTask.GetItemText(nId, m_nColPosTaskid);
			if (strEmbTaskID == TEXT("#"))
			{
				plvCD->clrTextBk = g_GlobalInfo.listInfoManualTask.clrErrTextBk;
				plvCD->clrText = g_GlobalInfo.listInfoManualTask.clrErrText;

			}
		}

	}
}

void CDlgManualTask::OnNMDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	LVHITTESTINFO hitInfo;
	hitInfo.pt = pNMItemActivate->ptAction;
	m_lstTask.SubItemHitTest(&hitInfo);
	if (hitInfo.flags & LVHT_ONITEMLABEL)
	{

		CString strText = m_lstTask.GetItemText(hitInfo.iItem, hitInfo.iSubItem);
		if(!strText.IsEmpty() &&OpenClipboard())
		{
			int nBufLen = strText.GetLength()+1;
			HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, nBufLen);
			if (hmem)
			{
				void* pBuf = GlobalLock(hmem);
				memset(pBuf, 0, nBufLen);
				strcpy((char*)pBuf, strText);
				GlobalUnlock(hmem);
				EmptyClipboard();
				HANDLE hret = SetClipboardData(CF_TEXT, hmem);
				ASSERT(hret != NULL);
			}

			CloseClipboard();
		}

		if (((GetKeyState(VK_LSHIFT) & 0x8000) != 0) && g_GlobalInfo.bEnableTaskTrace)
		{
			if (hitInfo.iItem >= 0 && hitInfo.iItem < m_vSearchData.size())
			{
				CDlgTaskQuery dlg;
				ST_INPOOLTASKDATA& dataRef =  m_vSearchData[hitInfo.iItem];
				CString strEmbID;
				if (!dataRef.strTaskId.IsEmpty() && dataRef.strTaskId != TEXT("#"))
				{
					strEmbID = dataRef.strTaskId;
				}
				dlg.InitQueryData(dataRef.strPgmCode, strEmbID);
				dlg.DoModal();
			}
			return;
		}
	}

}
