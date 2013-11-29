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

	MAPKEYVALUE& mapchild = itemIn.mapChildItem;
	MAPKEYVALUE::iterator itkvb = mapchild.begin();
	MAPKEYVALUE::iterator itkve = mapchild.end();
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
	m_vecCurrPath.push_back(TEXT("."));
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
		m_bInited = TRUE;
		return TRUE;
	}

	CMarkup markDoc(m_strRealString);
	if (!markDoc.IsWellFormed())
	{
		_RPT1(0, TEXT("paramString format illegual, %s"), m_strRealString.Left(100));
		return FALSE;
	}
	m_bInited = FindMarkItem(markDoc, m_kvData);
	if (m_vecCurrPath.size() == 0)
	{
		m_vecCurrPath.push_back(TEXT("."));
	}
	return TRUE;
}

void CTxParamString::Clear()
{
	m_kvData.mapAttrib.clear();
	m_kvData.mapChildItem.clear();
	m_kvData.bIsPath = TRUE;
	m_kvData.strKey = TEXT(".");
	m_kvData.strVal = "";
	m_vecCurrPath.clear();
	m_vecCurrPath.push_back(TEXT("."));

}

BOOL CTxParamString::GoToPath( LPCTSTR szRoot )
{
	if (szRoot == NULL)
	{
		m_vecCurrPath.clear();
		m_vecCurrPath.push_back(TEXT("."));
		return TRUE;
	}
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

CTxStrConvert CTxParamString::GetAttribVal( LPCTSTR szKey,LPCTSTR szAttribKey)
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
	BOOL bRet = FALSE;
	VECSTRINGS vPath = m_vecCurrPath;
	ST_MARKITEM* pItem = FindNode(vPath);
	if (pItem )
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

BOOL CTxParamString::GetSubNodeString( LPCTSTR szRoot, CTxParamString& subParamOut )
{
	BOOL bRet = FALSE;
	if (szRoot == NULL)
	{
		//return all
		subParamOut.m_kvData = m_kvData;
		subParamOut.m_bInited = TRUE;
		bRet = TRUE;
	}
	else
	{
		std::vector<std::string> vOut;
		SplitteStrings(szRoot, vOut, '\\');
		if (vOut.size() == 0 || vOut[0].compare(".") != 0)
		{
			_RPT1(0, TEXT("path not illegual, %s"), szRoot);
		}
		else
		{
			VECSTRINGS tmpPath;
			for (size_t i = 0; i < vOut.size(); ++i)
			{
				tmpPath.push_back(vOut[i].c_str());
			}
			ST_MARKITEM* pItem =FindNode(tmpPath);
			if (pItem)
			{
				subParamOut.m_kvData.mapChildItem[pItem->strKey] = *pItem;
				subParamOut.m_bInited = TRUE;
				subParamOut.UpdateData();
				bRet = TRUE;
			}
		}
	}

	return bRet;
}

BOOL CTxParamString::SetSubNodeString( LPCTSTR szRoot, CTxParamString& subParamIn )
{
	if (!subParamIn.m_bInited)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BOOL bRet = FALSE;
	if (szRoot == NULL)
	{
		//return all
		m_kvData = subParamIn.m_kvData;
		m_bInited = TRUE;
		bRet = TRUE;
	}
	else
	{
		std::vector<std::string> vOut;
		SplitteStrings(szRoot, vOut, '\\');
		if (vOut.size() == 0 || vOut[0].compare(".") != 0)
		{
			_RPT1(0, TEXT("path not illegual, %s"), szRoot);
		}
		else
		{
			VECSTRINGS tmpPath;
			for (size_t i = 0; i < vOut.size(); ++i)
			{
				tmpPath.push_back(vOut[i].c_str());
			}
			ST_MARKITEM* pItem =FindNode(tmpPath);
			if (pItem)
			{
				MAPKEYVALUE& mapchild = pItem->mapChildItem;
				MAPKEYVALUE& subChild = subParamIn.m_kvData.mapChildItem;
				MAPKEYVALUE::iterator itcb = subChild.begin();
				MAPKEYVALUE::iterator itce = subChild.end();
				for (; itcb != itce; ++itcb)
				{
					mapchild[itcb->first] = itcb->second;
				}
				
				m_bInited = TRUE;
				bRet = TRUE;
			}
		}
	}

	return bRet;

}

BOOL CTxParamString::GoIntoKey( LPCTSTR szKey )
{
	if (szKey == NULL)
	{
		return FALSE;
	}
	if (!m_bInited)
	{
		return FALSE;
	}

	if (m_vecCurrPath.size() == 0)
	{
		m_vecCurrPath.push_back(TEXT("."));
	}
	m_vecCurrPath.push_back(szKey);
	return TRUE;
}

BOOL CTxParamString::OutofKey()
{
	if (m_vecCurrPath.size() > 1)
	{
		m_vecCurrPath.pop_back();

		return TRUE;
	}

	return FALSE;
}



#ifdef VC6DEFINE

ST_CHILDMAP::ST_CHILDMAP()
	{
		pChildItem = new MAPKEYVALUE;
	}

ST_CHILDMAP::ST_CHILDMAP( ST_CHILDMAP& other )
	{
		pChildItem = new MAPKEYVALUE;
		*pChildItem = *other.pChildItem;
	}

ST_CHILDMAP& ST_CHILDMAP::operator=(const ST_CHILDMAP& other )
{
	
		if (&other == this)
		{
			return *this;
		}
		*pChildItem = *other.pChildItem;
	
}

ST_MARKITEM& ST_CHILDMAP::operator[]( const CString& key )
{
	return (*pChildItem)[key];
}

void ST_CHILDMAP::clear()
{
	(*pChildItem).clear();
}


#endif // VC6DEFINE
