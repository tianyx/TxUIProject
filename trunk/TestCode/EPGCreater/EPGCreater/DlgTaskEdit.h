#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "MainDef.h"
#include "afxcmn.h"

// CDlgTaskEdit 对话框

class CDlgTaskEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgTaskEdit)

public:
	CDlgTaskEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTaskEdit();

// 对话框数据
	enum { IDD = IDD_DLG_TASKEDIT };

public:
	BOOL Init(STEPGTASK* pTaskIn = NULL);
	STEPGTASK& GetTaskRef(){return m_task;}
protected:
	BOOL UpdateTaskData(BOOL bCtrlToData = TRUE);
	BOOL m_nEditMode;
	STEPGTASK m_task;

	VECCHNTASK m_vChnCtrlData;
	CString strDlgTitle;
	int nLastChnSel;

private:
	void UpdateWeedayCtrl();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cmbChn;
	CButton m_chkWeekDay1;
	CButton m_chkWeekDay2;
	CButton m_chkWeekDay3;
	CButton m_chkWeekDay4;
	CButton m_chkWeekDay5;
	CButton m_chkWeekDay6;
	CButton m_chkWeekDay7;
	CComboBox m_cmbSection1;
	CComboBox m_cmbSection2;
	CComboBox m_cmbSection3;
	CComboBox m_cmbSection4;
	CComboBox m_cmbSection5;
	CComboBox m_cmbSection6;
	CComboBox m_cmbSection7;

	CComboBox* m_vSectionBox[7];
	CButton* m_vchkWeekDay[7];

	CButton m_chkTimeGen;
	CEdit m_edtOffsetDay;
	CDateTimeCtrl m_dtStartTime;
	CButton m_chkMustSubmitted;
	afx_msg void OnCbnSelchangeComboChn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTaskgenontime();
	afx_msg void OnBnClickedCheckWeekday();
	CEdit m_edtDesc;
	CTreeCtrl m_TreeChn;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboSection();
	CButton m_chkWeekLoopMode;
	afx_msg void OnNMCustomdrawTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
