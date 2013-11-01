#pragma once

BOOL TxLoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut);
BOOL TxUnloadPlugin(HMODULE hModuleIn);

CString Guid2String(const GUID& guidIn);
GUID String2Guid(CString& strIn);
GUID TxGenGuid();

DWORD TxWaitObjWithQuit(HANDLE hWait, HANDLE hQuit, DWORD dwTimeOut = INFINITE);

