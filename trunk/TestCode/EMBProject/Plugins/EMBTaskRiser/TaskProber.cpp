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

/*
*Description：启动接收任务功能
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskProber::Run_Prober()
{
	return S_OK;
}

/*
*Description：停止接收任务功能
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskProber::Stop_Prober()
{
	
	return S_OK;
}

/*
*Description：添加任务
*Input Param：
*      taskIn ：EMB任务XML描述
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskProber::AddTask( CString& taskIn )
{
	if (!m_pTaskProcessor)
	{
		return EMBERR_NOTREADY;
	}
	
	return S_OK;
}

/*
*Description：设置回调函数接口
*Input Param：
*      pProcessor ：回调函数指针
*Return Param：返回成功或失败
*History：
*/
BOOL CTaskProber::SetTaskProcessor( ITaskCollectCallbackInterface* pProcessor )
{
	ASSERT(m_pTaskProcessor == NULL);
	m_pTaskProcessor = pProcessor;
	return TRUE;
}
