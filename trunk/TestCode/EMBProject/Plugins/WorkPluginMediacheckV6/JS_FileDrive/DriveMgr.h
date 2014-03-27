// DriveMgr.h: interface for the CDriveMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_H__F0DBA477_7063_42C3_B53A_FCEC2AE9A53B__INCLUDED_)
#define AFX_DRIVEMGR_H__F0DBA477_7063_42C3_B53A_FCEC2AE9A53B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "FileInfoExtract.h"
#include "BaseGraph.h"
//#include "Common.h"
#include "CallBack.h"
#include <afxmt.h>

class _JS_Fix CDriveMgr  
{
public:
	HANDLE   CreateThread(LPTHREAD_START_ROUTINE lpStartAddress,LPDWORD ThreadID);
	HRESULT  SetCallBack(JS_CCallBack *lpCallFun);
	HRESULT  SetBuffer(void *pA);
	HRESULT  SetAudioBuffer(void *pA);
	HRESULT  GetVideoMediaType(AM_MEDIA_TYPE* mt);
	HRESULT  GetAudioMediaType(AM_MEDIA_TYPE* mt);

	HANDLE   StartDecodeThread();
	HANDLE   StartInfoCheckThread();
	
	HRESULT OpenFile(LPCTSTR lpszPathName);
	BOOL    SetParent(HWND ParentHwnd);
	BOOL    Run(void);
	BOOL    Pause(void);
	BOOL    Stop(void);
	BOOL    Cancel(void);
	void    seekTo(LONGLONG som,LONGLONG duration);
	void	SetRetryTimes(DWORD dwTimes, DWORD dwInterVal);

	CDriveMgr();
	CDriveMgr(HWND ParentHwnd,CBaseGraph *pbasegraph,CFileInfoExtract *pfileIE);
	virtual ~CDriveMgr();
protected:
	static DWORD __stdcall InfoCheckThread(LPVOID lpParameter);
	static DWORD _stdcall DecoderThread(LPVOID lpParameter);
protected:
	HWND               m_ParentHwnd;//接收消息

	CFileInfoExtract   *m_pBasefInfoEt;
	CBaseGraph         *m_pBaseGraph;
	JS_CCallBack       *m_lpCallFunAdr;  //回调函数地址指针
	JS_StreamInfo      m_StreamInfo;     //回调传递参数
	std::string        m_strFilePath;    //文件路径
	DWORD              m_ThreadID[2];
	HANDLE             m_ThreadHandle[2];
	DWORD			   m_dwRetryTimes;	
	DWORD			   m_dwInterVal;	
//	CEvent             m_CancelDecoderTEvent;
//	CEvent             m_CancelInfoCheckTEvent;
	CRITICAL_SECTION   m_CritSect;      // 临界区


};

#endif // !defined(AFX_DRIVEMGR_H__F0DBA477_7063_42C3_B53A_FCEC2AE9A53B__INCLUDED_)
