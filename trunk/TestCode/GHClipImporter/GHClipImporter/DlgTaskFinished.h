#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgTaskFinished 对话框

class CDlgTaskFinished : public CDialog
{
	DECLARE_DYNAMIC(CDlgTaskFinished)

public:
	CDlgTaskFinished(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTaskFinished();

// 对话框数据
	enum { IDD = IDD_DIALOG_TASKFINISHED };
void Refresh();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstTaskFinished;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CButton m_btnRefresh;
	afx_msg void OnBnClickedBtnRefresh();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
