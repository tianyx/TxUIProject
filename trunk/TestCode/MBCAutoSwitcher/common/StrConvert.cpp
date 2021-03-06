#include "StdAfx.h"
#include "StrConvert.h"
#include <string>



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

wstring Ansi2W( const string& szIn )
{
	wstring wRet;
	if(szIn.length() <= 0)
	{
		return wRet;
	}

	LPCSTR pszSrc = szIn.c_str();
	int nSize = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, 0, 0);

	WCHAR *pwszDst = new WCHAR[nSize+1];
	ASSERT(pwszDst != NULL);
	//ZeroMemory(pwszDst, 2*(nSize+1));
	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, -1, pwszDst, nSize);
	pwszDst[nSize] = L'\0';
	wRet = pwszDst;
	delete pwszDst;
	return wRet;
}

std::string W2Ansi( const wstring& wszIn )
{
	string szRet;
	if (wszIn.length() <= 0) 
	{
		return szRet;
	}
	LPCWSTR pwszSrc = wszIn.c_str();
	int nWLen = wszIn.length();
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	
	char* pszDst = new char[nLen+1];
	ASSERT(NULL != pszDst);
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen] = '\0';
	szRet =pszDst;
	delete [] pszDst;
	return szRet;
}

void LTrim( wstring& wsInOut )
{
	size_t nPos = wsInOut.find_first_not_of(L" \n\r\t");
	if (nPos!= wstring::npos)
	{
		wsInOut = wsInOut.substr(nPos); 
	}
}

void RTrim( wstring& wsInOut )
{
	size_t nPos = wsInOut.find_last_not_of(L" \n\r\t");
	if (nPos!= wstring::npos)
	{
		wsInOut = wsInOut.substr(0, nPos+1); 
	}
}
void Trim( wstring& wsInOut )
{
	RTrim(wsInOut);
	LTrim(wsInOut); 
}

void RTrim( wstring& wsInOut , LPCWSTR lpChars)
{
	if (lpChars == NULL || lpChars[0] == '\0')
	{
		return;
	}
	size_t nPos = wsInOut.find_last_not_of(lpChars);
	if (nPos!= wstring::npos)
	{
		wsInOut.substr(0,nPos); 
	}
}








#ifdef UNICODE
CString ANSI2CStr( const string& szIn )
{
	CString strOut = Ansi2W(szIn).c_str();
	return strOut;
}

std::string CStr2Ansi( CString& strIn )
{
	wstring wszTmp = strIn.GetBuffer();
	return W2Ansi(wszTmp);
}
#else
std::wstring CStr2W(CString& strIn)
{
	string szTmp = strIn.GetBuffer();
	return Ansi2W(szTmp);
}

CString W2CStr( const wstring& wszIn )
{
	CString strOut = W2Ansi(wszIn).c_str();
	return strOut;
}

#endif // UNICODE