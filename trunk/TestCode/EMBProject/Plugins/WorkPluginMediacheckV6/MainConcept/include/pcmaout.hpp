//--------------------------------------------------------------------------
// File: pcmaout.hpp
//
// Desc: PCM audio encoder API declaration
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _pcmaout_h_
#define _pcmaout_h_

#include "bufstrm.hpp"
#include "mpegdef.h"
#include "mcprofile.h"

typedef struct pcm_a_encoder pcmaenc_tt;  // audio encoder object

// use these with the option parameter to the pcmOutAudioNew function

// if the audio samples being input to the encoder are already 
// byte swapped (LPCM) samples instead of normal PCM samples.
#define LPCM_SAMPLES      0x00000001  // for compatiblity
#define OPT_LPCM_SAMPLES  0x00000001


// this should be the same size as the mpeg_a_settings structure

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct pcm_a_settings 
{
  int pcm_channels;               // one of the PCM_?CHANNEL defines in mpegdef.h
  int audio_layer;                // to overlay with mpeg audio layer field
  int pcm_emphasis;               // 0 - off, 1 - on, on only allowed for 48 kHz audio
  int pcm_mute_flag;              // 0 - off, 1 - on
  int pcm_quantization;           // one of the PCM_??BITS defines in mpegdef.h
  int pcm_dynamic_range_control;  // 8 bit value, 0x80 (the default) = no range control (gain)
                                  // X = bits 7..5, Y = bits 4..0 where
                                  // gain (dB) = 24.082 - 6.0206*X - 0.2007*Y
  int reserved[3];                
};
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// audio routines in pcmaout.dll
/////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// call to fill an pcm_a_settings-structure with defaults values
// based on one of the MPEG preset values
// 
//  inputs:
//    set - pointer to an pcm_a_settings structure
//    profileID - one of the MCPROFILE_* or MPEG_* constants
// output:
//    modified pcm_a_settings structure
//  return:
//    profile description if valid or NULL else

char *pcmOutAudioDefaults(struct pcm_a_settings *set, int profileID);


// call to create an pcm audio encoder object
//
//  inputs:
//    get_rc - pointer to a get resource function 
//    set - pointer to a filled in pcm_a_settings structure
//    options - one or more of the OPT_ defines above
//    frameRateCode - one of FRAMERATE defines in mpegdef.h, required for
//                    AES3_???_AUDIO, not used for PCM_AUDIO
//    sampleRate - sample rate of the input audio, must be 48000 or 96000
// outputs:
//    none
//  return:
//    pointer to a pcmaenc_tt object if succesful
//    NULL if unsuccesful
        
pcmaenc_tt* pcmOutAudioNew(
    void *(*get_rc)(char* name), const struct pcm_a_settings *set,
    int options, int frameRateCode, int sampleRate);


// call to free an pcm audio encoder object
//
//  inputs:
//    instance - audio encoder object to free
// outputs:
//    none
//  return:
//    none

void  pcmOutAudioFree(pcmaenc_tt *instance);


// call to initialize the audio encoder for an encoding session, this 
// function must be called before the PutBytes function is called
//
//  inputs:
//    instance - pointer to an pcm audio encoder object
//    audiobs - pointer to a bufstream_tt object for the compressed data
// outputs:
//    none
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int  pcmOutAudioInit(pcmaenc_tt *instance, bufstream_tt *audiobs);


// call to process some audio samples
//
//  inputs:
//    instance - pointer to an pcm audio encode object
//    audioBuffer - pointer to a buffer of PCM audio samples
//    numAudioBytes - number of bytes of data in the audio buffer  
// outputs:
//    encoded samples to the audiobs object
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int  pcmOutAudioPutBytes(pcmaenc_tt *instance, unsigned char *audioBuffer, unsigned long numAudioBytes);


// call to finish encoding session, set abort non-zero if encoding is being
// aborted.
//
//  inputs:
//    instance - pointer to an pcm audio encoder object
//    abort - set to 0 to finish any leftover audio and clean up,
//            else just clean up
// outputs:
//    encoded samples to the audiobs object if needed
//  return:
//    mpegOutErrNone if successful
//    mpegOutError if not

int  pcmOutAudioDone(pcmaenc_tt *instance, int abort);


// call to get the setting errors/warnings in a pcm_a_settings structure
// use the get_rc callback to provide an err_printf callback to get error
// messages that can be localized
//
//  inputs:
//    get_rc - pointer to a get_rc function
//    set - pointer to an pcm_a_settings structure
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

int pcmOutAudioChkSettings(void *(*get_rc)(char *name), const struct pcm_a_settings *set, int mpeg_type, int sample_rate, unsigned int options, void *app);




#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------

#endif // _pcmaout_h_
