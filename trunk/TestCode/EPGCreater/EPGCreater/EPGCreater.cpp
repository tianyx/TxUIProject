
// EPGCreater.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "EPGCreater.h"
#include "EPGCreaterDlg.h"
#include "MainDef.h"
#include "TxLogManager.h"
#include "TxADOConn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEPGCreaterApp

BEGIN_MESSAGE_MAP(CEPGCreaterApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEPGCreaterApp ����

CEPGCreaterApp::CEPGCreaterApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CEPGCreaterApp ����

CEPGCreaterApp theApp;


// CEPGCreaterApp ��ʼ��

BOOL CEPGCreaterApp::InitInstance()
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

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	CTxLogManager::GetTxLogMgr()->AddNewLogFile(LOGKEYEPG, TEXT("epgcreater"));
	if (!InitGlobalConfig())
	{
		return FALSE;
	}

	if (!CTxADODBMgr::GetADODBMgr()->OpenDB(DBKEYEGP, g_GlobalInfo.szDBConnString))
	{
		AfxMessageBox(TEXT("���ݿ�����ʧ�ܣ����������ļ�"));
		return FALSE;
	}
	else
	{
		if (!LoadSectionInfo())
		{
			AfxMessageBox(TEXT("��ȡ��Ŀ����Ϣʧ��"));
			return FALSE;
		}
		CTxADODBMgr::GetADODBMgr()->CloseDB(DBKEYEGP);
	}

	CEPGCreaterDlg dlg;
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

int CEPGCreaterApp::ExitInstance()
{
	CTxLogManager::ReleaseTxLogMgr();
	CTxADODBMgr::ReleaseDBMgr();
	return CWinAppEx::ExitInstance();
}
