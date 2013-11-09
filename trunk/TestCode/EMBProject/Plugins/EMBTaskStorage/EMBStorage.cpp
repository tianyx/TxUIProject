#include "StdAfx.h"
#include "EMBStorage.h"

using namespace EMB;
CEMBStorageMem::CEMBStorageMem(void)
{
}

CEMBStorageMem::~CEMBStorageMem(void)
{
}

//////////////////////////////////////////////////////////////////////////
CEMBStorageDB::CEMBStorageDB(void)
{
}

CEMBStorageDB::~CEMBStorageDB(void)
{
}



//////////////////////////////////////////////////////////////////////////

HRESULT EMB::CEMBStorageMem::UpdateTaskToStorage( const int nDispatchID, CTaskString& szTaskIn )
{
	return S_OK;
}

HRESULT EMB::CEMBStorageMem::FetchTaskFromStorage( const int nDispatchID, int nDesiredNum, VECTASKS& vTasks )
{
	return S_OK;
}

HRESULT EMB::CEMBStorageMem::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

//implement for CEMBStorageDB
//////////////////////////////////////////////////////////////////////////
HRESULT EMB::CEMBStorageDB::UpdateTaskToStorage( const int nDispatchID, CTaskString& szTaskIn )
{
	return S_OK;
}

HRESULT EMB::CEMBStorageDB::FetchTaskFromStorage( const int nDispatchID, int nDesiredNum, VECTASKS& vTasks )
{
	return S_OK;
}

HRESULT EMB::CEMBStorageDB::SubmitTask( const CTaskString& szTaskIn, CTaskString& szRet )
{
	return S_OK;
}
