/********************************************************************
	created:	2013/05/22
	created:	2013:5
	filename: 	ZQGlobal.h
	file base:	ZQGlobal
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#define  MSG_LOGWRITE WM_USER + 1222
#include <string>
#include <vector>
extern HWND g_hwndLog;

using namespace std;
struct ST_GLOBAL
{
	std::string szAppPath;
	std::string szIniPath;
	std::string szCfgSrcPath;
	std::vector<std::string> vAddress;
	int nMaxHp;
	int nMinHp;
	int nDetaHp;
	int nMaxMp;
	int nMinMp;
	int nDetaMp;
	int nAgeMin;
	int nAgeMax;
	long nKeyColAge;
	long nKeyColAddr;
	long nKeyColFill;
	ST_GLOBAL()
	{
		nMaxHp = 110;
		nMinHp = 150;
		nMaxMp = 60;
		nMinMp = 94;
		nKeyColAge = 0;
		nKeyColAddr = 1;
		nKeyColFill = 3;
		nAgeMin = 35;
		nAgeMax = 190;
		nDetaHp = nMaxHp - nMinHp;
		nDetaMp = nMaxMp - nMinMp;
	}

};

extern ST_GLOBAL g_GlobalInfo;

void CFWriteLog( LPCTSTR format,...);
void CFWriteLog2( LPCTSTR format, va_list& ap );

std::string GetAppPath();

BOOL SplitteStrings( const char* szIn , std::vector<string>& vOut);
int GenRand();