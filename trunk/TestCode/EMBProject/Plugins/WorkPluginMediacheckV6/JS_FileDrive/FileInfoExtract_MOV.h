#ifndef __FILEINFOEXTRACT_H__
#define  __FILEINFOEXTRACT_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct tagVideoInfo 
{	
	char m_szCodec[256];
	__int64 m_llDur;
	int m_nWidth;
	int m_nHeight;
	int m_nFrameRate;
	int m_nBitrate;
	int m_nAspectX;
	int m_nAspectY;
}VideoInfo;

typedef struct tagAudioInfo
{
	char m_szCodec[256];
	int m_nSamplingRate;
	int m_nQuantizationBits;
	int m_nChannels;
}AudioInfo;

int __stdcall GetFileInfo(char* filename, VideoInfo** ppV, AudioInfo** ppA);

#ifdef __cplusplus
};
#endif


#endif //__FILEINFOEXTRACT_H__