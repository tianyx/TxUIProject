#pragma once
#include "MBCStateViewWnd.h"
#include "MBCStateQueryObj.h"
#include "AutoCritSec.h"
// CRemoteGraphStateViewWnd
#include <vector>
typedef std::vector<ST_OBJSTATEINFO> VECMASTERINFO;

class CRemoteGraphStateViewWnd : 
	public CMBCStateViewWnd, 
	public IGraphStateIncomingCallback,
	public IMBCRelayControlInterafce

{
	DECLARE_DYNAMIC(CRemoteGraphStateViewWnd)

public:
	CRemoteGraphStateViewWnd();
	virtual ~CRemoteGraphStateViewWnd();
	CMBCStateQueryObj m_RemoteGraphObj;
	virtual HRESULT SetGraphState(ST_GRAPHSTATE& gstateIn);

	HRESULT ChangeRelayAddr(char* szChId, SOCKADDR_IN& addrRelay);
	BOOL GetMasterChIds(VECMASTERINFO& vChIdOut);

protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


