// DlgPlaylistTask.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgPlaylistTask.h"
#include "GlobalDef.h"
#include "TxADOConn.h"
#include "DlgManualSubmitEmbTask.h"
#include "DlgNewEMBTask.h"
#include "DlgTaskQuery.h"

#define STRPLAYSHEETSEARCHFMT TEXT("select strPSID, strSectionGuid, nVersion, strRemark, tLastModifyTime,strLastAuditUser,strLastModifyUser \
from t_ProgramSheet where tPlayDate = '%s' and nState =2 and strSectionGuid in(%s)")


#define STRPLAYCLIPSEARCHFMT TEXT("select t0.strPGMGuid, t0.nPGMDuration, t0.strPGMName, DATEADD(ms, t0.nPGMSchStartTime, t0.tPGMSchPlayDate) as tPGMPlayDatetime, t1.strPrimaryID, t3.strClipID, t3.strLocation \
from t_PlaylistCompile as t0 join t_BIDMapCompile as t1 on t0.strPGMGuid = t1.strPGMGuid left join T_BVSID as t3 on t1.strPrimaryID = t3.strClipLogicID \
where t0.strPSID = '%s'") 


#define STRPLAYCLIPLOCATIONREFRESHFMT TEXT("select strLocation, strClipLogicID from t_bvsid where strClipLogicID in (%s)")

#define STRPLAYCLIPEMBREFRESHFMT TEXT("select strProgramID, nErrorCode, nState from T_EMBTask where nDeleted != 1 and strProgramID in (%s)")

#define STRPLAYCLIPINPOOLREFRESHFMT TEXT("select PGMCode, TaskID, PlayDateTime, nErrcode from T_IncomingTaskPool where PGMCode in (%s)")


// CDlgPlaylistTask 对话框

IMPLEMENT_DYNAMIC(CDlgPlaylistTask, CDialog)

CDlgPlaylistTask::CDlgPlaylistTask(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlaylistTask::IDD, pParent)
{
	m_nChnSel = -1;
	m_nColPosTaskid = -1;
}

CDlgPlaylistTask::~CDlgPlaylistTask()
{
}

void CDlgPlaylistTask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHN, m_cmbChn);
	DDX_Control(pDX, IDC_DATETIME_PLAYLIST, m_tmCtrlDate);
	DDX_Control(pDX, IDC_COMBO_SECTION, m_cmbSection);
	DDX_Control(pDX, IDC_STATIC_PSINFO, m_stcPsInfo);
	DDX_Control(pDX, IDC_LIST_TASK, m_lstTask);
	DDX_Control(pDX, IDC_GROUP_SEARCH, m_grpSearch);
}


BEGIN_MESSAGE_MAP(CDlgPlaylistTask, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CDlgPlaylistTask::OnBnClickedBtnSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_CHN, &CDlgPlaylistTask::OnCbnSelchangeComboChn)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIME_PLAYLIST, &CDlgPlaylistTask::OnDtnDatetimechangeDatetimePlaylist)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION, &CDlgPlaylistTask::OnCbnSelchangeComboSection)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TASK, &CDlgPlaylistTask::OnNMRClickListTask)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_TASK, &CDlgPlaylistTask::OnNMCustomdrawListTask)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TASK, &CDlgPlaylistTask::OnNMDblclkListTask)
END_MESSAGE_MAP()


// CDlgPlaylistTask 消息处理程序

void CDlgPlaylistTask::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	DoSearch();
}

BOOL CDlgPlaylistTask::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//init chn combo
	for (size_t i = 0; i < g_GlobalInfo.vChnInfo.size(); ++i)
	{
		m_cmbChn.AddString(g_GlobalInfo.vChnInfo[i].strChnName);
	}
	m_cmbChn.SetCurSel(0);
	InitListHeader();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPlaylistTask::OnCbnSelchangeComboChn()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshSectionList();
}

void CDlgPlaylistTask::OnDtnDatetimechangeDatetimePlaylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	RefreshSectionList();
}

void CDlgPlaylistTask::RefreshSectionList()
{
	m_cmbSection.ResetContent();
	m_vSelPSIs.clear();
	m_nChnSel = m_cmbChn.GetCurSel();
	if (m_nChnSel < 0 || m_nChnSel >= g_GlobalInfo.vChnInfo.size())
	{
		//error
		return;
	}

	m_tmCtrlDate.GetTime(m_tmSel);
	int nWeekDay = m_tmSel.GetDayOfWeek() -1;

	CString strSecList;
	STCHNSECTIONINFO& chnInfo = g_GlobalInfo.vChnInfo[m_nChnSel];
	for (size_t j = 0; j< chnInfo.vWeekSections[nWeekDay].vSections.size(); ++j)
	{	
		if (j > 0)
		{
			strSecList += TEXT(", ");
		}
		strSecList +=TEXT("'") + chnInfo.vWeekSections[nWeekDay].vSections[j].strSecId + TEXT("'");
	}

	if (strSecList.IsEmpty())
	{
		return;
	}

	CString strSql;
	strSql.Format(STRPLAYSHEETSEARCHFMT, m_tmSel.Format(TEXT("%Y-%m-%d 00:00:00")), strSecList);

	CTxADORecordSet rs(DBKEY_PLAYLIST);
	if (!rs.ExecuteSQL(strSql))
	{
		CFWriteLog(0, TEXT("execute sql error! sql = %s"), strSql);
		return;
	}
	else
	{
		while(!rs.IsEOF())
		{
			ST_PLAYSHEETINFO psinfo;
			psinfo.strPSId = rs.GetVal(TEXT("strPSID")).GetAsString();
			psinfo.strPlayListVersion = rs.GetVal(TEXT("strPlayListVersion")).GetAsString();
			psinfo.strLastAuditUser = rs.GetVal(TEXT("strLastAuditUser")).GetAsString();
			psinfo.strLastModifyUser = rs.GetVal(TEXT("strLastModifyUser")).GetAsString();
			psinfo.strRemark = rs.GetVal(TEXT("strRemark")).GetAsString();
			psinfo.strSectionId = rs.GetVal(TEXT("strSectionGuid")).GetAsString();
			psinfo.tmlastModify = rs.GetVal(TEXT("tLastModifyTime")).GetAsInt64();
			m_vSelPSIs.push_back(psinfo);
			rs.MoveNext();
		}
	}

	for (size_t i = 0; i< m_vSelPSIs.size(); ++i)
	{
		m_cmbSection.AddString(GetSectionName(m_vSelPSIs[i].strSectionId, chnInfo.vWeekSections[nWeekDay].vSections));
	}
	m_cmbSection.SetCurSel(0);
	RefreshPsInfo();
	
}

void CDlgPlaylistTask::RefreshPsInfo()
{
	int nSel = m_cmbSection.GetCurSel();
	CString strText;
	if (nSel >= 0 && nSel < m_vSelPSIs.size())
	{
		ST_PLAYSHEETINFO& psRef = m_vSelPSIs[nSel];
		strText.Format(TEXT("版本：%s, 审批：%s, 修改：%s/%s, 提交人：%s"), psRef.strPlayListVersion,
			psRef.strRemark, psRef.strLastModifyUser, Time2Str(psRef.tmlastModify), psRef.strLastAuditUser);
	}
	m_stcPsInfo.SetWindowText(strText);
}

void CDlgPlaylistTask::OnCbnSelchangeComboSection()
{
	// TODO: 在此添加控件通知处理程序代码
	RefreshPsInfo();
}

void CDlgPlaylistTask::InitListHeader()
{
	if (g_GlobalInfo.listInfoPlaylistTask.vListHeader.size() == 0)
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

		item.nId = mtaskcol_clipname;
		item.nWidth = 100;
		item.strName = TEXT("节目名称");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_clipid;
		item.nWidth = 100;
		item.strName = TEXT("素材ID");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_playtime;
		item.nWidth = 100;
		item.strName = TEXT("素材播出时间");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_clipstate;
		item.nWidth = 100;
		item.strName = TEXT("素材状态");
		m_vListCols.push_back(item);



		item.nId = mtaskcol_strLocation1;
		item.nWidth = 100;
		item.strName = TEXT("存储DB1");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_strLocation2;
		item.nWidth = 100;
		item.strName = TEXT("存储DB2");
		m_vListCols.push_back(item);


		item.nId = mtaskcol_embtaskid;
		item.nWidth = 100;
		item.strName = TEXT("EMB任务ID");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_embstate;
		item.nWidth = 100;
		item.strName = TEXT("emb任务状态");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_emberrcode;
		item.nWidth = 100;
		item.strName = TEXT("错误代码");
		m_vListCols.push_back(item);



		item.nId = mtaskcol_inpoolstate;
		item.nWidth = 100;
		item.strName = TEXT("传入任务状态");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_inpoolplaytime;
		item.nWidth = 100;
		item.strName = TEXT("传入任务播出时间");
		m_vListCols.push_back(item);

		item.nId = mtaskcol_inpoolerrcode;
		item.nWidth = 100;
		item.strName = TEXT("传入任务错误代码");
		m_vListCols.push_back(item);
		g_GlobalInfo.listInfoPlaylistTask.vListHeader = m_vListCols;
	}
	else
	{
		m_vListCols = g_GlobalInfo.listInfoPlaylistTask.vListHeader;
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

BOOL CDlgPlaylistTask::DoSearch()
{
	int nSectionSel = m_cmbSection.GetCurSel();
	if (nSectionSel == -1)
	{
		AfxMessageBox(TEXT("请选择节目单"));
		return FALSE;
	}

	m_vPSClips.clear();
	m_lstTask.DeleteAllItems();

	//get clips in playlist
	STCHNSECTIONINFO& chnInfo = g_GlobalInfo.vChnInfo[m_nChnSel];
	CString strSql;
	strSql.Format(STRPLAYCLIPSEARCHFMT, m_vSelPSIs[nSectionSel].strPSId);
	CTxADORecordSet rs(DBKEY_PLAYLIST);
	if (!rs.ExecuteSQL(strSql))
	{
		CFWriteLog(0,TEXT("exec sql err: %s"), strSql);
		return FALSE;
	}

	while(!rs.IsEOF())
	{
		ST_PSCLIPINFO tmpclip;
		tmpclip.strPgmGuid = rs.GetVal(TEXT("strPGMGuid")).GetAsString();
		tmpclip.strPgmName = rs.GetVal(TEXT("strPGMName")).GetAsString();
		tmpclip.nPgmDuration = rs.GetVal(TEXT("nPGMDuration")).GetAsInt();
		tmpclip.tPgmPlaytime = rs.GetVal(TEXT("tPGMPlayDatetime")).GetAsInt64();
		tmpclip.strPgmCode = rs.GetVal(TEXT("strPrimaryID")).GetAsString();
		tmpclip.strClipID = rs.GetVal(TEXT("strClipID")).GetAsString();
		tmpclip.strLocation1 = rs.GetVal(TEXT("strLocation")).GetAsString();
		tmpclip.nClipState = tmpclip.strLocation1.IsEmpty()? 0:psclipstate_inLoc1;
		m_vPSClips.push_back(tmpclip);
		rs.MoveNext();
	}
	
	RefreshPsClipState(refreshclipflag_notreadyonly|refreshclipflag_db2|refreshclipflag_emb|refreshclipflag_intaskpool);
	RefreshList();
	return TRUE;
}

void CDlgPlaylistTask::RefreshPsClipState(int nRefreshFlag)
{
	
	CString strPgmCodes = GetDesiredClips((nRefreshFlag&refreshclipflag_notreadyonly)!= 0?   (clipfinderflag_notindb1|clipfinderflag_notindb2) : clipfinderflag_all);
	if (refreshclipflag_db1 & nRefreshFlag)
	{
		//refresh loc in db1
		CString strSql;
		strSql.Format(STRPLAYCLIPLOCATIONREFRESHFMT, strPgmCodes);
		CTxADORecordSet rs(DBKEY_PLAYLIST);
		if (rs.ExecuteSQL(strSql))
		{
			while(!rs.IsEOF())
			{

				ST_PSCLIPINFO tmpclip;
				CString strPgmcode =   rs.GetVal(TEXT("strClipLogicID")).GetAsString();
				ASSERT(!strPgmcode.IsEmpty());
				//find clip
				for (size_t i = 0; i < m_vPSClips.size(); ++i)
				{
					if (strPgmcode.Compare(m_vPSClips[i].strPgmCode) == 0)
					{
						ST_PSCLIPINFO& tmpclipRef = m_vPSClips[i];
						tmpclipRef.strLocation1 = rs.GetVal(TEXT("strLocation")).GetAsString();
						tmpclipRef.nClipState = tmpclipRef.strLocation1.IsEmpty()? (tmpclipRef.nClipState & ~psclipstate_inLoc1):(tmpclipRef.nClipState|psclipstate_inLoc1);
						break;
					}
				}
				
				rs.MoveNext();
			}
		}
		else
		{
			CFWriteLog(0,TEXT("exec sql err: %s"), strSql);
			return;
		}
	}


	if (refreshclipflag_db2 & nRefreshFlag)
	{
		//refresh loc in db1
		CString strSql;
		strSql.Format(STRPLAYCLIPLOCATIONREFRESHFMT, strPgmCodes);
		CTxADORecordSet rs(DBKEY_PLAYLIST2);
		if (rs.ExecuteSQL(strSql))
		{
			while(!rs.IsEOF())
			{

				ST_PSCLIPINFO tmpclip;
				CString strPgmcode =   rs.GetVal(TEXT("strClipLogicID")).GetAsString();
				ASSERT(!strPgmcode.IsEmpty());
				//find clip
				for (size_t i = 0; i < m_vPSClips.size(); ++i)
				{
					if (strPgmcode.Compare(m_vPSClips[i].strPgmCode) == 0)
					{
						ST_PSCLIPINFO& tmpclipRef = m_vPSClips[i];
						tmpclipRef.strLocation1 = rs.GetVal(TEXT("strLocation")).GetAsString();
						tmpclipRef.nClipState = tmpclipRef.strLocation2.IsEmpty()? (tmpclipRef.nClipState & ~psclipstate_inLoc2):(tmpclipRef.nClipState|psclipstate_inLoc2);
						break;
					}
				}

				rs.MoveNext();
			}
		}
		else
		{
			CFWriteLog(0,TEXT("exec sql err: %s"), strSql);
			return;
		}
	}


	if ((nRefreshFlag & (refreshclipflag_emb| refreshclipflag_intaskpool)) == 0)
	{
		return;
	}

	//get not ready clip
	VECPSCLIPPTRS vClipPtrs;
	strPgmCodes = GetDesiredClips(clipfinderflag_notindb1|clipfinderflag_notindb2, &vClipPtrs);

	//refresh in emb
	if (nRefreshFlag & refreshclipflag_emb)
	{
		//reset emb state
		for (size_t i = 0; i < vClipPtrs.size(); ++i)
		{
			vClipPtrs[i]->nClipState &= ~psclipstate_inEmbtask;
		}

		CString strSql;
		strSql.Format(STRPLAYCLIPEMBREFRESHFMT, strPgmCodes);
		CTxADORecordSet rs(DBKEY_EMB);
		if (rs.ExecuteSQL(strSql))
		{
			while(!rs.IsEOF())
			{

				ST_PSCLIPINFO tmpclip;
				CString strPgmcode =   rs.GetVal(TEXT("strProgramID")).GetAsString();
				ASSERT(!strPgmcode.IsEmpty());
				//find clip
				for (size_t i = 0; i < vClipPtrs.size(); ++i)
				{
					if (strPgmcode.Compare(vClipPtrs[i]->strPgmCode) == 0)
					{
						ST_PSCLIPINFO& tmpclipRef = *(vClipPtrs[i]);
						tmpclipRef.nEMBErrCode = rs.GetVal(TEXT("nErrorCode")).GetAsInt();
						tmpclipRef.nEMBState = rs.GetVal(TEXT("nState")).GetAsInt();
						tmpclipRef.nClipState |= psclipstate_inEmbtask;
						break;
					}
				}

				rs.MoveNext();
			}
		}
		else
		{
			CFWriteLog(0,TEXT("exec sql err: %s"), strSql);
			return;
		}
	}

	//refresh in incoming task pool
	if (nRefreshFlag & refreshclipflag_intaskpool)
	{
		//reset inpool state
		for (size_t i = 0; i < vClipPtrs.size(); ++i)
		{
			vClipPtrs[i]->nClipState &= ~psclipstate_inIntaskpool;
		}

		CString strSql;
		strSql.Format(STRPLAYCLIPEMBREFRESHFMT, strPgmCodes);
		CTxADORecordSet rs(DBKEY_EMB);
		if (rs.ExecuteSQL(strSql))
		{
			while(!rs.IsEOF())
			{

				ST_PSCLIPINFO tmpclip;
				CString strPgmcode =   rs.GetVal(TEXT("PGMCode")).GetAsString();
				ASSERT(!strPgmcode.IsEmpty());
				//find clip
				for (size_t i = 0; i < vClipPtrs.size(); ++i)
				{
					if (strPgmcode.Compare(vClipPtrs[i]->strPgmCode) == 0)
					{
						ST_PSCLIPINFO& tmpclipRef = *(vClipPtrs[i]);
						tmpclipRef.nInPoolErrCode = rs.GetVal(TEXT("nErrcode")).GetAsInt();
						tmpclipRef.tInPoolPlaytime = rs.GetVal(TEXT("PlayDateTime")).GetAsInt64();
						tmpclipRef.strInPoolEmbtaskId = rs.GetVal(TEXT("TaskID")).GetAsString();
						tmpclipRef.nClipState |= psclipstate_inIntaskpool;
						break;
					}
				}

				rs.MoveNext();
			}
		}
		else
		{
			CFWriteLog(0,TEXT("exec sql err: %s"), strSql);
			return ;
		}
	}
}

CString CDlgPlaylistTask::GetDesiredClips(int nfindFlag, VECPSCLIPPTRS* pvPgmcodeOut/* = NULL*/)
{
	int nDbflag = (nfindFlag & clipfinderflag_notindb1) == 0? 0:psclipstate_inLoc1;
	nDbflag = (nfindFlag & clipfinderflag_notindb2) == 0? nDbflag:(nDbflag|psclipstate_inLoc2);

	CString strPgmCodes;
	for (size_t i = 0; i < m_vPSClips.size(); ++i)
	{
		ST_PSCLIPINFO& tmpclipRef = m_vPSClips[i];
		if (nfindFlag != 0 && (tmpclipRef.nClipState & nDbflag) != 0)
		{
			continue;
		}

		if (pvPgmcodeOut)
		{
			pvPgmcodeOut->push_back(&tmpclipRef);
		}
		if (!strPgmCodes.IsEmpty())
		{
			strPgmCodes += TEXT(", ");
		}
		strPgmCodes += TEXT("'") + tmpclipRef.strPgmCode + TEXT("'");
	}
	
	return strPgmCodes;
}

CString CDlgPlaylistTask::GetPSClipColText( const int nColTypeIn, ST_PSCLIPINFO& dataIn )
{
	CString strRet;
	if (nColTypeIn == mtaskcol_pgmcode)
	{
		return dataIn.strPgmCode;
	}
	else if (nColTypeIn == mtaskcol_clipname)
	{
		return dataIn.strClipName;
	}
	else if (nColTypeIn == mtaskcol_playtime)
	{
		return Time2Str(dataIn.tPgmPlaytime);
	}
	else if (nColTypeIn == mtaskcol_clipstate)
	{
		if (dataIn.nClipState == psclipstate_unknow)
		{
			return strRet;
		}
		
		if ((dataIn.nClipState & psclipstate_inLoc1) ||
			(dataIn.nClipState & psclipstate_inLoc1))
		{
			return TEXT("已入库");
		}
		
		if (dataIn.nClipState & psclipstate_inIntaskpool)
		{
			strRet = TEXT("已导入");
		}
		if (dataIn.nClipState & psclipstate_inEmbtask)
		{
			strRet = TEXT("已提交EMB");
		}
		
		return strRet;

	}
	else if (nColTypeIn == mtaskcol_clipid)
	{
		return dataIn.strClipID;
	}
	else if (nColTypeIn == mtaskcol_strLocation1)
	{
		return dataIn.strLocation1;
	}
	else if (nColTypeIn == mtaskcol_strLocation2)
	{
		return dataIn.strLocation2;
	}
	else if (nColTypeIn == mtaskcol_embtaskid)
	{
		return dataIn.strEmbTaskId;
	}
	else if (nColTypeIn == mtaskcol_embstate)
	{
		if ((dataIn.nClipState & psclipstate_inEmbtask) == 0)
		{
			return TEXT("");
		}
		if (dataIn.nEMBState == 0)
		{
			strRet = TEXT("排队中");
		}
		else if (dataIn.nEMBState == 1)
		{
			strRet = TEXT("执行中");
		}
		else if (dataIn.nEMBState == 3)
		{
			strRet = TEXT("已完成");
		}
		else if (dataIn.nEMBState == 99)
		{
			strRet = TEXT("任务失败");
		}
		return strRet;
	}
	else if (nColTypeIn == mtaskcol_emberrcode)
	{
		if ((dataIn.nClipState & psclipstate_inEmbtask) == 0)
		{
			return TEXT("");
		}
		else
		{
			return CTxStrConvert(dataIn.nEMBErrCode).GetAsString();
		}
	}
	else if (nColTypeIn == mtaskcol_inpoolstate)
	{
		if ((dataIn.nClipState & psclipstate_inIntaskpool) == 0)
		{
			return TEXT("");
		}
		if (dataIn.nInPoolErrCode != 0)
		{
			strRet = TEXT("任务提交失败");
		}
		else
		{
			if (dataIn.strInPoolEmbtaskId.IsEmpty())
			{
				strRet = TEXT("离线");
			}
			else
			{
				strRet = TEXT("已提交");
			}
		}
		
	}
	else if (nColTypeIn == mtaskcol_inpoolplaytime)
	{
		if (dataIn.tInPoolPlaytime == 0)
		{
			return TEXT("");
		}
		else
		{
			return Time2Str(dataIn.tInPoolPlaytime);
		}
	}
	else if (nColTypeIn == mtaskcol_inpoolerrcode)
	{
		if ((dataIn.nClipState & psclipstate_inIntaskpool) == 0)
		{
			return TEXT("");
		}
		else
		{
			return CTxStrConvert(dataIn.nInPoolErrCode).GetAsString();

		}
	}

	return strRet;
	
}

void CDlgPlaylistTask::RefreshList( BOOL bUpdateStateOnly /*= FALSE*/ )
{
	if (!bUpdateStateOnly)
	{
		m_lstTask.DeleteAllItems();
		for (size_t i = 0; i < m_vPSClips.size(); ++i)
		{
			CString strIdx;
			strIdx.Format(TEXT("%d"), i);
			m_lstTask.InsertItem(i, strIdx);
		}
	}
	
	if (m_lstTask.GetItemCount() != m_vPSClips.size())
	{
		ASSERT(FALSE);
		return;
	}

	for (size_t i = 0; i < m_vPSClips.size(); ++i)
	{
		for (int j = 1; j < m_vListCols.size(); ++j)
		{

			CString strText = GetPSClipColText(m_vListCols[j].nId, m_vPSClips[i]);
			m_lstTask.SetItemText(i, j, strText);
		}
	}

}

void CDlgPlaylistTask::OnNMRClickListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nCount = m_lstTask.GetSelectedCount();
	if (nCount == 0)
	{
		return;
	}

	if (nCount ==1)
	{
		//check if empty task
		POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
		int nSelItem = m_lstTask.GetNextSelectedItem(pos);
		if (nSelItem < 0 ||nSelItem >= m_vPSClips.size())
		{
			return;
		}
		ST_PSCLIPINFO& cpRef = m_vPSClips[nSelItem];
		if (cpRef.nClipState == psclipstate_unknow)
		{
			//do pop new task menu
			CDlgNewEMBTask dlg;
			dlg.m_pClipInfo = &cpRef;
			dlg.m_hwndNotify = GetSafeHwnd();
			return;

		}
	}

	//do manual submit menu
	VECSTRINGS vPgms;
	int nPgmCount =GetSelInPoolIdleTask(vPgms);
	if (nPgmCount == 0)
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
		VECINPOOLTASK vOrgTasks;
		GetClipInPoolTaskInfo(vPgms, vOrgTasks);
		VECINTS vSels;
		for (int i = 0; i < vOrgTasks.size(); ++i)
		{
			vSels.push_back(i);
		}

		CDlgManualSubmitEmbTask dlg;
		dlg.m_pvOrgTasks = &vOrgTasks;
		dlg.m_vSubmitIds = vSels;
		dlg.m_hwndNotify = GetSafeHwnd();
		dlg.DoModal();
	}
}

int CDlgPlaylistTask::GetSelInPoolIdleTask( VECSTRINGS& vPgmOut )
{
	POSITION pos = m_lstTask.GetFirstSelectedItemPosition();
	VECINTS vSels;
	while(pos)
	{
		ST_PSCLIPINFO& cpRef = m_vPSClips[m_lstTask.GetNextSelectedItem(pos)];
		if (cpRef.nClipState == psclipstate_inIntaskpool &&
			cpRef.strInPoolEmbtaskId.IsEmpty())
		{
			vPgmOut.push_back(cpRef.strPgmCode);
		}
	}

	return vPgmOut.size();
}

void CDlgPlaylistTask::GetClipInPoolTaskInfo( VECSTRINGS& vPgmIn, VECINPOOLTASK& vTaskOut )
{
	CString strPgmCodes;
	for (size_t i = 0; i < vPgmIn.size(); ++i)
	{
		ST_PSCLIPINFO& tmpclipRef = m_vPSClips[i];
		if (!strPgmCodes.IsEmpty())
		{
			strPgmCodes += TEXT(", ");
		}
		strPgmCodes += TEXT("'") + vPgmIn[i] + TEXT("'");
	}

	CString strSql;
	strSql.Format(TEXT("select * from T_IncomingTaskPool where PGMCode in (%s)"), strPgmCodes);
	CTxADORecordSet rst(DBKEY_EMB);
	if (!rst.ExecuteSQL(strSql))
	{
		CFWriteLog(0,"search sql failed! %s", strSql);
		return;
	}

	while(!rst.IsEOF())
	{
		ST_INPOOLTASKDATA tmpData;
		tmpData.strPgmCode = rst.GetVal(TEXT("PGMCode")).GetAsString();
		tmpData.strClipName = rst.GetVal(TEXT("PGMName")).GetAsString();
		tmpData.nClipType = ExternalClipType2Local(rst.GetVal(TEXT("nClipType")).GetAsString());
		tmpData.nClipSrctype =ExternalClipSourceId2Local(rst.GetVal(TEXT("SourceID")).GetAsString());
		tmpData.strChID = ExternalChId2LocalChid(rst.GetVal(TEXT("ChnID")).GetAsString());
		tmpData.nDuration = rst.GetVal(TEXT("nDuration")).GetAsInt64();
		tmpData.nPriority = rst.GetVal(TEXT("nPRI")).GetAsInt();
		tmpData.tmSubmit = rst.GetVal(TEXT("SubmitDateTime")).GetAsInt64();
		tmpData.tmPlayTime = rst.GetVal(TEXT("PlayDateTime")).GetAsInt64();
		tmpData.strTaskXml = rst.GetVal(TEXT("SourceStrXML")).GetAsString();
		tmpData.nVersion = rst.GetVal(TEXT("Version")).GetAsInt();
		tmpData.strTaskId = rst.GetVal(TEXT("TaskID")).GetAsString();
		tmpData.nErrcode = rst.GetVal(TEXT("nErrcode")).GetAsInt();

		vTaskOut.push_back(tmpData);
		rst.MoveNext();
	}
}

void CDlgPlaylistTask::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Relayout();

}

void CDlgPlaylistTask::Relayout()
{
	if (!m_grpSearch.GetSafeHwnd())
	{
		return;
	}

	CRect rcthis;
	GetClientRect(rcthis);

	CRect rcGrp(rcthis);
	rcGrp.bottom = 180;
	m_grpSearch.MoveWindow(rcGrp);

	CRect rcLst(rcthis);
	rcLst.top = rcGrp.bottom +5;
	m_lstTask.MoveWindow(rcLst);

}

LRESULT CDlgPlaylistTask::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
		return TRUE;
	}
	else if (MSG_NEWEMBTASKINFOUPDATE == message)
	{
		if (lParam)
		{
			ST_PSCLIPINFO& orgData = *((ST_PSCLIPINFO*)wParam);
			if (!orgData.strPgmCode.IsEmpty())
			{
				int nPos = GetItemPosByPgmcode(orgData.strPgmCode);
				RefreshListItem(nPos);
			}
		}
		return TRUE;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CDlgPlaylistTask::RefreshListItem( int nPos )
{
	if (nPos >= 0 && nPos < m_vPSClips.size())
	{
		ST_PSCLIPINFO& orgtaskRef = m_vPSClips[nPos];
		for (int j = 1; j < m_vListCols.size(); ++j)
		{
			CString strText = GetPSClipColText(m_vListCols[j].nId, orgtaskRef);
			m_lstTask.SetItemText(nPos, j, strText);
		}
	}
}

int CDlgPlaylistTask::GetItemPosByPgmcode( LPCTSTR strPgmCode )
{
	if (strPgmCode == NULL)
	{
		return -1;
	}

	for (int i = 0; i < m_vPSClips.size(); ++i)
	{
		if (m_vPSClips[i].strPgmCode.Compare(strPgmCode) == 0)
		{
			return i;
		}
	}

	return -1;
}

void CDlgPlaylistTask::OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult)
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
			plvCD->clrTextBk = g_GlobalInfo.listInfoPlaylistTask.clrTextBk;
			plvCD->clrText = g_GlobalInfo.listInfoPlaylistTask.clrText;


		}

		if (nCol == 0)
		{
			UINT nClrText = 0;
			UINT nClrtextBk = 0;
			if (GetItemTextClrByVal(nId, nClrText, nClrtextBk))
			{
				plvCD->clrTextBk = nClrtextBk;
				plvCD->clrText = nClrText;

			}
		}

	}

}

BOOL CDlgPlaylistTask::GetItemTextClrByVal(const int nIDIn, UINT& clrtext, UINT& clrtextBk )
{
	BOOL bRet = FALSE;
	if (nIDIn >= 0 && nIDIn < m_vPSClips.size())
	{
		ST_PSCLIPINFO& orgtaskRef = m_vPSClips[nIDIn];
		if (orgtaskRef.nClipState == psclipstate_inEmbtask)
		{
			if (orgtaskRef.nEMBState == 99)
			{
				clrtext = g_GlobalInfo.listInfoPlaylistTask.clrErrText;
				clrtextBk = g_GlobalInfo.listInfoPlaylistTask.clrErrTextBk;
				bRet = TRUE;
			}
		}

	}

	return bRet;
}

void CDlgPlaylistTask::OnNMDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult)
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
			if (hitInfo.iItem >= 0 && hitInfo.iItem < m_vPSClips.size())
			{
				CDlgTaskQuery dlg;
				ST_PSCLIPINFO& dataRef =  m_vPSClips[hitInfo.iItem];
				CString strEmbID;
				if (!dataRef.strEmbTaskId.IsEmpty() && dataRef.strEmbTaskId != TEXT("#"))
				{
					strEmbID = dataRef.strEmbTaskId;
				}
				dlg.InitQueryData(dataRef.strPgmCode, strEmbID);
				dlg.DoModal();
			}
			return;
		}
	}
}
