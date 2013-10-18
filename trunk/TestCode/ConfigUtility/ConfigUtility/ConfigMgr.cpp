#include "StdAfx.h"
#include "ConfigMgr.h"
#include <fstream>
#include <string>
#include "FGlobal.h"
#include "StrConvert.h"
#include <algorithm>
#include "shlwapi.h"
//////////////////////////////////////////////////////////////////////////
#define  FILEREAD_BUFF 2048

#define  CH_SQUARE_BRACKET_LEFT		L'['
#define	 CH_SQUARE_BRACKET_RIGHT	L']'
#define  CH_BRACE_LEFT				L'{'
#define  CH_BRACE_RIGHT				L'}'
#define  CH_ANGLE_BRACKET_LEFT		L'<'
#define  CH_ANGLE_BRACKET_RIGHT		L'>'
#define  CH_PARENTHESES_LEFT		L'('
#define  CH_PARENTHESES_RIGHT		L')'
#define  CH_COMMA					L','
#define  CH_SEMICOLON				L';'
#define  CH_DB_QUOTATION_MARK		L'\"'
#define  CH_SPACE					L' '
#define  CH_SLASH					L'/'
#define  CH_BACKSLASH				L'\\'
#define	 CH_EQUAL_MARK				L'='



typedef map<int, int> MAPSUBKEYPOS; //<pos, length>

CConfigMgr::CConfigMgr(void)
{
	m_nCurrLine = 0;
}

CConfigMgr::~CConfigMgr(void)
{
}

BOOL CConfigMgr::GetCfgContent( wstring& swFile, MAPCFGSECTION& dataOut )
{
	BOOL bRet = FALSE;
	m_nCurrLine = -1;
	fstream file;
	file.open(swFile.c_str(), ios_base::in);
	if (!file.is_open())
	{
		ASSERT(FALSE);
		WriteLog(TEXT("Error: can't open file! path = %s"), swFile.c_str());
		return bRet;
	}

	TRY 
	{

		int nRead = 0;
		MAPCFGKEY mapKey;
		wstring szSectionLast;
		while (!file.eof())
		{
			string szbuffLine;
			getline(file, szbuffLine);
			m_nCurrLine++;
			wstring wsbuffer =Ansi2W(szbuffLine);
			Trim(wsbuffer);
			
			if (wsbuffer.empty())
			{
				continue;
			}
			if (IsNote(wsbuffer))
			{
				continue;
			}
			FilterEndNote(wsbuffer);
			if (IsSection(wsbuffer))
			{
				
				if (dataOut.find(wsbuffer) != dataOut.end())
				{
					//already have section
					ASSERT(FALSE);
					WriteLog(TEXT("Error: find duplicate section! name = %s"), szSectionLast.c_str());
					return FALSE;
				}
				mapKey.clear();
				dataOut[wsbuffer] = mapKey;
				szSectionLast = wsbuffer;
				continue;
			}

			ST_CFGVALUE keyVal;
			if (ExtractLine(wsbuffer, keyVal))
			{
				dataOut[szSectionLast][keyVal.wskey] = keyVal;
			}

		}
		bRet = TRUE;

	}
	CATCH (CMemoryException, e)
	{
		file.close();
		ASSERT(FALSE);
		e->ReportError();
		WriteLog(TEXT("Error: process file"));

		return FALSE;
	}
	END_CATCH
	
	file.close();

	m_nCurrLine = -1;

	return bRet;
}

BOOL CConfigMgr::IsSection( wstring& strlineInOut )
{
	size_t nLPos = strlineInOut.find(CH_SQUARE_BRACKET_LEFT);
	if (nLPos == 0)
	{
		size_t nRPos = strlineInOut.rfind(CH_SQUARE_BRACKET_RIGHT);
		if (nRPos != wstring::npos)
		{
			strlineInOut = strlineInOut.substr(nLPos, nRPos+1);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CConfigMgr::ExtractLine( wstring& strline, ST_CFGVALUE& valOut )
{
	int nLen = strline.length();
	//at least 3 chars
	if (nLen < 3)
	{
		ASSERT(FALSE);
		WriteLog(TEXT("Error: error format! line = %s"), strline.c_str());
		return FALSE;
	}

	size_t nPos = strline.find(CH_EQUAL_MARK);
	if (nPos == wstring::npos || nPos == 0 || nPos == nLen -1)
	{
		ASSERT(FALSE);
		WriteLog(TEXT("Error: error format! line = %s"), strline.c_str());
		return FALSE;
	}
	
	valOut.wskey = strline.substr(0, nPos);
	Trim(valOut.wskey);
	valOut.wsOrgData = strline.substr(nPos+1, (nLen - nPos-1));
	Trim(valOut.wsOrgData);
	return ExtractValue(valOut);

}

BOOL CConfigMgr::IsNote( wstring& strLineIn )
{
	size_t nLen = strLineIn.length();
	return nLen < 0 || (strLineIn.at(0)==CH_SEMICOLON)
		|| ( nLen > 1 && strLineIn.at(0)== CH_SLASH && strLineIn.at(1) == CH_SLASH);
}

void CConfigMgr::FilterEndNote( wstring& strLineInOut )
{
	size_t nPos = strLineInOut.rfind(CH_SLASH);
	while(nPos != wstring::npos && nPos != 0)
	{
		if (strLineInOut.at(nPos-1) == CH_SLASH)
		{
			if (nPos == 1)
			{
				strLineInOut.clear();
				return;
			}
			strLineInOut = strLineInOut.substr(0, nPos-1);
			nPos = strLineInOut.rfind(CH_SLASH);
		}
		else
		{
			nPos = strLineInOut.rfind(CH_SLASH, nPos-1);
		}

	}
	
}

BOOL CConfigMgr::ExtractValue( ST_CFGVALUE& valOut )
{
	//check orgvalue
	size_t nOrgLen = valOut.wsOrgData.length();
	if (nOrgLen <= 0)
	{
		WriteLog(TEXT("Waring: Find empty value %s="), valOut.wskey.c_str());
		return FALSE;
	}

	wstring& wsRefOrgVal = valOut.wsOrgData;
	WCHAR chSign = wsRefOrgVal.at(0);

	//check string
	if (chSign == CH_DB_QUOTATION_MARK)
	{
		if (nOrgLen < 3 || wsRefOrgVal.at(nOrgLen-1) != CH_DB_QUOTATION_MARK)
		{
			WriteLog(TEXT("Waring: string value format error! %s=%s"), valOut.wskey.c_str(), wsRefOrgVal.c_str());
			ASSERT(FALSE);
			return FALSE;
		}
		
		valOut.nType = CFGVALTYPE_STR;
		valOut.wsVal = wsRefOrgVal.substr(1, nOrgLen-2);
		
		size_t nValLen = valOut.wsVal.size();
		if (nValLen == 4 || nValLen == 5)
		{
			wstring strTmpVal = valOut.wsVal;
		
			//transform(strTmpVal.begin(), strTmpVal.end(), strTmpVal.begin(), toupper);
			StringToUpper(strTmpVal);
			
			if (strTmpVal.compare(TEXT("TRUE"))== 0 || strTmpVal.compare(TEXT("FALSE"))== 0)
			{
				//find bool value
				valOut.nType = CFGVALTYPE_BOOL;
				valOut.wsVal = strTmpVal;
			}
		}
	
	}
	else if (chSign <= L'9' && chSign >=L'0')
	{
		//find num
		valOut.nType = CFGVALTYPE_NUMBER;
		valOut.wsVal = wsRefOrgVal;
	}
	else if (chSign == CH_BRACE_LEFT || chSign == CH_ANGLE_BRACKET_LEFT)
	{
		TCHAR chSignRight = (chSign == CH_BRACE_LEFT)? CH_BRACE_RIGHT:CH_ANGLE_BRACKET_RIGHT;
		if (nOrgLen < 3 || wsRefOrgVal.at(nOrgLen-1) != chSignRight)
		{
			WriteLog(TEXT("Waring: array value format error! %s=%s"), valOut.wskey.c_str(), wsRefOrgVal.c_str());
			ASSERT(FALSE);
			return FALSE;
		}

		//remove{}
		wstring strTmpOrgVal = wsRefOrgVal.substr(1, nOrgLen-2);
		Trim(strTmpOrgVal);
		size_t nTmpOrgLen = strTmpOrgVal.length();

		//check if sub key
		size_t nPosEqual = strTmpOrgVal.find(CH_EQUAL_MARK);
		if (nPosEqual != wstring::npos)
		{
// 			if (wsRefOrgVal.find(CH_COMMA) != wstring::npos)
// 			{
// 				//CFWriteLog(TEXT("Error: sub keys can't be separated with ','! %s=%s"), valOut.wskey.c_str(), wsRefOrgVal.c_str());
// 				//return FALSE;
// 				
// 			}
			
			TCHAR szSeparateMark[3];
			szSeparateMark[0]=CH_COMMA;
			szSeparateMark[1]= CH_DB_QUOTATION_MARK;
			szSeparateMark[2] = L'\0';
			int nKeyCount = 0;
			MAPSUBKEYPOS mapPos;
			BOOL bSucSplit= TRUE;
			while(nPosEqual != wstring::npos)
			{
				size_t nPosSubBegin = nKeyCount== 0? 0:strTmpOrgVal.find_last_of(szSeparateMark, nPosEqual)+1;
				size_t nPosSubEnd = strTmpOrgVal.find(CH_DB_QUOTATION_MARK, nPosEqual);

				if (nPosSubEnd != wstring::npos && nPosSubEnd < nTmpOrgLen-1)
				{
					nPosSubEnd = strTmpOrgVal.find(CH_DB_QUOTATION_MARK, nPosSubEnd+1);
					if (nPosSubEnd != wstring::npos
						&& nPosSubEnd - nPosSubBegin > 5) //at least 5 char: x="x"
					{
						//save to map
						mapPos[nPosSubBegin] = nPosSubEnd - nPosSubBegin+1;
					}
					else
					{
						bSucSplit = FALSE;
						break;
					}
					
				}
				else
				{
					bSucSplit = FALSE;
					break;
				}

				++nKeyCount;
				nPosEqual = strTmpOrgVal.find(CH_EQUAL_MARK, nPosEqual+1);
			}

			if (!bSucSplit || nKeyCount != mapPos.size())
			{
				bSucSplit = FALSE;
			}
			else
			{
				//check if overlapped
				MAPSUBKEYPOS::iterator itb = mapPos.begin();
				MAPSUBKEYPOS::iterator ite = mapPos.end();
				MAPSUBKEYPOS::iterator itp = itb;
				++itp;
				while(itp != ite)
				{
					if (itb->first + itb->second > itp->first)
					{
						bSucSplit = FALSE;
						break;
					}
					++itb;
					++itp;
				}
			}
			
			if (!bSucSplit)
			{
				WriteLog(TEXT("Error: sub keys fromat error! %s=%s"), valOut.wskey.c_str(), wsRefOrgVal.c_str());
				return FALSE;
			}

			//make subkey
			MAPSUBKEYPOS::iterator itb = mapPos.begin();
			MAPSUBKEYPOS::iterator ite = mapPos.end();
			while(itb != ite)
			{
				wstring wsTmpSubVal= strTmpOrgVal.substr(itb->first, itb->second);
				Trim(wsTmpSubVal);
				ST_CFGVALUE stKeyVal;
				if(ExtractLine(wsTmpSubVal, stKeyVal))
				{
					if (valOut.mapVal.find(stKeyVal.wskey) != valOut.mapVal.end())
					{
						ASSERT(FALSE);
						WriteLog(TEXT("Error: find duplicate key! name = %s"), stKeyVal.wskey.c_str());
					}
					valOut.mapVal[stKeyVal.wskey]= stKeyVal;
				}
				else
				{
					ASSERT(FALSE);
					return FALSE;
				}
				

				++itb;
			}

			valOut.nType = CFGVALTYPE_MAP;

		}
		else
		{
			//array type
			WCHAR chSeparate = L'\0';
			size_t nPosBegin = 0;
			size_t nPosEnd = 0;

			wstring wsRemain = strTmpOrgVal;
			while(wsRemain.length() > 0)
			{
				size_t nPosComma =wsRemain.find(CH_COMMA);
				if (nPosComma == wstring::npos)
				{
					//only one
					valOut.vecVal.push_back(strTmpOrgVal);
					wsRemain = TEXT("");
					break;
				}

				wstring wsSubVal = wsRemain.substr(nPosBegin, nPosComma - nPosBegin);
				Trim(wsSubVal);
				if (wsSubVal.length() <= 0)
				{
					WriteLog(TEXT("Error: array value fromat error! %s=%s"), valOut.wskey.c_str(), wsRefOrgVal.c_str());
					return FALSE;
				}
				
				ST_CFGVALUE tmpVal;
				tmpVal.wsOrgData = wsSubVal;
				if (ExtractValue(tmpVal))
				{
					valOut.vecVal.push_back(tmpVal.wsVal);
				}

				wsRemain = wsRemain.substr(nPosComma+1);

			}

			valOut.nType = CFGVALTYPE_VEC;	
		}
	}
	else
	{
		WriteLog(TEXT("Error: value fromat error! %s=%s"), valOut.wskey.c_str(), wsRefOrgVal.c_str());
		return FALSE;
	}

	return TRUE;
}


int CConfigMgr::Compare( wstring& swFileLeft, wstring& swFileRight )
{
	//reset
	m_nCurrLine = -1;
	m_mapValLeft.clear();
	m_mapValRight.clear();

	WriteLog(TEXT("注意: 如果输出日志中出现错误，请先排除错误后再检查比较结果！"));

	WriteLog(TEXT("info: start get config left..., path = %s"), swFileLeft.c_str());
	if (!GetCfgContent(swFileLeft, m_mapValLeft))
	{
		wstring wsMsg = TEXT("Loading config file error, check log for detail. file = ");
		wsMsg +=swFileLeft;
		//AfxMessageBox(wsMsg.c_str());
		WriteLog(wsMsg.c_str());
		ClearResult();
		return -1;
	}

	WriteLog(TEXT("info: start get config right..., path = %s"), swFileRight.c_str());
	if (!GetCfgContent(swFileRight, m_mapValRight))
	{
		wstring wsMsg = TEXT("Loading config file error, check log for detail. file = ");
		wsMsg +=swFileRight;
		WriteLog(wsMsg.c_str());
		//AfxMessageBox(wsMsg.c_str());
		ClearResult();
		return -1;
	}

	//fill left and right

	WriteLog(TEXT("info: start compare file left..., path = %s"), swFileLeft.c_str());
	ENUMCOMAPRETYPE nRet1 = CompareLeft(m_mapValLeft, m_mapValRight);
	WriteLog(TEXT("info: start compare file right..., path = %s"), swFileRight.c_str());

	ENUMCOMAPRETYPE nRet2 = CompareLeft(m_mapValRight, m_mapValLeft);
	WriteLog(TEXT("info: compare file finished..."));
	BOOL bEqual = (nRet1 == COMPTYPE_EQUAL && nRet2 == COMPTYPE_EQUAL);
	WriteLog(bEqual? TEXT("info: files are equal"): TEXT("info: difference founded!"));

	return bEqual? 0:1;
}


ENUMCOMAPRETYPE CConfigMgr::CompareLeft( MAPCFGSECTION& mapValLeft, MAPCFGSECTION& mapValRight)
{


	//MAPCFGSECTION::iterator itbL = mapValLeft.begin();
	//MAPCFGSECTION::iterator iteL = mapValLeft.end();
	ENUMCOMAPRETYPE nRet  = COMPTYPE_EQUAL;
	ENUMCOMAPRETYPE nRetTmp= COMPTYPE_EQUAL;

	MAPCFGSECTION::iterator iteR = mapValRight.end();
	MAPCFGSECTION::iterator itbR = mapValRight.begin();

	MAPCFGSECTION mapLossed;
	MAPCFGKEY* pCmpLeft = NULL;
	for (; itbR != iteR; ++itbR)
	{
		MAPCFGSECTION::iterator itfL = mapValLeft.find(itbR->first);
		if (itfL == mapValLeft.end())
		{
			MAPCFGKEY mapTmp;
			nRetTmp = CompareLeft(mapTmp, itbR->second);
			mapValLeft[itbR->first] = mapTmp;

		}
		else
		{
			nRetTmp= CompareLeft(itfL->second, itbR->second);
		}

		nRet = (nRet == COMPTYPE_EQUAL)? nRetTmp:nRet;
	}

	return nRet;

}

ENUMCOMAPRETYPE CConfigMgr::CompareLeft( ST_CFGVALUE& stValLeft, ST_CFGVALUE& stValRight )
{
	if (stValLeft.wskey.compare(stValRight.wskey) != 0)
	{
		WriteLog(TEXT("compare key value not equal %s vs %s"), stValLeft.wskey.c_str(), stValRight.wskey.c_str());
		ASSERT(FALSE);
		return COMPTYPE_NONE;
	}

	if (stValRight.nCompType != COMPTYPE_NONE && stValLeft.nType != CFGVALTYPE_MAP)
	{
		//already compared
		if (stValRight.nCompType == COMPTYPE_DIFF || stValRight.nCompType == COMPTYPE_TYPEDIFF)
		{
			stValLeft.nCompType = stValRight.nCompType;
		}
		else if (stValRight.nCompType == COMPTYPE_LOSS)
		{
			stValLeft.nCompType = COMPTYPE_EXTRA;
		}
		else if (stValRight.nCompType == COMPTYPE_EQUAL)
		{
			stValLeft.nCompType = COMPTYPE_EQUAL;
		}
		return stValLeft.nCompType;
	}

	if (stValLeft.nType != stValRight.nType)
	{
		WriteLog(TEXT("compare key type not equal %s"), stValLeft.wskey.c_str());
		stValLeft.nCompType = COMPTYPE_TYPEDIFF;
		return COMPTYPE_TYPEDIFF;
	}

	switch (stValLeft.nType)
	{
	case CFGVALTYPE_STR:
	case CFGVALTYPE_BOOL:
	case CFGVALTYPE_NUMBER:
		{
			stValLeft.nCompType = (stValLeft.wsVal.compare(stValRight.wsVal) == 0)? COMPTYPE_EQUAL:COMPTYPE_DIFF;
		}
		break;

	case CFGVALTYPE_MAP:
		{
			stValLeft.nCompType =  CompareLeft(stValLeft.mapVal, stValRight.mapVal);
		}break;
	case CFGVALTYPE_VEC:
		{
			stValLeft.nCompType = CompareLeft(stValLeft.vecVal, stValRight.vecVal);
		}
	}

	return stValLeft.nCompType;
	
}

ENUMCOMAPRETYPE CConfigMgr::CompareLeft( VECCFGVALUE& vecValLeft, VECCFGVALUE& vecValRight )
{
	if (vecValLeft.size() != vecValRight.size())
	{
		return COMPTYPE_DIFF;
	}
	
	for (size_t i = 0; i < vecValLeft.size(); i++)
	{
		if (std::find(vecValRight.begin(), vecValRight.end(), vecValLeft[i]) == vecValRight.end())
		{
			//not find!;
			return COMPTYPE_DIFF;
		}
	}

	return COMPTYPE_EQUAL;
}

ENUMCOMAPRETYPE CConfigMgr::CompareLeft( MAPCFGKEY& mapValLeft, MAPCFGKEY& mapValRight )
{

	ENUMCOMAPRETYPE nRet  = mapValLeft.size()== 0? COMPTYPE_LOSS:COMPTYPE_EQUAL;
	ENUMCOMAPRETYPE nRetTmp= COMPTYPE_EQUAL;
	
	MAPCFGKEY::iterator itbR = mapValRight.begin();
	MAPCFGKEY::iterator iteR = mapValRight.end();
	for (; itbR != iteR; ++itbR)
	{
		MAPCFGKEY::iterator itfL =  mapValLeft.find(itbR->first);
		if (itfL == mapValLeft.end())
		{
			//not find
			ST_CFGVALUE tmpVal;
			tmpVal.nType = itbR->second.nType;
			tmpVal.nCompType = COMPTYPE_LOSS;
			tmpVal.wskey = itbR->first;
			itbR->second.nCompType = COMPTYPE_EXTRA;
			nRetTmp = CompareLeft(tmpVal, itbR->second);
			mapValLeft[tmpVal.wskey] = tmpVal;

		}
		else
		{
			nRetTmp = CompareLeft(itfL->second, itbR->second);
		}


		nRet = (nRet == COMPTYPE_EQUAL)? nRetTmp:nRet;

	}
	return nRet;
}

ENUMCOMAPRETYPE CConfigMgr::CompareLeft( CFGVALUE& wsValLeft, CFGVALUE& wsValRight )
{
	return wsValLeft.compare(wsValRight)== 0? COMPTYPE_EQUAL:COMPTYPE_DIFF;
}

BOOL CConfigMgr::ResetData()
{
	if (_waccess(g_GlobalInfo.wsCfgSrcPath.c_str(), 00) == -1)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//find root
	CString strFindFilter = g_GlobalInfo.wsCfgSrcPath.c_str();
	strFindFilter += TEXT("\\*");
	CFileFind ffind;
	BOOL bFind= ffind.FindFile(strFindFilter);
	while(bFind)
	{
		bFind = ffind.FindNextFile();
		FindCfgCallback(ffind, 0, (LPARAM)&m_mapCfgRoot);
	}
	
	ffind.Close();
	
	return TRUE;
}

void CConfigMgr::ClearResult()
{
	m_mapValLeft.clear();
	m_mapValRight.clear();
}

void CConfigMgr::WriteLog( LPCTSTR format,... )
{

	//add line number to format
	CString strFmt;
	if (m_nCurrLine >= 0)
	{
		strFmt.Format(TEXT("Line:%d --%s"), m_nCurrLine, format);
	}
	else
	{
		strFmt = format;
	}

	va_list ap;
	va_start(ap, format);
	CFWriteLog2(strFmt, ap);
	va_end(ap);
}





void FindCfgCallback(CFileFind& ffindIn, int nLevel, LPARAM lparam )
{
	if (nLevel == 0)
	{
		//root
		CString strPath = ffindIn.GetFilePath();
		if (ffindIn.IsDirectory()&& !ffindIn.IsDots())
		{
			ST_MAPSSUB mapSub;
			CString strFindFilter = strPath;
			strFindFilter += TEXT("\\*");
			CFileFind ffindSub;
			BOOL bfind =ffindSub.FindFile(strFindFilter);
			while (bfind)
			{
				bfind = ffindSub.FindNextFile();
				FindCfgCallback(ffindSub, 1, (LPARAM)&mapSub);
				

				
			}
			ffindSub.Close();
			if (mapSub.m_data.size() > 0)
			{
				MAPSROOT* pMapRoot = (MAPSROOT*)lparam;
				wstring strRoot = ffindIn.GetFileName();
				(*pMapRoot)[strRoot] = mapSub;
			}
		}

	}
	else if (nLevel == 1)
	{
		//sub
		CString strPath = ffindIn.GetFilePath();
		if (ffindIn.IsDirectory() && !ffindIn.IsDots())
		{
			ST_MAPSSUB2 mapSubsub;
			CString strFindFilter = strPath;
			strFindFilter += TEXT("\\*.*");
			CFileFind ffindSubsub;
			BOOL bfind =ffindSubsub.FindFile(strFindFilter);
			while (bfind)
			{
				bfind = ffindSubsub.FindNextFile();
				FindCfgCallback(ffindSubsub, 2, (LPARAM)&mapSubsub);
				
				

			}
			ffindSubsub.Close();
			if (mapSubsub.m_data.size() > 0)
			{
				MAPSSUB* pMapSub = (MAPSSUB*)lparam;
				wstring strSubKey = ffindIn.GetFileName();
				(*pMapSub)[strSubKey] = mapSubsub;
			}
			
		}

	}
	else if (nLevel == 2)
	{
		//subsub
		CString strPath = ffindIn.GetFilePath();
		if (!PathIsDirectory(strPath))
		{
			CString strPath = ffindIn.GetFilePath();
			int nPos = strPath.ReverseFind(L'.');
			if (nPos > 0)
			{
				CString strExt = strPath.Right(strPath.GetLength() - nPos-1);
				strExt.MakeLower();
				if (strExt.Compare(TEXT("ini"))== 0 || strExt.Compare(TEXT("clsid"))== 0)
				{
					MAPSSUB2* pMapSubsub = (MAPSSUB2*)lparam;
					wstring wsSubsubKey = ffindIn.GetFileName();
					(*pMapSubsub)[wsSubsubKey] = ffindIn.GetFilePath();
				}
			}
		}
	}
}
