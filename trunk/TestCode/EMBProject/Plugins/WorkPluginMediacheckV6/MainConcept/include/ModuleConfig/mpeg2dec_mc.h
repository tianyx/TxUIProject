//////////////////////////////////////////////////////////////////////////
//
//	created:	2005/04/12
//	file name:	mpeg2dec_mc.h
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
//  Purpose: The definition of the MainConcept MPEG-2 Video Decoder parameters CLSIDs
//

#ifndef __EM2VD_FILTER_PROPID__
#define __EM2VD_FILTER_PROPID__
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	GUID								Type		Available range		Default Val		Note
// EM2VD_HardwareAcceleration			VT_UI4		[0, 1]				[0]				Switching between software and hardware decode modes
// EM2VD_IDCTPrecision					VT_UI4		[0, 1]				[0]				Selecting precision of inverse DCT procedure
// EM2VD_PostProcess					VT_UI4		[0, 1]				[0]				Enable/disable deblock and dering filtering (only software mode)
// EM2VD_Brightness						VT_UI4		[0, 255]			[128]			Brightness level
// EM2VD_Contrast						VT_UI4		[0, 255]			[128]			Contrast level
// EM2VD_Resolution						VT_UI4		[0, 3]				[0]				Output resolution (only software decode mode)
// EM2VD_Deinterlace					VT_UI4		[0, 3]				[0]				Selecting output deinterlace mode
// EM2VD_DeinterlaceCondition			VT_UI4		[0, 1]				[0]				Condition of execute deinterlace
// EM2VD_HQUpsample						VT_UI4		[0, 1]				[0]				Enable/disable chroma vertical upsample (only software decode mode)
// EM2VD_DoubleRate						VT_UI4		[0, 1]				[0]				Enable/disable output double rate (available only if software decode mode and video is interlaced)
// EM2VD_Quality						VT_UI4		[0, 3]				[0]				Selecting skip mode(I,IP,IBP) or obey quality messages
// EM2VD_OSD							VT_UI4		[0, 1]				[0]				Enable/disable on screen display info (only software decode mode)
// EM2VD_CCubeDecodeOrder				VT_UI4		[0, 1]				[0]				Selecting decode order for closed caption format of Cube
// EM2VD_FieldsReordering				VT_UI4		[0, 2]				[0]				Sets the mode of fields reordering
// EM2VD_FieldsReorderingCondition		VT_UI4		[0, 2]				[0]				Sets the condition of fields reordering
// EM2VD_FormatVideoInfo				VT_UI4		[0, 2]				[1]				Specified output format type of VideoInfo struct - VideoInfo,VideoInfo2 or both (subject to software media types only)
// EM2VD_CapturePicture					VT_BYREF	[]					[0]				Get last decoded picture. Call GetValue(...) and send empty VARIANT argument. Reinterpreted pbVal to EM2VD_CapturePictureInfo
//																						struct pointer. Free allocated memory throw CoTaskMemFree on the end.
// EM2VD_FramesDecoded					VT_UI4		[]					[0]				Count of decoded frames (read only)
// EM2VD_FramesSkipped					VT_UI4		[]					[0]				Count of skipped frames (read only)
// EM2VD_MultiThread					VT_UI4		[0, 1]				[1]				Use multi-thread if available (available on multi-processors systems, only software decode mode, only MPEG-2 streams)
// EM2VD_ForceSubpicture				VT_UI4		[0, 1]				[1]				Forced subpicture show
// EM2VD_CaptureUserData				VT_BYREF	[]					[0]				Get user data.
// EM2VD_MediaTimeStart					VT_I8		[]      							Media time of current frame (it may be a byte offset of current frame)
// EM2VD_MediaTimeStop					VT_I8		[]	
// EM2VD_ResetStatistics				VT_UI4		[0, 1]				[0]				Reset decoder statistics (decoded and skipped frames counter).
// EM2VD_ErrorConcealment				VT_UI4		[0, 1]				[1]				Don't show frames with errors.
// EM2VD_MediaTimeSource				VT_UI4		[0, 1]				[0]				Source(input/GOP time code) of media times.
// EM2VD_SetMediaTime					VT_UI4		[0, 1]				[0]				Set media time for output media samples.
// EM2VD_Synchronization                VT_UI4      [0, 3]              [0]             Sets synchronization mode
// EM2VD_CCProcessingCondition			VT_UI4		[0, 1]				[0]				Sets the condition of CC byte pairs processing.
// EM2VD_SequenceDisplayExtension		VT_BYREF	[]					[0]				Returns the sequence display extension data structure
// EM2VD_Cropping						VT_UI4		[0, 1]				[0]				Enable/disable output image cropping.
// EM2VD_CropTop						VT_UI4		[]					[0]				Sets top value of cropping rectangele.
// EM2VD_CropLeft						VT_UI4		[]					[0]				Sets left value of cropping rectangele.
// EM2VD_CropBottom						VT_UI4		[]					[0]				Sets bottom value of cropping rectangele.
// EM2VD_CropRight						VT_UI4		[]					[0]				Sets right value of cropping rectangele.
// EM2VD_Adding							VT_UI4		[0, 1]				[0]				Enable/disable adding blank lines or bands to output image.
// EM2VD_AddTop							VT_UI4		[]					[0]				Sets top value of adding rectangele.
// EM2VD_AddLeft						VT_UI4		[]					[0]				Sets left value of adding rectangele.
// EM2VD_AddBottom						VT_UI4		[]					[0]				Sets bottom value of adding rectangele.
// EM2VD_AddRight						VT_UI4		[]					[0]				Sets right value of adding rectangele.
// EM2VD_DimensionsAdjusting			VT_UI4		[0, 3]				[0]				Enable/disable output image dimensions adjusting.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace EM2VD{
	
	enum	State{
		State_Off = 0,
		State_On =1
	};
	
	enum DeinterlaceMode{
		Deinterlace_Weave = 0,					// No deinterlace
		Deinterlace_VerticalFilter = 1,			// Interpolation with using info from both fields
		Deinterlace_FieldInterpolation = 2,		// Stretch one of fields
		Deinterlace_VMR = 3,
		Deinterlace_Auto = 4
	};

	enum DeinterlaceCondition
	{
		DeinterlaceCondition_Always = 0,		// Execute deinterlace regardless of picture structute
		DeinterlaceCondition_Interlace = 1,		// Execute deinterlace only progressive flag is "false"
		DeinterlaceCondition_Progressive = 2	// Execute deinterlace only progressive flag is "true"
	};

	enum QualityMode{
		Quality_ObeyQualityMessages = 0,
		Quality_I  = 1,
		Quality_IP = 2,
		Quality_IPB = 3
	};

	enum ResolutionMode{
		Resolution_Full = 0,
		Resolution_HalfHorizontal = 1,
		Resolution_HalfVertical = 2,
		Resolution_Quarter = 3
	};

	enum IDCTPrecisionMode{
		IDCTPrecision_Integer = 0,
		IDCTPrecision_Float  = 1
	};

	enum DecodeOrderMode{
		DecodeOrder_Stream = 0,
		DecodeOrder_Display  = 1
	};

	enum FieldReorderingMode{
		FieldReordering_Off = 0,
		FieldReordering_FlagsInverting = 1,
		FieldReordering_FieldsInverting = 2,
		FieldReordering_Auto = 3
	};

	enum FieldReorderingConditionMode{
		FieldReorderingCondition_Always = 0,
		FieldReorderingCondition_TopFirst_True = 1,
		FieldReorderingCondition_TopFirst_False = 2,
	};

	enum FormatVideoInfo{
		FormatVideoInfo_1 = 0,
		FormatVideoInfo_2  = 1,
		FormatVideoInfo_Both  = 2
	};

	enum ErrorConcealmentMode{
		ErrorConcealment_Off = 0,
		ErrorConcealment_NotShowFramesWithErrors = 1,
		ErrorConcealment_Temporal  = 2,
		ErrorConcealment_Spatial  = 3,
		ErrorConcealment_Smart  = 4
	};


	enum DimensionsAdjustingMode{
		DimensionsAdjusting_Off = 0,
		DimensionsAdjusting_720x486 = 1,
		DimensionsAdjusting_1280x720  = 2,
		DimensionsAdjusting_1920x1080  = 3
	};

	typedef struct _EM2VD_CapturePictureInfo{
		VOID * pBuffer;
		SIZE_T szBuffer;
	}EM2VD_CapturePictureInfo;

	typedef struct _EM2VD_BufferInfo{
		VOID * pBuffer;
		SIZE_T szBuffer;
	}EM2VD_BufferInfo;

	enum MediaTimeSource {
		InputMediaTime = 0,
		GOPTimeCode = 1
	};

	enum SynchronizationMode {
		SynchronizationMode_PTS = 0,
		SynchronizationMode_IgnorePTS_NotRef,
		SynchronizationMode_IgnorePTS_All,
		SynchronizationMode_DirectTimestamps
	};

	enum CCProcessingConditionMode {
		FirstBytePairOnly = 0,
		FirstValidBytePair
	};

	typedef struct _EM2VD_SequenceDisplayExt {
		UINT nDisplayHorizontalSize;
		UINT nDisplayVerticalSize;
		UINT nHorizontalSizeValue;
		UINT nVerticalSizeValue;
		BOOL bSequenceDisplayExtPresent;
	} EM2VD_SequenceDisplayExt;
};

// {BA6DDF74-5F8A-4bdc-88BB-A2563314BC3E}
static const GUID EM2VD_HardwareAcceleration = 
{0xba6ddf74, 0x5f8a, 0x4bdc, {0x88, 0xbb, 0xa2, 0x56, 0x33, 0x14, 0xbc, 0x3e }};

// {9CF1A333-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_IDCTPrecision = 
{0x9cf1a333, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9CF1A334-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_PostProcess = 
{0x9cf1a334, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9CF1A336-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_Brightness = 
{0x9cf1a336, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9CF1A331-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_Resolution = 
{0x9cf1a331, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9CF1A332-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_Deinterlace = 
{0x9cf1a332, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {37A10D50-E5FC-45b4-A403-3305D6DCDDAB}
static const GUID EM2VD_DeinterlaceCondition = 
{ 0x37a10d50, 0xe5fc, 0x45b4, { 0xa4, 0x3, 0x33, 0x5, 0xd6, 0xdc, 0xdd, 0xab } };

// {9CF1A340-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_HQUpsample = 
{0x9cf1a340, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9CF1A339-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_DoubleRate = 
{0x9cf1a339, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9CF1A330-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_Quality = 
{0x9cf1a330, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {F5C51906-ED89-4c6e-9C37-A5CCB34F5389}
static const GUID EM2VD_OSD = 
{0xf5c51906, 0xed89, 0x4c6e, {0x9c, 0x37, 0xa5, 0xcc, 0xb3, 0x4f, 0x53, 0x89 }};

// {9CF1A338-E72B-4a6d-BBE8-199595944546}
static const GUID EM2VD_CCubeDecodeOrder = 
{0x9cf1a338, 0xe72b, 0x4a6d, {0xbb, 0xe8, 0x19, 0x95, 0x95, 0x94, 0x45, 0x46 }};

// {9951682E-0F78-4924-92A4-BD7BFBA30063}
static const GUID EM2VD_FieldsReordering = 
{0x9951682e, 0xf78, 0x4924, {0x92, 0xa4, 0xbd, 0x7b, 0xfb, 0xa3, 0x0, 0x63 }};

// {8AE4A3D8-240B-427d-B845-5474965CBB48}
static const GUID EM2VD_FieldsReorderingCondition = 
{ 0x8ae4a3d8, 0x240b, 0x427d, { 0xb8, 0x45, 0x54, 0x74, 0x96, 0x5c, 0xbb, 0x48 } };

// {110272F6-DA17-4162-B6BA-866CC5CB6889}
static const GUID EM2VD_FormatVideoInfo = 
{0x110272f6, 0xda17, 0x4162, {0xb6, 0xba, 0x86, 0x6c, 0xc5, 0xcb, 0x68, 0x89 }};

// {6118A160-0FF0-43c8-94E4-345AC9E9F362}
static const GUID EM2VD_CapturePicture = 
{0x6118a160, 0xff0, 0x43c8, {0x94, 0xe4, 0x34, 0x5a, 0xc9, 0xe9, 0xf3, 0x62 }};

// {B587EAF4-337E-4e64-92A4-B9F634467A1E}
static const GUID EM2VD_CaptureUserData = 
{ 0xb587eaf4, 0x337e, 0x4e64, { 0x92, 0xa4, 0xb9, 0xf6, 0x34, 0x46, 0x7a, 0x1e } };

// {575BA759-6F13-4a84-A126-005A5523D203}
static const GUID EM2VD_FramesDecoded = 
{0x575ba759, 0x6f13, 0x4a84, {0xa1, 0x26, 0x0, 0x5a, 0x55, 0x23, 0xd2, 0x3 }};

// {592A9FAB-CBF8-4592-A0B2-21D0C79DACE4}
static const GUID EM2VD_FramesSkipped = 
{0x592a9fab, 0xcbf8, 0x4592, {0xa0, 0xb2, 0x21, 0xd0, 0xc7, 0x9d, 0xac, 0xe4 }};

// {0612C1C6-DEF7-4d01-A0DA-90F426F9B312}
static const GUID EM2VD_MultiThread = 
{0x612c1c6, 0xdef7, 0x4d01, {0xa0, 0xda, 0x90, 0xf4, 0x26, 0xf9, 0xb3, 0x12 }};

// {BB8F00E9-655B-47c4-966A-A4B4BBF8D2D2}
static const GUID EM2VD_ErrorConcealment = 
{ 0xbb8f00e9, 0x655b, 0x47c4, { 0x96, 0x6a, 0xa4, 0xb4, 0xbb, 0xf8, 0xd2, 0xd2 } };


// {A4E0959C-4A56-4c41-BF86-51348264D74B}
static const GUID EM2VD_Cropping = 
{ 0xa4e0959c, 0x4a56, 0x4c41, { 0xbf, 0x86, 0x51, 0x34, 0x82, 0x64, 0xd7, 0x4b } };


// {CB9F3BF2-5B18-4102-B384-633EBCDBA490}
static const GUID EM2VD_CropTop = 
{ 0xcb9f3bf2, 0x5b18, 0x4102, { 0xb3, 0x84, 0x63, 0x3e, 0xbc, 0xdb, 0xa4, 0x90 } };

// {F57B60B9-ED56-4cab-A203-86B771D70C81}
static const GUID EM2VD_CropLeft = 
{ 0xf57b60b9, 0xed56, 0x4cab, { 0xa2, 0x3, 0x86, 0xb7, 0x71, 0xd7, 0xc, 0x81 } };

// {CE898DED-D03C-4448-8937-E8599B46E45A}
static const GUID EM2VD_CropBottom = 
{ 0xce898ded, 0xd03c, 0x4448, { 0x89, 0x37, 0xe8, 0x59, 0x9b, 0x46, 0xe4, 0x5a } };

// {2A3DA148-F045-4e2a-BBA0-8B4E23A91E6F}
static const GUID EM2VD_CropRight = 
{ 0x2a3da148, 0xf045, 0x4e2a, { 0xbb, 0xa0, 0x8b, 0x4e, 0x23, 0xa9, 0x1e, 0x6f } };


// {E66B38D1-B7F9-4f55-83B7-16A9B854E541}
static const GUID EM2VD_Adding = 
{ 0xe66b38d1, 0xb7f9, 0x4f55, { 0x83, 0xb7, 0x16, 0xa9, 0xb8, 0x54, 0xe5, 0x41 } };

// {7E41D6D1-C084-413a-AAA0-E084B7E14F64}
static const GUID EM2VD_AddTop = 
{ 0x7e41d6d1, 0xc084, 0x413a, { 0xaa, 0xa0, 0xe0, 0x84, 0xb7, 0xe1, 0x4f, 0x64 } };

// {D008A768-6091-4d65-835B-D92D7CDBE267}
static const GUID EM2VD_AddLeft = 
{ 0xd008a768, 0x6091, 0x4d65, { 0x83, 0x5b, 0xd9, 0x2d, 0x7c, 0xdb, 0xe2, 0x67 } };

// {11B9A48E-FC36-4bdd-B4EC-B19E650774E7}
static const GUID EM2VD_AddBottom = 
{ 0x11b9a48e, 0xfc36, 0x4bdd, { 0xb4, 0xec, 0xb1, 0x9e, 0x65, 0x7, 0x74, 0xe7 } };

// {B6213545-D209-47e8-AB3D-8890828324DF}
static const GUID EM2VD_AddRight = 
{ 0xb6213545, 0xd209, 0x47e8, { 0xab, 0x3d, 0x88, 0x90, 0x82, 0x83, 0x24, 0xdf } };

// {78F02F8B-62D2-4229-A0FB-DDD7AB7A8274}
static const GUID EM2VD_DimensionsAdjusting = 
{ 0x78f02f8b, 0x62d2, 0x4229, { 0xa0, 0xfb, 0xdd, 0xd7, 0xab, 0x7a, 0x82, 0x74 } };

// {81138942-7534-4700-B520-BE53BAEEE4CC}
static const GUID EM2VD_ForceSubpicture = 
{ 0x81138942, 0x7534, 0x4700, { 0xb5, 0x20, 0xbe, 0x53, 0xba, 0xee, 0xe4, 0xcc } };

// {81A015E4-A01F-4745-9355-5731AD89BA5F}
static const GUID EM2VD_InternalData = 
{ 0x81a015e4, 0xa01f, 0x4745, { 0x93, 0x55, 0x57, 0x31, 0xad, 0x89, 0xba, 0x5f } };

// {3903A73E-6A89-4e09-8E9F-95E8A56F614D}
static const GUID EM2VD_MediaTimeStart = 
{ 0x3903a73e, 0x6a89, 0x4e09, { 0x8e, 0x9f, 0x95, 0xe8, 0xa5, 0x6f, 0x61, 0x4d } };

// {BCF5D243-B80E-400a-9B60-035A1D3E5C38}
static const GUID EM2VD_MediaTimeStop = 
{ 0xbcf5d243, 0xb80e, 0x400a, { 0x9b, 0x60, 0x3, 0x5a, 0x1d, 0x3e, 0x5c, 0x38 } };

// {164966A1-2BFD-4c74-A80E-E5769A219B9F}
static const GUID EM2VD_ResetStatistics = 
{ 0x164966a1, 0x2bfd, 0x4c74, { 0xa8, 0xe, 0xe5, 0x76, 0x9a, 0x21, 0x9b, 0x9f } };

// {086B9C83-FFED-4470-96D5-6441D5394181}
static const GUID EM2VD_MediaTimeSource = 
{ 0x86b9c83, 0xffed, 0x4470, { 0x96, 0xd5, 0x64, 0x41, 0xd5, 0x39, 0x41, 0x81 } };

// {77638653-613F-4afc-AE6C-5AB47C7F0A14}
static const GUID EM2VD_SetMediaTime = 
{ 0x77638653, 0x613f, 0x4afc, { 0xae, 0x6c, 0x5a, 0xb4, 0x7c, 0x7f, 0xa, 0x14 } };

// {618815E6-51DD-4c31-955F-49465D2E7DB2}
static const GUID EM2VD_SynchronizationMode = 
{ 0x618815e6, 0x51dd, 0x4c31, { 0x95, 0x5f, 0x49, 0x46, 0x5d, 0x2e, 0x7d, 0xb2 } };

// {C2DF10E6-D901-41f6-962C-A995999F2506}
static const GUID EM2VD_CCProcessingCondition = 
{ 0xc2df10e6, 0xd901, 0x41f6, { 0x96, 0x2c, 0xa9, 0x95, 0x99, 0x9f, 0x25, 0x6 } };

// {D689E5CE-135E-43bf-8905-39BD9DBE7DCD}
static const GUID EM2VD_SequenceDisplayExtension = 
{ 0xd689e5ce, 0x135e, 0x43bf, { 0x89, 0x5, 0x39, 0xbd, 0x9d, 0xbe, 0x7d, 0xcd } };

#endif //__EM2VD_FILTER_PROPID__

