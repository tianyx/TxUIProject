// FileDriver_Leitch.h: interface for the CFileDriver_Leitch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_LEITCH_H__BF3707D0_8BB7_4553_AA5C_8949704C0BB7__INCLUDED_)
#define AFX_FILEDRIVER_LEITCH_H__BF3707D0_8BB7_4553_AA5C_8949704C0BB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_LXF.h"
class CFileDriver_Leitch : public CFileDriver
{
public:
	CFileDriver_Leitch();
	virtual ~CFileDriver_Leitch();
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
	static UINT TskCheckThread(LPVOID pParam);
	static UINT TskAudioCheckThread(LPVOID pParam);
private:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);
public:
	CString m_strMsg;
private:
	CDriveMgr_LXF   m_DriverMgr_LXF;  //技审文件驱动
};

#endif // !defined(AFX_FILEDRIVER_LEITCH_H__BF3707D0_8BB7_4553_AA5C_8949704C0BB7__INCLUDED_)
