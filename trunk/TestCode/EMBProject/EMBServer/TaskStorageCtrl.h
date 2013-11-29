#pragma once

class CTaskStorageCtrl
{
public:
	CTaskStorageCtrl(void);
	~CTaskStorageCtrl(void);

public:
	// �������
	bool Submit(CommonRequestType TaskInfo, char* cTaskData);
	// ȡ������
	bool CancelTask(char* cTaskID);
	// ��������
	bool ResetTask(char* cTaskID);
	// �޸���������
	bool ModifyTaskInfo(char* cTaskID, CommonRequestType TaskInfo, char* cTaskData);
	// �޸���������
	//0-��ʼ��1-�����죬2-����ִ�У�3-ִ�гɹ���4-ִ��ʧ��
	bool ModifyTaskStatus(char* cTaskID, long nStatus);
	// ��ѯ����״̬
	char* Querytask(char* cTaskID);
};
