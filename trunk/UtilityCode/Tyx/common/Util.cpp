#include "StdAfx.h"
#include "Util.h"

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

bool CUtil::SaveXmlFile( const CString& strFileName, const CString& strXmlContent )
{
	if (strFileName.IsEmpty() || strXmlContent.IsEmpty())
	{
		return false;
	}

	bool ok = false;
	CMarkup xmlMark(strXmlContent);

	if (xmlMark.IsWellFormed())
	{
		ok = xmlMark.Save(strFileName);
	}

	return ok;
}
