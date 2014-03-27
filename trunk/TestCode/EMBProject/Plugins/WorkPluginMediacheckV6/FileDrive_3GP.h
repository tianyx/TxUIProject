// FileDrive_3GP.h: interface for the CFileDrive_3GP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDRIVE_3GP_H__7437F624_1981_4131_A507_41382200E8C7__INCLUDED_)
#define AFX_FILEDRIVE_3GP_H__7437F624_1981_4131_A507_41382200E8C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileDriver.h"
#include "FCVSServerConfig.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr_3GP.h"

class CFileDriver_3GP : public CFileDriver  
{
public:
	CFileDriver_3GP();
	virtual ~CFileDriver_3GP();
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
	CDriveMgr_3GP    m_DriverMgr_3GP;  //技审文件驱动
public:
	static UINT TskCheckThread(LPVOID pParam);
	static UINT TskAudioCheckThread(LPVOID pParam);
};

#endif // !defined(AFX_FILEDRIVE_3GP_H__7437F624_1981_4131_A507_41382200E8C7__INCLUDED_)
