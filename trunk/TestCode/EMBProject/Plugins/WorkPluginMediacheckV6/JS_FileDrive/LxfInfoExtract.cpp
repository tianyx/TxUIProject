// LxfInfoExtract.cpp: implementation of the CLxfInfoExtract class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
#include "LxfInfoExtract.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char * aspect_ratio[]={"0","1:1","4:3","16:9","2:1"};
const long
CLxfInfoExtract::SampleRates[2][4] = {   
	     // version 0
    {22050, 24000, 16000, 0},
		// version 1
    {44100, 48000, 32000, 0}};
const double
CLxfInfoExtract::FrameRates[9]={0,23.976,24,25,29.97,30,50,59.94,60};

const long
CLxfInfoExtract::LxfReadHead=1024*512;

const long
CLxfInfoExtract::LxfReadEnd=1024*512;

#define HIGH_NUM_CHAGE 4294967296

CLxfInfoExtract::CLxfInfoExtract()
{
	OutputDebugString("WWW:CLxfInfoExtract::CLxfInfoExtract()\n");


}

CLxfInfoExtract::~CLxfInfoExtract()
{
   	OutputDebugString("WWW: CLxfInfoExtract::~CLxfInfoExtract()");
} 

BOOL CLxfInfoExtract::AnalyzeFile()
{
	bTransClip = FALSE;
	LONG offset = 0;
	LONG offset_End = 0;                   //从文件尾向前递推字节数；
	BYTE* lpBuf=new BYTE[LxfReadHead];
	BYTE* lpBuf_End=new BYTE[LxfReadEnd];   //从文件尾读取；
	DWORD Num;
	HANDLE FileHandle;

	//读文件

	FileHandle=CreateFile(m_strFileName.c_str(),GENERIC_READ,FILE_SHARE_READ, 0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if (!FileHandle)
	{
		return false;
	}
	if(!ReadFile(FileHandle,lpBuf,LxfReadHead,&Num,0)) 
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
	    lpBuf_End=NULL;
        CloseHandle(FileHandle);
		m_strError = "文件信息检测无法读取文件标识失败。";
		return false;
	}
	//wangjun 20081004
	if(memcmp(lpBuf,"LEITCH",6))//判断是否为Leitch服务器文件
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
		lpBuf_End=NULL;
CloseHandle(FileHandle);
		m_strError = "此文件不是Leitch文件，请选择正确的文件驱动";
		return false;
	}

    if(memcmp(&lpBuf[76],"hanhan00",8) == 0)
		bTransClip = TRUE;

	long hWord = 0xFFFFFFFF;
	long LWord = 0xFFF80000;
	DWORD dwHr = SetFilePointer(FileHandle,LWord,&hWord,FILE_END);
	if(!ReadFile(FileHandle,lpBuf_End,LxfReadEnd,&Num,0))
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
	    lpBuf_End=NULL;
       CloseHandle(FileHandle);
		m_strError = "文件信息检测读取文件头失败。";
		return false;
	}
	CloseHandle(FileHandle);

	if(Num == 0)
	{
		delete lpBuf_End;
		lpBuf_End = lpBuf;
	}

//按顺序依次寻找包头、视频包头、视频序列头；
//                    音频包头、音频序列头；

	while (offset<LxfReadHead)      
	{
		if(SWAP_ULONG(lpBuf+offset)==PACK_START_CODE) 		
		   break;		
		offset++;
	}

	if (offset >= LxfReadHead)
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
		lpBuf_End=NULL;
		m_strError = "文件信息检测没有找到视频包头.";
		return false;
	}

	if(lpBuf[offset+8]==0)     //版本检测；
	{
		m_VersionTrans=-8;
	    m_Version=0;
	}
	else
	{
		m_VersionTrans=0;
		m_Version=1;
	}

	

	while (offset<LxfReadHead)
	{
		if(SWAP_ULONG(lpBuf+offset)==PACK_START_CODE)  //视频包头
		{	
			if((*(lpBuf+offset+16))==0)
				break;
		}
		offset++;
	}

	if (offset >= LxfReadHead)
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
		lpBuf_End=NULL;
		m_strError = "文件信息检测没有找到视频包头.";
		return false;
	}

	PacketAnalyze_Video(lpBuf,offset);


	while (offset<LxfReadHead)
	{
		if(SWAP_ULONG(lpBuf+offset)==SEQUENCE_START_CODE) break;   //视频序列头；
		offset++;
	}
    
	if (offset >= LxfReadHead)
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
		lpBuf_End=NULL;
		m_strError = "文件信息检测没有找到视频序列头.";
		return false;
	}

	SequenceAnalyze_Video(lpBuf,offset);


	while (offset<LxfReadHead)
	{
		if(SWAP_ULONG(lpBuf+offset)==PACK_START_CODE)            //音频包头；
			if(*(lpBuf+offset+16)==1)
				break;
			offset++;
	}

	if (offset >= LxfReadHead)
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
		lpBuf_End=NULL;
		m_strError = "文件信息检测没有找到音频包头.";
		return false;
	}

	PacketAnalyze_Audio(lpBuf,offset);


/*	while (offset<LxfReadHead)                                    //音频序列头；
	{
		if ((lpBuf[offset] == 0xFF) &&((lpBuf[offset+1] & 0xF8) == 0xF8)) 
				if(SequenceAnalyze_Audio(lpBuf,offset))
                   break;    

	    offset++;
	}
	SequenceAnalyze_Audio(lpBuf,offset);
*/
    m_StreamInfo.AudioInfo = m_AudioInfo;
	m_StreamInfo.VideoInfo = m_VideoInfo;
	//提取文件长度（单位：帧）
	while (offset_End<(LxfReadEnd-100))
	{
		if(SWAP_ULONG(lpBuf_End+LxfReadEnd-100-offset_End)==PACK_START_CODE)
			if(lpBuf_End[LxfReadEnd-100-offset_End+16]==0)
				break;
			offset_End++;
	}
	offset_End=LxfReadEnd-100-offset_End;
	if(!offset_End) //文件尾中0.5MBYTE 无Leitch视频帧；
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf=NULL;
	    lpBuf_End=NULL;
		return false; 
	}

	FileDuration(lpBuf_End,offset_End);


	delete []lpBuf;
	if(lpBuf != lpBuf_End)
	    delete []lpBuf_End;
	lpBuf=NULL;
	lpBuf_End=NULL;
 	return true;

}



VOID CLxfInfoExtract::SequenceAnalyze_Video(BYTE *pBuf, LONG offset)
{
	//从视频序列头中提取信息（MPEG2）；

	BYTE* lpBuf=pBuf+offset;
	m_VideoInfo.iVideowidth=int(lpBuf[4]<<4|lpBuf[5]>>4);
	m_VideoInfo.iVideoHeight=int((lpBuf[5]&0x0f)<<8|lpBuf[6]);
	m_VideoInfo.strAspectRatio=aspect_ratio[lpBuf[7]>>4];
	m_VideoInfo.lvbvBufferSiae=long((lpBuf[10]&0x1f)<<5|lpBuf[11]>>3);
	m_VideoInfo.iFrameRate=FrameRates[lpBuf[7]&0x0f];
	m_VideoInfo.lVideoBitRate=long(lpBuf[8] << 10) + (lpBuf[9] << 2) + (lpBuf[10] >> 6);
 	if (m_VideoInfo.lVideoBitRate == 0x3FFFF)
     {
         m_VideoInfo.lVideoBitRate = 0;  // variable
     } else {
		 if(bTransClip)
             m_VideoInfo.lVideoBitRate *= 50;      //单位转换为 ： 字节/秒
		 else
			 m_VideoInfo.lVideoBitRate *= 10;
     }

	 //zhou 2009.4.16 begin
     m_VideoInfo.lVideoBitRate *= 8;
	 //zhou 2009.4.16 end

/*	m_StreamInfo.VideoInfo.iVideowidth=m_VideoInfo.iVideowidth;
	m_StreamInfo.VideoInfo.iVideoHeight=m_VideoInfo.iVideoHeight;
	m_StreamInfo.VideoInfo.strAspectRatio=m_VideoInfo.strAspectRatio;
	m_StreamInfo.VideoInfo.lvbvBufferSiae=m_VideoInfo.lvbvBufferSiae;
	m_StreamInfo.VideoInfo.iFrameRate=m_VideoInfo.iFrameRate;
	m_StreamInfo.VideoInfo.lVideoBitRate=m_VideoInfo.lVideoBitRate;
*/
}

VOID CLxfInfoExtract::PacketAnalyze_Audio(BYTE *pBuf, LONG offset)
{
	//Leitch音频包头中信息提取；

	BYTE* lpBuf=pBuf+offset;
	m_AudioInfo.lAudioQuantizationBits=(lpBuf[40]&0x3F);
	BYTE mask=lpBuf[44]&0x0F;
	int nChannels=0;
	while(mask & 1)
	{
		nChannels++;
	    mask=mask>>1;
	}
	m_AudioInfo.lChannels=nChannels;
	__int64 trackSize = 0;
    trackSize  = lpBuf[48];
	trackSize += lpBuf[49]<<8;
	trackSize += lpBuf[50]<<16;
	trackSize += lpBuf[51]<<24;
    
	m_AudioInfo.iAudioSamplingRate = trackSize*m_VideoInfo.iFrameRate/(m_AudioInfo.lAudioQuantizationBits/8);

//	m_StreamInfo.AudioInfo.lAudioQuantizationBits=m_AudioInfo.lAudioQuantizationBits;
 //   m_StreamInfo.AudioInfo.lChannels=m_AudioInfo.lChannels;
}

VOID CLxfInfoExtract::PacketAnalyze_Video(BYTE *pBuf, LONG offset)
{
	//Leitch视频包头中信息提取；

	BYTE* lpBuf=pBuf+offset;
	m_VideoInfo.iMaxGOP=((lpBuf[41+m_VersionTrans]&0x07)<<4)|(lpBuf[40+m_VersionTrans]>>4);
//	m_VideoInfo.lVideoBitRate=(lpBuf[42+m_VersionTrans]<<2|lpBuf[41+m_VersionTrans]>>6)*1000000;
	switch (lpBuf[40+m_VersionTrans]&0x0f)
	{
    	case 0: m_VideoInfo.strVideoCodecFormat="JPEG"; break;
	    case 1:
		case 2:
	    case 3: m_VideoInfo.strVideoCodecFormat="MPEG2"; break;
		case 4: m_VideoInfo.strVideoCodecFormat="Dv25";  break;
		case 5: m_VideoInfo.strVideoCodecFormat="DVCPRO";  break;
		case 6: m_VideoInfo.strVideoCodecFormat="DVCPRO50";  break;
		default: break;
	}


	//m_StreamInfo.VideoInfo.iMaxGOP=m_VideoInfo.iMaxGOP;
	//m_StreamInfo.VideoInfo.strVideoCodecFormat=m_VideoInfo.strVideoCodecFormat;

}

BOOL CLxfInfoExtract::SequenceAnalyze_Audio(BYTE *pBuf, LONG offset)
{
	//从音频序列头中提取信息（MPEG2）；

	BYTE* lpBuf=pBuf+offset;
	m_AudioInfo.iAudioSamplingRate= SampleRates[m_Version][(lpBuf[2] >> 2) & 3];
	if(m_AudioInfo.iAudioSamplingRate==0) return false;


//	m_StreamInfo.AudioInfo.iAudioSamplingRate=m_AudioInfo.iAudioSamplingRate;
	return true;

}



VOID CLxfInfoExtract::FileDuration(BYTE *pBuf, LONG offset)
{

	BYTE* lpBuf=pBuf+offset;
	LONGLONG Video_Step,Video_Number;
	if (m_Version==0)
	{
		Video_Number=LONGLONG(lpBuf[27]<<16|lpBuf[26]<<8|lpBuf[25]);
		Video_Number=LONGLONG(Video_Number<<8|lpBuf[24]);

		Video_Step=LONGLONG(lpBuf[31]<<24|lpBuf[30]<<16|lpBuf[29]<<8|lpBuf[28]);
		if (Video_Step > 0)
		{
	m_StreamInfo.llFileDuration=Video_Number/Video_Step+1;
		}
	}
	if (m_Version==1)
	{
		//zhou change 2009.4.18 begin
		//修改大素材读素材长度错误的问题
		//Video_Number=LONGLONG(lpBuf[27]<<16|lpBuf[26]<<8|lpBuf[25]);
		//Video_Number=LONGLONG(Video_Number<<8|lpBuf[24]);
		DWORD Video_NumberLow = 0;
		Video_NumberLow = DWORD(lpBuf[27]<<24|lpBuf[26]<<16|lpBuf[25]<<8|lpBuf[24]);
        DWORD Video_NumberHigh = 0;
		Video_NumberHigh = DWORD(lpBuf[31]<<24|lpBuf[30]<<16|lpBuf[29]<<8|lpBuf[28]);
		if(Video_NumberHigh > 0 )
		    Video_Number=LONGLONG(Video_NumberLow) + Video_NumberHigh * HIGH_NUM_CHAGE;
		else
			Video_Number = Video_NumberLow;
		//zhou change 2009.4.18 end

		Video_Step=LONGLONG(lpBuf[35]<<24|lpBuf[34]<<16|lpBuf[33]<<8|lpBuf[32]);
		if (Video_Step > 0)
		{
		m_StreamInfo.llFileDuration=Video_Number/Video_Step+1;
		}
	}

}
