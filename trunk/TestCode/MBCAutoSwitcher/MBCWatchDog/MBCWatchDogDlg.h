
// MBCWatchDogDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MBCwatchStateQueryObj.h"

enum ENUM_SWITCHSTATE
{
	SWITCHSTATE_UNKNOW,
	SWITCHSTATE_OK,
	SWITCHSTATE_DOWN
};

// CMBCWatchDogDlg 对话框
class CMBCWatchDogDlg : public CDialog, public IWatchStateIncomingCallback
{
// 构造
public:
	CMBCWatchDogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MBCWATCHDOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	CMBCWatchStateQueryObj m_ObjBack;
	CMBCWatchStateQueryObj m_ObjMaster;
	
	int m_nMasterStateCounting;
	int m_nBackStateCounting;
	ENUM_SWITCHSTATE m_bMasterDown;
	ENUM_SWITCHSTATE m_bBackDown;
	int m_nBackActCD;
	virtual HRESULT OnRecvGraphState(CMBCWatchStateQueryObj* pObj, ST_GRAPHSTATE& gstateIn);
	void CheckState();
	void RefreshCtrl();
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
	CButton m_btnStart;
	afx_msg void OnBnClickedBtnStart();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_edtMasterAddr;
	CEdit m_edtBackPath;
	CEdit m_edtBackAddr;
	afx_msg void OnClose();
	CStatic m_stcMasterState;
	CStatic m_stcBackState;
	afx_msg void OnBnClickedBtnQuit();
};
