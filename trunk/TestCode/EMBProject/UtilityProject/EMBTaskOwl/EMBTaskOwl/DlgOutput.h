#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgOutput �Ի���

class CDlgOutput : public CDialog
{
	DECLARE_DYNAMIC(CDlgOutput)

public:
	CDlgOutput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgOutput();

// �Ի�������
	enum { IDD = IDD_DLG_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CRichEditCtrl m_edtLog;
};
