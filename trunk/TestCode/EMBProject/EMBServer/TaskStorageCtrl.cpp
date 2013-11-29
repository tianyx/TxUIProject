#include "StdAfx.h"
#include "TaskCtrl.h"

CTaskStorageCtrl::CTaskStorageCtrl(void)
{
}

CTaskStorageCtrl::~CTaskStorageCtrl(void)
{
}

//添加任务
bool CTaskStorageCtrl::Submit(CommonRequestType TaskInfo, char* cTaskData)
{
	return false;
}

// 取消任务
bool CTaskStorageCtrl::CancelTask(char* cTaskID)
{
	return false;
}

// 重置任务
bool CTaskStorageCtrl::ResetTask(char* cTaskID)
{
	return false;
}

// 修改任务属性
bool CTaskStorageCtrl::ModifyTaskInfo(char* cTaskID, CommonRequestType TaskInfo, char* cTaskData)
{
	return false;
}

bool CTaskStorageCtrl::ModifyTaskStatus(char* cTaskID, long nStatus)
{
	return false;
}

// 查询任务状态
char* CTaskStorageCtrl::Querytask(char* cTaskID)
{
	return NULL;
}
