#pragma once

#define MSG_LICENSETOBEEXPIRE WM_USER + 4682

#define MACROERRNORET
#define MACROERRRETNONE return;
#define MACROERRRETFALSE return FALSE;
#define MACROERREXIT	exit(-11); 

#define MACROERRACTION(bPopErr, MACROERRRET) \
if (nRetCode < 0)\
{\
	if (bPopErr)\
	{\
	CString strLCMsgRet;\
	if (nRetCode >= -87 && nRetCode <= -85)\
	{\
		strLCMsgRet = TEXT("��Ȩ�ѹ��ڣ��뼰ʱ����!");\
	}\
	else if(nRetCode == -102)\
	{\
		strLCMsgRet = TEXT("δ�ҵ���Ȩ��Ϣ���뷢�ͳ���Ŀ¼�µ�applyCode.dat�ļ�������Ȩ��");\
	}\
	else if(nRetCode == -200)\
	{\
		strLCMsgRet = TEXT("δ�ҵ�zqLpt.dll�ļ�");\
	}\
	else\
	{\
		strLCMsgRet.Format("��Ȩ��Ϣ��֤����code = %d", nRetCode);\
	}\
	AfxMessageBox(strLCMsgRet);\
	}\
	MACROERRRET\
}\
else if(nRetCode == 11)\
{\
	if (bPopErr)\
	{\
	CString strLCMsgRet;\
	strLCMsgRet.Format("��Ȩ��Ϣ�ѹ��ڣ��뾡�����", nRetCode);\
	AfxMessageBox(strLCMsgRet);\
	}\
}\
else if(nRetCode !=0)\
{\
	AfxMessageBox("��Ȩ���򲻿�ʶ��");\
	MACROERRRET\
}\


#define MACROZQLCCHECK(bPopErr, MACROERRRET) \
typedef int (__cdecl *INITZQ)(char* lparam, int* nRand);\
int nRetCode = -200;\
char szbuff[256];\
memset(szbuff, 0, sizeof(szbuff));\
GetModuleFileName(NULL, szbuff, sizeof(szbuff) -1);\
CString strAppPath = szbuff;\
int nPos = strAppPath.ReverseFind('\\');\
CString szFile = strAppPath.Mid(0, nPos);\
szFile += TEXT("\\ZQLpt.dll");\
HMODULE hLCModule = LoadLibrary(szFile);\
if (hLCModule != NULL)\
{\
	INITZQ  zqFun = (INITZQ)GetProcAddress(hLCModule, "InitZQApp2");\
	if (zqFun)\
	{\
		SYSTEMTIME tmForLc;\
		GetSystemTime(&tmForLc);\
		int nRandForLc = tmForLc.wSecond*tmForLc.wMinute*tmForLc.wHour;\
		nRetCode = zqFun(NULL, &nRandForLc);\
		nRetCode -= nRandForLc;\
		if (nRetCode < 0)\
		{\
			typedef int (__cdecl *GenZQReqCode)();\
			GenZQReqCode pGenReqCodeFn;\
			pGenReqCodeFn = (GenZQReqCode)GetProcAddress(hLCModule, "GenZQReqCode");\
			if (pGenReqCodeFn)\
			{\
				int nRetReq  = pGenReqCodeFn();\
			}\
		}\
	}\
	FreeLibrary(hLCModule);\
	hLCModule = NULL;\
}\
MACROERRACTION(bPopErr, MACROERRRET) \


#define MACROZQLCCHECK_POP MACROZQLCCHECK(TRUE, MACROERRRETNONE)
#define MACROZQLCCHECK_NOPOP MACROZQLCCHECK(FALSE, MACROERRRETNONE)


///for chboradcast
#define DELAYNOTIFYINTERVAL 86400000
#define MACRODEFDELAYNOTIFYPROC(nInterval) \
DWORD __stdcall LicenseDelayNotifyProc(LPVOID wparam)\
{\
	while(TRUE)\
	{\
		Sleep(nInterval);\
		HWND hwnd = (HWND)wparam;\
		if (hwnd && ::IsWindow(hwnd))\
		{\
			::SendMessage(hwnd, MSG_LICENSETOBEEXPIRE, 0,0);\
		}\
	}\
}\

#define MACRORUNDELAYLICENSE(hDelayNotifyer) \
if(nRetCode == 11)\
{\
DWORD dwNotifyThread = 0;\
::CreateThread(NULL, 0, LicenseDelayNotifyProc, (LPVOID)hDelayNotifyer, 0, &dwNotifyThread);\
}\


