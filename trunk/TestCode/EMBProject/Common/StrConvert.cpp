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

void LTrim( txstring& wsInOut )
{
	size_t nPos = wsInOut.find_first_not_of(TEXT(" \n\r\t"));
	if (nPos!= txstring::npos)
	{
		wsInOut = wsInOut.substr(nPos); 
	}
}

void RTrim( txstring& wsInOut )
{
	size_t nPos = wsInOut.find_last_not_of(TEXT(" \n\r\t"));
	if (nPos!= txstring::npos)
	{
		wsInOut = wsInOut.substr(0, nPos+1); 
	}
}
void Trim( txstring& wsInOut )
{
	RTrim(wsInOut);
	LTrim(wsInOut); 
}

void RTrim( txstring& wsInOut , TCHAR* lpChars)
{
	if (lpChars == NULL)
	{
		return;
	}
	size_t nPos = wsInOut.find_last_not_of(lpChars);
	if (nPos!= txstring::npos)
	{
		wsInOut = wsInOut.substr(0,nPos); 
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



BOOL SplitteStrings( const char* szIn , vector<string>& vOut, char chSeperator/* = ','*/)
{
	string szData = szIn;
	if (szData.size() == 0)
	{
		return FALSE;
	}

	size_t nBegin = 0;
	for (size_t i = 0; i < szData.size();++i)
	{
		if (szData[i] == chSeperator)
		{
			if (i > nBegin)
			{
				std::string tmp = szData.substr(nBegin, i - nBegin);
				Trim(tmp);
				if (tmp.size() != 0)
				{
					vOut.push_back(tmp);
				}
			}
			nBegin = i+1;
		}		
	}

	if (nBegin < szData.size())
	{
		vOut.push_back(szData.substr(nBegin, szData.size() - nBegin));
	}
	return TRUE;
}






int CTxStrConvert::GetAsInt( int nDefault /*= 0*/ )
{
	if (m_szParam.IsEmpty())
	{
		return nDefault;
	}

	int nRet  = nDefault;
	int nSuc = sscanf_s(m_szParam, TEXT("%d"), &nRet);
	return nSuc> 0? nRet:nDefault;
}


int CTxStrConvert::GetAsUInt( unsigned int nDefault /*= 0*/ )
{
	if (m_szParam.IsEmpty())
	{
		return nDefault;
	}

	int nRet  = nDefault;
	int nSuc = sscanf_s(m_szParam, TEXT("%u"), &nRet);
	return nSuc> 0? nRet:nDefault;
}

INT64 CTxStrConvert::GetAsInt64( INT64 nDefault /*= 0*/ )
{
	if (m_szParam.IsEmpty())
	{
		return nDefault;
	}

	INT64 nRet = nDefault;
	int nSuc = sscanf_s(m_szParam, TEXT("%I64d"), &nRet);
	return nSuc> 0? nRet:nDefault;nDefault;
}

bool CTxStrConvert::GetAsBOOL( bool bDefault /*= FALSE*/ )
{
	if (m_szParam.IsEmpty())
	{
		return bDefault;
	}

	CString strTmp(m_szParam);
	strTmp.MakeLower();
	BOOL bFind = FALSE;
	int nRet = 0;
	if (strTmp.Compare("true") == 0 )
	{
		bFind = TRUE;
		nRet = 1;
	}
	else if (strTmp.Compare("false") == 0)
	{
		bFind = TRUE;
		nRet = 0;
	}

	if (bFind)
	{
		return nRet != 0;
	}
	else
	{
		int nRet  = 0;
		int nSuc = sscanf_s(m_szParam, TEXT("%d"), &nRet);
		return nSuc> 0? (nRet != 0):bDefault;
	}
}

CString CTxStrConvert::GetAsString( LPCTSTR szDefault /*= "" */ )
{
	if (m_szParam.IsEmpty())
	{
		return CString(szDefault);
	}
	else
	{
		return m_szParam;
	}

}


BOOL CTxStrConvert::GetAsStringArray( VECSTRINGS& vOut, TCHAR separator /*= ','*/ )
{
	if (m_szParam.IsEmpty())
	{
		return FALSE;
	}

	std::vector<std::string> vecstr;
	SplitteStrings(m_szParam, vecstr, separator);
	for (size_t i = 0; i < vecstr.size(); ++i)
	{
		vOut.push_back(CString(vecstr[i].c_str()));
	}
	return TRUE;
}

void CTxStrConvert::SetVal( unsigned int nVal )
{
	m_szParam.Format(TEXT("%u"), nVal);
}

void CTxStrConvert::SetVal( int nVal )
{
	m_szParam.Format(TEXT("%d"), nVal);
}

void CTxStrConvert::SetVal( INT64 nVal )
{
	m_szParam.Format(TEXT("%I64d"), nVal);
}

void CTxStrConvert::SetVal( bool bVal )
{
	m_szParam.Format(TEXT("%d"), bVal? 1:0);
}

void CTxStrConvert::SetVal( LPCTSTR szVal )
{
	m_szParam = szVal;
}

void CTxStrConvert::SetVal( VECSTRINGS& vVal )
{
	m_szParam.Empty();
	for (size_t i = 0; i < vVal.size(); ++i)
	{
		if (i != 0)
		{
			m_szParam += TEXT(",");
		}
		m_szParam += vVal[i];
	}
}

