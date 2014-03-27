#include "StdAfx.h"
#include "Util.h"
#include "io.h"

CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

bool CUtil::XmlFileAppend( const CString& strFileName, ST_TASKREPORT& tskReport )
{
	if (strFileName.IsEmpty() || tskReport.strGuid.IsEmpty())
	{
		return false;
	}

	bool ok = false;
	CString strV;
	tskReport.ToString(strV);

	// strFileName文件是否存在?
	if (0 == access(strFileName, 0))
	{
		CMarkup xmlMark;
		if (xmlMark.Load(strFileName))
		{
			xmlMark.FindElem(EK_MAIN);
			xmlMark.IntoElem();

			//---------------------------------
			CMarkup subMark(strV);
			subMark.FindChildElem("TaskReport");
			CString strSubNode = subMark.GetChildSubDoc();
			// ------------------------------------

			xmlMark.InsertSubDoc(strSubNode);
			xmlMark.OutOfElem();

			xmlMark.Save(strFileName);
		}
	}
	else
	{
		CMarkup xmlMark(strV);

		if (xmlMark.IsWellFormed())
		{
			ok = xmlMark.Save(strFileName);
		}
	}
	
	return ok;
}

bool CUtil::QueryXmlFile(const CString& strFileName, const CString& strTaskGuid, ST_TASKREPORT& tskInfor )
{
	// 文件不存在
	if (_access(strFileName, 0) == -1)
	{
		return false;
	}

	CMarkup xmlMark;

	if (!xmlMark.Load(strFileName))
	{
		return false;
	}

	// 解析xml信息
	CString strGuid;
	CString xmlContent;

	while(xmlMark.FindChildElem("TaskReport"))
	{
		strGuid = xmlMark.GetChildAttrib("strGuid");

		if (0 == strTaskGuid.CompareNoCase(strGuid))
		{
			xmlContent = xmlMark.GetChildSubDoc();
			break;
		}
	}

	if (xmlContent.IsEmpty())
	{
		return false;
	}

	CString strV;
	strV.Format("<edoc_main guid=\"\" type=\"6\" ver=\"1\">%s</edoc_main>", xmlContent);

	BOOL suc = tskInfor.FromString(strV);

	return suc ? true : false;
}
