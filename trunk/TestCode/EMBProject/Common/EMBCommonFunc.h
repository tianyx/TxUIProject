#pragma once

BOOL LoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut);
BOOL UnloadPlugin(HMODULE hModuleIn);

CString Guid2String(const GUID& guidIn);
GUID String2Guid(CString& strIn);

DWORD TxWaitObjWithQuit(HANDLE hWait, HANDLE hQuit, DWORD dwTimeOut = INFINITE);

