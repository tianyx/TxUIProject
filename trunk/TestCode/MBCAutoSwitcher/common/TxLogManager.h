#pragma once

#include <map>
#include "Log.h"
#include "AutoCritSec.h"
#include <cliext/queue>
using namespace std;
using namespace cliext;

#define LOGKEYBASE 0
#define LOGKEYMAIN LOGKEYBASE+1
//.......


struct ST_LOGDATA
{
	DWORD dwLogKey;
	CString strName;
	Log log;
};
struct ST_LOGQUEUEDATA
{
	DWORD dwLogKey;
	CString strData;
	
};

typedef map<DWORD, ST_LOGDATA> MAPLOGS;
typedef queue<ST_LOGQUEUEDATA> QUEUELOGDATA;
class CTxLogManager
{
public:
	CTxLogManager(void);
	~CTxLogManager(void);

	CAutoCritSec m_lock;


};
