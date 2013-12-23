
// EMBActorHostDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MainDef.h"


// CEMBActorHostDlg �Ի���
class CEMBActorHostDlg : public CDialog
{
// ����
public:
	CEMBActorHostDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EMBACTORHOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void InitUI();
	/*
    Description����ѯִ���߽�����Ϣ
	Input��		m_arrExecutors �����е�Executor.exe
	Return:		
	History��
	*/
	void GetExecutors(CArray<tagExecutorProcess, tagExecutorProcess>& arrExecutors, CArray<ST_TASKRUNSTATE, ST_TASKRUNSTATE>& arrTask); 

	// ����
	CString m_strVideoPath;
	CString m_strXmlTemplate;
	CArray<CString, CString> m_arrVideo;
	int m_nIdx;
	// ------------

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//test
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CListCtrl m_lstExecutor;
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnClose();
	afx_msg void OnBnClickedLuanchexec();
	CListCtrl m_lstTask;
	afx_msg void OnBnClickedButtonAutotest();
};
