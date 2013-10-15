// MBCRemoteCtrl.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "MBCRemoteCtrl.h"
#include "MBCWinApp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 唯一的应用程序对象

CMBCWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
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
