#include "StdAfx.h"
#include "TaskCtrl.h"

CTaskStorageCtrl::CTaskStorageCtrl(void)
{
}

CTaskStorageCtrl::~CTaskStorageCtrl(void)
{
}

//�������
bool CTaskStorageCtrl::Submit(CommonRequestType TaskInfo, char* cTaskData)
{
	return false;
}

// ȡ������
bool CTaskStorageCtrl::CancelTask(char* cTaskID)
{
	return false;
}

// ��������
bool CTaskStorageCtrl::ResetTask(char* cTaskID)
{
	return false;
}

// �޸���������
bool CTaskStorageCtrl::ModifyTaskInfo(char* cTaskID, CommonRequestType TaskInfo, char* cTaskData)
{
	return false;
}

bool CTaskStorageCtrl::ModifyTaskStatus(char* cTaskID, long nStatus)
{
	return false;
}

// ��ѯ����״̬
char* CTaskStorageCtrl::Querytask(char* cTaskID)
{
	return NULL;
}
