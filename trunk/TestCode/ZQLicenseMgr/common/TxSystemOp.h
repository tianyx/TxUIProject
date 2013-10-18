/********************************************************************
	created:	2013/05/22
	created:	2013:5
	filename: 	TxSystemOp.h
	file base:	TxSystemOp
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include <string>
#include <vector>

#define GENLENGTH 500
#define REGLENGTH 126

std::string HDGetMac();
std::string HDGetCPU();
std::string GenRegCode();
std::string GetProductName();

//BOOL GetLocalMacAddr(CString& strMacOut);
struct ST_NAME
{
	std::string key1;//class
	std::string key2;//mac
	std::string key3; //date
	std::string key4;
};

BOOL SplitteStrings(const char* szIn, std::vector<std::string>& vOut);

BOOL InitReg(ST_NAME& name);
BOOL UninitReg(ST_NAME& name);

BOOL InitZQ(ST_NAME& name);
BOOL UnInitZQ(ST_NAME& name);