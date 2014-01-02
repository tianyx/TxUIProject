/********************************************************************
	created:	2013/11/23
	filename: 	EMBClientDlg.h
	author:		tianyx
	purpose:	对话框
    History: 
*********************************************************************/

#pragma once
#include "UIClientConnector.h"
#include "EmbStructDef.h"
#include "afxwin.h"

// CEMBClientDlg 对话框
class CEMBClientDlg : public CDialog,
	public IUIClientMessageProcessInterface
{
// 构造
public:
	CEMBClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBCLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	//for IUIClientMessageProcessInterface
	virtual HRESULT OnMessage(int nMaster, CString& strMsgIn, CString& strRet);
	virtual HRESULT OnConnStateChange(int nMaster, int nStateIn);
	virtual HRESULT SendToUISvr(CString& strMsg, BOOL bSendAll = FALSE);

private:
	CUIClientConnector* GetActiveConn();
private:
	CUIClientConnector m_connMaster;
	CUIClientConnector m_connSlave;

	CAutoCritSec m_csLive;
	ST_SVRLIVEINFO m_svrInfoMaster;
	ST_SVRLIVEINFO m_svrInfoSlave;

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_edtLog;
	afx_msg void OnBnClickedBtnGetsvrinfo();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnGetactorlist();
	afx_msg void OnBnClickedBtnGettasklist();
	afx_msg void OnBnClickedBtnGetactorinfo();
	afx_msg void OnBnClickedBtnGettaskinfo();
	CEdit m_edtActorId;
	CEdit m_edtTaskId;
	afx_msg void OnDestroy();
};

