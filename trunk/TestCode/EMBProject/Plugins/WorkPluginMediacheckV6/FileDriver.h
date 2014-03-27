#if !defined(AFX_FILEDRIVER_H__A1970779_343E_48C1_9D0A_BB99BE849AD5__INCLUDED_)
#define AFX_FILEDRIVER_H__A1970779_343E_48C1_9D0A_BB99BE849AD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDriver.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileDriver window
#include "FCVSServerConfig.h"
#include "GeneralDef.h"
#include "XMLOp.h"
#include <afxmt.h>
#include ".\JS_FileDrive\DriveMgr.h"
#include ".\Detect DLL v1.11\AutoDetect.h"

#define WM_DS_EC_COMPLETE                  WM_USER+1
#define WM_DS_EC_PAUSED                    WM_USER+2
#define WM_DS_EC_SHUTTING_DOWN             WM_USER+3
#define WM_DS_EC_STOP                      WM_USER+4
#define WM_DS_EC_REPAINT                   WM_USER+5
#define WM_DS_EC_EC_SNDDEV_IN_ERROR        WM_USER+6
#define WM_DS_EC_EC_SNDDEV_OUT_ERROR       WM_USER+7
#define WM_DS_EC_STREAM_ERROR_STILLPLAYING WM_USER+8
#define WM_DS_EC_STREAM_ERROR_STOPPED      WM_USER+9
#define WM_DS_EC_USERABORT                 WM_USER+10

#define WM_DS_EC_USE_CLEANUP               WM_USER+50
#define WM_DS_EC_USE_UNRENDER              WM_USER+51
//发给上层的分析文件消息
#define WM_IC_UNRECOG                      WM_USER+100 //文件不能分析

#define NUM_OUT_TIME                       10000

enum DRIVER_RETURN_TYPE
{
	DR_NOT_RETURN = 0,
	DR_NOMAL_FINISHED,
	DR_PAUSE,
	DR_RUN,
	DR_STOP,
	DR_USER_CANCEL,
	DR_ERROR_ABORT
};

struct tagDetectFileInfo 
{
	CString m_strenFiledName;
	CString m_strcnFiledName;
	CString m_strFiledValue;
};

struct tagDetectResult
{
	LONGLONG m_lFrame;
	DETECT_TYPE m_DetectType;
};

struct tagAudioDetectResult
{
	LONGLONG m_lFrameStart;
	LONGLONG m_lDuration;
	DETECT_TYPE m_DetectType;
};

struct tagAnlysisResult 
{
	DETECT_TYPE m_DetectType;
	CString m_strDetectTypeName;
	CString m_strTimeCodeStart;
	CString m_strTimeCodeEnd;
	CString m_strTimeCodeDuration;
	LONGLONG m_nFrameStart;
	LONGLONG m_nFrameEnd;
	LONGLONG m_nDuration;
};

class CDetectResult
{
public:
	CDetectResult();
	virtual ~CDetectResult();
public:
	BOOL AppendFileInfo(CString strFiledValue,CString strenFiledName,CString strcnFiledName = "");
	BOOL CleanFileInfo();
	BOOL AppendDetectResult(LONGLONG lFrame,DETECT_TYPE DetectType);
	BOOL AppendAudioDetectResult(LONGLONG lFrameStart, LONGLONG lDuration, DETECT_TYPE DetectType);
	BOOL CleanDetectResult();
	CString GetDetectTypeName(DETECT_TYPE DetectType);
	CString GetTimeCodeString(LONGLONG lFrame);
public:
	void AnlysisResult();
public:
	tagResultLevel GetLowestLevel();
	CArray<tagDetectResult,tagDetectResult> m_DetectResultArr;
	CArray<tagAudioDetectResult,tagAudioDetectResult> m_ADetectResultArr;
	CArray<tagDetectFileInfo,tagDetectFileInfo> m_DetectFileInfoArr;
	CArray<tagAnlysisResult,tagAnlysisResult> m_AnlysisResultArr;
	CString m_strLevel;
	tagResultLevel m_LowestLevel;
	LONGLONG m_som;
private:
	CString AnlysisVideoResult();
	CString AnlysisAudioResult();
};

class CFileDriver : public CWnd,public JS_CCallBack,public ResultCB
{
// Construction
public:
	CFileDriver(CDriveMgr* pDriver);
	
public:
	CString m_strTskID;
	CString m_strCheckKind;
	CString m_strFileType;
    CString m_strClipID;
	CString m_strClipName;
	CString m_strFilePath;
	CString m_strCheckItem;
	CString m_strSaveXmlPath; // 保存结果xml的路径
	CString m_strClipType;//wangjun add for AutoDetect Dyn Init
	int     m_nPRI;
	int     m_nCheckOpt;

	CDetectResult m_DetectResult;
	//static int g_bIsWriteResultToDB;//wangjun 20081018

protected:
	HANDLE  m_hTaskThread;
	HANDLE  m_hAThread;
	HANDLE  m_evAThreadEvent;
	HANDLE  m_evAThreadExit;
	HANDLE  m_evThreadExit;
    HANDLE  m_evFileInfoComplete;
	CString m_strServiceNameOp;
	HANDLE  m_ThreadEvent;
	int     m_nStatus;
	CString m_strLastErrorInfo;
	HWND    m_hParentWnd;
	DWORD   m_startTime;

	LONGLONG nCount;
	DRIVER_RETURN_TYPE m_uReturnType;
	BOOL    m_bSubmit;
	JS_StreamInfo   m_StreamInfo;     //技审文件信息
	CDriveMgr* m_pFileDriver;
private:
	int nPos;
//	mutable CRITICAL_SECTION m_DBLock;
	mutable CRITICAL_SECTION m_Lock;
	inline void Lock()const {::EnterCriticalSection(&m_Lock);};
	inline void UnLock()const {::LeaveCriticalSection(&m_Lock);};
	HRESULT OnDecodeMessage(UINT Msg,WPARAM wParam,LPARAM lParam);
	BOOL AnalysisResult();
	CString GetLastAnlysisResultAsXmlString();
	BOOL SaveFileInfoToDB();
	BOOL SaveResultToDB();
	bool SetDBSpecialValue(CString TaskID,CString ItemName,int nItemValue);
	bool SetDBSpecialValue(CString TaskID,CString ItemName, CString strItemValue);
public:
	virtual void SetTaskStatus(int nStatus);
	virtual void SetTaskErrInfo(CString strError);
	virtual CString GetLastErrInfo();
	virtual int GetCurrentStatus();
	virtual LONGLONG GetFileDuration() = 0;

	void SetRetryInfo(DWORD dwRetryTimes, DWORD dwRetryInterval);
	void SetReturnType(DRIVER_RETURN_TYPE nType){Lock();m_uReturnType=nType;UnLock();};
	virtual void OnUpdateProgress();

protected:
	virtual STDMETHODIMP_(void)ProcessResult(DETECT_TYPE Result,long Reserved);
	virtual STDMETHODIMP_(void)ProcessResultAudio(DETECT_TYPE Result, __int64 nStart, __int64 nDuration, __int64 qReserved);
	virtual STDMETHODIMP_(void)AudioCount(__int64 count);
	virtual STDMETHODIMP_(void)VideoCount(long count);
	virtual BOOL GetFileInfoByArr(CStringArray& strFileInfoFiledArr,CStringArray& strFileInfoValueArr,BOOL bXML = TRUE) = 0;
	void OnGetFileInfoFinished();
	void ReportLog(UINT iType,CString userMsg= "");
public:
	virtual void __stdcall ShowMsg() = 0;
    virtual BOOL __stdcall InitTask(CString strXML) = 0;
	virtual BOOL __stdcall RunTask() = 0;
	virtual BOOL __stdcall PauseTask() = 0;
	virtual BOOL __stdcall CancelTask() = 0;
	virtual CString __stdcall GetTskID() = 0;
	virtual void __stdcall SetParentWnd(HWND hwnd) = 0;
	virtual void Submit() = 0;

// Attributes
public:
	CStringArray strSQLArr;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDriver)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	long CheckItemForStr2L(CString strCheckItem);
	virtual ~CFileDriver();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFileDriver)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CSList          m_SampleList;     //文件帧列表
	CSList          m_AudioSampleList;//文件音频数据列表

	AutoDetect*    m_pAutoDetect;     //技审核心检测

//技审切片
public:
	BOOL isUseSectionFuction;
	LONGLONG m_TaskSOM;
	LONGLONG m_TaskDuration;
	//LONGLONG m_TaskEOM
	CString m_TaskSectionID;
	long m_VideoCount;
	long m_AudioCount;
	int m_totalSectionCount;
	void SeekTo(LONGLONG som,LONGLONG duration); 
	int nDiv;
	BOOL isSetSectionEnd;
	BOOL isSectionEnd();
	void endSection();
};

// class IFileDriver  
// {
// public:
// 	virtual void __stdcall ShowMsg() = 0;
//     virtual BOOL __stdcall InitTask(CString strXML) = 0;
// 	virtual BOOL __stdcall RunTask() = 0;
// 	virtual BOOL __stdcall PauseTask() = 0;
// 	virtual BOOL __stdcall CancelTask() = 0;
// 	virtual CString __stdcall GetTskID() = 0;
// 	virtual void __stdcall SetParentWnd(HWND hwnd) = 0;
// };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDRIVER_H__A1970779_343E_48C1_9D0A_BB99BE849AD5__INCLUDED_)
