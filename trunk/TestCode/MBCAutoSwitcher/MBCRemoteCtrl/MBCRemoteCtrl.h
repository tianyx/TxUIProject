#pragma once
#include "MBCRemoteInterface.h"
#include "MBCRemoteObj.h"

class CMBCRemoteCtrlMgr :public IMBCRemoteMgr
{
private:
	CMBCRemoteCtrlMgr();
	~CMBCRemoteCtrlMgr();

	static CMBCRemoteCtrlMgr* m_pMgr;

	CMBCRemoteObj m_obj;
public:
	virtual HRESULT Init(SOCKADDR_IN& addrListenIn , IMBCMSGRemoteCallInterface* pCallBackProc);
	virtual HRESULT Run();
	virtual HRESULT Stop();
public:
	static CMBCRemoteCtrlMgr* CreateMgr();
	static void Release();

};

