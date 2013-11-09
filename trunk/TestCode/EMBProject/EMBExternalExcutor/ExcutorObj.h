/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:02
	filename: 	ExcutorObj.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "FGlobal.h"
#include "embdefine.h"
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"
using namespace EMB;
struct ST_SUBTASKDATA
{
	int nType;
	CString strSubTask;
};

interface IExcutorCallback
{

};
class CExcutorObj
{
private:
	CExcutorObj();
	~CExcutorObj();
public:
	BOOL Init(const EXCUTORID& guidIn, HWND hActor, HWND hMain);
	BOOL Start();
	BOOL Stop();
	BOOL ExcuteTask(ST_EMBTRANSMSG& taskIn, ST_EMBTRANSMSG& retOut);
	BOOL ProcessMsg(ST_EMBTRANSMSG& taskIn);

private:
	BOOL OnExcutorRegister(ST_EXCUTORREG& infoIn);
	BOOL OnExcutorUnRegister(ST_EXCUTORINFO& infoIn);
public:
	HWND m_hwndExcMsg;
private:
	CString m_strTask;
	vector<ST_SUBTASKDATA> vSubTasks;
	ST_TASKRUNSTATE m_runState;
	EXCUTORID m_ExcutorGuid;
	VECPLUGINFOS m_vexcInfo;
	HWND m_hwndMain;
	HWND m_hwndActor;
	HANDLE m_hThreadExc;
	static CExcutorObj* m_spExcObj;

public:
	static CExcutorObj* GetExcutorObj(const EXCUTORID& guidIn, HWND hActor, HWND hMain);
	static BOOL Release();
};