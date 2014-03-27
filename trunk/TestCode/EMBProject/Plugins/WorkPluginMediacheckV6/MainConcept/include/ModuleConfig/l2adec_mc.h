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

#ifndef _MC_LAYER_II_AUDIO_DECODER_MODULECONFIG_
#define _MC_LAYER_II_AUDIO_DECODER_MODULECONFIG_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	GUID								Type	Available range		Default Val		Note
//
//  Input stream info:
//	EL2AD_ISI_Audio_Type					VT_I4		[0;7]				0			Input audio type
//	EL2AD_ISI_Channels_Config				VT_I4		[0;20]				0			Config channels for AC-3 and MPEG-1,2,2.5
//	EL2AD_ISI_Mode						VT_I4		[0;20]				0			Layer for MPEG-1,2,2.5 and mode for AAC
//	EL2AD_ISI_Quantization				VT_I4		16,20,24			0			LPCM quantization
//	EL2AD_ISI_Number_Channels				VT_I4		   -				0			Number of channels in source stream
//
//		General:
//	ELAUDEC_Bit_Per_Sample					VT_I4		[0:1]				0			Output bits per sample
//  ELAUDEC_Mute							VT_I4		[0:1]				0			Mute output
//
//
//		MPEG:
//	EL2AD_MAL_Output_Channels_Config		VT_I4		[0:2]				0			Output channels config
//
//		MPEG:
//	EL2AD_MPEG_Channels_Config			VT_I4		[0:2]				2			MPEG channels config for reproduction (default MPEG_CHANNELS_CONFIG_Both)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace el2ad{
	
//////////////////////////////////////////////////////////////////////////
// General settings
//////////////////////////////////////////////////////////////////////////
	enum	BITS_PER_SAMPLE{
		BPS_16		=	0,
		BPS_32		=	1
	};
	
	enum	{DISABLE = 0, ENABLE = 1};
	enum	{OFF = 0, ON = 1};
	


//////////////////////////////////////////////////////////////////////////
// Input stream info (ISI)
//////////////////////////////////////////////////////////////////////////
	// Input audio types
	enum	AUDIO_TYPE{
			AUDIOTYPE_Unknow		=	0,
			AUDIOTYPE_DolbyAC3		=	1,
			AUDIOTYPE_MPEG1			=	2,
			AUDIOTYPE_MPEG2			=	3,
			AUDIOTYPE_MPEG25		=	4,
			AUDIOTYPE_LPCM			=	5,
			AUDIOTYPE_PLII			=	6,
			AUDIOTYPE_AAC			=	7,
            AUDIOTYPE_AES3          =   8
	};

	// Layers for MPEG and modes for AAC
	enum MODE{
		MODE_Unknow			=	0,
		MODEMPEG_Layer1		=	1,
		MODEMPEG_Layer2		=	2,
		MODEMPEG_Layer3		=	3,
		
		MODEAAC_Main		=	4,
		MODEAAC_LC			=	5,
		MODEAAC_SSR			=	6,
		MODEAAC_LTP			=	7,
		MODEAAC_Scalable	=	8,
		MODEAAC_TwinVQ		=	9,
		MODEAAC_CELP		=	10,
		MODEAAC_HVXC		=	11,
		MODEAAC_TTSI		=	12,
		MODEAAC_Main_Synthetic						=	13,
		MODEAAC_Wavetable_Synthesis					=	14,
		MODEAAC_General_MIDI						=	15,
		MODEAAC_Algorithmic_Syntesis_and_Audio_FX	=	16,
		
		MODEAAC_ER_LC			=	17,
		MODEAAC_ER_LTP			=	17,
		MODEAAC_ER_Scalable		=	17,
		MODEAAC_ER_TwinEQ		=	17,
		MODEAAC_ER_BSAC			=	17,
		MODEAAC_ER_LD			=	18,
		MODEAAC_ER_HVXC			=	19,
		MODEAAC_ER_CELP			=	18,
		MODEAAC_ER_HILN			=	19,
		MODEAAC_ER_Parametric	=	20
	};

	// Channels mode for MPEG and AC-3
	enum	CHANNELS_MODE{
		CHANMODE_Unknow					=	0,
		CHANMODEAC3_1_1_Dual			=	1,
		CHANMODEAC3_1_1_Dual_LFE		=	2,
		CHANMODEAC3_1_0_Center			=	3,
		CHANMODEAC3_1_0_Center_LFE		=	4,
		CHANMODEAC3_2_0_Stereo			=	5,	
		CHANMODEAC3_2_0_Stereo_LFE		=	6,	
		CHANMODEAC3_3_0_Front			=	7,	
		CHANMODEAC3_3_0_Front_LFE		=	8,	
		CHANMODEAC3_2_1_Surround		=	9,	
		CHANMODEAC3_2_1_Surround_LFE	=	10,	
		CHANMODEAC3_3_1_Surround		=	11,	
		CHANMODEAC3_3_1_Surround_LFE	=	12,	
		CHANMODEAC3_2_2_Quadro			=	13,	
		CHANMODEAC3_2_2_Quadro_LFE		=	14,	
		CHANMODEAC3_3_2_5Channels		=	15,	
		CHANMODEAC3_3_2_5Channels_LFE	=	16,	
		CHANMODEMPEG_SingleChannel		=	17,
		CHANMODEMPEG_DualChannel		=	18,
		CHANMODEMPEG_JointStereo		=	19,
		CHANMODEMPEG_Stereo				=	20,
	};


//////////////////////////////////////////////////////////////////////////
// AC-3 decoder settings
//////////////////////////////////////////////////////////////////////////

	enum	AC3DEC_STEREO_MODE{
			AC3DEC_STEREO_MODE_Auto = 0,
			AC3DEC_STEREO_MODE_LtRt = 1,
			AC3DEC_STEREO_MODE_LoRo = 2
	};

	enum	AC3DEC_DUALMONO_MODE{
			AC3DEC_DUALMONO_MODE_Stereo		= 0,
			AC3DEC_DUALMONO_MODE_Left_mono	= 1,
			AC3DEC_DUALMONO_MODE_Right_mono = 2,
			AC3DEC_DUALMONO_MODE_Mixed_mono = 3
	};

	enum	AC3DEC_COMPRESSION_MODE{
			AC3DEC_COMPRESSION_MODE_Custom_analog	= 0,
			AC3DEC_COMPRESSION_MODE_Custom_digital	= 1,
			AC3DEC_COMPRESSION_MODE_Line			= 2,
			AC3DEC_COMPRESSION_MODE_RF				= 3
	};

	enum	AC3DEC_CHANNEL_CONFIG{
			AC3DEC_CHANNEL_CONFIG_C				=	0,	// 1/0 Center
			AC3DEC_CHANNEL_CONFIG_L_R			=	1,	// 2/0 Stereo
			AC3DEC_CHANNEL_CONFIG_L_R_C			=	2,  // 3/0 Front
			AC3DEC_CHANNEL_CONFIG_L_R_Ls		=	3,  // 2/1 Surround
			AC3DEC_CHANNEL_CONFIG_L_R_C_Ls		=	4,  // 3/1 Surround
			AC3DEC_CHANNEL_CONFIG_L_R_Ls_Rs		=	5,  // 2/2 Quadro
			AC3DEC_CHANNEL_CONFIG_L_R_C_Ls_Rs	=	6   // 3/2 5 Channels
	};

	enum	AC3DEC_KARAOKE_MODE{
			AC3DEC_KARAOKE_MODE_No_vocal		=	0,
			AC3DEC_KARAOKE_MODE_Left_vocal		=	1,
			AC3DEC_KARAOKE_MODE_Right_vocal		=	2,
			AC3DEC_KARAOKE_MODE_Both_vocal		=	3
	};

	enum	AC3DEC_PANNING_MODE{
			AC3DEC_PANNING_MODE_Center			=	0,
			AC3DEC_PANNING_MODE_Left			=	1,
			AC3DEC_PANNING_MODE_Right			=	2
	};



//////////////////////////////////////////////////////////////////////////
// Pro Logic II decoder settings
//////////////////////////////////////////////////////////////////////////

	enum	PL2DEC_DECODE_MODE{
			PL2DEC_DECODE_MODE_Pro_Logic_emulation		=	0,
			PL2DEC_DECODE_MODE_Virtual_compatible		=	1,
			PL2DEC_DECODE_MODE_Music					=	2,
			PL2DEC_DECODE_MODE_Movie					=	3,
			PL2DEC_DECODE_MODE_Matrix					=	4,
			PL2DEC_DECODE_MODE_Custom					=	5
	};


	enum	PL2DEC_DIMENSION_SETTINGS{
			PL2DEC_DIMENSION_SETTINGS_minus_3			=	0,
			PL2DEC_DIMENSION_SETTINGS_minus_2			=	1,
			PL2DEC_DIMENSION_SETTINGS_minus_1			=	2,
			PL2DEC_DIMENSION_SETTINGS_neutral			=	3,
			PL2DEC_DIMENSION_SETTINGS_plus_1			=	4,
			PL2DEC_DIMENSION_SETTINGS_plus_2			=	5,
			PL2DEC_DIMENSION_SETTINGS_plus_3			=	6
	};

	enum	PL2DEC_CENTER_WIDHT_CONTROL{
			PL2DEC_CENTER_WIDHT_CONTROL_0_deg			=	0,
			PL2DEC_CENTER_WIDHT_CONTROL_20_8_deg		=	1,
			PL2DEC_CENTER_WIDHT_CONTROL_28_deg			=	2,
			PL2DEC_CENTER_WIDHT_CONTROL_36_deg			=	3,
			PL2DEC_CENTER_WIDHT_CONTROL_54_deg			=	4,
			PL2DEC_CENTER_WIDHT_CONTROL_62_deg			=	5,
			PL2DEC_CENTER_WIDHT_CONTROL_69_2_deg		=	6,
			PL2DEC_CENTER_WIDHT_CONTROL_90_deg			=	7
	};


	enum	PL2DEC_CHANNELS_CONFIG{
			PL2DEC_CHANNELS_CONFIG_L_R_C				=	0,
			PL2DEC_CHANNELS_CONFIG_L_R_C_Ls				=	1,
			PL2DEC_CHANNELS_CONFIG_L_R_Ls_Rs			=	2,
			PL2DEC_CHANNELS_CONFIG_L_R_C_Ls_Rs			=	3
	};

//////////////////////////////////////////////////////////////////////////
// MPEG,AAC,LPCM,AES3
//////////////////////////////////////////////////////////////////////////
	
	enum	MAL_CHANNELS_CONFIG{
			MAL_CHANNELS_CONFIG_Auto	=	0,
			MAL_CHANNELS_CONFIG_Mono	=	1,
			MAL_CHANNELS_CONFIG_Stereo	=	2
	};

//////////////////////////////////////////////////////////////////////////
// MPEG
//////////////////////////////////////////////////////////////////////////
	enum	MPEG_CHANNELS_CONFIG{
			MPEG_CHANNELS_CONFIG_First	=	0,
			MPEG_CHANNELS_CONFIG_Second	=	1,
			MPEG_CHANNELS_CONFIG_Both	=	2
	};
	
	enum	LTMode{
		LTMode_not_activate =0,
			LTMode_demo =1,
			LTMode_evaluation =2,
			LTMode_evaluation_expired =3,
			LTMode_full =4
	};

};

//////////////////////////////////////////////////////////////////////////
// Input stream info
//
// {47E44A96-706A-48ec-A083-78F805E7E419}
static const GUID EL2AD_ISI_Audio_Type = 
{ 0x47e44a96, 0x706a, 0x48ec, { 0xa0, 0x83, 0x78, 0xf8, 0x5, 0xe7, 0xe4, 0x19 } };

// For MPEG-1,2,2.5 and AC-3
// {0DC519E1-5F8C-4472-A95D-07304B7C0E6C}
static const GUID EL2AD_ISI_Channels_Config = 
{ 0xdc519e1, 0x5f8c, 0x4472, { 0xa9, 0x5d, 0x7, 0x30, 0x4b, 0x7c, 0xe, 0x6c } };

//For MPEG - Layers, AAC - Modes
// {FA0859A2-BDF7-40cc-8ACB-6CAE8C68CB44}
static const GUID EL2AD_ISI_Mode = 
{ 0xfa0859a2, 0xbdf7, 0x40cc, { 0x8a, 0xcb, 0x6c, 0xae, 0x8c, 0x68, 0xcb, 0x44 } };

// For AC-3
// {80E8E2E6-47D1-4f56-A0AF-D289FA0F9B04}
static const GUID ELAUDEC_ISI_Bit_Stream_ID = 
{ 0x80e8e2e6, 0x47d1, 0x4f56, { 0xa0, 0xaf, 0xd2, 0x89, 0xfa, 0xf, 0x9b, 0x4 } };

//  For LPCM, AES3
// {ACB2B424-9806-4b6e-8900-C37C1A7C9B15}
static const GUID EL2AD_ISI_Quantization = 
{ 0xacb2b424, 0x9806, 0x4b6e, { 0x89, 0x0, 0xc3, 0x7c, 0x1a, 0x7c, 0x9b, 0x15 } };

// For LPCM, AES3, PL2, AAC
// {9CB180BE-272B-42ad-9EB6-5725FBC9EB59}
static const GUID EL2AD_ISI_Number_Channels = 
{ 0x9cb180be, 0x272b, 0x42ad, { 0x9e, 0xb6, 0x57, 0x25, 0xfb, 0xc9, 0xeb, 0x59 } };

// {5DD57963-F8E8-442c-B5B7-06EDB4E5BF77}
static const GUID EL2AD_ISI_Sample_Rate = 
{ 0x5dd57963, 0xf8e8, 0x442c, { 0xb5, 0xb7, 0x6, 0xed, 0xb4, 0xe5, 0xbf, 0x77 } };

// {2AC261C6-F5CF-4380-AB86-147A9571E4EA}
static const GUID EL2AD_ISI_Bit_Rate = 
{ 0x2ac261c6, 0xf5cf, 0x4380, { 0xab, 0x86, 0x14, 0x7a, 0x95, 0x71, 0xe4, 0xea } };

//////////////////////////////////////////////////////////////////////////
// General decoder settings
//
// {14C401D1-FBC0-40af-8BA4-D2C5E3B78192}
static const GUID EL2AD_Bit_Per_Sample = 
{ 0x14c401d1, 0xfbc0, 0x40af, { 0x8b, 0xa4, 0xd2, 0xc5, 0xe3, 0xb7, 0x81, 0x92 } };

// {3918F528-87E4-4edc-814A-C079277E5D1C}
static const GUID ELAUDEC_Enable_Input_Pro_Logic_2 = 
{ 0x3918f528, 0x87e4, 0x4edc, { 0x81, 0x4a, 0xc0, 0x79, 0x27, 0x7e, 0x5d, 0x1c } };

// {0B8AD217-9CBA-4642-A6EB-498F1E072DFE}
static const GUID EL2AD_Mute = 
{ 0xb8ad217, 0x9cba, 0x4642, { 0xa6, 0xeb, 0x49, 0x8f, 0x1e, 0x7, 0x2d, 0xfe } };

// {3B41000F-4BD8-4627-A69F-FDE644DF42C7}
static const GUID EL2AD_Enable_SPDIF = 
{ 0x3b41000f, 0x4bd8, 0x4627, { 0xa6, 0x9f, 0xfd, 0xe6, 0x44, 0xdf, 0x42, 0xc7 } };


//////////////////////////////////////////////////////////////////////////
// MPEG 1,2,2.5, AAC, LPCM, AES3 decoder settings
// {B46D861B-FD25-48c1-AFA2-17F2227D4424}
static const GUID EL2AD_MAL_Output_Channels_Config  = 
{ 0xb46d861b, 0xfd25, 0x48c1, { 0xaf, 0xa2, 0x17, 0xf2, 0x22, 0x7d, 0x44, 0x24 } };

//////////////////////////////////////////////////////////////////////////
// MPEG 1,2,2.5
// {7A26EEDE-AB15-44a7-B4C6-D7CC4BFBBAA1}
static const GUID EL2AD_MPEG_Channels_Config = 
{ 0x7a26eede, 0xab15, 0x44a7, { 0xb4, 0xc6, 0xd7, 0xcc, 0x4b, 0xfb, 0xba, 0xa1 } };


//////////////////////////////////////////////////////////////////////////
// AC-3 decoder settings
//
// {F52D9B8C-C0C5-434a-93A1-E83335625DD6}
static const GUID ELAUDEC_AC3DEC_Stereo_Mode = 
{ 0xf52d9b8c, 0xc0c5, 0x434a, { 0x93, 0xa1, 0xe8, 0x33, 0x35, 0x62, 0x5d, 0xd6 } };

// {6EDF7B39-77A7-4824-968B-680674E5F6E8}
static const GUID ELAUDEC_AC3DEC_DualMono_Mode = 
{ 0x6edf7b39, 0x77a7, 0x4824, { 0x96, 0x8b, 0x68, 0x6, 0x74, 0xe5, 0xf6, 0xe8 } };

// {4FD233E9-ED26-4a74-AF6B-19935893E942}
static const GUID ELAUDEC_AC3DEC_Compression_Mode = 
{ 0x4fd233e9, 0xed26, 0x4a74, { 0xaf, 0x6b, 0x19, 0x93, 0x58, 0x93, 0xe9, 0x42 } };


// {A344E52F-C7E2-4ff8-89E3-04C80F6E9014}
static const GUID ELAUDEC_AC3DEC_Channels_Config = 
{ 0xa344e52f, 0xc7e2, 0x4ff8, { 0x89, 0xe3, 0x4, 0xc8, 0xf, 0x6e, 0x90, 0x14 } };

// {99ACB5CE-893F-4dfe-8675-760B35809521}
static const GUID ELAUDEC_AC3DEC_LFE_Channel = 
{ 0x99acb5ce, 0x893f, 0x4dfe, { 0x86, 0x75, 0x76, 0xb, 0x35, 0x80, 0x95, 0x21 } };

// {DD4EF209-0FC2-4d7f-B8B6-37FA308DCD6B}
static const GUID ELAUDEC_AC3DEC_Karaoke_Mode = 
{ 0xdd4ef209, 0xfc2, 0x4d7f, { 0xb8, 0xb6, 0x37, 0xfa, 0x30, 0x8d, 0xcd, 0x6b } };


// {BC6C865C-7D04-45e3-A4F7-9A462F6DE872}
static const GUID ELAUDEC_AC3DEC_Panning_Mode = 
{ 0xbc6c865c, 0x7d04, 0x45e3, { 0xa4, 0xf7, 0x9a, 0x46, 0x2f, 0x6d, 0xe8, 0x72 } };

// {58B45A5C-D4BD-458d-BB38-1F2692A87772}
static const GUID ELAUDEC_AC3DEC_PCM_Scale_Factor = 
{ 0x58b45a5c, 0xd4bd, 0x458d, { 0xbb, 0x38, 0x1f, 0x26, 0x92, 0xa8, 0x77, 0x72 } };

// {05845B36-A005-4f7c-AF65-8E369A34A13B}
static const GUID ELAUDEC_AC3DEC_DCR_Scale_Factor_High = 
{ 0x5845b36, 0xa005, 0x4f7c, { 0xaf, 0x65, 0x8e, 0x36, 0x9a, 0x34, 0xa1, 0x3b } };

// {29210F40-1858-4281-A422-FEBB01D21CEF}
static const GUID ELAUDEC_AC3DEC_DCR_Scale_Factor_Low = 
{ 0x29210f40, 0x1858, 0x4281, { 0xa4, 0x22, 0xfe, 0xbb, 0x1, 0xd2, 0x1c, 0xef } };

// {A1884CC1-7CF7-437d-9655-DF65CC4BA082}
static const GUID ELAUDEC_AC3DEC_AC3_SPDIF_Output = 
{ 0xa1884cc1, 0x7cf7, 0x437d, { 0x96, 0x55, 0xdf, 0x65, 0xcc, 0x4b, 0xa0, 0x82 } };


// {A03352D5-A1A3-474b-BED0-9E6E5A293BC0}
static const GUID ELAUDEC_AC3DEC_ProLogic2 = 
{ 0xa03352d5, 0xa1a3, 0x474b, { 0xbe, 0xd0, 0x9e, 0x6e, 0x5a, 0x29, 0x3b, 0xc0 } };

//////////////////////////////////////////////////////////////////////////
// Pro Logic II decoder settings
//////////////////////////////////////////////////////////////////////////

// {937CA545-453A-4143-9B92-0EB6588A3DC4}
static const GUID ELAUDEC_PL2DEC_Decode_Mode = 
{ 0x937ca545, 0x453a, 0x4143, { 0x9b, 0x92, 0xe, 0xb6, 0x58, 0x8a, 0x3d, 0xc4 } };

// {C539CAC9-F4F0-4590-A8E8-CEEB935F60B4}
static const GUID ELAUDEC_PL2DEC_Dimension_Mode = 
{ 0xc539cac9, 0xf4f0, 0x4590, { 0xa8, 0xe8, 0xce, 0xeb, 0x93, 0x5f, 0x60, 0xb4 } };

// {41393AE3-D300-4550-B546-FF9362923A7F}
static const GUID ELAUDEC_PL2DEC_Center_Width_Control = 
{ 0x41393ae3, 0xd300, 0x4550, { 0xb5, 0x46, 0xff, 0x93, 0x62, 0x92, 0x3a, 0x7f } };

// {942F5C92-E761-448f-9886-206DD13BF114}
static const GUID ELAUDEC_PL2DEC_Channel_Config = 
{ 0x942f5c92, 0xe761, 0x448f, { 0x98, 0x86, 0x20, 0x6d, 0xd1, 0x3b, 0xf1, 0x14 } };

// {A955F69D-BB85-4e79-BDC5-BEB051265239}
static const GUID ELAUDEC_PL2DEC_Auto_Balance = 
{ 0xa955f69d, 0xbb85, 0x4e79, { 0xbd, 0xc5, 0xbe, 0xb0, 0x51, 0x26, 0x52, 0x39 } };

// {E8B6EAEC-D19E-480f-8408-086FFE4995F0}
static const GUID ELAUDEC_PL2DEC_Surround_Shelf_Filter = 
{ 0xe8b6eaec, 0xd19e, 0x480f, { 0x84, 0x8, 0x8, 0x6f, 0xfe, 0x49, 0x95, 0xf0 } };

// {E6381AB3-B812-46d0-A31F-76A01973F64C}
static const GUID ELAUDEC_PL2DEC_Panorama = 
{ 0xe6381ab3, 0xb812, 0x46d0, { 0xa3, 0x1f, 0x76, 0xa0, 0x19, 0x73, 0xf6, 0x4c } };

// {C19E3F2C-5183-43d4-93D5-9C0F97A33CED}
static const GUID ELAUDEC_PL2DEC_Rs_Polarity_Inversion = 
{ 0xc19e3f2c, 0x5183, 0x43d4, { 0x93, 0xd5, 0x9c, 0xf, 0x97, 0xa3, 0x3c, 0xed } };

// {CC0848B8-EFBE-4be6-B3FD-2D4B96234904}
static const GUID ELAUDEC_PL2DEC_PCM_Scale_Factor = 
{ 0xcc0848b8, 0xefbe, 0x4be6, { 0xb3, 0xfd, 0x2d, 0x4b, 0x96, 0x23, 0x49, 0x4 } };

#endif // _MC_LAYER_II_AUDIO_DECODER_MODULECONFIG_

