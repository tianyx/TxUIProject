
// ZQLicenseMgrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CZQLicenseMgrDlg �Ի���
class CZQLicenseMgrDlg : public CDialog
{
// ����
public:
	CZQLicenseMgrDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ZQLICENSEMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	BOOL LoadConfig();

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
	CEdit m_edtPCCode;
	CComboBox m_cmbModule;
	CEdit m_stcModule;
	CButton m_chkExpireDate;
	CDateTimeCtrl m_tmCtlExpireDate;
	CEdit m_edtCodeResult;
	afx_msg void OnBnClickedBtnGen();
	afx_msg void OnCbnSelchangeComboModule();
	afx_msg void OnBnClickedBtnReset();
	CButton m_chkGenToFile;
	afx_msg void OnBnClickedChkExpireDate();
	CButton m_chkToFile2;
	afx_msg void OnBnClickedRegfromfile();
};
