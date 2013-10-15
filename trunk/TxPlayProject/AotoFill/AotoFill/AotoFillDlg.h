
// AotoFillDlg.h : ͷ�ļ�
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

// CAotoFillDlg �Ի���
class CAotoFillDlg : public CDialog
{
// ����
public:
	CAotoFillDlg(CWnd* pParent = NULL);	// ��׼���캯��


	void GetFilesInFolder(CString& strPath, VECFiles& vOut, BOOL bSearchInSubFolder = TRUE);

	BOOL ConvertExcellFiles(CString& strSrc, CString& strDes,  CApplication& appIn);
	BOOL MergeExcelFiles(CWorkbook&bookSrc, CWorkbook& bookDes);
// �Ի�������
	enum { IDD = IDD_AOTOFILL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
