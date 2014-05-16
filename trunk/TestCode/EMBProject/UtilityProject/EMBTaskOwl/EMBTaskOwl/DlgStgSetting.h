#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgStgSetting 对话框

class CDlgStgSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgStgSetting)

public:
	CDlgStgSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStgSetting();

// 对话框数据
	enum { IDD = IDD_DLGSTGSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
