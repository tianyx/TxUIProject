// InfoExtract_ASF.cpp: implementation of the CInfoExtract_ASF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoExtract_ASF.h"
#include "UUID.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const unsigned char ASF_Data_Object[] =	
{0x75,0xB2,0x26,0x36,0x66,0x8E,0x11,0xCF,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C};
const unsigned char ASF_Header_Object[] =
{0x75,0xB2,0x26,0x30,0x66,0x8E,0x11,0xCF,0xA6,0xD9,0x00,0xAA,0x00,0x62,0xCE,0x6C};
const unsigned char ASF_Stream_Properties_Object[] =
{0xB7,0xDC,0x07,0x91,0xA9,0xB7,0x11,0xCF,0x8E,0xE6,0x00,0xC0,0x0C,0x20,0x53,0x65};
const unsigned char ASF_File_Properties_Object[] = 
{0x8C,0xAB,0xDC,0xA1,0xA9,0x47,0x11,0xCF,0x8E,0xE4,0x00,0xC0,0x0C,0x20,0x53,0x65};
const unsigned char ASF_Codec_List_Object[]	= 
{0x86,0xD1,0x52,0x40,0x31,0x1D,0x11,0xD0,0xA3,0xA4,0x00,0xA0,0xC9,0x03,0x48,0xF6};
const unsigned char ASF_Stream_Bitrate_Properties_Object[]	= 
{0x7B,0xF8,0x75,0xCE,0x46,0x8D,0x11,0xD1,0x8D,0x82,0x00,0x60,0x97,0xC9,0xA2,0xB2};
const unsigned char ASF_Audio_Media[] = 
{0xF8,0x69,0x9E,0x40,0x5B,0x4D,0x11,0xCF,0xA8,0xFD,0x00,0x80,0x5F,0x5C,0x44,0x2B};
const unsigned char ASF_Video_Media[] =	
{0xBC,0x19,0xEF,0xC0,0x5B,0x4D,0x11,0xCF,0xA8,0xFD,0x00,0x80,0x5F,0x5C,0x44,0x2B};

const unsigned char ASF_Payload_Extension_System_Pixel_Aspect_Ratio[] = 
{0x1B,0x1E,0xE5,0x54,0xF9,0xEA,0x4B,0xC8,0x82,0x1A,0x37,0x6B,0x74,0xE4,0xC4,0xB8};
const unsigned char ASF_Payload_Extension_System_Sample_Duration[] = 
{0xC6,0xBD,0x94,0x50,0x86,0x7F,0x49,0x07,0x83,0xA3,0xC7,0x79,0x21,0xB7,0x33,0xAD};

const unsigned char ASF_Header_Extension_Object[] = 
{0x5F,0xBF,0x03,0xB5,0xA9,0x2E,0x11,0xCF,0x8E,0xE3,0x00,0xC0,0x0C,0x20,0x53,0x65};
const unsigned char ASF_Extended_Stream_Properties_Object[] = 
{0x14,0xE6,0xA5,0xCB,0xC6,0x72,0x43,0x32,0x83,0x99,0xA9,0x69,0x52,0x06,0x5B,0x5A};

CFileInfoExtract_ASF::CFileInfoExtract_ASF()
{

}

CFileInfoExtract_ASF::~CFileInfoExtract_ASF()
{

}
BOOL CFileInfoExtract_ASF::AnalyzeFile()
{
	BYTE*    pAnalyBuffer   = NULL;
	__int64  llBufferSize   = 0;
	__int64  llOffset       = 0;
	//打开文件
	m_hFile=CreateFile(m_strFileName.c_str(),GENERIC_READ,FILE_SHARE_READ, 
		                                        0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
    if (!m_hFile)
    {
		m_strError = "无法打开文件";
		OutputDebugString("无法打开文件\n");
		return false;
    }

	BYTE hdr[24];
    if(!Read(0, 24, hdr))
	{
		CloseHandle(m_hFile);
		m_strError = "无法读取文件";
		return false;
	}
	//定位文件开始
	CUUID Head_Uuid(hdr);
	if(!Head_Uuid.IsEqual(ASF_Header_Object))
	{
		CloseHandle(m_hFile);
		m_strError = "未知文件类型";
		return false;
	}

    llBufferSize = *((__int64*)(hdr+16));
	pAnalyBuffer = new BYTE[llBufferSize];
	if(!pAnalyBuffer)
	{
		OutputDebugString("无法创建堆空间\n");
		CloseHandle(m_hFile);
		return false;
	}
    if(!Read(0, llBufferSize, pAnalyBuffer))
	{
		delete []pAnalyBuffer;
		pAnalyBuffer = NULL;
		m_strError = "无法读取文件";
		CloseHandle(m_hFile);
		return false;
	}
	
	//定位流属性对象
	bool bGetVideo = false;
	bool bGetAudio = false;
	llOffset += 20;
	while (llOffset < llBufferSize - 16)
    {
		CUUID Top_Uuid(pAnalyBuffer+llOffset);
		if(Top_Uuid.IsEqual(ASF_Stream_Properties_Object))//流属性对象
		{
			llOffset += 16;
            __int64 ObjectSize = *((__int64*)(pAnalyBuffer+llOffset));

			llOffset += 8;
			CUUID Type(pAnalyBuffer+llOffset);
			if(Type.IsEqual(ASF_Video_Media))//video type
			{
				AnalyzeVideoInfo(pAnalyBuffer+llOffset);
				bGetVideo = true;
			}
			else if(Type.IsEqual(ASF_Audio_Media))//audio type
			{
				AnalyzeAudioInfo(pAnalyBuffer+llOffset);
				bGetAudio = true;
			}
			else
			{
               llOffset += ObjectSize - 24;
			}

		}
		//ASF_Codec_List_Object
		else if (Top_Uuid.IsEqual(ASF_Codec_List_Object))//编码对象
		{
			llOffset += 16;
			__int64 ObjectSize = *((__int64*)(pAnalyBuffer+llOffset));
			llOffset += 8;

			AnalyzeCodecInfo(pAnalyBuffer + llOffset);

			llOffset += ObjectSize - 24;
		}
		else if (Top_Uuid.IsEqual(ASF_File_Properties_Object))//文件属性对象
		{
			llOffset += 16;
			__int64 ObjectSize = *((__int64*)(pAnalyBuffer+llOffset));
			llOffset += 8;
            AnalyzeFileProperties(pAnalyBuffer + llOffset);
			llOffset += ObjectSize - 24;
		}
		else if(Top_Uuid.IsEqual(ASF_Extended_Stream_Properties_Object))
		{
           llOffset += 16;
		   
		   __int64 ObjectSize = *((__int64*)(pAnalyBuffer+llOffset));

		   if (ObjectSize > 84)
		   {
			   llOffset += 24;
			   //bit rate
/*			   long lBitRate = *((long*)(pAnalyBuffer+llOffset));*/
			   llOffset += 36;
			   //Average Time Per Frame
			   __int64 llATPF = *((__int64*)(pAnalyBuffer+llOffset));
               m_VideoInfo.iFrameRate = 10000000/llATPF;
		   }
		}
// 		else if (Top_Uuid.IsEqual(ASF_Payload_Extension_System_Sample_Duration))
// 		{
// 			llOffset += 16;
// 		}
		else
		{
			llOffset++;
		}
        if(bGetAudio && bGetVideo)
			break;
		
    }
	delete []pAnalyBuffer;
	pAnalyBuffer = NULL;
	//分析实际编码数据
// 	llOffset = llBufferSize;
//     while (llOffset < 2*1024*1024)//最大扫描2M数据
//     {
// 		BYTE hdr[24];
// 		if(!Read(llOffset, 24, hdr))
// 		{
// 			CloseHandle(m_hFile);
// 			return false;
// 		}
// 		
// 		CUUID Data_Uuid(hdr);
// 		if (Data_Uuid.IsEqual(ASF_Data_Object))//数据对象
// 		{
// 			__int64 llBufferSize = *((__int64*)(hdr+16));
// 			pAnalyBuffer = new BYTE[llBufferSize];
// 			if(!pAnalyBuffer)
// 			{
// 				OutputDebugString("无法创建堆空间\n");
// 				CloseHandle(m_hFile);
// 				return false;
// 			}
// 			if(!Read(llOffset, llBufferSize, pAnalyBuffer))
// 			{
// 				delete []pAnalyBuffer;
// 				pAnalyBuffer = NULL;
// 				CloseHandle(m_hFile);
// 				return false;
// 			}
// 			
// 			llOffset = 24 + 24 + 2;
// 			AnalyzeVC_1Data(pAnalyBuffer + llOffset,llBufferSize-llOffset);//Data Packets
// 	        
// 			break;
// 		}
// 		else llOffset++;
//     }

	 m_StreamInfo.VideoInfo = m_VideoInfo;
	 m_StreamInfo.AudioInfo = m_AudioInfo;
	 if (pAnalyBuffer)
	 {
		 delete []pAnalyBuffer;
		 pAnalyBuffer = NULL;
	 }
	 CloseHandle(m_hFile);
	return true;
}
//相对位置从Stream Type开始
void CFileInfoExtract_ASF::AnalyzeVideoInfo(const BYTE *pBuffer)
{
	 m_VideoStreamId = pBuffer[48] & 0x7F;
 //    m_StreamInfo.llFileDuration = *((__int64*)(pBuffer + 32))/400000; //Time Offset
	 BITMAPINFOHEADER* pBIH =(BITMAPINFOHEADER*)(pBuffer + 65);//Type-Specific Data
	// m_VideoInfo.iFrameRate = 0;
	// m_VideoInfo.iMaxGOP    = 0;
	 m_VideoInfo.iVideoHeight = pBIH->biHeight;
	 m_VideoInfo.iVideowidth  = pBIH->biWidth;
	// m_VideoInfo.lVideoBitRate = 0;
	// m_VideoInfo.strAspectRatio = "4:3";
   //  m_VideoInfo.strVideoCodecFormat = "wmv";
	
}

void CFileInfoExtract_ASF::AnalyzeAudioInfo(const BYTE *pBuffer)
{
   m_AudioStreamId = pBuffer[48] & 0x7F;
   WAVEFORMATEX* pWFE = (WAVEFORMATEX*)(pBuffer + 54 );//Type-Specific Data	
   m_AudioInfo.iAudioSamplingRate     = pWFE->nSamplesPerSec;
   m_AudioInfo.lAudioQuantizationBits = pWFE->wBitsPerSample;
   m_AudioInfo.lChannels              = pWFE->nChannels;
   /*switch(pWFE->wFormatTag)
   {
   case 0x161:
   m_AudioInfo.strAudioCodecFormat    = "Windows Media Audio";
   break;
   case 0x162:
   m_AudioInfo.strAudioCodecFormat    = "Windows Media Audio 9 Professional";
   break;
   case 0x163:
   m_AudioInfo.strAudioCodecFormat    = "Windows Media Audio 9 Lossless";
   break;
   Default:
   m_AudioInfo.strAudioCodecFormat    = "Unkown";
   break;
   
	 }
   */
   
   
}

void CFileInfoExtract_ASF::AnalyzeCodecInfo(const BYTE *pBuffer)
{
	__int64 llOffset = 0;

	llOffset += 16;
	long EntrieCount = *((long*)(pBuffer+llOffset));
	llOffset +=4;
	//
	for (int j=0; j<EntrieCount;j++)
	{
		short Type = *((short*)(pBuffer+llOffset));
		llOffset +=2;
		short  CodecNameLength = *((short*)(pBuffer+llOffset));
		CodecNameLength *=2;//wchar
		llOffset +=2;
		
		char *c = new char[CodecNameLength];
		memcpy((void*)c,pBuffer+llOffset,CodecNameLength);
		
		if(Type == 0x0001)//Video codec
		{
			m_VideoInfo.strVideoCodecFormat.empty();
			for(int i=0; i<CodecNameLength; i++)
				m_VideoInfo.strVideoCodecFormat += c+i;
		}
		if(Type == 0x0002)//Audio codec
		{
			m_AudioInfo.strAudioCodecFormat.empty();
			for(int i=0; i<CodecNameLength; i++)
				m_AudioInfo.strAudioCodecFormat += c+i;
		}
		delete c;

		llOffset += CodecNameLength;

		short CodecDescriptionLength = *((short*)(pBuffer+llOffset));
		llOffset += CodecDescriptionLength * 2 + 2;
		short CodecInformationLength = *((short*)(pBuffer+llOffset));
		llOffset += CodecInformationLength + 2;
		
	}
}
void CFileInfoExtract_ASF::AnalyzeFileProperties(const BYTE *pBuffer)
{
	__int64 llOffset = 0;
	
	llOffset = 16 + 16;
	__int64 DataPacketsCount = *((__int64*)(pBuffer+llOffset));
    llOffset += 8;
    __int64 PlayDuration = *((__int64*)(pBuffer+llOffset));
	PlayDuration = PlayDuration / 400000;
	llOffset += 8;
	__int64 SendDuration = *((__int64*)(pBuffer+llOffset));
	SendDuration = SendDuration / 400000;

	llOffset +=8;
	__int64 Preroll		=*((__int64*)(pBuffer + llOffset));
	Preroll = Preroll / 40;

	llOffset += 20;
	long MaximumBitrate = *((long*)(pBuffer+llOffset));
    m_StreamInfo.llFileDuration = PlayDuration - Preroll;
	m_VideoInfo.lVideoBitRate = MaximumBitrate;
 

}

void CFileInfoExtract_ASF::AnalyzeVC_1Data(const BYTE *pBuffer, __int64 llSize)
{
	__int64 llOffset = 0;
    while (Sequence_Start_Code != *((long*)(pBuffer+llOffset)) && llOffset < llSize - 14)
    {
		llOffset++;
    }
	if (Sequence_Start_Code == *((long*)(pBuffer+llOffset)))
	{
		llOffset += 4;
		int ColorDiffFormat = (pBuffer[llOffset] & 0x06)>>1;
		switch (ColorDiffFormat)
		{
		case 1:
			m_VideoInfo.strColorSpace  = "4:2:0";
			break;
	}
	//DisPlay_Ext
	int DisPlay_Ext = (pBuffer[llOffset + 5] & 0x01);
	if (DisPlay_Ext == 1)
	{
		int DISP_HORIZ_SIZE = pBuffer[llOffset + 6] + (pBuffer[llOffset + 7] & 0xFC);
		int DISP_VERT_SIZE  = (pBuffer[llOffset + 7] & 0x03)<<14 | (pBuffer[llOffset + 8]<<12) 
			                   | (pBuffer[llOffset + 9] & 0xF0)<<4;

		int Aspect_Ration_Flag = (pBuffer[llOffset + 9] & 0x04)>>2;
		if (Aspect_Ration_Flag == 1)
		{
			int Aspect_Ration = ((pBuffer[llOffset + 9] & 0x03)<<2) + ((pBuffer[llOffset + 10] & 0xC0)>>6);
			if (Aspect_Ration == 15)
			{
			}
		}
	}
	else
	{
		m_VideoInfo.iFrameRate     = 25;
		//m_VideoInfo.lVideoBitRate  = 0;
		m_VideoInfo.strAspectRatio = "4:3";
	}
			
	}
	
}
