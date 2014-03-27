//--------------------------------------------------------------------------
// File: mpgvout.hpp
//
// Desc: MPEG video encoder API declaration
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
// -------------------------------------------------------------------------

#ifndef _MPGVOUT_HPP_
#define _MPGVOUT_HPP_

//--------------------------------------------------------------------------
#ifndef _MC_TYPES_H
#include "mctypes.h"
#endif

#include "bufstrm.hpp"
#include "mpegdef.h"

#include "mcprofile.h"

typedef struct mpeg_v_encoder mpegvenc_tt;

struct mpeg_v_exports
{
  uint32_t  flags;
  void *reserved0;
  void *reserved1;
  void *reserved2;
  void *reserved3;
  void *reserved4;
  void *reserved5;
  void *raw_fifo_ref;
  void *raw_fifo_B;
};


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct motion_data
{
  int32_t forw_hor_f_code, forw_vert_f_code; /* vector range */
  int32_t sxf, syf; /* search range */
  int32_t back_hor_f_code, back_vert_f_code;
  int32_t sxb, syb;
};

// if you change the struct mpeg_v_settings keep the struct mpeg_v_settings2 syncronized
struct mpeg_v_settings 
{
  /* coding model parameters */
  int32_t video_type;          /* type of video stream, mpeg1, mpeg2, etc */
  int32_t video_pulldown_flag; /* video is 24fps converted to 29.97fps */
  int32_t constrparms;         /* constrained parameters flag (MPEG-1 only) */
  int32_t N;                   /* number of frames in Group of Pictures */
  int32_t M;                   /* distance between I/P frames */
  int32_t drop_frame_tc;       /* use "Drop Frame" timecode notation in GOP */
  int32_t tc0;                 /* timecode of first frame */
  int32_t ClosedGOP_Interval;  /* how oft to generate closed GOP, 0=disabled*/
  int32_t deinterlacing_mode;  /* one of the DEINTERLACE_ values */
  int32_t ignore_frame_interval; /* set to 1000 to ignore each 1000-th frame */
  int32_t features_flags;      /* different flags e.g. FEATURE_AUTO_GOP_PLACEMENT*/
  int32_t fieldpic;            /* use field pictures */

  /* sequence specific data (sequence header) */
  int32_t aspectratio; /* aspect ratio information (pel or display) */
  int32_t frame_rate_code; /* coded value of frame rate */
  double frame_rate; /* frames per second */
  double bit_rate; /* bits per second */
  double max_bit_rate; /* maximum bit rate for variable bitrate mode */
  int32_t mquant_limit; /* max limitation for macroblock quantization */
  int32_t vbv_buffer_size; /* size of VBV buffer (* 16 kbit) */
  int32_t constant_bitrate; /* constant bitrate flag */
  int32_t mquant_value; /* macroblock quantization value */
  int32_t write_sh; /* write a sequence header every GOP */
  int32_t write_sec; /* write a sequence end code */

  /* sequence specific data (sequence extension) */
  int32_t profile; /* syntax / parameter constraints */
  int32_t level; /* syntax / parameter constraints */
  int32_t prog_seq; /* progressive sequence */
  int32_t chroma_format;

  /* advanced gop parameters */
  int32_t min_N; /* minimum GOP length, used together with AUTO_GOP features to avoid consecutive I frames */

  /* motion data */
  struct motion_data motion_data[MAXM];

  uint32_t reserved08;
  uint32_t reserved09;
  uint32_t reserved10;

  /* sequence specific data (sequence display extension) */
  int32_t video_format; /* component, PAL, NTSC, SECAM or MAC */
  int32_t color_primaries; /* source primary chromaticity coordinates */
  int32_t transfer_characteristics; /* opto-electronic transfer char. (gamma) */
  int32_t matrix_coefficients; /* Eg,Eb,Er / Y,Cb,Cr matrix coefficients */
  int32_t display_horizontal_size; /* display size */
  int32_t display_vertical_size; /* display size */
  int32_t write_sde; /* write a sequence display extension every GOP, SVCD = 0! */

  int32_t prefiltering; /* blurs video out, useful for complex content with low bitrate (=0 -> disabled) */
                                    /* suitable values 1..15 */

  int32_t target_size_restriction; /* triggers frame target size planer (-50 .. +50 percent, 0 = no restriction) */

  int32_t user_iquant; /* use non-default (user-defined) quant. matrices */
  int32_t user_niquant;/* use non-default (user-defined) quant. matrices */

  /* picture specific data (picture coding extension) */
  int32_t dc_prec; /* DC coefficient precision for intra coded blocks */
  int32_t topfirst; /* display top field first */
  /* use only frame prediction and frame DCT (I,P,B,current) */
  int32_t frame_pred_dct_tab[3];
  int32_t conceal_tab[3]; /* use concealment motion vectors (I,P,B) */
  int32_t qscale_tab[3]; /* linear/non-linear quantizaton table */
  int32_t intravlc_tab[3]; /* intra vlc format (I,P,B,current) */
  int32_t altscan_tab[3]; /* alternate scan (I,P,B,current) */
  int32_t repeatfirst; /* repeat first field after second field */
  int32_t prog_frame; /* progressive frame */

  /* rate control vars */
  int32_t Xi;
  int32_t Xp;
  int32_t Xb;
  int32_t r;
  int32_t d0i;
  int32_t d0p;
  int32_t d0b;
  int32_t avg_act;
  int32_t fixed_vbv_delay; /* force a 0xffff vbv_delay */
  int32_t min_frame_percent; // initial VBV-fullnes in %, or (0x80000000 | vbv_fullnes_in_bits)
  int32_t pad_frame_percent;

  uint8_t intra_q[64];       /* user-defined quantization tab. for intra blocks */
  uint8_t inter_q[64];       /* user-defined quantization tab. for non-intra blocks */
  uint8_t chroma_intra_q[64];/* user-defined quantization tab. for intra blocks */
  uint8_t chroma_inter_q[64];/* user-defined quantization tab. for non-intra blocks */

  uint8_t motion_search_type;      /* type of motion search to do, one of SEARCH_* constants */
  uint32_t frame_alignement_power_two; //alignement of frame, 0..10; 0=none, 1:2, 2:4, 3:8, 4:16, 5:32, 6:64 ... 10:1024

  int32_t embed_SVCD_user_blocks; /* put blank SVCD scan offset blocks in the video stream */
  double avg_bit_rate; /* average bit rate for variable bitrate mode */
  double min_bit_rate; /* minmum bit rate for variable bitrate mode */
  int32_t calc_quality;      // calc_quality
  int32_t relaxed_standards; // not used in video encoder itself

  int32_t rate_control_mode;
  int32_t qt_noise_sensitivity;
  void *reserved19;

   /* picture display extension */
  int32_t write_pde;
  int32_t frame_centre_horizontal_offset;
  int32_t frame_centre_vertical_offset;

   /* suggested frame size for specific MPEG types */
  int32_t def_horizontal_size;
  int32_t def_vertical_size;
};

// the same as struct mpeg_v_settings, but all fields are aligned on 32/64-bit properly
// corresponds the structure struct mpeg_v_settings compiled with compiler without
// pragma pack(push,1) directive, t.e. all types (uint8, uint32, double(64-bit)) aligned
//on sizeof(type) border properly.
//
struct mpeg_v_settings2
{
  /* coding model parameters */
  int32_t video_type;          /* type of video stream, mpeg1, mpeg2, etc */
  int32_t video_pulldown_flag; /* video is 24fps converted to 29.97fps */
  int32_t constrparms;         /* constrained parameters flag (MPEG-1 only) */
  int32_t N;                   /* number of frames in Group of Pictures */
  int32_t M;                   /* distance between I/P frames */
  int32_t drop_frame_tc;       /* use "Drop Frame" timecode notation in GOP */
  int32_t tc0;                 /* timecode of first frame */
  int32_t ClosedGOP_Interval;  /* how oft to generate closed GOP, 0=disabled*/
  int32_t deinterlacing_mode;  /* one of the DEINTERLACE_ values */
  int32_t ignore_frame_interval; /* set to 1000 to ignore each 1000-th frame */
  int32_t features_flags;      /* different flags e.g. FEATURE_AUTO_GOP_PLACEMENT*/
  int32_t fieldpic;            /* use field pictures */

  /* sequence specific data (sequence header) */
  int32_t aspectratio; /* aspect ratio information (pel or display) */
  int32_t frame_rate_code; /* coded value of frame rate */
  double frame_rate; /* frames per second */
  double bit_rate; /* bits per second */
  double max_bit_rate; /* maximum bit rate for variable bitrate mode */
  int32_t mquant_limit; /* max limitation for macroblock quantization */
  int32_t vbv_buffer_size; /* size of VBV buffer (* 16 kbit) */
  int32_t constant_bitrate; /* constant bitrate flag */
  int32_t mquant_value; /* macroblock quantization value */
  int32_t write_sh; /* write a sequence header every GOP */
  int32_t write_sec; /* write a sequence end code */

  /* sequence specific data (sequence extension) */
  int32_t profile; /* syntax / parameter constraints */
  int32_t level; /* syntax / parameter constraints */
  int32_t prog_seq; /* progressive sequence */
  int32_t chroma_format;

  /* advanced gop parameters */
  int32_t min_N; /* minimum GOP length, used together with AUTO_GOP features to avoid consecutive I frames */

  /* motion data */
  struct motion_data motion_data[MAXM];

  uint32_t reserved08;
  uint32_t reserved09;
  uint32_t reserved10;

  /* sequence specific data (sequence display extension) */
  int32_t video_format; /* component, PAL, NTSC, SECAM or MAC */
  int32_t color_primaries; /* source primary chromaticity coordinates */
  int32_t transfer_characteristics; /* opto-electronic transfer char. (gamma) */
  int32_t matrix_coefficients; /* Eg,Eb,Er / Y,Cb,Cr matrix coefficients */
  int32_t display_horizontal_size; /* display size */
  int32_t display_vertical_size; /* display size */
  int32_t write_sde; /* write a sequence display extension every GOP, SVCD = 0! */

  int32_t prefiltering; /* blurs video out, useful for complex content with low bitrate (=0 -> disabled) */
                                    /* suitable values 1..15 */

  int32_t target_size_restriction; /* triggers frame target size planer (-50 .. +50 percent, 0 = no restriction) */

  int32_t user_iquant; /* use non-default (user-defined) quant. matrices, see user_quant_matrix flags below */
  int32_t user_niquant;/* use non-default (user-defined) quant. matrices, see user_quant_matrix flags below */

  /* picture specific data (picture coding extension) */
  int32_t dc_prec; /* DC coefficient precision for intra coded blocks */
  int32_t topfirst; /* display top field first */
  /* use only frame prediction and frame DCT (I,P,B,current) */
  int32_t frame_pred_dct_tab[3];
  int32_t conceal_tab[3]; /* use concealment motion vectors (I,P,B) */
  int32_t qscale_tab[3]; /* linear/non-linear quantizaton table */
  int32_t intravlc_tab[3]; /* intra vlc format (I,P,B,current) */
  int32_t altscan_tab[3]; /* alternate scan (I,P,B,current) */
  int32_t repeatfirst; /* repeat first field after second field */
  int32_t prog_frame; /* progressive frame */

  /* rate control vars */
  int32_t Xi;
  int32_t Xp;
  int32_t Xb;
  int32_t r;
  int32_t d0i;
  int32_t d0p;
  int32_t d0b;
  int32_t avg_act;
  int32_t fixed_vbv_delay; /* force a 0xffff vbv_delay */
  int32_t min_frame_percent; // initial VBV-fullnes in %, or (0x80000000 | vbv_fullnes_in_bits)
  int32_t pad_frame_percent;

  uint8_t intra_q[64];       /* user-defined quantization tab. for intra blocks */
  uint8_t inter_q[64];       /* user-defined quantization tab. for non-intra blocks */
  uint8_t chroma_intra_q[64];/* user-defined quantization tab. for intra blocks */
  uint8_t chroma_inter_q[64];/* user-defined quantization tab. for non-intra blocks */

  uint8_t motion_search_type;      /* type of motion search to do, one of SEARCH_* constants */
  uint8_t resreved_20;
  uint8_t resreved_21;
  uint8_t resreved_22;
  uint32_t frame_alignement_power_two; //alignement of frame, 0..10; 0=none, 1:2, 2:4, 3:8, 4:16, 5:32, 6:64 ... 10:1024

  int32_t embed_SVCD_user_blocks; /* put blank SVCD scan offset blocks in the video stream */
  double avg_bit_rate; /* average bit rate for variable bitrate mode */
  double min_bit_rate; /* minmum bit rate for variable bitrate mode */
  int32_t calc_quality;      // calc_quality
  int32_t relaxed_standards; // not used in video encoder itself

  int32_t rate_control_mode;
  int32_t qt_noise_sensitivity;
  int32_t reserved19;

   /* picture display extension */
  int32_t write_pde;
  int32_t frame_centre_horizontal_offset;
  int32_t frame_centre_vertical_offset;

   /* suggested frame size for specific MPEG types */
  int32_t def_horizontal_size;
  int32_t def_vertical_size;
};

void vsettings_to_vsettings2(struct mpeg_v_settings2 *set2, const struct mpeg_v_settings *set);


/* feature definitions */
/* automatically create new GOPs when appropriate, according to the source content and manage layout of GOP when necessary */
#define FEATURE_AUTO_GOP_FAST      (0x00000010L) /*< Allow P->I reassigning if appropiate (no time penality) */
#define FEATURE_AUTO_GOP_VCSD      (0x00000020L) /*< Allow GOP planning according to scene position (consume time) */
#define FEATURE_AUTO_GOP_SCD       (0x00000800L) /*< Scene change detection based on frame's histogram changing */
#define FEATURE_ALL_GOPS_SHORT     (0x00000040L) /*< All GOPs will be short, i.e. starting with I(0). */
#define FEATURE_FIRST_GOP_NOSHORT  (0x00000080L) /*< The first encoded GOP will be a full gop including the B Frames at startup */

/* use the qt_noise_sensitivity setting as a noise reduction value for the entire stream */
#define FEATURE_NR_WHOLE_STREAM    (0x00000100L)

/* use RGB16..235 instead of RGB0..255 (BGR4,BGRA) and Non-Clambed YUYV */
#define FEATURE_NON_CLAMPED_CC     (0x00000200L)

/* use video-in source if exist (some special HW accelerated builds), use new API to control the Video-Source */
#define FEATURE_VIDEO_IN           (0x00000400L)


/* make video encoding in separate thread */
#define FEATURE_ENCODING_THREAD    (0x00001000L) /*< run encoding separated from application*/
#define FEATURE_FORCE_VOBUV        (0x00010000L) /*< use VOBU-verifier on non DVD profiles*/
#define FEATURE_MT_OFF             (0x00020000L) /*< turn off multi-threaded motion search and multi-threaded encoding loop*/
#define FEATURE_STRICT_CQT         (0x00040000L) /*< do not change the quantization factors depending on MB activity */

/* default field-dominance for whole video-sequence */
/* {0,0}-undefined, {1,0} top-first, {0,1}-bottom first, {1,1}-progressive */
#define FEATURE_TOP_FIELD_FIRST    (0x00100000L)
#define FEATURE_BOT_FIELD_FIRST    (0x00200000L)

/* every frame is coded with constant size of bits (bps/fps) */
/* for IMX you also have to set N=M=1 */
#define FEATURE_FIXED_FRAME_SIZE   (0x01000000L)

/* place SMPTE 328M data in the stream */
#define FEATURE_SMPTE_328M         (0x02000000L)

/* minimize VBV activity in CBR mode, useful for transport stream muxing with tight conditions */
/* this will limit the minimum quantization values under some conditions with the aim of       */
/* never coming close to a VBV underflow                                                       */
#define FEATURE_MIN_VBV_ACTIVITY   (0x04000000L)

/* user quant matrix definitions */
#define USE_LUMA_QUANT_MATRIX      (0x00000001L)
#define USE_CHROMA_QUANT_MATRIX    (0x00000002L)

typedef struct tag_frame_v_tt
{
  uint8_t     *pSource;
  int32_t     lSrcLineSize;
  uint32_t    ulSrcWidth;
  uint32_t    ulSrcHeight;
  uint32_t    ulColorSpaceFourcc;
  uint32_t    ulOptFlags;
  void        *pUserData;                                        // user data for app, maybe used in UnlockCallback
  void        (*pUnlockCallback)(struct tag_frame_v_tt *pData);  // callback, will be called after frame is processed
} frame_v_tt, *pframe_v_tt;


#pragma pack(pop)


#define SEARCH_FASTEST      0
#define SEARCH_HALF_PEL     1
#define SEARCH_MODE_A       2
#define SEARCH_MODE_B       4
#define SEARCH_MODE_C       8
#define SEARCH_RIGOROUS     16
#define SEARCH_DIAMOND			17

#define RC_MODE_FAST        0
#define RC_MODE_1           1
#define RC_MODE_LAST        2
#define RC_MODE_128         128


// deinterlacing modes
#define DEINTERLACE_NONE            0
#define DEINTERLACE_ON              1
//////////////////////////////////////
//// future modes, not yet implemented
#define DEINTERLACE_USETOPFIELD     1
#define DEINTERLACE_USEBOTTOMFIELD  2
#define DEINTERLACE_USEBOTHFIELDS   3
//// future modes, not yet implemented
//////////////////////////////////////


// mpegOutVideoPerformance modes
#define PERF_OFFLINE    0
#define PERF_ONLINE     1
#define PERF_MOTIONAREA 2


#ifdef __cplusplus
extern "C" {
#endif

// Fill mpeg_v_settings-structure with defaults values
//

char *  mpegOutVideoDefaults(struct mpeg_v_settings *set, int video_type, int PAL);


// Change performance-related part of mpeg_v_settings-structure
//
// mode mappings:
//    PERF_ONLINE:     online mode, level:  0..31
//    PERF_OFFLINE:    offline mode, level: 0..31
//    PERF_MOTIONAREA: motion search area, level >= 0
//
void  mpegOutVideoPerformance(struct mpeg_v_settings *set, int mode, int level, int reserved);


// Create MPEG-Video-Encoder instance
//

#define INSTANCE_OPT_ACC_OFF 0x00000001

mpegvenc_tt*  mpegOutVideoNew(void *(*get_rc)(char* name), 
                              const struct mpeg_v_settings *v_settings,
                              int options, int cpu, int frame0, int nframes);

void  mpegOutVideoFree(mpegvenc_tt *instance);


// call to initialize the video encoder for an encoding session, this 
// function must be called before the PutFrame function is called
//
//  inputs:
//     instance: a pointer to a created mpegvenc_tt-object
//     options_flags: 1-UseExternalFramebuffer (I420 & YV12 only), 2-UsePreprocessing Filter
//     videobs:  a pointer to a initilized bitstream-object
//
//  outputs: none
//  return: mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not
//

#define INIT_OPT_EXTERN_FRM_BUF 0x00000001
#define INIT_OPT_NR_FILT        0x00000002
#define INIT_OPT_AUTHENT        0x00000010

#ifdef RC_2VBR
 #define INIT_OPT_VBR_TWO_PASSES  0x00000800
 #define INIT_OPT_VBR_ANALYSE     0x00000900
 #define INIT_OPT_VBR_ENCODING    0x00000F00
 #define INIT_OPT_VBR_PASSES_MASK 0x00000F00
#endif

#define INIT_OPT_VBR_EXT_STORAGE 0x00000100 //must be not disabled by RC_2VBR to keep binary compatibility

/*HACK to be able to use the decoder-header*/
#ifndef _MPGIDEC_HPP_
typedef struct
{
  uint32_t width;
  uint32_t height;
  uint32_t four_cc;
  uint32_t state;
  unsigned char *plane[4];
  int32_t stride[4];
  void (*done)(void *this_object);
  void *reserved1;
  uint32_t reserved2;
  uint32_t reserved3;
} frame_tt;
#endif


// the parameters exported about encoded frame or stream_end-state
// the structure is owned by encoder and valid during
// done() call only, application must copy the necessary
// data from this structure

typedef struct au_report_struct
{
  unsigned long support_flags;         // flag to show filled fields in this structure
  void         *application_pointer;   // not handled by encoder (stored only)
  unsigned long application_parameter; // not handled by encoder (stored only)
//(1)
  unsigned long vbv_state; // state of vbv-buffer, bits
  long video_in_frame; // size of video-payload, bits
  long bits_in_frame;  // size of frame in stream (headers + video + padding + etc.)
  int  pict_type;       // 1:I, 2:P, 3:B
  int  temp_ref, temp_ref_I; //temporal reference and temporal reference of I-frame in this GOP
  long c_frame_nr, c_frame_nr_I; //frame number in coded video, last I-frame number in coded video
//(2)
// can be used to create quality-bar, worstness = avg_quant*enc_comp;
// suggest to use (scala 0..100) 100/(1+log(avg_quant*enc_comp))
  int avg_quant; // 1..62(MPEG1,MPEG2) or 112(MPEG2)  average quantization, depends on
                 // the bit rate and content of the video-stream
  int avg_comp;  // 1..2048 average frame-complexity (usually near 350)
  int enc_comp;  // 1..2048 average misprediction-complexity (usually: P:150, B:60)
  int avg_quality; // 0..48 average estimated quality (dB, relative to frame-complexity)

} au_report_tt;


/*
#define DONE(p) if( p->done ) { p->done(p); p = NULL; } else { p->state |= STATE_DONE; }
*/

#define INIT_OPT_START_PAR        0x00002000
struct opt_startup_par
{
  long reserved0;
  long reserved1;
  long temp_ref_0; // temp_ref of frame0-frame (i.e. 0-means start new GOP)
  long temp_ref_I; // temp_ref of I-frame in continued GOP
  unsigned long vbv_state;
  long reserved[3];
  frame_tt reference_frame;
};

#define INIT_OPT_CHAPTER_LIST    0x00001000
struct chapter_position
{
  long frame_nr;
  long flags;
};

#define CHAPTER_OPEN_GOP       (0x00000001L)
#define CHAPTER_SHORT_GOP      (0x00000002L)
#define CHAPTER_FRAME_NR       (0x00000004L) // change the timecode at this point of the stream to the timecode corresponding to the given frame_nr
#define CHAPTER_FRAME_NR_MASK  (0xFFFFFF00L)
#define CHAPTER_REARRANGE_GOPS (0x00000010L) // rearrange preceding GOPs for getting gop length as equal as possible
#define CHAPTER_END_OF_LIST    (~0L)



// user data options
#define INIT_OPT_UD_FIRST_SEQHDR       0x00010000        // put user data after the first sequence hdr
#define INIT_OPT_UD_ALL_SEQHDR         0x00020000        // put user data after all sequence hdrs

// sr options
#define INIT_OPT_FAST_SR               0x10000000        // do fast smartrendering (SR copy), if SR is possible 
//////////////////////////////////////
//// future modes, not yet implemented
#define INIT_OPT_FAST_SR_REQUANT       0x20000000        // NOT IMPLEMENTED YET !!!
//////////////////////////////////////

#ifdef API_EXT_PARAM_LIST
// API-extension to give metadata-stream-pointer to encoder
 #ifdef __cplusplus
int  mpegOutVideoInit(mpegvenc_tt *instance, bufstream_tt *videobs, long options_flags, void **opt_ptr = 0);
 #else
int  mpegOutVideoInit(mpegvenc_tt *instance, bufstream_tt *videobs, long options_flags, void **opt_ptr);
 #endif
#else
int  mpegOutVideoInit(mpegvenc_tt *instance, bufstream_tt *videobs, long options_flags);
#endif


// call to finish encoding session, set abort non-zero if encoding is being
// aborted.
//
//  inputs: zero - finish any leftover video and clean up, else just clean up
// outputs: encoded frame if needed
//  return: mpegOutErrNone if successful, mpegOutError if not
//

int  mpegOutVideoDone(mpegvenc_tt *instance, int abort);


/* Visual Content Scene Detection (VCSD) */

/* enumeration of different detected content-types */

#define VCSD_FLAGS_NONE    0 /* no content-change was detected */
#define VCSD_FLAGS_SCENE   0x00000001 /* this frame starts a new scene (cut, probably VCSD_FLAGS_CUT)*/
#define VCSD_FLAGS_CUT     VCSD_FLAGS_SCENE /* this frame starts a new scene (cut, probably VCSD_FLAGS_CUT)*/
#define VCSD_FLAGS_FLASH   0x00000002 /* this frame has content speciality, next frame is simalar to previous frame */
/* will be more if implemented */
#define VCSD_FLAGS_MASK    0x0000000F /* mask of content-type enumeration */

#define VCSD_FLAGS_TC_CUT  0x00000100 /*this frame has timecode discontinuity, probably there is content change too */


#define VCSD_STATUS_NONE    0 /* handle as usual stream */
#define VCSD_STATUS_SC0     1 /* this place is prefferable to start GOP */
#define VCSD_STATUS_SC1     2 /* try to place GOP here */
#define VCSD_STATUS_CHAPTER 3 /* force to start new closed GOP */



//frame-Options in encoder
#define OPT_SKIP_MASK (0x0000000FL)
#define OPT_SKIP_1_B    (1)
#define OPT_SKIP_2_B    (2)
#define OPT_SKIP_ALL_B  (3)
#define OPT_SKIP_ALL_PB (4)
#define OPT_SKIP_ABORT_VBV_UNDERFLOW (13) // encoder aborts at vbv underflow
#define OPT_SKIP_ABORT (14) // encoder aborts once it needs to skip
#define OPT_SKIP_NEVER (15) // ignore any vbv issues, encode all frames

/* field-dominance for current frame */
/* {0,0}-undefined, {1,0} top-first, {0,1}-bottom first, {1,1}-progressive */
#define OPT_TOP_FIELD_FIRST    (0x00000010L)
#define OPT_BOT_FIELD_FIRST    (0x00000020L)

#define OPT_INSERT_FRAME (0x00000100L)
#define OPT_LINE_FILTER  (0x00000200L)
#define OPT_INV_AUTO_GOP (0x00000400L) /* invert of automatic GOP placement feature */


#ifdef API_EXT_PARAM_LIST
  #define OPT_EXT_PARAM_MASK      (0xFFFFF000L) //mask to all ext parameters


  #define OPT_EXT_PARAM_FRM_ORG   (0x00001000L) //show original frame
  #define OPT_EXT_PARAM_FRM_ENC   (0x00002000L) //show encoded frame
  #define OPT_EXT_PARAM_SHOW_ONLY (0x00004000L) //don't encode, show only
  #define OPT_EXT_PARAM_FRM_PRC   (0x00008000L) //show preprocessed frame


  #define OPT_EXT_PARAM_CONTENT   (0x00080000L) //data about current frame-content

 
// data about current frame-content - read and cached in encoder, used only during
// PutFrame()-call
typedef struct
{
  // filled by application to request frame-level features

  // flags marking filled fields in this structure
  unsigned long flags;

  void         *application_pointer;   // not handled by encoder (stored only)
  unsigned long application_parameter; // not handled by encoder (stored only)
  // called by encoder after AccessUnit containing this frame is stored
  void        (*application_report)(au_report_tt *au_state);

  long reserved[128-6]; //reserved, must be zero'ed
} ext_param_content_tt;



/*use setup, top_field_first, bottom_field_first, progressive */
/*use setup, pulldown_3_2, pulldown_2_3, no_pulldown */
/*parameters for build-in preprocesing */
/*pointer to preprocessor */
/*
  typedef struct
  {
    unsigned long reserved0;
    unsigned long reserved1;
    unsigned long reserved2;
    unsigned long reserved3;
    unsigned long reserved4;
    unsigned long reserved5;
    unsigned long reserved6;
    unsigned long reserved7;
  } aux_data_tt;
*/

  #define OPT_EXT_PARAM_TARGET_VBV (0x01000000L) // target a specific VBV state within given frame interval
typedef struct tag_target_VBV_data
{
  unsigned long API_version;    /* Used to detect version and size of struct in future */
  unsigned long VBV_bits;       /* VBV number of bits to have withing specified number of frames */
  unsigned long frames_togo;    /* Number of frames till VBV state specified above should be reached */
} target_VBV_data_tt;


  #define OPT_EXT_PARAM_CONSTRAIN_VBV (0x02000000L) // target a specific VBV st
typedef struct tag_constrain_VBV_data
{
  unsigned long API_version;    /* Used to detect version and size of strcut in future */
  unsigned long VBV_bits;       /* VBV number of bits to have withing specified number of frames */
  unsigned long frames_togo;    /* Number of frames till VBV state spec */
  unsigned short percentage;    /* Assumed variation percentage for framesize during bitrate planning */
} constrain_VBV_data_tt;
                                                
                                                

  #define OPT_EXT_PARAM_VCSD_DATA (0x00100000L)

  
  typedef struct tag_VCSD_data
  {
    unsigned long API_version;  /* used to make API backwards compatible */
    void (*app_callback)(struct tag_VCSD_data*); /* callback to application from encoder */
    void *app_ptr;              /* from application, just cached in encoder */
    unsigned long app_info;     /* from application, just cached in encoder */
  
    unsigned long input_flags;  /* content flags known at the of frame-submit */
    unsigned long reserved0[3];
  
    unsigned long VCSD_status;  /* suggested level of handling (VCSD_STATUS_XXXX)*/
    unsigned long chapter_flags;/* the same content as used in chapter-list, if status is VCSD_STATUS_CHAPTER */
    unsigned long output_flags; /* flags set by VCSD */
    unsigned long confidence0;  /* confidence fields set by VCSD */
  
    unsigned long reserved1[4];
  } VCSD_data_tt;


  #define OPT_EXT_PARAM_STOP_DATA (0x00020000L)

typedef struct
{
  unsigned long reserved1;
  unsigned long reserved2;
  unsigned long vbv_state;
  unsigned long reserved3;
} stop_data_tt;


  #define OPT_EXT_PARAM_RAW_DATA  (0x00040000L)

typedef struct
{
  unsigned long raw_data_flags;
  unsigned long reserved2;
  unsigned char *raw_frame_data;
  unsigned long raw_frame_size;
  unsigned long vbv_state;
  unsigned long reserved3;
  unsigned long reserved4;
  unsigned long reserved5;
} raw_frame_data_tt;

  #define RAW_DATA_FLAGS_FIFO_DIRECT (0x01000000)
  #define RAW_FRAME_GUARD 1024
  #define RAW_IGNORE_FAST_SR         (0x80000000) // ignore fast sr mode for specific frame


  #define FRM_STATE_FILLED      (0x0020)
  #define FRM_STATE_DONE        (0x0100)
  #define UD_STATE_DONE         (0x0100)

  #define OPT_EXT_PARAM_UD        (0x00010000L)

#ifdef API_UD
typedef struct tag_user_data_tt
{
  unsigned long state;
  void (*done)(struct tag_user_data_tt *p);
  unsigned long udata_len;
  unsigned char *udata;  // buffer filled with user data
} user_data_tt;
#endif

#define OPT_EXT_PARAM_TIMESTAMPS (0x10000000L) //Get stream time from entry stream
  typedef struct
  {
	  int64_t rtStart;
	  int64_t rtStop;
	  int	skipped_frames;
	  unsigned int   flags;
  } sample_info_t;

#define OPT_EXT_PARAM_BITRATE (0x20000000L)
  typedef struct
  {
	  int MaxBitrate;
	  int AvgBitrate;
	  int MinBitrate;
  } bitrate_info_t;
#endif


// call to encode video
//
//  inputs: 
//    instance :a pointer to a filled in mpegOutInfo structure
//    optFlags : reserved
// 
// outputs: encoded frame
//  return: mpegOutErrNone if successful, mpegOutError or mpegOutDecline if not

#ifdef API_EXT_PARAM_LIST
// API-extension to give additional parameter-pointer to encoder
#ifdef __cplusplus
int  mpegOutVideoPutFrame(mpegvenc_tt *instance,
                          unsigned char *pbSrc,   long SrcLineSize, unsigned long SrcWidth, unsigned long SrcHeight, 
                          unsigned long ColorSpaceFourcc, unsigned long optFlags, void **ext_info = 0);
 #else
int  mpegOutVideoPutFrame(mpegvenc_tt *instance,
                          unsigned char *pbSrc,   long SrcLineSize, unsigned long SrcWidth, unsigned long SrcHeight, 
                          unsigned long ColorSpaceFourcc, unsigned long optFlags, void **ext_info);
 #endif
#else
int  mpegOutVideoPutFrame(mpegvenc_tt *instance,
                          unsigned char *pbSrc,   long SrcLineSize, unsigned long SrcWidth, unsigned long SrcHeight, 
                          unsigned long ColorSpaceFourcc, unsigned long optFlags);
#endif


// the same as mpegOutVideoPutFrame, but with built-in RGB4 colorspace
//

int  mpegOutVideoPut(mpegvenc_tt *instance, unsigned char *videoBuffer);



// get maximal bitrate achieved on encoding session
//

int  mpegOutVideoGetMaxBitrate(mpegvenc_tt *instance);



// call to encode video
// outputs: encoded frame

#ifdef API_EXT_PARAM_LIST
// API-extension to give additional parameter-pointer to encoder
#ifdef __cplusplus
int  mpegOutVideoPutFrameV(mpegvenc_tt *instance, pframe_v_tt pframe_v, void **ext_info = 0);
 #else
int  mpegOutVideoPutFrameV(mpegvenc_tt *instance, pframe_v_tt pframe_v, void **ext_info);
 #endif
#else
int  mpegOutVideoPutFrameV(mpegvenc_tt *instance, pframe_v_tt pframe_v);
#endif



// call to get the settings errors/warnings in a mpeg_v_settings structure
// use the get_rc callback to provide an err_printf callback to get error messages
// that can be localized
//

int mpegOutVideoChkSettings(void *(*get_rc)(char *name), const struct mpeg_v_settings *set, unsigned int options, void *app);



#ifdef __cplusplus
}
#endif

#endif  /* _MPGVOUT_HPP_ */

