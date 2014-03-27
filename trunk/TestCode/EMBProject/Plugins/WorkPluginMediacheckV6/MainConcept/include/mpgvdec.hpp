//--------------------------------------------------------------------------
// File: mpgvdec.hpp
//
// Desc: video decoder API header
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------
 
#ifndef _MPGVDEC_HPP_
#define _MPGVDEC_HPP_


#include "bufstrm.hpp"
#include "mpegdef.h"
#include "mpgidec.hpp"

#ifdef __cplusplus
extern "C" {
#endif


/*
  sample usage of streaming MPEG-decoder

  int n;
  struct SEQ_Params *pSEQ;
  struct PIC_Params *pPIC;
  frame_tt output_frame;
  unsigned long state;

  
  vs = open_MPEGin_Video_stream();
  vs->auxinfo(vs,0,PARSE_SEQ_HDR,NULL,0);
  n = vs->copybytes(chunk_ptr,chunk_size);
  if(n==0)
  { 
    MPF(DBG_ERR,("Could not find sequence header"));
    exit_on_error();
  }
  vs->auxinfo(vs,0,GET_SEQ_PARAMSP,&pSEQ,sizeof(pSEQ));
  
  // here:
  // check header parameters, initialize play/framebuffer, etc.
  
  if(vs->auxinfo(vs,0,INIT_FRAME_PARSER,NULL,0))
  {
    MPF(DBG_ERR,("Could not find initialize frame parser"));
    exit_on_error();
  };
  
  // here:
  // Create framebuffer and fill frame_tt structure with output-buffer
  // information

  vs->auxinfo(vs,skip_frame_code,PARSE_FRAMES,&output_frame,sizeof(output_frame));

  state=0;

put_chunks_into_parser:

  // here:
  // put next stream-chunk into parser
  // we need to repeat this step to put all frame-data into parser

  n=vs->copybytes(chunk_ptr, chunk_size);

  // n - how many bytes from the chunk were used. It is possible (and normal)
  // to obtain 0 here: it means that no bytes from this chunk were used to
  // finish frame-decode. One need to call copybytes with this chunk to
  // start next frame decode.
  
  state=vs->auxinfo(vs,0,GET_PARSE_STATE,NULL,0);

  if(state | PARSE_DONE_FLAG)
  {
    //end of frame achieved
    if(state | PIC_VALID_FLAG)
    {
      vs->auxinfo(vs,0,GET_PIC_PARAMSP,&pPIC,sizeof(pPIC));
      // frame_buffer is valid
      // do something with it and contine/stop stream parcing.
      // parameters from pPIC can be used to do frame reordering
    }
  }
  if(!eos)
    goto put_chunks_into_parser;

  // we have no more chunks.
  // to finish decode we need to continue on cached part of stream:

  n=vs->copybytes(NULL, 0);

  //n means (opposite to normal copybytes-call) how many bytes are still cached in
  // parser

  state=vs->auxinfo(vs,0,GET_PARSE_STATE,NULL,0);
  if(state | PARSE_DONE_FLAG)
  {
    //end of frame achieved
    if(state | PIC_VALID_FLAG)
    {
      vs->auxinfo(vs,0,GET_PIC_PARAMSP,&pPIC,sizeof(pPIC));
      // frame_buffer is valid
      // do something with it and contine/stop stream parcing.
      // parameters from pPIC can be used to do frame reordering
    }
  }

  close_bufstream(vs);
*/

#define MPEG_DEC_OPT_MT_OFF        0x00000010L     // turn off multithreading in decoder
#define MPEG_DEC_OPT_I_ONLY		   0x00000020L	   // I-only decoder

int MPEGin_decoder_version();				
bufstream_tt *open_MPEGin_Video_stream(void); //quick and dirty default initialization
bufstream_tt *open_MPEGin_Video_stream_ex(void *(*get_rc)(char* name), long options, long reserved2);

#ifdef __cplusplus
}
#endif

#endif /* _MPGVDEC_HPP_ */
