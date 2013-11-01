
// EMBExternalExcutorDlg.h : ͷ�ļ�
//

#pragma once
#include "ExcutorObj.h"

// CEMBExternalExcutorDlg �Ի���
class CEMBExternalExcutorDlg : public CDialog
{
// ����
public:
	CEMBExternalExcutorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EMBEXTERNALEXCUTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CExcutorObj* m_pExcObj;

public:
	afx_msg void OnDestroy();
};
