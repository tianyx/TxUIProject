//////////////////////////////////////////////////////////////////////////
//
//	created:	2005/05/12
//	file name:	mpeg2enc_mc.h
//
///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2006 MainConcept AG
//  All rights are reserved.  Reproduction  in whole or  in part is  prohibited
//  without the written consent of the copyright owner.
//
//  MainConcept AG reserves the right  to make changes to this software program
//  without notice  at any time.  MainConcept AG makes no warranty,  expressed,
//  implied or statutory, including but not limited to  any implied warranty of
//  merchantability of fitness for any particular purpose.
//  MainConcept AG  does not represent  or warrant that  the programs furnished
//  hereunder are free of infringement of any third-party patents, copyright or
//  trademark.
//  In no event shall MainConcept AG be liable for any incidental, punitive, or
//  consequential damages of any kind  whatsoever arising from the use of these
//  programs.
//
///////////////////////////////////////////////////////////////////////////////
//
//  Purpose: The definition of the MainConcept Mpeg2 Encoder filter's parameters CLSIDs.
//
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////
//	GUID					Value Type		Available range		Default Value	Note
//
//	EMC_PRESET					VT_INT			MPEG1				MPEG_MPEG2		MPEG-1
//												VCD,								MPEG-1 VideoCD
//												DVD_MPEG1							MPEG-1 DVD
//												MPEG2								MPEG-2
//												SVCD								MPEG-2 SVCD
//												DVD									MPEG-2 DVD
//												DVB									MPEG-2 DVB
//												MMV									MPEG-2 MicroMV
//												DVHS								MPEG-1/2 DVHS
//												HD_DVD								MPEG-2 HD-DVD
//												HDV_HD1								MPEG-2 HDV HD1
//												HDV_HD2								MPEG-2 HDV HD2
//												ATSC								MPEG-2 ATSC
//												D10_25								MPEG-2 D10 25 Mbit/s
//												D10_30								MPEG-2 D10 30 Mbit/s
//												D10_40								MPEG-2 D10 40 Mbit/s
//												D10_50								MPEG-2 D10 50 Mbit/s
//												XDCAM_1080_CBR_25					XDCAM format (1440x1080 25 Mbit CBR)
//												XDCAM_1080_VBR_17_5					XDCAM format (1440x1080 17.5 Mbit VBR)
//												XDCAM_1080_VBR_25					XDCAM format (1440x1080 25 Mbit VBR)
//												XDCAM_1080_VBR_35					XDCAM format (1440x1080 35 Mbit VBR)
//												XDCAM_540_CBR_12_5					XDCAM format (1440x540 12.5 Mbit CBR)
//												XDCAM_540_VBR_8_75					XDCAM format (1440x540 8.75 Mbit VBR)
//												XDCAM_540_VBR_12_5					XDCAM format (1440x540 12.5 Mbit VBR)
//												XDCAM_540_VBR_17_5					XDCAM format (1440x540 17.5 Mbit VBR)
//	EM2VE_VideoFormat			VT_INT			0,1,2,3,4,5			0				0 - Autodetect, 1-PAL, 2-NTSC, 3- SECAM, 4-MAC, 5-Unspecified format
//	EMC_PROFILE					VT_INT			HIGH_PROFILE		PROFILE_MAIN	High profile
//												MAIN_PROFILE						Main profile
//												SIMPLE_PROFILE						Simple profile
//												PROFILE_4_2_2							442 profile
//	EMC_LEVEL					VT_INT			HIGH_LEVEL			LEVEL_MAIN			High level
//												HIGH1440_LEVEL						High1440 level
//												MAIN_LEVEL							Main level
//												LOW_LEVEL							Low level
//												UNSPECIFIED_LEVEL					Unspecified level. No level's restrictions presents.  
//	EMC_GOP_LENGTH				VT_INT			[1-100], multiply of M		12		Number of frames in GOP
//	EMC_GOP_BCOUNT				VT_INT			[0-7]				3				Number of B-frames between I/P frames
//	EM2VE_ClosedGOP_Interval	VT_INT								0			Specifies the closed_gop periodicity (0 - no closed GOP, 1 - every GOP is closed, 2- every second GOP is closed).
//	EMC_ASPECT_RATIO			VT_INT												The display aspect ratio code and is different depending on the MPEG type. (see AspectRatio enum)
//	EM2VE_Framerate				VT_INT			[0-8]				0				This field specifies the frame rate code. 0 - autodetect, 1 - 23.976 fps, 2 - 24.0 fps, 3 - 25.0 fps, 4 - 29.97 fps, 5 - 30.0 fps, 6 - 50.0 fps, 7 - 59.94 fps, 8 - 60.0 fps
//	EM2VE_InputFramerate		VT_R8								0			Indicate framerate of input video.
//	EM2VE_Pulldown				VT_INT			0,1,2				0				Specifies the NTSC pulldown generated in the video stream. 0 - no pulldown, 1 - encode 2:3 pulldown, 2 - encode 3:2 pulldown
//	EMC_BITRATE_AVG				VT_INT			Level limitation					Average bitrate in bits
//	EMC_BITRATE_MAX				VT_INT			Level limitation					Maximum bitrate in bits (available for VBR RC mode, should be larger then average bitrate) 
//	EMC_BITRATE_MIN				VT_INT			Level limitation					Minimum bitrate in bits (available for VBR RC mode, should be smaler then average bitrate) 
//	EMC_BITRATE_MODE			VT_INT			0,1					0				Indicate whether the encoder should encode in constant or variable bitrate mode. 1 - constant bitrate, 0 - variable bitrate.
//	EM2VE_WriteSh				VT_INT			0,1					1				Specifies whether to write a sequence header every GOP or just one at the beginning of the stream. 0 - only put a sequence header at the beginning of the stream, 1 - put a sequence header before every GOP.
//	EM2VE_WriteSec				VT_INT			0,1					1				Specifies whether the encoder writes a sequence end code at the end of the stream. 0 - do not write a sequence end code, 1 - write a sequence end code
//	EM2VE_WriteSde				VT_INT			0,1					0				Specifies whether a sequence display extension is written in every GOP. 0 - do not write any sequence display extensions, 1 - write a sequence display extension in every GOP
//	EM2VE_WritePde				VT_INT			0,1					0				This field specifies whether picture display extensions are written in the video stream. 0 - do not insert picture display extensions, 1 - insert picture display extensions
//	EM2VE_DC_Prec				VT_INT			[0-3]				0				The effective precision of the DC coefficient in MPEG-2 intra coded macroblocks. 0 - 8 bit, 1 - 9 bit, 2 - 10 bit, 3 - 11 bit.
//	EM2VE_ProgSeq				VT_INT			0,1					0				Specifies whether the stream can contain interlaced video. 0 - stream can contain interlaced video, 1 - stream is progressive only.
//	EM2VE_ProgFrame				VT_INT			0,1					0				This field specifies whether the frames are interlaced or progressive. 0 - interlaced frames, 1 - progressive frames.
//	EM2VE_RepeatFirst			VT_INT			0,1					0				Specifies whether to repeat the first field of a frame after displaying the two fields. 0 - display first and second fields, 1 - display first and second fields, then display the first again.
//	EM2VE_TopFirst				VT_INT			0,1					0				which of the two fields of an interlaced frame comes earlier in time, the top (odd) field or the bottom (even) field.  If NTSC pulldown is in effect, this field is set automatically. 0 - bottom field first, 1 - top field first.
//	EM2VE_ME_Level				VT_INT			[0-15]				6				This field specifies the motion search type used. 0 - no motion search (fast), 7 - middle motion search method, medium quality, 15 - best motion search method, high quality.
//	EM2VE_ME_HalfPel			VT_INT			0,1					1				Specifies wheather motion estimator uses half pel motion searching. 0 - no half pel search, 1 - half pel motion search.
//	EM2VE_ME_Range				VT_INT			[0-31]				15				Specifies motion search range. 0 - no motion search (fast), 31 - wide motion search range (slow).
//	EM2VE_DeinterlacingMode		VT_INT			0,1					0				This field specifies the deinterlacing mode. 0 - don’t deinterlace, 1 - enable deinterlacing.
//	EM2VE_SCD					VT_INT			EM2VE::SCDType		0				Scene change detection. 0 - none, 1 - fast, 2 - refined, 3-regular.
//	EMC_OUT_SAMPLE_SIZE			VT_INT			[0..-]				0				Specifies size of output samples. 0 - size of output sample will be calculeted automatically from picture size.
//	EMC_OEM_NAME				VT_BSTR			READ-ONLY							Indicate OEM custommer's name
//	EMC_OPERATION_MODE			VT_INT			EMC_COMMON::OperationMode			Indicate evaluation state of product
//	EMC_ENCODED_FRAMES			VT_INT			READ-ONLY							indicate amount of encoded frames
//	EM2VE_PAD_SKIPPED_FRAMES	VT_INT			0,1					0				Pad skipped frames
//	EM2VE_ChromaFormat			VT_INT			CHROMA_420,			CHROMA_420		Chroma format
//												CHROMA_422	
//	EM2VE_DisplayHorizontalSize VT_INT			READ-ONLY							Input picture horizontal size
//	EM2VE_DisplayVerticalSize	VT_INT			READ-ONLY							Input picture vertical size
//	EM2VE_QScale				VT_INT			LINEAR,				LINEAR			Quantization scale type
//												NON_LINEAR
//	EM2VE_IntraVLC				VT_INT			INTRA_VLC_TABLE1,					Intra VLC
//												INTRA_VLC_TABLE2
//	EM2VE_AltScan				VT_INT			ALTERNATE_SCAN,		ZIG_ZAG_SCAN	Scanning type
//												ZIG_ZAG_SCAN
//  EM2VE_VBV_BufferSize		VT_INT			depends on level	0				Specifies the size of the virtual buffer verifier in 2K byte (2048) units. If set to 0, the encoder will compute an appropriate value.
//	EM2VE_ClosedCaption			VT_INT			EM2VE::CC_FORMAT	CC_NONE			Specifies using of closed caption data.
//	EMC_BITRATE_PASS			VT_INT			EM2VE::TwoPassMode_t TWO_PASS_MODE_OFF Specifies using two-pass encoding mode.
//	EMC_BITRATE_STAT			VT_BSTR			-									File path for storage rate control statistics
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(__EM2VE_HEADER__)
#define __EM2VE_HEADER__
#include "common_mc.h"

/*****************************************
	Parameters GUIDs
*****************************************/
// {080AFC13-FF7B-4ec8-867D-86B9DD4B7228}
static const GUID EM2VE_QUANT_I = 
{ 0x80afc13, 0xff7b, 0x4ec8, { 0x86, 0x7d, 0x86, 0xb9, 0xdd, 0x4b, 0x72, 0x28 } };

// {88B2F2B7-B3D0-4e11-A10E-51C8E0352371}
static const GUID EM2VE_QUANT_P = 
{ 0x88b2f2b7, 0xb3d0, 0x4e11, { 0xa1, 0xe, 0x51, 0xc8, 0xe0, 0x35, 0x23, 0x71 } };

// {E34C30D7-C16D-4a73-AEE8-FABFE97F0202}
static const GUID EM2VE_QUANT_B = 
{ 0xe34c30d7, 0xc16d, 0x4a73, { 0xae, 0xe8, 0xfa, 0xbf, 0xe9, 0x7f, 0x2, 0x2 } };

// {785F2F74-898E-4007-BEC1-90A3BD7DDF5E}
static const GUID EM2VE_ClosedGOP_Interval = 
{ 0x785f2f74, 0x898e, 0x4007, { 0xbe, 0xc1, 0x90, 0xa3, 0xbd, 0x7d, 0xdf, 0x5e } };

// {56D56480-BD09-4f97-A59F-525086A8A70B}
static const GUID EM2VE_Framerate = 
{ 0x56d56480, 0xbd09, 0x4f97, { 0xa5, 0x9f, 0x52, 0x50, 0x86, 0xa8, 0xa7, 0xb } };

// {1406B33D-21F7-4a86-98CD-E34B93113901}
static const GUID EM2VE_ChromaFormat = 
{ 0x1406b33d, 0x21f7, 0x4a86, { 0x98, 0xcd, 0xe3, 0x4b, 0x93, 0x11, 0x39, 0x1 } };

// {E54F50CD-E336-4872-8710-34199CF72FFA}
static const GUID EM2VE_VideoFormat = 
{ 0xe54f50cd, 0xe336, 0x4872, { 0x87, 0x10, 0x34, 0x19, 0x9c, 0xf7, 0x2f, 0xfa } };

// {9A58E4BC-1B3E-4127-A0DD-87DBD7D2EEC6}
static const GUID EM2VE_DisplayHorizontalSize = 
{ 0x9a58e4bc, 0x1b3e, 0x4127, { 0xa0, 0xdd, 0x87, 0xdb, 0xd7, 0xd2, 0xee, 0xc6 } };

// {C77F6C0A-CE41-4717-A510-95CCC2F957FD}
static const GUID EM2VE_DisplayVerticalSize = 
{ 0xc77f6c0a, 0xce41, 0x4717, { 0xa5, 0x10, 0x95, 0xcc, 0xc2, 0xf9, 0x57, 0xfd } };

// {8F8FECAC-5CC1-4ce7-8386-4CCC54255485}
static const GUID EM2VE_DC_Prec = 
{ 0x8f8fecac, 0x5cc1, 0x4ce7, { 0x83, 0x86, 0x4c, 0xcc, 0x54, 0x25, 0x54, 0x85 } };

// {C65C2BF0-9AAC-4a5c-B828-ACB5975C6308}
static const GUID EM2VE_ProgSeq = 
{ 0xc65c2bf0, 0x9aac, 0x4a5c, { 0xb8, 0x28, 0xac, 0xb5, 0x97, 0x5c, 0x63, 0x8 } };

// {937DD7FB-8D25-4754-8B80-5B14E1F399E2}
static const GUID EM2VE_ProgFrame = 
{ 0x937dd7fb, 0x8d25, 0x4754, { 0x8b, 0x80, 0x5b, 0x14, 0xe1, 0xf3, 0x99, 0xe2 } };

// {DD352ACE-3B61-4f63-BC6B-90A78EEF6158}
static const GUID EM2VE_TopFirst = 
{ 0xdd352ace, 0x3b61, 0x4f63, { 0xbc, 0x6b, 0x90, 0xa7, 0x8e, 0xef, 0x61, 0x58 } };

// {02ADCA1F-FE7D-4487-BEAD-40A1B34CBDFF}
static const GUID EM2VE_ME_Level = 
{ 0x2adca1f, 0xfe7d, 0x4487, { 0xbe, 0xad, 0x40, 0xa1, 0xb3, 0x4c, 0xbd, 0xff } };

// {9EB2E1BD-945C-4b92-A5A2-894C7F6DBB37}
static const GUID EM2VE_ME_Range = 
{ 0x9eb2e1bd, 0x945c, 0x4b92, { 0xa5, 0xa2, 0x89, 0x4c, 0x7f, 0x6d, 0xbb, 0x37 } };

// {EB66122F-6933-4fa5-9A01-594B3341190E}
static const GUID EM2VE_ME_HalfPel = 
{ 0xeb66122f, 0x6933, 0x4fa5, { 0x9a, 0x1, 0x59, 0x4b, 0x33, 0x41, 0x19, 0xe } };

// {4572677E-B5E5-4612-82FD-B48F44948403}
static const GUID EM2VE_SCD = 
{ 0x4572677e, 0xb5e5, 0x4612, { 0x82, 0xfd, 0xb4, 0x8f, 0x44, 0x94, 0x84, 0x3 } };

// {77ABB9EA-E2AA-4517-9501-043FBD5E5A8C}
static const GUID EM2VE_Pulldown = 
{ 0x77abb9ea, 0xe2aa, 0x4517, { 0x95, 0x1, 0x4, 0x3f, 0xbd, 0x5e, 0x5a, 0x8c } };

// {9B52BBB7-E747-466f-B932-4D9D5623814F}
static const GUID EM2VE_RepeatFirst = 
{ 0x9b52bbb7, 0xe747, 0x466f, { 0xb9, 0x32, 0x4d, 0x9d, 0x56, 0x23, 0x81, 0x4f } };

// {1ADA1B70-F561-4370-AF17-653CDEDEFC8D}
static const GUID EM2VE_WritePde = 
{ 0x1ada1b70, 0xf561, 0x4370, { 0xaf, 0x17, 0x65, 0x3c, 0xde, 0xde, 0xfc, 0x8d } };

// {D3154625-7C86-4571-8581-16DE36BDDAE8}
static const GUID EM2VE_WriteSde = 
{ 0xd3154625, 0x7c86, 0x4571, { 0x85, 0x81, 0x16, 0xde, 0x36, 0xbd, 0xda, 0xe8 } };

// {2050E529-9211-4004-95E6-3F6DEDADA3AB}
static const GUID EM2VE_WriteSec = 
{ 0x2050e529, 0x9211, 0x4004, { 0x95, 0xe6, 0x3f, 0x6d, 0xed, 0xad, 0xa3, 0xab } };

// {BF0AC217-674E-4e01-AE02-7F37F7055614}
static const GUID EM2VE_WriteSh = 
{ 0xbf0ac217, 0x674e, 0x4e01, { 0xae, 0x2, 0x7f, 0x37, 0xf7, 0x5, 0x56, 0x14 } };

// {D80234D9-804A-450b-A590-0AB9CD8C0BBB}
static const GUID EM2VE_DeinterlacingMode = 
{ 0xd80234d9, 0x804a, 0x450b, { 0xa5, 0x90, 0xa, 0xb9, 0xcd, 0x8c, 0xb, 0xbb } };

// {093B53C0-62DA-44f5-802B-615CF07C84D4}
static const GUID EM2VE_ClosedCaption = 
{ 0x93b53c0, 0x62da, 0x44f5, { 0x80, 0x2b, 0x61, 0x5c, 0xf0, 0x7c, 0x84, 0xd4 } };

// {70078D07-0159-4ec4-B3F1-480410C75E7B}
static const GUID EM2VE_OriginalColorSpace = 
{ 0x70078d07, 0x159, 0x4ec4, { 0xb3, 0xf1, 0x48, 0x4, 0x10, 0xc7, 0x5e, 0x7b } };

// {7EBA814A-76ED-4880-AFBE-BA27704746C1}
static const GUID EM2VE_OriginalInterlace = 
{ 0x7eba814a, 0x76ed, 0x4880, { 0xaf, 0xbe, 0xba, 0x27, 0x70, 0x47, 0x46, 0xc1 } };

//-------------------------------------
// {884724C9-9B6B-4374-A8F5-19096C3B8518}
static const GUID EM2VE_ConstrParms = 
{ 0x884724c9, 0x9b6b, 0x4374, { 0xa8, 0xf5, 0x19, 0x9, 0x6c, 0x3b, 0x85, 0x18 } };

// {B5756AEF-AC44-48bb-AD44-15B1B9450B37}
static const GUID EM2VE_DropFrameTC = 
{ 0xb5756aef, 0xac44, 0x48bb, { 0xad, 0x44, 0x15, 0xb1, 0xb9, 0x45, 0xb, 0x37 } };

// {CA347ADF-BE47-4631-8064-BD186F5E1F19}
static const GUID EM2VE_TC0 = 
{ 0xca347adf, 0xbe47, 0x4631, { 0x80, 0x64, 0xbd, 0x18, 0x6f, 0x5e, 0x1f, 0x19 } };

// {8EDEA756-53FA-4b7d-B087-65635F4A187D}
static const GUID EM2VE_IgnoreFrameInterval = 
{ 0x8edea756, 0x53fa, 0x4b7d, { 0xb0, 0x87, 0x65, 0x63, 0x5f, 0x4a, 0x18, 0x7d } };

// {98B4BCEA-B6BD-4835-8716-97CFEEEC2CC4}
static const GUID EM2VE_VBV_BufferSize = 
{ 0x98b4bcea, 0xb6bd, 0x4835, { 0x87, 0x16, 0x97, 0xcf, 0xee, 0xec, 0x2c, 0xc4 } };

// {7237D1F2-8079-45fc-9A7C-B8526C9AC031}
static const GUID EM2VE_MB_QuantValue = 
{ 0x7237d1f2, 0x8079, 0x45fc, { 0x9a, 0x7c, 0xb8, 0x52, 0x6c, 0x9a, 0xc0, 0x31 } };

// {B89496DD-C3B3-429c-BCB1-F8753E5EC333}
static const GUID EM2VE_ColorPrimaries = 
{ 0xb89496dd, 0xc3b3, 0x429c, { 0xbc, 0xb1, 0xf8, 0x75, 0x3e, 0x5e, 0xc3, 0x33 } };

// {C790FFB9-4490-4d01-885D-38B4F7CB8795}
static const GUID EM2VE_TransferCharacteristics = 
{ 0xc790ffb9, 0x4490, 0x4d01, { 0x88, 0x5d, 0x38, 0xb4, 0xf7, 0xcb, 0x87, 0x95 } };

// {01CCA590-83DC-44f0-8356-539F7AE517B1}
static const GUID EM2VE_FeaturesFlags = 
{ 0x1cca590, 0x83dc, 0x44f0, { 0x83, 0x56, 0x53, 0x9f, 0x7a, 0xe5, 0x17, 0xb1 } };

// {5A2063BF-3807-407b-87D3-850113F136F5}
static const GUID EM2VE_MatrixCoefficients = 
{ 0x5a2063bf, 0x3807, 0x407b, { 0x87, 0xd3, 0x85, 0x1, 0x13, 0xf1, 0x36, 0xf5 } };

// {CFD17D2C-1920-494d-8ED2-97DA670AE3F3}
static const GUID EM2VE_UserIquant = 
{ 0xcfd17d2c, 0x1920, 0x494d, { 0x8e, 0xd2, 0x97, 0xda, 0x67, 0xa, 0xe3, 0xf3 } };

// {D49FD03E-4A78-4d5f-99F5-641251D72CD2}
static const GUID EM2VE_UserNIquant = 
{ 0xd49fd03e, 0x4a78, 0x4d5f, { 0x99, 0xf5, 0x64, 0x12, 0x51, 0xd7, 0x2c, 0xd2 } };

// {76C03009-E4A2-49b0-AB95-1CCBE04AFB61}
static const GUID EM2VE_SVCD_user_block = 
{ 0x76c03009, 0xe4a2, 0x49b0, { 0xab, 0x95, 0x1c, 0xcb, 0xe0, 0x4a, 0xfb, 0x61 } };

// {7A80B9E3-BD35-4b05-BD83-9AE51932E7D7}
static const GUID EM2VE_MinFramePercent = 
{ 0x7a80b9e3, 0xbd35, 0x4b05, { 0xbd, 0x83, 0x9a, 0xe5, 0x19, 0x32, 0xe7, 0xd7 } };

// {528B746B-320F-4f69-9AAE-41D5F12C5EE6}
static const GUID EM2VE_PadFramePercent = 
{ 0x528b746b, 0x320f, 0x4f69, { 0x9a, 0xae, 0x41, 0xd5, 0xf1, 0x2c, 0x5e, 0xe6 } };

// {F2794BD1-ED9B-4c15-86F7-3B46615B8489}
static const GUID EM2VE_QScale = 
{ 0xf2794bd1, 0xed9b, 0x4c15, { 0x86, 0xf7, 0x3b, 0x46, 0x61, 0x5b, 0x84, 0x89 } };

// {591DC293-B4B7-45b3-B32E-D3980B2F44EC}
static const GUID EM2VE_IntraVLC = 
{ 0x591dc293, 0xb4b7, 0x45b3, { 0xb3, 0x2e, 0xd3, 0x98, 0xb, 0x2f, 0x44, 0xec } };

// {F281DAD4-9CFF-45d4-8A27-1CD755309F9A}
static const GUID EM2VE_AltScan = 
{ 0xf281dad4, 0x9cff, 0x45d4, { 0x8a, 0x27, 0x1c, 0xd7, 0x55, 0x30, 0x9f, 0x9a } };

// {6B11FD92-CE22-4b0c-BED1-E4FDCFDFDCC6}
static const GUID EM2VE_SetDefValues = 
{ 0x6b11fd92, 0xce22, 0x4b0c, { 0xbe, 0xd1, 0xe4, 0xfd, 0xcf, 0xdf, 0xdc, 0xc6 } };

// {43CD5542-DDEC-4c13-9276-D25059C40760}
static const GUID EM2VE_IsRunning = 
{ 0x43cd5542, 0xddec, 0x4c13, { 0x92, 0x76, 0xd2, 0x50, 0x59, 0xc4, 0x7, 0x60 } };

// {106364D3-97BC-41a8-AA43-1D0EE2611670}
static const GUID EM2VE_InputFramerate = 
{ 0x106364d3, 0x97bc, 0x41a8, { 0xaa, 0x43, 0x1d, 0xe, 0xe2, 0x61, 0x16, 0x70 } };

// {3A79347D-CB9D-43d6-A53A-E11618CBAC0C}
static const GUID EM2VE_PAD_SKIPPED_FRAMES = 
{ 0x3a79347d, 0xcb9d, 0x43d6, { 0xa5, 0x3a, 0xe1, 0x16, 0x18, 0xcb, 0xac, 0xc } };

// {B5DF6FDD-BD91-4723-B3DE-A6A3FFD34455}
static const GUID EM2VE_PREVIEW = 
{ 0xb5df6fdd, 0xbd91, 0x4723, { 0xb3, 0xde, 0xa6, 0xa3, 0xff, 0xd3, 0x44, 0x55 } };

// {810FE8A3-C7D0-438e-8ECA-D3B9E59F323E}
static const GUID EM2VE_PerformanceOnline = 
{ 0x810fe8a3, 0xc7d0, 0x438e, { 0x8e, 0xca, 0xd3, 0xb9, 0xe5, 0x9f, 0x32, 0x3e } };

// {616757DA-667A-4e3e-A84E-8A8C944FCCC3}
static const GUID EM2VE_PerformanceOffline = 
{ 0x616757da, 0x667a, 0x4e3e, { 0xa8, 0x4e, 0x8a, 0x8c, 0x94, 0x4f, 0xcc, 0xc3 } };

// {B713CEFE-F118-4136-98E8-EFE90800049C}
static const GUID EM2VE_Prefiltering = 
{ 0xB713CEFE, 0xF118, 0x4136, { 0x98, 0xE8, 0xEF, 0xE9, 0x08, 0x00, 0x04, 0x9C } };

// {51D02FEC-4F45-4B18-9847-A165C5479180}
static const GUID EM2VE_SkipMode =
{ 0x51D02FEC, 0x4F45, 0x4B18, { 0x98, 0x47, 0xA1, 0x65, 0xC5, 0x47, 0x91, 0x80 } };

namespace EM2VE
{
#define MAX_II_DISTANCE 100
#define MAX_IP_DISTANCE 8
#define DEFAULT_FRAMERATE_CODE 3

	enum VideoType
	{
		MPEG1 = 0,
		VCD = 1,
		DVD_MPEG1 = 13,
		MPEG2 = 2,
		SVCD = 3,
		DVD = 4,
		DVB = 8,	
		MMV = 9,
		DVHS = 10,
		HDV_HD1 = 18,
		HDV_HD2 = 19,
		ATSC = 11,
		D10_25 = 20,
		D10_30 = 21,
		D10_40 = 22,
		D10_50 = 23,
		HD_DVD = 25,
		XDCAM_1080_CBR_25 = 28,
		XDCAM_1080_VBR_17_5 =29,
		XDCAM_1080_VBR_25 =30,
		XDCAM_1080_VBR_35 = 31,	   // XDCAM MP@HL	(35 Mbit VBR)
		XDCAM_540_CBR_12_5 = 32,	   // XDCAM MP@H-14	(12.5 Mbit CBR)
		XDCAM_540_VBR_8_75 = 33,	   // XDCAM MP@HL	(8.75 Mbit VBR)
		XDCAM_540_VBR_12_5 = 34,	   // XDCAM MP@HL	(12.5 Mbit VBR)
		XDCAM_540_VBR_17_5 = 35	   // XDCAM MP@HL	(17.5 Mbit VBR)
	};

	enum VideoFormat
	{
		FORMAT_AUTO = 0,
		FORMAT_PAL = 1,
		FORMAT_NTSC = 2,
		FORMAT_SECAM = 3,
		FORMAT_MAC = 4,
		FORMAT_UNSPECIFIED = 5
	};

	enum Profile
	{
		HIGH_PROFILE = 1,
		MAIN_PROFILE = 4,
		SIMPLE_PROFILE = 5,
		PROFILE_4_2_2 = 7
	};

	enum Level
	{
		HIGH_LEVEL = 4,
		HIGH1440_LEVEL = 6,
		MAIN_LEVEL = 8,
		LOW_LEVEL = 10,
		UNSPECIFIED_LEVEL = 15
	};

	enum Framerate
	{
		FRAMERATE_AUTO = 0,
		FRAMERATE_23 = 1,
		FRAMERATE_24 = 2,
		FRAMERATE_25 = 3,
		FRAMERATE_29 = 4,
		FRAMERATE_30 = 5,
		FRAMERATE_50 = 6,
		FRAMERATE_59 = 7,
		FRAMERATE_60 = 8		
	};

	enum SCDType
	{
		SCD_NONE = 0,
		SCD_FAST = 1,
		SCD_REFINED = 2,
		SCD_REGULAR = 3
	};

	enum PulldownType
	{
		PULLDOWN_NO = 0,
		PULLDOWN_2_3 = 1,
		PULLDOWN_3_2 = 2
	};

	enum DCPrecision
	{
		PRECISION_8_BIT = 0,
		PRECISION_9_BIT = 1,
		PRECISION_10_BIT = 2,
		PRECISION_11_BIT = 3
	};

	enum ChromaFormat
	{
		CHROMA_420 = 1,
		CHROMA_422 = 2
	};
	enum AspectRatio
	{
		ASPECT_MPEG1_1x1 = 1,
		ASPECT_MPEG1_0_6735 = 2,
		ASPECT_MPEG1_0_7031 = 3,
		ASPECT_MPEG1_0_7615 = 4,
		ASPECT_MPEG1_0_8055 = 5,
		ASPECT_MPEG1_0_8437 = 6,
		ASPECT_MPEG1_0_8935 = 7,
		ASPECT_MPEG1_0_9157 = 8,
		ASPECT_MPEG1_0_9815 = 9,
		ASPECT_MPEG1_1_0255 = 10,
		ASPECT_MPEG1_1_0695 = 11,
		ASPECT_MPEG1_1_0950 = 12,
		ASPECT_MPEG1_1_1575 = 13,
		ASPECT_MPEG1_1_2015 = 14,

		ASPECT_MPEG2_1x1 = 1,
		ASPECT_MPEG2_4x3 = 2,
		ASPECT_MPEG2_16x9 = 3,
		ASPECT_MPEG2_2_211x1 = 4		
	};

	enum QScaleType_t
	{
		NON_LINEAR = 1,
		LINEAR = 0
	};

	enum IntraVLC_t
	{
		INTRA_VLC_TABLE1 = 1,
		INTRA_VLC_TABLE2 = 0
	};

	enum ScanningType_t
	{
		ALTERNATE_SCAN,
		ZIG_ZAG_SCAN
	};

	enum CC_FORMAT_t
	{
		CC_FORMAT_NONE = 0,
		CC_FORMAT_ATSC = 1,
		CC_FORMAT_SCTE = 2,
		CC_FORMAT_CCUBE = 3,
		CC_FORMAT_PASSTHROUGH = 4
	} ;

	enum TwoPassMode_t
	{
		TWO_PASS_MODE_OFF = 0,
		TWO_PASS_MODE_ANALYZE = 1,
		TWO_PASS_MODE_ENCODING = 2,
	} ;

	enum MotionEstimation_t
	{
		OPTIMAL_ME = 17
	};

	enum RateControlMode_t
	{
		VBR = 0,
		CBR = 1,
		CQT = 2,
    CQT_Strict = 3
	};

    enum ColorSpace_t
    {
        ColorSpace_NA      = -1,
        ColorSpace_YUV_420 = 0,
        ColorSpace_YUV_422 = 1,
        ColorSpace_YUV_444 = 2,
        ColorSpace_RGB_24  = 3,
        ColorSpace_RGB_32  = 4
    };
};

#endif // __EM2VE_HEADER__