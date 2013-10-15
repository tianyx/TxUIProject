/********************************************************************
	created:	2011/08/10
	created:	10:8:2011   15:26
	filename: 	e:\Source\EZIM\Src\EZMessenger\common\StrConvert.h
	file path:	e:\Source\EZIM\Src\EZMessenger\common
	file base:	StrConvert
	file ext:	h
	author:		tian_yuanxin
	
	purpose:	
*********************************************************************/
#pragma once
#include <string>

LONG String2Wstring(std::string& strIn, std::wstring& wstrOut); 
LONG Wstring2String(std::wstring& wstrIn, std::string& strOut);

LONG Char2Wchar(const char* pszChar, wchar_t* pwszChar, int nwSize);  //nwSize = sizeof wchar in byte
LONG Wchar2Char(const wchar_t* pwszChar, char* pszChar, int nwSize); //nwSize = sizeof char in byte
//nSubchar is the character size you what to get
//make sure the nChar2Size > 2* nSubCharIn
LONG IMGetSubString(const char* pszChar, int nSubCharIn, char* pszChar2, int nChar2Size);
