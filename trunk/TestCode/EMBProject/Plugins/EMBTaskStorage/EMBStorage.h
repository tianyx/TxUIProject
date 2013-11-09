/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBStorage.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
namespace EMB{
class CEMBStorageMem:
	public IPluginStorageInterface,
	public IPluginTaskCommit
{
public:
	CEMBStorageMem(void);
	virtual ~CEMBStorageMem(void);

	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);

	virtual HRESULT UpdateTaskToStorage(const int nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage(const int nDispatchID, int nDesiredNum, VECTASKS& vTasks);

};


class CEMBStorageDB:public IPluginStorageInterface
{
public:
	CEMBStorageDB(void);
	virtual ~CEMBStorageDB(void);

	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);


	virtual HRESULT UpdateTaskToStorage(const int nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage(const int nDispatchID, int nDesiredNum, VECTASKS& vTasks);

};

}