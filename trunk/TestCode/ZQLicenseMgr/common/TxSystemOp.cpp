#include "StdAfx.h"
#include "TxSystemOp.h"
#include <iphlpapi.h>
#include "objbase.h"
#include "assert.h"
#include "CBase64.h"
#include "intrin.h"
#include "StrConvert.h"
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma  comment (lib, "Version.lib")
using namespace std;

string HDGetMac()
{
	string strMac;

	ULONG ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);
	IP_ADAPTER_INFO *pAdapterInfoBkp = NULL, *pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW ) // 缓冲区不够大
	{
		char* p = (char*)pAdapterInfo;
		delete []p;
		pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];		
	}

	if( GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_SUCCESS )
	{
		pAdapterInfoBkp = pAdapterInfo;
		do{ // 遍历所有适配器
			if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)    // 判断是否为以太网接口
			{
				// pAdapterInfo->Description 是适配器描述
				// pAdapterInfo->AdapterName 是适配器名称
				BOOL bValidMac = FALSE;
				if ((pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET) )
				{
					//invalid ip
					string strIp=pAdapterInfo->IpAddressList.IpAddress.String;
					if (strIp.compare(TEXT("0.0.0.0")) != 0)
					{
						bValidMac = TRUE;
					}
				}

				BOOL bAllZero = TRUE;
				strMac.clear();
				for(UINT i = 0; i < pAdapterInfo->AddressLength; i++)
				{
					if (pAdapterInfo->Address[i] != 0)
					{
						bAllZero = FALSE;
					}
					char szTmp[8];
					sprintf(szTmp, "%02X",pAdapterInfo->Address[i]/*, (i == pAdapterInfo->AddressLength-1) ? '\0':'-'*/);
					strMac.append(szTmp);
				}

				bValidMac = !bAllZero;

				if (!bValidMac)
				{
					//g_log.Print(TEXT("\ninvalid mac =%s"), strMac.c_str());
					pAdapterInfo = pAdapterInfo->Next;
					continue;
				}
				else
				{
					//g_log.Print(TEXT("\nvalid mac =%s"), strMac.c_str());
				}
#ifndef _DEBUG
				break;
#endif // _DEBUG
			}
			pAdapterInfo = pAdapterInfo->Next;
		}while(pAdapterInfo);
	}

	if(pAdapterInfoBkp)
	{
		char *p = (char*)pAdapterInfoBkp;
		delete []p;
	}	

	StringToUpper(strMac);
	return strMac;
}


int GenRand()
{
	GUID guid = GUID_NULL;
	::CoCreateGuid(&guid);

	if (guid == GUID_NULL)
	{
		srand(time(NULL));
		return rand();
	}

	srand(guid.Data1 + guid.Data2 +guid.Data3+
		guid.Data4[0]+ guid.Data4[1]+ guid.Data4[2]+guid.Data4[3] +
		guid.Data4[4]+ guid.Data4[5]+ guid.Data4[6]+ guid.Data4[7]);
	return rand();
}

string MakeCh1(const char chIn, const int nDeta = 1)
{
	assert(nDeta < 0x102);
	int  nP0 = (int)(chIn)*nDeta;
	char nP1 = (nP0 >> 10) & 0x1f;
	char nP2 =(nP0 >> 5) & 0x1f; 
	char nP3 = nP0& 0x1f; 
	string szTmp;
	szTmp.push_back(nP1  < 10? (nP1+0x30):nP1 +0x37);
	szTmp.push_back(nP2 <10? (nP2+0x30):nP2+0x37);
	szTmp.push_back(nP3 <10? (nP3+0x30):nP3+0x37);
	return szTmp;
}


char UnMakeCh(const char chIn1, const char chIn2, const char chIn3, const int nDeta  = 1)
{

	int  nP1 = (chIn1 > 0x40? chIn1 - 0x37 :chIn1 - 0x30)<<10;
	int  nP2 = (chIn2 > 0x40? chIn2 - 0x37 :chIn2 - 0x30) <<5;
	int  nP3 = (chIn3 > 0x40? chIn3 - 0x37 :chIn3 - 0x30) ;
	int nRes = (nP1 | nP2 | nP3) / nDeta;
	return (char)nRes;
}

BOOL InitZQ( ST_NAME& name )
{
	//check valid
	if (name.key1.size() == 0
		||name.key2.size() == 0)
	{
		return FALSE;
	}

	int nNeedLen = (name.key1.size() + name.key2.size() + 4)*3;
	if (nNeedLen > GENLENGTH)
	{
		return FALSE;
	}


	//gen
	string szOut;
	szOut.clear();
	szOut.reserve(GENLENGTH + 1);
	for (size_t i = 0; i < 12; i++)
	{
		szOut.push_back('0');
	}

	char nRnd = GenRand()%0x70 + 1;
	szOut.append( MakeCh1(nRnd));
	char nSize1 = name.key1.size()*3;
	szOut.append(MakeCh1(nSize1, nRnd));
	int nNum = szOut.size();
	for (size_t i = 0; i< name.key1.size(); ++i, ++nNum)
	{
		szOut.append(MakeCh1(name.key1[i], nRnd+ i%20));
	}
	
	nRnd = GenRand()%0x70 +2;
	szOut.append( MakeCh1(nRnd));
	short int nSize2 = name.key2.size()*3;
	char* pSize2 = (char*)&nSize2;
	szOut.append(MakeCh1(pSize2[0], nRnd));
	szOut.append(MakeCh1(pSize2[1], nRnd));

	nNum = szOut.size();
	for (size_t i = 0; i < name.key2.size(); ++i, ++nNum)
	{
		szOut.append( MakeCh1(name.key2[i], nRnd+ i%20));
	}

	nRnd = GenRand()%0x70 +2;
	szOut.append(MakeCh1(nRnd));
	char nSize3 = name.key3.size()*3;
	szOut.append( MakeCh1(nSize3, nRnd));
	nNum = szOut.size();
	for (size_t i = 0; i < name.key3.size(); ++i, ++nNum)
	{
		szOut.append( MakeCh1(name.key3[i], nRnd+ i%20));
	}

	int nRemain = GENLENGTH - szOut.size();
	int nCRC = 0;
	for (int i = 0; i < nRemain; i++)
	{
		string szRand = MakeCh1((char)GenRand());
		szOut.push_back(szRand[2]);
	}

	for (size_t i = 12; i < szOut.size(); ++i)
	{
		nCRC += szOut[i]*i;
	}

	//pushback crc
	char* pcrc=(char*)&nCRC;
	for (size_t i = 0; i< 4; ++i)
	{
		string tmp = MakeCh1(pcrc[i]);
		szOut[3*i] = tmp[0];
		szOut[3*i+1] = tmp[1];
		szOut[3*i+2] = tmp[2];
	}
	
	char szbuff[1024];
	ZeroMemory(szbuff, 1024);
	CBase64::base64((unsigned char*)szbuff, (const unsigned char*)szOut.c_str(), szOut.size());
	name.key4 = szbuff;
	return TRUE;
}

BOOL UnInitZQ( ST_NAME& name )
{
	if (name.key4.size() == 0)
	{
		return FALSE;
	}

	char szbuff[1024];
	ZeroMemory(szbuff, 1024);
	int nSuc = CBase64::unbase64((unsigned char*)szbuff,  (const unsigned char*)name.key4.c_str(), 512);
	string szKey4 = szbuff;
	const int nKeyLen =szKey4.size();
	if (nKeyLen != GENLENGTH)
	{
		return FALSE;
	}

	string& szKey = szKey4;

	char pCrc[4];
	for (size_t i = 0; i < 4; ++i)
	{
		pCrc[i] = UnMakeCh(szKey[3*i], szKey[3*i+1], szKey[3*i+2]);
	}

	int* nKeyCRC = (int*)pCrc;
	int nCRC = 0;

	for (size_t i = 12; i < szKey.size(); ++i)
	{
		nCRC += szKey[i]*i;
	}

	if (nCRC != *nKeyCRC)
	{
		return FALSE;
	}
	///
	int nPos = 12;
	char nRnd = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos +2]);
	nPos +=3;
	int nSize =UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2], nRnd);
	nPos +=3;
	if (nSize >= nKeyLen -nPos || nSize%3 != 0)
	{
		return FALSE;
	}

	string& szkey1 = name.key1;
	int nLoop = nSize/3;
	for (size_t i = 0; i < nLoop; ++i)
	{
		szkey1.push_back(UnMakeCh(szKey[nPos+3*i], szKey[nPos+3*i+1], szKey[nPos+3*i+2],nRnd+i%20));
	}

	///
	nPos += nSize;
	nRnd = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2]);
	nPos +=3;
	char cSize2[2];
	cSize2[0] = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2], nRnd);
	nPos +=3;
	cSize2[1] =UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2], nRnd);
	nSize = *((short int*)cSize2);
	nPos +=3;
	if (nSize >= nKeyLen -nPos || nSize%3 != 0)
	{
		return FALSE;
	}

	string& szkey2 = name.key2;
	nLoop = nSize/3;
	for (size_t i = 0; i < nLoop; ++i)
	{
		szkey2.push_back(UnMakeCh(szKey[nPos+3*i], szKey[nPos+3*i+1], szKey[nPos+3*i+2], nRnd+i%20));
	}

	///
	nPos += nSize;
	nRnd = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2]);
	nPos +=3;
	nSize = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2],nRnd);
	nPos +=3;
	if (nSize >= nKeyLen -nPos || nSize%3 != 0)
	{
		return FALSE;
	}

	string& szkey3 = name.key3;
	nLoop = nSize/3;
	for (size_t i = 0; i < nLoop; ++i)
	{
		szkey3.push_back(UnMakeCh(szKey[nPos+3*i], szKey[nPos+3*i+1], szKey[nPos+3*i+2], nRnd+i%20));
	}

	return TRUE;
}




// BOOL GetLocalMacAddr( CString& strMacOut )
// {
// 
// 	strMacOut.Empty();
// 	std::string strMac = HDGetMac();
// 	if (!strMac.empty())
// 	{
// 		strMacOut = strMac.c_str();
// 		return TRUE;
// 	}
// 	return FALSE;
// }

BOOL SplitteStrings( const char* szIn , vector<string>& vOut)
{
	string szData = szIn;
	if (szData.size() == 0)
	{
		return FALSE;
	}

	size_t nBegin = 0;
	for (size_t i = 0; i < szData.size();++i)
	{
		if (szData[i] == ',' || szData[i] == '，')
		{
			if (i > nBegin)
			{
				vOut.push_back(szData.substr(nBegin, i - nBegin));
			}
			nBegin = i+1;
		}		
	}

	if (nBegin < szData.size())
	{
		vOut.push_back(szData.substr(nBegin, szData.size() - nBegin));
	}
	return TRUE;
}

BOOL InitReg( ST_NAME& name )
{
	name.key1 = GenRegCode();
	if (name.key1.size() == 0)
	{
		return FALSE;
	}
	

	string szOut;
	for (size_t i = 0; i < 12; i++)
	{
		szOut.push_back('0');
	}
	
	char nRnd = GenRand()%0x70 + 1;
	szOut.append( MakeCh1(nRnd));
	char nSize1 = name.key1.size()*3;
	szOut.append(MakeCh1(nSize1, nRnd));
	int nNum = szOut.size();
	for (size_t i = 0; i< name.key1.size(); ++i, ++nNum)
	{
		szOut.append(MakeCh1(name.key1[i], nRnd+ i%20));
	}

	int nCRC = 0;
	for (size_t i = 12; i < szOut.size(); ++i)
	{
		nCRC += szOut[i]*i;
	}

	//pushback crc
	char* pcrc=(char*)&nCRC;
	for (size_t i = 0; i< 4; ++i)
	{
		string tmp = MakeCh1(pcrc[i]);
		szOut[3*i] = tmp[0];
		szOut[3*i+1] = tmp[1];
		szOut[3*i+2] = tmp[2];
	}


	char szbuff[512];
	ZeroMemory(szbuff, 512);
	CBase64::base64((unsigned char*)szbuff, (const unsigned char*)szOut.c_str(), szOut.size());
	name.key4 = szbuff;
	return TRUE;
}


BOOL UninitReg( ST_NAME& name )
{
	if (name.key4.size() == 0)
	{
		return FALSE;
	}

	char szbuff[512];
	ZeroMemory(szbuff, 512);
	int nSuc = CBase64::unbase64((unsigned char*)szbuff,  (const unsigned char*)name.key4.c_str(), 512);
	string szKey = szbuff;
	const int nKeyLen =szKey.size();
	if (nKeyLen != REGLENGTH)
	{
		return FALSE;
	}

	char pCrc[4];
	for (size_t i = 0; i < 4; ++i)
	{
		pCrc[i] = UnMakeCh(szKey[3*i], szKey[3*i+1], szKey[3*i+2]);
	}

	int* nKeyCRC = (int*)pCrc;
	int nCRC = 0;

	for (size_t i = 12; i < szKey.size(); ++i)
	{
		nCRC += szKey[i]*i;
	}

	if (nCRC != *nKeyCRC)
	{
		return FALSE;
	}

	int nPos = 12;
	char nRnd = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos +2]);
	nPos +=3;
	int nSize = UnMakeCh(szKey[nPos], szKey[nPos+1], szKey[nPos+2], nRnd);
	nPos +=3;
	if (nSize > nKeyLen -nPos || nSize%3 != 0)
	{
		return FALSE;
	}

	string& szkey1 = name.key1;
	int nLoop = nSize/3;
	for (size_t i = 0; i < nLoop; ++i)
	{
		szkey1.push_back(UnMakeCh(szKey[nPos+3*i], szKey[nPos+3*i+1], szKey[nPos+3*i+2],nRnd+i%20));
	}

	return TRUE;
}

std::string HDGetCPU()
{
 
	int nId[4];
	__cpuid(nId, 0);
	
	char szId[64];
	memset(szId,0, sizeof(szId));
	sprintf_s(szId, "%08x%08x%08x", nId[1], nId[2], nId[3]);
	string strRet = szId;
	StringToUpper(strRet);
	return strRet;
}

std::string GenRegCode()
{
	return HDGetMac() + HDGetCPU();
}

std::string GetProductName()
{
	string strRet;
	char szAppbuff[GENLENGTH*2+1];
	memset(szAppbuff, 0, sizeof(szAppbuff));
	GetModuleFileName(NULL, szAppbuff, sizeof(szAppbuff) -1);

	DWORD dwHdl = 0;
	DWORD dwVersize =GetFileVersionInfoSize(szAppbuff, &dwHdl);
	if(dwVersize > 0)
	{
		BYTE* strVerInfo = new BYTE[dwVersize];
		if(GetFileVersionInfo(szAppbuff, dwHdl, dwVersize, strVerInfo))
		{
			unsigned int  cbTranslate = 0;
			struct LANGANDCODEPAGE {
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;
			VerQueryValue(strVerInfo, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);
			// Read the file description for each language and code page.
			int iCode = 0;
			BOOL bFind = FALSE;
// 			for( int i=0; iCode < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); iCode++ )
// 			{
// 				if (nCurrLanCode == lpTranslate[iCode].wCodePage)
// 				{
// 					bFind = TRUE;
// 					break;
// 
// 				}
// 			}
			if (!bFind)
			{
				iCode = 0; //get default ver
			}
			char  SubBlock[200];
			wsprintf( SubBlock, 
				TEXT("\\StringFileInfo\\%04x%04x\\ProductName"),
				lpTranslate[iCode].wLanguage,
				lpTranslate[iCode].wCodePage);
			void *lpBuffer=NULL;
			unsigned int dwBytes=0;
			VerQueryValue(strVerInfo, 
				SubBlock, 
				&lpBuffer, 
				&dwBytes); 
			if (lpBuffer)
			{
				strRet =(char *)lpBuffer;
			}
		}
		delete[] strVerInfo;
	}

	return strRet;
}
