
// TS2ASISvrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Ts2ASIBuilder.h"
#include "afxcmn.h"

// CTS2ASISvrDlg �Ի���
class CTS2ASISvrDlg : public CDialog
{
// ����
public:
	CTS2ASISvrDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TS2ASISVR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	int m_nDevType;
	int m_nDevPort;
	int m_nTSRatebps;
	
	CTs2ASIBuilder m_builder;

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
	CEdit m_edtDevType;
	CEdit m_edtASIoutRateBps;
	CEdit m_edtOutPort;
	CEdit m_edtLog;
	CButton m_btnStart;
	afx_msg void OnBnClickedBtnstart();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CButton m_btnStop;
	afx_msg void OnBnClickedBtnstop();
	CButton m_chkASIAutoAdjustoutRate;
	CStatic m_stcASIInRate;
	CStatic m_stcASIOutRate;
	CListCtrl m_lstSrcAddr;
	CButton m_chkSrcMonOutRate;
	afx_msg void OnBnClickedBtnOutbpsapply();
	CButton m_btnApplyASIOutbps;
	afx_msg void OnBnClickedChkAutoadjustoutpbs();
	afx_msg void OnBnClickedChkMonsrcoutbps();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
