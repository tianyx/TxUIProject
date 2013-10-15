#pragma once
#include "afxwin.h"

// CDlgTopPanel 对话框

class CDlgTopPanel : public CDialog
{
	DECLARE_DYNAMIC(CDlgTopPanel)

public:
	CDlgTopPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTopPanel();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOPPANEL };
	
	void Relayout();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_btnCtlRun;
	BOOL m_bRuning;
	afx_msg void OnBnClickedBtnTest();
	CButton m_btnTest;
	virtual BOOL OnInitDialog();
};
