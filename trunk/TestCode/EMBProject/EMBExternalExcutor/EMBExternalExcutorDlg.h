
// EMBExternalExcutorDlg.h : 头文件
//

#pragma once
#include "ExcutorObj.h"

// CEMBExternalExcutorDlg 对话框
class CEMBExternalExcutorDlg : public CDialog
{
// 构造
public:
	CEMBExternalExcutorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EMBEXTERNALEXCUTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CExcutorObj* m_pExcObj;

public:
	afx_msg void OnDestroy();
};
