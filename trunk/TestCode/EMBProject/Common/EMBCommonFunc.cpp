#include "StdAfx.h"
#include "EMBCommonFunc.h"
#include "Rpc.h"
#pragma comment(lib, "Rpcrt4.lib")

typedef int (__cdecl *GETPLUGININSTANCE)(LPVOID& pInterface);

BOOL TxLoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut)
{
		hModuleOut = LoadLibrary(strFileIn);
		if (hModuleOut != NULL)
		{
			GETPLUGININSTANCE  pFun = (GETPLUGININSTANCE)GetProcAddress(hModuleOut, "GetPluginInstance");
			if (pFun)
			{
				pFun(pInterfaceOut);
			}
			else
			{
				FreeLibrary(hModuleOut);
				hModuleOut = NULL;
			}
			
		}
	return TRUE;
}

BOOL TxUnloadPlugin(HMODULE hModuleIn)
{
	BOOL bRet = TRUE;
	if (hModuleIn)
	{
		bRet = FreeLibrary(hModuleIn);
	}

	return bRet;
}

CString Guid2String(const GUID& guidIn )
{
	RPC_CSTR* pStr = NULL;
	UuidToString(&guidIn, pStr);
	CString strTmp;
	if (pStr)
	{
		strTmp = (char*)pStr;
		RpcStringFree(pStr);
	}
	return strTmp;
}
GUID String2Guid( CString& strIn )
{
	GUID guid = GUID_NULL;
	if(UuidFromString((RPC_CSTR)strIn.GetBuffer(), &guid)== RPC_S_OK)
	{
		return guid;
	}
	else
	{
		return GUID_NULL;
	}
}

DWORD TxWaitObjWithQuit( HANDLE hWait, HANDLE hQuit, DWORD dwTimeOut /*= INFINITE*/ )
{
	HANDLE handls[2];
	handls[0] = hWait;
	handls[1] = hQuit;
	return WaitForMultipleObjects(2, handls, FALSE, dwTimeOut);
}

GUID TxGenGuid()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);
	return guid;
}



