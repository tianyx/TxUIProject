// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__48D0D8B3_02EC_4E37_A5D6_FA9DE559B936__INCLUDED_)
#define AFX_STDAFX_H__48D0D8B3_02EC_4E37_A5D6_FA9DE559B936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//#include <windows.h>

// TODO: reference additional headers your program requires here
#include <afx.h>
#include <afxwin.h>
//directshow
#include "dshow.h"
#include "streams.h"
#include <Qedit.h>
#include <dmodshow.h>
#include <DMO.h>

#include <atlbase.h>
#include <AtlConv.h>
#include "Common.h"


#pragma comment(lib, "Quartz.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "uuid.lib")


#include "../MainConcept/include/ModuleConfig/ModuleConfig.h"
//#include "ModuleConfig/common_mc.h"
#include "../MainConcept/include/ModuleConfig/mpeg2dec_mc.h"


//#include <objbase.h>
//#include <initguid.h>
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__48D0D8B3_02EC_4E37_A5D6_FA9DE559B936__INCLUDED_)
