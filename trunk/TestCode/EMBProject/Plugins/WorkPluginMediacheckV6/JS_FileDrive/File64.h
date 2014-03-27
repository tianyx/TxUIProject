// File64.h: interface for the CFile64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE64_H__481D11AC_3115_4136_BE8F_ED2D3C1688EA__INCLUDED_)
#define AFX_FILE64_H__481D11AC_3115_4136_BE8F_ED2D3C1688EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFile64 : public CFile
{
public:
	CFile64();
    virtual ~CFile64();
public:
	
	// Attributes
	ULONGLONG GetPosition();
	
	
	// Overridables
	
	virtual ULONGLONG Seek(LONGLONG lOff, UINT nFrom);
	virtual void SetLength(ULONGLONG dwNewLen);
	ULONGLONG GetLength() ;
	
	virtual void LockRange(ULONGLONG dwPos, ULONGLONG dwCount);
	virtual void UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount);
	
	
};



#endif // !defined(AFX_FILE64_H__481D11AC_3115_4136_BE8F_ED2D3C1688EA__INCLUDED_)
