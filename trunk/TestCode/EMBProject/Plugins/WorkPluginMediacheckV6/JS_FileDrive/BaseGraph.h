// BaseGraph.h: interface for the CBaseGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_H__871DEB6B_C80D_4E61_A589_7348451F82A3__INCLUDED_)
#define AFX_BASEGRAPH_H__871DEB6B_C80D_4E61_A589_7348451F82A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "SampleGrabberCB.h"
#include "AudioGrabberCB.h"
#include "CallBack.h"


//#define ONE_SECOND 10000000L
#define SAFE_RELEASE(p) if(p){ p->Release(); p = NULL; }
#define MAX_FILEPATH    512
class _JS_Fix CBaseGraph  
{
public:
	HRESULT  GetRate(double *dRate);//播放速率
	virtual LONGLONG GetDuration();         //时长单位可以在继承函数中定义
	virtual HRESULT  GetSrcStreamMedaiType(int nStream,AM_MEDIA_TYPE* AM);
	virtual int      GetSrcStreamCount();

	HRESULT RenderFile();
	CBaseGraph();
	CBaseGraph(HWND ParentHwnd)	;
	virtual ~CBaseGraph();
	// 设置文件路径
	HRESULT LoadFile(LPCTSTR lpszPathName, DWORD dwRetryTimes, DWORD dwRetryInterval);
	
	HRESULT	LoadFile(IFileSourceFilter* pFileSourceFilter, const char* strFileName); // wang.zh add

	HRESULT SetBuffer(void *pA);
	HRESULT SetAudioBuffer(void *pA);
    BOOL    SetParent(HWND ParentHwnd);
	HRESULT  GetVideoMediaType(AM_MEDIA_TYPE* mt);
	HRESULT  GetAudioMediaType(AM_MEDIA_TYPE* mt);
	//任务控制
	HRESULT Run();
	HRESULT Pause();
	HRESULT Stop();
	HRESULT Cancel();//取消任务
	HRESULT MakeGraph();
//	GRAPH_STATE GetState();
protected:
	virtual HRESULT BuildGrabberGraph();

	
//protected:
	HRESULT RemoveFilterGraph(IGraphBuilder* pGB);
    HRESULT NukeDownstream(IGraphBuilder *pGB,IBaseFilter *pf);


	HRESULT EnumdPins( IBaseFilter* pFilter, PIN_DIRECTION direction, IPin **pPin ,ULONG cPins=1);
	HRESULT GetUnconnectedPin(IBaseFilter *pFilter,	PIN_DIRECTION PinDir,IPin **ppPin);
	HRESULT ConnectFilter(IGraphBuilder *pGB,IBaseFilter* pfirst,IBaseFilter* pNext,BOOL bDirect = FALSE);
	HRESULT AddSourceFilter2Graph(IGraphBuilder *pGB,IBaseFilter** pfileReader,const char* strFileName);
	HRESULT AddFilter2GraphByClassID(IGraphBuilder *pGB,REFCLSID clsid,IBaseFilter** pFilter,const char* Name);
    HRESULT AddLocalFilter2GraphByClassID(IGraphBuilder *pGB,REFCLSID clsid,IBaseFilter** pFilter,const char* Name);//zibj
	HRESULT AddFilter2GraphByFriendlyName(IGraphBuilder *pGB,const char* FriendlyName,IBaseFilter** pFilter);
	HRESULT GetMediaControl(IGraphBuilder *pFG,IMediaControl **pMediaControl);
	HRESULT GetMediaSeeking(IGraphBuilder *pFG,IMediaSeeking **pMediaSeeking);
	HRESULT AddDMO2GraphByClassID(IGraphBuilder *pGB,REFCLSID clsid,REFCLSID CategoryGuid,IBaseFilter** pFilter,const char* Name);

	HRESULT SendEvent(long Event,long Param1,long Param2);

	HRESULT SetVideoGrabber();
	HRESULT SetAudioGrabber();
	HRESULT CleanUp(long lParam1 = 0,long lParam2 = 0);
	void    Release();

	void ErrMsg(LPTSTR szFormat,...);

	static DWORD __stdcall EventProc(LPVOID lpvoid);
protected:
	typedef enum _GRAPH_STATE
	{
		State_Empty,
		State_Building,
		State_Builded
	}GRAPH_STATE;
	GRAPH_STATE         m_graphState;
	DWORD               m_reg;
	// 源文件路径
	char                m_filePath[MAX_FILEPATH];

	CSampleGrabberCB    m_SampleGrabberCB;
	CAudioGrabberCB     m_AudioGrabberCB;

	// graphc manger
	IGraphBuilder		*m_pGraphBuilder;
	IMediaControl		*m_pMediaControl;
	IMediaEvent         *m_pMediaEvent;
	IMediaEventSink     *m_pMediaEventSink;
	IMediaSeeking		*m_pMediaSeeking;
	//ISampleGrabber      *m_pSampGrabber;

	// file source
	IBaseFilter			*m_pFileSrcF;
	IBaseFilter         *m_pSampGrabberF;
	IBaseFilter         *m_pAudioGrabberF;
	IBaseFilter         *m_pParserFilter;
	IBaseFilter         *m_pVDecoder;
	IBaseFilter         *m_pADecoder;
    IBaseFilter         *m_pVNullRenderF;
	IBaseFilter         *m_pANullRenderF;
    IBaseFilter			*m_pColorSpaceFilter;
//	CCritSec            m_RenderCritSec;
	HWND                m_ParentHWND;
	//	bool                m_bCancelEventThread;
    HANDLE              m_hEventTreadHandle;

	//CCritSec             m_ConnectCritSec;
protected:

	DWORD				m_dwRetryTimes;
	DWORD				m_dwRetryInterval;
public:
	JS_CCallBack*       m_pMsgCallBack;//chenAdd for Detect

//zibj filter本地化动态加载
protected:
	class CMapClsidToDll;
	static CMapClsidToDll* m_pMapClsidToDll;

	class CLocalDlls;
	static CLocalDlls* m_pLocalDlls;

	static BOOL m_bAllLocalDllLoaded;
	static CCritSec m_LocalDllLock;

public:
	static void ClearLocalDlls();
//zibj end
public:
	BOOL seekTo(LONGLONG som,LONGLONG duration);
};

#endif // !defined(AFX_BASEGRAPH_H__871DEB6B_C80D_4E61_A589_7348451F82A3__INCLUDED_)
