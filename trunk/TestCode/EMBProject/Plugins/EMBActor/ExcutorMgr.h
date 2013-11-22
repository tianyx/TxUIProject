/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:59
	filename: 	ExcutorMgr.h
	author:		tianyx
	
	purpose:	excutor manager
*********************************************************************/
#pragma once
#include "fglobal.h"
#include "EmbStructDef.h"
#include "AutoCritSec.h"
#include <map>
using namespace std;

interface IExcutorMgrInterface
{
	virtual HRESULT SendToExcutor(const EXCUTORID excutorId,CString& szInfo) = 0;
};

interface IExcutorMsgCallBack
{
	virtual HRESULT OnExcutorMessage(const EXCUTORID excutorId, CString& szInfoIn) = 0;
	virtual HRESULT OnExcutorExit(const EXCUTORID excutorId) = 0;
};


typedef map<EXCUTORID, ST_EXCUTORINFO> MAPEXCUTORS;

class CExcutorMgr
{
private:
	CExcutorMgr(void);
	virtual ~CExcutorMgr(void);

public:
	BOOL Init(LPCTSTR strExcPathIn, ST_ACTORCONFIG& actCfg, IExcutorMsgCallBack* pCallBack);
	HRESULT Run();
	HRESULT Stop();
	EXCUTORID CreateNewExcutor();
	BOOL StopExcutor(const EXCUTORID guidIn);
	//interface for IExcutorMgrInterface
	virtual HRESULT SendToExcutor(const EXCUTORID excutorId,CString& szInfo);
private:
	//HRESULT SendMessageToExcutor(const EXCUTORID guid, WPARAM wParam, LPARAM lParam);
	EXCUTORID GetFirstNotUsedExcutorId();
	HRESULT CheckExcutor();
	BOOL LaunchExcutorFile(const EXCUTORID excId, DWORD& dwProcessId);
public:
	HRESULT OnExcutorMessage(ST_EMBWNDMSG& msgIn);
	HRESULT SaveMessageToPool(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	HRESULT OnExcutorReg(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	HRESULT CheckExcutorLoop();
	HRESULT WndMsgPoolCheckLoop();
	HANDLE CreateExchangemapping(EXCUTORID excId);

public:
	HWND m_hMessageWnd;

private:
	void ClearMsgPool();
private:
	CString m_strExcPath;
	CString m_strExcFolder;
	CString m_strExcutorWorkFolder;
	CString m_strExcName;
	EXCUTORID m_nMinExcutorId;
	EXCUTORID m_nMaxExcutorId;
	ACTORID m_nActorID;

	CAutoCritSec m_csExcutors;
	MAPEXCUTORS m_mapExcutors;
	static CExcutorMgr* m_spExcMgr;

	HANDLE m_hMsgLoopThread;
	HANDLE m_hCheckThread;
	HANDLE m_hEventQuitLoop;

	HANDLE m_hEventPoolMsgArrival;
	HANDLE m_hMsgPoolCheckThread;
	CAutoCritSec m_csMsgPool;
	VECWNDMSG m_vMsgPool;

	IExcutorMsgCallBack* m_pIExcCallBack;

	int nfgExcutorLaunchTimeout;
public:
	static CExcutorMgr* GetExcutorMgr();
	static BOOL Release();
};
