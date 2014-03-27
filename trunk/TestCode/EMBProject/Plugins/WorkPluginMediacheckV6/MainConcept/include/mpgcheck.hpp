//--------------------------------------------------------------------------
// File: mpgcheck.hpp
//
// Desc: MPEG settings conformance check library include file
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _MPGCHECK_H_
#define _MPGCHECK_H_

#include "mpegdef.h"
#include "mpgaout.hpp"
#include "mpgvout.hpp"
#include "mpgmux.hpp"
#include "pcmaout.hpp"

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

/* combined settings structure, this structure contains all of the */
/* video, audio and multiplexer settings                           */
struct mpeg_all_settings
{
  union
  {
    struct mpeg_v_settings v_settings;
    struct
    {
      /* coding model parameters */
      int video_type; /* type of video stream, mpeg1, mpeg2, etc */
      int video_pulldown_flag; /* video is 24fps converted to 29.97fps */
      int constrparms;  /* constrained parameters flag (MPEG-1 only) */
      int N; /* number of frames in Group of Pictures */
      int M; /* distance between I/P frames */
      int drop_frame_tc; /* do drop frame timecode in GOP */
      int tc0; /* timecode of first frame */
      int ClosedGOP_Interval; /* Generate closed GOP 0=disabled */
      int deinterlacing_mode; /* one of the DEINTERLACE_ values */
      int ignore_frame_interval; /* set to 1000 to ignore each 1000-th frame */
      int features_flags; /* different flags e.g. FEATURE_AUTO_GOP_PLACEMENT*/
      int fieldpic; /* use field pictures */

      /* sequence specific data (sequence header) */
      int aspectratio; /* aspect ratio information (pel or display) */
      int frame_rate_code; /* coded value of frame rate */
      double frame_rate; /* frames per second */
      double bit_rate; /* bits per second */
      double max_bit_rate; /* maximum bit rate for variable bitrate mode */
      int reserved6; /* automatically compute bitrate based on res and frame rate */
      int vbv_buffer_size; /* size of VBV buffer (* 16 kbit) */
      int constant_bitrate; /* constant bitrate flag */
      int mquant_value; /* macroblock quantization value */
      int write_sh; /* write a sequence header every GOP */
      int write_sec; /* write a sequence end code */

      /* sequence specific data (sequence extension) */
      int profile, level; /* syntax / parameter constraints */
      int prog_seq; /* progressive sequence */
      int chroma_format;
      int reserved7; /* no B pictures, skipped pictures */

      /* motion data */
      struct motion_data motion_data[MAXM];
      int xmotion, ymotion; /* x/y motion from frame to frame in pixels */
      int automotion; /* automatically compute motion vectors based on xmotion, ymotion */

      /* sequence specific data (sequence display extension) */
      int video_format; /* component, PAL, NTSC, SECAM or MAC */
      int color_primaries; /* source primary chromaticity coordinates */
      int transfer_characteristics; /* opto-electronic transfer char. (gamma) */
      int matrix_coefficients; /* Eg,Eb,Er / Y,Cb,Cr matrix coefficients */
      int display_horizontal_size, display_vertical_size; /* display size */
      int write_sde; /* write a sequence display extension every GOP, SVCD = 0! */

      /* picture specific data (picture coding extension) */
      int reserved11, reserved12; /* motion vector ranges */
      int user_iquant, user_niquant;/* use non-default (user-defined) quant. matrices */
      int dc_prec; /* DC coefficient precision for intra coded blocks */
      int topfirst; /* display top field first */
      /* use only frame prediction and frame DCT (I,P,B,current) */
      int frame_pred_dct_tab[3];
      int conceal_tab[3]; /* use concealment motion vectors (I,P,B) */
      int qscale_tab[3]; /* linear/non-linear quantizaton table */
      int intravlc_tab[3]; /* intra vlc format (I,P,B,current) */
      int altscan_tab[3]; /* alternate scan (I,P,B,current) */
      int repeatfirst; /* repeat first field after second field */
      int prog_frame; /* progressive frame */

      /* rate control vars */
      int Xi;
      int Xp;
      int Xb;
      int r;
      int d0i;
      int d0p;
      int d0b;
      int avg_act;
      int fixed_vbv_delay; /* force a 0xffff vbv_delay */
      int min_frame_percent;
      int pad_frame_percent;

      unsigned char intra_q[64];       /* user-defined quantization tab. for intra blocks */
      unsigned char inter_q[64];       /* user-defined quantization tab. for non-intra blocks */
      unsigned char chroma_intra_q[64];/* user-defined quantization tab. for intra blocks */
      unsigned char chroma_inter_q[64];/* user-defined quantization tab. for non-intra blocks */

      char motion_search_type;      /* type of motion search to do, one of SEARCH_* constants */
      int reserved16; /* do black and white, i.e. do no chromanance */
      int embed_SVCD_user_blocks; /* put blank SVCD scan offset blocks in the video stream */
      double avg_bit_rate; /* average bit rate for variable bitrate mode */
      double min_bit_rate; /* minmum bit rate for variable bitrate mode */
      int reserved17; 
      int relaxed_standards;

      int rate_control_mode;
      int qt_noise_sensitivity;
      int *reserved19;

      /* picture display extension */
      int write_pde;
      int frame_centre_horizontal_offset;
      int frame_centre_vertical_offset;

      /* suggested frame size for specific MPEG types */
      int def_horizontal_size;
      int def_vertical_size;
#ifdef __vxworks
    } vsettings;
#else
    } ;
#endif
  };

  union
  {
    struct mpeg_a_settings a_settings;
    struct
    {
      int audio_mode;
      int audio_layer;
      int psych_model;
      int audio_bitrate;
      int emphasis;
      int extension;
      int error_protection;
      int copyright;
      int original;
#ifdef __vxworks
    } mpegasettings;
#else
    };
#endif
    struct pcm_a_settings p_settings;
    struct
    {
      int pcm_channels;
      int pcm_audio_layer;
      int pcm_emphasis;               
      int pcm_mute_flag;              
      int pcm_quantization;           
      int pcm_dynamic_range_control;  
      int reserved[3];                
#ifdef __vxworks
    } pcmasettings;
#else
    } ;
#endif
  };

  union
  {
    struct mpeg_m_settings m_settings;
    struct
    {
      unsigned long sectors_delay;
      unsigned long video_delay_ms;
      unsigned long audio1_delay_ms;
      unsigned long audio2_delay_ms;
      unsigned long sector_size;
      unsigned long packets_per_pack;
      unsigned long audio1_buffer_size;
      unsigned long audio2_buffer_size;
      unsigned long video_buffer_size;
      int extra_padding_packs; // extra padding packets per second for transport streams only
      int user_mux_rate_bps; /* force a particular mux rate in bps */
      int mplex_type; /* type of program stream, mpeg1, mpeg2, etc */
      int mplex_pulldown_flag; /* video stream is 24fps converted to 29.97fps */
      int vcd_audio_pad; /* pad each vcd audio sector with zeros */
      int align_sequence_headers; /* align sequence headers to a sector */
      int user_mux_rate; /* force a particular mux rate in 50 byte/sec units */
      int put_private2; /* put private stream 2's instead of padding packets */
      int frame_timestamps; /* which frames to timestamp */
      int VBR_multiplex; /* do VBR style multiplexing */
      int write_pec; /* write a program end code */
      int mux_SVCD_scan_offsets; /* compute SVCD scan offsets */
      unsigned int max_file_size; /* split file every so many MB */
      unsigned int feature_flags; /* one or more of the MUX_FEATURE_* defines */
      int reserved3;
      int reset_clocks; /* reset the clocks in split files */
      int write_end_codes; /* write program end codes in split files */
      int set_broken_link; /* set the broken link flag in the first GOP of split files */
#ifdef __vxworks
    } muxsettings;
#else
    } ;
#endif
  };
  int online;
  int perf_level;
  int audio_frequency;
  int do_not_use;
};

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

// these check routines check a mpeg_all_settings structure or the
// mpeg_?_settings structures for MPEG and VCD/SVCD/DVD conformance
// problems. They return one of the INV_* error codes defined in
// mpegdef.h if a problem is found. 
//  
// In addition to checking the individual audio/video/muxer parts
// of these structures, an addition check (a cross check) is made to
// see if the audio/video/muxer settings will work together. This 
// cross check can also be called separately if needed.


// call to check the MPEG and VCD/SVCD/DVD conformance of a
// mpeg_all_settings structure, this also does a cross check
// of the audio/video/muxer settings.
// use with the get_rc callback to provide a err_printf callback to get
// error messages that can be localized
// returns one of the INV_* defines in mpegdef.h if a problem
// is found, mpegOutError if the parameter is invalid or
// mpegOutErrNone if no errors are found

int mpegChkAllSettings(void *(*get_rc)(char *name), const struct mpeg_all_settings *mpegSettings, unsigned int options, void *app);


// call to only do the audio/video/muxer cross checks of a
// mpeg_all_settings structure
// use with the get_rc callback to provide a err_printf callback to get
// error messages that can be localized
// returns one of the INV_* defines in mpegdef.h if a problem
// is found, mpegOutError if the parameter is invalid or
// mpegOutErrNone if no errors are found

int mpegChkAllCrossCheck(void *(*get_rc)(char *name), const struct mpeg_all_settings *mpegSettings, unsigned int options, void *app);



#ifdef __cplusplus
}
#endif

#endif // _MPGCHECK_H_

