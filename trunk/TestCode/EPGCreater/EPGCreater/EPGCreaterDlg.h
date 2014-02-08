
// EPGCreaterDlg.h : ͷ�ļ�
//

#pragma once

#include "MainDef.h"
#include "afxwin.h"
#include "afxcmn.h"
// CEPGCreaterDlg �Ի���
class CEPGCreaterDlg : public CDialog
{
// ����
public:
	CEPGCreaterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EPGCREATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	BOOL SerializeEPGData(BOOL bReadIn = TRUE);

	BOOL AddTask(STEPGTASK& taskIn);
	BOOL DelTask(int nNumber);
	BOOL RefreshTask(int nNumber);

	void UpdateCtrl();

	BOOL GenEPGFile(CTime& tmStartIn, CTime& tmEndIn, int nTaskId, BOOL bAutoGenIn = FALSE, LPCTSTR szFileIn = NULL);
	void AutoCheckTask();
protected:
	STEPGTASKS m_epgData;
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
	afx_msg void OnClose();
	CButton m_btnTaskNew;
	CButton m_btnTaskEdit;
	CButton m_btnTaskDel;
	CButton m_btnTaskGenNow;
	CButton m_chkEnableTimeGen;
	CEdit m_edtLog;
	afx_msg void OnBnClickedBtnTasknew();
	CListCtrl m_lstTask;
	afx_msg void OnBnClickedBtnTaskedit();
	afx_msg void OnBnClickedBtnTaskdel();
	afx_msg void OnLvnItemchangedListTask(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnGenmanual();
	afx_msg void OnBnClickedCheckGenontime();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMCustomdrawListTask(NMHDR *pNMHDR, LRESULT *pResult);
};

