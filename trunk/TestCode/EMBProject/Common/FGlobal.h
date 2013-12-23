#pragma once

#include <vector>
#include <string>
////log output macro//////////////////////////////////////////////////////////////////////
#define  MSG_LOGWRITE WM_USER + 1222

#define MACRO_EDITLOGOUTPUT \
if (message == MSG_LOGWRITE)\
{\
	if (wParam != NULL)\
	{\
		TCHAR* szMsg = (TCHAR*) wParam;\
		m_edtLog.SetSel(-1,-1);\
		CString strMsg = TEXT("\r\n");\
		strMsg += szMsg;\
		m_edtLog.ReplaceSel(strMsg);\
	}\
}\

//////////////////////////////////////////////////////////////////////////
class CTxFontLoader;

extern HWND g_hwndLog;
extern HANDLE g_hconsoleHandle;
extern CTxFontLoader*  g_pFontLoader;

extern int g_nEndBitRateCheckInterval;
extern int 	g_SOCK_INTERVAL_RECONN;
extern int g_INTERVAL_LIVECHECK;
extern int g_DELAY_BEFORELIVECHECK;
extern int g_nDefMBCDrawItemCX;
extern int g_nDefMBCDrawItemCY;
extern int g_nDefMBCDrawObjIntervalCX;
extern int g_nDefMBCDrawObjIntervalCY;
extern int g_nDefMBCOneSendPackSize;

extern int g_nMBCSockOptTTL;

extern int g_nScreenCX;
extern int g_nScreenCY;
extern int g_nScrollBarWidth;


void CFWriteLog( LPCTSTR format,...);
void CFWriteLog(DWORD dwLogKey, LPCTSTR format,...);
void CFWriteLog2(HWND hwndIn,  LPCTSTR format, va_list& ap );
void CFWriteLog2Wnd(HWND hwndIn, LPCTSTR format,...);


HINSTANCE GetSelfModuleHandle();
std::wstring GetAppPathW();
std::string GetAppPath();
SOCKADDR_IN GetAddrFromStr(CString& strAddrIn);

//szWideChars like "dll,txt"
void GetFilesInFolder(CString& strFolder, std::vector<CString>& dataOut, LPCTSTR szWideChars, BOOL bSearchInSubFolder = TRUE);
CString GetFileExten(CString& strFile);


BOOL IsEqualAddr(const SOCKADDR_IN& addr1, const SOCKADDR_IN& addr2);
bool operator ==(const SOCKADDR_IN& addr1, const SOCKADDR_IN& addr2);

std::string Addr2String(const SOCKADDR_IN& addr1);

CString Guid2String(const GUID& guidIn);
GUID String2Guid(CString& strIn);
GUID TxGenGuid();

int GenRand();

#define MUSTBESOK(hr) if(hr != S_OK){ASSERT(FALSE);return hr;}
#define MUSTNOTNULL(ptr) if(ptr == NULL){ASSERT(FALSE);return E_FAIL;}

#define MACRO_CREATEOUTPUTCONSOLE \
g_hconsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);\
if (g_hconsoleHandle == NULL)\
{\
	AllocConsole();\
	g_hconsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);\
}\

#define MACRO_FREEOUTPUTCONSOLE FreeConsole();


struct TXGUID
{
	TXGUID(const GUID& guidIn)
	{
		guid = guidIn;
	}

	TXGUID()
	{
		guid = GUID_NULL;
	}
	GUID guid;
	bool operator <(const TXGUID& other) const
	{
// 		return (guid.Data1 != other.guid.Data1)? (guid.Data1 < other.guid.Data1) :
// 			(guid.Data2 != other.guid.Data2)? (guid.Data2 < other.guid.Data2) :
// 			(guid.Data3 != other.guid.Data3)? (guid.Data3 < other.guid.Data3) :
// 			()?((UINT64)guid.Data4 <(UINT64)other.guid.Data4):false;
		if (guid.Data1 !=other.guid.Data1)
		{
			return guid.Data1 < other.guid.Data1;
		}
		else
		{
			if (guid.Data2 != other.guid.Data2)
			{
				return guid.Data2 < other.guid.Data2;
			}
			else
			{
				if (guid.Data3 != other.guid.Data3)
				{
					return guid.Data3 < other.guid.Data3;
				}
				else
				{
					UINT64& ud4 = (UINT64&)guid.Data4;
					UINT64& ud4Other = (UINT64&)other.guid.Data4;
					if (ud4 < ud4Other)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}
	}

	operator GUID() const {return guid;}
	operator CString() const{return Guid2String(guid);}
	TXGUID& operator =(const GUID& guidIn)
	{
	
		guid = guidIn;
		return *this;
	}
};