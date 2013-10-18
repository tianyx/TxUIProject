#pragma once
#include "afxwin.h"


// CDlgShutdown 对话框

class CDlgShutdown : public CDialog
{
	DECLARE_DYNAMIC(CDlgShutdown)

private:
	CDlgShutdown(CWnd* pParent = NULL);   // 标准构造函数
public:
	virtual ~CDlgShutdown();
	static CDlgShutdown*  CreateShutdownDlg();
	static HWND m_hwndNotifier;
private:
	static CDlgShutdown* m_spDlg;
public:
	int m_nCountDown;
	BOOL bSleep;
	CFont m_font;
// 对话框数据
	enum { IDD = IDD_DIALOG_SHUTDOWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedCancel();
	CStatic m_stcInfo;
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedDonow();
};
