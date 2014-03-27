//------------------------------------------------------------------------------
// File: mxfmux.h
//
// Desc: mxf muxer header
//
// Copyright (c) 2000-2008, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#if !defined(__MC_MXF_MUX_H__)
#define __MC_MXF_MUX_H__

#include "bufstrm.hpp"
#include "mcprofile.h"

typedef struct mxf_muxer mxf_muxer_tt; // muxer object

#define MAX_STREAMS					32

///////////////////////////////////////////////////////////////////////////
///////////// outdated - do not use - for compatibility only //////////////
// profiles
#define MXF_PROF_SONY_MPEG_HD		0 // XDCAM  
#define MXF_PROF_PANASONIC_DVCPRO	3 // P2 DVCPRO

// mux types
#define MXF_MUX_TYPE_DVPRO			100 
// already also defined in mpegdef.h
#ifndef MPEG_XDCAM_IMX
#define MPEG_XDCAM_IMX				28
#endif
#ifndef MPEG_XDCAM_HD
#define MPEG_XDCAM_HD				32
#endif
///////////// outdated ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


// profile type
#define MXF_PROF_SONY_XDCAM 		  0   // XDCAM  
#define MXF_PROF_PANASONIC_P2		  3   // Panasonic P2

// multiplex sub types
#define MXF_MUX_TYPE_XDCAM_HD		  32  // XDCAM HD   - matches MPEG_XDCAM_HD in mpegdef.h
#define MXF_MUX_TYPE_XDCAM_IMX		28  // XDCAM IMX  (not supported yet) - matches MPEG_XDCAM_IMX in mpegdef.h
#define MXF_MUX_TYPE_XDCAM_DV		  99  // XDCAM DV   (not supported yet)
#define MXF_MUX_TYPE_P2_DVCPRO		100 // P2 DVCPRO
#define MXF_MUX_TYPE_P2_AVCI		  101 // P2 AVC-Intra (not supported yet)

// flags
#define MXF_MUX_FLAG_DROP_FRAME       0x00000001  // use drop frame timecode
                                                  // If DV essence has embedded timecode this will be overwritten by the DF flag in DIF frames timecode

#define MXF_MUX_FLAG_XDCAM_11_COMP    0x00001000  // force old XDCAM 1.1 spec. format.  
                                                  // Only use for if the output streams compatibilty with old applications that do not 
                                                  // support 2008 1.2 specs w. XDCAM meta data correction and changed XDCAM HD index structure is needed.


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif


// this structure should remain at 108 bytes to match the mpgmux structure
struct mxf_m_settings
{
	int32_t  profile;					        // muxer profile - one of MXF_PROF_XX

	uint32_t video_sequence_length;		// obsolete - not used

	int32_t	 reserved1[9];

	int32_t	 mplex_type;				      // multiplex sub type - one of MXF_MUX_TYPE_XXX

	int32_t  reserved2[7];

  int32_t  flags;                   // or'ed combination of  MXF_MUX_FLAG_xxx

	uint32_t start_timecode;			    // start time code in frames - Time code Component:Start Time Code,
										                // in case of spanned clips represents the start TC of first clip
										                // - used for P2 only actualy

	uint32_t start_v_duration;			  // video duration in frames of all previous linked clips - 0 if
										                // first of spanned clips - used for P2 only actualy

	uint64_t start_a_duration;			  // audio duration in samples of all previous linked clips - 0 if
										                // first of spanned clips - used for P2 only actualy

	uint8_t  *material_id;				    // pointer to a 32 byte array containing UMID for P2 or 0. 
										                // If 0 UMID is generated internaly - In case of spanned clips UMID
										                // must be passed from external and be same for all clips

	char     *p2_user_clip_name;		  // string containing external P2 clip name or 0. 6 characters following P2 specification, eg
										                // 0023AB. If 0 name is generated internal from clip no and UMID

	int32_t  file_number;				      // file number for P2 0..999 used as first four digits for internal clip name generation
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// muxing routines in mcmxfmux.dll
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// call to fill an mxf_m_settings-structure with defaults values
// based on one of the FORMAT_xxx preset values
// 
//  inputs:
//    set - pointer to an mxf_m_settings structure
//    format_type - one of the FORMAT_xxx constants
// output:
//    modified mxf_m_settings structure
//  return:
//    none

char *mxfMuxDefaults(struct mxf_m_settings *set, int32_t profileID);


// call to create an mxf muxer object
//
//  inputs:
//    get_rc - pointer to a get resource function 
//    set - pointer to a filled in mxf_m_settings structure
// outputs:
//    none
//  return:
//    pointer to a mxf_muxer_tt object if succesful
//    NULL if unsuccesful

mxf_muxer_tt * mxfMuxNew(void *(*get_rc)(char* name), const struct mxf_m_settings *set);


// call to add a video, audio or subpic stream to the muxer in streaming mode,
//
//  inputs:
//    muxer - pointer to an mxf muxer object
//    reserved - reserved, set to NULL
//    input - pointer to a bufstream_tt object for the input data
// outputs:
//    none
//  return:
//    0 if successful

int32_t mxfMuxAddInputStream(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *input);


// call to add an output mxf stream to the muxer in streaming mode,
//
//  inputs:
//    muxer - pointer to an mxf muxer object
//    reserved - reserved, set to NULL
//    output - pointer to a bufstream_tt object for the output data
// outputs:
//    none
//  return:
//    0 if successful

int32_t mxfMuxAddOutputStream(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *output);


// call to add a video, audio or subpic stream to the muxer in file mode,
//
//  inputs:
//    muxer - pointer to an mxf muxer object
//    reserved - reserved, set to NULL
//    input - pointer to a filename or bufstream for the input data
// outputs:
//    none
//  return:
//    0 if successful

int32_t mxfMuxAddInputFile(mxf_muxer_tt *muxer, void *reserved, char *input);
int32_t mxfMuxAddInputFileBS(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *input);


// call to add an output mxf stream to the muxer in file mode,
//
//  inputs:
//    muxer - pointer to an mxf muxer object
//    reserved - reserved, set to NULL
//    output - pointer to a filename or bufstream for the output data
// outputs:
//    none
//  return:
//    0 if successful

int32_t mxfMuxAddOutputFile(mxf_muxer_tt *muxer, void *reserved, char *output);
int32_t mxfMuxAddOutputFileBS(mxf_muxer_tt *muxer, void *reserved, bufstream_tt *output);


// call to do the multiplexing in file mode
// 
//  inputs:
//    muxer -  pointer to an mxf muxer object
// outputs:
//    muxed data to output file
//  return:
//    0 if succesful

uint32_t mxfMux(mxf_muxer_tt *muxer);


// call to finish a muxing session, set abort non-zero if muxing is being
// aborted.
//
//  inputs:
//    muxer - pointer to an mxf muxer object
//    abort - set to 0 to finish any leftover muxing and clean up,
//            else just clean up
// outputs:
//    muxed data to output file if needed
//  return:
//    0 if successful

int32_t mxfMuxDone(mxf_muxer_tt *muxer, int32_t abort);


// call to free an mxf muxer object
//
//  inputs:
//    muxer - muxer object to free
// outputs:
//    none
//  return:
//    none

void mxfMuxFree(mxf_muxer_tt *muxer);


// call to get the setting errors/warnings in an mxf_m_settings structure
// use with the get_rc callback to provide an err_printf callback to get
// error messages that can be localized
//
//  inputs:
//    get_rc - pointer to a get_rc function
//    set - pointer to an mxf_m_settings structure
//    options - reserved, set to 0
//    app - reserved
// outputs:
//    none
//  return:
//    0 if no errors found
//    one of the INV_* error codes if an error is found

int32_t mxfMuxChkSettings(void *(*get_rc)(char *name), const struct mxf_m_settings *set, uint32_t options, void *app);

#ifdef __cplusplus
}
#endif


#endif //#if !defined(__MC_MXF_MUX_H__)
