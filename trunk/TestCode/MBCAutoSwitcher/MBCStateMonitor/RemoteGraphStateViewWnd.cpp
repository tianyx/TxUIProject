// RemoteGraphStateViewWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteGraphStateViewWnd.h"
#include "GlobalDef.h"

// CRemoteGraphStateViewWnd

IMPLEMENT_DYNAMIC(CRemoteGraphStateViewWnd, CMBCStateViewWnd)

CRemoteGraphStateViewWnd::CRemoteGraphStateViewWnd()
{

}

CRemoteGraphStateViewWnd::~CRemoteGraphStateViewWnd()
{
}


BEGIN_MESSAGE_MAP(CRemoteGraphStateViewWnd, CMBCStateViewWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

HRESULT CRemoteGraphStateViewWnd::SetGraphState( ST_GRAPHSTATE& gstateIn )
{
	CAutoLock lock(&m_lockgs);
	m_gsState = gstateIn;
	PostMessage(MSG_UPDATE_GRAPHSTATE, 0,0);
	return S_OK;
}



// CRemoteGraphStateViewWnd 消息处理程序



LRESULT CRemoteGraphStateViewWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	return __super::WindowProc(message, wParam, lParam);
}

int CRemoteGraphStateViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_RemoteGraphObj.m_addrLocal = g_GlobalInfo.addrLocal;
	m_RemoteGraphObj.m_addrRemote = g_GlobalInfo.addrRemote;
	m_RemoteGraphObj.SetGraphStateIncomingCallback(this);
	m_RemoteGraphObj.Run();

	return 0;
}

void CRemoteGraphStateViewWnd::OnDestroy()
{
	m_RemoteGraphObj.Stop();
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
