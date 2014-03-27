// FileInfoExtract_TS.h: interface for the CFileInfoExtract_TS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_TS_H__11E7E6E7_309A_4714_B26F_CFE0E8DB1CF1__INCLUDED_)
#define AFX_FILEINFOEXTRACT_TS_H__11E7E6E7_309A_4714_B26F_CFE0E8DB1CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"

class CFileInfoExtract_TS : public CFileInfoExtract  
{
public:
	CFileInfoExtract_TS();
	virtual ~CFileInfoExtract_TS();
	virtual BOOL    AnalyzeFile();
protected:
	void GenerateGUID(GUID guid,CString &strGUID);
	int	GCD(int nLeft, int nRight); // greatest common divisor
};

#endif // !defined(AFX_FILEINFOEXTRACT_TS_H__11E7E6E7_309A_4714_B26F_CFE0E8DB1CF1__INCLUDED_)
