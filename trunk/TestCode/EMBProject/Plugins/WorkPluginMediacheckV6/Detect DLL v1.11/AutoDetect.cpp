// AutoDetect.cpp : Defines the entry point for the DLL application.
//

#include "AutoDetect.h"

#include <afxwin.h>
#include <tchar.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <math.h>
#include <afxcmn.h>
#include "MarkupMSXML.h"

//比较两个DWORD值高字节的大小
//如果arg1大于arg2,返回1,否则返回-1;
//
inline int DWORDCompare(const void* arg1,const void* arg2)
{
	if(HIWORD(*(DWORD*)arg1)>HIWORD( *(DWORD*)arg2))
		return 1;
	else return -1;
	
}
//////////////////////////////////////////////////////////////////////////
//  [4/21/2010 wang.zh] 判断CPU类型的函数, 用于优化检测算法的速度 
//


#ifdef __cplusplus
extern "C"
{
#endif	
	void memcpy_x86(void *dst, const void *src, int size);
	void memcpy_sse(void *dst, const void *src, int size);
	void memcpy_mmx(void *dst, const void *src, int size);
	void memcpy_xmm(void *dst, const void *src, int size);
#ifdef __cplusplus
};
#endif

#define GET_CPUID(__id, __eax,__ebx,__ecx,__edx)\
{\
	__asm MOV eax, __id\
	__asm MOV esi, ebx\
	__asm CPUID\
	__asm XCHG esi, ebx\
	__asm MOV __eax,eax\
	__asm MOV __ebx,esi\
	__asm MOV __ecx,ecx\
	__asm MOV __edx,edx\
}

static int __forceinline IsCpuid()
{
	int nRet = 0;
	/* See if CPUID instruction is supported ... */
	/* ... Get copies of EFLAGS into eax and ecx */
	__asm pushfd;
	__asm pop eax;
	__asm mov ecx, eax;

	/* ... Toggle the ID bit in one copy and store */
	/*     to the EFLAGS reg */
	__asm xor eax, 200000h;
	__asm push eax;
	__asm popfd;
	__asm pushfd;
	__asm pop eax;

	__asm cmp eax,ecx;
	__asm je ret_tag;
	__asm mov nRet, 1;
ret_tag:
	return nRet;
}

static int __inline cpu_detect(void)
{
    int rval = 0;
    int eax=0, ebx=0, ecx=0, edx=0;
    int max_std_level=0, max_ext_level=0, std_caps=0, ext_caps=0;

	if( IsCpuid() == 0)
	{
		return 0;
	}

	GET_CPUID(0, max_std_level, ebx, ecx, edx);
	if(max_std_level >= 1)
	{
		GET_CPUID(1, eax, ebx, ecx, std_caps);
		if (std_caps & (1<<23))
			rval |= MM_MMX;
		if (std_caps & (1<<25))
			rval |= MM_MMXEXT | MM_SSE;
		if (std_caps & (1<<26))
			rval |= MM_SSE2;
    }
	
	GET_CPUID(0x80000000, max_ext_level, ebx, ecx, edx);
	if(max_ext_level >= 0x80000001)
	{
		GET_CPUID(0x80000001, eax, ebx, ecx, ext_caps);
		if (ext_caps & (1<<31))
			rval |= MM_3DNOW;
		if (ext_caps & (1<<30))
			rval |= MM_3DNOWEXT;
		if (ext_caps & (1<<23))
			rval |= MM_MMX;
	}
	
	GET_CPUID(0, eax, ebx, ecx, edx);
	if (ebx == 0x68747541 &&
		edx == 0x69746e65 &&
		ecx == 0x444d4163)
	{
		/* AMD */
		if(ext_caps & (1<<22))
			rval |= MM_MMXEXT;
	}
	else if (
		ebx == 0x746e6543 &&
		edx == 0x48727561 &&
		ecx == 0x736c7561) 
	{
		/*  "CentaurHauls" */
		/* VIA C3 */
		if(ext_caps & (1<<24))
			rval |= MM_MMXEXT;
	}
	else if (
		ebx == 0x69727943 &&
		edx == 0x736e4978 &&
		ecx == 0x64616574)
	{
		/* Cyrix Section */
		/* See if extended CPUID level 80000001 is supported */
		/* The value of CPUID/80000001 for the 6x86MX is undefined
		according to the Cyrix CPU Detection Guide (Preliminary
		Rev. 1.01 table 1), so we'll check the value of eax for
		CPUID/0 to see if standard CPUID level 2 is supported.
		According to the table, the only CPU which supports level
		2 is also the only one which supports extended CPUID levels.
		*/
        if (eax < 2)
			return rval;
		if (ext_caps & (1<<24))
			rval |= MM_MMXEXT;
	}
	return rval;
};


//-------------------------------------------------------
//				2008-9-4
//			deleted by wang.zh
//			for multiple instance 
//-------------------------------------------------------


//-------------------------------------------------------
//				2008-9-4
//			altered by wang.zh begin
//			for multiple instance 
//-------------------------------------------------------

const float AutoDetect::CAudioHighHelper::__a__ = 52.0f;

#define VALUE_OFFSET(option)				\
	OPTION_VALUE_OFFSET(AutoDetect,option)
#define TYPE_OFFSET(option)					\
	OPTION_TYPE_OFFSET(AutoDetect,option)
#define MAP(option)	\
	OPTION_MAP_NO_SECTION(#option, VALUE_OFFSET(option), TYPE_OFFSET(option))

//配置参数映射
BEGIN_OPTION_MAP(AutoDetect)
	MAP(YUV_FORMAT)
	MAP(CLIP_WIDTH)
	MAP(CLIP_HEIGHT)
	MAP(TOP_SECURE_SECTION)
	MAP(BOTTOM_SECURE_SECTION)	
	MAP(LEFT_SECURE_SECTION)		
	MAP(RIGHT_SECURE_SECTION)
	MAP(BLACK_SCENE_THRESHOLD)
	MAP(COLOR_SCENE_PERCENTAGE)
	MAP(COLOR_STRIP_PERCENTAGE)
	MAP(STATIC_FRAME_PERCENTAGE)
	MAP(LINE_COUNTS_FOR_STRIP_DETECT)
	MAP(AMBIT_DEEMED_TO_SAME_PIXEL)
	MAP(UNDULATE_AMBIT_OF_Y)
	MAP(UNDULATE_AMBIT_OF_U)
	MAP(UNDULATE_AMBIT_OF_V)
	MAP(AUDIO_LOW_THRESHOLD)
	MAP(AUDIO_LOW_PERIOD)
	MAP(AUDIO_HIGH_THRESHOLD)
	MAP(AUDIO_HIGH_PERIOD)
	MAP(AUDIO_MUTE_THRESHOLD)
	MAP(AUDIO_MUTE_PERIOD)

	MAP(LOUDNESS_LOW_THRESHOLD)
	MAP(LOUDNESS_HIGH_THRESHOLD)
END_OPTION_MAP()

//默认配置参数值
BEGIN_DEFAULT_CONFIG(AutoDetect)
	OPTION_MAP_NO_SECTION("YUV_FORMAT",					new int(YUV_FORMAT_YUY2),	"INT")
	OPTION_MAP_NO_SECTION("CLIP_WIDTH",					new int(720),				"INT")
	OPTION_MAP_NO_SECTION("CLIP_HEIGHT",				new int(576),				"INT")
	OPTION_MAP_NO_SECTION("TOP_SECURE_SECTION",			new float(0.1F),			"FLOAT")
	OPTION_MAP_NO_SECTION("BOTTOM_SECURE_SECTION",		new float(0.1F),			"FLOAT")
	OPTION_MAP_NO_SECTION("LEFT_SECURE_SECTION",		new float(0.1F),			"FLOAT")
	OPTION_MAP_NO_SECTION("RIGHT_SECURE_SECTION",		new float(0.1F),			"FLOAT")
	OPTION_MAP_NO_SECTION("BLACK_SCENE_THRESHOLD",		new int(60),				"INT")
	OPTION_MAP_NO_SECTION("COLOR_SCENE_PERCENTAGE",		new float(0.985F),			"FLOAT")
	OPTION_MAP_NO_SECTION("COLOR_STRIP_PERCENTAGE",		new float(0.70F),			"FLOAT")
	OPTION_MAP_NO_SECTION("STATIC_FRAME_PERCENTAGE",	new float(0.98F),			"FLOAT")
	OPTION_MAP_NO_SECTION("LINE_COUNTS_FOR_STRIP_DETECT",new int(100),				"INT")
	OPTION_MAP_NO_SECTION("AMBIT_DEEMED_TO_SAME_PIXEL",	new int(1),					"INT")
	OPTION_MAP_NO_SECTION("UNDULATE_AMBIT_OF_Y",		new int(3),					"INT")
	OPTION_MAP_NO_SECTION("UNDULATE_AMBIT_OF_U",		new int(3),					"INT")
	OPTION_MAP_NO_SECTION("UNDULATE_AMBIT_OF_V",		new int(3),					"INT")
	OPTION_MAP_NO_SECTION("AUDIO_LOW_THRESHOLD",		new int(-38),				"INT")
	OPTION_MAP_NO_SECTION("AUDIO_LOW_PERIOD",			new int(120),				"FLOAT")
	OPTION_MAP_NO_SECTION("AUDIO_HIGH_THRESHOLD",		new int(-9),				"INT")
	OPTION_MAP_NO_SECTION("AUDIO_HIGH_PERIOD",			new int(1),					"FLOAT")
	OPTION_MAP_NO_SECTION("AUDIO_MUTE_THRESHOLD",		new int(-60),				"INT")
	OPTION_MAP_NO_SECTION("AUDIO_MUTE_PERIOD",			new int(60),				"FLOAT")
	OPTION_MAP_NO_SECTION("LOUDNESS_LOW_THRESHOLD",     new double(-60),                 "DOUBLE")
	OPTION_MAP_NO_SECTION("LOUDNESS_HIGH_THRESHOLD",    new double(0),                   "DOUBLE")
END_DEFAULT_MAP()

//-------------------------------------------------------
//				2008-9-4
//			altered by wang.zh end
//			for multiple instance 
//-------------------------------------------------------
AutoDetect::AutoDetect(void)
: m_cRefCount(1)
, pFrameBuffer(NULL)
, m_pfnMemCpy(0)
, m_nCpuType(0)
, m_pFrameDiff(0)
{
	Initialize();
	LogWrite ("AutoDetect Initialized!!!\r\n");
	m_nCpuType = cpu_detect();
	if( m_nCpuType & MM_SSE )
	{
		m_pfnMemCpy = memcpy_sse;
	}
	else if( m_nCpuType & MM_MMXEXT )
	{
		m_pfnMemCpy = memcpy_xmm;
	}
	else if( m_nCpuType & MM_MMX )
	{
		m_pfnMemCpy = memcpy_mmx;
	}
	else 
	{
		m_pfnMemCpy = memcpy_x86;
	}
	volumeCalre.m_VolumeOut = this;
}

AutoDetect::~AutoDetect(void)
{
	LogWrite ("AutoDetect Uninitialized, Detect Count :%d!!!\r\n", count);
	if (pFrameBuffer)
	{
		delete pFrameBuffer;
		pFrameBuffer = NULL;
	}
	if( m_pFrameDiff )
	{
		delete m_pFrameDiff;
		m_pFrameDiff = 0;
	}
	CLEAN_UP_OPTION();
}
													
//计算中值.用于中值滤波.
//参数: tem : 一维数组.
//参数: tWidth: tem数组的宽,tHeight:tem 数组的高
//返回：tem中的中值。

int __cdecl cmpbyte(const void * p1, const void *p2)
{
	int n1 = *((BYTE*)(p1));
	int n2 = *((BYTE*)(p2));
	if( n1 > n2 )
	{
		return 1;
	}
	else if( n1== n2 )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

BYTE __forceinline AutoDetect::GetMed(BYTE tem[],DWORD tWidth,DWORD tHeight )
{
	size_t s = tWidth*tHeight;
	qsort((void*)tem, s, sizeof(BYTE), cmpbyte);
	return tem[s>>1];
}

void AutoDetect::Initialize(void)
{
	READ_DEFAULT_CONFIG();
	count = -1;
	m_VideoResult = 0;
	m_AudioResult = 0;

	m_nCurrentAudio = 0;

	nTBlack = 0;
	nTColor = 0;
	nTStrip = 0;
	nTstatic = 0;
	nTColorCast = 0;

	timer[0].stop();timer[0].reset();
	timer[1].stop();timer[1].reset();
	timer[2].stop();timer[2].reset();
	timer[3].stop();timer[3].reset();
	timer[4].stop();timer[4].reset();

	SendResult = NULL;

	m_detectType=DETECT_TYPE_COLOR_SCENE|
		DETECT_TYPE_COLOR_STRIP|
		DETECT_TYPE_STATIC_FRAME|
		DETECT_TYPE_BLACK_SCENE|
		DETECT_TYPE_AUDIO_MUTE|
		DETECT_TYPE_AUDIO_HIGH|
		DETECT_TYPE_AUDIO_LOW|
		DETECT_TYPE_MOSAIC|       //zibj 2011.7.1 马赛克、偏色 、响度 
		DETECT_TYPE_COLOR_CAST|
		DETECT_TYPE_LOUDNESS_LOW|
		DETECT_TYPE_LOUDNESS_HIGH;

	if (pFrameBuffer)
	{
		delete[] pFrameBuffer;
		pFrameBuffer =NULL;
	}
	OnConfigChanged();
}

//设置处理结果的回调函数

void AutoDetect::SetCallback(ResultCB* pCallback )
{
	SendResult = pCallback;	
}

//设置检测类型

void AutoDetect::SetDetectType(DETECT_TYPE type)
{
	m_detectType = 0;
	m_detectType|=type;
}

//保存信息到文件中。用于调试。
void AutoDetect::saveStr(LPSTR str)
{
}

//重置检测相关的变量，每检测一桢都要调用此函数。
void AutoDetect::VideoReset()
{
	m_VideoResult= 0;

	//color and black scene detect  varialbes;
	memset(lineHash[0],0,sizeof(lineHash[0]));
	memset(lineHash[1],0,sizeof(lineHash[1]));
	memset(lineHash[2],0,sizeof(lineHash[2]));
	memset(maxPixelCount,0,sizeof(maxPixelCount));
	lineHashCursor=0;
	memset(maxPixelCursor,0,sizeof(maxPixelCursor));

	memset(stripUHash,0,sizeof(stripUHash));
	maxUCount=0;
	maxUCursor=0;

	stripHashCursor = 0;
	pixelCursor=0;
	stripCursor=0;
}

void AutoDetect::AudioReset()
{
	switch (m_AudioResult)
	{
	case DETECT_TYPE_AUDIO_MUTE:
	case DETECT_TYPE_AUDIO_LOW:
	case DETECT_TYPE_AUDIO_LOW | DETECT_TYPE_AUDIO_MUTE:
		{
			m_AudioResult = 0;
			memset(&m_aiLow, 0, sizeof(m_aiLow));
			memset(&m_aiMute, 0, sizeof(m_aiMute));
		}
		break;
	case DETECT_TYPE_AUDIO_HIGH:
		{
			m_AudioResult = 0;
			memset(&m_aiHigh, 0, sizeof(m_aiHigh));
			m_xAudioHighHelper.reset();
		}
		break;
	case  DETECT_TYPE_LOUDNESS_HIGH:
		{
			m_AudioResult = 0 ;
			memset(&m_loudHigh,0,sizeof(m_loudHigh));
		}break;
	case DETECT_TYPE_LOUDNESS_LOW : 
		{
			m_AudioResult = 0;
			memset(&m_loudLow,0,sizeof(m_loudLow));
		}break;
	}
}
//处理检测结果。
void AutoDetect::ProcessVideoResult()
{
	if(m_VideoResult !=0 )
	{
		if(SendResult)
			SendResult->ProcessResult(m_VideoResult,count);
	}
}
void AutoDetect::ProcessAudioResult()
{
	if (m_AudioResult != 0 && SendResult)
	{
		if(m_AudioResult & DETECT_TYPE_AUDIO_LOW)
		{
			ADOutputDbg("\nAutoDetect:音频过低:s:%I64d,d:%I64d\r\n", m_aiLow.nStart, m_aiLow.nDuration);
			SendResult->ProcessResultAudio(DETECT_TYPE_AUDIO_LOW, m_aiLow.nStart, m_aiLow.nDuration, m_aiLow.nResevred);
		}

		if(m_AudioResult & DETECT_TYPE_AUDIO_MUTE)
		{
			ADOutputDbg("\nAutoDetect:音频丢失:s:%I64d,d:%I64d\r\n", m_aiMute.nStart, m_aiMute.nDuration);
			SendResult->ProcessResultAudio(DETECT_TYPE_AUDIO_MUTE, m_aiMute.nStart, m_aiMute.nDuration, m_aiMute.nResevred );
		}
		
		if(m_AudioResult & DETECT_TYPE_AUDIO_HIGH)
		{
			ADOutputDbg("\nAutoDetect:音频过高:s:%I64d,d:%I64d\r\n", m_aiHigh.nStart, m_aiHigh.nDuration);
			SendResult->ProcessResultAudio(DETECT_TYPE_AUDIO_HIGH, m_aiHigh.nStart, m_aiHigh.nDuration, m_aiHigh.nResevred);
		}

		if(m_AudioResult & DETECT_TYPE_LOUDNESS_LOW)
		{
			ADOutputDbg("\nAutoDetect:响度过低:s:%I64d,d:%I64d\r\n", m_aiHigh.nStart, m_aiHigh.nDuration);
			SendResult->ProcessResultAudio(DETECT_TYPE_LOUDNESS_LOW, m_loudLow.nStart, m_loudLow.nDuration, 0);
		}
		
		if(m_AudioResult & DETECT_TYPE_LOUDNESS_HIGH)
		{
			ADOutputDbg("\nAutoDetect:响度过高:s:%I64d,d:%I64d\r\n", m_aiHigh.nStart, m_aiHigh.nDuration);
			SendResult->ProcessResultAudio(DETECT_TYPE_LOUDNESS_HIGH, m_loudHigh.nStart, m_loudHigh.nDuration, 0);
 		}
	}
}

//配置参数
//参数：index：要配置的参数索引：
//参数：value：要给索引值为index的参数配置的值，可能需要进行强制类型转换。
//
void AutoDetect::SetConfig(PARAM_INDEX index,double value)
{
	switch(index)
	{
	case PARAM_YUV_FORMAT:
		SafeSetConfig(YUV_FORMAT,value,int);
		break;
	case PARAM_CLIP_WIDTH:
		SafeSetConfig(CLIP_WIDTH,value,int);
		break;
	case PARAM_CLIP_HEIGHT:
		SafeSetConfig(CLIP_HEIGHT,value,int);
		break;	
	case PARAM_TOP_SECURE_SECTION:
		SafeSetConfig(TOP_SECURE_SECTION ,value,float);
		break;
	case PARAM_BOTTOM_SECURE_SECTION:
		SafeSetConfig(BOTTOM_SECURE_SECTION ,value,float);
		break;
	case PARAM_LEFT_SECURE_SECTION:
		SafeSetConfig(LEFT_SECURE_SECTION ,value,float);
		break;
	case PARAM_RIGHT_SECURE_SECTION:
		SafeSetConfig(RIGHT_SECURE_SECTION ,value,float);
		break;
	case PARAM_BLACK_SCENE_THRESHOLD:
		SafeSetConfig(BLACK_SCENE_THRESHOLD ,value,int);
		break;
	case PARAM_COLOR_SCENE_PERCENTAGE:
		SafeSetConfig(COLOR_SCENE_PERCENTAGE ,value,float);
		break;
	case PARAM_COLOR_STRIP_PERCENTAGE:
		SafeSetConfig(COLOR_STRIP_PERCENTAGE ,value,float);
		break;
	case PARAM_STATIC_FRAME_PERCENTAGE:
		SafeSetConfig(STATIC_FRAME_PERCENTAGE ,value,float);
		break;
	case PARAM_LINE_COUNTS_FOR_STRIP_DETECT:
		SafeSetConfig(LINE_COUNTS_FOR_STRIP_DETECT ,value,int);
		break;
	case PARAM_AMBIT_DEEMED_TO_SAME_PIXEL:
		SafeSetConfig(AMBIT_DEEMED_TO_SAME_PIXEL ,value,int);
		break;
	case PARAM_UNDULATE_AMBIT_OF_Y:
		SafeSetConfig(UNDULATE_AMBIT_OF_Y,value,int);
		break;
	case PARAM_UNDULATE_AMBIT_OF_U:
		SafeSetConfig(UNDULATE_AMBIT_OF_U ,value,int);
		break;
	case PARAM_UNDULATE_AMBIT_OF_V:
		SafeSetConfig(UNDULATE_AMBIT_OF_V ,value, int);
		break;
	case PARAM_AUDIO_LOW_THRESHOLD:
		SafeSetConfig(AUDIO_LOW_THRESHOLD ,value, int);
		break;
	case PARAM_AUDIO_LOW_PERIOD:
		SafeSetConfig(AUDIO_LOW_PERIOD, value, float);
		break;
	case PARAM_AUDIO_HIGH_THRESHOLD:
		SafeSetConfig(AUDIO_HIGH_THRESHOLD, value, int);
		break;
	case PARAM_AUDIO_HIGH_PERIOD:
		SafeSetConfig(AUDIO_HIGH_PERIOD, value, float);
		break;
	case PARAM_AUDIO_MUTE_THRESHOLD:
		SafeSetConfig(AUDIO_MUTE_THRESHOLD, value, int);
		break;
	case PARAM_AUDIO_MUTE_PERIOD:
		SafeSetConfig(AUDIO_MUTE_PERIOD, value, float);
		break;
	case PARAM_LOUDNESS_LOW_THRESHOLD:
		SafeSetConfig(LOUDNESS_LOW_THRESHOLD, value, double);
		break;
	case PARAM_LOUDNESS_HIGH_THRESHOLD:
		SafeSetConfig(LOUDNESS_HIGH_THRESHOLD, value, double);
		break;
	}
}
//2008年6月27日
//通过XML字符串配置检测参数
//王召

#define TRYandSET(attr,index,val)\
{\
	CString str1 = "PARAM_"+attr;\
	CString str2 = #index;\
	if (_tcscmp(str1,str2) == 0)\
	{\
		SetConfig(index,atof(val));\
		continue;\
	}\
}

BOOL AutoDetect::XMLConfig(LPCSTR strXMLTemp)
{
	CString strXML = "";
	strXML = strXMLTemp;
	strXML.TrimLeft();
	strXML.TrimRight();
	if (strXML.IsEmpty()) return FALSE;

	CMarkupMSXML xml;
	if(!xml.SetDoc(strXML))  return FALSE;
	if( !xml.FindElem("AutoDetectPara") ) return FALSE;
	xml.IntoElem();
	if( !xml.FindElem("Info")) return FALSE;
	if (!xml.FindElem("Data")) return FALSE;
	
	int i = 0;
	CString attr,val;
	while(TRUE)
	{
		attr = xml.GetAttribName(i++);
		if (attr.IsEmpty())
		{
			break;
		}
		//attr = xml.GetAttrib(attr);
		val = xml.GetAttrib(attr);//wangjun 20081017
		TRYandSET(attr,PARAM_YUV_FORMAT,val);
		TRYandSET(attr,PARAM_CLIP_WIDTH,val);
		TRYandSET(attr,PARAM_CLIP_HEIGHT,val);
		TRYandSET(attr,PARAM_TOP_SECURE_SECTION,val);
		TRYandSET(attr,PARAM_BOTTOM_SECURE_SECTION,val);
		TRYandSET(attr,PARAM_LEFT_SECURE_SECTION,val);
		TRYandSET(attr,PARAM_RIGHT_SECURE_SECTION,val);
		TRYandSET(attr,PARAM_BLACK_SCENE_THRESHOLD,val);
		TRYandSET(attr,PARAM_COLOR_SCENE_PERCENTAGE,val);
		TRYandSET(attr,PARAM_COLOR_STRIP_PERCENTAGE,val);
		TRYandSET(attr,PARAM_STATIC_FRAME_PERCENTAGE,val);
		TRYandSET(attr,PARAM_LINE_COUNTS_FOR_STRIP_DETECT,val);
		TRYandSET(attr,PARAM_AMBIT_DEEMED_TO_SAME_PIXEL,val);
		TRYandSET(attr,PARAM_UNDULATE_AMBIT_OF_Y,val);
		TRYandSET(attr,PARAM_UNDULATE_AMBIT_OF_U,val);
		TRYandSET(attr,PARAM_UNDULATE_AMBIT_OF_V,val);
		TRYandSET(attr,PARAM_AUDIO_LOW_THRESHOLD,val);		
		TRYandSET(attr,PARAM_AUDIO_LOW_PERIOD,val);
		TRYandSET(attr,PARAM_AUDIO_HIGH_THRESHOLD,val);
		TRYandSET(attr,PARAM_AUDIO_HIGH_PERIOD,val);
		TRYandSET(attr,PARAM_AUDIO_MUTE_THRESHOLD,val);
		TRYandSET(attr,PARAM_AUDIO_MUTE_PERIOD,val);
		TRYandSET(attr,PARAM_LOUDNESS_LOW_THRESHOLD,val);
		TRYandSET(attr,PARAM_LOUDNESS_HIGH_THRESHOLD,val);
	}

	OnConfigChanged();
	return TRUE;
}

void AutoDetect::OnConfigChanged()
{
	if (m_mtAudioType.nBlockAlign == 0 ||
		m_mtAudioType.nSamplingRate == 0 ||
		m_mtAudioType.nChannels == 0 ||
		m_mtAudioType.nBit == 0)
	{
		return ;
	}
	if(AUDIO_LOW_THRESHOLD.value == NULL ||
		AUDIO_HIGH_THRESHOLD.value == NULL ||
		AUDIO_MUTE_THRESHOLD.value == NULL ||
		AUDIO_LOW_PERIOD.value == NULL || 
		AUDIO_HIGH_PERIOD.value == NULL ||
		AUDIO_MUTE_PERIOD.value == NULL)
	{
		return;
	}
	m_Audio_Low_Threshold.nThreshold = static_cast<__int64>( pow(10, *AUDIO_LOW_THRESHOLD.value/20.0) * Audio_MaxValue(m_mtAudioType.nBit) ) ;
	m_Audio_Low_Threshold.nPeriod = static_cast<LONG>(*AUDIO_LOW_PERIOD.value * m_mtAudioType.nSamplingRate);
	m_Audio_High_Threshold.nThreshold = static_cast<__int64>( pow(10, *AUDIO_HIGH_THRESHOLD.value / 20.0) * Audio_MaxValue(m_mtAudioType.nBit) );
	m_Audio_High_Threshold.nPeriod = static_cast<LONG>(*AUDIO_HIGH_PERIOD.value * m_mtAudioType.nSamplingRate);
	m_Audio_Mute_Threshold.nThreshold = static_cast<__int64>( pow(10, *AUDIO_MUTE_THRESHOLD.value / 20.0) * Audio_MaxValue(m_mtAudioType.nBit) );
	m_Audio_Mute_Threshold.nPeriod = static_cast<LONG>(*AUDIO_MUTE_PERIOD.value * m_mtAudioType.nSamplingRate);
	m_xAudioHighHelper.Init(m_mtAudioType.nSamplingRate);
}


//读取默认配置。
//调试过程中用ini格式的配置文件保存配置信息。
//
void AutoDetect::DefaultConfig()
{
	READ_DEFAULT_CONFIG();
	OnConfigChanged();
}


//-------------------------------------------------------
//				2008-9-4
//			altered by wang.zh begin
//			for multiple instance 
//-------------------------------------------------------
//创建检测类实例
//参数: YUVtype:指定要创建的类实例的类型,可以是YUV_FORMAT_YUY2或YUV_FORMAT_YV12;
//返回: 实例的地址
//
AutoDetect* AutoDetect::CreateInstance(_YUV_FORMAT YUVtype)
{
	if (YUV_FORMAT_YUY2 == YUVtype )
	{
			return new YUY2_Detect;
	}
	else if (YUV_FORMAT_YV12 == YUVtype) 
	{
		return new YV12_Detect;
	}
	else return NULL;
}

BOOL AutoDetect::DetectAudio(BYTE* pBuffer, long nBufferLen)
{
	if(pBuffer == 0 || nBufferLen == 0)
	{
		return FALSE;
	}

// 	loudCho.loud->CalLoudness(pBuffer,nBufferLen);
// 	
// 	if(loudCho.loud->isCalculated)
// 	{
// 		loudnessValue = loudCho.loud->loudnessValue;
// 		if (loudnessValue < *LOUDNESS_LOW_THRESHOLD.value)
// 		{
// 			//ADOutputDbg("\nAutoDetect:响度过低\r\n");
// 			m_AudioResult |= DETECT_TYPE_LOUDNESS_LOW;
// 			m_loudLow.nStart = loudCho.loud->currentPosition;
// 			m_loudLow.nDuration = loudCho.loud->errorDuration ;
// 			
// 		}
// 		else if (loudnessValue > *LOUDNESS_HIGH_THRESHOLD.value)
// 		{
// 			// ADOutputDbg("\nAutoDetect:响度过高\r\n");
// 			m_AudioResult |= DETECT_TYPE_LOUDNESS_HIGH;
// 			m_loudHigh.nStart = loudCho.loud->currentPosition;
// 			m_loudHigh.nDuration = loudCho.loud->errorDuration ;
// 		}	   
// 		ProcessAudioResult();
// 		AudioReset();
// 		loudCho.loud->isCalculated = FALSE;
// 		//如果判断是响度过高或过低，则不进行下面的检查
// 		return TRUE;
// 	}
// 上面是响度。 下面是音量。 这个真不一样。
	m_nCurrentAudio += volumeCalre.VolCal(pBuffer,nBufferLen);
// 	if(pBuffer == 0 || nBufferLen == 0 )
// 	{
// 		if (m_aiMute.nStart != 0)
// 		{
// 			if (m_aiMute.nDuration >= m_Audio_Mute_Threshold.nPeriod)
// 			{
// 				m_AudioResult |= DETECT_TYPE_AUDIO_MUTE;
// 				if (m_aiLow.nStart !=0 )
// 				{
// 					m_aiLow.nDuration -= m_aiMute.nDuration;
// 					if (m_aiLow.nDuration > m_Audio_Low_Threshold.nPeriod)
// 					{
// 						m_AudioResult |= DETECT_TYPE_AUDIO_LOW;							
// 					}
// 				}
// 				ProcessAudioResult();
// 				AudioReset();
// 			}
// 			else
// 			{
// 				memset(&m_aiMute, 0, sizeof(m_aiMute));
// 			}
// 		}
// 		if (m_aiLow.nStart != 0)
// 		{
// 			if (m_aiLow.nDuration >= m_Audio_Low_Threshold.nPeriod)
// 			{
// 				m_AudioResult |= DETECT_TYPE_AUDIO_LOW;
// 				ProcessAudioResult();
// 				AudioReset();
// 			}
// 			else
// 			{
// 				memset(&m_aiLow, 0, sizeof(m_aiLow));
// 			}
// 		}
// 		
// 		AUDIO_HIGH_STASTU s=DetectAudioHigh(0);
// 		if( DETECT_STATUS_YES == s )
// 		{
// 			m_AudioResult |= DETECT_TYPE_AUDIO_HIGH;
// 			ProcessAudioResult();
// 			AudioReset();
// 		}
// 		else if( DETECT_STATUS_NO == s )
// 		{
// 			memset(&m_aiHigh, 0, sizeof(m_aiHigh));
// 			m_xAudioHighHelper.reset();
// 		}
// 	}
// 	else
// 	{
// 		__int64 valt = 0;
// 		__int64 llMsk = Audio_BitMask(m_mtAudioType.nBit);
// 		BYTE* pEnd = pBuffer + nBufferLen;
// 		while (pBuffer < pEnd)
// 		{
// 			valt = (*(reinterpret_cast<LONGLONG*>(pBuffer))) & llMsk;
// 			valt = Audio_absll(valt, m_mtAudioType.nBit, llMsk);
// 			
// 			if (valt < m_Audio_Mute_Threshold.nThreshold)
// 			{
// 				if (m_aiMute.nStart == 0)
// 				{
// 					m_aiMute.nStart = m_nCurrentAudio;
// 					m_aiMute.nDuration = 0;
// 				}
// 				m_aiMute.nDuration++;
// 			}
// 			else
// 			{
// 				if (m_aiMute.nStart != 0)
// 				{
// 					if (m_aiMute.nDuration >= m_Audio_Mute_Threshold.nPeriod)
// 					{
// 						m_AudioResult |= DETECT_TYPE_AUDIO_MUTE;
// 						ProcessAudioResult();
// 						if (m_aiLow.nStart !=0 )
// 						{
// 							m_aiLow.nDuration -= m_aiMute.nDuration;
// 							if (m_aiLow.nDuration > m_Audio_Low_Threshold.nPeriod)
// 							{
// 								m_AudioResult |= DETECT_TYPE_AUDIO_LOW;
// 								
// 							}
// 						}
// 						ProcessAudioResult();
// 						AudioReset();
// 					}
// 					else
// 					{
// 						memset(&m_aiMute, 0, sizeof(m_aiMute));
// 					}
// 				}
// 			}
// 			
// 			if (valt < m_Audio_Low_Threshold.nThreshold)
// 			{
// 				if (m_aiLow.nStart == 0)
// 				{
// 					m_aiLow.nStart = m_nCurrentAudio;
// 					m_aiLow.nDuration = 0;
// 				}
// 				m_aiLow.nDuration++;
// 			}
// 			else
// 			{
// 				if (m_aiLow.nStart != 0)
// 				{
// 					if (m_aiLow.nDuration >= m_Audio_Low_Threshold.nPeriod)
// 					{
// 						m_AudioResult |= DETECT_TYPE_AUDIO_LOW;
// 						ProcessAudioResult();
// 						AudioReset();
// 					}
// 					else
// 					{
// 						memset(&m_aiLow, 0, sizeof(m_aiLow));
// 					}
// 				}
// 			}
// 			
// 			AUDIO_HIGH_STASTU s=DetectAudioHigh(&valt);
// 			if( DETECT_STATUS_YES == s )
// 			{
// 				m_AudioResult |= DETECT_TYPE_AUDIO_HIGH;
// 				ProcessAudioResult();
// 				AudioReset();
// 			}
// 			else if( DETECT_STATUS_NO == s )
// 			{
// 				memset(&m_aiHigh, 0, sizeof(m_aiHigh));
// 				m_xAudioHighHelper.reset();
// 			}
//这结束			
			// 		if (valt > m_Audio_High_Threshold.nThreshold)
			// 		{
			// 			if (m_aiHigh.nStart == 0)
			// 			{
			// 				m_aiHigh.nStart = m_nCurrentAudio;
			// 				m_aiHigh.nDuration = 0;
			// 			}
			// 			m_aiHigh.nDuration++;
			// 		}
			// 		else
			// 		{
			// 			if (m_aiHigh.nStart != 0)
			// 			{
			// 				if (m_aiHigh.nDuration >= m_Audio_High_Threshold.nPeriod)
			// 				{
			// 					m_AudioResult |= DETECT_TYPE_AUDIO_HIGH;
			// 					ProcessAudioResult();
			// 					AudioReset();
			// 				}
			// 				else
			// 				{
			// 					memset(&m_aiHigh, 0, sizeof(m_aiHigh));
			// 				}
			// 			}
			// 		}
			// 	
			//下面这段要开。
// 			pBuffer += m_mtAudioType.nBlockAlign;
// 
//			SendResult->AudioCount(m_nCurrentAudio);
//			m_nCurrentAudio++;

// 		}
// 	}
	
	return TRUE;
}

AutoDetect::AUDIO_HIGH_STASTU AutoDetect::DetectAudioHigh(__int64* valt)
{
	if( valt == 0 )
	{
		m_xAudioHighHelper.reset();
		if (m_aiHigh.nDuration >= m_Audio_High_Threshold.nPeriod)
		{
			return DETECT_STATUS_YES;
		}
		else
		{		
			return DETECT_STATUS_NO;
		}
	}
	else
	{
		if (m_aiHigh.nStart == 0)
		{
			if (*valt > m_Audio_High_Threshold.nThreshold)
			{
				m_xAudioHighHelper.Set(valt);
				m_aiHigh.nStart = m_nCurrentAudio;
				m_aiHigh.nDuration = 1;
				return DETECT_STATUS_CONTINUE;
			}
			else
			{
				return DETECT_STATUS_NO;
			}
		}
		else
		{
			__int64 b = m_xAudioHighHelper.Next();
			
			if( b >  m_Audio_High_Threshold.nThreshold || *valt > m_Audio_High_Threshold.nThreshold )
			{
				m_aiHigh.nDuration++;
				if( *valt > b)
				{
					m_xAudioHighHelper.reset();
					m_xAudioHighHelper.Set(valt);
				}
				return DETECT_STATUS_CONTINUE;
			}
			else
			{
				if (m_aiHigh.nDuration >= m_Audio_High_Threshold.nPeriod)
				{
					return DETECT_STATUS_YES;
				}
				else
				{
					return DETECT_STATUS_NO;
				}
			}
		}
	}
}

void AutoDetect::SetAudioType(int nChannels, int nSampleRate, int nBit, int nBlockAlign)
{
	memset(&m_mtAudioType, 0, sizeof(m_mtAudioType));
	m_mtAudioType.nChannels = nChannels;
	m_mtAudioType.nSamplingRate = nSampleRate;
	m_mtAudioType.nBit = nBit;
	m_mtAudioType.nBlockAlign = nBlockAlign;
	WAVEFORMATEX wa;
	wa.nBlockAlign = nBlockAlign;
	wa.nChannels = nChannels;
	wa.wBitsPerSample = nBit;
	wa.nSamplesPerSec = nSampleRate;
	volumeCalre.init(&wa,*AUDIO_HIGH_THRESHOLD.value,*AUDIO_LOW_THRESHOLD.value,*AUDIO_MUTE_THRESHOLD.value,2);
	loudCho.SetAudioType(nChannels,nSampleRate,nBit,nBlockAlign);
	OnConfigChanged();
}
 
//-------------------------------------------------------
//				2008-9-4
//			altered by wang.zh end
//			for multiple instance 
//-------------------------------------------------------


//-------------------------------------------------------
//				2008-9-4
//			added by wang.zh begin
//			for multiple instance 
//-------------------------------------------------------
void AutoDetect::DestroyDefaultOptionMap()
{
	CLEAN_UP_DEF_OPTION();
}

long  AutoDetect::AddRef()
{
	return InterlockedIncrement(&m_cRefCount);
}

long  AutoDetect::Release()
{
	if ( 0 == InterlockedDecrement(&m_cRefCount) )
	{
		delete this;
		return 0;
	}
	return m_cRefCount;
}
#ifdef _DEBUG
void AutoDetect::AssertValid()
{
	ASSERT(this);
	ASSERT_VALID_OPTION();
}
void AutoDetect::Dump(CDumpContext& dc)
{
	DUMP_OPTION(dc);
}
#endif

void __cdecl AutoDetect::LogWrite(LPCSTR strFormat,...)
{
	CString strOutput;
	va_list args;
	va_start(args,strFormat);
	strOutput.FormatV(strFormat, args);	
	va_end(args);
	OutputDebugString(strOutput);
}
AutoDetect::CFreezeFrameHelper::CFreezeFrameHelper()
{
	CleanUp ();
}

AutoDetect::CFreezeFrameHelper::~CFreezeFrameHelper()
{
	CleanUp();	
}

void AutoDetect::CFreezeFrameHelper::CleanUp()
{
	POSITION pos = m_xClusters.GetHeadPosition();
	while ( pos )
	{
		delete (m_xClusters.GetNext (pos));
	}
	m_xClusters.RemoveAll ();
	m_nTotalPixels = 0;
}

int AutoDetect::CFreezeFrameHelper::GetLevel (int xx)
{
	static const int st_LevelMap [xLevelMax+1] = { 0, 3, 5, 7, 9, 11};
	int nLevel = 0;
	for ( nLevel = xLevelMax; nLevel >= xLevelMin; nLevel--)
	{
		if ( xx > st_LevelMap[nLevel])
		{
			return nLevel;
		}
	}
	return 0;
}

int AutoDetect::CFreezeFrameHelper::Add(int x, int y, int nLevel)
{
	POSITION pos = m_xClusters.GetHeadPosition();
	BOOL bFind = FALSE;
	CCluster* pFind = NULL;
	while ( pos )
	{
		CCluster*& p = m_xClusters.GetNext (pos);
		if ( x >= p->m_nXmin-1 &&
			 x <= p->m_nXmax+1 &&
			 y >= p->m_nYmin-1 &&
			 y <= p->m_nYmax+1 )
		{
			bFind = TRUE;
			pFind = p;
			break;
		}
	}

	if ( !bFind )
	{
		pFind = new CCluster();
		if( pFind )
		{
			pFind->m_nXmin = x;
			pFind->m_nXmax = x;
			pFind->m_nYmin = y;
			pFind->m_nYmax = y;
			pFind->m_nCount = 1;
			pFind->m_nLevel = nLevel;
			pFind->m_dPercentage = (m_nTotalPixels > 0) ? (1.0f/m_nTotalPixels) : (1.0f);
			m_xClusters.AddHead (pFind);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		ASSERT(pFind);
		if ( pFind->m_nXmin > x )
			pFind->m_nXmin =  x;
		if ( pFind->m_nXmax < x )
			pFind->m_nXmax = x;
		if ( pFind->m_nYmin > y )
			pFind->m_nYmin = y;
		if ( pFind->m_nYmax < y )
			pFind->m_nYmax = y;
		if ( pFind->m_nLevel < nLevel )
			pFind->m_nLevel = nLevel;
		pFind->m_nCount++;
		pFind->m_dPercentage = (m_nTotalPixels > 0) ? ((float)(pFind->m_nCount) / m_nTotalPixels) : (1.0f);	
		return TRUE;
	}
}

float AutoDetect::CFreezeFrameHelper::Filter(int nCount, int nLevel)
{
	POSITION pos = m_xClusters.GetHeadPosition ();
	float ret = 0.0f;
	while ( pos )
	{
		CCluster*& p = m_xClusters.GetNext(pos);
		if( p->m_nCount >= nCount &&
			p->m_nLevel >= nLevel )
			ret += p->m_dPercentage;
	}
	return ret;
}
//-------------------------------------------------------
//				2008-9-4
//			added by wang.zh end
//			for multiple instance 
//-------------------------------------------------------


///////////////////////////////////////////////////////
//以下是YUY2格式检测的实现
///////////////////////////////////////////////////////


//检测一桢图像
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度.
//

BOOL YUY2_Detect::Detect(BYTE* pBuffer, long nBufferLen)
{	
	
	//reset detect result
	VideoReset();
	count++;
	SendResult->VideoCount(count);

	pBuffer= pBuffer+(int)((int)*CLIP_HEIGHT.value * *TOP_SECURE_SECTION.value) * *CLIP_WIDTH.value * YUY2_BYTES_PER_PIXEL;
	nBufferLen = nBufferLen 
		-int (*CLIP_HEIGHT.value * *TOP_SECURE_SECTION.value)  *   *CLIP_WIDTH.value * YUY2_BYTES_PER_PIXEL
		-int (*CLIP_HEIGHT.value * *BOTTOM_SECURE_SECTION.value) * *CLIP_WIDTH.value * YUY2_BYTES_PER_PIXEL;
	
	//zibj  2011.7.1 begin 马赛克、偏色算法不去掉安全区域
    BYTE *pBuf = pBuffer;
	long nBufLen = nBufferLen;
	//zibj 2011.7.1 end

	timer[0].ResetLastTime();
	timer[1].ResetLastTime();
	timer[2].ResetLastTime();
	timer[3].ResetLastTime();
	timer[4].ResetLastTime();

	DETECT_TYPE res;
	
	timer[0].start();

	timer[1].start();
	nTColor++;
	res = ColorSceneDetect(pBuffer,nBufferLen,m_detectType);
	timer[1].stop();

	if(res);

	else
	{
		
		timer[2].start();
		nTStrip++;
		res = _ColorStripDetect(pBuffer,nBufferLen,m_detectType);
		timer[2].stop();
		
		if(res);

		else
		
			{
				timer[3].start();
				nTstatic++;
				res = (this->*m_pfnStaticFrameDetect)(pBuffer,nBufferLen,m_detectType);
				timer[3].stop();

				if(res);
				else
				{
					timer[4].start();
					nTColorCast++;	
					//res = EQRoundCastDetection(pBuf,nBufLen,m_detectType); //by-TTW
					timer[4].stop();
				}
			}
		}
		
	timer[0].stop();
	ProcessVideoResult();
	return TRUE;
}

//彩条检测的一种实现方式: 通过均分图像,然后各条匹配的方式.
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度.
//参数: type: 要检测的类型.默认值:彩条.
//返回: 此桢是否是彩条.

DETECT_TYPE  YUY2_Detect::ColorStripDetect(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type)
{
	DWORD* StopP =(DWORD*) (pBuffer+nBufferLen);
	DWORD* p=(DWORD*)pBuffer;
	WORD  Rows = nBufferLen/(*CLIP_WIDTH.value*YUY2_BYTES_PER_PIXEL);
	long  RowInterval = (Rows-*LINE_COUNTS_FOR_STRIP_DETECT.value)/(*LINE_COUNTS_FOR_STRIP_DETECT.value)**CLIP_WIDTH.value*YUY2_BYTES_PER_PIXEL;
	RowInterval=RowInterval/sizeof(*p);
	if(RowInterval<0) RowInterval = 0;
	
	WORD NonStripLineCounts =WORD(*LINE_COUNTS_FOR_STRIP_DETECT.value*(1-*COLOR_STRIP_PERCENTAGE.value));
	WORD currentNonStripLine=0,currentNonStrip=0;
	
	//detect color strip blocks
	for(; p<StopP; p+=RowInterval)
	{
		currentNonStrip =0;

		for (stripCursor=0;stripCursor<STRIP_COUNTS;stripCursor++)
		{
			memset(stripUHash,0,sizeof(stripUHash));
			maxUCount = 0;

			for (pixelCursor=0;pixelCursor<(STRIP_WIDTH>>1);pixelCursor++)
			{
				stripUHash[(*p&0x0000ff00)>>8]++;
				p++;
			}
			for (stripHashCursor=0;stripHashCursor<256;stripHashCursor++)
			{
				if (stripUHash[stripHashCursor]>maxUCount)
				{
					maxUCount=stripUHash[stripHashCursor];
					maxUCursor=stripHashCursor;
				}
			}

			maxUCount=0;

			for (stripHashCursor=maxUCursor-5;stripHashCursor<=maxUCursor+5;stripHashCursor++)
			{
				if (stripHashCursor>=0&&stripHashCursor<=255)
				{	
					maxUCount+=stripUHash[stripHashCursor];
				}
			}

			if((float)maxUCount/(*CLIP_WIDTH.value/STRIP_COUNTS/2)<*COLOR_STRIP_PERCENTAGE.value)
			{
				currentNonStrip++;
				if(currentNonStrip>2)
				{
					currentNonStripLine++;
					p+=(STRIP_COUNTS-stripCursor-1)*STRIP_WIDTH/2;					
					break;
				}
				else continue;
			}
			
			if(abs((maxUCursor>>4)-COLOR_STRIP[stripCursor])>2)
			{
				currentNonStrip++;
				if(currentNonStrip>2)
				{
					currentNonStripLine++;
					p+=(STRIP_COUNTS-stripCursor-1)*STRIP_WIDTH/2;
					break;
				}
				else continue;
			}
		}
		if(currentNonStripLine > NonStripLineCounts)
		{
			return 0;
		}
	}
	m_VideoResult|=(type&DETECT_TYPE_COLOR_STRIP);
	return DETECT_TYPE_COLOR_STRIP;
}


//静桢检测
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度.
//参数: type: 要检测的类型.默认值:静桢.
//返回: 此桢是否是静桢.

DETECT_TYPE YUY2_Detect::StaticFrameDetect_x86(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type)
{
	if(pFrameBuffer == NULL)
	{
		pFrameBuffer = new BYTE[nBufferLen];
		m_pfnMemCpy(pFrameBuffer,pBuffer,nBufferLen);
		__asm {emms}
		return 0;
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		//  [9/27/2009 wang.zh] 静帧检测算法精确度修改	
		//
		int nWidth			= (int)(*CLIP_WIDTH.value - *CLIP_WIDTH.value *  (*LEFT_SECURE_SECTION.value + *RIGHT_SECURE_SECTION.value));
		int nHeight			= (int)(nBufferLen / (*CLIP_WIDTH.value * YUY2_BYTES_PER_PIXEL));

		nWidth -= nWidth % (sizeof(DWORD) / YUY2_BYTES_PER_PIXEL);

		const float dPercentage		= 1 - *STATIC_FRAME_PERCENTAGE.value;
		const int	nAmbit			=  *AMBIT_DEEMED_TO_SAME_PIXEL.value;

	
		DWORD LeftSafeSectionPixel  = DWORD(*CLIP_WIDTH.value * *LEFT_SECURE_SECTION.value);
		DWORD RightSafeSectionPixel = *CLIP_WIDTH.value - LeftSafeSectionPixel - nWidth;
		DWORD dwRightSafeSectionLen = RightSafeSectionPixel * YUY2_BYTES_PER_PIXEL;
		DWORD dwLeftSafeSectionLen  = LeftSafeSectionPixel * YUY2_BYTES_PER_PIXEL;
		
		DWORD RightSafeSecitonLine	= *CLIP_WIDTH.value - RightSafeSectionPixel;
		long dwValidPixel			= *CLIP_WIDTH.value - LeftSafeSectionPixel - RightSafeSectionPixel;
		
		DWORD*	StopP			= (DWORD*)(pBuffer+nBufferLen);
		DWORD*	p1				= (DWORD*)(pBuffer + dwLeftSafeSectionLen);
		DWORD*	p2				= (DWORD*)(pFrameBuffer + dwLeftSafeSectionLen);

		BYTE		i64x[sizeof(DWORD)] = {0},
					i64y[sizeof(DWORD)] = {0};


		m_xFreezeFrameHelper.CleanUp ();
		m_xFreezeFrameHelper.m_nTotalPixels = nWidth * nHeight;
		
		int nCurX = 0;
		int nCurY = 0;

		BYTE Y = 0;
		BYTE R = 0;
		
		//////////////////////////////////////////////////////////////////////////		
		
		while( p1 < StopP)
		{
			if (nCurX >= dwValidPixel)
			{
				p1 = (DWORD*)((BYTE*)(p1) + dwLeftSafeSectionLen + dwRightSafeSectionLen );
				p2 = (DWORD*)((BYTE*)(p2) + dwLeftSafeSectionLen + dwRightSafeSectionLen );
				if (p1 >= StopP)
				{
					break;
				}
				//  [9/27/2009 wang.zh]
				nCurX = 0;
				nCurY ++;
			}

			*(DWORD*)i64x = *p1;
			*(DWORD*)i64y = *p2;

			Y = abs(i64x[0] - i64y[0]);			
			R = abs(i64x[1] - i64y[1]);
			
			if ( R > nAmbit || Y > nAmbit )
			{
				m_xFreezeFrameHelper.Add (nCurX, nCurY, m_xFreezeFrameHelper.GetLevel (max(R,Y)));
				if( m_xFreezeFrameHelper.Filter (11, CFreezeFrameHelper::xLevel3) > dPercentage )
				{
					m_pfnMemCpy(pFrameBuffer, pBuffer, nBufferLen);
					__asm {emms}
					return 0;
				}
			}
			nCurX ++ ;
					
			Y = abs(i64x[2] - i64y[2]);			
			R = abs(i64x[3] - i64y[3]);
			
			if ( R > nAmbit || Y > nAmbit )
			{
				m_xFreezeFrameHelper.Add (nCurX, nCurY, m_xFreezeFrameHelper.GetLevel (max(R,Y)));
				if( m_xFreezeFrameHelper.Filter (11, CFreezeFrameHelper::xLevel3) > dPercentage )
				{
					m_pfnMemCpy(pFrameBuffer, pBuffer, nBufferLen);
					__asm {emms}
					return 0;
				}
			}
			nCurX ++ ;
			
// 			
// 			Y = abs(i64x[4] - i64y[4]);			
// 			R = abs(i64x[5] - i64y[5]);
// 			
// 			if ( R > nAmbit || Y > nAmbit )
// 			{
// 				m_xFreezeFrameHelper.Add (nCurX, nCurY, m_xFreezeFrameHelper.GetLevel (max(R,Y)));
// 				if( m_xFreezeFrameHelper.Filter (11, CFreezeFrameHelper::xLevel3) > dPercentage )
// 				{
// 					memcpy(pFrameBuffer, pBuffer, nBufferLen);
// 					return 0;
// 				}
// 			}
// 			nCurX ++ ;			
// 			
// 			Y = abs(i64x[6] - i64y[6]);			
// 			R = abs(i64x[7] - i64y[7]);
// 			
// 			if ( R > nAmbit || Y > nAmbit )
// 			{
// 				m_xFreezeFrameHelper.Add (nCurX, nCurY, m_xFreezeFrameHelper.GetLevel (max(R,Y)));
// 				if( m_xFreezeFrameHelper.Filter (11, CFreezeFrameHelper::xLevel3) > dPercentage )
// 				{
// 					memcpy(pFrameBuffer, pBuffer, nBufferLen);
// 					return 0;
// 				}
// 			}
// 			nCurX ++ ;		
			
			p1 ++;
			p2 ++;
		}

		//////////////////////////////////////////////////////////////////////////
		//  [9/27/2009 wang.zh] 静帧检测算法精确度修改
		if( m_xFreezeFrameHelper.Filter (11, CFreezeFrameHelper::xLevel1) > dPercentage )
		{
			m_pfnMemCpy (pFrameBuffer, pBuffer, nBufferLen );
			__asm {emms}
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////
		m_VideoResult |= (type & DETECT_TYPE_STATIC_FRAME);
		return DETECT_TYPE_STATIC_FRAME;
	}
}

DETECT_TYPE YUY2_Detect::StaticFrameDetect_acc(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type)
{
	if(pFrameBuffer == NULL)
	{
		pFrameBuffer = new BYTE[nBufferLen];
		m_pfnMemCpy(pFrameBuffer,pBuffer,nBufferLen);
		__asm {emms}
		return 0;
	}
	else
	{
		int nWidth			= (int)(*CLIP_WIDTH.value - *CLIP_WIDTH.value *  (*LEFT_SECURE_SECTION.value + *RIGHT_SECURE_SECTION.value));
		int nHeight			= (int)(nBufferLen / (*CLIP_WIDTH.value * YUY2_BYTES_PER_PIXEL));
		nWidth -= nWidth % (8 / YUY2_BYTES_PER_PIXEL);
		
		if( nWidth <= 0 || nHeight <= 0)
		{
			return 0;
		}

		if( !m_pFrameDiff )
		{
			m_pFrameDiff = new BYTE[nWidth*nHeight];
			if( !m_pFrameDiff )
			{
				return 0;
			}
		}
		const int nPercentage		= (int)((1 - *STATIC_FRAME_PERCENTAGE.value) * nWidth*nHeight);
		const int nAmbit			= (*AMBIT_DEEMED_TO_SAME_PIXEL.value & 0xFF);

		DWORD LeftSafeSectionPixels  = DWORD(*CLIP_WIDTH.value * *LEFT_SECURE_SECTION.value);
		DWORD RightSafeSectionPixels = *CLIP_WIDTH.value - LeftSafeSectionPixels - nWidth;
		DWORD dwRightSafeSectionLen = RightSafeSectionPixels * YUY2_BYTES_PER_PIXEL;
		DWORD dwLeftSafeSectionLen  = LeftSafeSectionPixels * YUY2_BYTES_PER_PIXEL;

		BYTE*	p1				= pBuffer + dwLeftSafeSectionLen;
		BYTE*	p2				= pFrameBuffer + dwLeftSafeSectionLen;
		BYTE*	pDst			= m_pFrameDiff;

		__asm push dword ptr nWidth;
		__asm push dword ptr nAmbit;

		__asm mov esi, dword ptr p1;		// pBuffer
		__asm mov edi, dword ptr p2;		// m_pFrameBuffer
		__asm mov eax, dword ptr pDst;		// m_pFrameDiff

		__asm mov ebx,dwLeftSafeSectionLen;
		__asm add ebx,dwRightSafeSectionLen;

		__asm sub esi,ebx;
		__asm sub edi,ebx;

		__asm mov ecx, dword ptr nHeight;	// Line Count
		__asm dec ecx;
		__asm jl LEAVE_LOOP;

		__asm push ebp;
		__asm mov ebp, 0x00ff00ff;

		// Loop For Every Line
LINE_LOOP:	
		__asm push ecx;
		__asm mov ecx, [esp+12];			// Line Width
		__asm sub ecx, 16;
		__asm jl PIXEL_UNALIGNED;

		__asm add esi, ebx;
		__asm add edi, ebx;

// 		__asm mov edx, [esi];
// 		__asm mov edx, [edi];
// 
// 		__asm mov edx, [esi+0x40];
// 		__asm mov edx, [edi+0x40];
// 
// 		__asm mov edx, [esi+0x80];
// 		__asm mov edx, [edi+0x80];
// 
// 		__asm mov edx, [esi+0x100];
// 		__asm mov edx, [edi+0x100];
// 
// 		__asm mov edx, [esi+0x140];
// 		__asm mov edx, [edi+0x140];
// 
// 		__asm mov edx, [esi+0x180];
// 		__asm mov edx, [edi+0x180];

		__asm mov edx, dword ptr [esp+8];
		__asm mov dh,dl;
		// Loop For Every Pixel
PIXEL_ALIGNED_LOOP:
//////////////////////////////////////////////////////////////////////////
// 处理像素
//
		__asm movq mm0, qword ptr [esi];
		__asm movq mm4, qword ptr [edi];
		__asm movq mm3,mm0;
		__asm PSUBUSB mm0,mm4;
		__asm PSUBUSB mm4,mm3;
		__asm PADDUSB mm0,mm4;

		__asm movq mm1, qword ptr [esi+8];
		__asm movq mm5, qword ptr [edi+8];
		__asm movq mm3,mm1;
		__asm PSUBUSB mm1,mm5;
		__asm PSUBUSB mm5,mm3;
		__asm PADDUSB mm1,mm5;

		__asm movq mm2, qword ptr [esi+16];	
		__asm movq mm6, qword ptr [edi+16];
		__asm movq mm3,mm2;
		__asm PSUBUSB mm2,mm6;
		__asm PSUBUSB mm6,mm3;
		__asm PADDUSB mm2,mm6;

		__asm movq mm3, qword ptr [esi+24];
		__asm movq mm7, qword ptr [edi+24];
		
		__asm movq mm4,mm3;
		__asm PSUBUSB mm3,mm7;
		__asm PSUBUSB mm7,mm4;
		__asm PADDUSB mm3,mm7;

		__asm MOVD mm4, ebp;
		__asm PUNPCKLDQ mm4,mm4;// 0x00ff00ff00ff00ff

		__asm MOVD mm6,edx;
		__asm PUNPCKLWD mm6,mm6;
		__asm PUNPCKLDQ mm6,mm6;

		__asm MOVQ mm5,mm0;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm0,8h;
		__asm PAND mm0,mm4;
		__asm PADDUSW mm0, mm5;
		__asm PSRLW mm0,1h;

		__asm MOVQ mm5,mm1;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm1,8h;
		__asm PAND mm1,mm4;
		__asm PADDUSW mm1, mm5;
		__asm PSRLW mm1,1h;

		__asm MOVQ mm5,mm2;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm2,8h;
		__asm PAND mm2,mm4;
		__asm PADDUSW mm2, mm5;
		__asm PSRLW mm2,1h;

		__asm MOVQ mm5,mm3;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm3,8h;
		__asm PAND mm3,mm4;
		__asm PADDUSW mm3, mm5;
		__asm PSRLW mm3,1h;

		__asm PACKUSWB mm0,mm1;
		__asm PACKUSWB mm2,mm3;

		__asm PSUBUSB mm0,mm6;
		__asm PSUBUSB mm2,mm6;

#if _MSC_VER >1200
		__asm movntq qword ptr [eax],mm0;		
		__asm movntq qword ptr [eax+8],mm2;
#else
		__asm _EMIT 0x0F;
		__asm _EMIT 0xE7;
		__asm _EMIT 0x00;

		__asm _EMIT 0x0F;
		__asm _EMIT 0xE7;
		__asm _EMIT 0x50;
		__asm _EMIT 0x08;
#endif
//////////////////////////////////////////////////////////////////////////

		__asm add eax, 16;
		__asm add esi, 32;
		__asm add edi, 32;

		__asm sub ecx, 16;
		__asm jge PIXEL_ALIGNED_LOOP;

PIXEL_UNALIGNED:
		__asm add ecx, 16;
		__asm je CONTINUE;

		__asm test ecx, 8;		
		__asm je ONE_PIXEL;

		__asm movq mm0, qword ptr [esi];
		__asm movq mm1, qword ptr [esi+8];

		__asm movq mm2, qword ptr [edi];
		__asm movq mm3, qword ptr [edi+8];

		__asm movq mm4,mm0;
		__asm PSUBUSB mm0,mm2;
		__asm PSUBUSB mm2,mm4;
		__asm PADDUSB mm0,mm2;

		__asm movq mm4,mm1;
		__asm PSUBUSB mm1,mm3;
		__asm PSUBUSB mm3,mm4;
		__asm PADDUSB mm1,mm3;

		__asm MOVD mm4, ebp;
		__asm PUNPCKLDQ mm4,mm4;// 0x00ff00ff00ff00ff

		__asm MOVD mm6,edx;
		__asm PUNPCKLWD mm6,mm6;
		__asm PUNPCKLDQ mm6,mm6;

		__asm MOVQ mm5,mm0;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm0,8h;
		__asm PAND mm0,mm4;
		__asm PADDUSW mm0, mm5;
		__asm PSRLW mm0,1h;

		__asm MOVQ mm5,mm1;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm1,8h;
		__asm PAND mm1,mm4;
		__asm PADDUSW mm1, mm5;
		__asm PSRLW mm1,1h;

		__asm PACKUSWB mm0,mm1;
		__asm PSUBUSB mm0,mm6;
#if (_MSC_VER > 1200)
		__asm movntq qword ptr [eax], mm0;
#else
		__asm _EMIT 0x0F;
		__asm _EMIT 0x7E;
		__asm _EMIT 0x00;
#endif

		__asm add eax, 8;
		__asm add esi, 16;
		__asm add edi, 16;
ONE_PIXEL:
		__asm test ecx, 4;
		__asm je CONTINUE;

		__asm movq mm0, qword ptr [esi];
		__asm movq mm1, qword ptr [edi];

		__asm movq mm2,mm0;
		__asm PSUBUSB mm0,mm1;
		__asm PSUBUSB mm1,mm2;
		__asm PADDUSB mm0,mm1;

		__asm MOVD mm4, ebp;

		__asm MOVD mm6,edx;
		__asm PUNPCKLWD mm6,mm6;

		__asm MOVQ mm5,mm0;
		__asm PAND mm5,mm4;
		__asm PSRLQ mm0,8h;
		__asm PAND mm0,mm4;
		__asm PADDUSW mm0, mm5;
		__asm PSRLW mm0,1h;
		
		__asm PACKUSWB mm0,mm4;
		__asm PSUBUSB mm0,mm6;
		__asm movd dword ptr [eax], mm0;

		__asm add eax, 4;
		__asm add esi, 8;
		__asm add edi, 8;
//////////////////////////////////////////////////////////////////////////
CONTINUE:
		__asm pop ecx;
		__asm dec ecx;
		__asm jge LINE_LOOP;

		__asm pop ebp;
#if (_MSC_VER > 1200)
		__asm sfence;
#else
		__asm _EMIT 0x0F;
		__asm _EMIT 0xAE;
		__asm _EMIT 0xF8;
#endif
LEAVE_LOOP:
//////////////////////////////////////////////////////////////////////////
// 分析结果  [4/22/2010 wang.zh]
//		
		__asm xor eax, eax
		__asm mov ecx, dword ptr [nHeight];	// Line Count
		__asm dec ecx;
		__asm jl CHECK_RESULT;

		__asm mov edi, dword ptr [pDst];		// 像素差内存块

		__asm push dword ptr [nPercentage];
		__asm push dword ptr [nHeight];

		__asm push ebp;
		__asm mov ebp, esp;

#define LINES		dword ptr [ebp+4]		// 行数
#define PERCENTAGE	dword ptr [ebp+8]		// 不同像素数量
#define LINE_WIDTH	dword ptr [ebp+16]		// 每行的像素数量

#define MAX_VALUE	dword ptr [ebp-4]
#define PARAM_X		dword ptr [ebp-8]
#define PARAM_Y		dword ptr [ebp-12]
#define PARAM_P		dword ptr [ebp-16]
#define TOTAL_FIND	dword ptr [ebp-20]

		__asm push eax;//临时变量
		__asm push eax;
		__asm push eax;
		__asm push eax;	
		__asm push eax;

		__asm mov eax, LINE_WIDTH;			// 保存行宽
		__asm xor ebx,ebx;					// 内存偏移量

FIND_LINE_LOOP:
#define LINE_INDEX	dword ptr [ebp-24]		// 行索引
		__asm push ecx;

		__asm mov ecx, eax;				// Line Width
		__asm dec ecx;
		__asm jl LEAVE_FIND_PIXEL_LOOP;

// 		//Prefetch data
// 		__asm mov edx, [edi+ebx];
// 		__asm mov edx, [edi+ebx+0x40];
// 		__asm mov edx, [edi+ebx+0x80];
// 		__asm mov edx, [edi+ebx+0x100];
// 		__asm mov edx, [edi+ebx+0x140];
// 		__asm mov edx, [edi+ebx+0x180];

FIND_PIXEL_LOOP:
		__asm push eax;
		__asm push edx;
		__asm xor eax,eax;
		__asm xor edx,edx;	// 保存像素数量
		__asm mov MAX_VALUE, edx;

//////////////////////////////////////////////////////////////////////////
		__asm mov esi,ebx;
		__asm add esi,ecx;
		__asm mov al, byte ptr [edi+esi];
		__asm test al,al;
		__asm je FIND_FINISH;

		__asm mov byte ptr[edi+esi], ah;	// 已经读取的像素置为零
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX;
		__asm mov MAX_VALUE, eax;
JMP_SET_MAX:

		__asm inc edx;		// 数量++

		__asm push edi;
		__asm push ecx;
		__asm push ebx;
		__asm mov ebx,esp;	// 保存栈指针

		__asm add esi,edi;
		__asm push esi;			// 内存指针
		__asm mov eax, LINE_INDEX;
		__asm SHL eax, 010h;
		__asm push eax;							// 行索引
		__asm mov word ptr [esp], cx;			// 列索引

		__asm mov esi,[ebp-28];	// 图像宽
		__asm mov edi,LINES;	// 图像高
		__asm dec esi;
		__asm dec edi;

FIND_LOOP_START:
		// edx,数量;
		// ebx 栈指针
		// ebp 栈指针
		// esi 图像宽
		// edi 图像高

		// 可用：eax,ecx
		__asm mov ecx, dword ptr [esp+4];	// 内存指针
		__asm mov PARAM_P, ecx;

		__asm mov eax, dword ptr [esp];
		__asm and eax, 0ffffh;
		__asm mov PARAM_X, eax;

		__asm mov eax, dword ptr [esp];
		__asm SHR eax, 010h;
		__asm mov PARAM_Y,eax;

		__asm add esp, 08h;

//////////////////////////////////////////////////////////////////////////
// TRY_RIGHT:
		__asm cmp esi,PARAM_X;
		__asm jle TRY_LEFT;

		__asm inc ecx;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al, al;
		__asm je TRY_RIGHT_TOP;

		__asm mov byte ptr[ecx],ah;	// 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_1;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_1:
		__asm push ecx;	


		__asm mov eax, PARAM_Y;
		__asm SHL eax, 010h;
		__asm push eax;
		
		__asm mov eax, PARAM_X;		// 列索引
		__asm inc eax;
		__asm mov word ptr[esp], ax;

TRY_RIGHT_TOP:
		__asm mov eax,PARAM_Y;
		__asm cmp eax,0;
		__asm jle TRY_RIGHT_BOTTOM;

		__asm add ecx,esi;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al,al;
		__asm je TRY_RIGHT_BOTTOM;

		__asm mov byte ptr[ecx],ah;	// 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_2;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_2:
		__asm push ecx;	
		
		__asm mov eax, PARAM_Y;		// 列索引
		__asm dec eax;
		__asm SHL eax,010h;
		__asm push eax;

		__asm mov eax,PARAM_X;
		__asm inc eax;
		__asm mov word ptr [esp], ax;

TRY_RIGHT_BOTTOM:
		__asm mov eax,PARAM_Y;
		__asm cmp eax,edi;
		__asm jge TRY_LEFT;

		__asm mov ecx, PARAM_P;
		__asm sub ecx,esi;
		__asm inc ecx;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al,al;
		__asm je TRY_LEFT;

		__asm mov byte ptr[ecx],ah; // 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_3;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_3:
		__asm push ecx;	
		
		__asm mov eax, PARAM_Y;		// 列索引
		__asm inc eax;
		__asm SHL eax,010h;
		__asm push eax;

		__asm mov eax, PARAM_X;
		__asm inc eax;
		__asm mov word ptr [esp], ax;

TRY_LEFT:
		__asm mov eax,PARAM_X;
		__asm cmp eax,0;
		__asm jle TRY_TOP;

		__asm mov ecx, PARAM_P;
		__asm dec ecx;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al, al;
		__asm je TRY_LEFT_TOP;

		__asm mov byte ptr[ecx],ah; // 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_4;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_4:
		__asm push ecx;
		__asm mov eax, PARAM_Y;
		__asm SHL eax,010h;
		__asm push eax;	
		
		__asm mov eax, PARAM_X;		// 列索引
		__asm dec eax;
		__asm mov word ptr [esp], ax;


TRY_LEFT_TOP:
		__asm mov eax,PARAM_Y;
		__asm cmp eax,0;
		__asm jle TRY_LEFT_BOTTOM;

		__asm mov ecx, PARAM_P;
		__asm add ecx,esi;
		__asm dec ecx;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al, al;
		__asm je TRY_LEFT_BOTTOM;

		__asm mov byte ptr[ecx],ah; // 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_5;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_5:
		__asm push ecx;	
		
		__asm mov eax, PARAM_Y;		// 列索引
		__asm dec eax;
		__asm SHL eax,010h;
		__asm push eax;

		__asm mov eax, PARAM_X;
		__asm dec eax;
		__asm mov word ptr [esp], ax;

TRY_LEFT_BOTTOM:
		__asm mov eax,PARAM_Y;
		__asm cmp eax,edi;
		__asm jge TRY_TOP;

		__asm mov ecx, PARAM_P;
		__asm sub ecx,esi;
		__asm dec ecx;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al,al;
		__asm je TRY_TOP;

		__asm mov byte ptr[ecx],ah; // 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_6;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_6:
		__asm push ecx;	
		
		__asm mov eax, PARAM_Y;		// 列索引
		__asm inc eax;
		__asm SHL eax,010h;
		__asm push eax;

		__asm mov eax, PARAM_X;
		__asm dec eax;
		__asm mov word ptr [esp], ax;
TRY_TOP:
		__asm mov eax,PARAM_Y;
		__asm cmp eax,0;
		__asm jbe TRY_BOTTOM;

		__asm mov ecx, PARAM_P;
		__asm add ecx,esi;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al, al;
		__asm je TRY_BOTTOM;

		__asm mov byte ptr[ecx],ah; // 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_7;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_7:
		__asm push ecx;	
		
		__asm mov eax, PARAM_Y;		// 列索引
		__asm dec eax;
		__asm SHL eax, 010h;
		__asm push eax;
		
		__asm mov eax, PARAM_X;
		__asm mov word ptr [esp], ax;
		
TRY_BOTTOM:
		__asm mov eax,PARAM_Y;
		__asm cmp eax,edi;
		__asm jge TRY_END;

		__asm mov ecx, PARAM_P;
		__asm sub ecx,esi;
		__asm xor eax,eax;
		__asm mov al, byte ptr[ecx];
		__asm test al,al;
		__asm je TRY_END;

		__asm mov byte ptr[ecx],ah; // 已经读取的像素置为零
		__asm inc edx;
		__asm cmp eax, MAX_VALUE;
		__asm jb JMP_SET_MAX_8;
		__asm mov MAX_VALUE, eax;

JMP_SET_MAX_8:
		__asm push ecx;	
		
		__asm mov eax, PARAM_Y;		// 列索引
		__asm inc eax;
		__asm SHL eax, 010h;
		__asm push eax;

		__asm mov eax, PARAM_X;
		__asm mov word ptr [esp], ax;

//////////////////////////////////////////////////////////////////////////
TRY_END:
		//////////////////////////////////////////////////////////////////////////
		//  [6/25/2010 wang.zh] 判断是否已经不是静帧

		//////////////////////////////////////////////////////////////////////////
		//  [6/30/2010 wang.zh]
		// 如果数量大于0x400 ,跳出循环，防止栈溢出
		__asm cmp edx,0400h;
		__asm jge STACK_OVER_FLOW;
		//////////////////////////////////////////////////////////////////////////

		__asm cmp edx,0bh;
		__asm jb JMP_JUDGE;
		__asm cmp MAX_VALUE, 05h;
		__asm jb JMP_JUDGE;

STACK_OVER_FLOW:
		__asm mov eax, TOTAL_FIND;
		__asm add eax,edx;
		__asm cmp eax, PERCENTAGE;
		__asm jb JMP_JUDGE;

		__asm mov esp, ebx;
		__asm pop ebx;
		__asm pop ecx;
		__asm pop edi;
		__asm jmp JMP_OUT;
		//////////////////////////////////////////////////////////////////////////
JMP_JUDGE:
		__asm cmp ebx,esp;
		__asm jne FIND_LOOP_START;
		__asm pop ebx;
		__asm pop ecx;
		__asm pop edi;
FIND_FINISH:
		__asm cmp edx,0bh;		// 像素块大小11
		__asm jb CONTINUE_FIND;
		__asm mov eax, MAX_VALUE;
		__asm cmp eax, 05h;		//像素差最大值
		__asm jb CONTINUE_FIND;
//////////////////////////////////////////////////////////////////////////
//Find Result 
//
		__asm mov eax,TOTAL_FIND;
		__asm add eax,edx;
		__asm cmp eax, PERCENTAGE;
		__asm jae JMP_OUT;
		__asm mov TOTAL_FIND, eax;

//////////////////////////////////////////////////////////////////////////
CONTINUE_FIND:
//////////////////////////////////////////////////////////////////////////
		__asm pop edx;
		__asm pop eax;
		__asm dec ecx;
		__asm jge FIND_PIXEL_LOOP;

		__asm add ebx, eax;
LEAVE_FIND_PIXEL_LOOP:

		__asm pop ecx;
		__asm dec ecx;
		__asm jge FIND_LINE_LOOP;

		__asm xor eax,eax;
		__asm jmp LEAVE_FIND_LOOP;

JMP_OUT:
		__asm add esp, 0ch; // edx, eax, ecx

LEAVE_FIND_LOOP:
		__asm mov ebp, [ebp];
		__asm add esp, 020h; // 014h for local variable nPercentage, nHeight, ebp

CHECK_RESULT:
		__asm add esp, 08h; // nAbmit, nWidth
		__asm test eax,eax;
		__asm jne RETURN_ZERO;

//////////////////////////////////////////////////////////////////////////

#undef LINE_WIDTH
#undef PERCENTAGE
#undef LINES		
#undef MAX_VALUE
#undef PARAM_X
#undef PARAM_Y
#undef PARAM_P
#undef TOTAL_FIND
#undef LINE_INDEX
		m_VideoResult |= (type & DETECT_TYPE_STATIC_FRAME);
		__asm {emms}
		return DETECT_TYPE_STATIC_FRAME;

RETURN_ZERO:
		m_pfnMemCpy (pFrameBuffer, pBuffer, nBufferLen );
		__asm {emms}
		return 0;
	}
}

//彩场检测
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度.
//参数: type: 要检测的类型.默认值:彩场.
//返回: 此桢是否是彩场.

DETECT_TYPE YUY2_Detect::ColorSceneDetect(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type)
{ 
 	DWORD* StopP =(DWORD*) (pBuffer+nBufferLen);
//	DWORD* p=(DWORD*)pBuffer;
	DWORD LeftSafeSectionPixel = DWORD(*CLIP_WIDTH.value * *LEFT_SECURE_SECTION.value);
	DWORD RightSafeSectionPixel = DWORD(*CLIP_WIDTH.value * *RIGHT_SECURE_SECTION.value);
	int s1=0,s2=0;
	//////////////////////////////////////////////////////////////////////////
	//由于接下来的检测过程以两个像素为一个单位进行,此处有必要进行偶数化.
	//
	// by wang.zh 2008.10.22
	LeftSafeSectionPixel &= ~((DWORD)1);
	RightSafeSectionPixel&= ~((DWORD)1);
	// end  wang.zh 2008.10.22
	
	DWORD nPixels=0, RightSafeSecitonLine = *CLIP_WIDTH.value - RightSafeSectionPixel;
    // 此处nRightSafeSecitonLine不需要进行偶数化,因为对于YUY2格式的图像,宽度不可能是奇数.
	DWORD* p=(DWORD*)(pBuffer + LeftSafeSectionPixel * YUY2_BYTES_PER_PIXEL );
	nPixels = LeftSafeSectionPixel;
	while (p<StopP)
	{	
		if (nPixels==RightSafeSecitonLine)
		{
			p=(DWORD*)((BYTE*)(p)+(RightSafeSectionPixel+LeftSafeSectionPixel)*YUY2_BYTES_PER_PIXEL);
			nPixels=LeftSafeSectionPixel;;
			if (p>=StopP)
			{
				break;
			}
		}
		
		nPixels++;
		nPixels++;
		
		lineHash[0][BYTE( *p&0x000000ff)     ]++;
		lineHash[0][BYTE((*p&0x00ff0000)>>16)]++;
		lineHash[1][BYTE((*p&0x0000ff00)>> 8)]++;
		lineHash[2][BYTE((*p&0xff000000)>>24)]++;

		p++;
	}
	
	//get the max count pixel;
	while(lineHashCursor<256)
	{
		if(lineHash[0][lineHashCursor]>maxPixelCount[0])
		{
			maxPixelCount[0]=lineHash[0][lineHashCursor];
			maxPixelCursor[0] = lineHashCursor;
		}
		
		if(lineHash[1][lineHashCursor]>maxPixelCount[1])
		{
			maxPixelCount[1]=lineHash[1][lineHashCursor];
			maxPixelCursor[1] = lineHashCursor;
		}
		
		if(lineHash[2][lineHashCursor]>maxPixelCount[2])		
		{
			maxPixelCount[2]=lineHash[2][lineHashCursor];
			maxPixelCursor[2] = lineHashCursor;
		}
		lineHashCursor++;
	}
	
	memset(maxPixelCount,0,sizeof(maxPixelCount));
	

	//count the total pixels;
	s1 = maxPixelCursor[0] - *UNDULATE_AMBIT_OF_Y.value;
	s2 = maxPixelCursor[0] + *UNDULATE_AMBIT_OF_Y.value;
	s1 = max(0, s1);
	s2 = min(s2, 255);
	for(lineHashCursor = s1; lineHashCursor<=s2; lineHashCursor++)
	{
		maxPixelCount[0]+=lineHash[0][lineHashCursor];		
	}

	s1 = maxPixelCursor[1] - *UNDULATE_AMBIT_OF_U.value;
	s2 = maxPixelCursor[1] + *UNDULATE_AMBIT_OF_U.value;
	s1 = max(0, s1);
	s2 = min(s2, 255);
	for(lineHashCursor = s1; lineHashCursor <=s2; lineHashCursor++)
	{
		maxPixelCount[1]+=lineHash[1][lineHashCursor];
	}

	s1 = maxPixelCursor[2] - *UNDULATE_AMBIT_OF_V.value;
	s2 = maxPixelCursor[2] + *UNDULATE_AMBIT_OF_V.value;
	s1 = max(0, s1);
	s2 = min(s2, 255);
	for(lineHashCursor = s1; lineHashCursor<=s2; lineHashCursor++)
	{
		maxPixelCount[2]+=lineHash[2][lineHashCursor];		
	}

	DWORD nDetectedPixels = (*CLIP_WIDTH.value - RightSafeSectionPixel - LeftSafeSectionPixel) * (nBufferLen/(*CLIP_WIDTH.value*YUY2_BYTES_PER_PIXEL));
	
	if(	(float)maxPixelCount[0]/nDetectedPixels      >=*COLOR_SCENE_PERCENTAGE.value&&
		(float)maxPixelCount[1]/(nDetectedPixels / 2)>=*COLOR_SCENE_PERCENTAGE.value&&
		(float)maxPixelCount[2]/(nDetectedPixels / 2)>=*COLOR_SCENE_PERCENTAGE.value)
	{
		if( (maxPixelCursor[0]<=(*BLACK_SCENE_THRESHOLD.value)) || 
			//////////////////////////////////////////////////////////////////////////
			//  [9/27/2009 wang.zh] 添加色差的判断, 如果没有颜色且亮度小于80，认为是黑场
			( maxPixelCursor[0] <= 60 
			&& maxPixelCursor[1] >= 128 - *BLACK_SCENE_THRESHOLD.value
			&& maxPixelCursor[1] <= 128 + *BLACK_SCENE_THRESHOLD.value
			&& maxPixelCursor[2] >= 128 - *BLACK_SCENE_THRESHOLD.value
			&& maxPixelCursor[2] <= 128 + *BLACK_SCENE_THRESHOLD.value ))
			//////////////////////////////////////////////////////////////////////////
		{
			m_VideoResult|=(type&DETECT_TYPE_BLACK_SCENE);
			return DETECT_TYPE_BLACK_SCENE;
		}
		else
		{
			m_VideoResult|=(type&DETECT_TYPE_COLOR_SCENE);
			return DETECT_TYPE_COLOR_SCENE;
		}
	}
	return 0;
}

//彩条检测的另一种实现方式: 通过边界检测方式实现.
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度.
//参数: type: 要检测的类型.默认值:彩条.
//返回: 此桢是否是彩条.

DETECT_TYPE YUY2_Detect::_ColorStripDetect(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type/* =DETECT_TYPE_STATIC_FRAME */)
{
	DWORD Rows = (nBufferLen/YUY2_BYTES_PER_PIXEL)/(*CLIP_WIDTH.value);
	long  RowInterval =Rows/(*LINE_COUNTS_FOR_STRIP_DETECT.value)*(*CLIP_WIDTH.value*YUY2_BYTES_PER_PIXEL);

	WORD NonStripLineCounts =WORD(*LINE_COUNTS_FOR_STRIP_DETECT.value*(1-*COLOR_STRIP_PERCENTAGE.value));
	WORD currentNonStripLine=0;
	
	BYTE* p_u_t = new BYTE[nBufferLen],*p = p_u_t;
	if(!p)//wangjun 20081001
	{
		MessageBox(NULL,"_ColorStripDetect()分配内存出错!","AotuDetect",MB_OK);
		return 0;
	}
	memcpy(p,pBuffer,nBufferLen);
	//BYTE* p = pBuffer;
	BYTE* StopP = p+nBufferLen;

	Canny(p,*CLIP_WIDTH.value,Rows,10,3);
	while( p<StopP )
	{
		if(isColorStripLine(p,*CLIP_WIDTH.value)==FALSE)
		{
			currentNonStripLine++;
			
			if (currentNonStripLine>NonStripLineCounts)
			{
				delete p_u_t;
				p_u_t = NULL;
				return 0;
			}
		}
		
		p+=RowInterval;
	}

	delete p_u_t;
	p_u_t = NULL;
	
	p = NULL;

	m_VideoResult|=(type&DETECT_TYPE_COLOR_STRIP);

	return DETECT_TYPE_COLOR_STRIP;

}

//检测一桢图像是否发生了偏色 by-TTW
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度
//先检测1%小图，发生可疑偏色时检测大图
//################### 注意 ############### 当素材每帧全是是灰白类型的时候，会一直进行全图检测。
DETECT_TYPE YUY2_Detect::EQRoundCastDetection(BYTE* pBuffer,long nBufferLen,DETECT_TYPE type)
{
	BYTE* YUVImage = pBuffer;
	BYTE* pStop = pBuffer + nBufferLen;  
	int ImageWitdh = *CLIP_WIDTH.value;
	int ImageHeight = *CLIP_HEIGHT.value;
	double CastD_uU=0;
	double CastD_uV=0;
	double CastD_uY=0;
	double CastD_u=0;
	double CastD_Delta_U=0;
	double CastD_Delta_V=0;
	double CastD_Delta_Y=0;
	double CastD_Delta=0;
	double CastD_Distance=0;
	int Pixelnumber=0;
	int GrayPixel=0;
	int NormalRegion=0;
	
    unsigned char* CastDetection_Y=new unsigned char[ImageHeight*ImageWitdh];
	unsigned char* CastDetection_U=new unsigned char[ImageHeight*ImageWitdh];
	unsigned char* CastDetection_V=new unsigned char[ImageHeight*ImageWitdh];
	
	int SmallpictureDetectionResult=0;

	Pixelnumber=ImageHeight*ImageWitdh;
	//以下是对大图的检测，将图像缩小96倍进行检测，对1000*1000的时间复杂度在>10ns左右。
	if (Pixelnumber>=30000)
	{
		int SmallPixelnumber=0;;
		int SmallImageWitdh;
		SmallImageWitdh =(int) ((ImageWitdh-1)/8);
		for(int i = 0; YUVImage < pStop; YUVImage += 16, i ++) //间隔8个点
		{
			CastDetection_Y[i] = *YUVImage;
			CastDetection_U[i] = *(YUVImage+1);
			CastDetection_V[i] = *(YUVImage+3);
			CastD_uU+=(double)CastDetection_U[i];
			CastD_uV+=(double)CastDetection_V[i];
			CastD_uY+=(double)CastDetection_Y[i];
			SmallPixelnumber++;
			if (i && i%SmallImageWitdh==0)
			{
				YUVImage += 12*ImageWitdh;    //取满一小行向下数12行
			}
		}

		CastD_uU=CastD_uU/SmallPixelnumber;
		CastD_uV=CastD_uV/SmallPixelnumber;
		CastD_uY=CastD_uY/SmallPixelnumber;
		CastD_u=sqrt(CastD_uU*CastD_uU+CastD_uV*CastD_uV);
		
		for(i=0;i<SmallPixelnumber;i++)
		{
			
			CastD_Delta_U+=(CastD_uU-CastDetection_U[i])*(CastD_uU-CastDetection_U[i]);
			CastD_Delta_V+=(CastD_uV-CastDetection_V[i])*(CastD_uV-CastDetection_V[i]);
			CastD_Delta_Y+=(CastD_uY-CastDetection_Y[i])*(CastD_uY-CastDetection_Y[i]);
			
		}
		CastD_Delta_U/=SmallPixelnumber;
		CastD_Delta_V/=SmallPixelnumber;
		CastD_Delta_Y/=SmallPixelnumber;
		CastD_Delta=sqrt(CastD_Delta_U+CastD_Delta_V);
		CastD_Distance=CastD_u-CastD_Delta;
		/*	CastD_Distance_Delta=CastD_Distance/CastD_Delta;*/
		
		if (CastD_Delta<3.5)
		{
			if (CastD_Delta_Y>10.0)
			{
// 				printf("这幅图的缩小区域发生偏色了！\n");
// 				printf("CastDelta:%f CastDelta-Y:%f\n\n",CastD_Delta,CastD_Delta_Y);
				SmallpictureDetectionResult=1;
			}
			else
			{
// 				printf("这幅图片的缩小区域是近乎纯色.\n");
// 				printf("CastDelta:%f CastDelta-Y:%f\n\n",CastD_Delta,CastD_Delta_Y);
				SmallpictureDetectionResult=1;
			}
		}
		else if (CastD_Delta<7.0)
		{
// 			printf("这幅图的缩小区域已被怀疑，将进一步进行全图检测！\n");
// 			printf("CastDelta:%f CastDelta-Y:%f\n\n",CastD_Delta,CastD_Delta_Y);
            SmallpictureDetectionResult=1;
		}
		else
		{
// 			printf("这幅图像的缩小区域未发生偏色！\n");
// 			printf("CastDelta:%f CastDelta-Y:%f\n\n",CastD_Delta,CastD_Delta_Y);
			delete [] CastDetection_Y;
			delete [] CastDetection_U;
	        delete [] CastDetection_V;
			return 0;
		}
	}
	//以下是对整个图像的检测，时间复杂度：1000*1000大约在200 ns
	if ((Pixelnumber<30000)||SmallpictureDetectionResult)
	{
		int SurprisingPixel=0; //该帧中与主色不同的奇异点数目。
// 		if (Pixelnumber<30000)
// 		{
// 			printf("总像素小于30000，直接全图检测。\n");
// 		}
		Pixelnumber=0;
		YUVImage = pBuffer;
		for(int i = 0; YUVImage < pStop-4; YUVImage += 4, i += 2)
		{
			CastDetection_Y[i] = *YUVImage;
			CastDetection_Y[i+1] = *(YUVImage+2);
			CastDetection_U[i] = CastDetection_U[i+1] = *(YUVImage+1);
			CastDetection_V[i] = CastDetection_V[i+1] = *(YUVImage+3);
			CastD_uU+=2.0*(double)CastDetection_U[i];
			CastD_uV+=2.0*(double)CastDetection_V[i];
			CastD_uY+=(double)CastDetection_Y[i]+(double)CastDetection_Y[i+1];
			Pixelnumber+=2;
			if ((double)CastDetection_U[i]<131.1 && (double)CastDetection_U[i]>125.9) //此处黑白灰门限可小至偏离128.5约1.5
			{
				if ((double)CastDetection_V[i]<131.1 && (double)CastDetection_V[i]>125.9)
				{
					GrayPixel+=2;
				}
			}
			if(CastDetection_Y[i]>59 && CastDetection_Y[i]<201)
			{
				NormalRegion++;
			}
			if(CastDetection_Y[i+1]>59 && CastDetection_Y[i+1]<201)
			{
				NormalRegion++;
			
			}
		}
		if ((double)NormalRegion/Pixelnumber<0.2) //非黑白部分不到20%
		{
			delete [] CastDetection_Y;
			delete [] CastDetection_U;
			delete [] CastDetection_V;
			return 0; //信息不够
		}
		
		if ((double)GrayPixel/Pixelnumber>0.95) //黑白灰匹配度达95%以上
		{
			delete [] CastDetection_Y;
			delete [] CastDetection_U;
	        delete [] CastDetection_V;
			return 0; //printf("这是一幅黑白灰图片，没有发生偏色.\n");
		}
		CastD_uU=CastD_uU/Pixelnumber;
		CastD_uV=CastD_uV/Pixelnumber;
		CastD_uY=CastD_uY/Pixelnumber;
		CastD_u=sqrt(CastD_uU*CastD_uU+CastD_uV*CastD_uV);
		
		for(i=0;i<Pixelnumber;i++)
		{
			
			CastD_Delta_U+=(CastD_uU-(double)CastDetection_U[i])*(CastD_uU-(double)CastDetection_U[i]);
			CastD_Delta_V+=(CastD_uV-(double)CastDetection_V[i])*(CastD_uV-(double)CastDetection_V[i]);
			CastD_Delta_Y+=(CastD_uY-(double)CastDetection_Y[i])*(CastD_uY-(double)CastDetection_Y[i]);
			double Cast_absU=((CastD_uU-(double)CastDetection_U[i])>0) ? (CastD_uU-(double)CastDetection_U[i]) : (-CastD_uU+(double)CastDetection_U[i]);
			double Cast_absV=((CastD_uV-(double)CastDetection_V[i])>0) ? (CastD_uV-(double)CastDetection_V[i]) : (-CastD_uV+(double)CastDetection_V[i]);
			if ((Cast_absU+Cast_absV)>10) //奇异点的阈值设置
			{
				SurprisingPixel++;
			}
		}
		CastD_Delta_U/=Pixelnumber;
		CastD_Delta_V/=Pixelnumber;
		CastD_Delta_Y/=Pixelnumber;
		CastD_Delta=sqrt(CastD_Delta_U+CastD_Delta_V);
		CastD_Distance=CastD_u-CastD_Delta;

		CastD_Delta=CastD_Delta*pow((1+1.0*(double)SurprisingPixel/(double)Pixelnumber),16); //16次方将奇异点的门限定在0.05
		
		if (CastD_Delta<3.6)// 偏色检测门限推荐在[2-6]之间，越高则越检测力度越大
		{
			if (CastD_Delta_Y>30) //纯色度参数调整
			{
				if (1.0*SurprisingPixel/Pixelnumber<0.05)//显示门限
				{
					delete [] CastDetection_Y;
					delete [] CastDetection_U;
					delete [] CastDetection_V;
					m_VideoResult|=(type&DETECT_TYPE_COLOR_CAST);
				    return DETECT_TYPE_COLOR_CAST;//printf("经过完整检测，这帧发生偏色了！\n");
				}
				else
				{
					delete [] CastDetection_Y;
					delete [] CastDetection_U;
					delete [] CastDetection_V;
				    return 0;
				}
			}
			else
			{
				delete [] CastDetection_Y;
				delete [] CastDetection_U;
				delete [] CastDetection_V;
				return 0;//printf("经过完整检测，这帧是近乎纯色.\n");
			}
		}
		else
		{
			delete [] CastDetection_Y;
			delete [] CastDetection_U;
	        delete [] CastDetection_V;
			return 0;//printf("经过完整检测，这帧未发生偏色.\n");
		}
	}
	delete [] CastDetection_Y;
	delete [] CastDetection_U;
	delete [] CastDetection_V;
	return 0;//printf("要是走到这里就出问题了哦 = =...\n");
}

//析取出一行数据的边界信息
//参数: pBuffer:一行数据的内存地址;
//参数: nBufferLen:一行数据的长度.
//参数: margin:两边的安全区,单位是BYTE
//返回: 存有边界信息的链表

Separator* YUY2_Detect::SplitLine(BYTE* pBuffer,DWORD nBufferLen,int margin)
{
	DWORD* p =(DWORD*)(pBuffer+margin*4);
	DWORD* pSotp= (DWORD*)(pBuffer+nBufferLen-margin*4);
	int c = 0;
	DWORD cursor = 0;
	Separator* pHead = new Separator;
	pHead->next = NULL;
	Separator* pNew = NULL;

	while(p<pSotp)
	{
		if ((BYTE)(((*p)&0x0000ff00)>>8)== 0x10)
		{
			if (c == 0)
			{
				pNew = new Separator;
				pNew->next = pHead->next;
				pNew->data=((cursor+margin)<<16);
			}
			c++;
		}
		else if ((BYTE)(((*p)&0x0000ff00)>>8) == 0xeb)
		{
			if (c >= 1 )
			{
				pNew->data|=c&0xffff;
				pHead->next = pNew;	
			}
			c = 0;
		}
		p++;
		cursor++;
	}
	if (c >= 1 )
	{
		pNew->data|=c&0xffff;
		pHead->next = pNew;	
	}

	pNew = pHead->next;
	delete pHead;
	return pNew;
}

//2008-7-3
//内存泄露问题
//王召

//判断一行数据是否是彩条
//参数: pLine:一行数据的内存地址
//参数: nLineWidth:一行数据的宽度
//返回: 是否是彩条

BOOL YUY2_Detect::isColorStripLine(BYTE* pLine,DWORD nLineWidth)
{
	Separator* pHead = SplitLine(pLine,nLineWidth*YUY2_BYTES_PER_PIXEL,20);
	if (!pHead) return FALSE;
	else 
	{
		int nSepCount = 0;
		Separator* p = pHead;
		while (p)
		{
			nSepCount ++;
			p = p->next;
		}
		
		if (nSepCount < STRIP_COUNTS-1) 
		{
			while (pHead)
			{
				p = pHead;
				pHead = p->next;
				delete p;
			}
			return FALSE;
		}
		else
		{
			DWORD SepCur[STRIP_COUNTS-1] = {0};
			Separator* pre = NULL;
			Separator* pMax = NULL;
			int n = 0;
			for (n = 0; n < STRIP_COUNTS-1; n++)
			{
				pre = NULL;
				p = pHead;
				while (p)
				{
					if (LOWORD(p->data)>LOWORD(SepCur[n]))
					{
						SepCur[n] = p->data;
						pMax = pre;
					}
					pre = p;
					p = p->next;
				}
				if (pMax == NULL)
				{
					p = pHead->next;
					delete pHead;
					pHead = p;
				}
				else
				{
					p = pMax->next;
					pMax->next = p->next;
					pMax = NULL;
					delete p;
					p = NULL;
				}
			}
			p = pHead;
			Separator* p2 = NULL;
			while (p)
			{
				p2 = p;
				p = p->next;
				delete p2;
				p2 = NULL;
			}
			qsort((void*)SepCur,STRIP_COUNTS - 1 ,sizeof(DWORD),DWORDCompare);
			
			int s[STRIP_COUNTS];
			int ts = 0;
			double average=0.0,sigma = 0.0;
			
			s[0] = HIWORD(SepCur[0]);
			ts += s[0];
			s[STRIP_COUNTS-1] = nLineWidth/2 - HIWORD(SepCur[STRIP_COUNTS-2]);
			ts+=s[STRIP_COUNTS-1];
			for (n = 1; n< STRIP_COUNTS-1; n++)
			{
				s[n] = HIWORD(SepCur[n])-HIWORD(SepCur[n-1]);
				ts+=s[n];
			}
			average = (double)ts/STRIP_COUNTS;
			for (n = 0; n < STRIP_COUNTS; n++)
			{
				sigma+=fabs(s[n]-average);
			}
			sigma = sigma/STRIP_COUNTS;
			if (sigma < 5)//wangjun 20081027
			{
				return TRUE;
			}
			else return FALSE;
		}
	}
}

//计算X方向导数
//参数: pImg:原始图像
//参数: nWidth,nHeight:原始图像的宽度,高度.
//参数: pnGradX,保存计算结果.

void YUY2_Detect::DirGrad(BYTE* pImg, int nWidth, int nHeight,BYTE *pnGradX)
{
	nWidth>>=1; // 将一个'YUYV'看成一个宏素.方便计算
	int y,cy = nHeight-1 ;
	int cx = nWidth -2;
	long pos;
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(y=1; y<cy; y++)
	{
		pos = y * nWidth + 1;
		int end = pos + cx;
		for(; pos < end; pos++)
		{
			pnGradX[pos] =abs( (int) ( pImg[((pos+1)<<2)+1]  
				- pImg[((pos-1)<<2)+1]));
		}
	}
}

// 门限运算,
//参数: pImg:保存二值图像结果
//参数: pImgEdge:图像导数
//参数: nWidth,nHeight,图像的宽度,高度
//参数: th 门限值

void YUY2_Detect::NonmaxSuppress(BYTE* pImg,BYTE* pImgEdge, int nWidth, int nHeight,int th)
{
	int y;
	nWidth>>=1; // 将一个'YUYV'看成一个宏素.方便计算
	int cy = nHeight - 1 ;
	int cx = nWidth - 2 ;
	long pos;
	for(y = 1; y< cy; y ++ )
	{
		pos = y*nWidth+1;
		long end = pos + cx;
		for (; pos < end; pos ++ )
		{
			if (pImgEdge[ pos]<th)
			{
				pImg[(pos<<2)+1] = 0xeb;
			}
			else
			{
				pImg[(pos<<2)+1] = 0x10;
			}
		}
	}
}


//边界检测
//参数: pImg: 原始图像;检测结束保存边界信息.
//参数: nWidth,nHeight,图像的宽度,高度
//参数: th 门限值
//参数: SmoHei:中值滤波窗口的高度

void YUY2_Detect::Canny(BYTE* pImg, int nWidth, int nHeight,int th,int SmoHei)
{
	// 指向x方向导数的指针
	BYTE * pImgEdge  = new BYTE [(nWidth>>1)*nHeight];
	// 计算方向导数
	DirGrad(pImg, nWidth, nHeight, pImgEdge);
	// 应用non-maximum 抑制
	
	NonmaxSuppress(pImg,pImgEdge, nWidth, nHeight,th);
	
	MedFilt(pImg, nWidth, nHeight,1,SmoHei,3);
	// 释放内存
	delete pImgEdge;
	pImgEdge = NULL ;
}

//中值滤波
//参数: lpImgData: 需要滤波的图像
//参数: nWidth,nHeight,图像的宽度,高度
//参数: tWidth,tHeight,滤波窗口的宽度,高度
//参数: interval:滤波窗口拾取附近颜色的间隔

void YUY2_Detect::MedFilt(BYTE* lpImgData,DWORD Width,DWORD Height,DWORD tWidth,DWORD tHeight,int interval)
{
	if((tWidth%2==0)||(tHeight%2==0)) return ;
	
	Width>>=1; //将一个YUYV看成一个宏素.方便处理

	if((tWidth>Width)||(tHeight>Height)||(interval<1)) return;
	
	int bx = (tWidth>>1 )*interval+1,sx = Width  - bx;
	int by = (tHeight>>1)*interval+1,sy = Height - by;
	int tx,tbx = bx-1,tsx;
	int ty,tby = by-1,tsy;
	BYTE* tem = new BYTE[tWidth*tHeight],* pTem=NULL;
	int x, y;
	
	for (y = by ;y<sy;y++)
	{
		for (x = bx ;x<sx;x++)
		{
			if (lpImgData[y*(Width<<2)+(x<<2)+1] == 0xeb) continue;
			pTem = tem;
			tsy = y + tby;
			tsx = x + tbx;
			for (ty = y-tby;ty<=tsy;ty+=interval)
			{
				for (tx=x-tbx;tx<=tsx;tx+=interval)
				{
					*pTem = lpImgData[ty*(Width<<2)+(tx<<2)+1]; //第二个BYTE是U分量.
					pTem++;
				}
			}
			lpImgData[y*(Width<<2)+(x<<2)+1] = GetMed(tem,tWidth,tHeight);
		}
	}
	delete tem;
	tem = NULL;

}
///////////////////////////////////////////////////////
//以下是YV12格式检测的实现
///////////////////////////////////////////////////////


//检测一桢图像
//参数: pBuffer:图像首地址
//参数: nBufferLen: 图像内存长度.
//


BOOL YV12_Detect::Detect(BYTE* pBuffer, long nBufferLen)
{

	//reset detect result
	VideoReset();
	count++;
	SendResult->VideoCount(count);
	p_y=pBuffer;
	p_v=pBuffer+*CLIP_WIDTH.value**CLIP_HEIGHT.value;
	p_u=p_v+*CLIP_WIDTH.value**CLIP_HEIGHT.value/4;

	p_y = p_y+int(*CLIP_HEIGHT.value * *TOP_SECURE_SECTION.value)* *CLIP_WIDTH.value;
	p_v = p_v+int(*CLIP_HEIGHT.value * *TOP_SECURE_SECTION.value/2)* *CLIP_WIDTH.value/2;
	p_u = p_u+int(*CLIP_HEIGHT.value * *TOP_SECURE_SECTION.value/2)* *CLIP_WIDTH.value/2;

	nPixels =*CLIP_WIDTH.value* *CLIP_HEIGHT.value-
		DWORD(*CLIP_HEIGHT.value * *TOP_SECURE_SECTION.value)* *CLIP_WIDTH.value-
		DWORD(*CLIP_HEIGHT.value* *BOTTOM_SECURE_SECTION.value)* *CLIP_WIDTH.value;

	timer[0].ResetLastTime();
	timer[1].ResetLastTime();
	timer[2].ResetLastTime();
	timer[3].ResetLastTime();

	timer[0].start();

	timer[1].start();
	nTColor++;
	DETECT_TYPE ret = ColorSceneDetect();
	timer[1].stop();

	if(ret);

	else
	{
		timer[2].start();
		nTStrip++;
		ret = _ColorStripDetect();
		timer[2].stop();

		if (ret);
	
		else 
		{
			timer[3].start();
			nTstatic++;
			ret = StaticFrameDetect();
			timer[3].stop();	
		}
	}

	timer[0].stop();
	ProcessVideoResult();

	return TRUE;
}


//彩条检测的一种实现方式: 通过均分图像,然后各条匹配的方式.


DETECT_TYPE  YV12_Detect::ColorStripDetect()
{
	DWORD  Rows = nPixels/4/(*CLIP_WIDTH.value/2);
	long  RowInterval = (Rows-*LINE_COUNTS_FOR_STRIP_DETECT.value)/(*LINE_COUNTS_FOR_STRIP_DETECT.value)**CLIP_WIDTH.value/2;
	if(RowInterval<0) RowInterval = 0;
	RowInterval = RowInterval/sizeof(DWORD);
	
	WORD NonStripLineCounts =WORD(*LINE_COUNTS_FOR_STRIP_DETECT.value*(1-*COLOR_STRIP_PERCENTAGE.value));
	WORD currentNonStripLine=0,currentNonStrip=0;

	DWORD* p=(DWORD*)p_u;
	DWORD* StopP = (DWORD*)(p_u+int(nPixels/4));
	//detect color strip blocks
	for(; p<StopP; p+=RowInterval)
	{	
		currentNonStrip =0;

		for(stripCursor=0;stripCursor<STRIP_COUNTS;stripCursor++)
		{
			memset(stripUHash,0,sizeof(stripUHash));
			maxUCount=0;
			
			for (pixelCursor=0;pixelCursor<int(STRIP_WIDTH/2/sizeof(DWORD));pixelCursor++)
			{
				stripUHash[(*p&0x000000ff)]++;
				stripUHash[(*p&0x0000ff00)>>8]++;
				stripUHash[(*p&0x00ff0000)>>16]++;
				stripUHash[(*p&0xff000000)>>24]++;
				p++;
			}

			for (stripHashCursor=0;stripHashCursor<256;stripHashCursor++)
			{
				if (stripUHash[stripHashCursor]>maxUCount)
				{
					maxUCount=stripUHash[stripHashCursor];
					maxUCursor=stripHashCursor;
				}
			}
			
			maxUCount=0;
			
			for (stripHashCursor=maxUCursor-5;stripHashCursor<=maxUCursor+5;stripHashCursor++)
			{
				if (stripHashCursor>=0&&stripHashCursor<=255)
				{	
					maxUCount+=stripUHash[stripHashCursor];
				}
			}
			
			if((float)maxUCount/(STRIP_WIDTH/2)<*COLOR_STRIP_PERCENTAGE.value)
			{
				currentNonStrip++;
				if(currentNonStrip>2)
				{
					currentNonStripLine++;
					p+=(STRIP_COUNTS-stripCursor-1)*STRIP_WIDTH/2;					
					break;
				}
				else continue;
			}
			
			if(abs((maxUCursor>>4)-COLOR_STRIP[stripCursor])>2)
			{
				currentNonStrip++;
				if(currentNonStrip>2)
				{
					currentNonStripLine++;
					p+=(STRIP_COUNTS-stripCursor-1)*STRIP_WIDTH/2;
					break;
				}
				else continue;
			}
		}
		if(currentNonStripLine > NonStripLineCounts)
		{
			return 0;
		}
	}
	m_VideoResult|=(m_detectType&DETECT_TYPE_COLOR_STRIP);
	return DETECT_TYPE_COLOR_STRIP;
}


//静桢检测


DETECT_TYPE YV12_Detect::StaticFrameDetect()
{
	if(pFrameBuffer==NULL)
	{
		pFrameBuffer = new BYTE[int(nPixels*YV12_BYTES_PER_PIXEL)];
		memcpy(pFrameBuffer,p_y,nPixels);
		memcpy(pFrameBuffer+nPixels,p_v,nPixels/4);
		memcpy(pFrameBuffer+nPixels+nPixels/4,p_u,nPixels/4);
		return 0;
	}
	else
	{
		WORD* StopP =(WORD*)(p_y+nPixels);
		WORD* p1=(WORD*)p_y,*p2=(WORD*)pFrameBuffer;
		DWORD NonSamePixel = DWORD(nPixels*(1-*STATIC_FRAME_PERCENTAGE.value));
		DWORD currentNonSamePixel=0;

		while(p1<StopP)
		{
			if (!EqualPixel(*p1++,*p2++,*AMBIT_DEEMED_TO_SAME_PIXEL.value))
			{
				currentNonSamePixel++;
			}
			
			if(currentNonSamePixel>NonSamePixel)
			{
				memcpy(pFrameBuffer,p_y,nPixels);
				memcpy(pFrameBuffer+nPixels,p_v,nPixels/4);
				memcpy(pFrameBuffer+nPixels+nPixels/4,p_u,nPixels/4);
				return 0;
			}
		}

		p1=(WORD*)p_v;
		StopP = (WORD*)(p_v+long(nPixels/4));

		while(p1<StopP)
		{
			if (!EqualPixel(*p1++,*p2++,*AMBIT_DEEMED_TO_SAME_PIXEL.value))
			{
				currentNonSamePixel++;
			}
			
			if(currentNonSamePixel>NonSamePixel)
			{
				memcpy(pFrameBuffer,p_y,nPixels);
				memcpy(pFrameBuffer+nPixels,p_v,nPixels/4);
				memcpy(pFrameBuffer+nPixels+nPixels/4,p_u,nPixels/4);
				return 0;
			}
		}
		p1=(WORD*)p_u;
		StopP = (WORD*)(p_u+long(nPixels/4));

		while(p1<StopP)
		{
			if (!EqualPixel(*p1++,*p2++,*AMBIT_DEEMED_TO_SAME_PIXEL.value))
			{
				currentNonSamePixel++;
			}
			
			if(currentNonSamePixel>NonSamePixel)
			{
				memcpy(pFrameBuffer,p_y,nPixels);
				memcpy(pFrameBuffer+nPixels,p_v,nPixels/4);
				memcpy(pFrameBuffer+nPixels+nPixels/4,p_u,nPixels/4);
				return 0;
			}
		}

		m_VideoResult|=(m_detectType&DETECT_TYPE_STATIC_FRAME);
		return DETECT_TYPE_STATIC_FRAME;	
	}
}

//彩场检测


DETECT_TYPE YV12_Detect::ColorSceneDetect()
{
	DWORD cur , stop = nPixels>>2;
	DWORD ycur;

	for (cur=0;cur<stop;cur++)
	{
		ycur = cur<<2;
		lineHash[0][p_y[ycur]]++;
		lineHash[0][p_y[ycur+1]]++;
		lineHash[0][p_y[ycur+2]]++;
		lineHash[0][p_y[ycur+3]]++;
		lineHash[1][p_u[cur]]++;
		lineHash[2][p_v[cur]]++;
	}
	
	//get the max count pixel;
	while(lineHashCursor<256)
	{
		if(lineHash[0][lineHashCursor]>maxPixelCount[0])
		{
			maxPixelCount[0]=lineHash[0][lineHashCursor];
			maxPixelCursor[0] = lineHashCursor;
		}
		
		if(lineHash[1][lineHashCursor]>maxPixelCount[1])
		{
			maxPixelCount[1]=lineHash[1][lineHashCursor];
			maxPixelCursor[1] = lineHashCursor;
		}
		
		if(lineHash[2][lineHashCursor]>maxPixelCount[2])		
		{
			maxPixelCount[2]=lineHash[2][lineHashCursor];
			maxPixelCursor[2] = lineHashCursor;
		}
		lineHashCursor++;
	}
	
	memset(maxPixelCount,0,sizeof(maxPixelCount));
	
	//count the total pixels;
	for(lineHashCursor = maxPixelCursor[0]-*UNDULATE_AMBIT_OF_Y.value;lineHashCursor<=maxPixelCursor[0]+*UNDULATE_AMBIT_OF_Y.value&&lineHashCursor>=0&&lineHashCursor<=255;lineHashCursor++)
	{
		maxPixelCount[0]+=lineHash[0][lineHashCursor];		
	}
	for(lineHashCursor = maxPixelCursor[1]-*UNDULATE_AMBIT_OF_U.value;lineHashCursor<=maxPixelCursor[1]+*UNDULATE_AMBIT_OF_U.value&&lineHashCursor>=0&&lineHashCursor<=255;lineHashCursor++)
	{
		maxPixelCount[1]+=lineHash[1][lineHashCursor];		
	}
	for(lineHashCursor = maxPixelCursor[2]-*UNDULATE_AMBIT_OF_V.value;lineHashCursor<=maxPixelCursor[2]+*UNDULATE_AMBIT_OF_V.value&&lineHashCursor>=0&&lineHashCursor<=255;lineHashCursor++)
	{
		maxPixelCount[2]+=lineHash[2][lineHashCursor];		
	}
	
	if(	(float)maxPixelCount[0]/(nPixels  )>=*COLOR_SCENE_PERCENTAGE.value&&
		(float)maxPixelCount[1]/(nPixels>>2)>=*COLOR_SCENE_PERCENTAGE.value&&
		(float)maxPixelCount[2]/(nPixels>>2)>=*COLOR_SCENE_PERCENTAGE.value)
	{
		if (maxPixelCursor[0]<=(*BLACK_SCENE_THRESHOLD.value))
		{
			m_VideoResult|=(m_detectType&DETECT_TYPE_BLACK_SCENE);
			return DETECT_TYPE_BLACK_SCENE;
		}
		else
		{
			m_VideoResult|=(m_detectType&DETECT_TYPE_COLOR_SCENE);
			return DETECT_TYPE_COLOR_SCENE;
		}
	}
	return 0;
}

//彩条检测的另一种实现方式: 通过边界检测方式实现.

DETECT_TYPE YV12_Detect::_ColorStripDetect()
{
	DWORD  Rows = (nPixels/(*CLIP_WIDTH.value))>>1;
	long  RowInterval =Rows/(*LINE_COUNTS_FOR_STRIP_DETECT.value)*(*CLIP_WIDTH.value>>1);
	
	WORD NonStripLineCounts =WORD(*LINE_COUNTS_FOR_STRIP_DETECT.value*(1-*COLOR_STRIP_PERCENTAGE.value));
	WORD currentNonStripLine=0;
	
	
	BYTE* p_u_t = new BYTE[nPixels>>2],*p = p_u_t;
	if(!p)//wangjun 20081001
	{
		MessageBox(NULL,"_ColorStripDetect()分配内存出错!","AotuDetect",MB_OK);
		return 0;
	}
	memcpy(p,p_u,nPixels>>2);

	BYTE* StopP = (p+int(nPixels>>2));

	Canny(p,*CLIP_WIDTH.value>>1,Rows,10,3);
	while( p<StopP )
	{
		if(isColorStripLine(p,*CLIP_WIDTH.value>>1)==FALSE)
		{
			currentNonStripLine++;
			
			if (currentNonStripLine>NonStripLineCounts)
			{
				delete p_u_t;
				p_u_t = NULL;
				return 0;
			}
		}
		p+=RowInterval;
	}
	
	delete p_u_t;
	p_u_t = NULL;
	
	p = NULL;

	m_VideoResult|=(m_detectType&DETECT_TYPE_COLOR_STRIP);

	return DETECT_TYPE_COLOR_STRIP;
}

//判断一行数据是否是彩条
//参数: pLine:一行数据的内存地址
//参数: nLineWidth:一行数据的宽度
//返回: 是否是彩条

BOOL YV12_Detect::isColorStripLine(BYTE* pLine,DWORD nLineWidth)
{
	Separator* pHead = SplitLine(pLine,nLineWidth,20);
	if (!pHead) return FALSE;
	else 
	{
		int nSepCount = 0;
		Separator* p = pHead;
		while (p)
		{
			nSepCount ++;
			p = p->next;
		}
		if (nSepCount < STRIP_COUNTS-1) 
		{
			while (pHead)
			{
				p = pHead;
				pHead = p->next;
				delete p;
			}
			return FALSE;
		}
		else
		{
			DWORD SepCur[STRIP_COUNTS-1] = {0};
			Separator* pre = NULL;
			Separator* pMax = NULL;
			int n = 0;
			for (n = 0; n < STRIP_COUNTS-1; n++)
			{
				pre = NULL;
				p = pHead;
				while (p)
				{
					if (LOWORD(p->data)>LOWORD(SepCur[n]))
					{
						SepCur[n] = p->data;
						pMax = pre;
					}
					pre = p;
					p = p->next;
				}
				if (pMax == NULL)
				{
					p = pHead->next;
					delete pHead;
					pHead = p;
				}
				else
				{
					p = pMax->next;
					pMax->next = p->next;
					pMax = NULL;
					delete p;
				}
			}
			p = pHead;
			Separator* p2 = NULL;
			while (p)
			{
				p2 = p;
				p = p->next;
				delete p2;
				p2 = NULL;
			}
			qsort((void*)SepCur,STRIP_COUNTS - 1 ,sizeof(DWORD),DWORDCompare);
			
			int s[STRIP_COUNTS];
			int ts = 0;
			double average=0.0,sigma = 0.0;

			s[0] = HIWORD(SepCur[0]);
			ts += s[0];
			s[STRIP_COUNTS-1] = (*CLIP_WIDTH.value>>1) - HIWORD(SepCur[STRIP_COUNTS-2]);
			ts+=s[STRIP_COUNTS-1];
			for (n = 1; n< STRIP_COUNTS-1; n++)
			{
				s[n] = HIWORD(SepCur[n])-HIWORD(SepCur[n-1]);
				ts+=s[n];
			}
			average = (double)ts/STRIP_COUNTS;
			for (n = 0; n < STRIP_COUNTS; n++)
			{
				sigma+=fabs(s[n]-average);
			}
			sigma = sigma/STRIP_COUNTS;
			if (sigma<3)
			{
				return TRUE;
			}
			else return FALSE;
		}
	}
	
}

//析取出一行数据的边界信息
//参数: pBuffer:一行数据的内存地址;
//参数: nBufferLen:一行数据的长度.
//参数: margin:两边的安全区,单位是BYTE
//返回: 存有边界信息的链表

Separator* YV12_Detect::SplitLine(BYTE* pBuffer,DWORD nBufferLen,int margin)
{
	BYTE* p = pBuffer+margin;
	BYTE*  pSotp= pBuffer+nBufferLen-margin;
	int c = 0;
	DWORD cursor = 0;
	Separator* pHead = new Separator;
	pHead->next = NULL;
	Separator* pNew = NULL;

	while(p<pSotp)
	{
		if (*p == 0x10)
		{
			if (c == 0)
			{
				pNew = new Separator;
				pNew->next = pHead->next;
				pNew->data=((cursor+margin)<<16);
			}
			c++;
		}
		else if (*p == 0xeb)
		{
			if (c >= 1 )
			{
				pNew->data|=c&0xffff;
				pHead->next = pNew;	
			}
			c = 0;
		}
		p++;
		cursor++;
	}
	pNew = pHead->next;
	delete pHead;

	return pNew;
}

//计算X方向导数
//参数: pImg:原始图像
//参数: nWidth,nHeight:原始图像的宽度,高度.
//参数: pnGradX,保存计算结果.

void YV12_Detect::DirGrad(BYTE* pImg, int nWidth, int nHeight,BYTE *pnGradX/* , int *pnGradY*/)
{
	// 循环控制变量
	int y ,cy = nHeight-1 ;
	int cx = nWidth -2;
	DWORD pos; 
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(y=1; y<cy; y++)
	{
		pos = y*nWidth + 1;
		int end = pos + cx;

		for(; pos<end; pos++)
		{
			pnGradX[pos] =abs( (int) ( pImg[pos+1] - pImg[pos-1]));
		}
	}	
}

// 门限运算,
//参数: pImg:保存二值图像结果
//参数: pImgEdge:图像导数
//参数: nWidth,nHeight,图像的宽度,高度
//参数: th 门限值

void YV12_Detect::NonmaxSuppress(BYTE* pImg,BYTE* pImgEdge, int nWidth, int nHeight,int th)
{
	int y;
	int cy = nHeight - 1 ;
	int cx = nWidth - 2;
	long pos;
	for(y = 1; y< cy; y ++ )
	{
		pos = y*nWidth +1;
		int end = pos + cx;
		for (; pos< end; pos ++ )
		{
			if ( pImgEdge[ pos] < th )
			{
				pImg[pos] = 0xeb;
			}
			else
			{
				pImg[pos] = 0x10;
			}
		}
	}
}

//边界检测
//参数: pImg: 原始图像;检测结束保存边界信息.
//参数: nWidth,nHeight,图像的宽度,高度
//参数: th 门限值
//参数: SmoHei:中值滤波窗口的高度

void YV12_Detect::Canny(BYTE* pImg, int nWidth, int nHeight,int th,int SmoHei)
{
	
 	// 指向x方向导数的指针
 	BYTE * pImgEdge  = new BYTE [nWidth*nHeight];
 	// 计算方向导数
 	DirGrad(pImg, nWidth, nHeight, pImgEdge);
	// 应用non-maximum 抑制
	NonmaxSuppress(pImg,pImgEdge, nWidth, nHeight,th);

	MedFilt(pImg, nWidth, nHeight,1,SmoHei,3);
	// 释放内存
	delete pImgEdge;
	pImgEdge = NULL ;
}

//中值滤波
//参数: lpImgData: 需要滤波的图像
//参数: nWidth,nHeight,图像的宽度,高度
//参数: tWidth,tHeight,滤波窗口的宽度,高度
//参数: interval:滤波窗口拾取附近颜色的间隔

void  __fastcall YV12_Detect::MedFilt(BYTE* lpImgData,DWORD Width,DWORD Height,DWORD tWidth,DWORD tHeight,int interval)
{ 
	if((tWidth%2==0)||(tHeight%2==0)) return ;
	if((tWidth>Width)||(tHeight>Height)||(interval<1)) return;
	
	int bx = (tWidth>> 1)*interval+1,sx = Width - bx;
	int by = (tHeight>>1)*interval+1,sy = Height - by;
	int tx,tbx = bx-1,tsx;
	int ty,tby = by-1,tsy;
	BYTE* tem = new BYTE[tWidth*tHeight],* pTem=NULL;
	int x,y;

	for (y = by;y<sy;y++)
	{
		for (x=bx;x<sx;x++)
		{
			if (lpImgData[y*Width+x] == 0xeb) continue;

			pTem = tem;
			tsy = y + tby;
			tsx = x + tbx;
			for (ty = y-tby;ty<=tsy;ty+=interval)
			{
				for (tx=x-tbx;tx<=tsx;tx+=interval)
				{
							
					*pTem = lpImgData[ty*Width+tx];
					pTem++;
				}
			}
			lpImgData[y*Width+x] = GetMed(tem,tWidth,tHeight);
		}
	}
	delete tem;
	tem = NULL;
}
void AutoDetect::reportVolumeRe(int errorInfo,int errorDuration,int nRelativeStart)
{
	switch(errorInfo)
	{
	case L_VOLUME_HIGH:		
		SendResult->ProcessResultAudio(DETECT_TYPE_AUDIO_HIGH,m_nCurrentAudio-errorDuration,errorDuration,0);
		break;
	case L_VOLUME_LOW:
		SendResult->ProcessResultAudio(DETECT_TYPE_AUDIO_LOW,m_nCurrentAudio-errorDuration,errorDuration,0);
		break;
	case L_VOLUME_MUTE:
		SendResult->ProcessResultAudio(DETECT_TYPE_AUDIO_MUTE,m_nCurrentAudio-errorDuration,errorDuration,0);
		break;
	default:
		break;
	}
}