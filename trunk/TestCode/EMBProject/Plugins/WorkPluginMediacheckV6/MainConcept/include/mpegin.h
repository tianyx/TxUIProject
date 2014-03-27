//------------------------------------------------------------------------------
// File: mpegin.h
//
// Desc: mpeg file decoder API header
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _MPEGIN_H_
#define _MPEGIN_H_

#include "mctypes.h"

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks) && !defined(__QNX__)
#include <windows.h>
#endif
#include "mpegdef.h"
#include "bufstrm.hpp"
#include "mpgvdec.hpp"

// find stream options
#define FIND_NONE      0
#define FIND_VIDEO     1   // stop looking after 1 video stream is found
#define FIND_AUDIO     2   // stop looking after 1 audio stream is found
#define FIND_VIDAUD    3   // stop looking after 1 video and 1 audio are found
#define FIND_SUBPIC    4   // stop looking after 1 subpic stream is found
#define FIND_VIDAUDSP  7   // stop looking after 1 video, 1 audio and 1 subpic are found
#define FIND_ALL       8   // do not stop looking until the MB limit is reached

// maximum number of streams supported
#define VIDEO_STREAM_MAX	16
#define AUDIO_STREAM_MAX	32		// mpeg and aes3 audio
#define AC3_STREAM_MAX		8
#define DTS_STREAM_MAX		16
#define SDDS_STREAM_MAX		8
#define PCM_STREAM_MAX		8
#define SUBPIC_STREAM_MAX	32
#define AMR_STREAM_MAX		8


// depreciated, output types
#define VIDEO_YUV      1   // depreciated, use fourCC field instead
#define VIDEO_RGB      3   // depreciated, use fourCC field instead
#define AUDIO_PCM      4   // depreciated, use the audio stream_to_get fields instead
#define VIDEO_BGR3     0xB // depreciated, use fourCC field instead

// depreciated, use the video_streams field instead
#define VIDEO_STREAM_0  0x00000001
#define VIDEO_STREAM_1  0x00000002
#define VIDEO_STREAM_2  0x00000004
#define VIDEO_STREAM_3  0x00000008
#define VIDEO_STREAM_4  0x00000010
#define VIDEO_STREAM_5  0x00000020
#define VIDEO_STREAM_6  0x00000040
#define VIDEO_STREAM_7  0x00000080
#define VIDEO_STREAM_8  0x00000100
#define VIDEO_STREAM_9  0x00000200
#define VIDEO_STREAM_10 0x00000400
#define VIDEO_STREAM_11 0x00000800
#define VIDEO_STREAM_12 0x00001000
#define VIDEO_STREAM_13 0x00002000
#define VIDEO_STREAM_14 0x00004000
#define VIDEO_STREAM_15 0x00008000

// depreciated, use the audio_streams field instead
#define AUDIO_STREAM_0  0x00000001
#define AUDIO_STREAM_1  0x00000002
#define AUDIO_STREAM_2  0x00000004
#define AUDIO_STREAM_3  0x00000008
#define AUDIO_STREAM_4  0x00000010
#define AUDIO_STREAM_5  0x00000020
#define AUDIO_STREAM_6  0x00000040
#define AUDIO_STREAM_7  0x00000080
#define AUDIO_STREAM_8  0x00000100
#define AUDIO_STREAM_9  0x00000200
#define AUDIO_STREAM_10 0x00000400
#define AUDIO_STREAM_11 0x00000800
#define AUDIO_STREAM_12 0x00001000
#define AUDIO_STREAM_13 0x00002000
#define AUDIO_STREAM_14 0x00004000
#define AUDIO_STREAM_15 0x00008000
#define AUDIO_STREAM_16 0x00010000
#define AUDIO_STREAM_17 0x00020000
#define AUDIO_STREAM_18 0x00040000
#define AUDIO_STREAM_19 0x00080000
#define AUDIO_STREAM_20 0x00100000
#define AUDIO_STREAM_21 0x00200000
#define AUDIO_STREAM_22 0x00400000
#define AUDIO_STREAM_23 0x00800000
#define AUDIO_STREAM_24 0x01000000
#define AUDIO_STREAM_25 0x02000000
#define AUDIO_STREAM_26 0x04000000
#define AUDIO_STREAM_27 0x08000000
#define AUDIO_STREAM_28 0x10000000
#define AUDIO_STREAM_29 0x20000000
#define AUDIO_STREAM_30 0x40000000
#define AUDIO_STREAM_31 0x80000000

// depreciated, use the ac3_streams field instead
#define AC3_STREAM_0  0x00000001
#define AC3_STREAM_1  0x00000002
#define AC3_STREAM_2  0x00000004
#define AC3_STREAM_3  0x00000008
#define AC3_STREAM_4  0x00000010
#define AC3_STREAM_5  0x00000020
#define AC3_STREAM_6  0x00000040
#define AC3_STREAM_7  0x00000080

// depreciated, use the dts_streams field instead
#define DTS_STREAM_0  0x00000001
#define DTS_STREAM_1  0x00000002
#define DTS_STREAM_2  0x00000004
#define DTS_STREAM_3  0x00000008
#define DTS_STREAM_4  0x00000010
#define DTS_STREAM_5  0x00000020
#define DTS_STREAM_6  0x00000040
#define DTS_STREAM_7  0x00000080

// depreciated, use the sdds_streams field instead
#define SDDS_STREAM_0  0x00000001
#define SDDS_STREAM_1  0x00000002
#define SDDS_STREAM_2  0x00000004
#define SDDS_STREAM_3  0x00000008
#define SDDS_STREAM_4  0x00000010
#define SDDS_STREAM_5  0x00000020
#define SDDS_STREAM_6  0x00000040
#define SDDS_STREAM_7  0x00000080

// depreciated, use the pcm_streams field instead
#define PCM_STREAM_0  0x00000001
#define PCM_STREAM_1  0x00000002
#define PCM_STREAM_2  0x00000004
#define PCM_STREAM_3  0x00000008
#define PCM_STREAM_4  0x00000010
#define PCM_STREAM_5  0x00000020
#define PCM_STREAM_6  0x00000040
#define PCM_STREAM_7  0x00000080

// depreciated, use the subpic_streams field instead
#define SUBPIC_STREAM_0  0x00000001
#define SUBPIC_STREAM_1  0x00000002
#define SUBPIC_STREAM_2  0x00000004
#define SUBPIC_STREAM_3  0x00000008
#define SUBPIC_STREAM_4  0x00000010
#define SUBPIC_STREAM_5  0x00000020
#define SUBPIC_STREAM_6  0x00000040
#define SUBPIC_STREAM_7  0x00000080
#define SUBPIC_STREAM_8  0x00000100
#define SUBPIC_STREAM_9  0x00000200
#define SUBPIC_STREAM_10 0x00000400
#define SUBPIC_STREAM_11 0x00000800
#define SUBPIC_STREAM_12 0x00001000
#define SUBPIC_STREAM_13 0x00002000
#define SUBPIC_STREAM_14 0x00004000
#define SUBPIC_STREAM_15 0x00008000
#define SUBPIC_STREAM_16 0x00010000
#define SUBPIC_STREAM_17 0x00020000
#define SUBPIC_STREAM_18 0x00040000
#define SUBPIC_STREAM_19 0x00080000
#define SUBPIC_STREAM_20 0x00100000
#define SUBPIC_STREAM_21 0x00200000
#define SUBPIC_STREAM_22 0x00400000
#define SUBPIC_STREAM_23 0x00800000
#define SUBPIC_STREAM_24 0x01000000
#define SUBPIC_STREAM_25 0x02000000
#define SUBPIC_STREAM_26 0x04000000
#define SUBPIC_STREAM_27 0x08000000
#define SUBPIC_STREAM_28 0x10000000
#define SUBPIC_STREAM_29 0x20000000
#define SUBPIC_STREAM_30 0x40000000
#define SUBPIC_STREAM_31 0x80000000

// additional time shifting return codes
#define mpegInNoOutputInit 10
#define mpegInNoVideoInit  11
#define mpegInNoAudioInit  12
#define mpegInNoMPEG       13

// audio_mode 
#define MODE_STEREO           0
#define MODE_JOINT_STEREO     1
#define MODE_DUAL_CHANNEL     2
#define MODE_MONO             3

// control flags
#define CONTROL_OK            0
#define CONTROL_EOF           1  // called when eof is detected by mpegSplit

#define PCM_AUDIO_BUFFER_SIZE 192000    // recommended PCM buffer size, 48000 * 4, enough for 1 second

// decoder object, used internally
typedef struct mpeg_decoder mpegdec_tt;

// splitter callbacks 

// called to seek in the source file in splitter mode
// position - file position to seek to
// return 0 if OK, 1 if fault
typedef int (*seekFileData)(void* lpData, int64_t position);

// called to read data in splitter file mode
// read bufferSize bytes from file into buffer
// return number of bytes read, 0 if fault
typedef int (*readFileData)(void* lpData, uint8_t *buffer, int bufferSize);

// called when a new stream/substream is encountered
// streamID - stream ID number
// subStreamID - sub-stream ID number
// return 1 if stream is to be split, 0 if not
typedef int (*initOutputStream)(void* lpData, int streamID, int subStreamID);

// called with a chunk of stream data when available
// streamID - stream ID number
// subStreamID - sub-stream ID number
// buffer - data buffer
// bufferSize - number of bytes in buffer
// return currently not used
typedef int (*sendStreamData)(void* lpData, int streamID, int subStreamID, uint8_t *buffer, int bufferSize, int64_t timecode);

// callback with control information, one of the CONTROL_* constants
// return currently not used
typedef int (*controlData)(void* lpData, int controlID);

// progress monitor prototype 
typedef void (*mpegInProg)(char *txt, int *abort);


// external i/o function prototypes
// note: if these are used, the mpegInInfo.file_length field MUST be filled in
//       with the correct file size by the app
// called to open a file
// and it will be passed back to the seek, read and close functions
// appData - the appData field in the mpegInInfo structure is passed back here
// fileName - if a filename was passed to mpegin, it is returned here
// return the file handle as a long or -1 if an error occurs
typedef long (*externalOpen)(void *appData, char *fileName);
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)
typedef long (*externalOpenW)(void *appData, wchar_t *fileName);
#else
typedef long (*externalOpenW)(void *appData, unsigned short *fileName);
#endif

// called to seek in a file
// appData - the appData field in the mpegInInfo structure is passed back here
// fileHandle - a file handle returned by the externalOpen function
// position - the position to seek to
// return 0 if Ok, 1 if an error occurs
typedef int (*externalSeek)(void *appData, long fileHandle, int64_t position);

// called to read from a file
// appData - the appData field in the mpegInInfo structure is passed back here
// fileHandle - a file handle returned by the externalOpen function
// buffer - a buffer for the data
// bufferSize - the number of bytes to read
// return the number of bytes actually read or 0 if an error occurs
typedef int (*externalRead)(void *appData, long fileHandle, uint8_t *buffer, int bufferSize);

// called to close a file
// appData - the appData field in the mpegInInfo structure is passed back here
// fileHandle - a file handle returned by the externalOpen function
typedef void (*externalClose)(void *appData, long fileHandle);


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct mpegin_stream_data
{
	uint16_t pid;			// transport stream only
	uint8_t id;
	uint8_t sub_id;			// DVD only

	uint16_t program_number;	// transport stream program number

	uint32_t reserved[2];
};



// mpegin structure 
// to be compatible this structure should be 77400 bytes big 

struct mpegInInfo
{
  // input items
  char *inputFilename;      // depreciated, pass filename to mpegInOpen function
  mpegInProg inProg;        // depreciated, use get_rc prg_printf
  int outputTypes;          // depreciated, use the fourCC and audio stream_to_get fields instead
  int currentTypes;         // depreciated, use the fourCC and audio stream_to_get fields instead

  int FastAccess;           // do PTS searching instead of parsing entire file, default is true
  int Quiet_Flag;           // suppress verbose output, default is true
  int additional_GOP_search; // additional GOP's to search when determing pulldown

  // output items 
  int MPEG_Open;            // flag indicating an MPEG is open, default is false;
  int program_stream_flag;  // program stream flag
                            // 0 = elementary stream
                            // 1 = mpeg-1 program stream
                            // 2 = mpeg-2 program stream
                            // 3 = mpeg-2 transport stream
                            // 4 = PVA stream
                            // 5 = MXF stream

  // video output variables 
  int videoAvail;
  int totalFrames;              // total video frames in file
  int horizontal_size;          // horizontal size of video
  int vertical_size;            // vertical size of video
  int MPEG2_video;              // MPEG-1 or MPEG-2 video flag, 1 = MPEG-2 video
  double bit_rate;              // bitrate in bits per second
  double frame_rate;            // coded frame rate
  double actual_frame_rate;     // actual frame rate

  // video output buffers 
  uint8_t *RGBBuffer;            // RGB video buffer, BGRA bitmap
  int RGBRowSize;               // number of bytes in 1 row of RGB video data
                                // note: the YUV buffers are back-to-back in memory
                                // so they can be treated as one large buffer starting 
                                // at YUVBuffers[0]
  uint8_t *YUVBuffers[3];        // YUV video buffers, 0 = Y, 1 = U, 2 = V
  int YRowSize;                 // number of bytes in 1 row of Y video data
  int YHeight;                  // number of rows of Y video data
  int UVRowSize;                // number of bytes in 1 row of U or V video data
  int UVHeight;                 // number of rows of U and V video data
  int YUVFormat;                // YUV chroma format. one of the CHROMA* consts

  uint8_t *raw_buffer;           // raw video buffer
  int raw_buffer_size;          // size of frame in raw video buffer

  // audio output variables 
  int audioAvail;               // if non-zero, audio is present in file
  uint32_t totalSamples;         // total audio samples in file
  uint32_t numAudioBytes;        // number of bytes returned in audio buffer
  uint32_t audioRate;            // audio sample rate, either 32000, 44100 or 48000
  int stereo;                   // non-zero indicates stereo samples
  uint32_t sampleSize;           // size in bits of samples, will be 16
  int audioLayer;               // MPEG audio layer encoded in file, 1 or 2
  int audioMode;                // MPEG audio mode encoded in file, one of the MODE_* consts
  int audioBitrate;             // MPEG audio bitrate
  int bytesPerSample;           // bytes per audio sample

  int get_yuv_frame;            // true - get YUV frame in addition to RAW frame
  int get_pcm_samples;          // true - get PCM samples in addition to RAW frames

  int no_frame_seek_available;  // frame seeking is not available

  uint8_t *raw_aud_buffer;       // raw audio buffer
  int raw_aud_size;                 // size of frame in raw audio buffer

  int flip_vertically;          // flip the frames vertically

  int sequence_header_found;    // true if sequence header was found with the current frame
  int gop_header_found;         // true if gop_header_found was found with the current frame
  int first_frame_repeat;       // true if the first frame in the stream had the repeat flag set

  frame_tt *frame_info;         // application supplied frame_tt structure

  int use_external_frame_buffer; // use the frame_info field below as the destination frame buffer

  int64_t totalSamples64;

  int requested_buffer_size;     // the requested buffer size for the internal file buffers, 0 = use default size

  int use_io_threads;            // do file i/o in threads, ignored in splitter mode, currently Win32 only

  int frame_size_flag;			 // one of the DECODE_*_SIZE flags in the mpgidec.h header

  int first_frame_top;           // true if the first frame in the stream had the top field first flag set

  int user_no_seeking;           // set to true if the duration and seeking (except to frame/sample 0) is required
                                 // use this if you just want to play file from the start, opening files is quicker
                                 // and problem files may be opened that cannot be opened in normal mode.
                                 // the duration for all streams will be 0 and the seek functions will return
                                 // an error if the seek position is not 0

  int fail_no_fast_mode;         // if a file cannot be opened in 'fast' mode,
                                 // return an error and set the flag below
  int no_fast_mode_flag;         // if the above field is true and a file cannopt be opened
                                 // in fast mode, the open call will return an error
                                 // and this flag will be set. The application can decide
                                 // whether to open in non-fast mode (FastAccess = 0)

  int use_all_frames;            // use all frames/samples in the file
  int broken_frame;              // the frame returned cannot be decoded
  int padding_frame;             // the frame returned is a padding frame
  int padding_samples;           // the samples returned are padding samples
  int broken_aframe;             // at least some of the sample returned could not be decoded

  int fixed_frame_size_flag;	 // inform mpegin the input video elementary stream is D10 type video
								 // used if the GOP timecodes in the stream are all zeros

  int avc_seek_start_point;		// specify the method of reference frame seeking in AVC video
								//
								// 0 = seek to the nearest IDR frame before the target frame,
								// decode from IDR to target frame (default)
								//
								// 1 = seek to the nearest frame with I slices before the target
								// frame, decode from this frame to target frame
								//
								// other values indicate the number of frames before the target
								// frame where decoding is to start. The actual starting frame
								// will be the nearest frame with I slices before the value
								// specified.
								// 
								// For example, if avc_seek_start_point = 30 and a seek is performed to
								// frame 130, mpegin will backup to frame 100 (130 - 30) to start decoding
								// until it reaches frame 130. If frame 100 does not contain any I slices
								// but frame 98 does, the decoding will actually start at frame 98 instead
								// of frame 100

  int reserved2[17];

  // header information 
  int profile, level;

  int reserved3[2];

  int M; // I frame distance
  int N; // P frame distance
  int closed_gops; // true if GOP's do not have the leading B frames

  // sequence_header 
  int aspect_ratio_information;
  int frame_rate_code;
  int bit_rate_value;
  int vbv_buffer_size;
  int constrained_parameters_flag;

  // sequence_extension 
  int profile_and_level_indication;
  int progressive_sequence;
  int chroma_format;
  int low_delay;
  int frame_rate_extension_n;
  int frame_rate_extension_d;

  // sequence_display_extension 
  int video_format;
  int color_description;
  int color_primaries;
  int transfer_characteristics;
  int matrix_coefficients;
  int display_horizontal_size;
  int display_vertical_size;

  // picture_header 
  int temporal_reference;
  int picture_coding_type; // picture type of last picture decoded
  int picture_type;        // picture type of current displayed frame
  int vbv_delay;
  int full_pel_forward_vector;
  int forward_f_code;
  int full_pel_backward_vector;
  int backward_f_code;

  // picture_coding_extension header 
  int f_code[2][2];
  int intra_dc_precision;
  int picture_structure;
  int top_field_first;
  int frame_pred_frame_dct;
  int concealment_motion_vectors;
  int intra_vlc_format;
  int repeat_first_field;
  int chroma_420_type;
  int progressive_frame;
  int composite_display_flag;
  int v_axis;
  int field_sequence;
  int sub_carrier;
  int burst_amplitude;
  int sub_carrier_phase;

  // picture_display_extension header 
  int frame_center_horizontal_offset[3];
  int frame_center_vertical_offset[3];

  // copyright_extension header 
  int copyright_flag;
  int copyright_identifier;
  int original_or_copy;
  int copyright_number_1;
  int copyright_number_2;
  int copyright_number_3;
  
  // group_of_pictures_header 
  int drop_flag;
  int hour;
  int minute;
  int sec;
  int frame;
  int closed_gop;
  int broken_link;

  // sequence header and quant_matrix_extension 
  int intra_quantizer_matrix[64];
  int non_intra_quantizer_matrix[64];
  int chroma_intra_quantizer_matrix[64];
  int chroma_non_intra_quantizer_matrix[64];

  int load_intra_quantizer_matrix;
  int load_non_intra_quantizer_matrix;
  int load_chroma_intra_quantizer_matrix;
  int load_chroma_non_intra_quantizer_matrix;

  // picture coding extension 
  int q_scale_type;
  int alternate_scan;

  int number_of_frame_center_offsets;
  int sequence_display_ext_present;
  int picture_display_ext_present;

  // no longer used! May be used in the future for other things!
  char savedInputFilename[512]; 

  int reserved5[258];

  // audio header 
  int channels;                 // number of channels
  uint8_t aac_object_type;
  uint8_t aac_home;
  uint8_t aac_copyright_start;
  uint8_t aac_raw_data_blocks;
  int aac_bytes_in_frame;
  int bytesInFrame;
  int samplesInFrame;
  int aac_buffer_fullness_bytes;
  int ID;
  int layer;
  int protection_bit;
  int bitrate_index;
  int sampling_frequency;
  int padding_bit;
  int private_bit;
  int mode_extension;
  int copyright;
  int original;
  int emphasis;
  int aac_flag;

  int MPEG_type;                // type of mpeg stream, one of the MPEG_* constants
  int current_MPEG_N;
  uint32_t audio_streams_found; // depreciated, use audio_streams field below
  uint32_t video_streams_found; // depreciated, use video_streams field below
  int audio_stream_count;       // number of audio streams found
  int video_stream_count;       // number of video streams found
  int audio_stream_to_get;
  int video_stream_to_get;
  int fourCC;                   // fourcc type, can be used in place of VIDEO_* constants

  // do a seek even if the current frame is equal to the seek frame
  int seek_if_dup;

  int reserved10[4086];

  // splitter data
  int64_t file_length;     // length of file

  // splitter callbacks
  seekFileData seek_file_data;
  readFileData read_file_data;
  initOutputStream init_output_stream;
  sendStreamData send_stream_data;
  controlData control_data;

  int64_t first_vPTS;
  int64_t last_vPTS;
  int64_t seek_gop_vPTS;
  int64_t ref_gop_vPTS;
  int64_t first_aPTS;
  int64_t last_aPTS;
  // enough room for 140 bytes of seq hdr and 10 bytes of seq ext
  // plus 2 extra to make it a quad boundary
  int seqhdr_length;
  uint8_t seqhdr[152];
  // audio header for MPEG, PCM and AC3 streams, 84 bytes
  int audhdr_length;
  uint8_t audhdr[84];
  // private stream 1 header information, 6 bytes needed, 8 to pad to quad word
  // used for PCM, AC3, DTS and SDDS audio
  // includes the substream ID and packet parameters
  int pvt1hdr_length;
  uint8_t pvt1hdr[8];

  // private 1 audio streams

  // AC3
  uint32_t ac3_streams_found;  // // depreciated, use ac3_streams field below
  int ac3_stream_count;            // number of ac3 audio streams found

  // DTS
  uint32_t dts_streams_found;  // depreciated, use dts_streams field below
  int dts_stream_count;            // number of dts audio streams found

  // SDDS
  uint32_t sdds_streams_found; // depreciated, use sdds_streams field below
  int sdds_stream_count;           // number of sdds audio streams found

  // PCM
  uint32_t pcm_streams_found;  // depreciated, use pcm_streams field below
  int pcm_stream_count;            // number of pcm audio streams found

  // audio substream to get from private 1 streams
  int audio_substream_to_get;	// depreciated, use respective stream_to_get fields

  // audio type, one of the *_AUDIO defines
  int audio_type;

  // PCM info
  int pcm_emphasis_flag;
  int pcm_mute_flag;
  int pcm_quantization_word_length;
  int pcm_sampling_frequency;
  int pcm_number_of_channels;
  int pcm_dynamic_range_control;

  // AC3 header info
  int ac3_crc1;
  int ac3_fscod;
  int ac3_frmsizecod;

  int ac3_bsid;
  int ac3_bsmod;
  int ac3_acmod;
  int ac3_cmixlev;
  int ac3_surmixlev;
  int ac3_dsurmod;
  int ac3_lfeon;
  int ac3_dialnorm;
  int ac3_compre;
  int ac3_compr;
  int ac3_langcode;
  int ac3_langcod;
  int ac3_audprodie;
  int ac3_mixlevel;
  int ac3_roomtyp;
  int ac3_dialnorm2;
  int ac3_compr2e;
  int ac3_compr2;
  int ac3_langcod2e;
  int ac3_langcod2;
  int ac3_audprodi2e;
  int ac3_mixlevel2;
  int ac3_roomtyp2;
  int ac3_copyrightb;
  int ac3_origbs;
  int ac3_timecod1e;
  int ac3_timecod1;
  int ac3_timecod2e;
  int ac3_timecod2;
  int ac3_addbsie;
  int ac3_addbsil;
  uint8_t ac3_addbsi[64];

  // these two variables are set during an open call, they can be changed
  // afterward
  //
  // always do video seeks, do not do any advance frame calls
  // mainly used for debugging and splitter mode
  int always_seek;

  // number of frames for which to do seek advances instead of an actual seek
  // defaults to the number of frames per second
  int advance_limit;

  // pointer to another mpegInInfo structure if needed
  mpegInInfo *rawInfo;

  // true if the file is an AC3 elementary stream with 16-byte timecodes
  // before each audio frame
  int ac3_timecode_flag;

  // subpicture items
  int subpicAvail;                   // subpicture streams available
  uint32_t subpic_streams_found; // combination of the SUBPIC_STREAM* constants above
  int subpic_stream_count;           // number of subpic streams found
  int subpic_stream_to_get;

  int64_t first_sPTS;
  int64_t last_sPTS;
  // subpic header, includes the substream ID and packet parameters
  int subpichdr_length;
  uint8_t subpichdr[8];

  // transport stream items
  int video_pids[16];	// depreciated, use video_streams field below
  int audio_pids[32];	// depreciated, use audio_streams field below
  int ac3_pids[8];		// depreciated, use ac3_streams field below
  int dts_pids[8];		// depreciated, use dts_streams field below
  int sdds_pids[8];		// depreciated, use sdds_streams field below
  int pcm_pids[8];		// depreciated, use pcm_streams field below
  int subpic_pids[32];	// depreciated, use subpic_streams field below
  
  // parsing options that will be passed to the audio and video decoders
  int video_parse_options;
  int audio_parse_options;

  int find_stream_options;
  int display_subpics;

  // the file position where a valid video or audio stream starts
  // if a program stream it points to the pack header
  int64_t first_filePos;

  // maximum limit (in MB) the find stream logic will search
  // for streams
  int find_stream_limit;

  // items to send a palette to the subpic/video decoders

  // set to have the subpic palette below be the current palette
  // the flag is cleared when the palette is read during the open 
  // functions or the video seek/get next functions
  int set_subpic_palette;
  // 16 24bpp YUV colors, 0 = reserved, 1 = Y, 2 = U, 3 = V, the DVD way!
  uint8_t subpic_palette[16][4];

  // returned value, the number of titles in a title set
  int dvd_num_titles;
  int dvd_num_angles;

  // input controls
  int dvd_language;
  int dvd_angle;
  int dvd_parental_level;
  int dvd_title_num;
  int dvd_get_vts_menu;

  // H264 flag
  int h264_video_flag;

  // external IO stuff
  externalOpen extOpen;
  externalSeek extSeek;
  externalRead extRead;
  externalClose extClose;
  int use_external_io;
  void *appData;

  // extra bytes after every transport stream packet that should be discarded
  int ts_extra_bytes;

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)
  wchar_t *inputFilenameW;  // pointer to a Unicode string input filename
#else
  int reserved12;
#endif

  int h264_assume_frame_rate;

  // AES3
  int aes3_pids[8];				// depreciated, use aes3_streams field below
  uint32_t aes3_streams_found;  // depreciated, use aes3_streams field below
  int aes3_stream_count;        // number of aes3 audio streams found

  int (*prompt_index_mode)();

  bufstream_tt *dmux_chunk_bs;

  externalOpenW extOpenW;

  int mpeg4_video_flag;

  // extended ac3 header
  unsigned char ac3_strmtyp;
  unsigned char ac3_substreamid;
  unsigned short ac3_frmsiz;
  unsigned char ac3_fscod2;
  unsigned char ac3_numblkscod;
  unsigned char ac3_chanmape;
  unsigned short ac3_chanmap;
  unsigned char ac3_mixmdate;
  unsigned char ac3_dmixmod;
  unsigned char ac3_ltrtcmixlev;
  unsigned char ac3_lorocmixlev;
  unsigned char ac3_ltrtsurmixlev;
  unsigned char ac3_lorosurmixlev;
  unsigned char ac3_lfemixlevcode;
  unsigned char ac3_lfemixlevcod;
  unsigned char ac3_pgmscle;
  unsigned char ac3_pgmscl;
  unsigned char ac3_pgmscl2e;
  unsigned char ac3_pgmscl2;
  unsigned char ac3_extpgmscle;
  unsigned char ac3_extpgmscl;
  unsigned char ac3_mixdef;
  unsigned char ac3_mixdeflen;
  unsigned char ac3_mixdata[32];
  unsigned char ac3_paninfoe;
  unsigned short ac3_paninfo;
  unsigned char ac3_paninfo2e;
  unsigned short ac3_paninfo2;
  unsigned char ac3_frmmixcfginfoe;
  unsigned char ac3_blkmixcfginfo[6];
  unsigned char ac3_infomdate;
  unsigned char ac3_dheadphonmod;
  unsigned char ac3_blkmixcfginfoe;
  unsigned char ac3_dsurexmod;
  unsigned char ac3_adconvtyp;
  unsigned char ac3_adconvtyp2;
  unsigned char ac3_sourcefscod;
  unsigned char ac3_convsync;
  unsigned char ac3_blkid;
  unsigned char ac3_xbsi1e;
  unsigned char ac3_xbsi2e;
  unsigned char ac3_xbsi2;
  unsigned char ac3_encinfo;
  unsigned char reserved11_1;

  int reserved11[5898];

  unsigned char h264_dpb_size;
  unsigned char reserved12_1[3];

  // note on the audio stream_to_get fields, including the
  // audio_stream_to_get fields above
  //
  // the fields have the range 1 .. stream count, a value of 
  // 0 means do not get a stream of this type
  //
  // the fields are checked in this order
  //
  // audio_stream_to_get
  // pcm_stream_to_get
  // ac3_stream_to_get
  // aes3_stream_to_get
  // amr_stream_to_get
  //
  // if there are multiple audio stream types (for instance both an mpeg
  // and an ac3 audio stream are present) and the desired stream is the ac3
  // stream, set audio_stream_to_get = 0 and ac3_stream_to_get = 1

  int pcm_stream_to_get;
  int ac3_stream_to_get;
  int aes3_stream_to_get;	// includes HDMV LPCM!

  mpegin_stream_data video_streams[VIDEO_STREAM_MAX];
  mpegin_stream_data audio_streams[AUDIO_STREAM_MAX];
  mpegin_stream_data ac3_streams[AC3_STREAM_MAX];
  mpegin_stream_data aes3_streams[AUDIO_STREAM_MAX];
  mpegin_stream_data dts_streams[DTS_STREAM_MAX];
  mpegin_stream_data sdds_streams[SDDS_STREAM_MAX];
  mpegin_stream_data pcm_streams[PCM_STREAM_MAX];
  mpegin_stream_data subpic_streams[SUBPIC_STREAM_MAX];
  mpegin_stream_data amr_streams[AMR_STREAM_MAX];

  int amr_stream_to_get;
  int amr_stream_count;

  int reserved13[7602];

  char *ac3_serial_number;

  int dec_size;

  void  (*err_printf)(char *fmt, ...);
  void  (*prg_printf)(int percent, char *fmt, ...);
  void  (*wrn_printf)(char *fmt, ...);
  void  (*inf_printf)(char *fmt, ...);
  void *(*memAlloc)(uint32_t size);
  void  (*memFree)(void *memblk);
  void *(*get_rc)(char *name);

  mpegdec_tt *dec;
};
#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif


//------------------------------- start normal functions ---------------------------------------

// call to create and initialize an mpegInInfo instance
//
//  inputs:
//    get_rc - a pointer to a get_rc resource function
//    existingInfo - a pointer to a previously saved mpegInInfo structure
//
// outputs:
//    none
//
//  return:
//    a pointer to an mpegInInfo structure if successful, else NULL

mpegInInfo *mpegInNew(void *(*get_rc)(char* name), mpegInInfo *existingInfo);


// call to open an mpeg file. Filename can be NULL if inInfo is a pointer to a
// saved mpegInInfo structure from a previously opened mpeg file (see the API
// documentation for more details)
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    filename - a pointer to a filename to open
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInOpen(mpegInInfo *inInfo, char *filename);


// call to open an Unicode mpeg file. Filename can be NULL if inInfo is a pointer to a
// saved mpegInInfo structure from a previously opened mpeg file (see the API
// documentation for more details)
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    filename - a pointer to a unicode filename to open
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)

int mpegInOpenW(mpegInInfo *inInfo, wchar_t *filename);

#else

// this version will currently return an error!
int mpegInOpenW(mpegInInfo *inInfo, unsigned short *filename);

#endif

// call to close an mpeg file opened with mpegInOpen
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//
// outputs:
//    none
//
//  return:
//    none

void mpegInClose(mpegInInfo *inInfo);


// call to free the memory allocated during an mpegInNew call, if inInfo
// points to a structure that was allocated by the application for
// re-opening a file, the aplication must free the inInfo memory. This
// call will only free the memory if it was allocated by mpegInNew.
// See the API documentation for more details
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//
// outputs:
//    none
//
//  return:
//    none

void mpegInFree(mpegInInfo *inInfo);


// call to close all open file handles, this must be followed by a call to
// mpegInWakeFiles before any of the other functions can be used again.
// this call does not release any memory
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//
// outputs:
//    none
//
//  return:
//    none

void mpegInSleepFiles(mpegInInfo *inInfo);


// call to re-open all file handles
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInWakeFiles(mpegInInfo *inInfo);


// call to seek to and decode a particular video frame 
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    frameNum - a zero based frame number, the range is 0 .. totalFrames
//
// outputs:
//    decoded frame
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInSeekFrame(mpegInInfo *inInfo, int frameNum);


// call to decode the next video frame 
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//
// outputs:
//    decoded frame
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInGetNextFrame(mpegInInfo *inInfo);


// call to seek to and decode a particular number of audio samples  
// into the passed audio buffer. To just seek and not decode any  
// samples, call this function with buffer == NULL or numSamples == 
// 0 or both, this function returns the number of samples returned, 
// zero if only seeking was performed or -1 if an error occurs      
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    buffer - a pointer to a buffer to receive the decoded PCM audio data
//    startSample - a zero based starting sample number
//    numSamples - the number of samples to decode
//
// outputs:
//    decoded samples
//
//  return:
//    the number of samples returned, 0 if only seeking was performed or -1 if an error occurs      

int mpegInSeekSample64(mpegInInfo *inInfo, uint8_t *buffer, int64_t startSample, int numSamples);


// call to decode the next numSamples audio samples into the passed buffer 
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    buffer - a pointer to a buffer to receive the decoded PCM audio data
//    numSamples - the number of samples to decode
//
// outputs:
//    decoded samples
//
//  return:
//    the number of samples returned or -1 if an error occurs      

int mpegInGetNextSample(mpegInInfo *inInfo, uint8_t *buffer, int numSamples);


// this function just opens a file and finds the streams and types   
// it does not get the number of frames or samples and it closes the 
// file when finished                                                
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//
// outputs:
//    file and stream information
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInGetInfo(mpegInInfo *inInfo);


// open an mpeg file using existing index files
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    audio_idx_file - a pointer to a filename for the audio index data
//    video_idx_file - a pointer to a filename for the video index data
//    load_idx_to_mem - a flag indicating whether the index is loaded to memory
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInOpenMPEGIdx(mpegInInfo *inInfo,
					 char *audio_idx_file, char *video_idx_file,
					 int load_idx_to_mem);


// open an mpeg file using existing index files, Unicode version
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    audio_idx_file - a pointer to a Unicode filename for the audio index data
//    video_idx_file - a pointer to a Unicode filename for the video index data
//    load_idx_to_mem - a flag indicating whether the index is loaded to memory
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)

int mpegInOpenMPEGIdxW(mpegInInfo *inInfo,
					   wchar_t *audio_idx_file, wchar_t *video_idx_file,
					   int load_idx_to_mem);
#else

// this version will currently return an error!
int mpegInOpenMPEGIdxW(mpegInInfo *inInfo,
					   unsigned short *audio_idx_file, unsigned short *video_idx_file,
					   int load_idx_to_mem);
#endif

// call to save the index data to files
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    audio_idx_file - a pointer to a filename for the audio index data
//    video_idx_file - a pointer to a filename for the video index data
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

int mpegInSaveIdx(mpegInInfo *inInfo, char *audio_idx_file, char *video_idx_file);


// call to save the index data to files, Unicode version
//
//  inputs:
//    inInfo - a pointer to an mpegInInfo structure
//    audio_idx_file - a pointer to a Unicode filename for the audio index data
//    video_idx_file - a pointer to a Unicode filename for the video index data
//
// outputs:
//    none
//
//  return:
//    mpegInErrNone if successful, else mpegInError

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks) && !defined(__QNX__)

int mpegInSaveIdxW(mpegInInfo *inInfo, wchar_t *audio_idx_file, wchar_t *video_idx_file);

#else

// this version will currently return an error!
int mpegInSaveIdxW(mpegInInfo *inInfo, unsigned short *audio_idx_file, unsigned short *video_idx_file);

#endif


// direct access to the decoder's auxinfo functions
int mpegInVideoAuxinfo(mpegInInfo *inInfo, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);
int mpegInAudioAuxinfo(mpegInInfo *inInfo, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);
int mpegInSubpicAuxinfo(mpegInInfo *inInfo, uint32_t offs, uint32_t info_ID, void *info_ptr, uint32_t info_size);

//---------------------------------- end normal functions -----------------------------------------



//------------------------------ start internal use functions -------------------------------------

int mpegInOpenTSFile(mpegInInfo *inInfo, char *audio_idx_file, char *video_idx_file, int load_to_mem);
int mpegInOpenTSStream(mpegInInfo *inInfo, bufstream_tt *audio_idx, bufstream_tt *video_idx);
int mpegInInitTS(mpegInInfo *inInfo);
int mpegInSeekFrameTS(mpegInInfo *inInfo, int frameNum);
int mpegInGetNextFrameTS(mpegInInfo *inInfo);
int mpegInSeekSample64TS(mpegInInfo *inInfo, uint8_t *buffer, int64_t startSample, int numSamples, int *numReturned);
int mpegInGetNextSampleTS(mpegInInfo *inInfo, uint8_t *buffer, int numSamples, int *numReturned);
int mpegSplitOpenFileTSFile(mpegInInfo *inInfo, void* lpData, char *audio_idx_name, char *video_idx_name, int load_to_mem);
int mpegSplitOpenFileTSStream(mpegInInfo *inInfo, void* lpData, bufstream_tt *audio_idx, bufstream_tt *video_idx);
int mpegSplitInitTS(mpegInInfo *inInfo);
int mpegSplitSeekTimeTS(mpegInInfo *inInfo, int64_t timecode, int64_t *seek_timecode, int64_t *seek_filepos, int64_t *ref_timecode, int64_t *ref_filepos, void* lpData, int audioSearch);
int mpegSplitSeekByteTS(mpegInInfo *inInfo, int64_t position, int64_t *actual_timecode, void* lpData, int audioSearch);
int mpegSplitOpenFile(mpegInInfo *inInfo, void* lpData);
int mpegSplitGetInfo(mpegInInfo *inInfo, void* lpData);
void mpegSplitClose(mpegInInfo *inInfo);
int mpegSplitSeekTime(mpegInInfo *inInfo, int64_t timecode, int64_t *seek_timecode, int64_t *seek_filepos, int64_t *ref_timecode, int64_t *ref_filepos, void* lpData, int audioSearch);
int mpegSplitSeekByte(mpegInInfo *inInfo, int64_t position, int64_t *actual_timecode, void* lpData, int audioSearch);
int mpegInOpenPrimary(void *(*get_rc)(char* name), mpegInInfo *inInfo);
int mpegInOpenSecondary(mpegInInfo *inInfo, mpegInInfo *orgInfo);
void mpegInCloseSecondary(mpegInInfo *inInfo);
int mpegInGetNextCodedFrame(mpegInInfo *inInfo);

#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks) && !defined(__QNX__)

int mpegInOpenTSFileW(mpegInInfo *inInfo, wchar_t *audio_idx_file, wchar_t *video_idx_file, int load_to_mem);
int mpegSplitOpenFileTSFileW(mpegInInfo *inInfo, void* lpData, wchar_t *audio_idx_name, wchar_t *video_idx_name, int load_to_mem);

#else

// this version will currently return an error!
int mpegInOpenTSFileW(mpegInInfo *inInfo, unsigned short *audio_idx_file, unsigned short *video_idx_file, int load_to_mem);
int mpegSplitOpenFileTSFileW(mpegInInfo *inInfo, void* lpData, unsigned short *audio_idx_name, unsigned short *video_idx_name, int load_to_mem);

#endif

//------------------------------- end internal use functions ---------------------------------------



//------------------ start do not use functions, these are being removed!!!!! ----------------------
void mpegInInfoInit(mpegInInfo *inInfo);
int mpegInOpenMPEG(mpegInInfo *inInfo);
void mpegInCloseMPEG(mpegInInfo *inInfo);
int mpegInSeekSample(mpegInInfo *inInfo, uint8_t *buffer, uint32_t startSample, int numSamples);
char *mpegInGetLastErrString(void);
mpegInInfo *mpegInOpenExisting(void *(*get_rc)(char* name),int filePtr);
void mpegInUpdateFiles(mpegInInfo *inInfo,int filePtr);
void mpegInCloseExisting(mpegInInfo *inInfo);
int mpegInSeekFrameExisting(mpegInInfo *inInfo, int frameNum, int decodeMode);
int mpegInGetNextFrameExisting(mpegInInfo *inInfo, int decodeMode);
int mpegInSeekSampleExisting(mpegInInfo *inInfo, uint8_t *buffer, int64_t startSample, int numSamples);
int mpegInGetNextSampleExisting(mpegInInfo *inInfo, uint8_t *buffer, int numSamples);
int mpegInSeekSampleTS(mpegInInfo *inInfo, uint8_t *buffer, uint32_t startSample, int numSamples, int *numReturned);
//------------------- end do not use functions, these are being removed!!!!! -----------------------


#ifdef __cplusplus
}
#endif

#endif // _mpegin_h_
