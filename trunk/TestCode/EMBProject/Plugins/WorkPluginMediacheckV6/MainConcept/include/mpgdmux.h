//------------------------------------------------------------------------------
// File: mpgdmux.h
//
// Desc: mpeg demuxer header
//
// Copyright (c) 2000-2006, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _MPGDMUX_H_
#define _MPGDMUX_H_

#include "mctypes.h"
#include "mpegdef.h"
#include "bufstrm.hpp"

// find stream options
#define MPGDMUX_FIND_NONE      0
#define MPGDMUX_FIND_VIDEO     1   // stop looking after 1 video stream is found
#define MPGDMUX_FIND_AUDIO     2   // stop looking after 1 audio stream is found
#define MPGDMUX_FIND_VIDAUD    3   // stop looking after 1 video and 1 audio are found
#define MPGDMUX_FIND_SUBPIC    4   // stop looking after 1 subpic stream is found
#define MPGDMUX_FIND_VIDAUDSP  7   // stop looking after 1 video, 1 audio and 1 subpic are found
#define MPGDMUX_FIND_ALL       8   // do not stop looking until the MB limit is reached

// ISO/IEC 13818-1 stream_type defines
#define STREAM_TYPE_MPEG1_VIDEO			0x01	// ISO/IEC 11172-2 Video
#define STREAM_TYPE_MPEG2_CP_VIDEO		0x02	// ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream
#define STREAM_TYPE_MPEG1_AUDIO			0x03	// ISO/IEC 11172-3 Audio
#define STREAM_TYPE_MPEG2_AUDIO			0x04	// ISO/IEC 13818-3 Audio
#define STREAM_TYPE_PRIVATE_SECTIONS	0x05	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections
#define STREAM_TYPE_PRIVATE_DATA		0x06	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data
#define STREAM_TYPE_MHEG				0x07	// ISO/IEC 13522 MHEG
#define STREAM_TYPE_DSM_CC				0x08	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM CC
#define STREAM_TYPE_H2221				0x09	// ITU-T Rec. H.222.1
#define	STREAM_TYPE_TYPEA				0x0A	// ISO/IEC 13818-6 type A
#define STREAM_TYPE_TYPEB				0x0B	// ISO/IEC 13818-6 type B
#define STREAM_TYPE_TYPEC				0x0C	// ISO/IEC 13818-6 type C
#define STREAM_TYPE_TYPED				0x0D	// ISO/IEC 13818-6 type D
#define STREAM_TYPE_AUX					0x0E	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary
#define STREAM_TYPE_MPEG2_AAC			0x0F	// ISO/IEC 13818-7 Audio with ADTS transport syntax
#define STREAM_TYPE_MPEG4_VISUAL		0x10	// ISO/IEC 14496-2 Visual
#define STREAM_TYPE_MPEG4_AAC			0x11	// ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1
#define STREAM_TYPE_FLEXMUX_PES			0x12	// ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets
#define STREAM_TYPE_FLEXMUX_SECTIONS	0x13	// ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.
#define STREAM_TYPE_SDP					0x14	// ISO/IEC 13818-6 Synchronized Download Protocol
#define STREAM_TYPE_METADATA_PES		0x15	// Metadata carried in PES packets
#define STREAM_TYPE_METADATA_SECTIONS	0x16	// Metadata carried in metadata_sections
#define STREAM_TYPE_DATA_CAROUSEL		0x17	// Metadata carried in ISO/IEC 13818-6 Data Carousel
#define STREAM_TYPE_OBJECT_CAROUSEL		0x18	// Metadata carried in ISO/IEC 13818-6 Object Carousel
#define STREAM_TYPE_METADATA_SDP		0x19	// Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol
#define STREAM_TYPE_MPEG2_IPMP			0x1A	// IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)
#define STREAM_TYPE_AVC					0x1B	// AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video
#define STREAM_TYPE_IPMP				0x7F	// IPMP stream
#define STREAM_TYPE_AC3					0x81	// AC3 for some streams

// sdk_stream_type defines, general stream types reported
#define MPGDMUX_UNKNOWN					0x00	// unknown type to this SDK
#define MPGDMUX_MPEG_VIDEO				0x01	// MPEG or H.264 video
#define MPGDMUX_MPEG_AUDIO				0x02	// MPEG audio including AAC
#define MPGDMUX_AC3_AUDIO				0x03	// AC3 audio
#define MPGDMUX_AES_302M_AUDIO			0x05	// AES 302M PCM audio
#define MPGDMUX_DVD_LPCM_AUDIO			0x04	// LPCM audio, DVD style
#define MPGDMUX_DTS_AUDIO				0x06	// DTS audio
#define MPGDMUX_SDDS_AUDIO				0x07	// SDDS audio
#define MPGDMUX_DVD_SUBPIC				0x08	// DVD style subpic
#define MPGDMUX_HDMV_LPCM_AUDIO			0x09	// HDMV LPCM audio
#define MPGDMUX_DVD_PCI					0x0A	// DVD PCI
#define MPGDMUX_DVD_DSI					0x0B	// DVD DSI
#define MPGDMUX_EAC3_AUDIO				0x0C	// Enhanced AC3 audio

// maximum number of programs handled by the demuxer
#define MPGDMUX_MAX_PROGRAMS			32
// maximum number of streams per program handled by the demuxer
#define MPGDMUX_MAX_STREAMS				64

// file info flags
#define MPGDMUX_FILE_AC3_TIMECODE		0x01

// index info flags
#define MPGDMUX_SEQ_FLAG				0x01
#define MPGDMUX_GOP_FLAG				0x02
#define MPGDMUX_IDR_FLAG				0x04

// called to seek in the source file in splitter mode
// position - file position to seek to
// return 0 if OK, 1 if fault
typedef int (*mpg_splitter_seek_callback)(void* split_app_ptr, int64_t position);

// called to read data in splitter file mode
// read bufferSize bytes from file into buffer
// return number of bytes read, 0 if fault
typedef int (*mpg_splitter_read_callback)(void* split_app_ptr, uint8_t *buffer, int bufferSize);


// external i/o function prototypes
// note: if these are used, the mpgdmux_settings.file_length field MUST be filled in
//       with the correct file size by the app
// called to open a file
// and the return value will be passed back to the seek, read and close functions
// ext_app_ptr - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileName - if a filename was passed to mpgdmux, it is returned here
// return the file handle as a long or -1 if an error occurs
typedef long (*mpg_external_open_callback)(void *ext_app_ptr, char *fileName);
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)
typedef long (*mpg_external_open_callbackW)(void *ext_app_ptr, wchar_t *fileName);
#else
typedef long (*mpg_external_open_callbackW)(void *ext_app_ptr, unsigned short *fileName);
#endif

// called to seek in a file
// ext_app_ptr - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileHandle - a file handle returned by the fn_external_open_callback function
// position - the position to seek to
// return 0 if Ok, 1 if an error occurs
typedef int (*mpg_external_seek_callback)(void *ext_app_ptr, long fileHandle, int64_t position);

// called to read from a file
// appData - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileHandle - a file handle returned by the fn_external_open_callback function
// buffer - a buffer for the data
// bufferSize - the number of bytes to read
// return the number of bytes actually read or 0 if an error occurs
typedef int (*mpg_external_read_callback)(void *ext_app_ptr, long fileHandle, uint8_t *buffer, int bufferSize);

// called to close a file
// appData - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileHandle - a file handle returned by the fn_external_open_callback function
typedef void (*mpg_external_close_callback)(void *ext_app_ptr, long fileHandle);


typedef void (*mpg_dvd_PCI_callback)(void *parser_app_ptr, void *buf, int buf_len);

typedef void (*mpg_dvd_DSI_callback)(void *parser_app_ptr, void *buf, int buf_len);


// demuxer instance
typedef struct mpeg_demuxer mpgdmux_tt;


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct mpgdmux_settings
{
	int dvd_get_vts_menu;
	int dvd_title_num;
	int dvd_language;
	int dvd_angle;
	int find_stream_limit;
	int find_stream_options;

	// splitter usage
	int splitter_mode;
	mpg_splitter_seek_callback splitter_seek_callback;
	mpg_splitter_read_callback splitter_read_callback;
	void *split_app_ptr;

	// external IO stuff
	int use_external_io;
	mpg_external_open_callback external_open_callback;
	mpg_external_open_callbackW external_open_callbackW;
	mpg_external_seek_callback external_seek_callback;
	mpg_external_read_callback external_read_callback;
	mpg_external_close_callback external_close_callback;
	void *ext_app_ptr;

	int64_t file_length;

	unsigned char reserved[64];
};


// file mode structures

struct mpgdmux_parser_settings
{
	int requested_buffer_size;

	mpg_dvd_PCI_callback dvd_PCI_callback;
	mpg_dvd_DSI_callback dvd_DSI_callback;
	void *parser_app_ptr;

	unsigned char reserved[64];
};


struct mpgdmux_stream_info
{
	int stream_type;					// stream_type from PMT or PSM
	int sdk_stream_type;				// stream type using SDK defines
	int elementary_PID;					// transport streams only
	unsigned char ID;					// elementary stream ID, program/PVA stream only
	unsigned char sub_ID;				// sub stream ID, DVD only
	int ES_info_length;					// length of the descriptors below
	unsigned char *descriptors;			// elementary stream descriptors present in PMT or PSM

	unsigned char PES_scrambled;		// the value of the PES_scrambling_control field of the
										// PES header, MPEG-2 transport/program streams only
										// a non-zero value indicates the PES packet is scrambled (encrypted)

	unsigned char ts_scrambled;			// the value of the transport_scrambling_control field
										// of the transport packet, transport streams only
										// a non-zero value indicates the transport payload
										// is scrambled (encrypted)
        unsigned char mpeg1_flag;           // = 1 for MPEG-1 video streams
	unsigned char reserved[61];
};


struct mpgdmux_program_info
{
	int program_number;					// program number
	int PID;							// network PID if program_number is 0, else program map PID, transport stream only
	int PCR_PID;						// transport stream only
	int stream_count;					// number of elementary streams in this program


	unsigned char section_syntax_indicator;	// from the PMT
	unsigned char private_indicator;		// from the PMT for program number 0

	int program_info_length;			// length of the descriptors below
	unsigned char *descriptors;			// program descriptors present in PMT or PSM

	unsigned char reserved[64];
};


struct mpgdmux_file_info
{
	int program_count;					// 1 for program, PVA or elementary streams
	int system_stream_flag;				// one of the STREAM_xxx defines in mpegdef.h
	int dvd_num_titles;					// number of titles in a VTS, only used for .IFO files
	int dvd_num_angles;					// number of angles in a VTS, only used for .IFO files
	int64_t file_size;					// total size of the input file(s)

	int transport_stream_id;			// transport_stream_id from the PAT

	// currently not supported
	int tsdt_info_length;				// length of the descriptors below
	unsigned char *tsdt_descriptors;	// transport stream descriptors present in TSDT

	// currently not supported
	int ca_info_length;					// length of the descriptors below
	unsigned char *ca_descriptors;		// conditional access descriptors

	int ts_packet_size;					// size of transport stream packets, usually 188
	unsigned int flags;					// one of the MPGDMUX_FILE_* flags

	unsigned char reserved[56];
};


struct mpgdmux_index_info
{
	int64_t file_pos;					// index file position
	int64_t PTS;						// index PTS
	int64_t DTS;						// index DTS
	uint8_t pic_type;					// picture type (mpeg and h264 video only)
	int16_t temp_ref;					// temporal reference (mpeg and h264 video only)
	uint8_t flags;						// flags
};


struct mpgdmux_stream_settings
{
	int program_idx;					// the program idx of the desired stream (not the same as the program number!)
	int stream_idx;						// the stream idx of the desired stream
	bufstream_tt *bs;					// the output bufstream for the desired stream

	unsigned char reserved[64];
};


// streaming mode structures

struct mpgdmux_sm_init_settings
{
	int stream_type;					// the type of the input stream, one of the STREAM_xxx defines in mpegdef.h
										// if this is set to -1, the input stream must start with the appropriate
										// start code so the demuxer can detect the stream type immediately
	bufstream_tt *input_bs;				// the input bufstream to the demuxer

	unsigned char reserved[68];
};


struct mpgdmux_sm_stream_settings
{
	int PID;							// the PID of the desired stream, transport and PVA streams only
	int ID;								// the ID of the desired stream, program/elementary streams only
	int sub_ID;							// the DVD sub ID of the desired stream, only used if
										// ID = 0xBD (private stream 1)
	bufstream_tt *bs;					// the output bufstream for the desired stream

	unsigned char reserved[64];
};


#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif


// ------------------------------------  general functions -----------------------------------------

	// call to create and initialize an demuxer instance
//
//  inputs:
//    get_rc_ex - a pointer to a get_rc_ex resource function
//    rc_app_ptr - an application supplied pointer that will be passed back with any get_rc calls
//    set - a pointer to an mpgdmux_settings structure
//
// outputs:
//    none
//
//  return:
//    a pointer to a demuxer instance if successful, else NULL

mpgdmux_tt *mpegDemuxNew(void *(*get_rc_ex)(void *rc_app_ptr, char* name), void *rc_app_ptr, mpgdmux_settings *set);


// call to free a demuxer instance
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//
// outputs:
//    none
//
//  return:
//    none

void mpegDemuxFree(mpgdmux_tt *demuxer);


// ---------------------------------  file mode functions -----------------------------------------

// call to open a file for demuxing
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    reserved - reserved
//    filename - a pointer to a filename to open
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxOpen(mpgdmux_tt *demuxer, void *reserved, char *filename);

// unicode version
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks) && !defined(__QNX__)

int mpegDemuxOpenW(mpgdmux_tt *demuxer, void *reserved, wchar_t *filename);

#else

// this version will currently return an error!
int mpegDemuxOpenW(mpgdmux_tt *demuxer, void *reserved, unsigned short *filename);

#endif


// call to close file
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//
// outputs:
//    none
//
//  return:
//    none

void mpegDemuxClose(mpgdmux_tt *demuxer);


// call to get information about the input file
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    file_info - a pointer to a file_info_struct
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxGetFileInfo(mpgdmux_tt *demuxer, mpgdmux_file_info *file_info);


// call to get information about the specified program
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    program_info - a pointer to a program_info_struct
//    program_idx - a zero-based program index number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxGetProgramInfo(mpgdmux_tt *demuxer, mpgdmux_program_info *program_info, int program_idx);


// call to get information about the specified stream in the specified program
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    stream_info - a pointer to a stream_info_struct
//    program_idx - a zero-based program index number
//    stream_idx - a zero-based stream index number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxGetStreamInfo(mpgdmux_tt *demuxer, mpgdmux_stream_info *stream_info, int program_idx, int stream_idx);


// call to create and initialize a parser instance
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    set - a pointer to a mpgdmux_parser_settings structure
//
// outputs:
//    none
//
//  return:
//    a non-zero parser number if successful, else 0

int mpegDemuxNewParser(mpgdmux_tt *demuxer, mpgdmux_parser_settings *set);


// call to free a parser instance
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    none

void mpegDemuxFreeParser(mpgdmux_tt *demuxer, int parser_num);


// call to add an output bufstream for the specified stream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    set - a pointer to an mpgdmux_stream_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxAddStream(mpgdmux_tt *demuxer, int parser_num, mpgdmux_stream_settings *set);


// call to remove an output bufstream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    set - a pointer to an mpgdmux_stream_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxRemoveStream(mpgdmux_tt *demuxer, int parser_num, mpgdmux_stream_settings *set);


// call to seek to the specified file position
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    file_pos - the position to seek to
//
// outputs:
//    none
//
//  return:
//    0 if successful, 2 if EOF, 1 if an error occurs

int mpegDemuxSeek(mpgdmux_tt *demuxer, int parser_num, int64_t file_pos);


// call to push a packet through the parser
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    non-zero if successful, the number of bytes consumed
//	  0 if EOF
//    -1 if an error occurs

int mpegDemuxPush(mpgdmux_tt *demuxer, int parser_num);


// call to re-open a parser
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxWakeParser(mpgdmux_tt *demuxer, int parser_num);


// call to close the actual file of a parser, the parser
// is left intact and can be re-opened with the wake function
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxSleepParser(mpgdmux_tt *demuxer, int parser_num);


// call to have the file size updated if it is getting bigger
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    file_size - the new file size if splitter or external i/o mode, else unused
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxUpdateFilesize(mpgdmux_tt *demuxer, int parser_num, int64_t file_size);


// call to have the parser perform a seek before the next read
// and optionally update the internal app_data ptr
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    split_app_ptr - can be used to updated the split_app_ptr from the mpgdmux_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxSetFirstSplitRead(mpgdmux_tt *demuxer, int parser_num, void *split_app_ptr);



// ------------------------------  streaming mode functions -----------------------------------------

// call to initialize for streaming demuxing
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    set - a pointer to a mpgdmux_sm_init_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxStreamModeInit(mpgdmux_tt *demuxer, mpgdmux_sm_init_settings *set);


// call to add a stream for streaming demuxing
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    set - a pointer to a mpgdmux_sm_stream_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxStreamModeAddStream(mpgdmux_tt *demuxer, mpgdmux_sm_stream_settings *set);


// call to finish a streaming demuxing session
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    abort - set to 1 to abort demuxing or 0 to finish normally
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxStreamModeDone(mpgdmux_tt *demuxer, int abort);


// call to create an index of the input stream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//	  get_pic_info - 1 = get picture type and temp ref (mpeg and h264 video only)
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxCreateIndex(mpgdmux_tt *demuxer, int get_pic_info);


// call to get the number of index entries for a stream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//	  program_idx - program index
//	  stream_idx - stream index
//
// outputs:
//    none
//
//  return:
//    number of entries if successful, else zero

int mpegDemuxGetIndexCount(mpgdmux_tt *demuxer, int program_idx, int stream_idx);


// call to get an index entry for a stream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//	  program_idx - program index
//	  stream_idx - stream index
//	  info - a pointer to a mpgdmux_index_info to receive the index info
//	  idx - index entry to get
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mpegDemuxGetIndexEntry(mpgdmux_tt *demuxer, int program_idx, int stream_idx, mpgdmux_index_info *info, unsigned int idx);


// call to free a created index
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//
// outputs:
//    none
//
//  return:
//    none

void mpegDemuxFreeIndex(mpgdmux_tt *demuxer);


#ifdef __cplusplus
}
#endif

#endif // _mpgdmux_h_
