
// GHClipImporter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GHClipImporter.h"
#include "GHClipImporterDlg.h"
#include "MainDef.h"
#include "TxLogManager.h"
#include "FGlobal.h"
#include "TxADOConn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGHClipImporterApp

BEGIN_MESSAGE_MAP(CGHClipImporterApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGHClipImporterApp ����

CGHClipImporterApp::CGHClipImporterApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGHClipImporterApp ����

CGHClipImporterApp theApp;


// CGHClipImporterApp ��ʼ��

BOOL CGHClipImporterApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	if (IsSameDirStarted())
	{
		AfxMessageBox(TEXT("ͬһĿ¼���Ѿ�������һ������"));
		return FALSE;
	}

	CTxLogManager::GetTxLogMgr()->AddNewLogFile(LOGKEYGHCI, TEXT("CHClipImporter"));
	

	if (!InitGlobalConfig())
	{
		AfxMessageBox(TEXT("�����ļ�����ȷ,������־"));
		return FALSE;
	}

#ifndef FORMONITOR
	if (!CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEYGHCI, g_GlobalInfo.szDBConnString))
	{
		AfxMessageBox(TEXT("���ݿ��ʧ��"));
		return FALSE;
	}
	if (!RefreshClipType())
	{
		CFWriteLog(0, TEXT("ý�����ͻ�ȡʧ�ܣ��������ݿ�"));
		AfxMessageBox(TEXT("ý�����ͻ�ȡʧ�ܣ��������ݿ�"));
		return FALSE;
	}

	CTxADODBMgr::GetADODBMgr()->CloseDB(DBKEYGHCI);
#endif

	CGHClipImporterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CGHClipImporterApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CTxADODBMgr::ReleaseDBMgr();
	CTxLogManager::ReleaseTxLogMgr();
	return CWinAppEx::ExitInstance();
}
