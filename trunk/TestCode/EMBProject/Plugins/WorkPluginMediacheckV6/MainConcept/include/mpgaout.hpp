//------------------------------------------------------------------------------
// File: mpgaout.hpp
//
// Desc: MPEG audio encoder API declaration
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _mpgaout_h_
#define _mpgaout_h_

#include "bufstrm.hpp"
#include "mpegdef.h"
#include "mcprofile.h"

typedef struct mpeg_a_encoder mpegaenc_tt;  // audio encoder object

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct mpeg_a_settings 
{
  int32_t audio_mode;		// 0 = stereo
							// 1 = joint stereo
							// 2 = dual channel
							// 3 = mono

  int32_t audio_layer;		// 1 = layer 1
							// 2 = layer 2

  int32_t psych_model;		// 1 = psychoacoustical model 1
							// 2 = psychoacoustical model 2

  int32_t audio_bitrate;	// 1 .. 14, a bitrate code depending on the layer
							// and the MPEG type (N/S = not supported):
							//
							//   16, 22.05 and 24 kHz = MPEG-2 LSF
							//   32, 44.1 and 48 kHz = MPEG-1
							//
							//       MPEG-1      MPEG-1      MPEG-2     MPEG-2
							//       Layer 1     Layer 2     Layer 1    Layer 2
							//  1 =   N/S         32          32         N/S
							//  2 =   N/S         48          48         N/S
							//  3 =   96          56          56         24
							//  4 =   128         64          64         32
							//  5 =   160         80          80         40
							//  6 =   192         96          96         48
							//  7 =   224         112         112        56
							//  8 =   256         128         128        64
							//  9 =   288         160         144        80
							// 10 =   320         192         160        96
							// 11 =   352         224         176        112
							// 12 =   384         256         192        128
							// 13 =   416         320         224        144
							// 14 =   448         384         256        160
							//
							// Note, the following bitrates are not supported by this
							// implementation
							//
							// MPEG-1 Layer 1:
							//
							//     2 channels - 96 .. 160 kbps
							//
							// MPEG-1 Layer 2:
							//
							//     1  channel - 224 .. 384 kbps
							//     2 channels -  32 ..  56 and 80 kbps
							//
							// MPEG-2 Layer 1:
							//
							//     2 channels - 32 .. 56 kbps
							//
							// MPEG-2 Layer 2:
							//
							//     2 channels - 24 .. 40 kbps

  int32_t emphasis;			// 0 = none
							// 1 = 50/15 microseconds
							// 3 = CCITT J.17

  int32_t extension;		// this is really the private bit
							// 0 = private bit not set
							// 1 = private bit set

  int32_t error_protection;	// 0 = error protection disabled (no CRC)
							// 1 = error protection enabled (CRC)

  int32_t copyright;		// 0 = copyright bit not set
							// 1 = copyright bit set

  int32_t original;			// 0 = original bit not set
							// 1 = original bit set
};

#pragma pack(pop)


/////////////////////////////////////////////////////////////////////////////
// audio routines in mpgaout.dll
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// call to fill an mpeg_a_settings-structure with defaults values
// based on one of the MPEG preset values
// 
//  inputs:
//    set - pointer to an mpeg_a_settings structure
//    profileID - one of the MCPROFILE_* or MPEG_* constants
// output:
//    modified mpeg_a_settings structure
//  return:
//    profile description if valid or NULL else

char *mpegOutAudioDefaults(struct mpeg_a_settings *set, int32_t profileID);


// call to create an mpeg audio encoder object
//
//  inputs:
//    get_rc - pointer to a get resource function 
//    set - pointer to a filled in mpeg_a_settings structure
//    options - reserved, set to 0
//    reserved - set to 0
//    sampleRate - sample rate of the input audio
//                 must be 16000, 22050, 24000, 32000, 44100 or 48000
// outputs:
//    none
//  return:
//    pointer to a mpegaenc_tt object if succesful
//    NULL if unsuccesful
        
mpegaenc_tt* mpegOutAudioNew(void *(*get_rc)(char* name), const struct mpeg_a_settings *set,
								int32_t options, int32_t reserved, int32_t sampleRate);


// call to free an mpeg audio encoder object
//
//  inputs:
//    a_enc - audio encoder object to free
// outputs:
//    none
//  return:
//    none

void  mpegOutAudioFree(mpegaenc_tt *a_enc);


// call to initialize the audio encoder for an encoding session, this 
// function must be called before the PutBytes function is called
//
//  inputs:
//    a_enc - pointer to an mpeg audio encoder object
//    audiobs - pointer to a bufstream_tt object for the compressed data
// outputs:
//    none
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t mpegOutAudioInit(mpegaenc_tt *a_enc, bufstream_tt *audiobs);


// call to encode some audio samples
//
//  inputs:
//    a_enc - pointer to an mpeg audio encode object
//    audioBuffer - pointer to a buffer of PCM audio samples
//    numAudioBytes - number of bytes of data in the audio buffer  
// outputs:
//    encoded samples to the audiobs object
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t mpegOutAudioPutBytes(mpegaenc_tt *a_enc, uint8_t *audioBuffer, uint32_t numAudioBytes);


// call to finish encoding session, set abort non-zero if encoding is being
// aborted.
//
//  inputs:
//    a_enc - pointer to an mpeg audio encoder object
//    abort - set to 0 to finish any leftover audio and clean up,
//            else just clean up
// outputs:
//    encoded samples to the audiobs object if needed
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int32_t mpegOutAudioDone(mpegaenc_tt *a_enc, int32_t abort);


// call to get the setting errors/warnings in a mpeg_a_settings structure
// use with the get_rc parameter to supply an err_printf callback to get error
// messages that can be localized
//
//  inputs:
//    get_rc - pointer to a get_rc function 
//    set - pointer to an mpeg_a_settings structure
//    mpeg_type - one of the MPEG_* constants to check for VCD/SVCD/DVD
//                audio conformance, use 0 for no VCD/SVCD/DVD conformance checks
//    sample_rate - sampling rate to check
//    options - check options, one or more of the CHECK_* defines in mpegdef.h
//    app - reserved
// outputs:
//    none
//  return:
//    mpegOutErrNone if no errors found
//    one of the INV_* error codes if an error is found

int32_t mpegOutAudioChkSettings(void *(*get_rc)(char *name), const struct mpeg_a_settings *set, int32_t mpeg_type, int32_t sample_rate, uint32_t options, void *app);



#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------

#endif // _mpgaout_h_
