// FileDriver_M2V.h: interface for the CFileDriver_M2V class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_M2V_H__04BCC45B_2DE7_4DF6_BAB3_71AD484EE64B__INCLUDED_)
#define AFX_FILEDRIVER_M2V_H__04BCC45B_2DE7_4DF6_BAB3_71AD484EE64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDriver.h"
#include ".\JS_FileDrive\DriveMgr_M2V.h"

class CFileDriver_M2V : public CFileDriver  
{
public:
	CFileDriver_M2V();
	virtual ~CFileDriver_M2V();
	virtual void __stdcall ShowMsg();
	virtual BOOL __stdcall InitTask(CString strXML);
	virtual BOOL __stdcall RunTask();
	virtual BOOL __stdcall PauseTask();
	virtual BOOL __stdcall CancelTask();
	virtual CString __stdcall GetTskID();
	virtual void __stdcall SetParentWnd(HWND hwnd);
	virtual LONGLONG GetFileDuration();
	virtual void __stdcall SetServiceName(CString strServiceNameOp);
	virtual void Submit();
public:
	static UINT TskCheckThread(LPVOID pParam);
public:
	static int m_nNum;
	CString m_strMsg;
private:
	CDriveMgr_M2V   m_DriverMgr_M2V;  //技审文件驱动
private:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);

};

#endif // !defined(AFX_FILEDRIVER_M2V_H__04BCC45B_2DE7_4DF6_BAB3_71AD484EE64B__INCLUDED_)
