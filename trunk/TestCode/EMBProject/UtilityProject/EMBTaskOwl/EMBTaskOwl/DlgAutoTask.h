#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "globaldef.h"
#include "AutoCritSec.h"
#include "AutoTaskFunc.h"

// CDlgAutoTask 对话框
#define MSG_EMBTASKCOMMITINAUTO WM_USER + 1601


class CDlgAutoTask : public CDialog
{
	DECLARE_DYNAMIC(CDlgAutoTask)

public:
	CDlgAutoTask(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAutoTask();

// 对话框数据
	enum { IDD = IDD_DLGAUTOTASK };
public:
	void UpdateAutoMsg(LPCTSTR szMsg);
	void Relayout();

	BOOL IsRunning(){return m_hThreadTask != NULL;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	HANDLE m_hThreadTask;
	HANDLE m_hQuitEvent;
	VECLSTHEADERS m_vListCols;

	CAutoTaskFunc m_autoFun;

	int m_nColPosTaskid;
public:
	void RunTaskLoop();
	void UpdateCtrl();
	void InitListHeader();
	void LogAutoTaskItem(ST_INPOOLTASKDATA& orgDataIn);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btnRun;
	CStatic m_stcAutoMsg;
	CEdit m_edtAutoLog;
	CListCtrl m_lstTask;
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult);
};
