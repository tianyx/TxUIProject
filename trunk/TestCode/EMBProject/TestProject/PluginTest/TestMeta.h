#pragma once

#include "../../common/TxParamString.h"
#include "../../Common/Markup.h"
#include "../../Common/EMBTask.h"
#include "../../Plugins/Interface/IEMBBaseInterface.h"
#include "../../Plugins/Interface/EMBGuid.h"


#include "gtest/gtest.h"
using namespace testing;
using namespace EMB;

#ifdef _DEBUG
#pragma  comment(lib, "gtestd.lib")
#else
#pragma  comment(lib, "gtest.lib")
#endif

class CTestMeta : public ITaskReportToExcutorInterface
{
public:
	CTestMeta(void);
	~CTestMeta(void);

	void PluginTest();

	virtual HRESULT OnDllReportTaskProgress( const CTaskString& szInfo );

};
