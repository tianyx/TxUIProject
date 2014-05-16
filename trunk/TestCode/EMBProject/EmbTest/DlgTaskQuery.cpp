// DlgTaskQuery.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTaskQuery.h"

#define STRGETCOLANDTYPEFMT TEXT("select name, xtype from syscolumns where id = object_id('%s')")

// CDlgTaskQuery 对话框

BOOL CDlgTaskQuery::m_bstHeaderInited = FALSE;
ST_QUERYCOLINFO CDlgTaskQuery::m_vInpoolHeaders;
ST_QUERYCOLINFO CDlgTaskQuery::m_vEmbHeaders;
ST_QUERYCOLINFO CDlgTaskQuery::m_vEmbLogHeaders;

IMPLEMENT_DYNAMIC(CDlgTaskQuery, CDialog)

CDlgTaskQuery::CDlgTaskQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTaskQuery::IDD, pParent)
	, m_strQueryEmbTaskID(_T(""))
	, m_strQueryPgmCode(_T(""))
{

}

CDlgTaskQuery::~CDlgTaskQuery()
{
}

void CDlgTaskQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EMBTASKID, m_strQueryEmbTaskID);
	DDX_Text(pDX, IDC_EDIT_PGMCODE, m_strQueryPgmCode);
	DDX_Control(pDX, IDC_LIST1, m_lstInpoolTask);
	DDX_Control(pDX, IDC_LIST2, m_lstEmbTask);
	DDX_Control(pDX, IDC_LIST3, m_lstEmbTaskLog);
}


BEGIN_MESSAGE_MAP(CDlgTaskQuery, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgTaskQuery::OnBnClickedBtnSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CDlgTaskQuery::OnLvnItemchangedList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CDlgTaskQuery::OnLvnItemchangedList2)
END_MESSAGE_MAP()


// CDlgTaskQuery 消息处理程序

void CDlgTaskQuery::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码

	DoSearch();



}

void CDlgTaskQuery::LoadListHeaders()
{
	if (m_bstHeaderInited)
	{
		return;
	}
	m_bstHeaderInited = TRUE;
	CString strSql;
	strSql.Format(STRGETCOLANDTYPEFMT, TEXT("T_IncomingTaskPool"));
	CTxADORecordSet rs(DBKEY_EMB);
	if (rs.ExecuteSQL(strSql))
	{
		while(!rs.IsEOF())
		{
			ST_SQLCOLINFO tmpdata;
			tmpdata.strName = rs.GetVal(TEXT("name")).GetAsString();
			tmpdata.nDataType = rs.GetVal(TEXT("xtype")).GetAsInt(sqldatetype_charmin);
			m_vInpoolHeaders.vCols.push_back(tmpdata);	
			if (tmpdata.strName == TEXT("PGMCode"))
			{
				m_vInpoolHeaders.nPgmcodePos = m_vInpoolHeaders.vCols.size() -1;
			}
			else if (tmpdata.strName == TEXT("TaskID"))
			{
				m_vInpoolHeaders.nEmbTaskIdPos = m_vInpoolHeaders.vCols.size() -1;

			}
			rs.MoveNext();

		}
	}



	strSql.Format(STRGETCOLANDTYPEFMT, TEXT("t_embTask"));
	if (rs.ExecuteSQL(strSql))
	{
		while(!rs.IsEOF())
		{
			ST_SQLCOLINFO tmpdata;
			tmpdata.strName = rs.GetVal(TEXT("name")).GetAsString();
			tmpdata.nDataType = rs.GetVal(TEXT("xtype")).GetAsInt(sqldatetype_charmin);
			m_vEmbHeaders.vCols.push_back(tmpdata);
			if (tmpdata.strName == TEXT("strProgramID"))
			{
				m_vEmbHeaders.nPgmcodePos = m_vInpoolHeaders.vCols.size() -1;
			}
			else if (tmpdata.strName == TEXT("strTaskID"))
			{
				m_vEmbHeaders.nEmbTaskIdPos = m_vInpoolHeaders.vCols.size() -1;

			}
			rs.MoveNext();

		}
	}

	

	strSql.Format(STRGETCOLANDTYPEFMT, TEXT("t_embTaskLog"));
	if (rs.ExecuteSQL(strSql))
	{
		while(!rs.IsEOF())
		{
			ST_SQLCOLINFO tmpdata;
			tmpdata.strName = rs.GetVal(TEXT("name")).GetAsString();
			tmpdata.nDataType = rs.GetVal(TEXT("xtype")).GetAsInt(sqldatetype_charmin);
			m_vEmbLogHeaders.vCols.push_back(tmpdata);
			if (tmpdata.strName == TEXT("strTaskID"))
			{
				m_vEmbHeaders.nEmbTaskIdPos = m_vInpoolHeaders.vCols.size() -1;

			}
			rs.MoveNext();

		}
	}
	
}

void CDlgTaskQuery::DoSearch()
{
	m_lstInpoolTask.DeleteAllItems();
	m_lstEmbTask.DeleteAllItems();
	m_lstEmbTaskLog.DeleteAllItems();

	m_vInpoolTask.clear();	
	UpdateData(TRUE);
	if (!m_strQueryPgmCode.IsEmpty())
	{
		CString strSql;
		if (!m_strQueryEmbTaskID.IsEmpty())
		{
			strSql.Format(TEXT("select * from T_IncomingTaskPool where PGMCode = '%s' and TaskID = '%s'"), m_strQueryPgmCode, m_strQueryEmbTaskID);
		}
		else
		{
			strSql.Format(TEXT("select * from T_IncomingTaskPool where PGMCode = '%s'"), m_strQueryPgmCode);

		}
		CTxADORecordSet rs(DBKEY_EMB);
		if (rs.ExecuteSQL(strSql))
		{
			while(!rs.IsEOF())
			{
				ST_INPOOLTASKQUERYDATA tmpData;
				for (size_t i = 0; i < m_vInpoolHeaders.vCols.size(); ++i)
				{

					tmpData.vData.push_back(GetDataByColInfo(rs, m_vInpoolHeaders.vCols[i]));
					if (i == m_vInpoolHeaders.nPgmcodePos)
					{
						tmpData.strPgmCode = tmpData.vData[tmpData.vData.size() -1];
					}

				}
				m_vInpoolTask.push_back(tmpData);
				rs.MoveNext();
			}
			
		}
				
	}
	else if (!m_strQueryEmbTaskID.IsEmpty())
	{
		ST_INPOOLTASKQUERYDATA inPoolData;
		for (size_t i = 0; i < m_vInpoolHeaders.vCols.size(); ++i)
		{
			inPoolData.vData.push_back(TEXT("-"));
		}

		inPoolData.bDataHolder = TRUE;
		CString strSql;
		strSql.Format(TEXT("select * from t_embTask where strTaskID = '%s'"), m_strQueryEmbTaskID);
		CTxADORecordSet rs(DBKEY_EMB);
		if (rs.ExecuteSQL(strSql))
		{
			while(!rs.IsEOF())
			{
				ST_EMBTASKQUERYDATA embData;
				embData.strEmbTaskID = m_strQueryEmbTaskID;
				for (size_t i = 0; i < m_vEmbHeaders.vCols.size(); ++i)
				{
					embData.vData.push_back(GetDataByColInfo(rs, m_vEmbHeaders.vCols[i]));
				}
				inPoolData.vEmbData.push_back(embData);
				rs.MoveNext();
			}
		}

		if (inPoolData.vEmbData.size() > 0)
		{
			m_vInpoolTask.push_back(inPoolData);
		}

	}
	
	//init first list
	for (size_t i = 0; i < m_vInpoolTask.size(); ++i)
	{
		m_lstInpoolTask.InsertItem(i, TEXT(""));
		for (size_t icol = 0; icol < m_vInpoolHeaders.vCols.size(); ++icol)
		{
			m_lstInpoolTask.SetItemText(i, icol, m_vInpoolTask[i].vData[icol]);
		}
	}

	if (m_lstInpoolTask.GetItemCount() > 0)
	{
		m_lstInpoolTask.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		RefreshList(lstid_inpool);
	}
	
}

void CDlgTaskQuery::RefreshList( int nListId )
{
	if (nListId == lstid_inpool)
	{
		int nItem = -1;
		POSITION pos = m_lstInpoolTask.GetFirstSelectedItemPosition();
		if (pos)
		{
			nItem = m_lstInpoolTask.GetNextSelectedItem(pos);
		}

		if (nItem == -1)
		{
			if (m_vInpoolTask.size() == 1 && m_vInpoolTask[0].bDataHolder)
			{
				nItem = 0;
			}
		}

		if (nItem != m_nInpoolLstSel)
		{
			m_nInpoolLstSel = nItem;
			m_lstEmbTask.DeleteAllItems();
			if (nItem >= 0 && m_vInpoolTask.size() > nItem)
			{
				m_nInpoolLstSel = nItem;
				if (!m_vInpoolTask[nItem].bSubQueryed)
				{
					QueryEmbDBData(m_vInpoolTask[nItem]);
				}
				VECEMBTASKCQUERYDATA& vEmbDataRef = m_vInpoolTask[nItem].vEmbData;
				for (size_t i = 0; i < vEmbDataRef.size(); ++i)
				{
					m_lstEmbTask.InsertItem(i, TEXT(""));
					for (size_t icol = 0; icol < m_vEmbHeaders.vCols.size(); ++icol)
					{
						m_lstEmbTask.SetItemText(i, icol, vEmbDataRef[i].vData[icol]);
					}
				}

			}

			if (m_lstEmbTask.GetItemCount() > 0)
			{
				m_lstEmbTask.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			}

			RefreshList(lstid_embtask);
		}

	}
	else if (nListId == lstid_embtask)
	{
		int nItem = -1;
		POSITION pos = m_lstEmbTask.GetFirstSelectedItemPosition();
		if (pos)
		{
			nItem = m_lstEmbTask.GetNextSelectedItem(pos);
		}

		if (nItem != m_nEmbTaskLstSel)
		{
			m_nEmbTaskLstSel = nItem;
			m_lstEmbTaskLog.DeleteAllItems();
			if (nItem >= 0 && m_nInpoolLstSel >= 0 && m_vInpoolTask.size() > m_nInpoolLstSel
				&& m_vInpoolTask[m_nInpoolLstSel].vEmbData.size() > nItem)
			{
				if (!m_vInpoolTask[m_nInpoolLstSel].vEmbData[nItem].bSubQueryed)
				{
					QueryEmbLogDBData(m_vInpoolTask[m_nInpoolLstSel].vEmbData[nItem]);
				}
				VECEMBLOGDATA vEmbDataRef = m_vInpoolTask[m_nInpoolLstSel].vEmbData[nItem].vLogData;
				for (size_t i = 0; i < vEmbDataRef.size(); ++i)
				{
					m_lstEmbTaskLog.InsertItem(i, TEXT(""));
					for (size_t icol = 0; icol < m_vEmbLogHeaders.vCols.size(); ++icol)
					{
						m_lstEmbTaskLog.SetItemText(i, icol, vEmbDataRef[i].vData[icol]);
					}
				}
			}
		
		}
	}
}

void CDlgTaskQuery::Reset()
{	
	m_nInpoolLstSel = -1;
	m_nEmbTaskLstSel = -1;
	m_nEmbLogLstSel = -1;
	m_vInpoolTask.clear();

}

void CDlgTaskQuery::QueryEmbDBData( ST_INPOOLTASKQUERYDATA& dataInOut )
{
	dataInOut.bSubQueryed = TRUE;
	if (dataInOut.strPgmCode.IsEmpty())
	{
		return;
	}
	CString strSql;
	strSql.Format(TEXT("select * from t_embTask where strProgramID = '%s'"), dataInOut.strPgmCode);
	CTxADORecordSet rs(DBKEY_EMB);
	if (rs.ExecuteSQL(strSql))
	{
		while(!rs.IsEOF())
		{
			ST_EMBTASKQUERYDATA embData;
			embData.strEmbTaskID = dataInOut.strPgmCode;
			for (size_t i = 0; i < m_vEmbHeaders.vCols.size(); ++i)
			{
				embData.vData.push_back(GetDataByColInfo(rs, m_vEmbHeaders.vCols[i]));
			}
			dataInOut.vEmbData.push_back(embData);
			rs.MoveNext();
		}
	}
}

void CDlgTaskQuery::QueryEmbLogDBData( ST_EMBTASKQUERYDATA& dataInOut )
{
	dataInOut.bSubQueryed = TRUE;

	if (dataInOut.strEmbTaskID.IsEmpty())
	{
		return;
	}

	CString strSql;
	strSql.Format(TEXT("select * from T_EMBTaskLog where strTaskID = '%s'"), dataInOut.strEmbTaskID);
	CTxADORecordSet rs(DBKEY_EMB);
	if (rs.ExecuteSQL(strSql))
	{
		while(!rs.IsEOF())
		{
			ST_EMBLOGQUERYDATA embData;
			embData.strEmbTaskID = dataInOut.strEmbTaskID;
			for (size_t i = 0; i < m_vEmbLogHeaders.vCols.size(); ++i)
			{
				embData.vData.push_back(GetDataByColInfo(rs, m_vEmbLogHeaders.vCols[i]));
			}
			dataInOut.vLogData.push_back(embData);
			rs.MoveNext();
		}
	}
}

CString GetDataByColInfo( CTxADORecordSet& rs, ST_SQLCOLINFO& colInfo )
{
	if (colInfo.nDataType == sqldatetype_time)
	{
		return Time2Str(rs.GetVal(colInfo.strName).GetAsInt64());
	}
	else if (colInfo.nDataType == sqldatetype_int
		||colInfo.nDataType == sqldatetype_int64)
	{
		return rs.GetVal(colInfo.strName).GetAsString();
	}
	else
	{
		return rs.GetVal(colInfo.strName).GetAsString();
	}
}

void CDlgTaskQuery::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	RefreshList(lstid_inpool);
}

void CDlgTaskQuery::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	RefreshList(lstid_embtask);
}

BOOL CDlgTaskQuery::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!m_bstHeaderInited)
	{
		LoadListHeaders();
	}
	
	if (m_vEmbHeaders.vCols.size() == 0)
	{
		AfxMessageBox(TEXT("列表初始化失败！"));
		return FALSE;
	}

	m_lstInpoolTask.SetExtendedStyle( (m_lstInpoolTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));
	m_lstEmbTaskLog.SetExtendedStyle( (m_lstEmbTaskLog.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));
	m_lstEmbTask.SetExtendedStyle( (m_lstEmbTask.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));

	InitListHeaders();

	if (!m_strQueryPgmCode.IsEmpty() || !m_strQueryEmbTaskID.IsEmpty())
	{
		UpdateData(FALSE);
		DoSearch();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTaskQuery::InitQueryData( LPCTSTR szPgmCode, LPCTSTR szEmbTaskId )
{
	if (szPgmCode != NULL)
	{
		m_strQueryPgmCode = szPgmCode;

	}
	if (szEmbTaskId != NULL)
	{
		m_strQueryEmbTaskID = szEmbTaskId;

	}
}

void CDlgTaskQuery::InitListHeaders()
{
	for (size_t i = 0; i < m_vInpoolHeaders.vCols.size(); ++i)
	{
		m_lstInpoolTask.InsertColumn(i, m_vInpoolHeaders.vCols[i].strName, LVCFMT_LEFT, 100);
	}
	for (size_t i = 0; i < m_vEmbHeaders.vCols.size(); ++i)
	{
		m_lstEmbTask.InsertColumn(i, m_vEmbHeaders.vCols[i].strName, LVCFMT_LEFT, 100);
	}
	for (size_t i = 0; i < m_vEmbLogHeaders.vCols.size(); ++i)
	{
		m_lstEmbTaskLog.InsertColumn(i, m_vEmbLogHeaders.vCols[i].strName, LVCFMT_LEFT, 100);
	}
}
