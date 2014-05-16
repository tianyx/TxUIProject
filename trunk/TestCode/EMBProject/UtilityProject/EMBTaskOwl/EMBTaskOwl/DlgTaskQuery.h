#pragma once
#include "afxcmn.h"
#include "FGlobal.h"
#include "StrConvert.h"
#include "TxADOConn.h"
// CDlgTaskQuery 对话框
#include <vector>
using namespace std;
#define sqldatetype_charmin 37
#define sqldatetype_charmax 167
#define sqldatetype_int		56
#define sqldatetype_time   61
#define sqldatetype_int64	127

#define lstid_inpool 0
#define lstid_embtask 1
#define lstid_emblog	2

struct ST_SQLCOLINFO 
{
	CString strName;
	int nDataType;
	ST_SQLCOLINFO()
	{
		nDataType = sqldatetype_charmin;
	}
};

typedef vector<ST_SQLCOLINFO> VECSQLCOLINFO;

struct ST_QUERYCOLINFO
{
	int nPgmcodePos;
	int nEmbTaskIdPos;
	VECSQLCOLINFO vCols;
	ST_QUERYCOLINFO()
	{
		nPgmcodePos = -1;
		nEmbTaskIdPos = -1;
	}
};

struct ST_EMBLOGQUERYDATA
{
	CString strEmbTaskID;
	VECSTRINGS vData;

};

typedef vector<ST_EMBLOGQUERYDATA> VECEMBLOGDATA;

struct ST_EMBTASKQUERYDATA
{
	BOOL bSubQueryed;
	VECSTRINGS vData;
	CString strEmbTaskID;
	VECEMBLOGDATA vLogData;
	void Reset()
	{
		bSubQueryed = FALSE;
		vData.clear();
	}
	ST_EMBTASKQUERYDATA()
	{
		bSubQueryed = FALSE;
	}
};



typedef vector<ST_EMBTASKQUERYDATA> VECEMBTASKCQUERYDATA;

struct ST_INPOOLTASKQUERYDATA
{
	BOOL bDataHolder; //data is false, only for holding position for embtaskid, 
	BOOL bSubQueryed;
	CString strPgmCode;
	VECSTRINGS vData;
	VECEMBTASKCQUERYDATA vEmbData;

	ST_INPOOLTASKQUERYDATA()
	{
		bDataHolder = FALSE;
		bSubQueryed = FALSE;
	}
};

typedef vector<ST_INPOOLTASKQUERYDATA> VECINPOOLQUERYTASK;
class CDlgTaskQuery : public CDialog
{
	DECLARE_DYNAMIC(CDlgTaskQuery)

public:
	CDlgTaskQuery(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTaskQuery();

// 对话框数据
	enum { IDD = IDD_DLG_TASKQUERY };
public:
	void InitQueryData(LPCTSTR szPgmCode, LPCTSTR szEmbTaskId);
	void DoSearch();
	void RefreshList(int nChangedListId); //0,1,2 to lstid_inpool
	void Reset();
	void QueryEmbDBData(ST_INPOOLTASKQUERYDATA& dataInOut);
	void QueryEmbLogDBData(ST_EMBTASKQUERYDATA& dataInOut);
private:
	int m_nInpoolLstSel;
	int m_nEmbTaskLstSel;
	int m_nEmbLogLstSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strQueryEmbTaskID;
	CString m_strQueryPgmCode;
	afx_msg void OnBnClickedBtnSearch();
	CListCtrl m_lstInpoolTask;
	CListCtrl m_lstEmbTask;
	CListCtrl m_lstEmbTaskLog;

	void InitListHeaders();

	static void LoadListHeaders();

	static ST_QUERYCOLINFO m_vInpoolHeaders;
	static ST_QUERYCOLINFO m_vEmbHeaders;
	static ST_QUERYCOLINFO m_vEmbLogHeaders;
	static BOOL m_bstHeaderInited;

	VECINPOOLQUERYTASK m_vInpoolTask;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};

CString GetDataByColInfo(CTxADORecordSet& rs, ST_SQLCOLINFO& colInfo);