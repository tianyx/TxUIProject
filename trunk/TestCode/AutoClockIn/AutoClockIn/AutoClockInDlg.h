
// AutoClockInDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "HttpNetOp.h"
#include <vector>
#include "afxdtctl.h"
#include "ACConfig.h"



// CAutoClockInDlg 对话框
class CAutoClockInDlg : public CDialogEx
{
// 构造
public:
	CAutoClockInDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUTOCLOCKIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//config data
	
	CACConfig* m_pCfg;
	//auto start btn state
	BOOL m_bAutoMode;

	int m_nRandomMinute;
	CFont m_btnFont;
	// 实现

private:
	void UpdateCtrlState();
	void LoadConfig();
	BOOL SaveConfig();
	BOOL TryClock1();
	BOOL ShowBubbleTip(CString& strTitle, CString& strTipIn, DWORD dwInfoFlag = NIIF_INFO);
	BOOL CheckClockInResult(int nClockNum, CString& strFileIn);

	BOOL TryClock(int nClockNum,  BOOL bNow = FALSE);
	BOOL ShutDown();

	COLORREF GetStcColor(int nClockNum);
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lstLog;
	afx_msg void OnBnClickedMfcbtnTest();
	afx_msg void OnBnClickedBtnAutostart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_stcInfo[4];
	CDateTimeCtrl m_ctlTimeClockIn[4];
	CEdit m_edtSvr;
	CButton m_chkModify;
	CButton m_chkShowLog;


protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CEdit m_edtUser;
	CEdit m_edtPw;
	CStatic m_edtLastTip;
	CButton m_chkAutorun;
	CButton m_chkAutoStart;
	CButton m_chkAutoClock1;
	CButton m_chkPopBubble;
	CButton m_chkShutdown;
	CEdit m_edtPreMinute;
protected:
	virtual void PostNcDestroy();
public:
	CButton m_btnAutoStart;
	afx_msg void OnBnClickedCheckModify();
	afx_msg void OnBnClickedBtnExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckShowlog();
	CButton m_btnTry[4];
	afx_msg void OnBnClickedBtnTry1();
	afx_msg void OnBnClickedBtnTry2();
	afx_msg void OnBnClickedBtnTry3();
	afx_msg void OnBnClickedBtnTry4();
	CButton m_chkSleep;
	afx_msg void OnBnClickedCheckAutoshutdown();
};

