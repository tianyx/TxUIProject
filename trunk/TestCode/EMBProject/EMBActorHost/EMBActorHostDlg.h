
// EMBActorHostDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "GdiPlusNewHeader.h"
#include "TxImgButton.h"

#include "DlgDbgTest.h"

// CEMBActorHostDlg 对话框
class CEMBActorHostDlg : public CDialog
{
// 构造
public:
	CEMBActorHostDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBACTORHOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CDlgDbgTest m_dlgtest;

// 实现
protected:
	HICON m_hIcon;

	Gdiplus::Bitmap* m_pbmpBack;
	Gdiplus::Bitmap* m_pbmpLogo2;
	Gdiplus::Bitmap* m_pbmpSublogo;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//test
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	
	afx_msg void OnClose();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
