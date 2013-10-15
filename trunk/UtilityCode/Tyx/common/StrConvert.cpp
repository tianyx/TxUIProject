#include "StdAfx.h"
#include "StrConvert.h"
#include <string>

LONG String2Wstring( std::string& UTF8String, std::wstring& UTF16String )
{
	if (UTF8String.length() == 0)
	{
		UTF16String.clear();
		return 0;
	}
	UTF16String.resize(UTF8String.length());
	std::copy(UTF8String.begin(), UTF8String.end(), UTF16String.begin());
	return UTF16String.length();
}

LONG Wstring2String( std::wstring& wstrIn, std::string& strOut )
{
	int nLen = wstrIn.length() *2;
	if (nLen == 0)
	{
		strOut.clear();
		return 0;
	}
	char* pszTmp = new char[nLen];
	ZeroMemory(pszTmp, sizeof(wstrIn.length() *2));
	wcstombs(pszTmp, wstrIn.c_str(), nLen -1);
	strOut = pszTmp;
	delete[] pszTmp;
	return strOut.length();
}

LONG Char2Wchar( const char* pszChar, wchar_t* pwszChar, int nwSize)
{
	ZeroMemory(pwszChar, nwSize);
	LONG nLen = MultiByteToWideChar( CP_ACP,0,pszChar,strlen(pszChar),pwszChar, nwSize);
	return nLen;
}

LONG Wchar2Char( const wchar_t* pwszChar, char* pszChar, const int nSize )
{
	ZeroMemory(pszChar, nSize);
	LONG nLen = WideCharToMultiByte( CP_ACP,0,pwszChar, wcslen(pwszChar),pszChar,nSize,NULL,NULL );
	return nLen;
}

LONG IMGetSubString(const char* pszChar, int nSubCharIn, char* pszChar2, int nChar2Size)
{
	LONG OutLen = 0;

	long nLen = strlen(pszChar);
	if (nLen <= nSubCharIn)
	{
		if (nChar2Size > nLen)
		{
			StrCpy(pszChar2, pszChar);
			OutLen = nLen;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else 
	{
		int nwLen = nLen*2+1;
		wchar_t* pwchar = new wchar_t[nwLen];
		Char2Wchar(pszChar, pwchar, nwLen);
		wchar_t* pwIdx = pwchar;
		pwIdx += nSubCharIn;
		*pwIdx = L'\0';
		OutLen = Wchar2Char(pwchar, pszChar2, nChar2Size);
		delete pwchar;
	}
	return OutLen;

}
