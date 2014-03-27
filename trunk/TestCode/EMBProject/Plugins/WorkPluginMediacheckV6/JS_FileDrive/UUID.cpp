// GUID.cpp: implementation of the CGUID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UUID.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CUUID::CUUID()
{
}
*/
CUUID::CUUID(const BYTE* pGuid )
{
	if(!IsBadReadPtr(pGuid,16))
	{
       m_UUID.Data1  = pGuid[0];
	   m_UUID.Data1 += pGuid[1]<<8;
	   m_UUID.Data1 += pGuid[2]<<16;
	   m_UUID.Data1 += pGuid[3]<<24;

	   m_UUID.Data2  = pGuid[4];
	   m_UUID.Data2 += pGuid[5]<<8;

	   m_UUID.Data3  = pGuid[6];
	   m_UUID.Data3 += pGuid[7]<<8;
	   for (int i = 0; i< 8; i++)
	   {
		   m_UUID.Data4[i] = *(pGuid+8+i);
	   }
	   Uuid2Str(m_UUID,m_szUUID);
	}
}
CUUID::CUUID(string strGuid)
{
}
CUUID::~CUUID()
{

}
////////
UUID& CUUID::GetUUID()
{
   return m_UUID;
}
bool CUUID::IsEqual(UUID& UUID2)
{
	if (m_UUID.Data1 != UUID2.Data1)
	{
		return false;
	}
	if (m_UUID.Data2 != UUID2.Data2)
	{
		return false;
	}
	if (m_UUID.Data3 != UUID2.Data3)
	{
		return false;
	}

	for (int i = 0; i<8; i++)
	{
		if (m_UUID.Data4[i] != UUID2.Data4[i])
		{
			return false;
		}
	}
	return true;
}
bool CUUID::IsEqual(string strUUID2)
{
	return true;
}

bool CUUID::IsEqual(const unsigned char* pszUUID2)
{
	if (strcmp(m_szUUID,(char*)pszUUID2) != 0)
	{
		return false;
	}
	return true;
}

char* CUUID::Uuid2Str(UUID &UUID, char* pStr)
{
	/*pStr[0] = ((BYTE*)(&UUID.Data1))[3];
	pStr[1] = ((BYTE*)(&UUID.Data1))[2];
	pStr[2] = ((BYTE*)(&UUID.Data1))[1];
	pStr[3] = ((BYTE*)(&UUID.Data1))[0];
	
	pStr[4] = ((BYTE*)(&UUID.Data2))[5];
	pStr[5] = ((BYTE*)(&UUID.Data2))[4];
	
	pStr[6] = ((BYTE*)(&UUID.Data3))[7];
    pStr[7] = ((BYTE*)(&UUID.Data3))[6];
*/
	pStr[0] = UUID.Data1>>24;
	pStr[1] = UUID.Data1>>16;
	pStr[2] = UUID.Data1>>8;
	pStr[3] = UUID.Data1;
	
	pStr[4] = UUID.Data2>>8;
	pStr[5] = UUID.Data2;
	
	pStr[6] = UUID.Data3>>8;
    pStr[7] = UUID.Data3;
   for (int i = 0; i<8; i++)
   {
	   pStr[8+i] = ((BYTE*)(&UUID.Data4))[i];
   }

   return (pStr);
}
