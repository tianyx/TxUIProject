#pragma once

class CTaskStorageCtrl
{
public:
	CTaskStorageCtrl(void);
	~CTaskStorageCtrl(void);

public:
	// 添加任务
	bool Submit(CommonRequestType TaskInfo, char* cTaskData);
	// 取消任务
	bool CancelTask(char* cTaskID);
	// 重置任务
	bool ResetTask(char* cTaskID);
	// 修改任务属性
	bool ModifyTaskInfo(char* cTaskID, CommonRequestType TaskInfo, char* cTaskData);
	// 修改任务属性
	//0-初始，1-被认领，2-正在执行，3-执行成功，4-执行失败
	bool ModifyTaskStatus(char* cTaskID, long nStatus);
	// 查询任务状态
	char* Querytask(char* cTaskID);
};
