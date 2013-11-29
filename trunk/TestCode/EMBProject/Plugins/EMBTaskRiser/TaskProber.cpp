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
*Description����������������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
HRESULT CTaskProber::Run_Prober()
{
	return S_OK;
}

/*
*Description��ֹͣ����������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
HRESULT CTaskProber::Stop_Prober()
{
	
	return S_OK;
}

/*
*Description���������
*Input Param��
*      taskIn ��EMB����XML����
*Return Param�����سɹ���ʧ��
*History��
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
*Description�����ûص������ӿ�
*Input Param��
*      pProcessor ���ص�����ָ��
*Return Param�����سɹ���ʧ��
*History��
*/
BOOL CTaskProber::SetTaskProcessor( ITaskCollectCallbackInterface* pProcessor )
{
	ASSERT(m_pTaskProcessor == NULL);
	m_pTaskProcessor = pProcessor;
	return TRUE;
}
