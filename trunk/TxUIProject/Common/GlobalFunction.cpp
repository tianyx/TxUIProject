#include "stdafx.h"
#include "GlobalFunction.h"

CString GenerateGUID()
{
	BOOL		bSucess   =  TRUE;
	CString		strFormat =  CString("%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"); 

	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);

	CString strGUID;
	if (guid == GUID_NULL)
	{
		bSucess = FALSE;
		strGUID.Empty();
	}

	if(bSucess)
	{
		strGUID.Format(strFormat,
			// first copy...
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
			// second copy...
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	}

	return strGUID;
}