// AutoDetect

// for FCVS founder.

// Be Sure You 've read these pieces carefully

//2008-9-4 by wang.zh

//1.alter for multiple instance
//2.add a static function <DestroyDefaultOptionMap> which should be called when application shutting down 
//	aimed at cleaning up the default option map
//3.Add functions <AddRef>,<Release> for Instances' lifecycle management 
//	invoke <AddRef> for maintain instance and <Release> for release instance.
//	Instance suicides when ref = 0 
//4.Add functions <AssertValid>,<Dump> for debug purpose.
//5.XML Configuration items <TOP_SAFE_SECTION>,<BOTTOM_SAFE_SECTION>,<LEFT_SAFE_SECTION> and <RIGHT_SAFE_SECTION>
//	are altered to <TOP_SECURE_SECTION>,<BOTTOM_SECURE_SECTION>,<LEFT_SECURE_SECTION> and <RIGHT_SECURE_SECTION>
//  (Pay attention to SAFE-->SECURE).

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AUTODETECT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AUTODETECT_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#pragma once
#pragma warning(disable:4251)

#ifdef AUTODETECT_EXPORTS
#define AUTODETECT_API __declspec(dllexport)
#else
#define AUTODETECT_API __declspec(dllimport)
#endif

#include <afxcmn.h>
#include <wtypes.h>
#include <objbase.h>
#include <tchar.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <math.h>

#include "OptionMap.h"
#include "define.h"
#include "CTimer.h"
#include "polyloud.h"
#include "loudclass.h"

#include <afxtempl.h>
#include "volume.h"

#ifndef _detect_type_
#define _detect_type_

typedef long DETECT_TYPE;

#define	DETECT_TYPE_BLACK_SCENE				0x00000001L
#define	DETECT_TYPE_COLOR_STRIP				0x00000002L
#define	DETECT_TYPE_COLOR_SCENE				0x00000004L
#define	DETECT_TYPE_STATIC_FRAME			0x00000008L

#define DETECT_TYPE_AUDIO_LOW				0x00010000L	
#define DETECT_TYPE_AUDIO_MUTE				0x00020000L
#define DETECT_TYPE_AUDIO_HIGH				0x00040000L

#endif

#define YUY2_BYTES_PER_PIXEL				2
#define YV12_BYTES_PER_PIXEL				1.5f
#define STRIP_COUNTS						8
#define STRIP_WIDTH							( *CLIP_WIDTH.value >> 3 )

#ifndef CPUID_TEST
#define CPUID_TEST

#define MM_MMX			0x0001 /* standard MMX */
#define MM_3DNOW		0x0004 /* AMD 3DNOW */
#define MM_MMXEXT		0x0002 /* SSE integer functions or AMD MMX ext */
#define MM_SSE			0x0008 /* SSE functions */
#define MM_SSE2			0x0010 /* PIV SSE2 functions */
#define MM_3DNOWEXT		0x0020 /* AMD 3DNowExt */

#endif


#ifndef __MEM_COPY__
#define __MEM_COPY__
typedef void (*pfnMemcpy)(void* dst,const void* src, int size);
#endif

#define DATABASE_LOG
#define FILE_LOG



//callback interface;
DECLARE_INTERFACE(ResultCB) 
{
	STDMETHOD_(void,ProcessResult)(DETECT_TYPE Result,long nFrame) PURE;
	STDMETHOD_(void,ProcessResultAudio)(DETECT_TYPE Result, __int64 nStart, __int64 nDuration, __int64 qReserved) PURE;
	STDMETHOD_(void,AudioCount)(__int64 count) PURE;
	STDMETHOD_(void,VideoCount)(long count) PURE;
};

// typedef struct _tagAudioType
// {
// 	int nChannels;
// 	int nSamplingRate;
// 	int nBit;
// 	int nBlockAlign;
// 	_tagAudioType::_tagAudioType():nChannels(0),nSamplingRate(0),nBit(0), nBlockAlign(0){};
// } ADAudioType;

typedef struct _tagAudioInfo
{
	__int64 nStart;
	__int64 nDuration;
	__int64 nResevred;
	_tagAudioInfo::_tagAudioInfo():nStart(0), nDuration(0), nResevred(0) {};
} ADAudioInfo;
typedef struct _tagAudioRuler
{
	__int64 nThreshold;
	long		nPeriod;
	_tagAudioRuler::_tagAudioRuler():nThreshold(0),nPeriod(0){};
}ADAudioRuler;

// This class is exported from the AutoDetect.dll
class AUTODETECT_API AutoDetect : public iVolumeCalReport
{
public:

	AutoDetect();	
	virtual ~AutoDetect(void);
protected:
	
	long count ;

	DETECT_TYPE m_detectType;
	DETECT_TYPE m_VideoResult;
	DETECT_TYPE	m_AudioResult;

	CTimer timer[5];
	DWORD nTBlack,nTColor,nTstatic,nTStrip,nTColorCast;
	//some assist variable
		
	//analyze Frame;
	long lineHash[3][256],	maxPixelCount[3];
	int lineHashCursor, maxPixelCursor[3];
		
	//color_strip;
	int stripUHash[256],maxUCount,maxUCursor;
	int stripHashCursor,pixelCursor,stripCursor;


	//frame buffer;
	BYTE* pFrameBuffer;
	BYTE* m_pFrameDiff;

	ADAudioType m_mtAudioType;
	ADAudioInfo	m_aiLow;
	ADAudioInfo	m_aiMute;
	__int64		m_nCurrentAudio;
	ADAudioRuler	m_Audio_Low_Threshold;
	ADAudioRuler	m_Audio_High_Threshold;
	ADAudioRuler	m_Audio_Mute_Threshold;

		
	ResultCB*	SendResult;

	DECLARE_OPTION_MAP()
	DECLARE_DEFAULT_CONFIG_MAP()

protected:	
	DECLARE_OPTION(int,		YUV_FORMAT)
	DECLARE_OPTION(int,		CLIP_WIDTH)
	DECLARE_OPTION(int,		CLIP_HEIGHT)
	DECLARE_OPTION(float,	TOP_SECURE_SECTION)
	DECLARE_OPTION(float,	BOTTOM_SECURE_SECTION)
	DECLARE_OPTION(float,	LEFT_SECURE_SECTION)
	DECLARE_OPTION(float,	RIGHT_SECURE_SECTION)
	DECLARE_OPTION(int,		BLACK_SCENE_THRESHOLD)
	DECLARE_OPTION(float,	COLOR_SCENE_PERCENTAGE)
	DECLARE_OPTION(float,	COLOR_STRIP_PERCENTAGE)
	DECLARE_OPTION(float,	STATIC_FRAME_PERCENTAGE)
	DECLARE_OPTION(int,		LINE_COUNTS_FOR_STRIP_DETECT)
	DECLARE_OPTION(int,		AMBIT_DEEMED_TO_SAME_PIXEL)
	DECLARE_OPTION(int,		UNDULATE_AMBIT_OF_Y)
	DECLARE_OPTION(int,		UNDULATE_AMBIT_OF_U)
	DECLARE_OPTION(int,		UNDULATE_AMBIT_OF_V)
	DECLARE_OPTION(int,		AUDIO_LOW_THRESHOLD)
	DECLARE_OPTION(float,	AUDIO_LOW_PERIOD)
	DECLARE_OPTION(int,		AUDIO_HIGH_THRESHOLD)
	DECLARE_OPTION(float,	AUDIO_HIGH_PERIOD)
	DECLARE_OPTION(int,		AUDIO_MUTE_THRESHOLD)
	DECLARE_OPTION(float,	AUDIO_MUTE_PERIOD)
	DECLARE_OPTION(double,     LOUDNESS_LOW_THRESHOLD)
	DECLARE_OPTION(double,     LOUDNESS_HIGH_THRESHOLD)
protected:
	void VideoReset();
	void AudioReset();
	void saveStr(LPSTR str);
	void ProcessVideoResult();
	void ProcessAudioResult();
	BYTE  __forceinline GetMed(BYTE* tem,DWORD tWidth,DWORD tHeight);

	virtual void OnConfigChanged();
public:
	//音量返回接口
	void reportVolumeRe(int errorInfo,int errorDuration,int nRelativeStart);
	VolumeCal volumeCalre;
	//config
	void SetConfig(PARAM_INDEX index,double value);
	void DefaultConfig();
	BOOL XMLConfig(LPCSTR strXMLTemp);

	//public functions 
	virtual BOOL Detect(BYTE* pBuffer, long nBufferLen) = 0;
	virtual BOOL DetectAudio(BYTE* pBuffer, long nBufferLen);
	void SetCallback(ResultCB* pCallback);
	void Initialize(void);
	void SetDetectType(DETECT_TYPE type);
	void SetAudioType(int nChannels, int nSampleRate, int nBit, int nBlockAlign);
public:
	static AutoDetect* CreateInstance(_YUV_FORMAT YUVtype = YUV_FORMAT_YUY2);
	static void	DestroyDefaultOptionMap();

public:
	double loudnessValue; //zibj 2011.07.21 响度值
	
	loudChoix loudCho;

	ADAudioInfo m_loudHigh;
	ADAudioInfo m_loudLow;

public:
#ifdef _DEBUG
#define ADOutputDbg LogWrite
#else
#define ADOutputDbg	(0)
#endif
	virtual void __cdecl LogWrite(LPCSTR strFormat,...);

public:
	virtual long AddRef();
	virtual long Release();
protected:
	long m_cRefCount;

#ifdef _DEBUG
public:
	virtual void AssertValid();
	virtual void Dump(CDumpContext& dc);
#endif

	enum AUDIO_HIGH_STASTU{ DETECT_STATUS_YES, DETECT_STATUS_CONTINUE, DETECT_STATUS_NO };
	AUDIO_HIGH_STASTU DetectAudioHigh(__int64* v);
	ADAudioInfo	m_aiHigh;
	class CAudioHighHelper
	{
	public:
		static const float __a__;
	private: 
		double m_dcoeff;
		long m_x;
		__int64 m_current;

	public:
		void Init(int freq)
		{
			m_current = 0;
			m_x = 0;
			m_dcoeff = __a__/(40.0f*freq*freq);
		}
		void reset(){m_x = 0;m_current = 0;}
		void Set(__int64* v){m_current = *v;};
		__int64 Next()
		{
			m_current = ( __int64 )( 1.0f / pow ( 10, m_dcoeff * ( 2 * m_x + 1 )) * m_current + 0.5);
			m_x++;
			return m_current;
		}

	}m_xAudioHighHelper;

	class CFreezeFrameHelper
	{
	public:
		enum{ xLevelMin = 1, xLevel1, xLevel2, xLevel3, xLevel4, xLevel5, xLevelMax = xLevel5, };
		
		struct CCluster
		{
			int m_nXmin,m_nXmax;
			int m_nYmin,m_nYmax;
			int m_nCount;
			int m_nLevel;
			float m_dPercentage;
		};
		
		CList<CCluster*, CCluster*> m_xClusters;	
		LONG m_nTotalPixels;
		
		public:
			CFreezeFrameHelper();
			~CFreezeFrameHelper();
		public:
			void CleanUp();
			int __forceinline GetLevel(int xx);
			int __forceinline Add(int x, int y, int nLevel);
			float __forceinline Filter(int nCount, int nLevel);
	}m_xFreezeFrameHelper;

	int m_nCpuType;
	pfnMemcpy m_pfnMemCpy;

};

struct Separator 
{
	DWORD data;
	struct Separator* next;
};


class  YUY2_Detect:public AutoDetect
{
public:
	YUY2_Detect()
	{
		SafeSetConfig(YUV_FORMAT,YUV_FORMAT_YUY2,int); 
	/*/
		if( m_nCpuType & MM_SSE )
		{
			m_pfnStaticFrameDetect = StaticFrameDetect_x86;
		}
		else if( m_nCpuType & MM_MMXEXT )
		{
			m_pfnStaticFrameDetect = StaticFrameDetect_x86;
		}
		else if( m_nCpuType & MM_MMX )
		{
			m_pfnStaticFrameDetect = StaticFrameDetect_x86;
		}
		else
		{
			m_pfnStaticFrameDetect = StaticFrameDetect_x86;
		}
		/*/
		if( m_nCpuType & MM_SSE )
		{
		m_pfnStaticFrameDetect = StaticFrameDetect_acc;
		}
		else if( m_nCpuType & MM_MMXEXT )
		{
		m_pfnStaticFrameDetect = StaticFrameDetect_acc;
		}
		else if( m_nCpuType & MM_MMX )
		{
		m_pfnStaticFrameDetect = StaticFrameDetect_acc;
		}
		else
		{
		m_pfnStaticFrameDetect = StaticFrameDetect_x86;
		}
		//*/
		saveStr("YUY2\r\n");
	}
	BOOL Detect(BYTE* pBuffer, long nBufferLen);
	DETECT_TYPE ColorSceneDetect (BYTE* pBuffer,long nBufferLen,DETECT_TYPE type=DETECT_TYPE_COLOR_SCENE|DETECT_TYPE_BLACK_SCENE);
	DETECT_TYPE ColorStripDetect (BYTE* pBuffer,long nBufferLen,DETECT_TYPE type=DETECT_TYPE_COLOR_STRIP);
	DETECT_TYPE StaticFrameDetect_x86(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type=DETECT_TYPE_STATIC_FRAME);
	DETECT_TYPE _ColorStripDetect(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type=DETECT_TYPE_COLOR_STRIP);
	DETECT_TYPE StaticFrameDetect_acc(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type=DETECT_TYPE_STATIC_FRAME);
	DETECT_TYPE EQRoundCastDetection(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type=DETECT_TYPE_COLOR_CAST);//偏色检测 by TTW

protected:
	Separator* __fastcall SplitLine(BYTE* pBuffer,DWORD nBufferLen,int margin);
	BOOL __fastcall isColorStripLine(BYTE* pLine,DWORD nLineWidth);
	void __fastcall DirGrad(BYTE* pImg, int nWidth, int nHeight,BYTE *pnGradX);
	void __fastcall NonmaxSuppress(BYTE* pImg,BYTE* pImgEdge, int nWidth, int nHeight,int th);
	void __fastcall Canny(BYTE* pImg, int nWidth, int nHeight,int th,int SmoHei);
	void __fastcall MedFilt(BYTE* lpImgData,DWORD Width,DWORD Height,DWORD tWidth=3,DWORD tHeight=3,int interval=1);

	typedef DETECT_TYPE (YUY2_Detect::*pfnDetect)(BYTE* pBuffer, long nBufferLen, DETECT_TYPE type);
	pfnDetect m_pfnStaticFrameDetect;

};

class  YV12_Detect:public AutoDetect
{
protected:
	BYTE* p_y;
	BYTE* p_v;
	BYTE* p_u;
	DWORD nPixels;
public:
	YV12_Detect(){SafeSetConfig(YUV_FORMAT,YUV_FORMAT_YV12,int); saveStr("YV12\r\n");}
	BOOL Detect(BYTE* pBuffer, long nBufferLen);
	DETECT_TYPE ColorSceneDetect ();	
	DETECT_TYPE ColorStripDetect ();
	DETECT_TYPE StaticFrameDetect();
	DETECT_TYPE _ColorStripDetect();
protected:
	Separator* __fastcall SplitLine(BYTE* pBuffer,DWORD nBufferLen,int margin);
	BOOL __fastcall isColorStripLine(BYTE* pLine,DWORD nLineWidth);
	void __fastcall DirGrad(BYTE* pImg, int nWidth, int nHeight,BYTE *pnGradX);
	void __fastcall NonmaxSuppress(BYTE* pImg,BYTE* pImgEdge, int nWidth, int nHeight,int th);
	void __fastcall Canny(BYTE* pImg, int nWidth, int nHeight,int th,int SmoHei);
	void __fastcall MedFilt(BYTE* lpImgData,DWORD Width,DWORD Height,DWORD tWidth=3,DWORD tHeight=3,int interval=1);

};
