// WorkPluginMediacheckV6.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "EMBPluginBase.h"
#include "MediaCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
extern EMB::CMediaCheck* g_pPluginInstane = NULL;


static AFX_EXTENSION_MODULE WorkPluginMediacheckV6DLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("WORKPLUGINMEDIACHECKV6.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(WorkPluginMediacheckV6DLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(WorkPluginMediacheckV6DLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("WORKPLUGINMEDIACHECKV6.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(WorkPluginMediacheckV6DLL);
	}
	return 1;   // ok
}



extern "C" int EMBPLUGIN_API GetPluginInstanceVC6( LPVOID& pInterface )
{
	if (!g_pPluginInstane)
	{
		g_pPluginInstane = new EMB::CMediaCheck;
		ASSERT(g_pPluginInstane);
	}
	g_pPluginInstane->QueryInterface(GuidEMBPlugin_IBaseVC6, pInterface);
	return 0;
}

