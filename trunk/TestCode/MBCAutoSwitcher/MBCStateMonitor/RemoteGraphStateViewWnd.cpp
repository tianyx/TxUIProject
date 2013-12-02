// RemoteGraphStateViewWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteGraphStateViewWnd.h"
#include "GlobalDef.h"
#include "FGlobal.h"
#include "MBCTransMsg.h"

#define IDMENU_RELAYCHANGE 100
#define	IDMENU_MASTERBASE 200
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
	ON_WM_CONTEXTMENU()
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

HRESULT CRemoteGraphStateViewWnd::ChangeRelayAddr( char* szChId, SOCKADDR_IN& addrRelay )
{
	if (m_RemoteGraphObj.GetState() != MBCSTATE_OK)
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("remote connect error!"));
		return FALSE;
	}

	ST_CHANGERELAY msgOut;
	msgOut.nId = GetTickCount();
	msgOut.addrRelay = addrRelay;
	strcpy(msgOut.strChId, szChId);
	char buffer[256];
	int nUsed = 0;
	PackMBCMsg(msgOut, buffer, 256, nUsed);
	HRESULT hr = send(*m_RemoteGraphObj.m_pSockBase, buffer, nUsed, 0);
	if (hr == SOCKET_ERROR)
	{
		hr = WSAGetLastError();
		ASSERT(FALSE);
	}

	return S_OK;
}

void CRemoteGraphStateViewWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
	ST_OBJSTATEINFO endBackInfo;
	if (!HitTestObject(endBackInfo))
	{
		return;
	}

	if (endBackInfo.nObjType != MBCOBJTYPE_ENDBACK
		|| endBackInfo.nSelfState != MBCSTATE_OK)
	{
		return;
	}

	std::vector<ST_MBCCHANNELINFO>& vAddrRelay = endBackInfo.remoteInfo.vCHInfo;

	VECMASTERINFO vMasterInfos;
	GetMasterChIds(vMasterInfos);
	if (vMasterInfos.size() == 0)
	{
		return;
	}

	std::vector<size_t> vNotRelayedMaster;

	for (size_t i = 0; i < vMasterInfos.size(); ++i)
	{
		SOCKADDR_IN addrMaster = vMasterInfos[i].addrRemote;
		BOOL bRelyed = FALSE;
		for (size_t j = 0; j < vAddrRelay.size(); ++j)
		{
			if (addrMaster == vAddrRelay[j].addrDest)
			{
				//already relayed, skip it
				bRelyed = TRUE;
				break;
			}
		}

		if (bRelyed)
		{
			continue;
		}
		vNotRelayedMaster.push_back(i);
	}

	if (vNotRelayedMaster.size() == 0)
	{
		return;
	}


	CMenu menu;
	menu.CreatePopupMenu();
	CMenu menuPop;
	menuPop.CreatePopupMenu();
	for (size_t i = 0; i < vNotRelayedMaster.size(); ++i)
	{
		ST_OBJSTATEINFO& infoTmp = vMasterInfos[vNotRelayedMaster[i]];
		CString strMenuText;
		strMenuText.Format(TEXT("%s, %s"),infoTmp.strChId, Addr2String(infoTmp.addrRemote).c_str());
		menuPop.InsertMenu(i, MF_BYPOSITION|MF_ENABLED|MF_STRING, i+IDMENU_MASTERBASE, strMenuText);
	}

	menu.AppendMenu(MF_POPUP|MF_STRING, (UINT)menuPop.GetSafeHmenu(), TEXT("Relay to..."));

	CPoint pt;
	GetCursorPos(&pt);
	BOOL IdRet = menu.TrackPopupMenu(TPM_RETURNCMD|TPM_LEFTALIGN|TPM_TOPALIGN|TPM_LEFTBUTTON, pt.x, pt.y, this);
	if (IdRet >= IDMENU_MASTERBASE && IdRet < IDMENU_MASTERBASE + vNotRelayedMaster.size())
	{
		//send message
		HRESULT hr = ChangeRelayAddr(vMasterInfos[vNotRelayedMaster[IdRet - IDMENU_MASTERBASE]].strChId.LockBuffer(), endBackInfo.addrRemote);
		if (hr != S_OK)
		{
			ASSERT(FALSE);
		}
	}


}

BOOL CRemoteGraphStateViewWnd::GetMasterChIds( VECMASTERINFO& vInfoOut )
{
	CAutoLock lock(&m_lockgs);
	for (size_t i = 0; i < m_gsState.vEndInfo.size(); ++i)
	{
		if (m_gsState.vEndInfo[i].nObjType == MBCOBJTYPE_ENDMASTER)
		{
			vInfoOut.push_back(m_gsState.vEndInfo[i]);
		}
	}

	return TRUE;
}
