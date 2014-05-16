#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "LuaStrategy.h"

// CDlgTest 对话框

class CDlgTest : public CDialog, public IStrategyRuntimeCallback
{
	DECLARE_DYNAMIC(CDlgTest)

public:
	CDlgTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTest();

	virtual BOOL OnStrategyRunMessage(int nMsgType, CString& strValIn);

	void InitCtrl();
// 对话框数据
	enum { IDD = IDD_DIALOG_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnGetconfig();
	CEdit m_edtLog;
	CComboBox m_cmbStg;
	CListBox m_lstLuaKt;
	CDateTimeCtrl m_tmCtlPlaydate;
	CEdit m_edtChid;
	CEdit m_edtClipType;
	CEdit m_edtClipDur;
	afx_msg void OnCbnSelchangeCmbStg();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLuaTest();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CDateTimeCtrl m_tmCtlPlaytime2;
	CEdit m_edtChid2;
	CEdit m_edtCliptype2;
	CEdit m_edtDur2;
	CEdit m_edtClipSrcId;
	CEdit m_edtTpXml;
	CEdit m_edtDbConn;
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnCommit();
	CEdit m_edtPri;
	CEdit m_edtOutfile;
};
