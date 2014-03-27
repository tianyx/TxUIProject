//------------------------------------------------------------------------------
// File: sr_wrap.hpp
//
// Desc: smartrender wrapper header
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _SRWRAP_H_
#define _SRWRAP_H_

#ifndef API_EXT_PARAM_LIST
  #define API_EXT_PARAM_LIST
#endif

#include "mpgvout.hpp"
#include "mpgaout.hpp"
#include "mpegin.h"

// additional return codes for smart rendering. They are returned by the 
// srWrapperInputCheck and srWrapperInputAttach functions

// these will be or'd together if both video and audio SR is available
// they will not be set if one of the corresponding bits below is set
#define mpegOutVidSR           0x00000010   // video can be smart rendered
#define mpegOutAudSR           0x00000020   // audio can be smart rendered
#define mpegOutVidSRRequant    0x00000040   // video will be requantized (bitrates do not match)

// these may be or'd together to indicate why the video or audio
// cannot be smart rendered
#define mpegOutVidError        0x00000100  // video error flag   
#define mpegOutVidNoVid        0x00000300  // no video available
#define mpegOutVidHSize        0x00000500  // horizontal sizes do not match
#define mpegOutVidVSize        0x00000900  // vertical sizes do not match
#define mpegOutVidM            0x00001100  // MPEG M's do not match
#define mpegOutVidN            0x00002100  // MPEG N's do not match
#define mpegOutVidType         0x00004100  // MPEG type (1 or 2) do not match
#define mpegOutVidBitrateType  0x00008100  // the bitrate types (constant/VBR) do not match
#define mpegOutVidBitrate      0x00010100  // the bitrates do not match
#define mpegOutVidFieldOrder   0x00020100  // the progressive frame or field order do not match
#define mpegOutVidFrameRate    0x00040100  // the frame rate does not match
#define mpegOutChromaFormat    0x00080100  // chroma format does not match

#define mpegOutAudError        0x00100000  // audio error flag   
#define mpegOutAudNoAud        0x00300000  // no audio avaliable
#define mpegOutAudLayer        0x00500000  // layers do not match
#define mpegOutAudFreq         0x00900000  // frequencies do not match
#define mpegOutAudBitrate      0x01100000  // bitrates do not match
#define mpegOutAudCRC          0x02100000  // CRC's (enabled/disabled) do not match
#define mpegOutAudMode         0x04100000  // mode's (stereo, mono, etc.) do not match


// smartrender wrapper object
typedef struct sr_wrapper srwrapper_tt;


#ifdef __cplusplus
extern "C" {
#endif

//------------------- general routines -----------------


// call to check if an input file and output settings are compatible
// for smart rendering
//
// return: mpegOutErrNone if both video and audio smart rendering is available
//         srWrapperNoVidSR if video smart rendering is not available
//         srWrapperNoAudSR if audio smart rendering is not available
//         mpegOutError if an error occurs

int srWrapperInputCheck(struct mpeg_v_settings *v_settings, struct mpeg_a_settings *a_settings,
						struct mpegInInfo *mpgInInfo, unsigned int audioRate);


// call to inform the smart renderer of the current input file, this
// function must be called before the PutFrame functions are called with
// smart rendered frames. 
// Pass in an mpegInInfo structure that has already been opened with an
// mpegin open function
//
// return: mpegOutError if an error occurs
//         mpegOutErrNone if both video and audio smart rendering is available
//         srWrapperNoVidSR if video smart rendering is not available
//         srWrapperNoAudSR if audio smart rendering is not available

int srWrapperInputAttach(srwrapper_tt *srwrap, struct mpegInInfo *mpgInInfo);


// call to detach an input file from the smart renderer, this can also
// be done by calling the srWrapperDone function.
//
// return: none

void srWrapperInputDetach(srwrapper_tt *srwrap);


// call to finish a session, set abort non-zero if session is being
// aborted. This finishes both video and audio if available
//
//  inputs:
//    srwrap - pointer to a smartrender wrapper object
//    abort - 0 = finish any leftover video and clean up, else just clean up
// outputs:
//    encoded frame if needed
//  return:
//    mpegOutErrNone if successful, mpegOutError if not

int srWrapperDone(srwrapper_tt *srwrap, int abort);


// call to free a smartrender wrapper object, free's both video and audio
// portions of the wrapper object

void srWrapperFree(srwrapper_tt *srwrap);



//--------------- video routines ------------------------------------

// fill mpeg_v_settings-structure with default values

char *srWrapperVideoDefaults(struct mpeg_v_settings *set, int video_type, int PAL);


// check mpeg_v_settings-structure for errors

int srWrapperVideoChkSettings(void *(*get_rc)(char *name), const struct mpeg_v_settings *set, unsigned int options, void *app);


// change performance-related part of the mpeg_v_settings structure

void srWrapperVideoPerformance(struct mpeg_v_settings *set, int online, int level, int reserved);


// call to create a video smartrender wrapper object. Only video is setup for
// smart rendering with this function, the srWrapperAudioNew function
// must be called to add audio smart rendering
        
srwrapper_tt* srWrapperVideoNew(void *(*get_rc)(char* name),
	const struct mpeg_v_settings *set,
    int options, int CPU, int frame0, int nframes);


// call to initialize the smartrender wrapper for an encoding session, this 
// function must be called before the PutFrame or PutByte functions are called
// Only video is setup for smart rendering with this function, the 
// srWrapperAudioInit function must be called to add audio smart rendering
//
//  inputs:
//    srwrap - pointer to a smartrender wrapper object
//    options_flags: 1-UseExternalFramebuffer (I420 & YV12 only), 2-UsePreprocessing Filter
//    videobs:  a pointer to a initilized bitstream-object if video is being smart rendered
// outputs: none
//  return: 
//    mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not

int srWrapperVideoInit(srwrapper_tt *srwrap, bufstream_tt *videobs, long options_flags, void **opt_ptr);


// call to push a frame of video into the wrapper
//
//  inputs:
//    srwrap - pointer to a smartrender wrapper object
//    frame_num - if rendered is 1 and this frame is not from the source
//                file (i.e. this frame is being inserted between frames of
//                the source file), set this to -1
//              - else set this to the source file frame number
//    rendered  - if 1, frame in pbSrc needs to be encoded
//              - if 0, frame_num from source file is to be encoded/smartrendered
// 
// outputs:
//    encoded frame
//  return:
//    mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not
#ifdef API_EXT_PARAM_LIST
//API-extention to give additional parameter-pointer to encoder
#ifdef __cplusplus
  int  srWrapperVideoPutFrame(srwrapper_tt *srwrap, int frame_num, int rendered,
        unsigned char *pbSrc,   long SrcLineSize, unsigned long SrcWidth, unsigned long SrcHeight, 
        unsigned long ColorSpaceFourcc, unsigned long optFlags, void **ext_info=NULL);
 #else
  int  srWrapperVideoPutFrame(srwrapper_tt *srwrap, int frame_num, int rendered,
        unsigned char *pbSrc,   long SrcLineSize, unsigned long SrcWidth, unsigned long SrcHeight, 
        unsigned long ColorSpaceFourcc, unsigned long optFlags, void **ext_info);
 #endif
#else
int  srWrapperVideoPutFrame(srwrapper_tt *srwrap, int frame_num, int rendered,
        unsigned char *pbSrc,   long SrcLineSize, unsigned long SrcWidth, unsigned long SrcHeight, 
        unsigned long ColorSpaceFourcc, unsigned long optFlags);
#endif



// the same as srWrapperVideoPutFrame, but with built in FOURCC_BGR4 colorspace
//
//  inputs:
//    srwrap - pointer to a smartrender wrapper object
//    frame_num - if rendered is 1 and this frame is not from the source
//                file (i.e. this frame is being inserted between frames of
//                the source file), set this to -1
//              - else set this to the source file frame number
//    rendered  - if 1, frame in pbSrc needs to be encoded
//              - if 0, frame_num from source file is to be encoded/smartrendered
// 
// outputs:
//    encoded frame
//  return:
//    mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not

int srWrapperVideoPut(srwrapper_tt *srwrap, int frame_num, int rendered, unsigned char *videoBuffer);


// get maximum bitrate achieved during a session

int srWrapperVideoGetMaxBitrate(srwrapper_tt *srwrap);


// call to push a video frame into the wrapper
//
//  inputs:
//    srwrap - pointer to a smartrender wrapper object
//    frame_num - if rendered is 1 and this frame is not from the source
//                file (i.e. this frame is being inserted between frames of
//                the source file), set this to -1
//              - else set this to the source file frame number
//    rendered  - if 1, frame in pbSrc needs to be encoded
//              - if 0, frame_num from source file is to be encoded/smartrendered
// 
// outputs:
//    encoded frame
//  return:
//    mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not

int srWrapperVideoPutFrameV(srwrapper_tt *srwrap, int frame_num, int rendered, pframe_v_tt pframe_v);



//--------------- audio routines ------------------------------------


// call to create a smartrender audio wrapper object or to initialize
// the audio portion of a wrapper object created with srWrapperNew
//
// set srwrap = NULL if no video is being smart rendered
// pass the srwrapper_tt object created with the srWrapperNew function
// to use it so audio and video can be smart rendered together        

srwrapper_tt* srWrapperAudioNew(void *(*get_rc)(char* name),
    srwrapper_tt *srwrap,
    const struct mpeg_a_settings *m_set,
    int options, int CPU, int sampleRate);


// call to initialize the smartrender wrapper for an audio encoding session, this 
// function must be called before the PutByte function is called
//
//  inputs:
//    srwrap - pointer to a smartrender wrapper object
//    audiobs:  a pointer to a initilized bitstream-object
// outputs: none
//  return: 
//    mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not

int srWrapperAudioInit(srwrapper_tt *srwrap, bufstream_tt *audiobs);


void srWrapperAudioDefaults(struct mpeg_a_settings *set, int video_type);
int srWrapperAudioChkSettings(void *(*get_rc)(char *name), const struct mpeg_a_settings *set, int mpeg_type, int sample_rate, unsigned int options, void *app);
int srWrapperAudioPutBytes(srwrapper_tt *srwrap, unsigned int sampleNum, int numSamples, int rendered, unsigned char *audioBuffer, unsigned long numAudioBytes);


#ifdef __cplusplus
}
#endif

#endif // _srwrap_h_
