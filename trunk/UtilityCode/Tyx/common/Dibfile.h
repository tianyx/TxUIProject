#pragma once
#include "dibapi.h"
BOOL SaveDIB(HDIB hDib, CString& strfile);
BOOL WINAPI SaveDIB(HDIB hDib, CFile& file);
HDIB WINAPI ReadDIBFile(CFile& file);