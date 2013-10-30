#include "StdAfx.h"
#include "TxParamString.h"
#include "Markup.h"
#define TXMACRO_CHECKPARAMINIT \
	if (!m_bInited){ASSERT(FALSE);default;} \

//////////////////////////////////////////////////////////////////////////
BOOL FindMarkItem(CMarkup& markDoc, ST_MARKITEM& kvOut)
{
	while(markDoc.FindElem())
	{
		//tree node, set val to empty
		kvOut.strVal.Empty();
		kvOut.bIsPath = TRUE;
		ST_MARKITEM tmpItem;
		CString strAttKey;
		CString strAttVal;
		int nAttIdx = 0;
		while(markDoc.GetNthAttrib(nAttIdx, strAttKey, strAttVal))
		{
			tmpItem.mapAttrib[strAttKey] = strAttVal;
			++nAttIdx;
		}

		tmpItem.strKey = markDoc.GetTagName();
		tmpItem.strVal = markDoc.GetElemContent();
		markDoc.IntoElem();
		FindMarkItem(markDoc, tmpItem);
		markDoc.OutOfElem();

		kvOut.mapChildItem[tmpItem.strKey] = tmpItem;
	}

	return TRUE;
}

BOOL WriteMarkupString(CMarkup& markDoc, ST_MARKITEM& itemIn)
{	
	if (!itemIn.strKey.IsEmpty() && itemIn.strKey.Compare(TEXT(".")) != 0)
	{
		markDoc.AddElem(itemIn.strKey, itemIn.strVal);		
	}
	MAPKEYATTRIB ::iterator itb = itemIn.mapAttrib.begin();
	MAPKEYATTRIB ::iterator ite = itemIn.mapAttrib.end();
	for (; itb != ite; ++itb)
	{
		markDoc.AddAttrib(itb->first, itb->second);
	}

	MAPKEYVALUE::iterator itkvb = itemIn.mapChildItem.begin();
	MAPKEYVALUE::iterator itkve = itemIn.mapChildItem.end();
	for (; itkvb != itkve; ++itkvb)
	{
		markDoc.IntoElem();
		WriteMarkupString(markDoc, itkvb->second);
		markDoc.OutOfElem();
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

CTxParamString::CTxParamString(LPCTSTR szParamIn)
{
	m_strRealString = szParamIn;
	m_bInited = FALSE;
	InitLayout();
}
CTxParamString::CTxParamString(const ST_MARKITEM& kvDataIn)
{
	m_kvData = kvDataIn;
	m_strRealString = "";
	m_bInited = TRUE;
}
CTxParamString::CTxParamString( const CTxParamString& other )
{
	if (&other == this)
	{
		return;
	}
	Clear();
	m_strRealString = other.m_strRealString;
	m_bInited = FALSE;
	InitLayout();
}

CTxParamString::~CTxParamString(void)
{
	Clear();
}

CTxParamString& CTxParamString::operator=( const CTxParamString& other )
{
	if (&other == this)
	{
		return *this;
	}

	this->Clear();
	this->m_strRealString = other.m_strRealString;
	this->InitLayout();
	return *this;
}

bool CTxParamString::operator<( const CTxParamString& other )
{
	if (&other == this)
	{
		return false;
	}
	return m_strRealString < other.m_strRealString;
}

bool CTxParamString::operator==( const CTxParamString& other )
{
	if (&other == this)
	{
		return true;
	}
	return m_strRealString == other.m_strRealString;
}




BOOL CTxParamString::InitLayout()
{
	Clear();
	if (m_strRealString.IsEmpty())
	{
		return TRUE;
	}

	CMarkup markDoc(m_strRealString);
	if (!markDoc.IsWellFormed())
	{
		_RPT1(0, TEXT("paramString format illegual, %s"), m_strRealString.Left(100));
		return FALSE;
	}

	m_kvData.bIsPath = TRUE;
	m_kvData.strKey = TEXT(".");
	m_kvData.strVal = "";
	m_bInited = FindMarkItem(markDoc, m_kvData);
	return TRUE;
}

void CTxParamString::Clear()
{
	m_kvData.mapAttrib.clear();
	m_kvData.mapChildItem.clear();
}

BOOL CTxParamString::GoToPath( LPCTSTR szRoot )
{
	std::vector<std::string> vOut;
	SplitteStrings(szRoot, vOut, '\\');
	if (vOut.size() == 0 || vOut[0].compare(".") != 0)
	{
		_RPT1(0, TEXT("path not illegual, %s"), szRoot);
		return FALSE;
	}
	
	m_vecCurrPath.clear();
	for (size_t i = 0; i < vOut.size(); ++i)
	{
		m_vecCurrPath.push_back(vOut[i].c_str());
	}
	
	return TRUE;
}


ST_MARKITEM* CTxParamString::FindNode( VECSTRINGS& vPaths )
{
	if (vPaths.size() == 0)
	{
		return &m_kvData;
	}
	ST_MARKITEM* pCurrNode = &m_kvData;
	for (size_t i = 1; i < vPaths.size(); ++i)
	{
		MAPKEYVALUE& currNode = pCurrNode->mapChildItem;
		MAPKEYVALUE::iterator itf = currNode.find(vPaths[i]);
		if (itf != currNode.end())
		{
			pCurrNode = &(itf->second);
		}
		else
		{
			pCurrNode = NULL;
			break;
		}
	}

	return pCurrNode;
}


BOOL CTxParamString::UpdateData( BOOL bToReal /*= TRUE*/ )
{
	CString strTmp;
	CMarkup markDoc;
	WriteMarkupString(markDoc, m_kvData);
	m_strRealString = markDoc.GetDoc();
	return TRUE;
}

CTxStrConvert CTxParamString::GetElemVal( LPCTSTR szKey )
{
	CTxStrConvert txConvert;
	if (!m_bInited)
	{
		ASSERT(FALSE);
		return txConvert;
	} 

	VECSTRINGS vPath = m_vecCurrPath;
	if (szKey != NULL)
	{
		vPath.push_back(szKey);
	}
	ST_MARKITEM* pItem = FindNode(vPath);
	if (pItem && !pItem->bIsPath)
	{
		txConvert.m_szParam = pItem->strVal;
	}

	return txConvert;
}

CTxStrConvert CTxParamString::GetAttribVal(LPCTSTR szAttribKey,  LPCTSTR szKey)
{
	CTxStrConvert txConvert;
	if (!m_bInited)
	{
		ASSERT(FALSE);
		return txConvert;
	} 
	VECSTRINGS vPath = m_vecCurrPath;
	if (szKey != NULL)
	{
		vPath.push_back(szKey);
	}
	ST_MARKITEM* pItem = FindNode(vPath);
	if (pItem)
	{
		MAPKEYATTRIB::iterator itf = pItem->mapAttrib.find(szAttribKey);
		if (itf != pItem->mapAttrib.end())
		{
			txConvert.m_szParam = itf->second;
		}
	}
	return txConvert;
}

BOOL CTxParamString::SetElemVal( LPCTSTR szKey, CTxStrConvert& valIn )
{
	if (!m_bInited)
	{
		ASSERT(FALSE);
		return FALSE;
	} 
	BOOL bRet = FALSE;
	VECSTRINGS vPath = m_vecCurrPath;
	ST_MARKITEM* pItem = FindNode(vPath);
	if (pItem && pItem->bIsPath)
	{
		ST_MARKITEM item;
		item.strKey = szKey;
		item.strVal = valIn.m_szParam;
		(pItem->mapChildItem)[item.strKey] = item;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CTxParamString::SetAttribVal( LPCTSTR szKey, LPCTSTR szAttribKey, CTxStrConvert& valIn )
{
	if (!m_bInited)
	{
		ASSERT(FALSE);
		return FALSE;
	} 
	BOOL bRet = FALSE;
	VECSTRINGS vPath = m_vecCurrPath;
	if (szKey != NULL)
	{
		vPath.push_back(szKey);
	}
	ST_MARKITEM* pItem = FindNode(vPath);
	if (pItem)
	{
		(pItem->mapAttrib)[szAttribKey] = valIn.m_szParam;
		bRet = TRUE;
	}

	return bRet;
}



