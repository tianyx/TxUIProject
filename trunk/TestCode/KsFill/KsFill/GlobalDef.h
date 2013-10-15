#pragma once

#include <string>
#include <vector>
struct ST_GLOBAL
{
	std::string szAppPath;
	std::string szIniPath;
	std::string szCfgSrcPath;
	std::vector<std::string> vAddress;

	ST_GLOBAL()
	{

	}

};
extern ST_GLOBAL g_GlobalInfo;