#pragma once
#include "MBCCommonDef.h"

#ifdef MBCREMOTECTRL_EXPORTS
#define MBCREMOTECTRL_API __declspec(dllexport)
#else
#define MBCREMOTECTRL_API __declspec(dllimport)
#endif

interface IMBCMSGRemoteCallInterface
{
	virtual HRESULT GetLiveInfo(ST_MBCMSGBASE& basemsg, int& nRemoteState, int& nSelftype,  ST_MBCCHANNELINFO_FORVC6* pArrayOut, const int nArrLen, int& chUsed) = 0;
	virtual HRESULT DoAction(ST_MBCMSG_ACTQA& msg) = 0;
};


interface IMBCRemoteMgr
{
	virtual HRESULT Init(SOCKADDR_IN& addrListenIn , IMBCMSGRemoteCallInterface* pCallBackProc) = 0;
	virtual HRESULT Run() = 0;
	virtual HRESULT Stop() = 0;
};


extern "C"
MBCREMOTECTRL_API IMBCRemoteMgr* GetMBCRemoteMgr(void);
extern "C"
MBCREMOTECTRL_API void ReleaseMBCRemoteMgr(void);

// class MBCREMOTECTRL_API CMBCRemoteCtrl {
// public:
// 	CMBCRemoteCtrl(void);
// 	// TODO: 在此添加您的方法。
// };