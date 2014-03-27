 /********************************************************************
 Created: 2001/05/14 
 File name: aux_mpeg.hpp
 Purpose: defines aux-exchange-structures for a/v-encoders and muxers

 Copyright (c) 2001-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef _AUX_MPEG_H_
#define _AUX_MPEG_H_

#include "mctypes.h"

enum aux_ID
{
/* AcessUnit Identification */
  UNSPECIFIED_AU        = 0x000F0000L,
  VIDEO_AU_CODE         = 0x000F0001L,
  AUDIO_AU_CODE         = 0x000F0002L,
  SP_AU_CODE            = 0x000F0004L,
  VIDEO_AU_START        = 0x000F0008L,

/* use to get contents of bytecount field */
  BYTECOUNT_INFO        = 0x000F0010L,
  FLUSH_BUFFER          = 0x000F0020L,
  SPLIT_OUTPUT          = 0x000F0040L,

/* used to signal end of stream */
  STREAM_END_CODE       = 0x000F0080L,

/* muxer output information codes */
  VIDEO_STREAM_INFO     = 0x000F0100L,
  VIDEO_AU_INFO         = 0x000F0200L,
  AUDIO_STREAM_INFO     = 0x000F0400L,
  AUDIO_AU_INFO         = 0x000F0800L,

  VIDEO_SEQ_INFO_XML    = 0x000F0810L,
  VIDEO_AU_INFO_XML     = 0x000F0811L,
  AUDIO_SEQ_INFO_XML    = 0x000F0812L,
  AUDIO_AU_INFO_XML     = 0x000F0813L,

/* DVD navigation codes */
  DVD_DO_NAV_INFO       = 0x000F1000L,
  DVD_SECTOR_NUM        = 0x000F2000L,
  DVD_PTM_TIME          = 0x000F4000L,
  DVD_ABORT_NAV_INFO    = 0x000F8000L,

  DVD_FIXUP_LAST_VOBU   = 0x000E0000L,
  DVD_SETUP_VOBIDN      = 0x000E0001L,
  DVD_RLBN_INFO         = 0x000E0002L,
  DVD_SEQ_END_CODE      = 0x000E0004L,
  DVD_GET_LAST_RLBN     = 0x000E0008L,
  DVD_GET_RLBN_COUNT    = 0x000E0009L,
  DVD_SET_HLI_INFO      = 0x000E0010L,
  DVD_DO_HLI_PTM_TIME   = 0x000E0020L,
  DVD_SETUP_CELLIDN     = 0x000E0040L,
  DVD_CELL_CHANGE       = 0x000E0080L,

  DVD_DO_ASYNCI_INFO    = 0x000E0101L,  // return value is the stream ID which must be passed back to DVD_ASYNCI_INFO
  DVD_ASYNCI_INFO       = 0x000E0102L,
  DVD_DO_SPSYNCI_INFO   = 0x000E0201L,  // return value is the stream ID which must be passed back to DVD_SPSYNCI_INFO
  DVD_SPSYNCI_INFO      = 0x000E0202L,
  DVD_FIXUP_SYNCI       = 0x000E0203L,

/* VCD items */
  VCD_UPDATE_VBV        = 0x000D0000L,

/* demuxer messages */
  DMUX_CHUNK_INFO		    = 0x000D1000L,

  /* transport stream ID messages */
  TS_MICROMV            = 0x000C0001L,
  TS_DVB                = 0x000C0002L,
  TS_DVHS               = 0x000C0004L,

/* buffered seek messages */
  BUF_SEEK_INFO				    = 0x000C0011L,
  BUF_SEEK_SAMPLE_INFO		= 0x000C0012L,
  BUF_SEEK_STREAM_INFO		= 0x000C0014L,
  BUF_SEEK_UPDATE_SETTINGS  = 0x000C0015L,
  BUF_SEEK_RESET			    = 0x000C0016L,
  BUF_SEEK_REMOVE_STREAM	= 0x000C0017L,
  BUF_SEEK_INIT_OUTPUT		= 0x000C0018L,
  BUF_SEEK_MSTART		      = 0x000C0019L,
  BUF_SEEK_REORDER		    = 0x000C0020L,

/* filename and filenumber codes */
  FILENUMBER_INFO       = 0x00F00001L,
  FILENAME_INFO         = 0x00F00002L,
  SWAP_ENDIAN           = 0x00F00004L,
  FILESIZE_INFO         = 0x00F00008L, // only when opened for reading!!!

/* Audio-sync Identification */
  AUDIO_SYNC_CODE       = 0xFFF00000L,

/* Video-sync (exactly the same as MPEG-Codes) */
  ID_PICTURE_START_CODE = 0x00000100L,
  ID_SLICE_MIN_START    = 0x00000101L,
  ID_USER_START_CODE    = 0x000001B2L,
  ID_SEQ_START_CODE     = 0x000001B3L,
  ID_EXT_START_CODE     = 0x000001B5L,
  ID_SEQ_END_CODE       = 0x000001B7L,
  ID_GOP_START_CODE     = 0x000001B8L,
  ID_ISO_END_CODE       = 0x000001B9L,
  ID_PACK_START_CODE    = 0x000001BAL,
  ID_SYSTEM_START_CODE  = 0x000001BBL,

/* Auxiliary information */
  TIMESTAMP_INFO        = 0x0F000000L,  // used to pass actual timestamps
  STATISTIC_INFO        = 0x0F000001L,  // used to pass encoding statistics
  CPB_FULLNESS          = 0x0F000002L,  // used to pass current cpb fullness

/* Windows Media Audio information */
  WMA_STREAM_INFO	      = 0x1F000000L	// pass data from WMA encoder to (ASF) muxer
};


//VIDEO_AU_CODE
struct v_au_struct
{
  uint32_t length;
  uint16_t type;
  // hold the SEQHDR_FLAG and/or GOPHDR_FLAG (see mpegdef.h) if the
  // headers
  // are present in this au
  uint16_t flags;
  // this can always be set to zero by the encoder
  // it is used by the muxer to add user data blocks to streams that
  // don't have them
  uint16_t svcd_offset;
  uint16_t pict_hdr_offset;
  uint32_t first_frame_offset;
  int64_t  DTS;
  int64_t  PTS;
};

//TIMESTAMP_INFO
struct sample_info_struct
{
  int64_t  rtStart;
  int64_t  rtStop;
  uint32_t flags;
  int32_t  mode;
};

//STATISTIC_INFO
struct encode_stat_struct
{
  int32_t  frames_incoming;
  int32_t  frames_encoded;
  uint32_t time_ms_elapsed;
  double   summary_size;
  double   average_bitrate;
  double   average_speed;

  double   current_psnr_y;
  double   current_psnr_u;
  double   current_psnr_v;
  double   current_psnr_a;

  double   overall_psnr_y;
  double   overall_psnr_u;
  double   overall_psnr_v;
  double   overall_psnr_a;

  double   min_qp;
  double   max_qp;
  double   avg_qp;
};

// CPB fullness
struct cpb_fullness_struct
{
  int32_t initial_fullness;  // buffer level before encoding picture
  int32_t final_fullness;    // buffer level after encoding picture
  int32_t removal_delay;

  int32_t fullness_units;    // 0 = fullness in %, 1 = fullness in bits, 2 = fullness in 90kHz

  int32_t buffer_size_in_bits;

  int32_t reserved[7];
};


//AUDIO_AU_CODE
struct a_au_struct
{
  uint32_t length; 
  int64_t  pts;
};


//SP_AU_CODE
struct sp_au_struct
{
  uint32_t length; 
  int64_t  pts;
};


//AUDIO_SYNC_CODE
struct aud_info 
{
  int32_t version;
  int32_t lay;
  int32_t error_protection;
  int32_t bitrate_index;
  int32_t sampling_frequency;
  int32_t padding;
  int32_t extension;
  int32_t mode;
  int32_t mode_ext;
  int32_t copyright;
  int32_t original;
  int32_t emphasis;
};

// WMA_STREAM_INFO
struct wma_stream_info
{
  int32_t sampling_frequency; 
  int32_t bits_per_sample; 
  int32_t official_bitrate;
  int32_t num_channels;
  int32_t average_bytes_per_second;
  int32_t samples_per_block;
  int32_t super_block_align;
  int32_t encode_options;
  int32_t block_align;  


  // align structure to 64 bytes:
  uint8_t reserved[64 - 9 * sizeof(int32_t)]; 
}; 


//SEQ_START_CODE,            (without size, I will correct it later, not used)
struct seq_start_info
{
  uint32_t video_type;
  uint32_t frame_rate_code;
  uint32_t bit_rate;
  uint32_t max_bit_rate;
  uint32_t pulldown_flag;
  uint32_t vbv_buffer_size;    
};


//GOP_START_CODE
struct gop_start_info
{
  uint32_t info_size;          // size of the passed information

  uint32_t max_govu_length;    // max. number of frames in a GOVU
  uint32_t reordering_delay;   // max. distance between two ref frames
  uint32_t idr_frequency;      // 0 = only one IDR at the start, 1 = every GOVU starts with and IDR, etc.

  uint32_t IDR;                // 1 = instantaneous decoder refresh

  uint32_t reserved[32];
};


//PICTURE_START_CODE,        (without size, I will correct it later, not used)
struct pic_start_info
{
  uint32_t pict_type;
  uint32_t temp_ref;
  uint32_t repeat_first_field;
  uint32_t vbv_delay; // 90khz based
  uint32_t top_field_first;
    uint32_t progressive_frame;
};


//USER_START_CODE,            (without size, I will correct it later, not used)
struct userdata_info
{
  uint8_t tag_name;
  uint8_t size;
  uint8_t userdata[1];
};


#define SECTOR_FLAG_HAS_VIDEO  0x00000001


//DVD_SECTOR_NUM,   
struct dvd_sector_info
{
  int32_t sector_num;     // sector number
  int32_t sector_length;  // length of sector in 90000 mHz clocks
  int32_t flags;
};

//DVD_PTM_TIME,       
struct dvd_ptm_info
{
  uint32_t PTM_time;      // ending time of VOB
};


struct dvd_fixup_info
{
  int32_t sec_present;    // a sequence end code is present
  int32_t first_sector;   // the first sector number of the added sectors
  int32_t sectors_to_add; // number of sectors to add to the last vobu
  int32_t sector_length;  // length of sector in 90000 mHz clocks
};


struct dvd_vobidn_info
{
  int32_t vob_idn;        // new vob idn number
  int32_t sectors_to_add; // number of sectors to add to the NV_PCK_LBN entries
};


struct dvd_nv_info
{
  uint32_t nv_sa;
  uint32_t frame_num;
  int32_t  vob_num;
  int32_t  cell_num;
};


struct dvd_synci_info
{
  int32_t stream_num;
  int32_t sector_offset;
};


//GOP_START_CODE, NULL
//EXT_START_CODE, NULL
//SEQ_END_CODE, NULL
//SLICE_MIN_START+j, &j       (without size, I will correct it later, not used)
//SLICE_MIN_START+(j&127), &j (without size, I will correct it later, not used)


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

// NOTE! the size of these structures must be < 256 bytes or they
// cannot be passed to an auxinfo function!

// VIDEO_STREAM_INFO
struct video_stream_info
{
  uint8_t ID[4];
  int32_t length;
  int32_t program_stream_flag;
  int32_t stream_id;
  int32_t PID;
  int32_t pulldown;
  int32_t reserved[19];
  int32_t seqhdr_length;
  uint8_t seq_hdr[150];
};

// AUDIO_STREAM_INFO
struct audio_stream_info
{
  uint8_t ID[4];
  int32_t length;
  int32_t program_stream_flag;
  int32_t stream_id;
  int32_t substream_id;
  int32_t PID;
  int32_t audio_type;
  int32_t reserved[20];
  int32_t audhdr_length;
  uint8_t audhdr[84];
  int32_t pvt1hdr_length;
  uint8_t pvt1hdr[8];
};


// VIDEO_AU_INFO
struct video_au_info
{
  int64_t filePos;
  uint8_t num_in_sector;
  uint8_t frame_type; 
  int16_t temp_ref;
  int32_t forw_ref_frame;
  int32_t coding_order;
  int32_t display_order;
  int64_t PTS;
};

// VIDEO_AU_START
struct video_au_start
{
  uint32_t chapter_state; /* chapter, scene-change what else? */
  uint32_t tc_frame_nr;   /* frame_nr used to generate GOP-timecode */
  uint32_t ts_frame_nr;   /* frame_nr used to generate PTS/DTS-timestamps */
  uint32_t flags;         /* nothing */
};


// AUDIO_AU_INFO
struct audio_au_info
{
  int64_t filePos;
  int32_t num_in_sector;
  int64_t PTS;
};


// VIDEO_SEQ_INFO_XML
struct video_seq_info_xml
{
  uint8_t pulldown_flag;
  uint8_t frame_rate_code;
  uint8_t reserved[62]; 
};


// VIDEO_AU_INFO_XML
struct video_au_info_xml
{
  int64_t  filePos;
  uint32_t flags;
  int32_t  length;
  uint8_t  repeat_first_field;
  int64_t  PTS;
  int32_t  hdr_length;
  uint8_t* hdr;
  uint8_t  reserved[31]; 
};


// AUDIO_SEQ_INFO_XML
struct audio_seq_info_xml
{
  int64_t clocks_per_audio_frame;
  uint8_t reserved[56]; 
};


// AUDIO_AU_INFO_XML
struct audio_au_info_xml
{
  int64_t PTS;
  int32_t length;
  uint8_t reserved[52]; 
};

//////////////////////  DVD HLI structures  ///////////////////////////////

// section 4.4.3.2 Highlight General Information (HL_GI)

// table 4.4.3.2-1 HL_GI
struct HL_GI
{
  uint8_t HLI_SS[2];
  uint8_t HLI_S_PTM[4];
  uint8_t HLI_E_PTM[4];
  uint8_t BTN_SL_E_PTM[4];
  uint8_t BTN_MD[2];
  uint8_t BTN_OFN[1];
  uint8_t BTN_Ns[1];
  uint8_t NSL_BTN_Ns[1];
  uint8_t reserved1[1];
  uint8_t FOSL_BTNN[1];
  uint8_t FOAC_BTNN[1];
};


// section 4.4.3.3 Button Color Information Table (BTN_COLIT)

// figure 4.4.3.3-1 BTN_COLI
struct BTN_COLI
{
  uint8_t SL_COLI[4];
  uint8_t AC_COLI[4];
};


// section 4.4.3.4 Button Information Table (BTNIT)

// figure 4.4.3.4-1 BTNI
struct BTNI
{
  uint8_t BTN_POSI[6];
  uint8_t AJBTN_POSI[4];
  uint8_t BTN_CMD[8];
};


struct HLI
{
  struct HL_GI    hl_gi;
  struct BTN_COLI btn_coli[3];
  struct BTNI     bnti[36];
};


// pointer to an HLI structure with size
struct HLI_PTR
{
  int32_t still_menu;
  int32_t size;
  struct HLI*    hli;
};


// BUF_SEEK_INFO
struct buf_seek_info
{
  int64_t  seek_pos;
  uint8_t* bfr;
  uint32_t bfr_size;
};


// DMUX_CHUNK_INFO
#define MCDMUX_FLAGS_UNIT_START			0x00000001
#define MCDMUX_FLAGS_LPCM_FRAME_FOUND	0x00000002	// a DVD LPCM frame was found
#define MCDMUX_FLAGS_DVD_LPCM			0x00010000	// DVD LPCM stream
#define MCDMUX_FLAGS_HDMV_LPCM			0x00020000	// HDMV LPCM stream
#define MCDMUX_FLAGS_AES3_302M			0x00040000	// AES3-302M LPCM stream
#define MCDMUX_FLAGS_AES3_331M			0x00080000	// AES3-331M LPCM stream
#define MCDMUX_FLAGS_AES3_382M			0x00100000	// AES3-382M LPCM stream
#define MCDMUX_FLAGS_TWOS_LPCM			0x00200000	// TWOS LPCM stream


struct dmux_chunk_info
{
	unsigned char *ptr;
	int length;
	unsigned int flags;
	int64_t pack_pos;
	int64_t file_pos;
	int64_t PTS;

	// additional data
	// for DVD it contains the extra bytes immediately after the PES header
	// but before the actual audio data (for AC3, LPCM, DTS and SDDS)
	// for AES3 331M PCM audio it contains the first 4 bytes immediately
	// after the PES header but before the actual audio data
	unsigned char info[32];
	unsigned char info_len;
	unsigned char end_of_stream;

	// AES3 specific info 
	int aes3_payload_length;
	int aes3_channel;
};

#pragma pack(pop)

#endif /* _AUX_MPEG_H_ */
