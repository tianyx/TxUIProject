 /********************************************************************
 Created: 2001/05/14 
 File name: mpgidec.hpp
 Purpose: constants and flags for decoders

 Copyright (c) 2001-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef _MPGIDEC_HPP_
#define _MPGIDEC_HPP_

#include "mctypes.h"

////////////////////////////////
// parser return code flags
enum parse_flags
{
  PARSE_DONE_FLAG   = 0x00000001, //last parse operation was OK, current stream state related
  PARSE_ERR_FLAG    = 0x00000002, //error in stream detected

  SEQ_HDR_FLAG      = 0x00000100, // next flags are related to
  EXT_CODE_FLAG     = 0x00000200, // current stream state and means parsed
  GOP_HDR_FLAG      = 0x00000400, // elements of MPEG syntax
  PIC_HDR_FLAG      = 0x00000800,
  USER_DATA_FLAG    = 0x00001000,
  SEQ_END_FLAG      = 0x00002000,
  SLICE_START_FLAG  = 0x00004000,
  UNKNOWN_CODE_FLAG = 0x00008000,
  START_CODE_FLAG   = 0x00010000,
  SEQ_EXT_HDR_FLAG	= 0x00020000,

  PIC_DECODED_FLAG  = 0x00100000, // current picture in stream decoded
  PIC_FULL_FLAG     = 0x00200000, // output picture is full decoded (seen last makroblock)
  PIC_VALID_FLAG    = 0x00400000, // output picture is decoded (at least first makroblock)
  FRAME_BUFFERED_FLAG=0x00800000, // show that there picture in reordering buffer
  PIC_ERROR_FLAG    = 0x01000000, // output picture has some errors
  PIC_MV_ERROR_FLAG = 0x02000000 // output picture has motion-vector errors (clipped in decoder)
};

// if doing reordering output picture params
// are related to reordered picture and not to current (in stream) picture

/*
FRAME_BUFFERED_FLAG means: once more frame is available.
1. first I-frame parced: VALID_FLAG is not set, FRAME_BUFFERED_FLAG is set.
2. current B-Frame:      VALID_FLAG is set,     FRAME_BUFFERED_FLAG is set.
3. last Frame: FRAME_BUFFERED_FLAG is set
4. To peek buffered frames one need to repeat 
     PARCE_FRAME-copybyte(NULL,0)-GET_STSTUS sequence
     while FRAME_BUFFERED_FLAG is set or VALID_FLAG is set
*/


////////////////////////////////
// parser auxillery commands
enum parse_aux
{
  PARSE_INIT           = 0x00010000, //clear parser-state, save-buffer etc. Call it after seeking
  PARSE_FRAMES         = 0x00010001, //set video parser to parse frame-header/data, setup output-frame
  PARSE_OPTIONS        = 0x00010002,

  PARSE_AUD_HDR        = 0x00010FFF, //set audio parser to parse sample-header/data

  GET_PARSE_STATE      = 0x00010003,
  GET_SEQ_PARAMSP      = 0x00010004,
  GET_SEQ_PARAMSPEX    = 0x00010014,
  GET_GOP_PARAMSP      = 0x00010005,
  GET_PIC_PARAMSP      = 0x00010006,
  GET_PIC              = 0x00010007, // can be sent to both the subpic and video decoders
  SET_USER_DATA        = 0x00010008,
  GET_USER_DATAP       = 0x00010009,
                                     // use these messages to get video frames from the decoder
									 // with a subpic overlayed
  GET_PIC_WITH_SP      = 0x0001000A, // send to get a frame with subpic overlayed
  PARSE_FRAMES_WITH_SP = 0x0001000B, //set video parser to parse frame-header/data, setup output-frame with subpic overlay

  PARSE_INIT_AFTER_SEEK= 0x0001000C,
  GET_DECODED_PIC_PAR  = 0x0001000D,
  GET_DECODED_PIC      = 0x0001000E,

  GET_PIC_PARAMSPEX    = 0x0001000F, // get video picture header and coding extension (MPEG2 only)

  CLEAN_PARSE_STATE    = 0x00010010, // return actual and clear parse state
  GET_PARSE_SETTINGS   = 0x00010011, // get the parser settings
  PUT_PARSE_SETTINGS   = 0x00010012, // put the parser settings 

  GET_OVERLAY          = 0x00010020, // overlay a subpic on a frame
  GET_LAST_PTS         = 0x00010021, // get the last subpic PTS available
  GET_PALETTE          = 0x00010022, // get the subpicture palette, using subpic_palette_tt below
                                     // can be sent to both the subpic and video decoders
  SET_PALETTE          = 0x00010023, // set the subpicture palette, using subpic_palette_tt below
                                     // can be sent to both the subpic and video decoders
  SET_SP_PTS           = 0x00010024, // send the PTS for the current input subpic
  SET_PIC_PTS          = 0x00010025, // set the PTS of the frame for which a subpic will be requested
                                     // with either GET_PIC or GET_OVERLAY
  INIT_FRAME_PARSER    = 0x00010202, // init parser (allocate memory etc.) to be able to decode
                                     // stream with seq-header defined parameters
 
  GET_STREAM_USER_DATA = 0x000101B2,
  PARSE_SEQ_HDR        = 0x000101B3,
//  PARSE_GOP_HDR        = 0x000101B8, //unused
//  SEARCH_PIC_HDR       = 0x00000100, //unused

  GET_SEQ_EXT_PARAMSP  = 0x000101B4,

  // extra Dolby Digital decoder commands
  DD_GET_CONFIG_INFO   = 0x00020001,
  DD_GET_GEN_INFO      = 0x00020002,
  DD_GET_MIX_INFO      = 0x00020003,
  DD_GET_PROD_INFO     = 0x00020004,
  DD_GET_TIMECODE_INFO = 0x00020005,
  DD_GET_ADDBSI_INFO   = 0x00020006,
  DD_GET_AUXDATA_INFO  = 0x00020007
};

#ifndef __APPLE__
#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif
#endif

////////////////////////////////
// video sequence header
// ISO/IEC 13818-2 section 6.2.2.1:  sequence_header
struct SEQ_Params
{
  int horizontal_size;          // horizontal size of video
  int vertical_size;            // vertical size of video
  int aspect_ratio_information;
  int frame_rate_code;
  int bit_rate_value;
  int vbv_buffer_size;
  int constrained_parameters_flag;
};

struct SEQ_ParamsEx
{
  int horizontal_size;          // horizontal size of video
  int vertical_size;            // vertical size of video
  int aspect_ratio_information;
  int frame_rate_code;
  int bit_rate_value;
  int vbv_buffer_size;
  int constrained_parameters_flag;
  int aspect_ratio_width;
  int aspect_ratio_height;
  // Those fields used if frame_rate_code == 0. frame_rate = (double)scale/(double)units.
  int scale;                    // Time scale.
  int units;                    // Number of units in tick.
};

// video sequnce extension
// ISO/IEC 13818-2 section 6.2.2.3: sequence_extension
struct SEQ_Ext_Params
{
	int profile_and_level_indication;
	int progressive_sequence;
	int chroma_format;
	int horizontal_size_extension;
	int vertical_size_extension;
	int bit_rate_extension;
	int vbv_buffer_size_extension;
	int low_delay;
	int frame_rate_extension_n;
	int frame_rate_extension_d;
};

////////////////////////////////
// gop header
struct GOP_Params
{
  int drop_flag;
  int hour;
  int minute;
  int sec;
  int frame;
  int closed_gop;
  int broken_link;
};

////////////////////////////////
// user data
typedef struct {
	long		data_size;
	uint8_t *	p_buffer;
} UserData_Info;

typedef struct {
	int						found_user_data;
	int						start_code;
	UserData_Info **	pp_params; 		
} UserData_Params;

////////////////////////////////
// video picture header
// ISO/IEC 13818-2 section 6.2.3: picture_header
struct PIC_Params
{
  int temporal_reference;
  int picture_coding_type; // picture type of last picture decoded
  int picture_type;        // picture type of current displayed frame
  int vbv_delay;
  int full_pel_forward_vector;
  int forward_f_code;
  int full_pel_backward_vector;
  int backward_f_code;
};

////////////////////////////////
// video picture header and coding extension
// ISO/IEC 13818-2 section 6.2.3 and 6.2.3.1: et seqq. */
struct PIC_ParamsEx
{
  // picture_header
  int temporal_reference;
  int picture_coding_type; // picture type of last picture decoded
  int picture_type;        // picture type of current displayed frame
  int vbv_delay;
  int full_pel_forward_vector;
  int forward_f_code;
  int full_pel_backward_vector;
  int backward_f_code;

  //  picture_coding_extension() header
  int f_code[2][2];
  int intra_dc_precision;
  int picture_structure;
  int top_field_first;
  int frame_pred_frame_dct;
  int concealment_motion_vectors;
  int q_scale_type;
  int intra_vlc_format;
  int alternate_scan;
  int repeat_first_field;
  int chroma_420_type;
  int progressive_frame;
  int composite_display_flag;
  int v_axis;
  int field_sequence;
  int sub_carrier;
  int burst_amplitude;
  int sub_carrier_phase;
	
	// picture_display_extension() header
	int number_of_frame_center_offsets; /* derived */
	int frame_center_horizontal_offset[3];
	int frame_center_vertical_offset[3];
	
	// quant matrix entension
	int load_intra_quantizer_matrix;
	int load_non_intra_quantizer_matrix;
	int load_chroma_intra_quantizer_matrix;
	int load_chroma_non_intra_quantizer_matrix;
	//  unsigned char quant_matrixes[4][64];
};


////////////////////////////////
// video frame info for parser

/* HACK */
#ifndef _MPGVOUT_HPP_
typedef struct
{
  uint32_t width;
  uint32_t height;
  uint32_t four_cc;
  uint32_t state;
  unsigned char *plane[4];
  int32_t stride[4];
  void (*done)(void *this_object);
  void *ext_cc;
  uint32_t reserved2;
  uint32_t reserved3;
  
} frame_tt;

#define FRAME_EXT_TT_VERSION 1

typedef struct
{
  uint8_t  version;
  
  frame_tt frame_data;
  
  UserData_Params * pic_user_data_params;

} frame_ext_tt;
#endif

/* external, 3-plane colorspace-converter */
#ifndef FOURCC_EXTP
#define FOURCC_EXTP ((unsigned long)'E' | ((unsigned long)'X' << 8) | \
                    ((unsigned long)'T' << 16) | ((unsigned long)'P' << 24 ))

#endif

struct ext_cc_struct
{
  int (*convert_planes)(frame_tt *frame, unsigned char *src[], long stride[], long chroma_format, long mode, unsigned long cpu);
  void *reserved1;
  void *reserved2;
  void *reserved3;
};

////////////////////////////////
// video parser frame skip types
#define SKIP_NONE      0  // decode all frame types
#define SKIP_B         1  // skip B frames
#define SKIP_PB        2  // skip P and B frames
#define SKIP_IPB       3  // skip I, P and B frames


////////////////////////////////
// video reordering flag
#define INTERN_REORDERING_FLAG		0x00000010L  // make frame reordering in the parser
#define IGNORE_BROKEN_LINK			0x00000020L  // ignore the broken link flag, only use when feeding back-to-back files to the decoder
#define H264_ASSUME_FRAME_RATE		0x00000040L  // rate in H264 bitstream is a frame rate, not a field rate
#define DEINTERLACING_FLAG			0x00000100L  // deinterlace video, bitfield bit 8..12
#define DEINTERLACING_FLAGS			0x00000F00L  // deinterlace video, bitfield bit 8..12


////////////////////////////////
// video downscaling flag	
#define DECODE_FULL_SIZE			0x00000000L  // full size decoding
#define DECODE_HALF_SIZE			0x00000001L  // horz/vert half size decoding
#define DECODE_QUARTER_SIZE			0x00000002L  // quarter size decoding
#define DECODE_HALF_TFONLY			0x00000010L  // Decode only Topfield for interlace/half
#define DECODE_HALF_BFONLY			0x00000020L	// Decode only Bottomfield for interlace/half


////////////////////////////////
// video OSD flag
#define OSD_FORMAT_FLAGS			0x0001E000L	// OSD format, bits 13..16

#define OSD_PICT_TIMECODE			0x00002000L	// Show/hide picture time code, bit 13 (1 - show, 0 - hide )
#define OSD_PICT_TYPE				0x00004000L	// Show/hide picture type, bit 14 (1 - show, 0 - hide )
#define OSD_GOP_MARKER_BIT			0x00008000L	// Show/hide GOP header marker bit, bit 15 (1 - show, 0 - hide )
#define OSD_GOP_DROP_FLAG			0x00010000L // Show/hide GOP drop flag, bit 16 (1 - show, 0 - hide )	

#define OSD_DISABLE_AUTO_SCALING	0x00020000L // Disable/enable auto OSD scaling OSD, bit 17 (disable - 1, enable - 0)
#define OSD_SCALE_MASK				0x001C0000L // Scale coefficient for OSD, bits 18..20 
#define OSD_SCALE_START_BIT			0x00000013L // First bit of scale coefficient

#define POST_PROCESSING_FLAG		0x00200000L// Disable/enable post processing, bit 21

#define LOW_LATENCY_FLAG			0x00400000L // decode with minimal output delay, bit 22
#define FLOATING_POINT_IDCT			0x00800000L // Use double precision floating point IDCT, bit 23

////////////////////////////////
// audio header

struct aud_frame_hdr
{
  unsigned long header; //unparsed header of AudioFrame
  int ID;
  int layer;
  int protection_bit;
  int bitrate_index;
  int sampling_frequency;
  int padding_bit;
  int private_bit;
  int audioMode;         // MPEG audio mode encoded in file, one of the MODE_* consts
  int mode_extension;
  int copyright;
  int original;
  int emphasis;

  unsigned int audioRate;       // audio sample rate, either 32000, 44100 or 48000
  unsigned int sampleSize;      // size in bits of samples, will be 16
  int audioBitrate;             // MPEG audio bitrate
  int bytesPerSample;           // bytes per audio sample (2 or 4)
  int audioLayer;               // MPEG audio layer encoded in file, 1 or 2
  int channels;                 // number of channels
  int samplesInFrame;
  int bytesInFrame;
};

typedef struct
{
  int syncword;
  int id;
  int layer;
  int protection_abs;
  int profile;
  int sampling_freq_idx;
  int private_bit;
  int channel_config;
  int original_copy;
  int home;
  int copyright_id_bit;
  int copyright_id_start;
  int frame_length;
  int adts_buffer_fullness;
  int num_of_rdb;
  int crc_check;
  int rdb_position[4-1];
  int crc_check_rdb;

  unsigned int sampling_frequency;
  unsigned int bits_per_sample;
  unsigned int channels;
} aac_adts_header;

////////////////////////////////
// audio buffer
typedef struct
{
  unsigned char *bfr;
  unsigned long bfr_size;
} aud_bfr_tt;


///////////////////////////////
// old ac3 audio header for compatibility

struct ac3_frame_hdr
{
  unsigned char header[7]; //unparsed header of AudioFrame
  int fscod;
  int frmsizecod;
  int acmod;

  unsigned int audioRate;       // audio sample rate in Hz
  unsigned int sampleSize;      // size in bits of samples, will be 16
  int audioBitrate;             // audio bitrate
  int bytesPerSample;           // bytes per audio sample
  int channels;                 // number of channels
  int samplesInFrame;
  int bytesInFrame;
} ;


// extra Dolby Digital decoder structures

// DD_GET_CONFIG_INFO
typedef struct
{
	unsigned char two_channel_limit;	// 0 - 5.1 channel decoder
										// 1 - 2 channel decoder

	unsigned char karaoke_support;		// 0 - unsupported
										// 1 - karaoke aware
										// 2 - karaoke capable

	unsigned char TPNP_support;			// 0 - unsupported
										// 1 - supported

	unsigned char stream_type1_support;	// 0 - unsupported
										// 1 - supported

	unsigned char stream_type2_support;	// 0 - unsupported
										// 1 - supported

	unsigned char reserved[11];

}  dd_config_info_tt;


// DD_GET_GEN_INFO
typedef struct
{
	unsigned short bsid;
	unsigned short sample_rate;
	unsigned short streamtype;
	unsigned short substreamid;
	unsigned short nblksperfrm;
	unsigned short acmod;
	unsigned short lfeon;
	unsigned short compre;
	unsigned short compr;
	unsigned short blkid;
	unsigned short frmsizcod;
	unsigned short dialnorm;
	unsigned short convsync;
	unsigned short encinfo;
	unsigned short chanmap;

	unsigned short reserved[16];

} dd_gen_info_tt;


// DD_GET_MIX_INFO
typedef struct
{
	unsigned short dmixmod;
	unsigned short cmixlev;
	unsigned short surmixlev;
	unsigned short ltrtcmixlev;
	unsigned short ltrtsurmixlev;
	unsigned short lorocmixlev;
	unsigned short lorosurmixlev;
	unsigned short pgmscl[2];
	unsigned short extpgmscl;
	unsigned short paninfo[2];
	unsigned short panmean[2];
	unsigned short lfemixlevcod;
	unsigned short premixcmpsel;
	unsigned short drcsrc;
	unsigned short premixcmpscl;
	unsigned short extpgmlscl;
	unsigned short extpgmcscl;
	unsigned short extpgmrscl;
	unsigned short extpgmlsscl;
	unsigned short extpgmrsscl;
	unsigned short extpgmlfescl;
	unsigned short dmixscl;
	unsigned short extpgmaux1scl;
	unsigned short extpgmaux2scl;

	unsigned short reserved[16];

} dd_mix_info_tt;


// DD_GET_PROD_INFO

typedef struct
{
	unsigned short audprodie;
	unsigned short roomtyp;
	unsigned short mixlevel;
	unsigned short adconvtyp;

	unsigned short reserved[16];

} dd_audprod_info_tt;


typedef struct
{
	unsigned short bsmod;
	unsigned short copyrightb;
	unsigned short origbs;
	unsigned short dsurmod;
	unsigned short dsurexmod;
	unsigned short dheadphonmod;
	unsigned short langcod[2];
	dd_audprod_info_tt audprod[2];
	unsigned short sourcefsflag;

	unsigned short reserved[16];

} dd_prod_info_tt;


// DD_GET_TIMECODE_INFO
typedef struct
{
	unsigned short mode;
	unsigned short hours;
	unsigned short minutes;
	unsigned short secs;
	unsigned short frames;
	unsigned short frame64ths;

	unsigned short reserved[16];

} dd_timecode_info_tt;


// DD_GET_ADDBSI_INFO
typedef struct
{
	unsigned short num_bytes;
	unsigned char add_bsi_data[64];

} dd_addbsi_info_tt;


// DD_GET_AUXDATA_INFO
typedef struct
{
	unsigned short num_bits;
	unsigned short p_data[1024];

} dd_auxdata_info_tt;


////////////////////////////////
// subpic frame info
//
// the subpic is an 8bpp lut
// the bottom 4 bits are the lut entry into the subpic palette
// top 4 bits are the contrast ratio k, where:
// 
//    if (k > 0)
//    {
//      k += 1
//      source pixel = ((16 - k) / 16) * source pixel + (k / 16) * subpic pixel
//    }
//    else
//      source pixel = source pixel
//
typedef struct
{
  unsigned long width;     // width of subpic
  unsigned long height;    // heigth of subpic
  unsigned char *plane;    // subpic buffer, must be large enough to handle a subpic as large as the main picture
  long stride;             // subpic stride
  // destination coordinates: 0,0 = upper left of picture
  long dest_x;             // destination x coordinate of subpic in main pic
  long dest_y;             // destination y coordinate of subpic in main pic
  unsigned long reserved0;
  unsigned long reserved1;
  unsigned long reserved2;
  unsigned long reserved3;
} sp_frame_tt;


// subpic palette info
// 16 24bpp YUV colors, 0 = reserved, 1 = Y, 2 = U, 3 = V, the DVD way!
//
typedef struct
{
  unsigned char yuv_palette[16][4];
} sp_palette_tt;


//////////////////////////////////////////////////
// video frame info for parser with subpic overlay
typedef struct
{
  unsigned long width;
  unsigned long height;
  unsigned long four_cc;
  sp_frame_tt *sp_frame; // set to non-NULL value to overlay subpic
  unsigned char *plane[4];
  long stride[4];
  unsigned long reserved0;
  unsigned long reserved1;
  unsigned long reserved2;
  unsigned long reserved3;
} frame_with_sp_tt;


#ifndef __APPLE__
#pragma pack(pop)
#endif


#endif /* _MPGIDEC_HPP_ */
