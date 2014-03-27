/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBServerDlg.h
	author:		zhouliang
	
	purpose:	�������Ի���
*********************************************************************/

#pragma once
#include "afxcmn.h"
#include "GdiPlusNewHeader.h"
#include "TxImgButton.h"
#include "afxwin.h"
#include "DlgDbgTest.h"


// CEMBServerDlg �Ի���
class CEMBServerDlg : public CDialog,
	public IParentBackDrawInterface
{
// ����
public:
	CEMBServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EMBSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	virtual void GetParentBack(CDC* pDc);
public:

	BOOL m_bRunning;    //�Ƿ���������
// ʵ��
protected:
	HICON m_hIcon;
	CDlgDbgTest m_dlgtest;

private:
	Gdiplus::Bitmap* m_pbmpBack;
	Gdiplus::Bitmap* m_pbmpLogo2;
	Gdiplus::Bitmap* m_pbmpSublogo;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();

	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CTxImgButton m_btnStart;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
