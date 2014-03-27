///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007 MainConcept AG
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GUID                           Value Type    Available range     Default     Note

// EMPGDMX_SUBPICTURE_SUPPORT       VT_I4       0,1                 0	        This parameter enables or disables the subpicture support.
// EMPGDMX_INIT_MODE	            VT_I4       0,1                 0	        This parameter specifies algorithm of the initial stream duration calculation
// EMPGDMX_INIT_DATA_LENGTH         VT_UI4      1000-ULONG_MAX      4194304     This parameter specifies the data length (in bytes) that is read and analized during the initial detection of streams.
// EMPGDMX_DISABLE_ACCURATENAVIGATION   VT_I4   0,1                 0           When this parameter is equal to 1, the demultiplexer simply calculates the position on the basis of bitrate information and does not perform the stream scanning.
// EMPGDMX_DISABLE_NAVIGATION       VT_I4       0,1      		    0           This parameter enables or disables the demultiplexer navigation abilities.
// EMPGDMX_ALWAYS_PTS_NAVIGATION    VT_I4       0,1          		0           This parameter sets the PTS navigation as the preferred navigation mode. If the correct navigation using PTS is impossible the navigation mode is switched to the navigation using bitrate.
// EMPGDMX_DISABLE_NAVIGATIONINITSCAN   VT_I4	0,1    	            0	        If this parameter is equal to 1 then the demultiplexer does not try to scan data at the end of stream and the duration is calculated only on basis of bitrate. 
// EMPGDMX_EXTERNAL_DURATION        VT_I8	    0-_I64_MAX	        _I64_MAX    This parameter allows specifying the stream duration which will be returned with the IMediaSeeking interface. The value is specified in 100 ns. units.
// EMPGDMX_PTSJUMPTHRESHOLD         VT_I8       0-_I64_MAX          15000000    This parameter allows specifying the maximum threshold value for the PTS discontinuity detection. The value is specified in 100 ns. units.      
// EMPGDMX_STREAMTYPE               VT_I4       Read only           0           This parameter contains the type of the detected stream after connecting to the source. Described in EMC_COMMON::STREAM_TYPE.
// EMPGDMX_BASETIME                 VT_I8       0-_I64_MAX          _I64_MAX    This parameter contains the time of base PTS of stream. The value is specified in 100 ns. units.      
// EMPGDMX_INDEX_MODE				VT_I4		0,1,2,3				0			0 - noindex, 1 - load index if present, 2 - load index always, 3 - as 2 and store index
// EMPGDMX_INDEX_LOAD				VT_BSTR										Force load index - if empty string, then load default name index
// EMPGDMX_INDEX_SAVE				VT_BSTR										Force save index - if empty string, then save default name index
// EMPGDMX_TS_TYPE_TABLE          VT_SAFEARRAY                                  { USHORT ,USHORT } = original type and type for replacement
// EMPGDMX_DISABLE_IDR_SEARCH     VT_I4         0,1                  0          During  navigation  disable search for IDR (H264 streams). It is  correct to start  playback   from IDR but some streams has no IDRs or just  one at the  begining

#if !defined(__PROPID_PULLDEMUXERH25320031526_)
#define __PROPID_PULLDEMUXERH25320031526_

#include "common_mc.h"

// {9DA8B680-8F52-4a88-806D-EBEB289BBB25}
static const GUID EMPGDMX_SUBPICTURE_SUPPORT = { 0x9da8b680, 0x8f52, 0x4a88, { 0x80, 0x6d, 0xeb, 0xeb, 0x28, 0x9b, 0xbb, 0x25 } };

// {64B52AAA-C4E5-45be-AD97-82B5E247FFCA}
static const GUID EMPGDMX_INIT_MODE = { 0x64b52aaa, 0xc4e5, 0x45be, { 0xad, 0x97, 0x82, 0xb5, 0xe2, 0x47, 0xff, 0xca } };

// {2BB2DCDC-E4AD-4ea6-B3C7-7D5ACA983968}
static const GUID EMPGDMX_INIT_DATA_LENGTH = { 0x2bb2dcdc, 0xe4ad, 0x4ea6, { 0xb3, 0xc7, 0x7d, 0x5a, 0xca, 0x98, 0x39, 0x68 } };

// {4CEC0C13-A72D-42e4-90DF-CE8B91D5C5E1}
static const GUID EMPGDMX_DISABLE_ACCURATENAVIGATION = { 0x4cec0c13, 0xa72d, 0x42e4, { 0x90, 0xdf, 0xce, 0x8b, 0x91, 0xd5, 0xc5, 0xe1 } };

// {36CBFB53-6D75-4ccd-B67A-CFA4093E20F3}
static const GUID EMPGDMX_DISABLE_NAVIGATION = { 0x36cbfb53, 0x6d75, 0x4ccd, { 0xb6, 0x7a, 0xcf, 0xa4, 0x9, 0x3e, 0x20, 0xf3 } };

// {F02C83CA-29E3-4044-BFD4-59906D358683}
static const GUID EMPGDMX_ALWAYS_PTS_NAVIGATION = { 0xf02c83ca, 0x29e3, 0x4044, { 0xbf, 0xd4, 0x59, 0x90, 0x6d, 0x35, 0x86, 0x83 } };

// {25A0589F-C6AE-40b1-998E-FDF145ACA5EC}
static const GUID EMPGDMX_DISABLE_NAVIGATIONINITSCAN = { 0x25a0589f, 0xc6ae, 0x40b1, { 0x99, 0x8e, 0xfd, 0xf1, 0x45, 0xac, 0xa5, 0xec } };

// {AC02E7D0-19D2-4ffe-90E4-DD7285E1515D}
static const GUID EMPGDMX_EXTERNAL_DURATION = { 0xac02e7d0, 0x19d2, 0x4ffe, { 0x90, 0xe4, 0xdd, 0x72, 0x85, 0xe1, 0x51, 0x5d } };

// {FE8522C7-E8E6-44a2-B61A-DF72ECA6DB45}
static const GUID EMPGDMX_PTSJUMPTHRESHOLD = { 0xfe8522c7, 0xe8e6, 0x44a2, { 0xb6, 0x1a, 0xdf, 0x72, 0xec, 0xa6, 0xdb, 0x45 } };

// {59D26E5F-6C2A-4406-B9B8-19B6A1C87EE1}
static const GUID EMPGDMX_STREAMTYPE = { 0x59d26e5f, 0x6c2a, 0x4406, { 0xb9, 0xb8, 0x19, 0xb6, 0xa1, 0xc8, 0x7e, 0xe1 } };

// {84512624-A6CE-4537-B0D7-2D10BA8DE6C1}
static const GUID EMPGDMX_BASETIME = { 0x84512624, 0xa6ce, 0x4537, { 0xb0, 0xd7, 0x2d, 0x10, 0xba, 0x8d, 0xe6, 0xc1 } };

// {8CD97D7F-6606-4acd-8732-3F7A84862ED2}
static const GUID EMPGDMX_INDEX_MODE = { 0x8cd97d7f, 0x6606, 0x4acd, { 0x87, 0x32, 0x3f, 0x7a, 0x84, 0x86, 0x2e, 0xd2 } };

// {B64E15C7-57AB-40d9-BD91-5A7FD029F071}
static const GUID EMPGDMX_INDEX_LOAD = { 0xb64e15c7, 0x57ab, 0x40d9, { 0xbd, 0x91, 0x5a, 0x7f, 0xd0, 0x29, 0xf0, 0x71 } };

// {97C096E0-3B2E-4b56-A030-2164D3B940F9}
static const GUID EMPGDMX_INDEX_SAVE = { 0x97c096e0, 0x3b2e, 0x4b56, { 0xa0, 0x30, 0x21, 0x64, 0xd3, 0xb9, 0x40, 0xf9 } };


// {2177BB43-C806-4f9d-930D-25ECEF4B8EF7}
static const GUID EMPGDMX_TS_TYPE_TABLE = { 0x2177bb43, 0xc806, 0x4f9d, { 0x93, 0xd, 0x25, 0xec, 0xef, 0x4b, 0x8e, 0xf7 } };


// {51A02F2A-5BA6-456e-A653-4AA923EDBF0E}
static const GUID EMPGDMX_DISABLE_IDR_SEARCH = { 0x51a02f2a, 0x5ba6, 0x456e, { 0xa6, 0x53, 0x4a, 0xa9, 0x23, 0xed, 0xbf, 0xe } };

#endif //#if !defined(__PROPID_PULLDEMUXERH25320031526_)
