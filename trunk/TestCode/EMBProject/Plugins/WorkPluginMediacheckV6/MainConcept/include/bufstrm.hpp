 /********************************************************************
 Created: 2001/05/14 
 File name: bufstrm.hpp
 Purpose: Buffered stream base header

 Copyright (c) 2001-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef _BUFSTRM_HPP_
#define _BUFSTRM_HPP_

#include "mctypes.h"

// used when dll exports symbols to project with different function call type
#if defined(_WIN32)
#define MC_EXPORT_API	__cdecl
#else
#define MC_EXPORT_API
#endif

// comment this out to use non-UNICODE functions
// in a UNICODE project. Meant for Windows platforms
// if commented out, leave _BS_MAX_PATH defined as 2048
#ifdef _UNICODE
#define _BS_UNICODE
#define _BS_MAX_PATH  32768
#else
#define _BS_MAX_PATH  2048
#endif

#define BS_OK       (0)
#define BS_IO_ERROR (1)
#define BS_ERROR    (0xFFFFFFFF)


#define BS_FLAGS_DST 0x00000001 // owns AuxInfo channel
#define BS_FLAGS_ACT 0x00000002 // owns Drive channel
// i.e. part calling HT-channel to drive information


typedef struct bufstream bufstream_tt;

struct bufstream
{

// how many bytes can be put_into/read_from buffer
// used if "request"-call fails
// fifo_w_empty/fifo_r_filled
  uint32_t  (MC_EXPORT_API * usable_bytes)(bufstream_tt *bs);

//request the buffer with at least numbytes-bytes
// fifo_w_sampbuf/fifo_r_sampbuf
  uint8_t * (MC_EXPORT_API * request)(bufstream_tt *bs, uint32_t numbytes);

//confirm numbytes-bytes filled in in requested after last "request"-call
// fifo_w_commit/fifo_r_remove
  uint32_t  (MC_EXPORT_API * confirm)(bufstream_tt *bs, uint32_t numbytes);

//put/get numbytes-bytes into/from bufsteam
// fifo_w_sampput/fifo_r_sampget
  uint32_t  (MC_EXPORT_API * copybytes)(bufstream_tt *bs, uint8_t *ptr, uint32_t numSamples);

// maximal chunk-size in buffer-mode (i.e. for "request"-call)
  uint32_t  (MC_EXPORT_API * chunksize)(bufstream_tt *bs);
  

//inform bufstream about some additional info - for example
// to inform MUXer about encoding-units
//
// offs: offset from last request-call (if called after
// confirm/copybyte means next copybyte-chunk, i.e.
// we need to call auxinfo() before copybyte() or confirm()
// if offs != 0). AcessUnit-auxinfo is alway called with offs=0
// after confirm()/copybyte()-call!
// Other aux-info can be putted earlier then stream arrive.
//
// Can one asssume that area 0...offs-1 (after last request-call)
// is filled with valid part of stream? Do we need it?
//
  uint32_t (MC_EXPORT_API * auxinfo)(bufstream_tt *bs, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);

// allow to split output-stream {if(split!=NULL) - splittable}
// return 0 = OK
  uint32_t (MC_EXPORT_API * split) (bufstream_tt *bs);
  void  (MC_EXPORT_API * done)  (bufstream_tt *bs, int32_t Abort);
  void  (MC_EXPORT_API * free)  (bufstream_tt *bs);

// used to drive io-pipe, undefined = NULL, currently unused
  struct drive_struct *drive_ptr;
  uint32_t  (MC_EXPORT_API * drive) (bufstream_tt *bs, uint32_t info_ID, void *info_ptr, uint32_t info_size);

  uint32_t flags; //currently unused, setted once on create/initialisation time
  uint32_t (MC_EXPORT_API * state) (bufstream_tt *bs); //currently unused

  struct impl_stream* Buf_IO_struct;
};


// close_bufstream is generic done and destroy makro
//
#define close_bufstream(bs, abort) \
do { bs->done(bs, abort); bs->free(bs); bs = NULL; } while(0)
//
// bs = NULL; is very important!
//
// It force usage of close_bufstream()-macro
// on pointer-variables and produce Compiler-error
// on static/stack/part_of_struct allocated objects


#endif /* _BUFSTRM_HPP_ */
