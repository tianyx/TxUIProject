// TestWorkString.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TestWorkString.h"
#include "..\..\Plugins\Interface\IEMBBaseInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace EMB;

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CTestWorkStringApp

BEGIN_MESSAGE_MAP(CTestWorkStringApp, CWinApp)
	//{{AFX_MSG_MAP(CTestWorkStringApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestWorkStringApp construction

CTestWorkStringApp::CTestWorkStringApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestWorkStringApp object

CTestWorkStringApp theApp;

class CTestWorkString:public ITaskWorkerCallInterface
{
public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback)
	{
		return S_OK;
	}
	virtual HRESULT CancelTask(){return S_OK;}
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo)
	{
		CString strTmp = TEXT("1123");
		szInfo = strTmp;
		return S_OK;
	}
};

extern "C"  int __declspec(dllexport) TestWorkString( LPVOID& pInterface )
{
	pInterface = (LPVOID) new CTestWorkString;
	return 0;
}
