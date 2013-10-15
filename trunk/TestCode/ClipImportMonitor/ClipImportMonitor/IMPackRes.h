/********************************************************************
	created:	2010/02/23
	created:	23:2:2010   17:49
	filename: 	IMPackRes.h
	file path:	Common
	file base:	IMPackRes
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include <vector>

#define IMPACKEFILENUM  3
#define IDR_IMRESDATA 130     //the id of impackdata.bin;

enum SPLITPAR
{
	DRIVE	= 0x0001,
	PATH	= 0x0002,
	FNAME	= 0x0004,
	EXT		= 0x0008
};

typedef std::pair<CString, CString> RESPAIR;
typedef std::vector<RESPAIR> RESDATA;

BOOL GetPackFileList(RESDATA& dataOut);

BOOL ExtractRes();
void PackRes();