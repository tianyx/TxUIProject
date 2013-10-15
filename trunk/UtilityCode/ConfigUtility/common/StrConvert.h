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
using namespace std;
LONG Char2Wchar(const char* pszChar, wchar_t* pwszChar, int nwSize);  //nwSize = sizeof wchar in byte
LONG Wchar2Char(const wchar_t* pwszChar, char* pszChar, int nwSize); //nwSize = sizeof char in byte

wstring Ansi2W(const string& szIn);
string W2Ansi(const wstring& wszIn);

void LTrim(wstring& wsInOut);
void RTrim(wstring& wsInOut);
void Trim(wstring& wsInOut);

void RTrim( wstring& wsInOut , LPCWSTR lpChars);
