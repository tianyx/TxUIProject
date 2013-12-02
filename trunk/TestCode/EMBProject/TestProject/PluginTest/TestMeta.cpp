#include "StdAfx.h"
#include "TestMeta.h"

// {AE65F659-C982-40da-B09C-AAB70A48990D}
static const GUID GuidEMBPlugin_ITaskWorkerCall = 
{ 0xae65f659, 0xc982, 0x40da, { 0xb0, 0x9c, 0xaa, 0xb7, 0xa, 0x48, 0x99, 0xd } };

TEST(MetaTest, CEMBTaskRequestXml)
{
	CEMBTaskRequest tsk;
	tsk.m_request.m_MsgID = "msg0101";
	tsk.m_taskInfo.m_strTaskID = "taskid";

	CString strXml = tsk.ToXml();

	CEMBTaskRequest meta;
	EXPECT_EQ(TRUE, meta.ToMeta(strXml, ".\\EMBTaskRequest"));
	EXPECT_EQ(tsk.m_request.m_MsgID, meta.m_request.m_MsgID);

	EXPECT_EQ(FALSE, meta.ToMeta("", ".\\EMBTaskRequest"));
}

TEST(MetaTest, TransferXml)
{
	CTransferTaskRequest tsk;

	tsk.m_strTaskID = "0103";
	tsk.m_SourceFile.m_strType = "ftp";
	tsk.m_SourceFile.m_strFileName = "test.mpg";
	tsk.m_SourceFile.m_strFilePath = "";
	tsk.m_SourceFile.m_strIP = "192.168.1.207";
	tsk.m_SourceFile.m_strPort = "21";
	tsk.m_SourceFile.m_strUser = "sa";
	tsk.m_SourceFile.m_strPwd = "123";

	tsk.m_DestFile.m_strType = "ftp";
	tsk.m_DestFile.m_strFileName = "0102.mpg";
	tsk.m_DestFile.m_strFilePath = "";
	tsk.m_DestFile.m_strIP = "192.168.1.207";
	tsk.m_DestFile.m_strPort = "21";
	tsk.m_DestFile.m_strUser = "sa02";
	tsk.m_DestFile.m_strPwd = "123";

	CString strXml = tsk.ToXml();

	CTransferTaskRequest meta;
	EXPECT_EQ(TRUE, meta.ToMeta(strXml, ".\\TransferTaskRequest"));
	EXPECT_EQ(tsk.m_strTaskID, meta.m_strTaskID);
	EXPECT_EQ("test.mpg", meta.m_SourceFile.m_strFileName);
	EXPECT_EQ("0102.mpg", meta.m_DestFile.m_strFileName);
}

CTestMeta::CTestMeta(void)
{
}

CTestMeta::~CTestMeta(void)
{
}

int OnCallBack(LPSTR pOutXml)
{
	printf("-----oncallback\n");
	printf("%s\n", pOutXml);

	return 1;
}

void CTestMeta::PluginTest()
{
	IPluginBaseInterfaceVC6* pLogic = NULL;
	ITaskWorkerCallInterface* pWorker = NULL;
	HMODULE hM = ::LoadLibrary(_T("TransferPlugin.dll"));
	if (NULL != hM)
	{
		typedef int (*fnILogicFunc)( LPVOID& );
		fnILogicFunc pFunc = (fnILogicFunc)GetProcAddress(hM, "GetPluginInstanceVC6");

		if (NULL != pFunc)
		{
			if (pFunc((LPVOID&)pLogic) == S_OK)
			{
				pLogic->QueryInterface(GuidEMBPlugin_ITaskWorkerCall, (LPVOID&)pWorker);
			}

			if (NULL != pWorker)
			{
				CMarkup xmlMark;
				xmlMark.AddElem("TransferTaskRequestType");
				xmlMark.AddAttrib("TaskId", "0102");
				xmlMark.IntoElem();

				// SourceFile 
				xmlMark.AddElem("SourceFile", "源文件");
				//Type, FilePath, FileName, IP, User, Pwd, Port
				xmlMark.AddAttrib("Type", "disk");
				xmlMark.AddAttrib("FilePath", "C:\\workdir");
				xmlMark.AddAttrib("FileName", "streamDB.bak");
				xmlMark.AddAttrib("IP", "192.168.1.207");
				xmlMark.AddAttrib("User", "sa");
				xmlMark.AddAttrib("Pwd", "123");
				xmlMark.AddAttrib("Port", "21");

				// DestFile 
				xmlMark.AddElem("DestFile", "目标文件");
				//Type, FilePath, FileName, IP, User, Pwd, Port
				xmlMark.AddAttrib("Type", "ftp");
				xmlMark.AddAttrib("FilePath", "");
				xmlMark.AddAttrib("FileName", "streamDB.bak");
				xmlMark.AddAttrib("IP", "192.168.1.207");
				xmlMark.AddAttrib("User", "sa02");
				xmlMark.AddAttrib("Pwd", "123");
				xmlMark.AddAttrib("Port", "21");

				xmlMark.OutOfElem();

				CString strXml = xmlMark.GetDoc();

				CTaskString strRet;
				pWorker->DoTask(strXml, strRet, this);
			}
		}
	}
}

HRESULT CTestMeta::OnDllReportTaskProgress( const CTaskString& szInfo )
{
	printf("%s\n", szInfo);
	return S_OK;
}
