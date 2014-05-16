#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "AutoTaskFunc.h"
#include "mxttimecodectrl.h"
#include "afxdtctl.h"

#define MSG_NEWEMBTASKINFOUPDATE WM_USER+1600

// CDlgNewEMBTask 对话框

class CDlgNewEMBTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewEMBTask)

public:
	CDlgNewEMBTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewEMBTask();

public:
	ST_PSCLIPINFO* m_pClipInfo;
	HWND m_hwndNotify;
	VECLSTHEADERS m_vListCols;
	int nDefChnSel;

	void InitFtpListHeader();
	void InitCtrl();
	void InitInfo();

// 对话框数据
	enum { IDD = IDD_DLG_NEWTASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtPgmCode;
	CEdit m_edtPgmName;
	CComboBox m_cmbChnId;
	CEdit m_edtClipId;
	CEdit m_edtFilename;
	CListCtrl m_lstSrcFtp;
	CComboBox m_cmbTpl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnChangeclipid();
	virtual BOOL OnInitDialog();
	CMxtTimecodeCtrl* m_ptcCtrlLength;
	CMxtTimecodeCtrl* m_ptcCtrlSom;
	CMxtTimecodeCtrl* m_ptcCtrlEom;
	CDateTimeCtrl m_dtCtrlPlaytime;
};
