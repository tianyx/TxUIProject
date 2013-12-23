/********************************************************************
	created:	2013/11/23
	filename: 	EMBClientDlg.h
	author:		tianyx
	purpose:	�Ի���
    History: 
*********************************************************************/

#pragma once
#include "UIClientConnector.h"
#include "EmbStructDef.h"

// CEMBClientDlg �Ի���
class CEMBClientDlg : public CDialog,
	public IUIClientMessageProcessInterface
{
// ����
public:
	CEMBClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EMBCLIENT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//for IUIClientMessageProcessInterface
	virtual HRESULT OnMessage(int nMaster, CString& strMsgIn, CString& strRet);
	virtual HRESULT OnConnStateChange(int nMaster, int nStateIn);

private:
	CUIClientConnector m_connMaster;
	CUIClientConnector m_connSlave;

	CAutoCritSec m_csLive;
	ST_SVRLIVEINFO m_svrInfoMaster;
	ST_SVRLIVEINFO m_svrInfoSlave;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
