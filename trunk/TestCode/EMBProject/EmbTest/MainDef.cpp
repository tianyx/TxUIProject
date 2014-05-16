#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "EMBCommonFunc.h"
#include "TxAutoComPtr.h"
#include "IEMBBaseInterface.h"
#include "TxParamString.h"
using namespace EMB;

ST_GLOBAL g_GlobalInfo;
HMODULE g_hModulePluginMgr = NULL;
EMB::IPluginBaseInterface* g_pIPluginMgr = NULL;

HMODULE g_hActorPlugin  = NULL;
EMB::IPluginBaseInterface* g_pIActorPlugin = NULL;

extern int DBKEY_EMB = 150;

BOOL InitGlobalConfig()
{
		
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EmbTest.xml");

	CFile file;
	CString strXml;
	if (file.Open(g_GlobalInfo.szIniPath, CFile::modeRead, NULL))
	{
		int nFileLen = file.GetLength();
		if (nFileLen == 0)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		char* pbuff = new char[nFileLen+1];
		ZeroMemory(pbuff, nFileLen +1);
		int nSize = file.Read(pbuff, nFileLen);
		ASSERT(nSize == nFileLen);
		strXml = pbuff;
		file.Close();
		delete[] pbuff;

		CTxParamString txParam(strXml);
		txParam.GoIntoKey(TEXT("EmbTest"));
		g_GlobalInfo.szDBConn = txParam.GetElemVal(TEXT("dbconn")).GetAsString(TEXT("Provider=SQLOLEDB; Server=192.168.1.125; Database=zjtv; uid=sa; pwd=123"));
	}

	return TRUE;
}
