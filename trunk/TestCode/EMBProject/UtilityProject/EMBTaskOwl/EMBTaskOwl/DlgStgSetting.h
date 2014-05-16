#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgStgSetting �Ի���

class CDlgStgSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgStgSetting)

public:
	CDlgStgSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgStgSetting();

// �Ի�������
	enum { IDD = IDD_DLGSTGSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void UpdateCtrl();
	void LoadFromFile();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstStg;
	CButton m_btnNew;
	CButton m_btnCopy;
	CButton m_btnEdit;
	CButton m_btnMoveUp;
	CButton m_btnMoveDown;
	CButton m_btnEnable;
	CEdit m_edtContent;
	CEdit m_edtStgContent;
	virtual BOOL OnInitDialog();
	afx_msg void OnHdnItemchangedListStg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListStg(NMHDR *pNMHDR, LRESULT *pResult);
};
