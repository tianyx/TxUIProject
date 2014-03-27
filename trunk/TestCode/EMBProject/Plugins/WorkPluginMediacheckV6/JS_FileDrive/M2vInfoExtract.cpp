// M2vInfoExtract.cpp: implementation of the CM2vInfoExtract class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "M2vInfoExtract.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


const long
CM2vInfoExtract::M2vReadHead=1024*512;
const long
CM2vInfoExtract::M2vReadEnd=1024*1024*4;

const double
CM2vInfoExtract::FrameRates[9]={0,23.976,24,25,29.97,30,50,59.94,60};

CM2vInfoExtract::CM2vInfoExtract()
{
   OutputDebugString("WWW: CM2vInfoExtract::CM2vInfoExtract()");
}

CM2vInfoExtract::~CM2vInfoExtract()
{
   OutputDebugString("WWW: CM2vInfoExtract::~CM2vInfoExtract()");
}

BOOL CM2vInfoExtract::AnalyzeFile()
{
	LONG offset = 0;
	LONG offset_End1 = 0;
	LONG offset_End2 = 0;  //从文件尾向前递推字节数；
	LONG rem;
	BYTE* lpBuf=new BYTE[M2vReadHead];
	BYTE* lpBuf_End=new BYTE[M2vReadEnd];   //从文件尾读取；
	DWORD Num;
	HANDLE FileHandle;
	
	//读文件
	
	FileHandle=CreateFile(m_strFileName.c_str(),GENERIC_READ,FILE_SHARE_READ, 0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if (!FileHandle)
	{
		m_strError = "无法打开文件"; 
		return false;
	}
	if(!ReadFile(FileHandle,lpBuf,M2vReadHead,&Num,0)) 
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		CloseHandle(FileHandle);
		m_strError ="无法读取文件";
		return false;
	}
	long hWord = 0xFFFFFFFF;
	long LWord = 0xFFC00000;
	DWORD dwHr = SetFilePointer(FileHandle,LWord,&hWord,FILE_END);
	if(!ReadFile(FileHandle,lpBuf_End,M2vReadEnd,&Num,0)) 
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		CloseHandle(FileHandle);
		m_strError ="无法读取文件";
		return false;
	}
	CloseHandle(FileHandle);
	
/*	while (offset<M2vReadHead)
	{
		if(SWAP_ULONG(lpBuf+offset)==SEQUENCE_START_CODE) break;
		offset++;
	}
	if(offset>=M2vReadHead) 
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		return false;
	}
*/
	if(SWAP_ULONG(lpBuf) != SEQUENCE_START_CODE)
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		m_strError ="没有找到序列头";
		return false;
	}

	m_SequenceHeader.Parse(lpBuf+offset,M2vReadHead);
	
	m_VideoInfo.strVideoCodecFormat="MPEG2";	
	m_VideoInfo.iFrameRate=FrameRates[lpBuf[7]&0x0f];
	m_VideoInfo.lVideoBitRate=m_SequenceHeader.Bitrate()/8;
	m_VideoInfo.strAspectRatio=m_SequenceHeader.GetAspectRatio();
	m_VideoInfo.iVideoHeight=m_SequenceHeader.Height();
	m_VideoInfo.iVideowidth=m_SequenceHeader.Width();
	m_VideoInfo.lvbvBufferSiae=m_SequenceHeader.vbvBufferSize();
	

	m_StreamInfo.VideoInfo.strVideoCodecFormat=m_VideoInfo.strVideoCodecFormat;	
	m_StreamInfo.VideoInfo.iFrameRate=m_VideoInfo.iFrameRate;
	m_StreamInfo.VideoInfo.lVideoBitRate=m_VideoInfo.lVideoBitRate;
	m_StreamInfo.VideoInfo.strAspectRatio=m_VideoInfo.strAspectRatio;
	m_StreamInfo.VideoInfo.iVideoHeight=m_VideoInfo.iVideoHeight;
	m_StreamInfo.VideoInfo.iVideowidth=m_VideoInfo.iVideowidth;
	m_StreamInfo.VideoInfo.lvbvBufferSiae=m_VideoInfo.lvbvBufferSiae;


	while (offset_End1<(M2vReadEnd-100))
	{
		if(SWAP_ULONG(lpBuf_End+M2vReadEnd-100-offset_End1)==GOP_START_CODE)			
				break;
			offset_End1++;
	}
	offset_End2=M2vReadEnd-100-offset_End1;
	rem=offset_End1+100;                     //最后一个GOP到文件尾的长度；
	//offset_End=M2vReadEnd-100-offset_End;
	if(!offset_End2) //文件尾中0.5M BYTE 无GOP标识；
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		m_strError = "文件尾中0.5M BYTE 无GOP标识";
		return false;
	}
	offset_End1 = offset_End1+100;

	while (offset_End1<(M2vReadEnd-100))
	{
		if(SWAP_ULONG(lpBuf_End+M2vReadEnd-100-offset_End1)==GOP_START_CODE)			
				break;
			offset_End1++;
	}

	offset_End1=M2vReadEnd-100-offset_End1;
	if(!offset_End1)//文件尾中0.5M BYTE 无GOP标识；
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		m_strError = "文件尾中0.5M BYTE 无GOP标识";
		return false;   
	}
	
	FileEndAnalyse(lpBuf_End,offset_End1,offset_End2,rem);

	delete []lpBuf;
	delete []lpBuf_End;
	lpBuf     = NULL;
	lpBuf_End = NULL;
	return true;
		
}





VOID CM2vInfoExtract::FileEndAnalyse(BYTE *pBuf, LONG offset1, LONG offset2,LONG rem)
{
	BYTE* lpBuf_Next=pBuf+offset1;    //倒数第二个GOP；
	BYTE* lpBuf_Last=pBuf+offset2;    //倒数第一个GOP；

	LONGLONG GOPTime_Last;
	LONGLONG GOPTime_Next;

	Time PictureTime_Last;
	Time PictureTime_Next;
	PictureTime_Last.hours=(lpBuf_Last[4]&0x7C)>>2;
	PictureTime_Last.minutes=(lpBuf_Last[4]&0x03)<<4|lpBuf_Last[5]>>4;
	PictureTime_Last.seconds=((lpBuf_Last[5]&0x07)<<3)|lpBuf_Last[6]>>5;
	GOPTime_Last=PictureTime_Last.hours*3600+PictureTime_Last.minutes*60+PictureTime_Last.seconds;

	PictureTime_Next.hours=(lpBuf_Next[4]&0x7C)>>2;
	PictureTime_Next.minutes=(lpBuf_Next[4]&0x03)<<4|lpBuf_Next[5]>>4;
	PictureTime_Next.seconds=((lpBuf_Next[5]&0x07)<<3)|lpBuf_Next[6]>>5;
	GOPTime_Next=PictureTime_Next.hours*3600+PictureTime_Next.minutes*60+PictureTime_Next.seconds;


	INT PictureNum_Last=(lpBuf_Last[6]&0x1F)<<1|lpBuf_Last[7]>>7;
	INT PictureNum_Next=(lpBuf_Next[6]&0x1F)<<1|lpBuf_Next[7]>>7;
	INT ByteRem=rem;                  //文件末尾最后一个GOP距文件尾字节数；
	INT PictureRem=0;                   //文件末尾不足一个GOP的图像数；
	LONG offset=0;
	LONGLONG FileLength=0;
	LONGLONG GopNum;
	while ((ByteRem-100-offset)>0)
	{
		if (SWAP_ULONG(lpBuf_Last+offset)==PICTURE_START_CODE)
			PictureRem++;	
        offset++;		
	}
	GopNum=(GOPTime_Last-GOPTime_Next)*INT(m_VideoInfo.iFrameRate+0.5)+(PictureNum_Last-PictureNum_Next);
	FileLength=GOPTime_Last*INT(m_VideoInfo.iFrameRate+0.5)+PictureNum_Last+PictureRem;
	
	m_VideoInfo.iMaxGOP=GopNum;

	m_StreamInfo.VideoInfo.iMaxGOP=GopNum;
	m_StreamInfo.llFileDuration=FileLength;




}
