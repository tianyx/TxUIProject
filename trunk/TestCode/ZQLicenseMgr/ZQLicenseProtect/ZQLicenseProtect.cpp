// ZQLicenseProtect.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ZQLicenseProtect.h"
#include "TxSystemOp.h"
#include "atlbase.h"
#include "shlobj.h"
#include "atltime.h"
#include "io.h"
#include <algorithm>
#include <fstream>
#include "StrConvert.h"

using namespace std;


struct ST_OUTINFO
{
	__int64 nLastCheckTime;
	__int64 nChangedDate;
	ST_NAME name;
};

enum ENUMLCERR
{
	LCERR_SUCCESS					= 0,
	LCERR_UNMATCHKEY3_TIMEBACKWARD	= 10,
	LCERR_LICENSEINDELAYUSEMODE		= 11,

	LCERR_KEYNOTFOUND				= -102,

	LCERR_REGOPEN					= -101,
	LCERR_FILEOPEN					= -100,
	LCERR_FILEACCESS				= -99,
	LCERR_FILELENCHECK				= -98,

	LCERR_KEYEXTRACT				= -97,
	LCERR_SYSDATAPATH				= -96,

	LCERR_GETMAC					= -95,
	LCERR_PROCDUCTNAME_GETERR		= -94,
	LCERR_DATEFILEOPEN				= -93,

	LCERR_UNMATCHKEY1				= -90,
	LCERR_UNMATCHKEY2				= -89,
	LCERR_UNMATCHKEY3_FMT			= -88,
	LCERR_UNMATCHKEY3_EXPIRED		= -87,
	LCERR_UNMATCHKEY3_CHANGEDEXPIRED	= -86,
	LCERR_UNMATCHKEY3_CHANGEDNOWEXPIRED = -85,
	LCERR_CRASHED						= -200,

	LCERR_REQCODEGENERR					= -300,
	LCERR_REQFILEWRITEERR				= -301,
	LCERR_REQFILEALREADYEXIST			= -302,

};

#define LCCHECKTYPE_NONE 0
#define LCCHECKTYPE_REG 1
#define LCCHECKTYPE_FILE 2

int CheckLC(const char* szData, ST_OUTINFO& out, BOOL bWriteBack = TRUE);

ZQLICENSEPROTECT_API int __cdecl InitZQApp(const char* szData)
{
	ST_OUTINFO out;
	return CheckLC(szData, out);
	//{added by tianyx  10:7:2013   9:45
}

ZQLICENSEPROTECT_API int __cdecl InitZQApp2(const char* szData, int* dwRand)
{
	if (dwRand == NULL)
	{
		return 0xffffffff;
	}
	ST_OUTINFO out;
	int nRet = CheckLC(szData, out);
	//{added by tianyx  10:7:2013   9:45
	//encrypt the return value
	nRet += *dwRand;
	return nRet;
	//}end added by tianyx
}

ZQLICENSEPROTECT_API int __cdecl GetZQApp( char* szData, const int nBuff )
{
	if (nBuff < 1024 || szData == NULL)
	{
		return 1024;
	}
	ST_OUTINFO out;
	CheckLC(szData, out, FALSE);
	string szLastCheck = out.nLastCheckTime == 0? TEXT(""):CTime(out.nLastCheckTime).Format(TEXT("%Y-%m-%d"));
	string szChangedDate = out.nChangedDate == 0? TEXT(""):CTime(out.nChangedDate).Format(TEXT("%Y-%m-%d"));
	CTime tmCheck(out.nLastCheckTime);
	sprintf_s(szData, nBuff, "lastcheckdate = %s\r\nexpireDate = %s\r\nkey1 = %s\r\nkey2=%s\r\nkey3=%s\r\nkey4=%s", 
		szLastCheck.c_str(), szChangedDate.c_str(), out.name.key1.c_str(), out.name.key2.c_str(),out.name.key3.c_str(), out.name.key4.c_str());
	return 0;
}

int CheckLC(const char* szData, ST_OUTINFO& out, BOOL bWriteBack)
{
	ifstream ifs;
	ifstream ifsDt;
	ofstream ofile;
	int nRet = 0;
	ST_NAME& name = out.name;
	int nLCCheckType = LCCHECKTYPE_NONE;

	//get app name
	string szProcName = GetProductName();
	if (szProcName.size() == 0)
	{
		nRet = LCERR_PROCDUCTNAME_GETERR;
		return nRet;
	}

/*
	char szbuff[GENLENGTH*2];
	GetModuleFileName(NULL, szbuff, 512);
	std::string szAppName = szbuff;
	size_t nPos = szAppName.rfind('\\');
	size_t nPos2 = szAppName.rfind('.');
	if (nPos == string::npos || nPos2 == string::npos
		|| nPos2 < nPos)
	{
		nRet = LCERR_UNMATCHKEY2;
		return nRet;
	}

	szProcName = szAppName.substr(nPos+1, nPos2 - nPos-1);
	for_each(name.key2.begin(), name.key2.end(), toupper);
*/

	try
	{
		if (szData != NULL)
		{
			name.key4 = szData;
		}

		if (name.key4.size() == 0)
		{
			char szbuff[GENLENGTH*2+1];
			memset(szbuff, 0, sizeof(szbuff));
			GetModuleFileName(NULL, szbuff, sizeof(szbuff) -1);
			std::string wsPath = szbuff;
			size_t nPos = wsPath.rfind('\\');
			string szFile = wsPath.substr(0, nPos);
			szFile += TEXT("\\license.dat");
			if (_access(szFile.c_str(), 00) == -1)
			{
				nRet = LCERR_KEYNOTFOUND;
			}
			else
			{
				ifs.open(Ansi2W(szFile).c_str(), ios_base::in);
				if (!ifs.is_open())
				{
					nRet = LCERR_FILEOPEN;
				}
				else
				{
					ifs.seekg(0, ios_base::end );
					int nFileLen = ifs.tellg();
					if (nFileLen < GENLENGTH)
					{
						nRet = LCERR_FILELENCHECK;
					}
					else
					{
						ifs.seekg(0, ios_base::beg);
						memset(szbuff, 0, sizeof(szbuff));
						ifs.read((char*)szbuff, sizeof(szbuff) -1);
						nLCCheckType = LCCHECKTYPE_FILE;
						name.key4 = szbuff;
					}
					ifs.close();
				}

				if (nRet != LCERR_SUCCESS && nRet != LCERR_KEYNOTFOUND)
				{
					return nRet;
				}
			}
			
		}

		nRet = LCERR_SUCCESS;
		if ( name.key4.size() == 0)
		{
			//check reg
			CRegKey reg;
			LONG lRet = reg.Open(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\zqvideo"), KEY_READ|KEY_WOW64_64KEY);
			if (lRet == ERROR_SUCCESS)
			{
				char szkeybuff[GENLENGTH*2+1];
				memset(szkeybuff, 0, sizeof(szkeybuff));
				DWORD dwBuffSize = sizeof(szkeybuff) -1;
				DWORD dwType = REG_SZ;
				reg.QueryValue(TEXT("Licensekey0"), &dwType, szkeybuff, &dwBuffSize);
				if (strlen(szkeybuff) > 0)
				{
					nLCCheckType = LCCHECKTYPE_REG;
					nRet = LCERR_SUCCESS;
					name.key4 = szkeybuff;
				}	
			}
			else if (lRet != 2)
			{
				nRet = LCERR_REGOPEN;
			}
		}
		
		if (nRet == LCERR_SUCCESS && name.key4.size() == 0)
		{
			nRet = LCERR_KEYNOTFOUND;
		}

		if (nRet < 0)
		{
			return nRet;
		}
		//
		if (!UnInitZQ(name))
		{
			nRet = LCERR_KEYEXTRACT;
			return nRet;
		}


		//check last open file;
		BOOL bHaveCheckFile = TRUE;
		char szMyDir[512];
		string szDateFile;
		__int64 tmLastCheck = 0;
		__int64 tmExpire = 0;
		__int64 tmChangedExpire = 0;
		const int nReadNeed = sizeof(__int64);
		char szLastCheckGenKey[GENLENGTH*2];
		memset(szLastCheckGenKey, 0, sizeof(szLastCheckGenKey));
		if(::SHGetSpecialFolderPath(NULL,szMyDir,CSIDL_COMMON_APPDATA,0))
		{
			szDateFile = szMyDir;
			szDateFile +=TEXT("\\ZQShared\\");
			CreateDirectory(szDateFile.c_str(), NULL);
			if (nLCCheckType == LCCHECKTYPE_REG)
			{
				szDateFile += TEXT("shared.cld");
			}
			else
			{
				szDateFile += szProcName;
				szDateFile += TEXT("shared.cld");
			}
			if (_access(szDateFile.c_str(), 04) != -1)
			{
				ifsDt.open(Ansi2W(szDateFile).c_str(), ios_base::in);
				if (ifsDt.is_open())
				{	
					ifsDt.seekg(0, ios_base::end );
					int nDtFileLen = ifsDt.tellg();
					if (nDtFileLen > 28)
					{
						ifsDt.seekg(0, ios_base::beg);
						while(TRUE)
						{
							int nRealLen = 0;
							ifsDt.read((char*)&nRealLen, 4);
							if (nRealLen != nDtFileLen)
							{
								bHaveCheckFile = FALSE;
								break;
							}
							ifsDt.read((char*)&tmLastCheck, 8);
							ifsDt.read((char*)&tmExpire, 8);
							ifsDt.read((char*)&tmChangedExpire, 8);
							ifsDt.read(szLastCheckGenKey, sizeof(szLastCheckGenKey));
							if (name.key4.compare(szLastCheckGenKey) != 0)
							{
								//gen key changed
								bHaveCheckFile = FALSE;
							}
							break;
						}

						out.nChangedDate = tmChangedExpire;
						out.nLastCheckTime = tmLastCheck;
					}
					else
					{
						bHaveCheckFile = FALSE;
					}
					ifsDt.close();
				}
				else
				{
					nRet = LCERR_DATEFILEOPEN;
				}
			}
			else
			{
				bHaveCheckFile = FALSE;
			}
		}
		else
		{
			nRet = LCERR_SYSDATAPATH;
		}

		if (nRet < 0)
		{
			return nRet;
		}

		//check key1

#ifndef ZQLPT_SKIPKEY1
		std::string szRegKey = GenRegCode();
		if (szRegKey.size() == 0)
		{
			nRet = LCERR_GETMAC;
			return nRet;
		}
		

		if (szRegKey.compare(name.key1) != 0)
		{
			nRet = LCERR_UNMATCHKEY1;
			return nRet;
		}

#endif

		//check key2
		size_t  itPosKey2 = name.key2.find(szProcName);
		if (itPosKey2 == string::npos)
		{
			nRet = LCERR_UNMATCHKEY2;
			return nRet;
		}

		//check key3
		int nExpireY = 0;
		int nExpireM = 0;
		int nExpireD = 0;
		sscanf_s(name.key3.c_str(), TEXT("%4d-%2d-%2d"), &nExpireY, &nExpireM, &nExpireD);
		if (nExpireY < 1900 || nExpireM < 0 || nExpireM > 12 || nExpireD < 0 || nExpireD > 31)
		{
			nRet = LCERR_UNMATCHKEY3_FMT;
		}
		CTime otmNow(time(NULL));
		CTime otmKey2(nExpireY, nExpireM, nExpireD, 0,0,0);
		CTime otmRealKey2(otmKey2);
		CTimeSpan tmSpanRemain(0,0,0,0);
		if (otmRealKey2 < otmNow)
		{
			nRet = LCERR_UNMATCHKEY3_EXPIRED;
		}
		else
		{
			if (bHaveCheckFile)
			{
				if (tmChangedExpire != 0 && otmKey2 > tmChangedExpire)
				{
					otmRealKey2 = CTime(tmChangedExpire);
				}

				if (otmRealKey2 < otmNow)
				{
					nRet = LCERR_UNMATCHKEY3_CHANGEDEXPIRED;
				}
				else
				{
					CTime otmLastCheck(tmLastCheck);
					CTimeSpan otmSpan = otmLastCheck - otmNow;
					if (otmSpan.GetTotalHours() > 24)
					{
						//date have been changed
						nRet = LCERR_UNMATCHKEY3_TIMEBACKWARD;
						otmRealKey2 -=otmSpan;
					}

					if (otmRealKey2 < otmNow)
					{
						nRet = LCERR_UNMATCHKEY3_CHANGEDNOWEXPIRED;
					}

				}
			}
		}

		if (nRet == LCERR_SUCCESS)
		{
			tmSpanRemain = otmRealKey2 - otmNow;
			if (tmSpanRemain.GetTotalHours() < 720)
			{
				nRet = LCERR_LICENSEINDELAYUSEMODE;
			}
		}

		if (!bWriteBack)
		{
			return nRet;
		}
		//save check point
		ofile.open(szDateFile.c_str(), ios_base::trunc|ios_base::out);
		if (ofile.is_open())
		{
			//file len
			int nfileLen = 28 + name.key4.size();
			ofile.write((char*)&nfileLen, 4);
			//check time
			__int64 tmTmp = otmNow.GetTime();
			ofile.write((char*)&tmTmp, 8);
			//expire time
			tmTmp = otmKey2.GetTime();
			ofile.write((char*)&tmTmp, 8);
			//changed expire tm
			tmTmp = 0;
			CTimeSpan tmSpan2 = otmKey2 - otmRealKey2;
			if (tmSpan2.GetTotalHours() > 0)
			{
				tmTmp = otmRealKey2.GetTime();
			}
			ofile.write((char*)&tmTmp, 8);
			//key
			ofile.write(name.key4.c_str(), name.key4.size());
			ofile.flush();
			ofile.close();
		}
		else
		{
			nRet = LCERR_FILEOPEN;
			return nRet;
		}

	}
	catch (...)
	{
		if (ifs.is_open())
		{
			ifs.close();
		}
		if (ofile.is_open())
		{
			ofile.close();
		}
		if (ifsDt.is_open())
		{
			ifsDt.close();
		}
		nRet = -LCERR_CRASHED;
	}
	return nRet;
}

ZQLICENSEPROTECT_API int __cdecl GenZQReqCode()
{
	char szbuff[512];
	GetModuleFileName(NULL, szbuff, 512);
	string wsPath = szbuff;
	size_t nPos = wsPath.rfind('\\');
	string szFile = wsPath.substr(0, nPos);
	szFile +=TEXT("\\applyCode.dat");
	if (_access(szFile.c_str(), 00) == 0)
	{
		//file alreay existed.
		return LCERR_REQFILEALREADYEXIST;
	}

	int nRet = LCERR_SUCCESS;
	ST_NAME name;
	if (InitReg(name))
	{
		
		ofstream ofile; 
		ofile.open(Ansi2W(szFile).c_str(), ios_base::trunc|ios_base::out);
		if (ofile.is_open())
		{
			//file len
			int nfileLen = 28 + name.key4.size();
			ofile.write(name.key4.c_str(), name.key4.size());
			ofile.flush();
			ofile.close();
		}
		else
		{
			nRet = LCERR_REQFILEWRITEERR;
		}
	}
	else
	{
		nRet = LCERR_REQCODEGENERR;
	}

	return nRet;
}
