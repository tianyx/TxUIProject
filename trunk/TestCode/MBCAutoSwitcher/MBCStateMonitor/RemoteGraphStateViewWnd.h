#pragma once
#include "MBCStateViewWnd.h"
#include "MBCStateQueryObj.h"
#include "AutoCritSec.h"
// CRemoteGraphStateViewWnd

class CRemoteGraphStateViewWnd : public CMBCStateViewWnd, public IGraphStateIncomingCallback
{
	DECLARE_DYNAMIC(CRemoteGraphStateViewWnd)

public:
	CRemoteGraphStateViewWnd();
	virtual ~CRemoteGraphStateViewWnd();
	CMBCStateQueryObj m_RemoteGraphObj;
	virtual HRESULT SetGraphState(ST_GRAPHSTATE& gstateIn);

protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


