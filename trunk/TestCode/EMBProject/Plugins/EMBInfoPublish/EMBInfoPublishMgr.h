/********************************************************************
	created:	2014/05/08
	created:	5:08:2014   18:04
	filename: 	EMBInfoPublishMgr.h
	author:		tianyx
	
	purpose:	�����������
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include "AutoCritSec.h"
#include "EmbStructDef.h"


typedef int (__cdecl *FUNCEMBPUBLISHINFO)(const char* szData, const int nPubType);
typedef int (__cdecl *FUNCGETDESIREDPUBTYPE)();


struct ST_PUBLISHINFODATA
{
	int nType;
	CString strPubData;
	BOOL ToString();
	BOOL FromString();
	ST_PUBLISHINFODATA(int nTypeIn, CString& strPubDataIn)
	{
		nType = nTypeIn;
		strPubData = strPubDataIn;
	}
};

struct ST_PUBLISHDLL
{
	int nType;
	CString strFile;
	FUNCEMBPUBLISHINFO pFunc;
	HMODULE hmodule;
	ST_PUBLISHDLL()
	{
		nType = 0;
		pFunc = NULL;
		hmodule = NULL;
	}
};

typedef vector<ST_PUBLISHDLL> VECPUBDLLINFOS;

typedef vector<ST_PUBLISHINFODATA> VECPUBINFODATA;

namespace EMB{

class CEMBInfoPublishMgr:
	public IPluginBaseInterface, 
	public IPluginControlInterface, 
	public IPluginConnectorInterce,
	public IPluginConfigInterface,
	public IEMBPublishCallbackInterface
{
public:
	CEMBInfoPublishMgr(void);
	virtual ~CEMBInfoPublishMgr(void);

	//interface for Iunknow
	//����ͷź���
	virtual void	OnFinalRelease();
	//�����ʼ������
	virtual HRESULT OnFirstInit();
	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//interface IPluginConnectorInterce
	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);

	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);


	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();

	//IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);


	virtual BOOL TaskCheckProc();

	//for IEMBPublishCallbackInterface
	virtual HRESULT OnReceivePublishInfo(CTaskString& strInfoIn, const int nPubType);

	BOOL LoadPublishPlugin();
	BOOL UnLoadPublishPlugin();
	BOOL PopTask( VECPUBINFODATA& vTaskOut );
private:

	ITxUnkown* m_pPublishRegister;

	CAutoCritSec m_lockTask;
	//�˳��¼�
	HANDLE m_hQuitEvent;
	//��������ʱ��
	HANDLE m_hTaskEvent;
	//�����߳̾��
	HANDLE m_hTaskCheckProc;

	//�ɽ������������
	int m_nMaxTaskLimit;

	int m_nAllNeedPubType;
	VECPUBINFODATA m_vPubTasks;
	ST_PUBLISHCONFIG m_cfgPub;
	VECPUBDLLINFOS m_vPubDlls;
};

}

extern EMB::CEMBInfoPublishMgr* g_pPluginInstane;

BOOL LoadPublishDll(const CString& strFileIn, HMODULE& hModuleOut, FUNCEMBPUBLISHINFO& pOut, int& nDesiredPubType);

BOOL UnLoadPublishDll(HMODULE hModuleIn);

extern HMODULE g_hGlobalDllModule;