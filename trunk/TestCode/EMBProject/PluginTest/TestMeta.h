#pragma once
#include "../BLPlugin/TransferPlugin/TransferPluginDll.h"
#include "../common/TxParamString.h"
#include "../Common/Markup.h"
#include "../Common/MetaData/EMBTask.h"

#pragma comment(lib, "MetaData.lib")

#include "gtest/gtest.h"
using namespace testing;

#ifdef _DEBUG
#pragma  comment(lib, "gtestd.lib")
#else
#pragma  comment(lib, "gtest.lib")
#endif

class CTestMeta
{
public:
	CTestMeta(void);
	~CTestMeta(void);

	void PluginTest();
};
