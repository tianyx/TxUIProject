// FileInfoExtract_MPG.cpp: implementation of the CFileInfoExtract_MPG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileInfoExtract_MPG.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInfoExtract_MPG::CFileInfoExtract_MPG()
{

}

CFileInfoExtract_MPG::~CFileInfoExtract_MPG()
{
   m_StreamIDList.clear();
}

bool CFileInfoExtract_MPG::AddStreamID(ULONG ID)
{
    m_StreamIDList.push_back(ID);
	return true;
}
bool CFileInfoExtract_MPG::RemoveStreamID(ULONG ID)
{
	bool bRe = false;
	list<ULONG>::iterator it = m_StreamIDList.begin();
	ULONG uStreamID = 0;
	while (it != m_StreamIDList.end())
	{
		uStreamID = *it;
       if(uStreamID == ID)
	   {
		   m_StreamIDList.remove(ID);
		   bRe = true;
		   break;
	   }
       it++;
	}
	return bRe;
}
bool CFileInfoExtract_MPG::FindStreamID(ULONG ID)
{
	bool bRe = false;
	list<ULONG>::iterator it = m_StreamIDList.begin();
	ULONG uStreamID = 0;
	while (it != m_StreamIDList.end())
	{
		uStreamID = *it;
		if(uStreamID == ID)
		{
			bRe = true;
			break;
		}
		it++;
	}
	return bRe;
}

BOOL CFileInfoExtract_MPG::AnalyzeFile()
{
	const long	M2vReadHead=1024*512;
	const long  M2vReadEnd=1024*1024*4;
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
		return false;
	}
	if(!ReadFile(FileHandle,lpBuf,M2vReadHead,&Num,0)) 
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
		CloseHandle(FileHandle);
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

	if(SWAP_ULONG(lpBuf) == PS_PACK_START_CODE)
	{

		Pack_Header(lpBuf,M2vReadHead);
		
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
			return false;   
		}
		
		FileEndAnalyse(lpBuf_End,offset_End1,offset_End2,rem);
	}

	delete []lpBuf;
	delete []lpBuf_End;
	lpBuf     = NULL;
	lpBuf_End = NULL;
	return true;
}





VOID CFileInfoExtract_MPG::FileEndAnalyse(BYTE *pBuf, LONG offset1, LONG offset2,LONG rem)
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
	INT PictureRem=0;                 //文件末尾不足一个GOP的图像数；
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

long CFileInfoExtract_MPG::Pack_Header(const BYTE *pBuf, long nLen)
{
	//这里15为不包含system_header且pack_stuffing_length = 0时的Packet_Head大小
	if (!pBuf || nLen < 15)
	{
		return false;
	}
    long nOffset = 14;
	while(nOffset < nLen -4)
	{
		if (SWAP_ULONG(pBuf+nOffset) == SYSTEM_START_CODE)//system_header
		{
			//m_StreamIDList.clear();
			nOffset += 12;
			while (pBuf[nOffset] & 0x80)
			{
				nOffset += 3;
			}
		}
		if (SWAP_ULONG(pBuf+nOffset) == PS_PACK_START_CODE)//Pack_Header
		{
			nOffset += Pack_Header(pBuf + nOffset, nLen - nOffset);
		}
		else if (SWAP_ULONG(pBuf+nOffset)>>8 == 0x000001 && pBuf[nOffset+3] > 0xBB )//PES_Packet
		{
			nOffset += PES_Packet(pBuf + nOffset, nLen - nOffset);
		}
		else
		{
			if(nOffset < 64)
			{
				nOffset++;
				continue;
			}
			else
                break;
		}

		if (m_StreamIDList.size() == 2 )
		{
			break;
		}
	}
	return nOffset;
}
//
long CFileInfoExtract_MPG::PES_Packet(const BYTE *pBuf, long nLen)
{
	if(nLen < 9)//分析数据需要大于9BYTE
		return 0;
	BYTE uStreamID = pBuf[3];
	if((uStreamID >> 4) == 0x0E)//video stream
	{
		if (FindStreamID(uStreamID))
		{
			AddStreamID(uStreamID);
		}
		SequenceHeader sh;
		long nLocate = FindStartCode(pBuf, nLen, SEQUENCE_START_CODE);
		if (nLocate > -1)
		{
			sh.Parse(pBuf+nLocate, nLen-nLocate);
			m_VideoInfo.strVideoCodecFormat="MPEG2";	
			m_VideoInfo.iFrameRate=(int)sh.FrameRate();
			m_VideoInfo.lVideoBitRate=sh.Bitrate();
			m_VideoInfo.strAspectRatio=sh.GetAspectRatio();
			m_VideoInfo.iVideoHeight=sh.Height();
			m_VideoInfo.iVideowidth=sh.Width();
			m_VideoInfo.lvbvBufferSiae=sh.vbvBufferSize();
			
			m_StreamInfo.VideoInfo=m_VideoInfo;	
			
		}
	}
	else if((uStreamID >> 5) == 0x06)//mpeg1 audio stream
	{
		if (FindStreamID(uStreamID))
		{
			AddStreamID(uStreamID);
		}
		//assume PES_header_data_length = 22
		//int PES_packet_length = pBuf[nOffset + 4] | pBuf[nOffset + 5]<<8;
		int PES_header_data_length = pBuf[8];
		const BYTE* pData = pBuf+ 9 + PES_header_data_length;
		int audio_frame_header = pData[0] << 8;
		audio_frame_header += pData[1] & 0xE0;
		if (audio_frame_header == 0xFFE0)
		{
			int iMpegVersion = (pData[1] >> 3) & 0x03;
			switch (iMpegVersion)
			{
			case 0x00:
				m_AudioInfo.strAudioCodecFormat = "MPEG2.5";
				break;
			case 0x01:
				m_AudioInfo.strAudioCodecFormat = "MPEG2V2";
				break;
			case 0x02:
				m_AudioInfo.strAudioCodecFormat = "MPEG2V1";
				break;
			case 0x03:
				m_AudioInfo.strAudioCodecFormat = "MPEG1";
				break;
			}
			
			BYTE iLayer = (pData[1] >> 1) & 0x03;
			switch (iLayer)
			{
			case 0x01:
				m_AudioInfo.strAudioCodecFormat += " LayerIII";
				break;
			case 0x02:
				m_AudioInfo.strAudioCodecFormat += " LayerII";
				break;
			case 0x03:
				m_AudioInfo.strAudioCodecFormat += " LayerI";
				break;
			}
			
			int iAudioSamplingRate = (pData[2] >> 2) & 0x03;
			switch (iAudioSamplingRate)
			{
			case 0x00:
				if (iMpegVersion == 0x10)
				{
					m_AudioInfo.iAudioSamplingRate = 22050;
				}
				if (iMpegVersion == 0x03)
				{
					m_AudioInfo.iAudioSamplingRate = 44100;
				}
				break;
			case 0x01:
				if (iMpegVersion == 0x10)
				{
					m_AudioInfo.iAudioSamplingRate = 24000;
				}
				if (iMpegVersion == 0x03)
				{
					m_AudioInfo.iAudioSamplingRate = 48000;
				}
				break;
			case 0x02:
				if (iMpegVersion == 0x10)
				{
					m_AudioInfo.iAudioSamplingRate = 16000;
				}
				if (iMpegVersion == 0x03)
				{
					m_AudioInfo.iAudioSamplingRate = 32000;
				}
				break;
			case 0x03:
				m_AudioInfo.iAudioSamplingRate = 48000;
				break;
			}
			//mpge1 layer1 默认量化
			m_AudioInfo.lAudioQuantizationBits = 16;
			
			int lChannels = pData[3] >> 6;
			switch (lChannels)
			{
			case 0x00:
			case 0x01:
			case 0x02:
				m_AudioInfo.lChannels = 2;//具有多个Stream的情况
				break;
			case 0x03:
				m_AudioInfo.lChannels = 1;
				break;
			}
		}
		
		m_StreamInfo.AudioInfo = m_AudioInfo;	
	}
	else if(uStreamID == 0xBC)//Program Stream map
	{
	}
	else if (uStreamID == 0xBE)
	{
	}

	long nOffset = 0;
	ULONG PES_packet_length = 0;
	PES_packet_length = pBuf[4] <<8;
	PES_packet_length += pBuf[5];
	nOffset += PES_packet_length;
	nOffset += 6;
	return nOffset;
}
long CFileInfoExtract_MPG::FindStartCode(const BYTE* pData,long nLen,DWORD code)
{
    code = SWAP_ULONG((BYTE*)&code);
    long offset = 0;
    while (offset < (nLen-4))
    {
        if (* (DWORD*)(pData + offset) == code)
        {
            return offset;
        }
        offset++;
    }
    return -1;
}