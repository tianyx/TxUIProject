// UUID.h: interface for the CGUID class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUID_H__27165CB0_5483_4220_ACCE_EDF998FA7938__INCLUDED_)
#define AFX_GUID_H__27165CB0_5483_4220_ACCE_EDF998FA7938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace std;

class CUUID //:public UUID 
{
public:
	char* Uuid2Str(UUID& UUID,char* pStr);
	//CUUID();
	CUUID(const BYTE* pGuid = NULL);
	CUUID(string strGuid);
	virtual ~CUUID();
    //
	UUID& GetUUID();
	bool IsEqual(UUID& UUID2);
	bool IsEqual(string strUUID2);
	bool IsEqual(const unsigned char* pszUUID2);

protected:
	std::string m_strGUID;
	UUID        m_UUID;
	char        m_szUUID[16];

};

#endif // !defined(AFX_GUID_H__27165CB0_5483_4220_ACCE_EDF998FA7938__INCLUDED_)
