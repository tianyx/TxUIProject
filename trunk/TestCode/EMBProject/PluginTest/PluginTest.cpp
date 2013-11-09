// PluginTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestMeta.h"

int _tmain(int argc, _TCHAR* argv[])
{
	InitGoogleTest(&argc, argv);

	//CTestMeta metaTest;
	//metaTest.PluginTest();

	RUN_ALL_TESTS();
	
    // wait: 输入 quit 退出
	char cmd[20];
	while (1)
	{
		memset(cmd, 0, sizeof(cmd));
		scanf("%s", cmd);

		if (0 == strcmp("quit", cmd))
		{
			break;
		}
	}

	return 0;
}

