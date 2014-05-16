// dlgTopBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "dlgTopBar.h"
#include "AutoTaskFunc.h"
#include "MainFrm.h"
#include "DlgNewEMBTask.h"
#include "DlgTest.h"

// CdlgTopBar �Ի���

IMPLEMENT_DYNAMIC(CDlgTopBar, CDialog)

CDlgTopBar::CDlgTopBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTopBar::IDD, pParent)
{

}

CDlgTopBar::~CDlgTopBar()
{
}

void CDlgTopBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RDO_AUTO, m_rdoTab);
	DDX_Control(pDX, IDC_BTNTEST, m_btnTest);
}


BEGIN_MESSAGE_MAP(CDlgTopBar, CDialog)
	ON_BN_CLICKED(IDC_BTNTEST, &CDlgTopBar::OnBnClickedBtntest)
	ON_BN_CLICKED(IDC_BTN_NEWTASK, &CDlgTopBar::OnBnClickedBtnNewtask)
	ON_BN_CLICKED(IDC_RDO_AUTO, &CDlgTopBar::OnBnClickedRdoAuto)
	ON_BN_CLICKED(IDC_RDO_MANUAL, &CDlgTopBar::OnBnClickedRdoManual)
	ON_BN_CLICKED(IDC_RDO_PLAYLIST, &CDlgTopBar::OnBnClickedRdoPlaylist)

END_MESSAGE_MAP()


// CdlgTopBar ��Ϣ�������

void CDlgTopBar::OnBnClickedBtntest()
{
	CDlgTest dlg;
	dlg.DoModal();
	return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//for test
	CString strFcvsfile;
	strFcvsfile = GetAppPath().c_str();
	strFcvsfile += TEXT("\\srctask.xml");
	CFile file;
	BOOL bOpen = file.Open(strFcvsfile, CFile::modeRead, NULL);
	char szBuff[10000];
	ZeroMemory(szBuff, sizeof(szBuff));
	file.Read(szBuff, 10000);

	ST_INPOOLTASKDATA indata;
	indata.strTaskXml = szBuff;
	CAutoTaskFunc func;
	func.ExtractTaskInfoFromXml(indata);
}

void CDlgTopBar::OnBnClickedBtnNewtask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgNewEMBTask dlg;
	dlg.DoModal();
}

void CDlgTopBar::OnBnClickedRdoAuto()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pMainFrame->m_pMainView->ActiveTab(0);
}

void CDlgTopBar::OnBnClickedRdoManual()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pMainFrame->m_pMainView->ActiveTab(1);

}

void CDlgTopBar::OnBnClickedRdoPlaylist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pMainFrame->m_pMainView->ActiveTab(2);

}


BOOL CDlgTopBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_rdoTab.SetCheck(1);
#ifndef _DEBUG
	m_btnTest.ShowWindow(SW_HIDE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


