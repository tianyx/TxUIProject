// FileDriver_TS.h: interface for the CFileDriver_TS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_TS_H__F1C0FE6C_F909_4243_85EA_917C1FE2BF9C__INCLUDED_)
#define AFX_FILEDRIVER_TS_H__F1C0FE6C_F909_4243_85EA_917C1FE2BF9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_TS.h"

class CFileDriver_TS : public CFileDriver 
{
public:
	CFileDriver_TS();
	virtual ~CFileDriver_TS();
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
private:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);
    bool InitDetectParam();
public:
	CString m_strMsg;
private:
	CDriveMgr_TS    m_DriverMgr_TS;  //技审文件驱动
public:
	static UINT TskCheckThread(LPVOID pParam);
	static UINT TskAudioCheckThread(LPVOID pParam);
};

#endif // !defined(AFX_FILEDRIVER_TS_H__F1C0FE6C_F909_4243_85EA_917C1FE2BF9C__INCLUDED_)
