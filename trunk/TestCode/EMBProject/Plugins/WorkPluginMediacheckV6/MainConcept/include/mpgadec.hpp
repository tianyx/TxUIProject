//--------------------------------------------------------------------------
// File: mpgadec.hpp
//
// Desc: audio decoder API header
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _MPGADEC_HPP_
#define _MPGADEC_HPP_

#include "bufstrm.hpp"
#include "mpgidec.hpp"
#include "mpegdef.h"

#ifdef __cplusplus
#define MPEGDEC_API extern "C" 
#else
#define MPEGDEC_API extern 
#endif


/////////////////////////////////////////////////
// wrapper functions for streaming interface
typedef struct mpeg_a_decoder mpegadec_tt;

MPEGDEC_API mpegadec_tt *new_audio_decoder(void);
MPEGDEC_API int32_t free_audio_decoder(mpegadec_tt *a_dec);
MPEGDEC_API int32_t init_audio_decoder(mpegadec_tt *a_dec, uint8_t *src, uint32_t src_size);
MPEGDEC_API uint32_t get_audio_samples(mpegadec_tt *a_dec, uint8_t *dst, int32_t *dst_size, uint8_t *src, int32_t src_size);
MPEGDEC_API uint32_t get_audio_state(mpegadec_tt *a_dec);
MPEGDEC_API struct aud_frame_hdr *get_audio_header(mpegadec_tt *a_dec);
MPEGDEC_API void reset_audio_decoder(mpegadec_tt *a_dec);

// wrapper functions for streaming interface
/////////////////////////////////////////////////


/////////////////////////////////////////////////
// open MPEG audio stream, returns a bufstream interface
MPEGDEC_API bufstream_tt *open_MPEGin_Audio_stream(void);
MPEGDEC_API bufstream_tt *open_MPEGin_Audio_stream_ex(void *(*get_rc)(char* name), int32_t reserved1, int32_t reserved2);

#endif /* _MPGADEC_HPP_ */
