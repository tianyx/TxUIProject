#include "StdAfx.h"
#include "MxfInfoExtract.h"
#include <stdio.h>
#include <Windows.h>

#include "SequenceHeader.h"
#include "AFDWriter.h"

#define MAX_DUMPSIZE 128
//#define MAX_PATH     512
CMxfInfoExtract::CMxfInfoExtract(void)
{
   OutputDebugString("WWW: CMxfInfoExtract::CMxfInfoExtract()");
	m_ui8VideoCoderFormat = 0;
	m_ui8AudioCoderFormat = 0;
}

CMxfInfoExtract::~CMxfInfoExtract(void)
{
	OutputDebugString("WWW: CMxfInfoExtract::~CMxfInfoExtract()");
}
BOOL CMxfInfoExtract::AnalyzeFile()
{
	byte btAfd;
	if(GetAFDInfo(m_strFileName, btAfd))
	{
		char szAFDVal[5];
		ZeroMemory(szAFDVal, 5);
		MAKESTRINGFROMAFDBYTE(szAFDVal, btAfd);
		m_StreamInfo.VideoInfo.strAFDInfo = szAFDVal;
	}
	//using namespace mxflib;
	using namespace mxflib;
	{
	MXFFilePtr  m_MxfFilePtr = new MXFFile;
	if (!m_MxfFilePtr->Open(m_strFileName, true))
	{
		return false;
	}
// 	PartitionPtr pFileHeadPartition=m_MxfFilePtr->ReadMasterPartition();
// 	if(!pFileHeadPartition)
// 	{
// 		m_MxfFilePtr->Seek(0);
// 		pFileHeadPartition = m_MxfFilePtr->ReadPartition();
// 		warning("File %s does not contain a closed copy of header metadata - using the open copy in the file header\n", m_MxfFilePtr->Name.c_str());
// 	}
// 
// 	if(!pFileHeadPartition) 
// 	{
// 		//error("Could not read header metadata from file %s\n", m_MxfFilePtr->Name.c_str());
// 		m_MxfFilePtr->Close();
// 		return false;
// 	}
// 
// 	BuildEssenceInfo(pFileHeadPartition);

	// Get a RIP (however possible)
	m_MxfFilePtr->GetRIP();
	
	RIP::iterator it = m_MxfFilePtr->FileRIP.begin();
	UInt32 iPart = 0;
	while(it != m_MxfFilePtr->FileRIP.end())
	{
		m_MxfFilePtr->Seek((*it).second->ByteOffset);
		PartitionPtr ThisPartition = m_MxfFilePtr->ReadPartition();
		if(ThisPartition)
		{
           BuildEssenceInfo(ThisPartition);
		}
		it++;
	}
// 
// 	//m_StreamInfo.llFileDura
// 	DumpObject(pFileHeadPartition->Object,"");
// 
// 	// Read and parse the head metadata
// 	if(pFileHeadPartition->ReadMetadata() == 0)
// 	{
// 		m_MxfFilePtr->Close();
// 		return false;
// 	}
//     //得到文件操作模式
// 	MetadataPtr HMeta = pFileHeadPartition->ParseMetadata();
// 
// 	if(!HMeta) 
//    {
// 		//error("Could not read header metadata from file %s\n", m_MxfFilePtr->Name.c_str());
// 		return false;
// 	}
// 	printf("\nHeader Metadata:\n");
// 	PackageList::iterator it = HMeta->Packages.begin();
// 	while(it != HMeta->Packages.end())
// 	{
// 		MDObjectPtr Descriptor = (*it)->Object;
// 		DumpObject(Descriptor, "  ");
// 		it++;
// 	}
// 
// 	printf("\n");


	if(m_ui8VideoCoderFormat == 0x01)//得到MPEG2-GC数据
	{
		PartitionPtr pFileBodyPartition=m_MxfFilePtr->ReadPartition();
		if(!pFileBodyPartition)
		{
			warning("File %s does not contain a cloased copy of header metadata - using the open copy in the file header\n", m_MxfFilePtr->Name.c_str());
		}
		
		KLVObjectPtr anElement;
		int nSuc = pFileBodyPartition->StartElements();
		if(nSuc == -1)
		{
			warning("File %s does not StartElements\n", m_MxfFilePtr->Name.c_str());
			m_MxfFilePtr->Close();
			return false;
		}
		ULONG GCCount=0;
		while  (anElement = pFileBodyPartition->NextElement())
		{
			GCElementKind kind = anElement->GetGCElementKind();
			
			if( kind.IsValid )//this is a GC Element 
			{
				Position Offset = 0;
				// Limit chunk size to 32Mb
				const Length MaxSize = 32 * 1024 * 1024;
				// Work out the chunk-size
				Length CurrentSize = anElement->GetLength() ;//- (Length)Offset;
				if(CurrentSize <= 0)
				{
					m_MxfFilePtr->Close();
					return false;
				}
				if(CurrentSize > MaxSize) CurrentSize = MaxSize;
				
				size_t Bytes = anElement->ReadDataFrom(Offset, static_cast<size_t>(CurrentSize));
				if(!Bytes) 
				{
					m_MxfFilePtr->Close();
					return false;
				}
				Offset += Bytes;
				
				long L=FindStartCode((anElement->GetData()).Data,CurrentSize,SEQUENCE_START_CODE);
				if( L>=0 )
				{
					SequenceHeader  sd;
					sd.Parse((anElement->GetData()).Data+L,CurrentSize-L);
					m_StreamInfo.VideoInfo.lvbvBufferSiae = sd.vbvBufferSize();
					m_VideoInfo.lvbvBufferSiae            = sd.vbvBufferSize();
					m_StreamInfo.VideoInfo.strAspectRatio = sd.GetAspectRatio();
					m_VideoInfo.strAspectRatio            = sd.GetAspectRatio();
					
					break;
				}
				
			} 
			
		}
	}
	m_MxfFilePtr->Close();
	return true;//return 0;
	}
	

}
//! Dump an object and any physical or logical children
// void CMxfInfoExtract::DumpObject(MDObjectPtr Object, std::string Prefix)
// {
// 	if(Object->IsModified()) 
// 		printf("%s%s is *MODIFIED*\n", Object->FullName().c_str(), Prefix.c_str() );
// 	if(Object->GetLink())  
// 	{  
// 		DumpObject(Object->GetLink(), Prefix + "  ");
// 	}
// 	else
// 	{
// 		if(Object->IsDValue())
// 		{
// 			printf("%s%s = <Unknown>\n", Prefix.c_str(), Object->Name().c_str());
// 		}
// 		else
// 		{
// 			if(Object->Value)
// 			{
// 				//视频信息
// 				if(Object->IsA(SampleRate_UL))
// 				{
// 					m_VideoInfo.iFrameRate =atoi(Object->GetString().c_str());
// 					m_StreamInfo.VideoInfo.iFrameRate=atoi(Object->GetString().c_str());
// 
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(StoredHeight_UL))
// 				{
// 					m_VideoInfo.iVideoHeight =atoi(Object->GetString().c_str());
// 					m_StreamInfo.VideoInfo.iVideoHeight=atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(StoredWidth_UL))
// 				{
// 					m_VideoInfo.iVideowidth =atoi(Object->GetString().c_str());
// 					m_StreamInfo.VideoInfo.iVideowidth=atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(MaxGOP_UL))
// 				{
// 					m_VideoInfo.iMaxGOP =atoi(Object->GetString().c_str());
// 					m_StreamInfo.VideoInfo.iMaxGOP=atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(AspectRatio_UL))
// 				{
// 					m_VideoInfo.strAspectRatio =(Object->GetString().c_str());
// 					m_StreamInfo.VideoInfo.strAspectRatio=(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(BitRate_UL))
// 				{
// 					m_VideoInfo.lVideoBitRate =atoi(Object->GetString().c_str());
// 					m_StreamInfo.VideoInfo.lVideoBitRate=atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(PictureEssenceCoding_UL))
// 				{
// 					// strPEC = "[060e2b34.0401.0103.04010202.????????]";
// 					// strPEC = "[060e2b34.0401.0103.04010202.????????]";
// 					// strPEC = "[060e2b34.0401.0101.04010202.????????]";//dv
// 					m_VideoInfo.strVideoCodecFormat            = (Object->GetString().c_str());
// 					m_i16VideoCoderFormat = Object->GetInt();
// 					switch (m_i16VideoCoderFormat )
// 					{
// 					case 0x0103:
// 						m_VideoInfo.strVideoCodecFormat            = "Mpeg2 Codec";
// 						m_StreamInfo.VideoInfo.strVideoCodecFormat = "Mpeg2 Codec";
// 						break;
// 					case 0x0101:
// 						m_VideoInfo.strVideoCodecFormat            = "DV Codec";
// 						m_StreamInfo.VideoInfo.strVideoCodecFormat = "DV Codec";
// 						break;
// 					default:
// 						m_VideoInfo.strVideoCodecFormat            = (Object->GetString().c_str());
// 					    m_StreamInfo.VideoInfo.strVideoCodecFormat = (Object->GetString().c_str());
// 						break;
// 					}
// // 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(HorizontalSubsampling_UL))
// 				{
// 					if(atoi(Object->GetString().c_str()) > 0)
// 					{
// 						m_VideoInfo.iVideoHeight *= atoi(Object->GetString().c_str());
// 						m_StreamInfo.VideoInfo.iVideoHeight *= atoi(Object->GetString().c_str());
// 					}
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(VerticalSubsampling_UL))
// 				{
// 					if(atoi(Object->GetString().c_str()) > 0)
// 					{
// 						m_VideoInfo.iVideowidth            *= atoi(Object->GetString().c_str());
// 					    m_StreamInfo.VideoInfo.iVideowidth *= atoi(Object->GetString().c_str());
// 					}
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				
// 				if (Object->IsA(Duration_UL))
// 				{
// 					m_StreamInfo.llFileDuration        =atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 
// 				//音频信息
// 				if(Object->IsA(SoundEssenceCompression_UL))
// 				{
// 					m_AudioInfo.strAudioCodecFormat =(Object->GetString().c_str());
// 					m_i16AudioCoderFormat = Object->GetInt();
// 					switch (m_i16AudioCoderFormat )
// 					{
// 					case 0x0101:
// 						m_AudioInfo.strAudioCodecFormat = "PCM_S16LE";
// 						m_StreamInfo.AudioInfo.strAudioCodecFormat = "PCM_S16LE";
// 					case 0x0107:
// 						m_AudioInfo.strAudioCodecFormat = "PCM_S16BE";
// 						m_StreamInfo.AudioInfo.strAudioCodecFormat = "PCM_S16BE";
// 						break;
// 					default:
// 						m_AudioInfo.strAudioCodecFormat =(Object->GetString().c_str());
// 						m_StreamInfo.AudioInfo.strAudioCodecFormat=(Object->GetString().c_str());
// 						break;
// 					}
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(ChannelCount_UL))
// 				{
// 					m_AudioInfo.lChannels            = atoi(Object->GetString().c_str());
// 					m_StreamInfo.AudioInfo.lChannels = atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(QuantizationBits_UL))
// 				{
// 					m_AudioInfo.lAudioQuantizationBits            = atoi(Object->GetString().c_str());
// 					m_StreamInfo.AudioInfo.lAudioQuantizationBits = atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(AudioSamplingRate_UL))
// 				{
// 					m_AudioInfo.iAudioSamplingRate            = atoi(Object->GetString().c_str());
// 					m_StreamInfo.AudioInfo.iAudioSamplingRate = atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				if(Object->IsA(ChannelCount_UL))
// 				{
// 					m_AudioInfo.lChannels            = atoi(Object->GetString().c_str());
// 					m_StreamInfo.AudioInfo.lChannels = atoi(Object->GetString().c_str());
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 				//文件信息
//                 if(Object->IsA(OperationalPattern_UL))
// 				{
// 					m_StreamInfo.strOP = Object->GetString().c_str();
// 					printf("%s%s = %s\n", Prefix.c_str(), Object->Name().c_str(), Object->GetString().c_str());
// 				}
// 
// 			}
// 			else
// 			{
// 				printf("%s%s\n", Prefix.c_str(), Object->Name().c_str());
// 
// 			}
// 
// 		}
// 
// 		MDObjectULList::iterator it = Object->begin();
// 		while(it != Object->end())
// 		{
// 			DumpObject((*it).second, Prefix + "  ");
// 			it++;
// 		}
// 		
// 	}
// 	return;
// }

long CMxfInfoExtract::FindStartCode(const BYTE * pBuffer,long Lentgh,DWORD code)
{
	code = SWAP_ULONG((BYTE*)&code);
	long offset = 0;
	while (offset < (Lentgh-4))
	{
		if (* (DWORD*)(pBuffer + offset) == code)
		{
			return offset;
		}
		offset ++;
	}
	return -1;
}

bool CMxfInfoExtract::BuildEssenceInfo(mxflib::PartitionPtr &pPartition)
{
	using namespace mxflib;
	if(!pPartition) 
	{
		return false;
	}
    
	// Read and parse the metadata
	pPartition->ReadMetadata();
	MetadataPtr HMeta = pPartition->ParseMetadata();
	
	if(!HMeta) 
	{
		return false;
	}
	//得到操作模式OperationalPattern_UL
	MDObjectPtr ECDSet = HMeta[OperationalPattern_UL];
	if(!ECDSet)
	{
		return false;
	}
    m_StreamInfo.strOP = ECDSet->GetString().c_str();

	long lOrigin = 0;
	// Now find the other items for the essence lookup map 
	PackageList::iterator it = HMeta->Packages.begin();
	while(it != HMeta->Packages.end())
	{
		if((*it)->IsA(MaterialPackage_UL))
		{
			TrackList::iterator Track_it = (*it)->Tracks.begin();
			while(Track_it != (*it)->Tracks.end())
			{
				if ((*Track_it)->GetType() == Track::TypePicture)
				{
					MDObjectPtr OriginObj = (*Track_it)->Child(Origin_UL);
					lOrigin = OriginObj->Value->GetUInt();
					break;
				}
				Track_it++;
			}
		}
		if((*it)->IsA(SourcePackage_UL))
		{
			
			TrackList::iterator Track_it = (*it)->Tracks.begin();
			while(Track_it != (*it)->Tracks.end())
			{
				if ((*Track_it)->GetType() == Track::TypePicture)
				{
					if(lOrigin <= 0)
					{
						MDObjectPtr OriginObj = (*Track_it)->Child(Origin_UL);
						lOrigin = OriginObj->Value->GetUInt();
					}
					MDObjectPtr SequenceObj = (*Track_it)->Child(Sequence_UL);
//                    m_StreamInfo.llFileDuration = (*Track_it)->UpdateDuration();
					if (SequenceObj)
					{
						if (SequenceObj->GetLink())
						{
							SequenceObj = SequenceObj->GetLink();
						}
						if (!SequenceObj->GetLink())
						{
							if (!SequenceObj->Value)
							{
								MDObjectULList::iterator  ObjIt = SequenceObj->begin();
								while(ObjIt != SequenceObj->end())
								{
									MDObjectPtr Obj = (*ObjIt).second;
									if (Obj->IsA(Duration_UL))
									{
										m_StreamInfo.llFileDuration = Obj->Value->GetUInt();
									}
									ObjIt++;
									
								}
							}
						}
						
					}
					break;
				}
				Track_it++;
			}
			MDObjectPtr Descriptor = (*it)->Child(Descriptor_UL);
			if(Descriptor) 
				Descriptor = Descriptor->GetLink();
			
			if(Descriptor)//分析信息
			{
				//////////////////////////////////////////
				if(Descriptor->IsA(MultipleDescriptor_UL))
				{
					MDObjectPtr DescriptorList = Descriptor[SubDescriptorUIDs_UL];
					if(DescriptorList)
					{
						int DescriptorPos = 0;
						
						MDObject::iterator Despit = DescriptorList->begin();
						while(Despit != DescriptorList->end())
						{
							MDObjectPtr SubDescriptor = (*Despit).second->GetLink();
							if(SubDescriptor)
							{
								if(SubDescriptor->IsA(WaveAudioDescriptor_UL)
									|| SubDescriptor->IsA(AES3PCMDescriptor_UL)
									|| SubDescriptor->IsA(GenericSoundEssenceDescriptor_UL))
								{
									GetAudioInfo(SubDescriptor);
								}
								if(SubDescriptor->IsA(MPEG2VideoDescriptor_UL)
									|| SubDescriptor->IsA(CDCIEssenceDescriptor_UL)
									|| SubDescriptor->IsA(GenericPictureEssenceDescriptor_UL))
								{
									GetVideoInfo(SubDescriptor);
								}

							}
							Despit++;
						}
					}
				}
				else
				{
					GetAudioInfo(Descriptor);
					GetVideoInfo(Descriptor);
				}
			}
		}
		it++;
	}
  

	if (lOrigin > 0)
	{
		m_StreamInfo.llFileDuration = m_StreamInfo.llFileDuration - lOrigin;
	}
	return true;
}

bool CMxfInfoExtract::GetAudioInfo(MDObjectPtr pDesp)
{
	if (!pDesp)
	{
		return false;
	}
    
	MDObjectULList::iterator it = pDesp->begin();
	while(it != pDesp->end())
	{
		//音频信息
		MDObjectPtr Object = (*it).second;
		if(Object->GetLink())
			break;
		if(Object->IsA(SoundEssenceCompression_UL))
		{
			m_ui8AudioCoderFormat = -1;
			UInt8* Ptr = Object->GetData().Data;
			m_AudioInfo.strAudioCodecFormat = ConvertEssenceCoding2String(Ptr);
			m_StreamInfo.AudioInfo.strAudioCodecFormat = m_AudioInfo.strAudioCodecFormat;
// 			if (Ptr)
// 			{
// 				m_ui8AudioCoderFormat = *(Ptr+12);
// 			}
// 
//  			switch (m_ui8AudioCoderFormat )
//  			{
// 			case 0x7F:
// 				m_AudioInfo.strAudioCodecFormat = "UnCompressed Sound Codec";
// 				m_StreamInfo.AudioInfo.strAudioCodecFormat = "UnCompressed Sound Codec";
// 			case 0x7E:
// 				m_AudioInfo.strAudioCodecFormat = "AIFF UnCompressed Codec";
// 				m_StreamInfo.AudioInfo.strAudioCodecFormat = "AIFF UnCompressed Codec";
// 				break;
// 			default:
// 				m_AudioInfo.strAudioCodecFormat =(Object->GetString().c_str());
// 				m_StreamInfo.AudioInfo.strAudioCodecFormat=(Object->GetString().c_str());
// 				break;
// 			}
		}
		if(Object->IsA(ChannelCount_UL))
		{
			m_AudioInfo.lChannels            += atoi(Object->GetString().c_str());
			m_StreamInfo.AudioInfo.lChannels += atoi(Object->GetString().c_str());
		}
		if(Object->IsA(QuantizationBits_UL))
		{
			m_AudioInfo.lAudioQuantizationBits            = atoi(Object->GetString().c_str());
			m_StreamInfo.AudioInfo.lAudioQuantizationBits = atoi(Object->GetString().c_str());
		}
		if(Object->IsA(AudioSamplingRate_UL))
		{
			m_AudioInfo.iAudioSamplingRate            = atoi(Object->GetString().c_str());
			m_StreamInfo.AudioInfo.iAudioSamplingRate = atoi(Object->GetString().c_str());
		}
		it++;
	}
				
	return true;
				
}

bool CMxfInfoExtract::GetVideoInfo(MDObjectPtr pDesp)
{
   	if (!pDesp)
	{
		return false;
	}

	UInt8  nFrameLayout = 0;
	int iStoredHeight   = 0;
	int iStoredWidth    = 0;
	int iDisplayHeight  = 0;
	int iDisplayWidth   = 0;
	int iDisplayXOffset = 0;
    int iDisplayYOffset = 0;
	MDObjectULList::iterator it = pDesp->begin();
	while(it != pDesp->end())
	{
		MDObjectPtr Object = (*it).second;
		if(Object->GetLink())
			break;
		//视频信息
		if(Object->IsA(SampleRate_UL))
		{
			m_VideoInfo.iFrameRate =atoi(Object->GetString().c_str());
			m_StreamInfo.VideoInfo.iFrameRate=atoi(Object->GetString().c_str());
		}
		if(Object->IsA(StoredHeight_UL))
		{
			iStoredHeight =atoi(Object->GetString().c_str());
		}
		if(Object->IsA(StoredWidth_UL))
		{
			iStoredWidth =atoi(Object->GetString().c_str());
		}
		if(Object->IsA(DisplayHeight_UL))
		{
			iDisplayHeight =atoi(Object->GetString().c_str());
		}
		if(Object->IsA(DisplayWidth_UL))
		{
			iDisplayWidth =atoi(Object->GetString().c_str());
		}
		if(Object->IsA(MaxGOP_UL))
		{
			m_VideoInfo.iMaxGOP =atoi(Object->GetString().c_str());
			m_StreamInfo.VideoInfo.iMaxGOP=atoi(Object->GetString().c_str());
		}
		if(Object->IsA(AspectRatio_UL))
		{
			m_VideoInfo.strAspectRatio =(Object->GetString().c_str());
			m_StreamInfo.VideoInfo.strAspectRatio=(Object->GetString().c_str());
		}
		if(Object->IsA(BitRate_UL))
		{
			m_VideoInfo.lVideoBitRate =atoi(Object->GetString().c_str());
			m_StreamInfo.VideoInfo.lVideoBitRate=atoi(Object->GetString().c_str());
		}
		if(Object->IsA(PictureEssenceCoding_UL))
		{
			m_ui8VideoCoderFormat = -1;
			UInt8* Ptr = Object->GetData().Data;
			m_VideoInfo.strVideoCodecFormat = ConvertEssenceCoding2String(Ptr);
			m_StreamInfo.VideoInfo.strVideoCodecFormat = m_VideoInfo.strVideoCodecFormat;
		}
		if((*it).second->IsA(DisplayXOffset_UL))
		{
			iDisplayXOffset = atoi(Object->GetString().c_str());
		}
		if((*it).second->IsA(DisplayYOffset_UL))
		{
			iDisplayYOffset = atoi(Object->GetString().c_str());
		}
		//FrameLayout_UL
		if(Object->IsA(FrameLayout_UL))
		{
		   nFrameLayout = atoi(Object->GetString().c_str());
		}
		it++;
	}
	
	switch (nFrameLayout)
	{
	case 0:
		break;
	case 1:
	case 2:
	case 3:
		iDisplayHeight *= 2;
		iStoredHeight  *= 2;
		break;
	case 4:
		iDisplayWidth *= 2;
		iStoredWidth  *= 2;
		break;
	}

	// 	m_VideoInfo.iVideoHeight -= iDisplayYOffset;
	// 	m_VideoInfo.iVideowidth  -= iDisplayXOffset;
	if (iStoredHeight > 0 && iStoredWidth > 0)
	{
		m_VideoInfo.iVideoHeight = iStoredHeight;
        m_VideoInfo.iVideowidth  = iStoredWidth;
	}
	if(iDisplayHeight > 0 && iDisplayWidth > 0)
	{
		m_VideoInfo.iVideoHeight = iDisplayHeight + iDisplayYOffset*2;
        m_VideoInfo.iVideowidth  = iDisplayWidth + iDisplayXOffset*2;
	}

	m_StreamInfo.VideoInfo.iVideoHeight = m_VideoInfo.iVideoHeight;
	m_StreamInfo.VideoInfo.iVideowidth = m_VideoInfo.iVideowidth;
	return true;
}
//RP 224
string CMxfInfoExtract::ConvertEssenceCoding2String(const UInt8 *pData)
{
	string strCodecName = "";

    if (IsBadReadPtr(pData,16))
	{
		return strCodecName;
	}

	switch (pData[9])//audio or video
	{
	case 0x01:
		switch (pData[12])
		{
		case 0x01://Mpeg
			switch (pData[13])
			{
			case 0x01:
			case 0x02:
			case 0x03:
			case 0x04:
			case 0x05:
			case 0x06:
			case 0x07:
			case 0x08:
				m_ui8VideoCoderFormat = 0x01;
				strCodecName = "Mpeg2 Codec";
				break;
			case 0x10:
				strCodecName = "Mpeg1 Codec";
				break;
			case 0x20:
				strCodecName = "Mpeg4 Codec";
				break;
			}
			break;
		case 0x02://DV
			switch (pData[13])
			{
			case 0x01:
				strCodecName = "DV25 Codec";
				break;
			case 0x02:
				switch (pData[14])
				{
				case 0x01:
				case 0x02:
					strCodecName = "DV25 Codec";
					break;
				case 0x03:
				case 0x04:
					strCodecName = "DV50 Codec";
					break;
				case 0x05:
				case 0x06:
				case 0x07:
				case 0x08:
					strCodecName = "DV100 Codec";
					break;	
				}
				break;
			}
			break;
		}
		break;
	case 0x02:
		switch (pData[11])
		{
		case 0x01:
			switch (pData[12])
			{
			case 0x7E:
				strCodecName = "AIFF Uncompressed Codec";
				break;
			case 0x7F:
				strCodecName = "Undefined Sound Codec";
				break;
			}
			break;
		case 0x02:
			switch (pData[12])
			{
			case 0x01:
			case 0x02:
				strCodecName = "Uncompressed Sound Codec";
				break;
			case 0x03:
				switch (pData[13])
				{
				case 0x01:
					switch (pData[14])
					{
					case 0x01:
                        strCodecName = "A-law Coded Audio";
						break;
					case 0x10:
                        strCodecName = "DV Compressed Audio";
						break;
					}
					break;
				case 0x02:
					switch (pData[14])
					{
					case 0x01:
                        strCodecName = "ATSC A-52 Compressed Audio";
						break;
					case 0x04:
					case 0x05:
                        strCodecName = "MPEG-1 Layer-1/2 Compressed Audio";
						break;
					case 0x06:
                        strCodecName = "MPEG-2 Layer-1 Compressed Audio";
						break;
					case 0x1C:
                        strCodecName = "Dolby-E Compressed Audio";
						break;
					}
					break;
				case 0x03:
					switch (pData[14])
					{
					case 0x01:
						strCodecName = "MPEG-2 AAC Compressed Audio";
						break;
					}
					break;
				}
				break;
			}
			break;
		case 0x10:
			switch (pData[12])
			{
			case 0x02:
				switch (pData[13])
				{
				case 0x01:
					strCodecName = "SMPTE-320M 8-Channel ModeA";
					break;
				case 0x02:
					strCodecName = "SMPTE-320M 8-Channel ModeB";
					break;
				}
				break;
			}
			break;
		}
		break;
	}
	return strCodecName;
}

bool CMxfInfoExtract::GetAFDInfo( std::string& strFileIn, byte& outInfo )
{
	
	CAFDWriter writer;
	Int64 nSeekPos = 0;
	int nRet = writer.GetAFDInfo(CString(strFileIn.c_str()), outInfo, nSeekPos);
	return nRet == AWERR_SUCCESS;
}
