#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "AutoTaskFunc.h"

// CDlgManualTask 对话框

class CDlgManualTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgManualTask)

public:
	CDlgManualTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgManualTask();

// 对话框数据
	enum { IDD = IDD_DLGMANUALTASKSEARCH };

public:
	void RefreshCtrls();
	BOOL DoSearch();
	void InitListHeader();
	BOOL RefreshTaskList();
	void RefreshListItem(int nPosIn);
	BOOL CollectSearchOpt(CString& strRetOut);

	VECLSTHEADERS m_vListCols;
	VECINPOOLTASK m_vSearchData;
	int m_nColPosTaskid;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void Relayout();

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_chkPgmCode;
	CEdit m_edtPgmCode;
	CButton m_chkClipName;
	CEdit m_edtClipName;
	CButton m_chkTimeRemain;
	CButton m_chkPgmListRange;
	CButton m_chkChid;
	CButton m_chkEmergency;
	CButton m_chkEmbTaskState;
	CEdit m_edtPlayTimeMin;
	CEdit m_edtPlayTimeMax;
	CEdit m_edtPlayListMax;
	CEdit m_edtPlayListMin;
	CEdit m_edtChId;
	CEdit m_edtEmergency;
	CComboBox m_cmbEmbTaskState;
	CButton m_chkShowAdv;
	CStatic m_grpSearch;
	CListCtrl m_lstTask;
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedCheckShowadvsearch();

	CButton m_btnSearch;
	afx_msg void OnBnClickedCheckOpt();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTask(NMHDR *pNMHDR, LRESULT *pResult);
};
