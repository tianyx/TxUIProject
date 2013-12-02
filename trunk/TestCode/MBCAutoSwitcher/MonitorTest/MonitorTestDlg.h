
// MonitorTestDlg.h : ͷ�ļ�
//

#pragma once

#include "MBCCommonDef.h"
#include "MBCRemoteInterface.h"
#include "TxTimer.h"

// CMonitorTestDlg �Ի���
class CMonitorTestDlg : public CDialog, IMBCMSGRemoteCallInterface,
	public ITxTimerCallbackInterface
{
// ����
public:
	CMonitorTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MONITORTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	IMBCRemoteMgr* m_pMgr;

	virtual HRESULT GetLiveInfo(ST_MBCMSG_LIVEQA& msg);
	virtual HRESULT DoAction(ST_MBCMSG_ACTQA& msg);
	virtual HRESULT TransActData(int nmsgType,char* bufferIn, int nLenIn, char* bufferToFill, int nMaxLen, int& nFillLen);
	virtual HRESULT GetLiveInfo(ST_MBCMSGBASE& basemsg, int& nRemoteState, int& nSelftype,  ST_MBCCHANNELINFO_FORVC6* pArrayOut, const int nArrLen, int& chUsed);

	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

// ʵ��
protected:
	HICON m_hIcon;
	CTxTimer timerTest;


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedButton2();
};
