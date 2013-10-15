
// AotoFillDlg.h : 头文件
//

#pragma once
#include <vector>
#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorkbook.h"
#include "CWorksheets.h"
#include "CWorksheet.h"
#include "afxwin.h"

typedef std::vector<CString> VECFiles;

// CAotoFillDlg 对话框
class CAotoFillDlg : public CDialog
{
// 构造
public:
	CAotoFillDlg(CWnd* pParent = NULL);	// 标准构造函数


	void GetFilesInFolder(CString& strPath, VECFiles& vOut, BOOL bSearchInSubFolder = TRUE);

	BOOL ConvertExcellFiles(CString& strSrc, CString& strDes,  CApplication& appIn);
	BOOL MergeExcelFiles(CWorkbook&bookSrc, CWorkbook& bookDes);
// 对话框数据
	enum { IDD = IDD_AOTOFILL_DIALOG };

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
public:
	afx_msg void OnBnClickedBtnStart();

	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CListBox m_log;
	CButton m_chkSubDir;
	afx_msg void OnBnClickedBtnMerge();
};
