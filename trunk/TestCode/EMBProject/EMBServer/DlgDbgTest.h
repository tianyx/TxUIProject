#pragma once


// CDlgDbgTest �Ի���

class CDlgDbgTest : public CDialog
{
	DECLARE_DYNAMIC(CDlgDbgTest)

public:
	CDlgDbgTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDbgTest();

// �Ի�������
	enum { IDD = IDD_DLG_TEST };
	void InitUI();
	void RefreshActorList();
	

protected:
	afx_msg void OnBnClickedBtntest();
	afx_msg void OnBnClickedButtonXml();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CListCtrl m_actorList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
