// FileDrive_MP4.h: interface for the CFileDrive_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVE_MP4_H__05F02953_2A9D_4F88_8A3B_A23780A35580__INCLUDED_)
#define AFX_FILEDRIVE_MP4_H__05F02953_2A9D_4F88_8A3B_A23780A35580__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_MP4.h"

class CFileDriver_MP4 : public CFileDriver  
{
public:
	CFileDriver_MP4();
	virtual ~CFileDriver_MP4();
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
	CDriveMgr_MP4    m_DriverMgr_MP4;  //技审文件驱动
public:
	static UINT TskCheckThread(LPVOID pParam);
	static UINT TskAudioCheckThread(LPVOID pParam);
};

#endif // !defined(AFX_FILEDRIVE_MP4_H__05F02953_2A9D_4F88_8A3B_A23780A35580__INCLUDED_)
