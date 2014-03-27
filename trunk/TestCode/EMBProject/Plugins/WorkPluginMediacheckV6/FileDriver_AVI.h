// FileDriver_AVI.h: interface for the CFileDriver_AVI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_AVI_H__85CCD014_4286_4568_91B4_A14118FA1466__INCLUDED_)
#define AFX_FILEDRIVER_AVI_H__85CCD014_4286_4568_91B4_A14118FA1466__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_AVI.h"

class CFileDriver_AVI : public CFileDriver  
{
public:
	CFileDriver_AVI();
	virtual ~CFileDriver_AVI();
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
	CDriveMgr_AVI   m_DriverMgr_AVI;  //技审文件驱动
private:
	HRESULT GetFileInfo(JS_StreamInfo p);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE);
};

#endif // !defined(AFX_FILEDRIVER_AVI_H__85CCD014_4286_4568_91B4_A14118FA1466__INCLUDED_)
