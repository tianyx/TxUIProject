/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TrTemplateItem.h
	file base:	TrTemplateItem
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include "PreDef.h"
#include "TxSerialize.h"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class CTrMonItem
{
public:
	CTrMonItem(void);
	~CTrMonItem(void);

public:
	//these function  convert db value to show value
	CString GetShownStrValue(const ENUM_TEMPCOL_TYPE typeIn) const;	//get value for shown
	BOOL	GetShownValue(VECSHOWNSTRING& vValOut)	const;				//get all info for shown.
	BOOL	SetShownValue(const ENUM_TEMPCOL_TYPE typeIn, const CString& strIn); //convert shown value to db value and set
	BOOL	SetShownValue(const VECSHOWNSTRING&  vValIn);	//convert shown value to db value and set

	CString	GetValue(const ENUM_MONTYPE typeIn);
	int		GetIntValue(const ENUM_MONTYPE typeIn);

	//for mon
	COLORREF GetMonColor();
	BOOL GetMonColor(DWORD& dwClrFont, DWORD& dwClrBk);
	ENUM_TRANSSTATE GetMonState();

	//for compare pgmId
	BOOL IsSamePGMID(CTrMonItem& other);


	//these function direct set db value
	BOOL	DirectSetValue(const ENUM_TEMPCOL_TYPE typeIn, const int nValIn);
	BOOL	DirectSetValue(const ENUM_TEMPCOL_TYPE typeIn, const time_t nValIn);
	BOOL	DirectSetValue(const VECSTRING&  vValIn);
	BOOL	DirectSetValue(const ENUM_TEMPCOL_TYPE typeIn, const CString& strIn); //force set value, no convert
	int		DirectGetIntValue(const ENUM_TEMPCOL_TYPE typeIn) const;	
	CString DirectGetValue(const ENUM_TEMPCOL_TYPE typeIn) const;	//
	BOOL	DirectGetValue(VECSTRING& vValOut)	const;	//get all info.
	bool operator==(const CTrMonItem& other) const;

public:
	BOOL operator <<(CTxSerialize& ar) const
	{
		return ar <<m_vData;

	}
	BOOL operator >>(CTxSerialize& ar)
	{
		return ar >> m_vData;

	}
private:
	VECSTRING m_vData;
};

//////////////////////////////////////////////////////////////////////////
//data for OleDataObject
struct ST_TRCPITEM
{
	int nPos;
	CTrMonItem item;
	ST_TRCPITEM()
	{
		nPos = -1;
	}
	BOOL operator <<(CTxSerialize& ar) const
	{
		return(
			ar << nPos
			&&	ar << item);

	}
	BOOL operator >>(CTxSerialize& ar) 
	{
		return(
			ar >> nPos
			&& ar >> item);

	}
};

typedef std::vector<ST_TRCPITEM> VECTRCPITEM;
struct ST_TRTMP_CLIPBOARD_DATA 
{
	VECTRCPITEM vItems;
	BOOL operator <<(CTxSerialize& ar) const
	{
		return ar << vItems;

	}
	BOOL operator >>(CTxSerialize& ar)
	{
		return ar >> vItems;

	}
};


bool IsEqualString(const CString& e1, const CString& e2);