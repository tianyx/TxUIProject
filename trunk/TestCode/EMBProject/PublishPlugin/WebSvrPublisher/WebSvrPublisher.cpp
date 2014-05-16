// WorkPluginTransFile.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "PublishPluginBase.h"
#include "EMBDefine.h"
#include "FGlobal.h"
#include ".\PgmlistWebService\ClipServiceSoap.nsmap"
#include ".\PgmlistWebService\soapClipServiceSoapProxy.h"
#include "TxParamString.h"
#include "EmbStructDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// <ResultInfo>
// <ProgramID Value="对应的节目代码"/>
// <FinishedTime Value="2014-05-18 02:23:22"/>
// <Status Value="Success"/> <!--失败填Fails-->
// <Message Value="成功了填空，失败时填写失败原因"/>
// </ResultInfo>

#define STRCLIPRESPFMT TEXT("<ResultInfo> \
<ProgramID Value=\"%s\"/> \
<FinishedTime Value=\"%s\"/> \
<Status Value=\"%s\"/> \
<Message Value=\"%d\"/> \
</ResultInfo> ")

struct ST_GLOBALCFG
{
	CString* pStrSoapEndpoint;
	ST_GLOBALCFG()
	{
		pStrSoapEndpoint = NULL;

	}
	
	~ST_GLOBALCFG()
	{
		if (pStrSoapEndpoint)
		{
			delete pStrSoapEndpoint;
			pStrSoapEndpoint = NULL;
		}

	}
	
	CString GetSoapEndPoint()
	{
		if (pStrSoapEndpoint == NULL)
		{
			pStrSoapEndpoint = new CString;
			LoadConfig();
		}
		
		return *pStrSoapEndpoint;
	}

private:
	BOOL LoadConfig()
	{
		char szPath[MAX_PATH];
		GetModuleFileName(g_hGlobalDllModule, szPath, MAX_PATH);
		CString strThisPath = szPath;
		int nPos = strThisPath.ReverseFind(_T('.'));
		CString strFilexml = strThisPath.Left(nPos);
		strFilexml += TEXT(".xml");
		
		CFile xmlfile;
		CString strParam;
		if (xmlfile.Open(strFilexml, CFile::modeRead, NULL))
		{
			int nFileLen = xmlfile.GetLength();
			char* pBuff = new char[nFileLen +1];
			pBuff[nFileLen] = '\0';
			int nRead = xmlfile.Read(pBuff, nFileLen);
			strParam = pBuff;
			delete[] pBuff;
			pBuff = NULL;
			xmlfile.Close();
			
		}
		else
		{
			ASSERT(FALSE);
			AfxMessageBox("publisher config file err");
			return FALSE;
		}

		if (strParam.IsEmpty())
		{
			return FALSE;
		}

		CTxParamString txParam(strParam);
		txParam.GoIntoKey(TEXT("PublishConfig"));
		if (pStrSoapEndpoint)
		{
			*pStrSoapEndpoint = txParam.GetElemVal(TEXT("soapurl")).GetAsString();
			//CFWriteLog(0, TEXT("soap point changed to =%s"), *pStrSoapEndpoint);
		}

		return TRUE;
	}

};

ST_GLOBALCFG g_globalCfg;




extern "C" int EMBPLUGIN_API GetDesiredPubType()
{
	CString strEndPoint = g_globalCfg.GetSoapEndPoint();
	//CFWriteLog(TEXT("GetDesiredPubType called.. "));
	return embpublishtype_ontaskend;
}

extern "C" int EMBPLUGIN_API OnPublishEmbInfo(const char* szPubInfo, const int nPubType)
{
	//CFWriteLog(TEXT("OnPublishEmbInfo called.. "));
	if (nPubType  != embpublishtype_ontaskend || szPubInfo == NULL)
	{
		return EMBERR_NOTMYTASK;
	}
	
	ST_TASKPUBLISHINFO rpt;
	rpt.FromString(szPubInfo);
	if (rpt.strClipPgmId.IsEmpty())
	{
		//CFWriteLog(0, TEXT("pubreport info error!"));
	}
	CString strEndPoint = g_globalCfg.GetSoapEndPoint();
	string szEndPoint = strEndPoint;
	ClipServiceSoapProxy proxy;
	if (szEndPoint.size() > 0)
	{
		proxy.soap_endpoint = szEndPoint.c_str();
	}

	_CLIPSVR__TransferTaskExcuteResultNotify notify;
	CString strMsg;
	strMsg.Format(STRCLIPRESPFMT, rpt.strClipPgmId, Time2Str(rpt.tReportTime), rpt.nState == embtaskstate_finished? TEXT("Success"):TEXT("Fails"), rpt.nErrcode);
	string szInfo = strMsg;
	notify.ResultXML = &szInfo;
	_CLIPSVR__TransferTaskExcuteResultNotifyResponse rsp;
	if (proxy.TransferTaskExcuteResultNotify(&notify, &rsp) != SOAP_OK)
	{
		//CFWriteLog(0, TEXT("websvr callfailed! soapend=%d"), strEndPoint);
	}

	return S_OK;
}