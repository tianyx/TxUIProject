// FileDriver_MOV.h: interface for the CFileDriver_MOV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_OMN_H__FF2D9A44_BD90_4508_B352_8651F024006D__INCLUDED_)
#define AFX_FILEDRIVER_MOV_H__FF2D9A44_BD90_4508_B352_8651F024006D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_MOV.h"

class CFileDriver_MOV : public CFileDriver 
{
public:
	CFileDriver_MOV();
	virtual ~CFileDriver_MOV();
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
public:
	static int nNum;
	CString m_strMsg;
private:
	CDriveMgr_MOV   m_DriverMgr_MOV;  //技审文件驱动
public:
	static UINT TskCheckThread(LPVOID pParam);
	static UINT TskAudioCheckThread(LPVOID pParam);
};

#endif // !defined(AFX_FILEDRIVER_MOV_H__FF2D9A44_BD90_4508_B352_8651F024006D__INCLUDED_)
