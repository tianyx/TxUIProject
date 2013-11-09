#include "StdAfx.h"
#include "TaskProber.h"
#include "AutoCritSec.h"
#include "FGlobal.h"
#include "EMBDefine.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CTaskProber::CTaskProber(void)
{
	m_pTaskProcessor = NULL;
}

CTaskProber::~CTaskProber(void)
{
}


HRESULT CTaskProber::Run_Prober()
{
	return TRUE;
}

HRESULT CTaskProber::Stop_Prober()
{
	
	return TRUE;
}


HRESULT CTaskProber::AddTask( CString& taskIn )
{
	if (!m_pTaskProcessor)
	{
		return EMBERR_NOTREADY;
	}
	
	return S_OK;
}



BOOL CTaskProber::SetTaskProcessor( ITaskCollectCallbackInterface* pProcessor )
{
	ASSERT(m_pTaskProcessor == NULL);
	m_pTaskProcessor = pProcessor;
	return TRUE;
}
