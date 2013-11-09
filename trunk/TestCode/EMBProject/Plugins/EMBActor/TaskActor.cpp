#include "StdAfx.h"
#include "TaskActor.h"
#include "TxLogManager.h"
#include "io.h"
using namespace EMB;
CTaskActor::CTaskActor(void)
{
	m_bRuning = FALSE;
	m_pExcMgr = CExcutorMgr::GetExcutorMgr();
	ASSERT(m_pExcMgr);
}

CTaskActor::~CTaskActor(void)
{
	CExcutorMgr::Release();
	g_pPluginInstane = NULL;
}

HRESULT CTaskActor::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IPluginManager;
	info.nPlugInType = PluginType_Actor;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT CTaskActor::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

	if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_IControler)
	{
		pInterfaceOut = dynamic_cast<IPluginControlInterface*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_IConnector)
	{
		pInterfaceOut = dynamic_cast<IPluginConnectorInterce*>(this);
		AddRef();
		return S_OK;

	}
	if (guidIn == GuidEMBPlugin_ITaskCommit)
	{
		pInterfaceOut = dynamic_cast<IPluginTaskCommit*>(this);
		AddRef();
		return S_OK;
	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

HRESULT CTaskActor::Run_Plugin()
{
	Stop_Plugin();
	SOCKADDR_IN addrLocal;
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(0);
	addrLocal.sin_addr.S_un.S_addr = inet_addr( INADDR_ANY );

	m_actorconnMain.SetScokAddr(&m_ActRegInfo.addrMain, &addrLocal);
	m_actorconnSlave.SetScokAddr(&m_ActRegInfo.addrSlave, &addrLocal);
	m_pExcMgr->Init(m_ActRegInfo.strExcPath);
	HRESULT hr = m_actorconnMain.Run();
	MUSTBESOK(hr);
	hr = m_actorconnSlave.Run();
	MUSTBESOK(hr);
	hr = m_pExcMgr->Run();
	MUSTBESOK(hr);
	m_bRuning = TRUE;
	return S_OK;
}

HRESULT CTaskActor::Stop_Plugin()
{
	m_bRuning = FALSE;
	m_actorconnMain.Stop();
	m_actorconnSlave.Stop();
	m_pExcMgr->Stop();

	return S_OK;
}

HRESULT EMB::CTaskActor::OnActorConnectorMsg( ST_EMBTRANSMSG& pMsg )
{
	return S_OK;
}

void EMB::CTaskActor::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCTaskRiserMgr::OnFinalRelease() ");
	delete this;
}

HRESULT EMB::CTaskActor::OnFirstInit()
{
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskActor.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKACTOR, strFile);

	return S_OK;
}

HRESULT EMB::CTaskActor::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	//return all
	m_ActRegInfo.ToString(szOut);
	return S_OK;
}

HRESULT EMB::CTaskActor::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	ST_EMBRET ret;
	if (m_bRuning)
	{
		ret.nRetVal = EMBERR_NOTREADY;
	}
	else
	{
		ST_ACTORREG regIn;
		regIn.FromString(szIn);

		//check valid
		if (regIn.guid  < 0
			|| regIn.addrMain.sin_family  != AF_INET
			|| regIn.addrSlave.sin_family != AF_INET
			|| regIn.nExcutorMinId >= regIn.nExcutorMaxId
			|| _access(regIn.strExcPath, 0) ==-1)
		{
			ret.nRetVal = EMBERR_INVALIDARG;
		}
		else
		{
			m_ActRegInfo = regIn;
		}
	}

	ret.ToString(szOut);
	return ret.nRetVal;
}
