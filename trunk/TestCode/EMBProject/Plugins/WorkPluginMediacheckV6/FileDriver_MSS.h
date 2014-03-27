// FileDriver_MSS.h: interface for the CFileDriver_MSS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVER_MSS_H__1C4FB5B3_E07B_4AEE_AC85_A4BF82586D0D__INCLUDED_)
#define AFX_FILEDRIVER_MSS_H__1C4FB5B3_E07B_4AEE_AC85_A4BF82586D0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_MSS.h"

class CFileDriver_MSS : public CFileDriver  
{
public:
	CFileDriver_MSS();
	virtual ~CFileDriver_MSS();
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
	CDriveMgr_MSS   m_DriverMgr_MSS;  //技审文件驱动
public:
	static UINT TskCheckThread(LPVOID pParam);
	static UINT TskAudioCheckThread(LPVOID pParam);

};

#endif // !defined(AFX_FILEDRIVER_MSS_H__1C4FB5B3_E07B_4AEE_AC85_A4BF82586D0D__INCLUDED_)
