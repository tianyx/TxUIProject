// DlgTest.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgTest.h"
#include "GlobalDef.h"
#include "LuaStrategy.h"
#include "io.h"
#include "TxADOConn.h"


// CDlgTest 对话框

IMPLEMENT_DYNAMIC(CDlgTest, CDialog)

CDlgTest::CDlgTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest::IDD, pParent)
{

}

CDlgTest::~CDlgTest()
{
}

void CDlgTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_edtLog);
	DDX_Control(pDX, IDC_CMB_STG, m_cmbStg);
	DDX_Control(pDX, IDC_LIST_LUAKT, m_lstLuaKt);
	DDX_Control(pDX, IDC_DATETIME_TIME, m_tmCtlPlaydate);
	DDX_Control(pDX, IDC_EDIT_CHID, m_edtChid);
	DDX_Control(pDX, IDC_EDIT_TYPE, m_edtClipType);
	DDX_Control(pDX, IDC_EDIT_DURATION, m_edtClipDur);
	DDX_Control(pDX, IDC_DATETIME_TIME2, m_tmCtlPlaytime2);
	DDX_Control(pDX, IDC_EDIT_CHID2, m_edtChid2);
	DDX_Control(pDX, IDC_EDIT_TYPE2, m_edtCliptype2);
	DDX_Control(pDX, IDC_EDIT_DURATION2, m_edtDur2);
	DDX_Control(pDX, IDC_EDIT_SRCID, m_edtClipSrcId);
	DDX_Control(pDX, IDC_EDIT_INCOMINGTPL, m_edtTpXml);
	DDX_Control(pDX, IDC_EDIT_DB, m_edtDbConn);
	DDX_Control(pDX, IDC_EDIT_DB2, m_edtPri);
	DDX_Control(pDX, IDC_EDIT_FILE, m_edtOutfile);
}


BEGIN_MESSAGE_MAP(CDlgTest, CDialog)
	ON_BN_CLICKED(IDC_BTN_GETCONFIG, &CDlgTest::OnBnClickedBtnGetconfig)
	ON_CBN_SELCHANGE(IDC_CMB_STG, &CDlgTest::OnCbnSelchangeCmbStg)
	ON_BN_CLICKED(IDC_LUA_TEST, &CDlgTest::OnBnClickedLuaTest)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CDlgTest::OnBnClickedBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_COMMIT, &CDlgTest::OnBnClickedBtnCommit)
END_MESSAGE_MAP()


// CDlgTest 消息处理程序

void CDlgTest::OnBnClickedBtnGetconfig()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strXml;
	g_GlobalInfo.ToString(strXml);
	m_edtLog.SetWindowText(strXml);
}

void CDlgTest::InitCtrl()
{	
	DateTime_SetFormat(m_tmCtlPlaydate.GetSafeHwnd(), TEXT("yyyy-MM-dd HH:mm:ss"));
	DateTime_SetFormat(m_tmCtlPlaytime2.GetSafeHwnd(), TEXT("yyyy-MM-dd HH:mm:ss"));

	for (size_t i = 0; i < g_GlobalInfo.stgConfig.vStgs.size(); ++i)
	{
		ST_STGPARAM& stgRef = g_GlobalInfo.stgConfig.vStgs[i];
		CString strStg;
		strStg.Format(TEXT("%s,type=%d, pri=%d, enable=%d, valid=%d"), 
			stgRef.strName, stgRef.nStgType, stgRef.nStgPriority, stgRef.bEnable, stgRef.bValid);
		m_cmbStg.AddString(strStg);
	}

	m_cmbStg.SetCurSel(0);
	OnCbnSelchangeCmbStg();

	m_edtChid2.SetWindowText(TEXT("1"));
	m_edtDur2.SetWindowText(TEXT("2500"));
	CString strXml;
	strXml.Format(TEXT("%s\\externalTaskTpl.xml"), g_GlobalInfo.szAppPath.c_str());
	m_edtTpXml.SetWindowText(strXml);
	m_edtCliptype2.SetWindowText(TEXT("1"));
	m_edtDbConn.SetWindowText(TEXT("Provider=SQLOLEDB; Server=.; Database=emb; uid=sa; pwd=123"));
	m_edtClipSrcId.SetWindowText(TEXT("1"));
}

void CDlgTest::OnCbnSelchangeCmbStg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_lstLuaKt.ResetContent();
	int nSel = m_cmbStg.GetCurSel();
	if (nSel >= 0 && nSel < g_GlobalInfo.stgConfig.vStgs.size()) 
	{
		ST_STGPARAM& stgRef = g_GlobalInfo.stgConfig.vStgs[nSel];
		MAPKEYTEMPLATE::iterator itb = stgRef.mapKt.begin();
		MAPKEYTEMPLATE::iterator ite = stgRef.mapKt.end();
		for (; itb != ite; ++itb)
		{
			CString strKt;
			strKt.Format(TEXT("<%d> <%s>"), itb->first, itb->second);
			m_lstLuaKt.AddString(strKt);
		}

	}
}

BOOL CDlgTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTest::OnBnClickedLuaTest()
{
	int nSel = m_cmbStg.GetCurSel();
	if (!(nSel >= 0 && nSel < g_GlobalInfo.stgConfig.vStgs.size()))
	{
		AfxMessageBox(TEXT("请选择策略"));
		return;
	}
		ST_STGPARAM& stgRef = g_GlobalInfo.stgConfig.vStgs[nSel];

	// TODO: 在此添加控件通知处理程序代码
	VECPARTOLUA vPar;
	//tmPlayTimeIn, strChidIn, nClipTypeIn, tClipDurIn
	CTime tmPlay;
	m_tmCtlPlaydate.GetTime(tmPlay);
	INT64 tPlayTime = tmPlay.GetTime();
	CString strChid;
	m_edtChid.GetWindowText(strChid);
	CString strType;
	m_edtClipType.GetWindowText(strType);
	CString strDuration;
	m_edtClipDur.GetWindowText(strDuration);

	vPar.push_back(ST_PARAMTOLUA(toluapartype_int64, luaparamkey_playtime, tPlayTime));
	vPar.push_back(ST_PARAMTOLUA(toluapartype_string, luaparamkey_chid, strChid));
	vPar.push_back(ST_PARAMTOLUA(toluapartype_int32, luaparamkey_cliptype, CTxStrConvert(strType).GetAsInt(-1)));
	vPar.push_back(ST_PARAMTOLUA(toluapartype_int64, luaparamkey_duration,  CTxStrConvert(strDuration).GetAsInt(-1)));
	int nRetVal = -1;
	CLuaStrategy stg;
	if (!stg.InitStrategyFromString(stgRef.strName, stgRef.strLua, this))
	{
		AfxMessageBox(TEXT("策略初始化失败"));
		return;
	}
	if (stg.RunStrategy(STGLUACALLFUNC, vPar, nRetVal)
		&& nRetVal >= 0)
	{
		CString strRet;
		strRet.Format(TEXT("策略返回值:%d"), nRetVal);
		AfxMessageBox(strRet);
	}
	else
	{
		AfxMessageBox(TEXT("策略执行失败"));
	}
}

BOOL CDlgTest::OnStrategyRunMessage( int nMsgType, CString& strValIn )
{
	CFWriteLog2Wnd(GetSafeHwnd(),  TEXT("lua callback msg type = %d, msg = %s"), nMsgType, strValIn);
	return TRUE;
}

LRESULT CDlgTest::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	MACRO_EDITLOGOUTPUT
	return __super::WindowProc(message, wParam, lParam);
}

void CDlgTest::OnBnClickedBtnBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strfile =GetAppPath().c_str();
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, NULL, this, 0 );
	if (dlg.DoModal() == IDOK)
	{
		strfile = dlg.GetOFN().lpstrFile;
		m_edtTpXml.SetWindowText(strfile);
	}
	else
	{
		return;
	}

}

void CDlgTest::OnBnClickedBtnCommit()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTplXml;
	m_edtTpXml.GetWindowText(strTplXml);
	if (_access(strTplXml, 04) == -1)
	{
		AfxMessageBox(TEXT("xml无法访问!"));
	}
	CTime tmPlay;
	m_tmCtlPlaytime2.GetTime(tmPlay);
	INT64 tPlayTime = tmPlay.GetTime();
	CString strChid;
	m_edtChid2.GetWindowText(strChid);
	CString strType;
	m_edtCliptype2.GetWindowText(strType);
	CString strDuration;
	m_edtDur2.GetWindowText(strDuration);
	CString strSrcType;
	m_edtClipSrcId.GetWindowText(strSrcType);
	CString strPri;
	m_edtPri.GetWindowText(strPri);

	CString strOutFile;
	m_edtOutfile.GetWindowText(strOutFile);


	if (strOutFile.IsEmpty() || strChid.IsEmpty() ||strType.IsEmpty()|| strDuration.IsEmpty())
	{
		AfxMessageBox(TEXT("参数不正确"));
		return;
	}

	CFile file;
	CString strFileAuto = strTplXml;
	CString strXml;

	if(file.Open(strFileAuto, CFile::modeRead, NULL))
	{
		int nLen = file.GetLength();
		char szbuff[9096];
		ZeroMemory(szbuff, 9096);
		file.Read(szbuff, 9096);
		file.Close();
		strXml = szbuff;
	}


	if (strXml.IsEmpty())
	{
		return;
	}

	CString strClipId = Guid2String(TxGenGuid());
	strXml.Replace("{PGMCODE}", strClipId);
	CString strPlaytime = tmPlay.Format(TEXT("%Y-%m-%d %H:%M:%S"));
	strXml.Replace("{PLAYTIME}", strPlaytime);
	strXml.Replace("{CLIPTYPE}", strType);
	strXml.Replace("{SRCTYPE}", strSrcType);
	strXml.Replace("{DURATION}", strDuration);
	strXml.Replace("{CHNID}", strChid);
	strXml.Replace("{PRI}", strPri);
	strXml.Replace("{SRCFILE}", strOutFile);

	CString strTmp;
	strTmp.Format(TEXT(" insert into T_IncomingTaskPool (PGMCode,PGMName, SourceID, SubmitDateTime, PlayDateTime, ChnID, nDuration, nClipType,nPRI, SourceStrXML, TaskID, Version) \
					   values ('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', null,'1')"),
					   strClipId, strClipId, strSrcType, CTime(time(NULL)).Format(TEXT("%Y-%m-%d %H:%M:%S")), strPlaytime, strChid, strDuration, strType, strPri, strXml);

	CTxADORecordSet rs(DBKEY_EMB);
	if(rs.ExecuteSQL(strTmp))
	{
		AfxMessageBox(TEXT("提交成功"));
	}
	else
	{
		AfxMessageBox(TEXT("提交失败"));
	}
	
}
