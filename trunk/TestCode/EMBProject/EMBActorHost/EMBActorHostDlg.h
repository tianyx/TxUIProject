
// EMBActorHostDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "GdiPlusNewHeader.h"
#include "TxImgButton.h"

#include "DlgDbgTest.h"

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
	CDlgDbgTest m_dlgtest;

// ʵ��
protected:
	HICON m_hIcon;

	Gdiplus::Bitmap* m_pbmpBack;
	Gdiplus::Bitmap* m_pbmpLogo2;
	Gdiplus::Bitmap* m_pbmpSublogo;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//test
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	
	afx_msg void OnClose();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
