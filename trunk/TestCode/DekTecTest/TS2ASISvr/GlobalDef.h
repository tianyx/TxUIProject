#pragma once
#include <string>
#include <vector>
struct ST_TSADDR
{
	std::string ip;
	int nPort;
};

struct ST_GLOBAL
{
	std::string szAppPath;
	std::string szIniPath;
	std::string szCfgSrcPath;
	std::vector<ST_TSADDR> vtsAddr;
};

extern ST_GLOBAL g_GlobalInfo;