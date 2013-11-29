/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	TaskProber.h
	author:		tianyx
	
	purpose:	任务接收基类
*********************************************************************/
#pragma once
#include "EMBCommonFunc.h"
#include "AutoCritSec.h"
#include "TxParamString.h"
#include <vector>

using namespace std;

//媒体处理中心任务结构体
struct ST_EMBTASKDATA
{
	time_t tm;                       //提交时间
	CString taskDesc;                //任务XML内容
	//任务数据
	ST_EMBTASKDATA()
	{
		tm = time(NULL);           
	}
};

typedef vector<ST_EMBTASKDATA> VECTASKDATAS;     //任务列表

interface ITaskCollectCallbackInterface
{
	virtual HRESULT TaskCollectCallback(CString& taskIn) = 0;    //任务收集回调函数
};


class CTaskProber
{
public:
	 CTaskProber(void);
	virtual ~CTaskProber(void);
	// 设置回调函数
	BOOL SetTaskProcessor(ITaskCollectCallbackInterface* pProcessor);
	// 添加任务
	virtual HRESULT AddTask(CString& taskIn);
	// 启动接收任务接口
	virtual HRESULT Run_Prober();
	// 停止接收任务接口
	virtual HRESULT Stop_Prober();
private:
	// 回调函数指针
	ITaskCollectCallbackInterface* m_pTaskProcessor; 

};
