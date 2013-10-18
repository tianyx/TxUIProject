
// ConfigUtilityDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ConfigMgr.h"
#include "resource.h"
#include "CFDropTarget.h"

struct ST_SHOWPARAM
{
	BOOL bShowDiffOnly;
	BOOL bHideLeftLoss;
	BOOL bHideRightLoss;
	ST_SHOWPARAM()
	{
		bShowDiffOnly = FALSE;
		bHideLeftLoss = FALSE;
		bHideRightLoss = FALSE;
	}

};

// CConfigUtilityDlg 对话框


class CConfigUtilityDlg : public CDialog
{
// 构造
public:
	CConfigUtilityDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONFIGUTILITY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void Reset();
	void ChangeSrc();
	void RefreshList();

	void Relayout();
private:
	void InsertKey(MAPCFGKEY& mapLeftIn, MAPCFGKEY& mapRightIn, CString strPreIn, ST_SHOWPARAM& paramIn);
	void InsertKey(ST_CFGVALUE& valLeftIn, ST_CFGVALUE& valRightIn, CString strPreIn, ST_SHOWPARAM& paramIn);

private:
	CConfigMgr m_cfgMgr;
public:
	CCFDropTarget m_drapTarget;
	CButton m_rdoModeLocal;
	CButton m_btnStartCompare;
	afx_msg void OnBnClickedButtonStartcompare();
	CEdit m_edtSrcPath;
	CEdit m_edtDesPath;
	CComboBox m_cmbClassRoot;
	CComboBox m_cmbClassSub;
	CComboBox m_cmbClassSubSub;
	CListBox m_lstLog;
	CListCtrl m_lstResult;
	CButton m_btnSrcBrowse;
	CButton m_btnDesBrowse;
	CButton m_chkHideSrcLoss;
	CButton m_chkHideDesLoss;
	CButton m_chkShowDiffOnly;
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboClassRoot();
	afx_msg void OnCbnSelchangeComboClassSub();
	afx_msg void OnBnClickedRadioModeInternal();
	afx_msg void OnBnClickedRadioModeOpenfile();
	afx_msg void OnBnClickedButtonSrcBrowse();
	afx_msg void OnBnClickedButtonDesBrowse();
	afx_msg void OnBnClickedCheckShowdiffonly();
	afx_msg void OnBnClickedCheckHideSrcLoss();
	afx_msg void OnBnClickedCheckHideDesLoss();
	afx_msg void OnNMCustomdrawListResult(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	CStatic m_stcLog;
};
