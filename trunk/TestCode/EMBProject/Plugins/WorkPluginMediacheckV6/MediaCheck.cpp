#include "StdAfx.h"
#include "MediaCheck.h"
using namespace EMB;

CMediaCheck::CMediaCheck(void)
{
	m_pFCVSClass = new CFCVSClass();
}

CMediaCheck::~CMediaCheck(void)
{
	if (m_pFCVSClass != NULL)
	{
		delete m_pFCVSClass;
	}
}

HRESULT EMB::CMediaCheck::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;
	if (guidIn == GuidEMBPlugin_IBaseVC6)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterfaceVC6*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_ITaskWorkerCall)
	{
		pInterfaceOut = dynamic_cast<ITaskWorkerCallInterface*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return IPluginBaseInterfaceVC6::QueryInterface(guidIn, pInterfaceOut);
	}
}

HRESULT EMB::CMediaCheck::QueryPluginInfo( ST_PluginInfo& InfoOut )
{
	InfoOut.pluginGuid = GuidEMBPlugin_PWorkerMediaCheck;
	InfoOut.nPlugInType = PluginType_Wroker;
	InfoOut.nSubType = SubType_MediaCheck;
	return S_OK;
}

HRESULT EMB::CMediaCheck::OnFirstInit()
{
	return S_OK;
}

void EMB::CMediaCheck::OnFinalRelease()
{

	
	delete this;
}

HRESULT EMB::CMediaCheck::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	m_pFCVSClass->MSGget = szTaskIn;
	
	HRESULT hr =  m_pFCVSClass->ConfigInit();
	if (FAILED(hr))
	{
		return hr;
	}
	m_pFCVSClass->m_reportCallBack = pICallback;
	hr = 	m_pFCVSClass->Taskstart();
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT EMB::CMediaCheck::CancelTask()
{
	m_pFCVSClass->TaskCancel();
	return S_OK;
}

HRESULT EMB::CMediaCheck::GetTaskProgress( CEMBWorkString& szInfo )
{
	
	CString stemp = 	m_pFCVSClass->reportPencent(TRUE);
	szInfo = stemp;
	return S_OK;
}
