/********************************************************************
	created:	2011/08/10
	created:	10:8:2011   15:26
	filename: 	StrConvert.h
	file path:	
	file base:	StrConvert
	file ext:	h
	author:		tian_yuanxin
	
	purpose:	
*********************************************************************/
#pragma once
#include <string>
#include <vector>
using namespace std;
LONG Char2Wchar(const char* pszChar, wchar_t* pwszChar, int nwSize);  //nwSize = sizeof wchar in byte
LONG Wchar2Char(const wchar_t* pwszChar, char* pszChar, int nwSize); //nwSize = sizeof char in byte

wstring Ansi2W(const string& szIn);
string W2Ansi(const wstring& wszIn);

#ifdef UNICODE
CString ANSI2CStr(const string& szIn);
string CStr2Ansi(CString& strIn);
#else
CString W2CStr(const wstring& wszIn);
wstring CStr2W(CString& strIn);
#endif // UNICODE

#ifdef UNICODE
typedef wstring txstring;
#else
typedef string txstring;
#endif

void LTrim(txstring& wsInOut);
void RTrim(txstring& wsInOut);
void Trim(txstring& wsInOut);

void RTrim( txstring& wsInOut , TCHAR* lpChars);

BOOL SplitteStrings( const char* szIn , std::vector<std::string>& vOut, char chSeperator = ',');


typedef vector<CString> VECSTRINGS;
typedef vector<int> VECINTS;


class CTxStrConvert
{
public:
	CTxStrConvert(){}
	CTxStrConvert(LPCTSTR szParamIn){m_szParam = szParamIn;}
	~CTxStrConvert(){};
	CString m_szParam;
public:
	int GetAsInt(int nDefault = 0);
	int GetAsUInt(unsigned int nDefault = 0);
	INT64 GetAsInt64(INT64 nDefault = 0);
	bool GetAsBOOL(bool bDefault = FALSE);
	CString GetAsString(LPCTSTR szDefault = TEXT(""));
	BOOL GetAsStringArray(VECSTRINGS& vOut, TCHAR separator = ',');
	BOOL GetAsIntArray(VECINTS& vOut, TCHAR separator = ',');

	void SetVal(int nVal);
	void SetVal(unsigned int nVal);
	void SetVal(INT64 nVal);
	void SetVal(bool bVal);
	void SetVal(LPCTSTR szVal);
	void SetVal(VECSTRINGS& vVal);
	void SetVal(VECINTS& vVal);

};