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
//  Purpose: The definition of the MainConcept Mpeg-1/2 Muxer filter's CLSIDs of its parameters
//

#ifndef __MCM12MUX_FILTER_PROPID__
#define __MCM12MUX_FILTER_PROPID__


// {F41C29BE-7E55-4E78-8A05-7B1FDE6EE482}
static const GUID CLSID_MCM12MUX_InternalData = 
{ 0xf41c29be, 0x7e55, 0x4e78, { 0x8a, 0x5, 0x7b, 0x1f, 0xde, 0x6e, 0xe4, 0x82 } };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	     GUID								        Type		   Available range       Default value   Note
// MCM12MUX_MpegType               VT_UI4  [MPEG_MPEG1, MPEG_AVCHD]	[MPEG_MPEG2]	Determine the Mpeg (and "mplex_") type to mux
// MCM12MUX_sectors_delay          VT_INT       [0, 2^31-1]               [0]       Time stamp of first mpeg pack          
// MCM12MUX_video_delay_ms         VT_INT         [0, 1000]             [400]       Default time stamp of the first video packet for all video streams, 0 = automatically computed
// MCM12MUX_audio1_delay_ms        VT_INT         [0, 1000]             [400]       Default time stamp of the first audio packet for all audio streams, 0 = automatically computed
// MCM12MUX_sector_size            VT_INT       [312, MAX_SECTOR_SIZE] [2048]       Size of the mpeg packs
// MCM12MUX_packets_per_pack       VT_INT       [1,(sector_size-35)/264]  [1]       Number of PES packets per pack
// MCM12MUX_audio1_buffer_size     VT_INT       0, 2,...,64               [0]       Default P-STD audio buffer size for all audio streams, 0 = automatically computed
// MCM12MUX_video_buffer_size      VT_INT       0, 23 ... 256             [0]       Default P-STD video buffer size for all video streams, 0 = automatically computed
// MCM12MUX_extra_padding_packs    VT_INT       [0, 2^31-1]               [0]       Extra padding packets per second for transport streams only
// MCM12MUX_user_mux_rate_bps      VT_INT       [0, 2^31-1]               [0]       Force a particular mux rate (in bps)
// MCM12MUX_mplex_type             VT_INT  [MPEG_MPEG1, MPEG_AVCHD] [MPEG_MPEG2]  Type of program stream, one of the MPEG_ defines in mpegdef.h (use MCM12MUX_MpegType instead to adjust all mpeg_m_settings to proper defaults)
// MCM12MUX_mplex_pulldown_flag    VT_INT           [0, 7]          [PULLDOWN_AUTO] One of the PULLDOWN_ flags from mpegdef.h
// MCM12MUX_vcd_audio_pad          VT_INT           [0, 1]                [0]       Pad each vcd audio sector with 20 zeros
// MCM12MUX_align_sequence_headers VT_INT           [0, 1]                [0]       Align sequence headers to the start of a PES packet
// MCM12MUX_user_mux_rate          VT_INT           [0, 2^31-1]           [0]       Force a particular mux rate (in 50 byte/sec units)
// MCM12MUX_put_private2           VT_INT           [0, 1]                [0]       Put DVD private stream 2's instead of padding packets
// MCM12MUX_frame_timestamps       VT_INT           [0, 2]         [TIMESTAMPS_ALL] One of the TIMESTAMPS_ flags from mpegdef.h
// MCM12MUX_VBR_multiplex          VT_INT           [0, 1]                [1]       Do VBR style multiplexing
// MCM12MUX_write_pec              VT_INT           [0, 1]                [1]       Write a program end code
// MCM12MUX_mux_SVCD_scan_offsets  VT_INT           [0, 1]                [0]       Compute SVCD scan offsets
// MCM12MUX_max_file_size          VT_INT           [0, 4096]             [0]       Split file every so many MB
// MCM12MUX_feature_flags          VT_UINT       0 or MUX_FEATURE_TS_ATS  [0]       One or more of the MUX_FEATURE_* defines
// MCM12MUX_reset_clocks           VT_INT           [0, 1]                [1]       Reset the clocks in split files
// MCM12MUX_write_end_codes        VT_INT           [0, 1]                [1]       Write program end codes in split files
// MCM12MUX_set_broken_link        VT_INT           [0, 1]                [1]       Set the broken link flag in the first GOP of split files
//
// MCM12MUX_ext_flags              VT_UINT          []                    [0]       Flags, one or more of the EXT_FLAG_ defines
// MCM12MUX_NIT_PID                VT_INT           [0, 0x1FFF]           [0]       Network information section PID, 0 = automatically computed
// MCM12MUX_SDT_PID                VT_INT           [0, 0x1FFF]           [0]       Service description section PID, 0 = automatically computed
// MCM12MUX_EIT_PID                VT_INT           [0, 0x1FFF]           [0]       Event information section PID, 0 = automatically computed
// MCM12MUX_TDT_PID                VT_INT           [0, 0x1FFF]           [0]       Time and date section PID, 0 = automatically computed
// MCM12MUX_PMT_PID                VT_INT           [0, 0x1FFF]           [0]       Program map section PID, 0 = automatically computed
// MCM12MUX_PCR_PID                VT_INT           [0, 0x1FFF]           [0]       PID the PCR is placed on, 0 = automatically computed
// MCM12MUX_PAT_LIMIT              VT_INT           >=0                   [0]       Time (in ms) before the PAT and other tables are repeated, 0 = automatically computed
// MCM12MUX_PCR_LIMIT              VT_INT           >=0                   [0]       Upper limit (in ms) of the PCR repeat rate, 0 = automatically computed
// MCM12MUX_program_number         VT_INT           [0, 65535]            [0]       Transport stream program number
// MCM12MUX_network_id             VT_INT           [0, 65535]            [0]       Transport stream network ID
// MCM12MUX_transport_id           VT_INT           [0, 65535]            [0]       Transport stream ID
//
// MCM12MUX_PAGEDISPLAYMODE        VT_UI4           [0, 2]             [AUTO]       Send display mode for property page: MFS_PAGEDISPLAYMODE_AUTO,_ON,_OFF
// MCM12MUX_SHOW_SPLIT_OPTIONS     VT_UI4           [0, 1]                [1]       Show split options on property page
// MCM12MUX_SPLIT_OUTPUT           VT_BOOL          [0, 1]                [0]       Split file on next GOP boundary
// MCM12MUX_CREATELISTFILE         VT_BOOL          [0, 1]                [0]       Create a .mzl file with a list of captured files
// MCM12MUX_WRITERMODE             VT_UI4           [0, 1]     [MFS_WRITER_SINGLE]  Set the writer mode to one of the MFS_WRITER defines
// MCM12MUX_TIMELIMIT              VT_UI4           >=0                   [0]       Time limit in msec, default 0: disabled
// MCM12MUX_WRITE_IDX_FILES        VT_BOOL          [0, 1]                [0]       Write index output to files - takes effect if a file has been set
// MCM12MUX_SYNC_ON_TIMESTAMPS     VT_UI4           [0, 1]                [0]       Synchronize audio inputs on first video input's first I-Frames timestamp
// MCM12MUX_VIDEO_INPUT_BUFFERSIZE VT_UI4           [>=4096]           [4096]       Size of video inputs fifo in kB. default (4MB)
// MCM12MUX_AUDIO_INPUT_BUFFERSIZE VT_UI4           [>=2048]           [2048]       Size of audio inputs fifo in kB. default (2MB)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// special property page parameters:
//

// {3C9FE97B-9A26-4BC2-AA11-7EB801CB39DD}
static const GUID MCM12MUX_MpegType = 
{ 0x3c9fe97b, 0x9a26, 0x4bc2, { 0xaa, 0x11, 0x7e, 0xb8, 0x1, 0xcb, 0x39, 0xdd } };

//
// mpeg_m_settings parameters (see mpgmux.hpp):
//

// {7D10D768-3214-4E91-BDE6-CE60C84EC3DE}
static const GUID MCM12MUX_sectors_delay = 
{ 0x7d10d768, 0x3214, 0x4e91, { 0xbd, 0xe6, 0xce, 0x60, 0xc8, 0x4e, 0xc3, 0xde } };

// {A2682719-A4C3-4D51-BEC9-8FB7E6C07B5C}
static const GUID MCM12MUX_video_delay_ms = 
{ 0xa2682719, 0xa4c3, 0x4d51, { 0xbe, 0xc9, 0x8f, 0xb7, 0xe6, 0xc0, 0x7b, 0x5c } };

// {2D800E51-2106-4B0B-AE49-DBDA41B1F8F4}
static const GUID MCM12MUX_audio1_delay_ms = 
{ 0x2d800e51, 0x2106, 0x4b0b, { 0xae, 0x49, 0xdb, 0xda, 0x41, 0xb1, 0xf8, 0xf4 } };

// {60E8B79A-7935-4F4D-9CBD-349AF5499CD4} // NOT USED!
//static const GUID MCM12MUX_audio2_delay_ms = 
//{ 0x60e8b79a, 0x7935, 0x4f4d, { 0x9c, 0xbd, 0x34, 0x9a, 0xf5, 0x49, 0x9c, 0xd4 } };

// {C2549D27-4136-4812-B9CA-939CECB6A0E7}
static const GUID MCM12MUX_sector_size = 
{ 0xc2549d27, 0x4136, 0x4812, { 0xb9, 0xca, 0x93, 0x9c, 0xec, 0xb6, 0xa0, 0xe7 } };

// {0604BACE-2EF8-4963-BCFA-86AD807877BD}
static const GUID MCM12MUX_packets_per_pack = 
{ 0x604bace, 0x2ef8, 0x4963, { 0xbc, 0xfa, 0x86, 0xad, 0x80, 0x78, 0x77, 0xbd } };

// {4B0D0E65-D1B3-4EF7-801D-46D0A484B0D9}
static const GUID MCM12MUX_audio1_buffer_size = 
{ 0x4b0d0e65, 0xd1b3, 0x4ef7, { 0x80, 0x1d, 0x46, 0xd0, 0xa4, 0x84, 0xb0, 0xd9 } };

// {EDEBAC0A-2269-4E92-96CF-7B69995354F9} // NOT USED!
//static const GUID MCM12MUX_audio2_buffer_size = 
//{ 0xedebac0a, 0x2269, 0x4e92, { 0x96, 0xcf, 0x7b, 0x69, 0x99, 0x53, 0x54, 0xf9 } };

// {91C70041-B5DA-45E3-A629-8C2139F0D610}
static const GUID MCM12MUX_video_buffer_size = 
{ 0x91c70041, 0xb5da, 0x45e3, { 0xa6, 0x29, 0x8c, 0x21, 0x39, 0xf0, 0xd6, 0x10 } };

// {7690E735-03B3-4F45-AB4C-ECBD655E8014}
static const GUID MCM12MUX_extra_padding_packs = 
{ 0x7690e735, 0x3b3, 0x4f45, { 0xab, 0x4c, 0xec, 0xbd, 0x65, 0x5e, 0x80, 0x14 } };

// {96151F49-D6E8-4C09-861B-C400862B5BF7}
static const GUID MCM12MUX_user_mux_rate_bps = 
{ 0x96151f49, 0xd6e8, 0x4c09, { 0x86, 0x1b, 0xc4, 0x0, 0x86, 0x2b, 0x5b, 0xf7 } };

// {3F5DF75A-305F-4512-AE22-D2CD3B99BD0B}
static const GUID MCM12MUX_mplex_type = 
{ 0x3f5df75a, 0x305f, 0x4512, { 0xae, 0x22, 0xd2, 0xcd, 0x3b, 0x99, 0xbd, 0xb } };

// {5541A918-BE23-433C-92F0-F442301D7F52}
static const GUID MCM12MUX_mplex_pulldown_flag = 
{ 0x5541a918, 0xbe23, 0x433c, { 0x92, 0xf0, 0xf4, 0x42, 0x30, 0x1d, 0x7f, 0x52 } };

// {145179CF-3259-4D54-84B9-ADF48961CD82}
static const GUID MCM12MUX_vcd_audio_pad = 
{ 0x145179cf, 0x3259, 0x4d54, { 0x84, 0xb9, 0xad, 0xf4, 0x89, 0x61, 0xcd, 0x82 } };

// {0C45D853-6773-4312-A339-04F97B2AA722}
static const GUID MCM12MUX_align_sequence_headers = 
{ 0xc45d853, 0x6773, 0x4312, { 0xa3, 0x39, 0x4, 0xf9, 0x7b, 0x2a, 0xa7, 0x22 } };

// {95873F3C-7DFE-4832-A57A-E5DD17D0BE2E}
static const GUID MCM12MUX_user_mux_rate = 
{ 0x95873f3c, 0x7dfe, 0x4832, { 0xa5, 0x7a, 0xe5, 0xdd, 0x17, 0xd0, 0xbe, 0x2e } };

// {91DA5A46-2B12-44C5-AD14-C4607A39EDDB}
static const GUID MCM12MUX_put_private2 = 
{ 0x91da5a46, 0x2b12, 0x44c5, { 0xad, 0x14, 0xc4, 0x60, 0x7a, 0x39, 0xed, 0xdb } };

// {7E966517-5011-47EB-9EEF-691E1364EECF}
static const GUID MCM12MUX_frame_timestamps = 
{ 0x7e966517, 0x5011, 0x47eb, { 0x9e, 0xef, 0x69, 0x1e, 0x13, 0x64, 0xee, 0xcf } };

// {C90A3A67-4D55-4D4A-9CC3-B817F108D6B6}
static const GUID MCM12MUX_VBR_multiplex = 
{ 0xc90a3a67, 0x4d55, 0x4d4a, { 0x9c, 0xc3, 0xb8, 0x17, 0xf1, 0x8, 0xd6, 0xb6 } };

// {45F44975-BB3A-44FD-917D-E5CAAA217CF5}
static const GUID MCM12MUX_write_pec = 
{ 0x45f44975, 0xbb3a, 0x44fd, { 0x91, 0x7d, 0xe5, 0xca, 0xaa, 0x21, 0x7c, 0xf5 } };

// {9C9F3814-F4A4-4E3D-AE44-4B5534C9705E}
static const GUID MCM12MUX_mux_SVCD_scan_offsets = 
{ 0x9c9f3814, 0xf4a4, 0x4e3d, { 0xae, 0x44, 0x4b, 0x55, 0x34, 0xc9, 0x70, 0x5e } };

// {E65A7EB7-7FB9-491c-9326-8E101ABE204C} // used on property page, too
static const GUID MCM12MUX_max_file_size = 
{ 0xe65a7eb7, 0x7fb9, 0x491c, { 0x93, 0x26, 0x8e, 0x10, 0x1a, 0xbe, 0x20, 0x4c } };

// {7BCA2126-99A2-46E3-B0D8-54A8D02748BE}
static const GUID MCM12MUX_feature_flags = 
{ 0x7bca2126, 0x99a2, 0x46e3, { 0xb0, 0xd8, 0x54, 0xa8, 0xd0, 0x27, 0x48, 0xbe } };

// {BCF55CC7-1DBC-4F73-BB6D-4D2AF9F1C8E7} // FUTURE USE ONLY!
//static const GUID MCM12MUX_reserved3 = 
//{ 0xbcf55cc7, 0x1dbc, 0x4f73, { 0xbb, 0x6d, 0x4d, 0x2a, 0xf9, 0xf1, 0xc8, 0xe7 } };

// {6B22267F-FF17-45D3-B132-14CC80153836}
static const GUID MCM12MUX_reset_clocks = 
{ 0x6b22267f, 0xff17, 0x45d3, { 0xb1, 0x32, 0x14, 0xcc, 0x80, 0x15, 0x38, 0x36 } };

// {A40AF281-56EB-4C3E-A699-6A5C4A39F222}
static const GUID MCM12MUX_write_end_codes = 
{ 0xa40af281, 0x56eb, 0x4c3e, { 0xa6, 0x99, 0x6a, 0x5c, 0x4a, 0x39, 0xf2, 0x22 } };

// {71594DAA-38A2-4239-8BA6-59B316D6EBD8}
static const GUID MCM12MUX_set_broken_link = 
{ 0x71594daa, 0x38a2, 0x4239, { 0x8b, 0xa6, 0x59, 0xb3, 0x16, 0xd6, 0xeb, 0xd8 } };


//
// extended_m_settings parameters (see mpgmux.hpp):
//

// {2199C823-89DE-4ABA-AC62-E0F741CE4D5E}
static const GUID MCM12MUX_ext_flags = 
{ 0x2199c823, 0x89de, 0x4aba, { 0xac, 0x62, 0xe0, 0xf7, 0x41, 0xce, 0x4d, 0x5e } };

// {4804E83C-F5AB-4C98-B9B0-6D4F8FB3814D}
static const GUID MCM12MUX_NIT_PID = 
{ 0x4804e83c, 0xf5ab, 0x4c98, { 0xb9, 0xb0, 0x6d, 0x4f, 0x8f, 0xb3, 0x81, 0x4d } };

// {51139864-0F6D-48D9-9587-996FE4BB7A83}
static const GUID MCM12MUX_SDT_PID = 
{ 0x51139864, 0xf6d, 0x48d9, { 0x95, 0x87, 0x99, 0x6f, 0xe4, 0xbb, 0x7a, 0x83 } };

// {44373177-A931-4C11-B8E9-E61EF6866B62}
static const GUID MCM12MUX_EIT_PID = 
{ 0x44373177, 0xa931, 0x4c11, { 0xb8, 0xe9, 0xe6, 0x1e, 0xf6, 0x86, 0x6b, 0x62 } };

// {B967759E-CA32-45D1-86D2-F20F4588A429}
static const GUID MCM12MUX_TDT_PID = 
{ 0xb967759e, 0xca32, 0x45d1, { 0x86, 0xd2, 0xf2, 0xf, 0x45, 0x88, 0xa4, 0x29 } };

// {FDBA84E2-A7B7-4900-9977-83B3519A040D}
static const GUID MCM12MUX_PMT_PID = 
{ 0xfdba84e2, 0xa7b7, 0x4900, { 0x99, 0x77, 0x83, 0xb3, 0x51, 0x9a, 0x4, 0xd } };

// {2BFA7CD5-B916-41C1-B358-A1A46A950D76}
static const GUID MCM12MUX_PCR_PID = 
{ 0x2bfa7cd5, 0xb916, 0x41c1, { 0xb3, 0x58, 0xa1, 0xa4, 0x6a, 0x95, 0xd, 0x76 } };

// {2FD249DB-4DCC-4D82-810E-D1EAAE7D8008}
static const GUID MCM12MUX_PAT_LIMIT = 
{ 0x2fd249db, 0x4dcc, 0x4d82, { 0x81, 0xe, 0xd1, 0xea, 0xae, 0x7d, 0x80, 0x8 } };

// {1CAC2120-4810-4624-9411-3F4B23CFEEC2}
static const GUID MCM12MUX_PCR_LIMIT = 
{ 0x1cac2120, 0x4810, 0x4624, { 0x94, 0x11, 0x3f, 0x4b, 0x23, 0xcf, 0xee, 0xc2 } };

// {50BCFFD8-D1EA-4557-B7F7-F53232DCD7DC}
static const GUID MCM12MUX_program_number = 
{ 0x50bcffd8, 0xd1ea, 0x4557, { 0xb7, 0xf7, 0xf5, 0x32, 0x32, 0xdc, 0xd7, 0xdc } };

// {6743DB03-276C-44D6-B348-B354A7CA10FA}
static const GUID MCM12MUX_network_id = 
{ 0x6743db03, 0x276c, 0x44d6, { 0xb3, 0x48, 0xb3, 0x54, 0xa7, 0xca, 0x10, 0xfa } };

// {1889F1A7-7732-4CE6-8C16-D632B2DA80D5}
static const GUID MCM12MUX_transport_id = 
{ 0x1889f1a7, 0x7732, 0x4ce6, { 0x8c, 0x16, 0xd6, 0x32, 0xb2, 0xda, 0x80, 0xd5 } };

//
// MFSETTINGS_ parameters (see imceprop.h):
//

// {D8D54A7C-0CF4-4D0F-91C4-A8B8B5EB9997}
static const GUID MCM12MUX_PAGEDISPLAYMODE = 
{ 0xd8d54a7c, 0xcf4, 0x4d0f, { 0x91, 0xc4, 0xa8, 0xb8, 0xb5, 0xeb, 0x99, 0x97 } };

// {7A4EC230-11B0-4D97-838B-E6730B564E2B}
static const GUID MCM12MUX_SHOW_SPLIT_OPTIONS = 
{ 0x7a4ec230, 0x11b0, 0x4d97, { 0x83, 0x8b, 0xe6, 0x73, 0xb, 0x56, 0x4e, 0x2b } };

// {4AFEFE74-E1A7-48CB-9C20-DAF34AA1F459}
static const GUID MCM12MUX_SPLIT_OUTPUT = 
{ 0x4afefe74, 0xe1a7, 0x48cb, { 0x9c, 0x20, 0xda, 0xf3, 0x4a, 0xa1, 0xf4, 0x59 } };

// {E8DC294B-37F1-4DA2-B702-D8BFED7A403E}
static const GUID MCM12MUX_CREATELISTFILE = 
{ 0xe8dc294b, 0x37f1, 0x4da2, { 0xb7, 0x2, 0xd8, 0xbf, 0xed, 0x7a, 0x40, 0x3e } };

// {5E7128DC-C69D-4FFC-99A8-3D2365A8FF32}
static const GUID MCM12MUX_WRITERMODE = 
{ 0x5e7128dc, 0xc69d, 0x4ffc, { 0x99, 0xa8, 0x3d, 0x23, 0x65, 0xa8, 0xff, 0x32 } };

// {B8B12850-A8BB-42D4-B61B-5081BB52E0E8}
static const GUID MCM12MUX_TIMELIMIT = 
{ 0xb8b12850, 0xa8bb, 0x42d4, { 0xb6, 0x1b, 0x50, 0x81, 0xbb, 0x52, 0xe0, 0xe8 } };

// {D12C2EBC-362A-4D76-AA25-D248E0F47949}
static const GUID MCM12MUX_WRITE_IDX_FILES = 
{ 0xd12c2ebc, 0x362a, 0x4d76, { 0xaa, 0x25, 0xd2, 0x48, 0xe0, 0xf4, 0x79, 0x49 } };

// {F0CC11E9-8BD6-4019-B4E0-65F7BA1B2797}
static const GUID MCM12MUX_SYNC_ON_TIMESTAMPS = 
{ 0xf0cc11e9, 0x8bd6, 0x4019, { 0xb4, 0xe0, 0x65, 0xf7, 0xba, 0x1b, 0x27, 0x97 } };

// {9B47B43B-3717-4B49-B164-70F3D14263CB}
static const GUID MCM12MUX_VIDEO_INPUT_BUFFERSIZE = 
{ 0x9b47b43b, 0x3717, 0x4b49, { 0xb1, 0x64, 0x70, 0xf3, 0xd1, 0x42, 0x63, 0xcb } };

// {36339C16-B5B0-4367-A20B-52230736927E}
static const GUID MCM12MUX_AUDIO_INPUT_BUFFERSIZE = 
{ 0x36339c16, 0xb5b0, 0x4367, { 0xa2, 0xb, 0x52, 0x23, 0x7, 0x36, 0x92, 0x7e } };

// MFSETTING_PAD_PES_EXTENSION and MFSETTING_AUDIO_NO_SPAN_VOBU are deprecated,
// use the corresponding MCM12MUX_ext_flags parameter instead;
// MFSETTING_STATISTIC is accessible via IMCMpegMuxerFilter interface only

#endif //__MCM12MUX_FILTER_PROPID__

