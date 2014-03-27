// FileInfoExtract_PNCL.cpp: implementation of the CFileInfoExtract_PNCL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileInfoExtract_MSS.h"
#include <list>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInfoExtract_MSS::CFileInfoExtract_MSS()
{

}

CFileInfoExtract_MSS::~CFileInfoExtract_MSS()
{

}

BOOL CFileInfoExtract_MSS::AnalyzeFile()
{
	const long	M2vReadHead=1024*512;
	const long  M2vReadEnd=1024*1024*4;
//	LONG offset = 0;
// 	LONG offset_End1 = 0;
// 	LONG offset_End2 = 0;  //���ļ�β��ǰ�����ֽ�����
// 	LONG rem;
	BYTE* lpBuf=new BYTE[M2vReadHead];
	BYTE* lpBuf_End=new BYTE[M2vReadEnd];   //���ļ�β��ȡ��
	DWORD Num;
	HANDLE FileHandle;
	
	std::string strHeaderFileName = m_strFileName + "\\header";
	//���ļ�
	m_strFileName += "\\std";
	FileHandle=CreateFile(m_strFileName.c_str(),GENERIC_READ,FILE_SHARE_READ, 0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if (!FileHandle)
	{
		delete []lpBuf;
		delete []lpBuf_End;
		lpBuf     = NULL;
		lpBuf_End = NULL;
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

	if(SWAP_ULONG(lpBuf) == PS_PACK_START_CODE)//����MSS�ز�
	{
		AnalyPSStream(lpBuf,M2vReadHead);//����ý������

		//�����ļ�����
		ULONG Scan = 0xffffffff;
        long nOffset = 0;
        
		int  nGOPCount = 0;
		int  nMaxGOP = 0;
		Time GOPTime;
		long nPicNum = 0;
		while(nOffset < (M2vReadEnd-4))
		{
		    Scan = (Scan << 8) | lpBuf_End[nOffset];
			if (Scan == GOP_START_CODE)
			{
                nGOPCount++;
				if(nGOPCount == 1)
				{
					GOPTime.hours   = (lpBuf_End[nOffset+1]&0x7C)>>2;
					GOPTime.minutes = (lpBuf_End[nOffset+1]&0x03)<<4   | lpBuf_End[nOffset+2]>>4;
				    GOPTime.seconds = ((lpBuf_End[nOffset+2]&0x07)<<3) | lpBuf_End[nOffset+3]>>5;
					GOPTime.picNums = (lpBuf_End[nOffset+3]&0x1F)<<1   | lpBuf_End[nOffset+4]>>7;
				}
			}
			if (Scan == PICTURE_START_CODE)
			{
				if (nGOPCount == 1)
				{
					nMaxGOP++;
					nPicNum++;
				}
				else if(nGOPCount > 1)
					nPicNum++;
			}
			nOffset++;
		}
		long nFirstTime = (m_FirstGOPTime.hours*3600 + m_FirstGOPTime.minutes*60 + m_FirstGOPTime.seconds)
			*INT(m_VideoInfo.iFrameRate+0.5) + m_FirstGOPTime.picNums;
		long nEndTime   = (GOPTime.hours*3600 + GOPTime.minutes*60 + GOPTime.seconds)
			*INT(m_VideoInfo.iFrameRate+0.5) + GOPTime.picNums + nPicNum;

		m_StreamInfo.llFileDuration = nEndTime - nFirstTime;
		
		m_VideoInfo.iMaxGOP=nMaxGOP;
		m_StreamInfo.VideoInfo.iMaxGOP=nMaxGOP;

		if(m_StreamInfo.AudioInfo.strAudioCodecFormat == "PCM uncompressed audio")
		{
			DWORD nReadNum = 0;
			HANDLE HFileHandle;
			BYTE* lpReadBuf = new BYTE[131];
			HFileHandle=CreateFile(strHeaderFileName.c_str(),GENERIC_READ,FILE_SHARE_READ, 0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
			if (!HFileHandle)
			{
				delete []lpReadBuf;
				lpReadBuf     = NULL;
				return false;
			}
			if(!ReadFile(HFileHandle,lpReadBuf,131,&nReadNum,0)) 
			{
				delete []lpReadBuf;
				lpReadBuf     = NULL;
				CloseHandle(HFileHandle);
				return false;
			}
			int nH = lpReadBuf[0x2F];
			int nM = lpReadBuf[0x2E];
			int nS = lpReadBuf[0x2D];
			int nF = lpReadBuf[0x2C];
			long nDuration   = (nH*3600 + nM*60 + nS)
			*INT(m_VideoInfo.iFrameRate+0.5) + nF;
			m_StreamInfo.llFileDuration = nDuration;

			delete []lpReadBuf;
			lpReadBuf     = NULL;
			CloseHandle(HFileHandle);
		}

// 		while (offset_End1<(M2vReadEnd-100))
// 		{
// 			if(SWAP_ULONG(lpBuf_End+M2vReadEnd-100-offset_End1)==GOP_START_CODE)			
// 				break;
// 			offset_End1++;
// 		}
// 		offset_End2=M2vReadEnd-100-offset_End1;
// 		rem=offset_End1+100;                     //���һ��GOP���ļ�β�ĳ��ȣ�
// 		//offset_End=M2vReadEnd-100-offset_End;
// 		if(!offset_End2) //�ļ�β��0.5M BYTE ��GOP��ʶ��
// 		{
// 			delete []lpBuf;
// 			delete []lpBuf_End;
// 			lpBuf     = NULL;
// 			lpBuf_End = NULL;
// 			return false;
// 		}
// 		offset_End1 = offset_End1+100;
// 		
// 		while (offset_End1<(M2vReadEnd-100))
// 		{
// 			if(SWAP_ULONG(lpBuf_End+M2vReadEnd-100-offset_End1)==GOP_START_CODE)			
// 				break;
// 			offset_End1++;
// 		}
// 		
// 		offset_End1=M2vReadEnd-100-offset_End1;
// 		if(!offset_End1)//�ļ�β��0.5M BYTE ��GOP��ʶ��
// 		{
// 			delete []lpBuf;
// 			delete []lpBuf_End;
// 			lpBuf     = NULL;
// 			lpBuf_End = NULL;
// 			return false;   
// 		}
// 		
// 		FileEndAnalyse(lpBuf_End,offset_End1,offset_End2,rem);
 	}

	delete []lpBuf;
	delete []lpBuf_End;
	lpBuf     = NULL;
	lpBuf_End = NULL;
	return true;
}





VOID CFileInfoExtract_MSS::FileEndAnalyse(BYTE *pBuf, LONG offset1, LONG offset2,LONG rem)
{
	BYTE* lpBuf_Next=pBuf+offset1;    //�����ڶ���GOP��
	BYTE* lpBuf_Last=pBuf+offset2;    //������һ��GOP��

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
	INT ByteRem=rem;                  //�ļ�ĩβ���һ��GOP���ļ�β�ֽ�����
	INT PictureRem=0;                 //�ļ�ĩβ����һ��GOP��ͼ������
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

bool CFileInfoExtract_MSS::AnalyPSStream(const BYTE *pBuf, long nLen)
{
	//����15Ϊ������system_header��pack_stuffing_length = 0ʱ��Packet_Head��С
	if (!pBuf || nLen < 15)
	{
		return false;
	}
	UINT uStreamCount = 0;

	using namespace std ;
	list<UINT> StreamID;
	int iAudioStreams = 0;

	long nOffset = 0;
	long nOffset1 = 12;
	while(1)
	{
		if (SWAP_ULONG(pBuf+nOffset1) == SYSTEM_START_CODE)//system_header
		{
			nOffset = nOffset1 + 6 + SWAP_INT16(pBuf+nOffset1+4);
			nOffset1 += 12;
			while (pBuf[nOffset1] & 0x80)
			{
				UINT ID = pBuf[nOffset1];
				StreamID.push_back(ID);
				if ((ID >> 5) == 0x00000006)
				{
					iAudioStreams++;
				}
				
				nOffset1 += 3;
			}
			break;
		}
		if(nOffset1 < 64)
			nOffset1++;
		else
			break;
	}

	//PES
    DWORD dwPESStartCode = 0x00000100;

	//Ϊ������ݷ���ת��MSS��ʽ�زģ���Ѱ������ͷʱ��Ҫ�������0x00000100��ͷ�����
	/******************************************************************************************/
	int nTry = 0;
	while(nTry++ < 50)
	{	
		if((SWAP_ULONG(pBuf+nOffset) >> 8) == (dwPESStartCode >> 8) && nOffset < nLen -4)
			break;
		nOffset++;
	}
	/******************************************************************************************/

	while((nOffset < nLen -4) && (SWAP_ULONG(pBuf+nOffset) >> 8) == (dwPESStartCode >> 8))
	{
		if (StreamID.empty())
		{
			break;
		}
		list<UINT>::iterator it = StreamID.begin();
		UINT uStreamID = 0;
		while (it != StreamID.end())
		{
			dwPESStartCode = 0x00000100;
			uStreamID = *it;
            dwPESStartCode += (UINT)uStreamID;
			if (SWAP_ULONG(pBuf+nOffset) == dwPESStartCode)
			{
				StreamID.remove(uStreamID);

				if((uStreamID >> 4) == 0x0000000E)//video stream
				{
					SequenceHeader sh;
					long nLocate = FindStartCode(pBuf+nOffset, nLen-nOffset, SEQUENCE_START_CODE);
					if (nLocate > -1)
					{
						sh.Parse(pBuf+nOffset+nLocate, nLen-nOffset-nLocate);
						m_VideoInfo.strVideoCodecFormat="MPEG2";	
						m_VideoInfo.iFrameRate=(int)sh.FrameRate();
						m_VideoInfo.lVideoBitRate=sh.Bitrate();
						m_VideoInfo.strAspectRatio=sh.GetAspectRatio();
						m_VideoInfo.iVideoHeight=sh.Height();
						m_VideoInfo.iVideowidth=sh.Width();
						m_VideoInfo.lvbvBufferSiae=sh.vbvBufferSize();
						
						
						m_StreamInfo.VideoInfo=m_VideoInfo;	
						
					}
                    long nPos = FindStartCode(pBuf+nOffset+nLocate, nLen-nOffset-nLocate, GOP_START_CODE);
					if (nPos > -1)
					{
						long nGOP = nOffset+nLocate+nPos+4;
						m_FirstGOPTime.hours   = (pBuf[nGOP]&0x7C)>>2;
						m_FirstGOPTime.minutes = (pBuf[nGOP]&0x03)<<4   | pBuf[nGOP+1]>>4;
						m_FirstGOPTime.seconds = ((pBuf[nGOP+1]&0x07)<<3) | pBuf[nGOP+2]>>5;
					    m_FirstGOPTime.picNums = (pBuf[nGOP+2]&0x1F)<<1   | pBuf[nGOP+3]>>7;
					}
					//������һ��GOPʱ��
				}
				if((uStreamID >> 5) == 0x00000006)//mpeg1 audio stream
				{
					BYTE dPesFlag = pBuf[nOffset+6] & 0x90;
					int PES_header_data_length = 0;
					//�ж��Ƿ�ΪPES����չ��Ϣ
					if(dPesFlag == 0x80)
					{
						PES_header_data_length = pBuf[nOffset+8] + 9;
					}
					else
					{
						//�������PES����չ��Ϣ���������Ϊ
						//Length = ����ʶ(4Byte) + ������(2Byte) + FF���λ���� + ������Ϣ(5Byte)
						int nFFCount = 0;
						while(1)
						{
							if(pBuf[nOffset + 6 + nFFCount] == 0xFF)
								nFFCount++;
							else
								break;
						}
						PES_header_data_length = 6 + nFFCount + 5;
					}
					
					//assume PES_header_data_length = 22
					//int PES_packet_length = pBuf[nOffset + 4] | pBuf[nOffset + 5]<<8;
					
					//int PES_header_data_length = pBuf[nOffset+4] << 8 + pBuf[nOffset+5];
					const BYTE* pData = pBuf+nOffset + PES_header_data_length;
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
					    //mpge1 layer1 Ĭ������
						m_AudioInfo.lAudioQuantizationBits = 16;

						int lChannels = pData[3] >> 6;
						switch (lChannels)
						{
						case 0x00:
						case 0x01:
						case 0x02:
							m_AudioInfo.lChannels = 2;//���ж��Stream�����
							break;
						case 0x03:
							m_AudioInfo.lChannels = 1;
							break;
						}
					}
                    
					m_StreamInfo.AudioInfo = m_AudioInfo;	
				}

				if(uStreamID == 0xBF)//mpeg1 audio stream
				{
					const BYTE* pData = pBuf+nOffset;
					int iPS2_ID = (pData[6] & 0xF0) >> 4;
					if (iPS2_ID == 0x00)//vbi
					{
						break;
					}
					if (iPS2_ID == 0x01)//ancillary audio formats
					{
						int iDataType = pData[6] & 0x0F;
						switch (iDataType)
						{
						case 0x01:
							m_AudioInfo.strAudioCodecFormat = "PCM uncompressed audio";
							break;
						case 0x02:
							m_AudioInfo.strAudioCodecFormat = " user_type_1";
							break;
						case 0x03:
							m_AudioInfo.strAudioCodecFormat = "AC-3";
							break;
						case 0x04:
							m_AudioInfo.strAudioCodecFormat = "Dolby-E";
							break;
						case 0x05:
							m_AudioInfo.strAudioCodecFormat = "AES/EBU";
							break;
						}
						int iAudioSamplingRate = (pData[7] & 0xF0) >> 4;
						switch (iAudioSamplingRate)
						{
						case 0x01:
							m_AudioInfo.iAudioSamplingRate = 32000;
							break;
						case 0x02:
							m_AudioInfo.iAudioSamplingRate = 44100;
							break;
						case 0x03:
							m_AudioInfo.iAudioSamplingRate = 48000;
							break;
						case 0x04:
							m_AudioInfo.iAudioSamplingRate = 96000;
							break;
						}
						long lAudioQuantizationBits = pData[7] & 0x0F;
						switch (iDataType)
						{
						case 0x01:
							m_AudioInfo.lAudioQuantizationBits = 16;
							break;
						case 0x02:
							m_AudioInfo.lAudioQuantizationBits = 20;
							break;
						case 0x03:
							m_AudioInfo.lAudioQuantizationBits = 24;
							break;
						case 0x04:
							m_AudioInfo.lAudioQuantizationBits = 32;
							break;
						}
						m_AudioInfo.lChannels = (pData[8] & 0xF0) >> 4;
					    m_StreamInfo.AudioInfo=m_AudioInfo;
					}
				}
				break;
			}
			else if (SWAP_ULONG(pBuf+nOffset) == PS_PACK_START_CODE)
			{
				nOffset1 = nOffset + 12;
				long nEnd = nOffset1 + 64;
				while(1)
				{
					if (SWAP_ULONG(pBuf+nOffset1) == SYSTEM_START_CODE)//system_header
					{
						nOffset = nOffset1 + 6 + SWAP_INT16(pBuf+nOffset1+4);
						break;
					}
					if(nOffset1 < nEnd)
						nOffset1++;
					else
						break;
				} 
			}
			it++;
		}
		
		ULONG PES_packet_length = 0;
		PES_packet_length = pBuf[nOffset+4] <<8;
		PES_packet_length += pBuf[nOffset+5];
		nOffset += PES_packet_length;
        nOffset += 6;
	}
	StreamID.clear();
	return TRUE;
}
long CFileInfoExtract_MSS::FindStartCode(const BYTE* pData,long nLen,DWORD code)
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