// FileDriver_MPG.h: interface for the CFileDriver_MPG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_MPG_H__C1C95083_10FC_4561_87A7_D73FD0007A43__INCLUDED_)
#define AFX_FILEDRIVER_MPG_H__C1C95083_10FC_4561_87A7_D73FD0007A43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_MPG.h"

class CFileDriver_MPG : public CFileDriver  
{
public:
	CFileDriver_MPG();
	virtual ~CFileDriver_MPG();
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
	CDriveMgr_MPG   m_DriverMgr_MPG;  //技审文件驱动
private:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);

};

#endif // !defined(AFX_FILEDRIVER_MPG_H__C1C95083_10FC_4561_87A7_D73FD0007A43__INCLUDED_)
