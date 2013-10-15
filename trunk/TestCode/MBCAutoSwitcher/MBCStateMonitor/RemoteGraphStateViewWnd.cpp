// RemoteGraphStateViewWnd.cpp : ʵ���ļ�
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



// CRemoteGraphStateViewWnd ��Ϣ�������



LRESULT CRemoteGraphStateViewWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	return __super::WindowProc(message, wParam, lParam);
}

int CRemoteGraphStateViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
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

	// TODO: �ڴ˴������Ϣ����������
}
