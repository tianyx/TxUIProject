
// BatchCheckClipInfoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
#include "afxdtctl.h"
using namespace  std;

typedef vector<CString> VECFILES;
// CBatchCheckClipInfoDlg �Ի���

class CBatchCheckClipInfoDlg : public CDialog
{
// ����
public:
	CBatchCheckClipInfoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BATCHCHECKCLIPINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
	public:
	HICON m_hIcon;
	HANDLE m_hThreadCheckInfo;
	BOOL m_bRuning;
	VECFILES m_vecFiles;
	CString m_strCheckFolderPath;
	
	UINT m_nCheckFlag;
	
	INT m_nExcludeWidth;
	INT m_nExcludeHeight;

	BOOL m_bUseTimeRange;
	CTime m_tmMin;
	CTime m_tmMax;

	void RefreshCtrl();
	int InitCheckFileList();
	BOOL InitParam();
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnDestroy();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CEdit m_edtLog;
public:
	CEdit m_edtFolder;
	CListCtrl m_lstResult;
	CEdit m_edtResWidth;
	CEdit m_edtResHeight;
	afx_msg void OnBnClickedBtnbrowse();
	CButton m_btnStart;
	CStatic m_stcProcess;
	CButton m_chkTimeRange;
	CDateTimeCtrl m_tmCtlMin;
	CDateTimeCtrl m_tmCtlMax;

	afx_msg void OnBnClickedCheckTimerange();
	CComboBox m_cmbRatio;
	afx_msg void OnBnClickedBtnSavetofile();
};
