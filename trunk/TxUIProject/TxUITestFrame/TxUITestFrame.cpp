
// TxUITestFrame.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TxUITestFrame.h"
#include "TxUITestFrameDlg.h"
#include "ITxDrawBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "txdrawbase.lib")

using namespace namespace_TxDrawBase;
// CTxUITestFrameApp

BEGIN_MESSAGE_MAP(CTxUITestFrameApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTxUITestFrameApp ����

CTxUITestFrameApp::CTxUITestFrameApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTxUITestFrameApp ����

CTxUITestFrameApp theApp;


// CTxUITestFrameApp ��ʼ��

BOOL CTxUITestFrameApp::InitInstance()
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

	TxGlobalInit();
	//load draw base
// 	ITxDrawBase* pDrawBase = CreateDrawBase();
// 	if (pDrawBase)
// 	{
// 		pDrawBase->Release();
// 	}
// 
// 	pDrawBase = CreateDrawBase_C();
// 	if (pDrawBase)
// 	{
// 		pDrawBase->Release();
// 	}
//	pDrawBase = NULL;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CTxUITestFrameDlg dlg;
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

int CTxUITestFrameApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	TxGlobalUnInit();
	return CWinAppEx::ExitInstance();
}
