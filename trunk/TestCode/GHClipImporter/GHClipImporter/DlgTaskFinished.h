#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgTaskFinished �Ի���

class CDlgTaskFinished : public CDialog
{
	DECLARE_DYNAMIC(CDlgTaskFinished)

public:
	CDlgTaskFinished(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTaskFinished();

// �Ի�������
	enum { IDD = IDD_DIALOG_TASKFINISHED };
void Refresh();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
