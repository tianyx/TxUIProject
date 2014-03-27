//
//  Copyright (C) 2006 MainConcept AG
//  All rights are reserved.  Reproduction in whole or in part is prohibited
//  without the written consent of the copyright owner.
//
//  MainConcept AG reserves the right to make changes to this software program
//  without notice at any time. MainConcept AG makes no warranty, expressed, implied
//  or statutory, including but not limited to any implied warranty of
//  merchantability of fitness for any particular purpose.
//  MainConcept AG does not represent or warrant that the programs furnished
//  hereunder are free of infringement of any third-party patents, copyright or
//  trademark.
//  In no event shall MainConcept AG be liable for any incidental, punitive, or
//  consequential damages of any kind whatsoever arising from the use of these programs.
//
//
//  Purpose: The definition of the MainConcept Mpeg-2 Muxer filter's CLSIDs of its parameters
//

#ifndef __MPEG2MUX_FILTER_PROPID__
#define __MPEG2MUX_FILTER_PROPID__


//Properties GUIDs


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// MUXER object.
//
// Muxer operations
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			GUID             Value Type		Range		Default		Description
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





static const GUID ELMux_Program =
{0xbeb95059, 0xef42, 0x4599, {0x8b, 0x5e, 0x54, 0x3c, 0x31, 0xc, 0xc1, 0x5f}};

static const GUID ELMux_Stream =
{0xde798a3e, 0x5e4d, 0x484f, {0xaa, 0x3, 0x1d, 0xed, 0x84, 0x75, 0x6d, 0xdb}};


//
// M72_GetNextProgram		VT_UNKNOWN		[0, IUnk]		0		Retreve next program. Value = 0 return the first program.
// {197745C4-A403-44de-89C3-969B127901F3}
static const GUID ElMux_GetNextProgram =
{0x197745c4, 0xa403, 0x44de, {0x89, 0xc3, 0x96, 0x9b, 0x12, 0x79, 0x1, 0xf3}};

// ElMux_CreateProgram		VT_UNKNOWN			-			0		Create program.
// {1C10A3F8-98AA-45a7-8332-3E82640C64AA}
static const GUID ElMux_CreateProgram = 
{0x1c10a3f8, 0x98aa, 0x45a7, {0x83, 0x32, 0x3e, 0x82, 0x64, 0xc, 0x64, 0xaa}};

// ElMux_DestroyProgram		VT_UNKNOWN			-            0		Destroy program.
// {06250D30-F7D6-4f5c-A4B4-36170F3FE552}
static const GUID ElMux_DestroyProgram = 
{0x6250d30, 0xf7d6, 0x4f5c, {0xa4, 0xb4, 0x36, 0x17, 0xf, 0x3f, 0xe5, 0x52}};



// Muxer properties
//
//			GUID             Value Type		Range		Default		Description
//------------------------------------------------------------------------------------------------------
//

// ElMux_ResultStreamType		VT_I4		[1, 2]         1		1=Program stream, 2=Transport stream output
// {6E7564E3-05FA-417a-AA8E-50E474537EC3}
static const GUID ElMux_OutputStreamType = 
{0x6e7564e3, 0x5fa, 0x417a, {0xaa, 0x8e, 0x50, 0xe4, 0x74, 0x53, 0x7e, 0xc3}};

// ElMux_ReqiuredBitrate	VT_I4			[0 - 100M]		0              Destination  TS bit rate value, bits/sec. If == 0 then bit rate estimated automatically based on stream rates
// {94E294E0-8B4C-40df-B429-B43B3F2BE4E7}
static const GUID ElMux_ReqiuredBitrate = 
{0x94e294e0, 0x8b4c, 0x40df, {0xb4, 0x29, 0xb4, 0x3b, 0x3f, 0x2b, 0xe4, 0xe7}};

// ElMux_PCR_Interval	VT_I4				[10 - 100 ms]	50              Time interval between two consecutive PCR stamps. For TS only
// {4F6F3CCD-989A-4318-B9F7-FC605C7EDFED}
static const GUID ElMux_PCR_Interval = 
{0x4f6f3ccd, 0x989a, 0x4318, {0xb9, 0xf7, 0xfc, 0x60, 0x5c, 0x7e, 0xdf, 0xed}};

// ElMux_Compatibility			VT_I4		[0,1,2]       0			Compatibility: 0=None, 1=DVD, 2=SVCD
// {6165514A-BF3D-40b3-BB2C-D97BA344850A}
static const GUID ElMux_Compatibility_PS = 
{0x6165514a, 0xbf3d, 0x40b3, {0xbb, 0x2c, 0xd9, 0x7b, 0xa3, 0x44, 0x85, 0xa}};

// ElMux_TransportStreamID		VT_I4		[0-0xFFFF]     0		16-bit field that serves as a label to identify the Transport Stream among other streams in a network
// {6C376C4D-D6B9-40ea-AD1F-A70C7948A4E9}
static const GUID ElMux_TransportStreamID = 
{0x6c376c4d, 0xd6b9, 0x40ea, {0xad, 0x1f, 0xa7, 0xc, 0x79, 0x48, 0xa4, 0xe9}};

// ElMux_OutputBufSize			VT_I4		[2K-500K]      100000	The size of output buffer, bytes
// {6BB968B3-7E26-44fe-ADB2-D0C81C96E304}
static const GUID ElMux_OutputBufSize = 
{0x6bb968b3, 0x7e26, 0x44fe, {0xad, 0xb2, 0xd0, 0xc8, 0x1c, 0x96, 0xe3, 0x4}};

// ElMux_AutoCreatePins			VT_I4		[0,1]			1		Flag specifies will filter create pins automatically or by user request only 
// {EFA4CA3D-E626-47f4-A045-4F717175962E}
static const GUID ElMux_AutoCreatePins = 
{0xefa4ca3d, 0xe626, 0x47f4, {0xa0, 0x45, 0x4f, 0x71, 0x71, 0x75, 0x96, 0x2e}};

// ElMux_TS_Padding				VT_I4		[0,1]			0		Flag specifies will filter pads TS with packets of stream with PID 0xFF  or not. 
// {218C3E56-54FC-43fe-87AC-39D3EE2AB3BB}
static const GUID ElMux_TS_Padding = 
{0x218c3e56, 0x54fc, 0x43fe, {0x87, 0xac, 0x39, 0xd3, 0xee, 0x2a, 0xb3, 0xbb}};

// ElMux_TS_Outpack_Size		VT_INT		[1,100]				5		Buffer size at output pin, in TS packets (188 bytes)
// {2394EEE2-51FD-4169-96D1-74D9E45B3343}
static const GUID ElMux_TS_Outpack_Size =
{0x2394eee2, 0x51fd, 0x4169, {0x96, 0xd1, 0x74, 0xd9, 0xe4, 0x5b, 0x33, 0x43}};

// {B2B31D6C-F17D-4e0f-8EFA-13CD324A8A74}
static const GUID ElMux_TimingMode = 
{ 0xb2b31d6c, 0xf17d, 0x4e0f, { 0x8e, 0xfa, 0x13, 0xcd, 0x32, 0x4a, 0x8a, 0x74 } };


// ElMux_OutputBitrate           VT_I4		-				0		For statistic only. Value of TS bit rate, bits/sec
// {762F0204-DB00-44e4-9928-EF7828DB9E5F}
static const GUID ElMux_OutputBitrate =
{0x762f0204, 0xdb00, 0x44e4, {0x99, 0x28, 0xef, 0x78, 0x28, 0xdb, 0x9e, 0x5f}};

// ElMux_MuxTime				VT_I8		-				-		Current system stream time 
// {A32C0F2B-37FE-4417-B469-349C3C271E88}
static const GUID ElMux_MuxTime =
{0xa32c0f2b, 0x37fe, 0x4417, {0xb4, 0x69, 0x34, 0x9c, 0x3c, 0x27, 0x1e, 0x88}};

// ElMux_Overhead				VT_I4		[0-100%]		0		Overhead given by muxer in percents to payload data
// {A8F9850E-68D5-460c-B929-FA7499BF28E8}
static const GUID ElMux_Overhead =
{0xa8f9850e, 0x68d5, 0x460c, {0xb9, 0x29, 0xfa, 0x74, 0x99, 0xbf, 0x28, 0xe8}};

// ElMux_Padding				VT_I8		-				0		Padding given by muxer in percents to payload data
// {BF817BCE-199F-47d9-8641-B95D263381F0}
static const GUID ElMux_Padding =
{0xbf817bce, 0x199f, 0x47d9, {0x86, 0x41, 0xb9, 0x5d, 0x26, 0x33, 0x81, 0xf0}};

// {002D6EC9-8E71-4b4c-AA75-297C94EDF9F6}
static const GUID ElMux_InitWaitInterval = 
{ 0x2d6ec9, 0x8e71, 0x4b4c, { 0xaa, 0x75, 0x29, 0x7c, 0x94, 0xed, 0xf9, 0xf6 } };

// {dad58b2f-1ff4-4363-9fbd-c670ea1b188b}
//ELMux_AllowMissingStreams			VT_I4	[0, 1]			0		Allows/Permits missing input streams 	
static const GUID ELMux_AllowMissingStreams = 
{ 0xdad58b2f, 0x1ff4, 0x4363, {0x9f, 0xbd, 0xc6, 0x70, 0xea, 0x1b, 0x18, 0x8b } };

// {8727A467-7C6C-4bf9-91AE-FB8DE64BCAE9}
static const GUID ElMux_192b_TS_Packets = 
{ 0x8727a467, 0x7c6c, 0x4bf9, { 0x91, 0xae, 0xfb, 0x8d, 0xe6, 0x4b, 0xca, 0xe9 } };

static const GUID ElMux_PES_LengthForVideoInTS = 
{ 0x17be9772, 0x1161, 0x4cae, {0x87, 0x44, 0xf2, 0x09, 0xcb, 0xda, 0x4a, 0x15} };

static const GUID ElMux_Set_PTS_Only = 
{ 0xb7ad0215, 0x0677, 0x4043, {0xab, 0x7b, 0x3b, 0xd1, 0x11, 0xdd, 0x02, 0x3c} };

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PROGRAM object.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			GUID             Value Type		Range			Default		Description
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Program operation
//
// ElMux_GetNextStream		VT_UNKNOWN		[0, IUnk]		0			For retrieving next stream while fetching streams of program. Value = 0 return the first stream.
// {8DB05E7F-3AE4-4738-ADB2-134175DD238C}
static const GUID ElMux_GetNextStream =
{0x8db05e7f, 0x3ae4, 0x4738, {0xad, 0xb2, 0x13, 0x41, 0x75, 0xdd, 0x23, 0x8c}};

// ElMux_CreateStream		VT_UNKNOWN		[0, IUnk]		0			Create stream.
// {C91DD135-569A-4f48-802D-B1E0BE15E1D5}
static const GUID ElMux_CreateStream = 
{0xc91dd135, 0x569a, 0x4f48, {0x80, 0x2d, 0xb1, 0xe0, 0xbe, 0x15, 0xe1, 0xd5}};

// ElMux_DestroyStream		VT_UNKNOWN		[0, IUnk]		0			Destroy stream.
// {9F420C44-CF0C-423e-AA32-BFB081F57422}
static const GUID ElMux_DestroyStream = 
{0x9f420c44, 0xcf0c, 0x423e, {0xaa, 0x32, 0xbf, 0xb0, 0x81, 0xf5, 0x74, 0x22}};

// Program Properties
//
// ElMux_ProgramNumber		VT_I4			-              0			0 - auto select
// {BFB26B95-AC4A-4d48-8F2F-7DF8D03C673F}
static const GUID ElMux_ProgramNumber =
{0xbfb26b95, 0xac4a, 0x4d48, {0x8f, 0x2f, 0x7d, 0xf8, 0xd0, 0x3c, 0x67, 0x3f}};

// ElMux_ProgramID			VT_I4			[0,0x1FFF]		0			0 - auto select
// {F21C447B-3870-4ff1-B97E-6154067FCCE8}
static const GUID ElMux_ProgramID =
{0xf21c447b, 0x3870, 0x4ff1, {0xb9, 0x7e, 0x61, 0x54, 0x6, 0x7f, 0xcc, 0xe8}};

// ElMux_ProgramBitrate		VT_I4			-				0			For statistic only. Value of Program bit rate, bits/sec
// {B9067BD7-735F-4aad-A643-36911E9ADE11}
static const GUID ElMux_ProgramBitrate = 
{0xb9067bd7, 0x735f, 0x4aad, {0xa6, 0x43, 0x36, 0x91, 0x1e, 0x9a, 0xde, 0x11}};

// ElMux_PCR_PID		VT_I4			-				0				PCR PID for this program		
// {c54afa11-3172-42a6-b92f-ceac92ab3d12}
static const GUID ElMux_PCR_PID =
{0xc54afa11, 0x3172, 0x42a6, {0xb9, 0x2f, 0xce, 0xac, 0x92, 0xab, 0x3d, 0x12}};

// {6B914A19-555D-42c2-8638-3EF76D34436A}
static const GUID ElMux_UserDef_PCR_PID = 
{ 0x6b914a19, 0x555d, 0x42c2, {0x86, 0x38, 0x3e, 0xf7, 0x6d, 0x34, 0x43, 0x6a}};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// STREAM object.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			GUID             Value Type		Range			Default		Description
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stream operation

// ElMux_MoveStreamTo		VT_UNKNOWN		[0, IUnk]		0			Move stream to another program
// {F3A8774E-58F3-44e5-940D-842155353BAB}
static const GUID ElMux_MoveStreamTo =
{0xf3a8774e, 0x58f3, 0x44e5, {0x94, 0xd, 0x84, 0x21, 0x55, 0x35, 0x3b, 0xab}};

// ElMux_StreamType			VT_I4			[0..15]         0			Input stream type.
// {1811ABAA-F568-4016-A728-0A9B8E7799AE}
static const GUID ElMux_StreamType = 
{0x1811abaa, 0xf568, 0x4016, {0xa7, 0x28, 0xa, 0x9b, 0x8e, 0x77, 0x99, 0xae}};

// ElMux_PESPacketSize		VT_I4			[0, 0xFFFF]     0			Size of PES packet in stream. 0 - variable length.
// {769E9811-5D67-472a-8846-95B355A606ED}
static const GUID ElMux_PESPacketSize =
{0x769e9811, 0x5d67, 0x472a, {0x88, 0x46, 0x95, 0xb3, 0x55, 0xa6, 0x6, 0xed}};

// ElMux_PaddingOn			VT_I4			[0,1]          0			Padding input samples if its size is less then "PES Packet Size". Actual only if "PES Packet Size" > 0
// {A8AC9943-DE6F-4c8a-881C-B7BB52353102}
static const GUID ElMux_PaddingOn = 
{0xa8ac9943, 0xde6f, 0x4c8a, {0x88, 0x1c, 0xb7, 0xbb, 0x52, 0x35, 0x31, 0x2}};
	
// ElMux_CarriagePCR		VT_I4			[0,1]			1			Flag specifies that given stream PID is PCR_PID for Transport Stream
// {84C4E4C2-88F2-4c19-82AF-3D94F9FE5B2A}
static const GUID ElMux_CarriagePCR =  
{0x84c4e4c2, 0x88f2, 0x4c19, {0x82, 0xaf, 0x3d, 0x94, 0xf9, 0xfe, 0x5b, 0x2a}};

// ElMux_InputBufSize		VT_I4			[1MB-45MB]     500000		The size of input buffer in bytes. Default values sets automatically depending on media type.
// {A4FE0D19-6AB8-4367-8124-07D45B1384DA}
static const GUID ElMux_InputBufSize = 
{0xa4fe0d19, 0x6ab8, 0x4367, {0x81, 0x24, 0x7, 0xd4, 0x5b, 0x13, 0x84, 0xda}};

// ElMux_InitialShift		VT_I4			-				0			The specified value will be added to all PTS / DTS in chosen stream.
// {3A34CBF2-E6C4-4c84-8541-70C6961C87AD}
static const GUID ElMux_InitialShift = 
{0x3a34cbf2, 0xe6c4, 0x4c84, {0x85, 0x41, 0x70, 0xc6, 0x96, 0x1c, 0x87, 0xad}};

// ElMux_StreamID			VT_I4			-				0			0 - auto select
// {4F382B24-2E36-43d7-AFF3-04047B0C2F87}
static const GUID ElMux_StreamID = 
{0x4f382b24, 0x2e36, 0x43d7, {0xaf, 0xf3, 0x4, 0x4, 0x7b, 0xc, 0x2f, 0x87}};

// ElMux_StreamPESID		VT_I4			[0,0xFF]        0			0 - auto select
// {474C78F4-2DC9-4ce2-9CA9-161E0C93B988}
static const GUID ElMux_StreamPESID = 
{0x474c78f4, 0x2dc9, 0x4ce2, {0x9c, 0xa9, 0x16, 0x1e, 0xc, 0x93, 0xb9, 0x88}};

// ElMux_SubID_num			VT_I4			-				-			For Program Stream, types PCM & AC3 only. Stream sub id. See DVD specs for details about default value and range of values
// {5CAED249-7594-410e-8E5C-2FA5D973BE6C}
static const GUID ElMux_SubID_num =  
{0x5caed249, 0x7594, 0x410e, {0x8e, 0x5c, 0x2f, 0xa5, 0xd9, 0x73, 0xbe, 0x6c}};

// ElMux_StreamBitrateAnnounced		VT_I4		-			0			Announced bit rate for stream under consideration
// {D2BF1AF3-2AA6-47a9-B983-B528E042AA17}
static const GUID ElMux_StreamBitrateAnnounced = 
{0xd2bf1af3, 0x2aa6, 0x47a9, {0xb9, 0x83, 0xb5, 0x28, 0xe0, 0x42, 0xaa, 0x17}};

// ElMux_StreamBitrateReal			VT_I4		-			0			For statistic only. Real bit rate for stream under consideration
// {D59D182B-A954-4759-81E7-7311CC58271B}
static const GUID ElMux_StreamBitrateReal = 
{0xd59d182b, 0xa954, 0x4759, {0x81, 0xe7, 0x73, 0x11, 0xcc, 0x58, 0x27, 0x1b}};

// ElMux_StreamBitrateReal			VT_I4		-			0			Correct bit rate for stream specified by user. Use it if Announced bitrate is incorrect
// {2D7E6A07-0C99-47ca-99E3-E83FCA7CA058}
static const GUID ElMux_StreamBitrateCorrect = 
{0x2d7e6a07, 0xc99, 0x47ca, {0x99, 0xe3, 0xe8, 0x3f, 0xca, 0x7c, 0xa0, 0x58}};

// ElMux_EB_SizeCorrect				VT_I4		-			0			For video only. Elementary Buffer size specified by user. 0 - value will be calculated automatically depending on header information
// {95AC7C07-8777-4294-8F7C-0E59F84DE757}
static const GUID ElMux_EB_SizeCorrect =
{0x95ac7c07, 0x8777, 0x4294, {0x8f, 0x7c, 0xe, 0x59, 0xf8, 0x4d, 0xe7, 0x57}};

// ElMux_FrameRateAnnounced			VT_I4		-			0			Announced frame rate
// {2B50384B-03C0-4576-B28A-85B0F96A9DE2}
static const GUID ElMux_FrameRateAnnounced = 
{0x2b50384b, 0x3c0, 0x4576, {0xb2, 0x8a, 0x85, 0xb0, 0xf9, 0x6a, 0x9d, 0xe2}};

// ElMux_StreamTime					VT_I8		-			0			For statistic only. PTS Value of last delivered access unit
// {E808C80B-4298-4dea-B64C-4884BBDFFD36}
static const GUID ElMux_StreamTime = 
{0xe808c80b, 0x4298, 0x4dea, {0xb6, 0x4c, 0x48, 0x84, 0xbb, 0xdf, 0xfd, 0x36}};

// ElMux_AudioFrequency				VT_I4		-			0			Anounced audio frequency (Hz)
// {9A49DB34-8EDB-4358-9C94-CCE447954EF8}
static const GUID ElMux_AudioFrequency =
{0x9a49db34, 0x8edb, 0x4358, {0x9c, 0x94, 0xcc, 0xe4, 0x47, 0x95, 0x4e, 0xf8}};

//ElMux_InternalMediaType			VT_PTR		-			0			Ptr to internal CmediaType
// {05D41460-9666-4ebc-9181-FCC4DC9B90D2}
static const GUID ElMux_InternalMediaType = 
{ 0x5d41460, 0x9666, 0x4ebc, { 0x91, 0x81, 0xfc, 0xc4, 0xdc, 0x9b, 0x90, 0xd2 } };

//ELMux_ES_info_length				VT_I4		-			0			For HDMV. Length of ES_info data in PMT 2nd loop 
// {BA0B67A0-6D9A-4af8-ACB3-3A1847681FAD}
static const GUID ELMux_ES_InfoLength = 
{ 0xba0b67a0, 0x6d9a, 0x4af8, { 0xac, 0xb3, 0x3a, 0x18, 0x47, 0x68, 0x1f, 0xad } };

//ELMux_ES_Info					VT_PTR		-			0				For HDMV. Pointer to ES_info data in PMT 2nd loop 
// {C8F37A44-1131-4d21-AEDC-125C9E834B47}
static const GUID ELMux_ES_Info = 
{ 0xc8f37a44, 0x1131, 0x4d21, { 0xae, 0xdc, 0x12, 0x5c, 0x9e, 0x83, 0x4b, 0x47 } };


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ElMuxer
{
	typedef enum OutputStreamType
	{
	RESULT_STREAM_PROGRAM   = 1,
	RESULT_STREAM_TRANSPORT = 2,
	RESULT_STREAM_HDMV_TS = 3
	} OutputStreamType;

	typedef enum PS_Compatibility
	{
	PS_CompatNone  = 0,
	PS_CompatDVD   = 1,
	PS_CompatSVCD  = 2
	} PS_Compatibility;

	typedef enum DMB_Compatibility
	{
		TS_CompatNone  = 0,
		TS_CompatDMB   = 1
	} DMB_Compatibility;

	typedef enum PTSSource
	{
	TimeStampBased,
	BitRateBased,
	FrameRateBased
	} PTSSource;

	typedef enum PinType
	{
	PT_Unknown  = 0,
	PT_MPEG2V   = 1,
	PT_MPEG2A   = 2,
	PT_AC3      = 3,
	//   PT_DIV3     = 4,
	PT_DIV4     = 5,
	PT_DIV5     = 6,
	PT_MPEG1A   = 7,
	PT_MPEG1V   = 8,
	PT_MPEG2PS  = 9,
	PT_H264     = 10,
	PT_AAC      = 11,
	PT_VSSH     = 12,    
	PT_PCM      = 13,    
	PT_MPEG4    = 14,
	PT_DVDSUB   = 15,
	PT_VC1 = 17,
	PT_IG_PES = 18,
	PT_PCR_ONLY = 19,
	PT_PES_MPEG2A = 20,	
	PT_PES_AC3 = 21, 
	PT_PES_MPEG2V = 22, 
	PT_PRIVATE = 55,
	PT_DVDSUB_PES = 32,
	PT_DVDSUB_PS = 33
	} PinType;

	typedef enum FrameRate
	{
	frUnknown = 0,
	fr2397 = 1, // 23.97
	fr24 = 2,   // 24
	fr25 = 3,   // 25
	fr2997 = 4, // 29.97
	fr30 = 5,   // 30
	fr50 = 6,   // 50
	fr5994 = 7, // 59.94
	fr60 = 8   // 60
	} FrameRate;

	typedef enum TimingMode
	{
		eMODE_CAPTURE = 0,
		eMODE_RECONSTRUCTION = 1
	} TimingMode_t;
}
#endif // __MPEG2MUX_FILTER_PROPID__

