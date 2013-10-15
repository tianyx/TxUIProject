// MBCRemoteCtrl.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "MBCRemoteCtrl.h"
#include "MBCWinApp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// Ψһ��Ӧ�ó������

CMBCWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}

	return nRetCode;
}

CMBCRemoteCtrlMgr* CMBCRemoteCtrlMgr::m_pMgr = NULL;
CMBCRemoteCtrlMgr* CMBCRemoteCtrlMgr::CreateMgr()
{
	if (CMBCRemoteCtrlMgr::m_pMgr)
	{
		return CMBCRemoteCtrlMgr::m_pMgr;
	}

	m_pMgr = new CMBCRemoteCtrlMgr;
	return m_pMgr;
}

void CMBCRemoteCtrlMgr::Release()
{
	if (CMBCRemoteCtrlMgr::m_pMgr)
	{
		m_pMgr->Stop();
		delete m_pMgr;
		m_pMgr = NULL;
	}
}

CMBCRemoteCtrlMgr::CMBCRemoteCtrlMgr()
{

}

CMBCRemoteCtrlMgr::~CMBCRemoteCtrlMgr()
{
}

HRESULT CMBCRemoteCtrlMgr::Init( SOCKADDR_IN& addrListenIn , IMBCMSGRemoteCallInterface* pICallback )
{
	HRESULT hr = S_OK;
	m_obj.m_addrLocal = addrListenIn;
	m_obj.m_pIRemoteCallInterface = pICallback;
	return hr;
}

HRESULT CMBCRemoteCtrlMgr::Run()
{
	HRESULT hr = S_OK;
	hr = m_obj.Run();
	return hr;
}

HRESULT CMBCRemoteCtrlMgr::Stop()
{
	HRESULT hr = S_OK;
	hr = m_obj.Stop();
	return hr;
}


MBCREMOTECTRL_API IMBCRemoteMgr* GetMBCRemoteMgr( void )
{
	return CMBCRemoteCtrlMgr::CreateMgr();
}

MBCREMOTECTRL_API void ReleaseMBCRemoteMgr( void )
{
	CMBCRemoteCtrlMgr::Release();
}
