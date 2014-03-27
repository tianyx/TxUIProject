/********************************************************************
 Created: 2001/05/14 
 File name: mpeg12mux_guid.h
 Purpose: GUIDs for MPEG1/2 Muxer DS Filter

 Copyright (c) 2001-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef __MPEGM12MUX_GUIDS_H__
#define __MPEGM12MUX_GUIDS_H__

////////////////////////////////////////////////////////////

#ifdef COMPANY_SHORTNAME
  #define COMPANY_REGISTRYNAME COMPANY_SHORTNAME
#else
  #define COMPANY_REGISTRYNAME "MainConcept"
#endif

#undef  COMPANY_HIDEICON

#define COMPANY_MPEGMUXER_NAME         "MCMUXMPEG.AX"

// status defines
#define COMPANY_STATUS_NONE  0
#define COMPANY_STATUS_TRIAL 1
#define COMPANY_STATUS_FULL  2
#define COMPANY_GETSTATUS(dwParam, lpData) COMPANY_STATUS_FULL

////////////////////////////////////////////////////////////
#ifndef RC_INVOKED // don't let resource compiler see this part

// {2BE4D180-6F2E-4b3a-B0BD-E880917238DC}
DEFINE_GUID(CLSID_MainConceptMPEGMuxer, 
0x2BE4D180, 0x6f2e, 0x4b3a, 0xb0, 0xbd, 0xe8, 0x80, 0x91, 0x72, 0x38, 0xdc);

// {2BE4D188-6F2E-4b3a-B0BD-E880917238DC}
DEFINE_GUID(CLSID_MainConceptMpegMuxerProp, 
0x2BE4D188, 0x6f2e, 0x4b3a, 0xb0, 0xbd, 0xe8, 0x80, 0x91, 0x72, 0x38, 0xdc);

// {2BE4D153-6F2E-4b3a-B0BD-E880917238DC}
DEFINE_GUID(IID_IMCMpegMuxer, 
0x2BE4D153, 0x6f2e, 0x4b3a, 0xb0, 0xbd, 0xe8, 0x80, 0x91, 0x72, 0x38, 0xdc);

// {2BE4D156-6F2E-4b3a-B0BD-E880917238DC}
DEFINE_GUID(IID_IMCMpegMuxerFilter, 
0x2BE4D156, 0x6f2e, 0x4b3a, 0xb0, 0xbd, 0xe8, 0x80, 0x91, 0x72, 0x38, 0xdc);

#endif // RC_INVOKED
////////////////////////////////////////////////////////////

#endif // __MPEGM12MUX_GUIDS_H__

