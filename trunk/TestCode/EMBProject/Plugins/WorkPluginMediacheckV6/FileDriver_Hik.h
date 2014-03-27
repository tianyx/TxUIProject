// FileDriver_Hik.h: interface for the CFileDriver_Hik class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_HIK_H__5AA33559_26CA_439F_A2E2_A4B6CD9EE677__INCLUDED_)
#define AFX_FILEDRIVER_HIK_H__5AA33559_26CA_439F_A2E2_A4B6CD9EE677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_Hik.h"

class CFileDriver_Hik : public CFileDriver  
{
public:
	CFileDriver_Hik();
	virtual ~CFileDriver_Hik();
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
	static UINT TskAudioCheckThread(LPVOID pParam);

private:
	CDriveMgr_Hik   m_DriverMgr_Hik;  //技审文件驱动
private:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);

};

#endif // !defined(AFX_FILEDRIVER_HIK_H__5AA33559_26CA_439F_A2E2_A4B6CD9EE677__INCLUDED_)
