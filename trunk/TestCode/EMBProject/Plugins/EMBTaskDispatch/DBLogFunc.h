#pragma once
#include "FGlobal.h"

#define embdblogtype_main 1
#define embdblogtype_actor 2
#define embdblogtype_executor 3

void CFWriteDBLog_Task(DWORD dwLogKey, int nSrvCode,  LPCTSTR szTaskID, int nState, LPCTSTR szRemark);
void CFWriteDBLog_EMB(DWORD dwLogKey, int nSrvCode, int nType, LPCTSTR szRemark);