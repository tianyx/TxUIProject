#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include <vector>
#include "afxcmn.h"
#include "GlobalDef.h"
#include "AutoTaskFunc.h"

using namespace std;

#define refreshclipflag_notreadyonly	1
#define refreshclipflag_db1				2
#define refreshclipflag_db2				4
#define refreshclipflag_emb				8
#define refreshclipflag_intaskpool		0x10	

#define clipfinderflag_all				0
#define clipfinderflag_notindb1			1
#define clipfinderflag_notindb2			2
class CDlgPlaylistTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgPlaylistTask)

public:
	CDlgPlaylistTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPlaylistTask();
	VECPLAYSHEETS m_vSelPSIs; //playlist ids

	VECLSTHEADERS m_vListCols;

	CTime m_tmSel;
	int m_nChnSel;

	VECPSCLIPS m_vPSClips;  //clip in playlist
public:
	void RefreshSectionList();
	void RefreshPsInfo();

	void InitListHeader();

	BOOL DoSearch();
	void RefreshPsClipState(int nRefreshFlag);
	void RefreshListItem( int nPos );
	int GetItemPosByPgmcode(LPCTSTR strPgmCode);
	//use clipfinderflag
	CString GetDesiredClips(int nfindFlag, VECPSCLIPPTRS* pvecOut = NULL);
	CString GetPSClipColText(const int ColTypeIn, ST_PSCLIPINFO& dataIn);
	
	void RefreshList(BOOL bUpdateStateOnly = FALSE);

	int GetSelInPoolIdleTask(VECSTRINGS& vPgmOut);

	void GetClipInPoolTaskInfo(VECSTRINGS& vPgmIn, VECINPOOLTASK& vTaskOut);
	
	void Relayout();

	BOOL GetItemTextClrByVal(const int nIDIn, UINT& clrtext, UINT& clrtextBk);

// 对话框数据
	enum { IDD = IDD_DLGPLAYLISTTASKSEARCH };

	int m_nColPosTaskid;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbChn;
	CDateTimeCtrl m_tmCtrlDate;
	CComboBox m_cmbSection;
	afx_msg void OnBnClickedBtnSearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboChn();
	afx_msg void OnDtnDatetimechangeDatetimePlaylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboSection();
	CStatic m_stcPsInfo;
	CListCtrl m_lstTask;
	afx_msg void OnNMRClickListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_grpSearch;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult);
};
