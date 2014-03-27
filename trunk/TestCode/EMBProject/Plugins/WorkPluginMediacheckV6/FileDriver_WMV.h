// FileDriver_WMV.h: interface for the CFileDriver_WMV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_WMV_H__8BE08578_E85A_4ED4_8D76_3EDBED1998CD__INCLUDED_)
#define AFX_FILEDRIVER_WMV_H__8BE08578_E85A_4ED4_8D76_3EDBED1998CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDriver.h"
#include ".\JS_FileDrive\DriveMgr_WMV.h"
class CFileDriver_WMV : public CFileDriver  
{
public:
	CFileDriver_WMV();
	virtual ~CFileDriver_WMV();
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
protected:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);
    bool InitDetectParam();
public:
	CString m_strMsg;
private:
	CDriveMgr_WMV    m_DriverMgr_WMV;  //技审文件驱动
};

#endif // !defined(AFX_FILEDRIVER_WMV_H__8BE08578_E85A_4ED4_8D76_3EDBED1998CD__INCLUDED_)
